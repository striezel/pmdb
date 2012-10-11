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

#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include "bbcode/BBCodeParser.h"

/* tries to read the cofiguration file from the given file name.
   Returns true in case of success, or false if an error occured.
   The parameters after the file name will be used to store the values that
   have been read from the the configuration file.

   parameters:
       fileName - path to the configuration file
       parser   - the parser that will be used (needed to add smilies)
       forumURL - string that will hold the forum URL from the config file, if
                  any was found
       tplFile  - string that will hold the path to the basic template file for
                  messages, if any was found
*/
bool loadConfigFile(const std::string& fileName, BBCodeParser& parser, std::string& forumURL, std::string& tplFile);

#endif // CONFIG_H
