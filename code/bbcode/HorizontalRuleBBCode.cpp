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

#include "HorizontalRuleBBCode.hpp"
#include "../../libstriezel/common/StringUtils.hpp"

HorizontalRuleBBCode::HorizontalRuleBBCode(const std::string& code, const bool isXHTML)
: BBCode(code), m_isXHTML(isXHTML)
{ }

void HorizontalRuleBBCode::applyToText(std::string& text) const
{
  const std::string code = "["+getName()+"][/"+getName()+"]";
  std::string::size_type pos = find_ci(text, code);
  while (pos!=std::string::npos)
  {
    text.replace(pos, code.length(), m_isXHTML ? "<hr />" : "<hr>");
    pos = find_ci(text, code);
  }//while
}
