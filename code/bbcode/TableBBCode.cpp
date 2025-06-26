/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2013, 2016, 2025  Dirk Stolle

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

#include "TableBBCode.hpp"
#ifdef DEBUG
  #include <iostream>
#endif
#include "../../libstriezel/common/StringUtils.hpp"
#include "Notifier.hpp"

TableBBCode::OpeningElem::OpeningElem()
: open_pos(std::string::npos), open_end(std::string::npos),
  attributes(std::map<std::string, std::string>())
{
}

TableBBCode::TableBBCode(const std::string& code, const TableClasses tblClasses,
                const unsigned int tableWidthMax)
: BBCode(code), classes(tblClasses), m_TableWidthLimit(tableWidthMax)
{
}

void TableBBCode::applyToText(std::string& text) const
{
  actualApplyToText(text, 0);
}

bool TableBBCode::actualApplyToText(std::string& text, const std::string::size_type offset) const
{
  const std::string end_code = "[/" + getName() + "]";
  std::string::size_type end_pos = std::string::npos;
  OpeningElem elemOpener = getNextOpeningElement(text, offset, getName());
  while (elemOpener.isValid())
  {
    end_pos = find_ci(text, end_code, elemOpener.open_end);
    if (end_pos == std::string::npos)
      return false;
    OpeningElem innerOpener = getNextOpeningElement(text, elemOpener.open_end, getName());
    while (innerOpener.isValid() && (innerOpener.open_pos < end_pos))
    {
      // We have an inner element, process it first.
      if (!actualApplyToText(text, innerOpener.open_pos))
        return false;
      innerOpener = getNextOpeningElement(text, elemOpener.open_end, getName());
    }//inner while

    /* all inner tables processed, go on with outermost in that scope */
    #ifdef DEBUG
    std::map<std::string, std::string>::const_iterator dbg_iter = elemOpener.attributes.begin();
    std::cout << "table attributes:\n";
    while (dbg_iter != elemOpener.attributes.end())
    {
      std::cout << dbg_iter->first << " = " << dbg_iter->second << "\n";
      ++dbg_iter;
    }//while
    std::cout << "total: "<<elemOpener.attributes.size() << "\n";
    #endif
    // update end position
    end_pos = find_ci(text, end_code, elemOpener.open_end);
    if (end_pos == std::string::npos)
      return false;
    OpeningElem rowOpener = getNextOpeningElement(text, elemOpener.open_end, "tr");
    if (!rowOpener.isValid() || (rowOpener.open_pos >= end_pos))
      return false;
    while (rowOpener.isValid() && (rowOpener.open_end < end_pos))
    {
      std::string::size_type row_end = find_ci(text, "[/tr]", rowOpener.open_end);
      if (row_end == std::string::npos)
        return false;
      OpeningElem cellOpener = getNextOpeningElement(text, rowOpener.open_end, "td");
      if (!cellOpener.isValid() || (cellOpener.open_pos > row_end))
        return false;
      while (cellOpener.isValid() && (cellOpener.open_end < row_end))
      {
        std::string::size_type cell_end = find_ci(text, "[/td]", cellOpener.open_end);
        if ((cell_end == std::string::npos) || (cell_end >= row_end))
          return false;
        // replace cell codes with HTML code
        text.replace(cell_end, 5, "</td>");
        text.replace(cellOpener.open_pos, cellOpener.open_end - cellOpener.open_pos + 1, "<td" + attributesToString(tetCell, cellOpener.attributes, rowOpener.attributes, elemOpener.attributes) + ">");
        // update row end position
        row_end = find_ci(text, "[/tr]", rowOpener.open_end);
        // get next open cell position
        cellOpener = getNextOpeningElement(text, cellOpener.open_pos + 1, "td");
      }//while cell opener
      //replace row codes with HTML code
      text.replace(row_end, 5, "</tr>");
      text.replace(rowOpener.open_pos, rowOpener.open_end - rowOpener.open_pos + 1, "<tr" + attributesToString(tetRow, rowOpener.attributes, elemOpener.attributes) + ">");
      // update table end position
      end_pos = find_ci(text, end_code, elemOpener.open_end);
      // get next open row position
      rowOpener = getNextOpeningElement(text, rowOpener.open_pos + 1, "tr");
    }//while row
    //replace "table" codes with HTML code
    text.replace(end_pos, end_code.length(), "</table>");
    text.replace(elemOpener.open_pos, elemOpener.open_end - elemOpener.open_pos + 1, "<table" + attributesToString(tetTable, elemOpener.attributes) + ">");
    //get next table
    elemOpener = getNextOpeningElement(text, elemOpener.open_pos, getName());
  }//while
  return true;
}

TableBBCode::OpeningElem TableBBCode::getNextOpeningElement(const std::string& text, const std::string::size_type offset, const std::string& tag) const
{
  OpeningElem result;
  result.open_pos = std::string::npos;
  result.open_end = std::string::npos;
  result.attributes.clear();

  const std::string code = "["+tag;
  std::string::size_type start = find_ci(text, code, offset);
  //if nothing found, return
  if (start==std::string::npos) return result;

  //check for length
  const std::string::size_type len = text.length();
  if (len<=start+code.length()) return result;

  switch (text[start+code.length()])
  {
    case ']':
         //normal opening table element with no attribute
         result.open_pos = start;
         result.open_end = start+code.length();
         return result;
         break;
    case '=':
         //element with attributes found
         result.open_pos = start;
         break;
    default:
         //No valid character, i.e. no real tag. Search for next one.
         return getNextOpeningElement(text, start+code.length(), tag);
         break;
  }//swi

  //if we get here, an attribute should be present
  if (len<start+code.length()+5) //minimum value has five chars: '"a:b"'
  {
    //too short
    result.open_pos = std::string::npos;
    return result;
  }

  //quotes there?
  if (text[start+code.length()+1]!='"')
  {
    //no, wrong syntax, search next
    return getNextOpeningElement(text, start+code.length()+1, tag);
  }
  //quotes there, search for end
  std::string::size_type maybe_end = text.find("\"]", start+code.length()+2);
  if (maybe_end==std::string::npos)
  {
    //nothing found, incomplete syntax
    result.open_pos = std::string::npos;
    return result;
  }
  //end found
  result.open_end = maybe_end+1;
  result.attributes = explodeAttributes(text.substr(start+code.length()+2, maybe_end-(start+code.length()+2)));
  return result;
}

std::map<std::string, std::string> TableBBCode::explodeAttributes(std::string attr)
{
  std::map<std::string, std::string> result;
  std::string::size_type pos = attr.find(',');
  do
  {
    std::string key_value = attr.substr(0, pos);
    trim(key_value);
    std::string::size_type sep_pos = key_value.find(':');
    if (sep_pos==std::string::npos) return result;
    std::string key = key_value.substr(0, sep_pos);
    std::string value = key_value.substr(sep_pos+1);
    trim(key); trim(value);
    result[key] = value;
    attr.erase(0, pos!=std::string::npos ? pos+1 : pos);
    pos = attr.find(';');
  } while (!attr.empty());
  return result;
}

void TableBBCode::appendGridAttributes(std::string& text, const TableElementType eleType) const
{
  if (classes.useClasses)
  {
    text += " class=\"";
    switch (eleType)
    {
      case tetTable:
           text += classes.table;
           break;
      case tetRow:
           text += classes.row;
           break;
      default:
           text += classes.cell;
           break;
    }//swi
    text += "\"";
  }//if
  else
    text += " style=\"border: 1px solid #000000; border-collapse: collapse;\"";
}

std::string TableBBCode::attributesToString(const TableElementType eleType,
                                          const std::map<std::string, std::string>& attrs,
                                          const std::map<std::string, std::string>& parent_attrs,
                                          const std::map<std::string, std::string>& grandparent_attrs) const
{
  std::string result;
  std::map<std::string, std::string>::const_iterator iter = attrs.begin();
  bool got_grid = false;
  while (iter!=attrs.end())
  {
    if ((iter->first=="class") and (iter->second=="grid"))
    {
      appendGridAttributes(result, eleType);
      got_grid = true;
    }
    else if (iter->first=="width")
    {
      if ((eleType==tetTable))
      {
        if (passesWidthLimit(iter->second))
        {
          result += " width=\""+iter->second+"\"";
        }
        else
        {
          //notify user about change
          notify<DefaultNotifier>("Width value of "+iter->second+" pixels was ignored in table code, because the table width limit is set to "
                                  +intToString(m_TableWidthLimit)+" pixels.\n");
        }
      }//if element==table
      else result += " width=\""+iter->second+"\"";
    }
    else if (iter->first=="align")
    {
      //align
      if ((iter->second=="left") or (iter->second=="right") or (iter->second=="center"))
      {
        result += " align=\""+iter->second+"\"";
      }
    }
    else if (iter->first=="colspan")
    {
      result += " colspan=\""+iter->second+"\"";
    }
    ++iter;
  }//while
  if (!got_grid)
  {
    iter = parent_attrs.find("class");
    if ((iter!=parent_attrs.end()) and (iter->second == "grid"))
    {
      appendGridAttributes(result, eleType);
      got_grid = true;
    }
    iter = grandparent_attrs.find("class");
    if (!got_grid and (iter!=grandparent_attrs.end()) and (iter->second == "grid"))
    {
      appendGridAttributes(result, eleType);
    }
  }//if no grid yet
  return result;
}

bool TableBBCode::passesWidthLimit(const std::string& attrValue) const
{
  if (m_TableWidthLimit==0) return true; //zero means no limit
  unsigned int width = 0;
  if (stringToUnsignedInt(attrValue, width))
  {
    return (width<=m_TableWidthLimit);
  }
  /* String conversion failed, so it's no plain integer value, but maybe a
     percentage, and those don't need to be limited. */
  return true;
}
