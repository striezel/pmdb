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

#ifndef BBCODE_TABLE_H
#define BBCODE_TABLE_H

#include "BBCode.h"
#include <map>

struct TableBBCode: public BBCode
{
  public:
    /* constructor

       parameters:
           code - "name" of the code, i.e. "b" for [B]bold text[/B]
    */
    TableBBCode(const std::string& code);

    /* "applies" the BB code to the given text, i.e. transforms the BB code
       into its HTML representation

       parameters:
           text - the message text that (may) contain the BB code
    */
    virtual void applyToText(std::string& text) const;

    //aux. structure
    struct OpeningElem
    {
      std::string::size_type open_pos, open_end;
      std::map<std::string, std::string> attributes;
      virtual inline bool isValid() const
      {
        return ((open_pos!=std::string::npos) and (open_end!=std::string::npos));
      }
    }; //struct
  private:
    bool actualApplyToText(std::string& text, const std::string::size_type offset) const;

    struct TableElem: public OpeningElem
    {
      std::string::size_type end_pos;
      virtual inline bool isValid() const
      {
        return ((open_pos!=std::string::npos) and (open_end!=std::string::npos) and (end_pos!=std::string::npos));
      }
    }; //struct
    OpeningElem getNextOpeningElement(const std::string& text, const std::string::size_type offset, const std::string& tag) const;

    static std::map<std::string, std::string> explodeAttributes(std::string attr);

    static std::string attributesToString(const std::map<std::string, std::string>& attrs);
}; //struct

#endif // BBCODE_TABLE_H
