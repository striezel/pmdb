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

#include "BBCode.hpp"
#include <map>

struct TableBBCode: public BBCode
{
  public:
    //constants for default class names used in constructor
    static const std::string DefaultTableClass;
    static const std::string DefaultRowClass;
    static const std::string DefaultCellClass;

    /* constructor

       parameters:
           code           - "name" of the code, i.e. "b" for [B]bold text[/B]
           useGridClasses - if set to true, the generated HTML code will use
                            classes (as in CSS class) instead of a somewhat
                            bloated style attribute to generate the grids around
                            the table elements, if such the grids are required
           tableClass     - name of the class for grids on <table> tags
           rowClass       - name of the class for grids on <tr> tags
           cellClass      - name of the class for grids on <td> tags
           tableWidthMax  - the maximum width value (in pixels) that is allowed
                            in table attributes. Larger values will be ignored.

       remarks:
           If useGridClasses is set to false, the last three parameters do not
           have any effect on the generated HTML code.
    */
    TableBBCode(const std::string& code, const bool useGridClasses=false,
                const std::string& tableClass=DefaultTableClass, const std::string& rowClass=DefaultRowClass,
                const std::string& cellClass=DefaultCellClass,
                const unsigned int tableWidthMax=0);

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

      /* constructor */
      OpeningElem();

      virtual inline bool isValid() const
      {
        return ((open_pos!=std::string::npos) and (open_end!=std::string::npos));
      }
    }; //struct
  private:
    bool m_UseClasses;
    std::string m_TableClass;
    std::string m_RowClass;
    std::string m_CellClass;

    unsigned int m_TableWidthLimit;

    enum TableElementType {tetTable, tetRow, tetCell};

    void appendGridAttributes(std::string& text, const TableElementType eleType) const;

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

    std::string attributesToString(const TableElementType eleType,
                                   const std::map<std::string, std::string>& attrs,
                                   const std::map<std::string, std::string>& parent_attrs = std::map<std::string, std::string>(),
                                   const std::map<std::string, std::string>& grandparent_attrs  = std::map<std::string, std::string>()) const;

    /* returns true, if the passed attribute value contains an integer value
       that passes the table width limit

       parameters:
           attrValue - the attribute string (i.e. "650")
    */
    bool passesWidthLimit(const std::string& attrValue) const;
}; //struct

#endif // BBCODE_TABLE_H
