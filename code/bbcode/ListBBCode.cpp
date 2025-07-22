/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2015, 2016  Dirk Stolle

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

#include "ListBBCode.hpp"
#include "../../libstriezel/common/StringUtils.hpp"

ListBBCode::ListBBCode(const std::string& code, bool unordered)
: BBCode(code), m_Unordered(unordered)
{
}

bool ListBBCode::actualApply(std::string& text, const std::string::size_type offset) const
{
  const std::string code = "[" + getName() + "]";
  const std::string end_code = "[/" + getName() + "]";
  const std::string item_code = "[*]";
  std::string::size_type pos = find_ci(text, code, offset);
  std::string::size_type end_pos = std::string::npos;
  while (pos != std::string::npos)
  {
    end_pos = find_ci(text, end_code, pos + 1);
    // no closing tag means invalid BB code use
    if (end_pos == std::string::npos)
    {
      return false;
    }
    std::string::size_type item_pos = find_ci(text, item_code, pos + 1);
    //no [*] within opening and closing tag means error
    if ((item_pos == std::string::npos) || (item_pos > end_pos))
    {
      return false;
    }
    std::string::size_type inner_pos = find_ci(text, code, pos + 1);
    while ((inner_pos != std::string::npos) && (inner_pos < end_pos))
    {
      // malformed code, if next list starts before first list item
      if (inner_pos < item_pos)
        return false;
      // We have nested list tags, so treat them first.
      if (!actualApply(text, inner_pos))
        return false;
      // update positions
      item_pos = find_ci(text, item_code, pos + 1);
      inner_pos = find_ci(text, code, pos + 1);
      end_pos = find_ci(text, end_code, pos + 1);
      // If there are no more end tags or item "tags" left, the code is invalid.
      if ((item_pos == std::string::npos) || (end_pos == std::string::npos))
        return false;
    }//inner while

    // replace first "[*]" with "<li>"
    text.replace(item_pos, 3, "<li>");
    // update end_pos and search next item
    ++end_pos;
    item_pos = find_ci(text, item_code, item_pos + 3);
    // replace all remaining "[*]" of that list
    while ((item_pos != std::string::npos) && (item_pos < end_pos))
    {
      text.replace(item_pos, 3, "</li><li>");
      end_pos += 6;
      item_pos = find_ci(text, item_code, item_pos+8);
    }//while (item code [*])
    // replace end tag
    text.replace(end_pos, end_code.length(), std::string("</li></") + (m_Unordered ? "ul>" : "ol>"));
    // replace start tag
    text.replace(pos, code.length(), m_Unordered ? "<ul>" : "<ol>");
    pos = find_ci(text, code, pos + 1);
  }//while
  return true;
}

void ListBBCode::applyToText(std::string& text) const
{
  if (!text.empty())
    actualApply(text, 0);
}
