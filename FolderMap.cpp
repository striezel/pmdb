/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2014  Thoronador

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

#include "FolderMap.hpp"
#include <iostream>
#include <stdexcept>
#include "libthoro/common/StringUtils.h"

FolderMap::FolderMap()
: m_FolderMap(std::map<SHA256::MessageDigest, std::string>())
{
}

void FolderMap::add(const SHA256::MessageDigest& pm_digest, const std::string& folder)
{
  m_FolderMap[pm_digest] = folder;
}

bool FolderMap::hasEntry(const SHA256::MessageDigest& pm_digest) const
{
  return (m_FolderMap.find(pm_digest)!=m_FolderMap.end());
}

const std::string& FolderMap::getFolderName(const SHA256::MessageDigest& pm_digest) const
{
  const std::map<SHA256::MessageDigest, std::string>::const_iterator iter = m_FolderMap.find(pm_digest);
  if (iter!=m_FolderMap.end()) return iter->second;
  throw std::runtime_error("The message database's folder map has no message entry for the given hash "+pm_digest.toHexString()+"!");
}

bool FolderMap::save(const std::string& directory) const
{
  std::ofstream output;
  output.open((directory+"foldermap").c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
  if (!output)
  {
    return false;
  }
  const char space = ' ';
  const char end = '\n';
  std::map<SHA256::MessageDigest, std::string>::const_iterator iter = m_FolderMap.begin();
  while (iter!=m_FolderMap.end())
  {
    const std::string hexRepresentation = iter->first.toHexString();
    //write hash
    output.write(hexRepresentation.c_str(), hexRepresentation.length());
    //write space
    output.write(&space, 1);
    //write folder name
    output.write(iter->second.c_str(), iter->second.length());
    //write end of line character
    output.write(&end, 1);
    ++iter;
  }//while
  const bool well = output.good();
  output.close();
  return well;
}

bool FolderMap::load(const std::string& directory)
{
  std::ifstream inFile;
  inFile.open((directory+"foldermap").c_str(), std::ios_base::in | std::ios_base::binary);
  if (!inFile)
  {
    std::cout << "Error: could not open folder map in \"" << directory << "\"!\n";
    return false;
  }
  SHA256::MessageDigest md;
  const unsigned int cMaxLine = 1024;
  char buffer[cMaxLine];
  std::string line = "";
  while (inFile.getline(buffer, cMaxLine-1))
  {
    buffer[cMaxLine-1] = '\0';
    line = std::string(buffer);
    if (!md.fromHexString(line.substr(0, 64)))
    {
      inFile.close();
      std::cout << "Error: string \""<<line.substr(0,64)<<"\" is no valid hash!\n";
      return false;
    }
    //remove hash
    line.erase(0, 64);
    //remove leading spaces
    trimLeft(line);
    if (!line.empty())
    {
      m_FolderMap[md] = line;
    }//if
  }//while
  inFile.close();
  return true;
}

std::set<std::string> FolderMap::getPresentFolders() const
{
  std::set<std::string> allFolders;
  std::map<SHA256::MessageDigest, std::string>::const_iterator iter = m_FolderMap.begin();
  while (iter!=m_FolderMap.end())
  {
    allFolders.insert(iter->second);
    ++iter;
  }//while
  return allFolders;
}
