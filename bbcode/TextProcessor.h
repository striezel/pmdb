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

#ifndef BBCODE_TEXTPROCESSOR_H
#define BBCODE_TEXTPROCESSOR_H

#include <string>

/* struct TextProcessor:
       basic interface for BB code pre- and post-processing structs
*/
struct TextProcessor
{
  public:
    /* destructor */
    virtual ~TextProcessor() {}

    /* processes the given text, i.e. performs transformations

       parameters:
           text - the message text that should be processed
    */
    virtual void applyToText(std::string& text) const = 0;
};//struct


/* struct ListNewlinePreProcessor:
      removes a single newline character (if present) right before the
      beginning of the BB code for a new list item. This helps to prevent
      unwanted line breaks in the list BB codes.

      Example:
      Consider the following BB code sequence.
      "[list]
       [*]item one
       [*]second item
       [/list]"

      This sequence would usually produce HTML code similar to this one, if
      line breaks will be converted to HTML code, too:
      "<ul><br>
       <li>item one<br>
       </li><li>second item<br>
       </li></ul>"

      However, the HTML code that actually is desired in most situations is
      similar to this:
      "<ul>
       <li>item one
       </li><li>second item
       </li></ul>"

       The ListNewlinePreProcessor helps to achieve that.
       However, the disadvantage is that in cases where those line breaks are
       actually wanted by the user, the input text has to contain an additional
       line break in this place.
*/
struct ListNewlinePreProcessor: public TextProcessor
{
  public:
    /* processes the given text, i.e. performs transformations

       parameters:
           text - the message text that should be processed
    */
    virtual void applyToText(std::string& text) const
    {
      std::string::size_type pos = text.find("\n[*]");
      while (pos!=std::string::npos)
      {
        text.replace(pos, 1, "");
        pos = text.find("\n[*]", pos+2);
      }//while
    }
};//struct

/* struct KillSpacesBeforeNewline:
      removes spaces at the end of lines
*/
struct KillSpacesBeforeNewline: public TextProcessor
{
  public:
    /* processes the given text, i.e. performs transformations

       parameters:
           text - the message text that should be processed
    */
    virtual void applyToText(std::string& text) const
    {
      std::string::size_type pos = text.find(" \n");
      while (pos!=std::string::npos)
      {
        text.replace(pos, 1, "");
        pos = text.find(" \n", (pos==0) ? 0 : pos-1);
      }//while

      pos = text.find(" \r\n");
      while (pos!=std::string::npos)
      {
        text.replace(pos, 1, "");
        pos = text.find(" \r\n", (pos==0) ? 0 : pos-1);
      }//while
    }
};//struct

#endif // BBCODE_TEXTPROCESSOR_H
