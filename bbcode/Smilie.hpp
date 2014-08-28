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

#ifndef SMILIE_H
#define SMILIE_H

#include <string>

/* struct Smilie:
       represents a smilie code and its replacement image
*/
struct Smilie
{
  public:
    /* constructor

       parameters:
           code     - code for that smilie
           url      - URL of the smilie image
           relative - set to true, if the URL is relative to the forum URL
    */
    Smilie(const std::string& code, const std::string& url, const bool relative);

    /* "applies" the smilie to the given text, i.e. transforms the smilie code
       into its HTML representation

       parameters:
           text     - the message text that (may) contain the smilie code
           forumURL - base URL of the forum
           isXHTML  - if set to true, the replacement will XHTML code. Otherwise
                      HTML code is used.
    */
    void applyToText(std::string& text, const std::string& forumURL, const bool isXHTML) const;
  private:
    std::string m_Code;
    std::string m_URL;
    bool m_Relative;
};//struct

#endif // SMILIE_H
