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

#include "AdvancedTemplateBBCode.hpp"
#include "../../libstriezel/common/StringUtils.hpp"

AdvancedTemplateBBCode::AdvancedTemplateBBCode(const std::string& code, const MsgTemplate& tpl, const std::string& inner, const std::string& attr)
: SimpleTemplateBBCode(code, tpl, inner), m_AttrName(attr)
{

}

void AdvancedTemplateBBCode::applyToText(std::string& text) const
{
  const std::string code = "["+getName()+"=";
  const std::string end_code = "[/"+getName()+"]";
  std::string::size_type pos = find_ci(text, code);
  std::string::size_type bracket_pos = std::string::npos;
  std::string::size_type end_pos = std::string::npos;
  MsgTemplate tpl = getTemplate();
  while (pos!=std::string::npos)
  {
    bracket_pos = find_ci(text, "]", pos+1);
    if (bracket_pos==std::string::npos) return;
    end_pos = find_ci(text, end_code, bracket_pos+1);
    if (end_pos==std::string::npos) return;
    std::string attr_text = text.substr(pos+code.length(), bracket_pos-(pos+code.length()));
    if (attr_text.length()>1)
    {
      if ((attr_text[0]=='"') and (attr_text[attr_text.length()-1]=='"'))
      {
        attr_text.erase(attr_text.length()-1, 1);
        attr_text.erase(0, 1);
      }
    }
    const std::string inner_text = text.substr(bracket_pos+1, end_pos-(bracket_pos+1));
    tpl.addReplacement(getInnerName(), transformInner(inner_text), false);
    tpl.addReplacement(m_AttrName, transformAttribute(attr_text), false);
    text.replace(pos, end_pos+end_code.length()-pos, tpl.show());
    pos = find_ci(text, code, pos);
  }//while
}
