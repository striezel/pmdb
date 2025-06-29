/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database test suite.
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

#include "functions.hpp"
#include <fstream>
#include <iostream>
#include "../../libstriezel/filesystem/directory.hpp"
#include "../../libstriezel/filesystem/file.hpp"
#include "../paths.hpp"
#include "../ReturnCodes.hpp"
#include "defaults.hpp"

namespace pmdb::tpl
{

int ensureTemplateFile(const std::string& file_path, const std::string_view default_content)
{
  if (!libstriezel::filesystem::file::exists(file_path))
  {
    std::ofstream stream(file_path, std::ios::out | std::ios::binary);
    if (!stream)
    {
      std::cerr << "Error: Could not create template file " << file_path << "!\n";
      return rcFileError;
    }
    stream.write(default_content.data(), default_content.size());
    stream.close();
    if (!stream.good())
    {
      std::cerr << "Error: Could not write template file " << file_path << "!\n";
      return rcFileError;
    }
  }

  // File already existed or was created successfully.
  return 0;
}

int ensureFilesExist()
{
  const auto directory = pmdb::paths::templates();
  if (!libstriezel::filesystem::directory::exists(directory))
  {
    if (!libstriezel::filesystem::directory::createRecursive(directory))
    {
      std::cerr << "Error: Could not create missing template directory "
                << directory << ".\n";
      return rcFileError;
    }
  }

  const auto tpl_directory = libstriezel::filesystem::slashify(directory);

  int rc = ensureTemplateFile(tpl_directory + "message.tpl", pmdb::tpl::defaults::message);
  if (rc != 0)
  {
    return rc;
  }
  rc = ensureTemplateFile(tpl_directory + "folder.tpl", pmdb::tpl::defaults::folder);
  if (rc != 0)
  {
    return rc;
  }
  rc = ensureTemplateFile(tpl_directory + "index_entry.tpl", pmdb::tpl::defaults::index_entry);
  if (rc != 0)
  {
    return rc;
  }
  rc = ensureTemplateFile(tpl_directory + "folder_entry.tpl", pmdb::tpl::defaults::folder_entry);
  if (rc != 0)
  {
    return rc;
  }
  return ensureTemplateFile(tpl_directory + "folder_list.tpl", pmdb::tpl::defaults::folder_list);
}

} // namespace
