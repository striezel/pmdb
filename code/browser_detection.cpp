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

std::optional<BrowserInformation> detect_browser()
{
  #ifdef _WIN32
  // TODO: Not implemented yet.
  return std::optional<BrowserInformation>();
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
    default:
         return {};
  }
}
