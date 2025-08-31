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

#include "open_file.hpp"
#include <iostream>
#include <boost/version.hpp>
#if (BOOST_VERSION <= 108100) || defined(USE_BOOST_PROCESS_V1)
  #include <boost/process.hpp>
#else
  #include <boost/asio/io_context.hpp>
  #include <boost/process/v2/process.hpp>
  #if defined(_WIN32)
    #include <boost/process/v2/windows/default_launcher.hpp>
  #else
    #include <boost/process/v2/posix/fork_and_forget_launcher.hpp>
  #endif
#endif
#include "browser_detection.hpp"
#include "../libstriezel/filesystem/directory.hpp"
#include "../libstriezel/hash/sha256/BufferSourceUtility.hpp"

std::string getFirstFolderFileName(const FolderMap& fm)
{
  const auto folders = fm.getPresentFolders();
  if (folders.empty())
  {
    return "";
  }
  const std::string& name = *folders.begin();
  return "folder_"
    + SHA256::computeFromBuffer((uint8_t*) name.c_str(), name.length() * 8).toHexString()
    + ".html";
}

void openFirstIndexFile(const FolderMap& fm, const std::string& html_dir)
{
  const auto fileName = getFirstFolderFileName(fm);
  if (fileName.empty())
  {
    return;
  }
  const auto fullFileName = libstriezel::filesystem::slashify(html_dir) + fileName;
  const auto browser = detect_browser();
  if (!browser.has_value())
  {
    std::cout << "Info: Could not find a browser to open generated files.\n"
              << "Info: Open " << fullFileName << " in a browser to see the generated HTML files.\n";
    return;
  }
  // Open file via Boost Process.
  std::cout << "Opening " << fullFileName << " in browser ...\n";
  std::vector<std::string> params = additional_parameters(browser.value().type);
  #if (BOOST_VERSION <= 108100) || defined(USE_BOOST_PROCESS_V1)
  std::string command = browser.value().path.string();
  for (const auto& param: params)
  {
    command += " " + param;
  }
  command += " " + fullFileName;
  boost::process::child child(command);
  child.detach();
  #else
  // After Boost 1.81.0 use process v2 API.
  boost::asio::io_context context;
  params.push_back(fullFileName);
  #if defined(_WIN32)
  auto launcher = boost::process::v2::windows::default_launcher();
  #else
  auto launcher = boost::process::v2::posix::fork_and_forget_launcher();
  #endif
  boost::process::v2::process proc(context, browser.value().path.string(), params);
  proc.detach();
  #endif
}
