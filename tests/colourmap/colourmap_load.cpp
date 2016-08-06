/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database test suite.
    Copyright (C) 2015  Dirk Stolle

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

#include <iostream>
#include <vector>
#include "../../code/ColourMap.hpp"

/* Info:
   The programme tests, if colour map files are loaded.

   Recognized command line arguments: up to one
     1st - directory that contains the colour map files for the test suite.
           If not specified, the current directory will be used.
*/


int main(int argc, char **argv)
{
  // directory where the maps for this test case are located
  std::string mapDirectory = "";
  if ((argc==2) && (argv!=NULL))
  {
    if (argv[1]!=NULL)
      mapDirectory = std::string(argv[1]);
    if (mapDirectory.empty())
    {
      std::cout << "Error: invalid parameter!\n";
      return 1;
    }
  }
  else
  {
    #if defined(_WIN32) || defined(_WIN32_)
    mapDirectory = ".\\";
    #else
    mapDirectory = "./";
    #endif // defined
    std::cout << "Info: no parameter given. Will use '" << mapDirectory
              << "' as map directory.\n";
  }

  const std::string::size_type path_len = mapDirectory.size();
  //add trailing slash
  #if defined(_WIN32) || defined(_WIN32_)
  if (mapDirectory[path_len-1]!='\\')
    mapDirectory.append(1, '\\');
  #else
  if (mapDirectory[path_len-1]!='/')
    mapDirectory.append(1, '/');
  #endif // defined

  //create list of file names that shall be loaded later
  std::vector<std::string> files;
  files.push_back("colourmap_empty.map");
  files.push_back("colourmap_comments_only.map");
  files.push_back("colourmap_all_colours.map");
  files.push_back("colourmap_repeat.map");


  ColourMap cMap;

  //load stuff
  std::vector<std::string>::const_iterator iter = files.begin();
  while (iter!=files.end())
  {
    if (!cMap.loadFromFile(mapDirectory + *iter))
    {
      std::cout << "Error: Unable to load file " << *iter
                << " from directory \"" << mapDirectory << "\". Exiting.\n";
      return 1;
    }
    ++iter;
  } //while

  //success
  std::cout << "Success: all of the " << (int) files.size() << " example colour maps could be loaded.\n";
  return 0;
}
