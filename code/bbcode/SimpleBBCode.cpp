/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014, 2015, 2016  Dirk Stolle

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

#include "SimpleBBCode.hpp"
#include "../../libstriezel/common/StringUtils.hpp"

SimpleBBCode::SimpleBBCode(const std::string& code)
: BBCode(toLowerString(code))
{
}

void SimpleBBCode::applyToText(std::string& text) const
{
  const std::string code = "[" + getName() + "]";
  const std::string end_code = "[/" + getName() + "]";
  std::string::size_type pos = find_ci(text, code);
  std::string::size_type end_pos = std::string::npos;
  while (pos != std::string::npos)
  {
    end_pos = find_ci(text, end_code, pos + 1);
    if (end_pos == std::string::npos)
      return;
    text.replace(pos, code.length(), "<" + getName() + ">");
    text.replace(end_pos, end_code.length(), "</" + getName() + ">");
    pos = find_ci(text, code, pos);
  }
}
