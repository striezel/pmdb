/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014  Thoronador

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

#include "ColourMap.hpp"
#include <fstream>
#include <iostream>
#include "ConsoleColours.h"

ColourMap::ColourMap()
: colours(std::map<std::string, std::string>())
{
}

void ColourMap::clear()
{
  colours.clear();
}

bool ColourMap::loadFromFile(const std::string& fileName)
{
  std::ifstream input;
  input.open(fileName.c_str(), std::ios::in | std::ios::binary);
  if (!input)
  {
    return false;
  }

  //clear existing values
  clear();

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
        std::cout << "ColourMap::loadFromFile: ERROR: Invalid line found: \""
                  << line <<"\".\nGeneral format: \"Name of folder=colour\"\n"
                  << "Loading from file cancelled.\n";
        input.close();
        return false;
      }

      const std::string folder = line.substr(0, sep_pos);
      const std::string colour = line.substr(sep_pos).erase(0, 1);
      if (folder.empty() || colour.empty())
      {
        std::cout << "ColourMap::loadFromFile: ERROR: empty folder name or empty value!\n";
        input.close();
        return false;
      }
      const std::string code = nameToControlSequence(colour);
      if (!code.empty())
      {
        colours[folder] = code;
      }
      else
      {
        std::cout << "ColourMap::loadFromFile: ERROR: Invalid colour specification!\n"
                  << "\""<<colour<<"\" is not a known colour name.\n";
          input.close();
          return false;
      }//else
    }//if line not empty
  }//while
  input.close();
  return true;
}

void ColourMap::add(const std::string& folder, const std::string& sequence)
{
  if (!folder.empty() && !sequence.empty())
    colours[folder] = sequence;
}

const std::string ColourMap::colouredFolder(const std::string& folder) const
{
  const std::map<std::string, std::string>::const_iterator iter = colours.find(folder);
  if (iter!=colours.end())
    return iter->second + folder + colourDefault;
  //no colour found for that folder
  return folder;
}
