/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012  Thoronador

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

#include "Config.h"
#include <fstream>
#include <iostream>

bool loadConfigFile(const std::string& fileName, BBCodeParser& parser, std::string& forumURL, std::string& tplFile)
{
  std::ifstream input;
  input.open(fileName.c_str(), std::ios::in | std::ios::binary);
  if (!input)
  {
    return false;
  }
  const unsigned int cMaxLine = 1024;
  char buffer[cMaxLine];
  std::string line = "";
  std::string::size_type sep_pos = 0;
  while (input.getline(buffer, cMaxLine-1))
  {
    buffer[cMaxLine-1] = '\0';
    line = std::string(buffer);
    //check for possible carriage return at end (happens on Windows systems)
    if (!line.empty())
    {
      if (line.at(line.length()-1)=='\r')
      {
        line.erase(line.length()-1);
      }//if
    }

    if (!line.empty())
    {
      sep_pos = line.find('=');
      if (sep_pos == std::string::npos || sep_pos == 0)
      {
        std::cout << "loadConfigFile: ERROR: Invalid line found: \""
                  << line <<"\".\nGeneral format: \"Name of Setting=value\"\n"
                  << "Loading from file cancelled.\n";
        input.close();
        return false;
      }

      const std::string name = line.substr(0, sep_pos);
      if (name=="forum")
      {
        forumURL = line.substr(sep_pos+1);
      }
      else if (name=="template")
      {
        tplFile = line.substr(sep_pos+1);
      }
      else if ((name=="smilie") or (name=="smilie_r"))
      {
        line = line.substr(sep_pos+1);
        sep_pos = line.find('=');
        if (sep_pos == std::string::npos || sep_pos == 0)
        {
          std::cout << "loadConfigFile: ERROR: Invalid smilie specification!\n";
          input.close();
          return false;
        }
        const std::string code = line.substr(0, sep_pos);
        const std::string s_url = line.substr(sep_pos+1);
        if (s_url.empty())
        {
          std::cout << "loadConfigFile: ERROR: Invalid smilie specification!\n";
          input.close();
          return false;
        }
        parser.addSmilie(Smilie(code, s_url, (name=="smilie_r")));
      }//else if (smilie)
    }//if not empty
  }//while
  input.close();
  return true;
}
