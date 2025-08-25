/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2025  Dirk Stolle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "browser_detection.hpp"
#ifdef _WIN32
#include <algorithm>
#include <windows.h>
#endif

#ifdef _WIN32
std::vector<std::string> listSubKeyNames()
{
  std::vector<std::string> result;
  HKEY startMenuInternet;
  LONG res = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clients\\StartMenuInternet", 0, KEY_READ, &startMenuInternet);
  if (res != ERROR_SUCCESS)
  {
    // Failed to open key, maybe due to permissions.
    return {};
  }
  for (DWORD idx = 0; ; ++idx)
  {
    constexpr DWORD buffer_size = 256;
    char buffer[buffer_size];
    DWORD name_length = buffer_size;
    res = RegEnumKeyExA(startMenuInternet, idx, &buffer[0], &name_length, nullptr, nullptr, nullptr, nullptr);
    if (res == ERROR_NO_MORE_ITEMS)
    {
      // reached last sub key
      RegCloseKey(startMenuInternet);
      break;
    }
    if (res != ERROR_SUCCESS)
    {
      // Failed to query key, maybe due to permissions?
      RegCloseKey(startMenuInternet);
      return {};
    }
    const DWORD nul_pos = name_length >= buffer_size ? buffer_size - 1 : name_length;
    buffer[nul_pos] = '\0';
    result.emplace_back(std::string(buffer));
  }
  return result;
}

std::vector<std::filesystem::path> getBrowserPaths()
{
  std::vector<std::filesystem::path> result;

  const auto subKeyNames = listSubKeyNames();
  for (const auto& subKeyName: subKeyNames)
  {
    const auto keyName = "SOFTWARE\\Clients\\StartMenuInternet\\" + subKeyName + "\\shell\\open\\command";
    HKEY commandKey;
    LONG res = RegOpenKeyExA(HKEY_LOCAL_MACHINE, keyName.c_str(), 0, KEY_READ, &commandKey);
    if (res != ERROR_SUCCESS)
    {
      // Failed to open command key, maybe due to permissions.
      continue;
    }
    constexpr DWORD value_buffer_size = 1024;
    unsigned char value_buffer[value_buffer_size];
    DWORD valueType = 0;
    DWORD valueSize = value_buffer_size - 1;
    res = RegQueryValueExA(commandKey, "", nullptr, &valueType, value_buffer, &valueSize);
    if (res != ERROR_SUCCESS)
    {
      // Failed to query key, maybe due to permissions?
      RegCloseKey(commandKey);
      continue;
    }
    if (valueType != REG_SZ)
    {
      // Wrong value type, only strings allowed.
      RegCloseKey(commandKey);
      continue;
    }
    if ((valueSize == 0) || (valueSize >= value_buffer_size))
    {
      // Invalid size.
      RegCloseKey(commandKey);
      continue;
    }
    // Value may or may not be NUL-terminated, so ensure the NUL byte is there.
    if (value_buffer[valueSize - 1] != 0)
    {
      value_buffer[valueSize] = 0;
    }
    auto potential_path = std::string(reinterpret_cast<char*>(value_buffer));
    RegCloseKey(commandKey);
    const auto size = potential_path.size();
    if ((potential_path[0] == '"') && (potential_path[size-1] == '"'))
    {
      potential_path.erase(size - 1, 1).erase(0, 1);
    }
    std::error_code error;
    std::filesystem::path path(potential_path);
    if (std::filesystem::is_regular_file(path, error) && !error)
    {
      result.push_back(path);
    }
  } // for

  return result;
}

std::optional<Browser> getBrowserType(const std::filesystem::path& path)
{
  const auto basename = path.filename().string();
  if ((basename == "firefox.exe") || (basename == "librewolf.exe"))
  {
    return Browser::Firefox;
  }
  if (basename == "chrome.exe")
  {
    return Browser::Chromium;
  }
  if (basename == "seamonkey.exe")
  {
    return Browser::SeaMonkey;
  }
  if (basename == "msedge.exe")
  {
    return Browser::Edge;
  }

  // Unknown browser type.
  return std::nullopt;
}
#endif // _WIN32

std::optional<BrowserInformation> detect_browser()
{
  #ifdef _WIN32
  const auto paths = getBrowserPaths();
  std::vector <BrowserInformation> browsers;
  for (const auto& path: paths)
  {
    const auto type = getBrowserType(path);
    if (!type.has_value())
    {
      continue;
    }
    browsers.push_back({path, type.value()});
  } // for

  if (browsers.empty())
  {
    return std::nullopt;
  }

  // Sort to make sure we have stuff like Edge only as last resort.
  std::stable_sort(browsers.begin(), browsers.end(),
                   [](const BrowserInformation& left, const BrowserInformation& right) {
                        return left.type < right.type;
                   });

  return browsers.at(0);
  #else
  // On Linux systems the paths are usually similar, depending on how the
  // browser was packaged. So it's easiest to just test some known paths.
  const std::vector<BrowserInformation> known_paths = {
    { "/usr/bin/firefox", Browser::Firefox },
    { "/usr/bin/firefox-esr", Browser::Firefox },
    { "/usr/bin/firefox-devedition", Browser::Firefox },
    { "/usr/bin/chromium", Browser::Chromium },
    { "/usr/bin/chromium-browser", Browser::Chromium },
  };

  for (const auto& element: known_paths)
  {
    std::error_code error;
    if (std::filesystem::is_regular_file(element.path, error) && !error)
    {
      return element;
    }
  }

  // No known browser found.
  return std::optional<BrowserInformation>();
  #endif
}

std::vector<std::string> additional_parameters(const Browser type)
{
  switch(type)
  {
    case Browser::Firefox:
         return { "--private-window" };
    case Browser::Chromium:
         return { "--incognito" };
    case Browser::SeaMonkey:
         return { "-private" };
    case Browser::Edge:
         return { "--inprivate" };
    default:
         return {};
  }
}
