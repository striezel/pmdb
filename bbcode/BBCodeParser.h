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

#ifndef BBCODEPARSER_H
#define BBCODEPARSER_H

#include <string>
#include <vector>
#include "BBCode.h"
#include "Smilie.h"

class BBCodeParser
{
  public:
    /* transforms BB codes in text to HTML codes (still incomplete)

       parameters:
           text - the original text
    */
    std::string parse(std::string text, const std::string& forumURL) const;

    /* adds a new bb code to the parser

       parameters:
           code - pointer to the BB code object

       remarks:
           The passed pointers must live for the whole lifetime of the
           BBCodeParser instance they are passed to. Otherwise parse() will
           fail.
    */
    void addCode(BBCode* code);

    /* adds a new smilie to the parser

       parameters:
           sm - the smilie
    */
    void addSmilie(const Smilie& sm);

    /* clears all added BB codes */
    void clearCodes();

    /* clears all added smilies */
    void clearSmilies();
  private:
    std::vector<BBCode*> m_Codes;
    std::vector<Smilie>  m_Smilies;
};//class

#endif // BBCODEPARSER_H
