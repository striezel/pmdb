/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014, 2025  Dirk Stolle

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

#include "Config.hpp"
#include <fstream>
#include <iostream>

Config::Config()
: forumURL(""),
  tplFile("")
  #ifndef NO_SMILIES_IN_PARSER
  , smilies(std::vector<Smilie>())
  #endif // NO_SMILIES_IN_PARSER,
{
}

const std::string& Config::getForumURL() const
{
  return forumURL;
}

void Config::setForumURL(const std::string& newURL)
{
  if (!newURL.empty())
    forumURL = newURL;
}

const std::string& Config::getTPL() const
{
  return tplFile;
}

void Config::setTPLFile(const std::string& newTPL)
{
  if (!newTPL.empty())
    tplFile = newTPL;
}

#ifndef NO_SMILIES_IN_PARSER
const std::vector<Smilie>& Config::getSmilies() const
{
  return smilies;
}
#endif

void Config::clear()
{
  forumURL.clear();
  tplFile.clear();
  #ifndef NO_SMILIES_IN_PARSER
  smilies.clear();
  #endif
}

bool Config::loadFromFile(const std::string& fileName)
{
  std::ifstream input;
  input.open(fileName, std::ios::in | std::ios::binary);
  if (!input)
  {
    return false;
  }

  // clear existing values
  clear();

  const unsigned int cMaxLine = 1024;
  char buffer[cMaxLine];
  std::string line = "";
  std::string::size_type sep_pos = 0;
  while (input.getline(buffer, cMaxLine-1))
  {
    buffer[cMaxLine-1] = '\0';
    line = std::string(buffer);
    // check for possible carriage return at end (happens on Windows systems)
    if (!line.empty())
    {
      if (line.at(line.length() - 1) == '\r')
      {
        line.erase(line.length() - 1);
      }
    }

    if (!line.empty())
    {
      sep_pos = line.find('=');
      if (sep_pos == std::string::npos || sep_pos == 0)
      {
        std::cout << "Config::loadFromFile: ERROR: Invalid line found: \""
                  << line << "\".\nGeneral format: \"Name of Setting=value\"\n"
                  << "Loading from file cancelled.\n";
        input.close();
        return false;
      }

      const std::string name = line.substr(0, sep_pos);
      if (name == "forum")
      {
        forumURL = line.substr(sep_pos+1);
      }
      else if (name == "template")
      {
        tplFile = line.substr(sep_pos+1);
      }
      #ifndef NO_SMILIES_IN_PARSER
      else if ((name == "smilie") || (name == "smilie_r"))
      {
        line = line.substr(sep_pos + 1);
        sep_pos = line.find('=');
        if (sep_pos == std::string::npos || sep_pos == 0)
        {
          std::cout << "Config::loadFromFile: ERROR: Invalid smilie specification!\n";
          input.close();
          return false;
        }
        const std::string code = line.substr(0, sep_pos);
        const std::string s_url = line.substr(sep_pos+1);
        if (s_url.empty())
        {
          std::cout << "Config::loadFromFile: ERROR: Invalid smilie specification!\n";
          input.close();
          return false;
        }
        smilies.push_back(Smilie(code, s_url, (name == "smilie_r" ? UrlType::Relative : UrlType::Absolute)));
      } // smilie
      #endif
    }
  }
  input.close();
  return true;
}
