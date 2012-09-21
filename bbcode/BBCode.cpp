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

#include "BBCode.h"
#include "../libthoro/common/StringUtils.h"

SimpleBBCode::SimpleBBCode(const std::string& code)
: BBCode(code)
{

}

void SimpleBBCode::applyToText(std::string& text) const
{
  const std::string code = "["+getName()+"]";
  const std::string end_code = "[/"+getName()+"]";
  std::string::size_type pos = find_ci(text, code);
  std::string::size_type end_pos = std::string::npos;
  while (pos!=std::string::npos)
  {
    end_pos = find_ci(text, end_code, pos+1);
    if (end_pos==std::string::npos) return;
    text.replace(pos, code.length(), "<"+getName()+">");
    text.replace(end_pos, end_code.length(), "</"+getName()+">");
    pos = find_ci(text, code);
  }//while
}

CustomizedSimpleBBCode::CustomizedSimpleBBCode(const std::string& code, const std::string& before, const std::string& after)
: BBCode(code), m_Before(before), m_After(after)
{
}

void CustomizedSimpleBBCode::applyToText(std::string& text) const
{
  const std::string code = "["+getName()+"]";
  const std::string end_code = "[/"+getName()+"]";
  std::string::size_type pos = find_ci(text, code);
  std::string::size_type end_pos = std::string::npos;
  while (pos!=std::string::npos)
  {
    end_pos = find_ci(text, end_code, pos+1);
    if (end_pos==std::string::npos) return;
    text.replace(end_pos, end_code.length(), m_After);
    text.replace(pos, code.length(), m_Before);
    pos = find_ci(text, code);
  }//while
}

SimpleTemplateBBCode::SimpleTemplateBBCode(const std::string& code, const MsgTemplate& tpl, const std::string& inner)
: BBCode(code), m_Template(tpl), m_InnerName(inner)
{

}

void SimpleTemplateBBCode::applyToText(std::string& text) const
{
  const std::string code = "["+getName()+"]";
  const std::string end_code = "[/"+getName()+"]";
  std::string::size_type pos = find_ci(text, code);
  std::string::size_type end_pos = std::string::npos;
  MsgTemplate tpl = m_Template;
  while (pos!=std::string::npos)
  {
    end_pos = find_ci(text, end_code, pos+1);
    if (end_pos==std::string::npos) return;
    const std::string inner_text = text.substr(pos+code.length(), end_pos-(pos+code.length()));
    tpl.addReplacement(m_InnerName, inner_text, false);
    text.replace(pos, end_pos+end_code.length()-pos, tpl.show());
    pos = find_ci(text, code);
  }//while
}
