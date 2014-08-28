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

#include "BBCode.hpp"
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
    pos = find_ci(text, code, pos);
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
    pos = find_ci(text, code, pos);
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
    tpl.addReplacement(m_InnerName, transformInner(inner_text), false);
    text.replace(pos, end_pos+end_code.length()-pos, tpl.show());
    pos = find_ci(text, code, pos);
  }//while
}

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

ListBBCode::ListBBCode(const std::string& code, bool unordered)
: BBCode(code), m_Unordered(unordered)
{

}

bool ListBBCode::actualApply(std::string& text, const std::string::size_type offset) const
{
  const std::string code = "["+getName()+"]";
  const std::string end_code = "[/"+getName()+"]";
  const std::string item_code = "[*]";
  std::string::size_type pos = find_ci(text, code, offset);
  std::string::size_type end_pos = std::string::npos;
  while (pos!=std::string::npos)
  {
    end_pos = find_ci(text, end_code, pos+1);
    //no closing tag means invalid BB code use
    if (end_pos==std::string::npos) return false;
    std::string::size_type item_pos = find_ci(text, item_code, pos+1);
    //no [*] within opening and closing tag means error
    if ((item_pos==std::string::npos) or (item_pos>end_pos)) return false;
    std::string::size_type inner_pos = find_ci(text, code, pos+1);
    while ((inner_pos!=std::string::npos) and (inner_pos<end_pos))
    {
      //malformed code, if next list starts before first list item
      if (inner_pos<item_pos) return false;
      //We have nested list tags, so treat them first.
      if (!actualApply(text, inner_pos)) return false;
      //update positions
      item_pos = find_ci(text, item_code, pos+1);
      inner_pos = find_ci(text, code, pos+1);
      end_pos = find_ci(text, end_code, pos+1);
      //If there are no more end tags or item "tags" left, the code is invalid.
      if ((item_pos==std::string::npos) or (end_pos==std::string::npos))
        return false;
    }//inner while

    //replace first "[*]" with "<li>"
    text.replace(item_pos, 3, "<li>");
    //update end_pos and search next item
    ++end_pos;
    item_pos = find_ci(text, item_code, item_pos+3);
    //replace all remaining "[*]" of that list
    while ((item_pos!=std::string::npos) and (item_pos<end_pos))
    {
      text.replace(item_pos, 3, "</li><li>");
      end_pos += 6;
      item_pos = find_ci(text, item_code, item_pos+8);
    }//while (item code [*])
    //replace end tag
    text.replace(end_pos, end_code.length(), std::string("</li></") + (m_Unordered ? "ul>" : "ol>"));
    //replace start tag
    text.replace(pos, code.length(), m_Unordered ? "<ul>" : "<ol>");
    pos = find_ci(text, code, pos+1);
  }//while
  return true;
}

void ListBBCode::applyToText(std::string& text) const
{
  if (!text.empty())
    actualApply(text, 0);
}

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
