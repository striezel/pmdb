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

#include "BBCodeParser.h"
#include "quotes.h"

std::string BBCodeParser::parse(std::string text, const std::string& forumURL, const bool isXHTML) const
{
  std::string::size_type pos = text.find("\n");
  //handle line breaks
  while (pos!=std::string::npos)
  {
    text.replace(pos, 1, isXHTML ? "<br />" : "<br>");
    pos = text.find("\n");
  }//while

  //handle bb codes
  std::vector<BBCode*>::const_iterator iter = m_Codes.begin();
  while (iter!=m_Codes.end())
  {
    (*iter)->applyToText(text);
    ++iter;
  }//while

  //handle smilies
  std::vector<Smilie>::const_iterator sm_iter = m_Smilies.begin();
  while (sm_iter!=m_Smilies.end())
  {
    sm_iter->applyToText(text, forumURL, isXHTML);
    ++sm_iter;
  }//while

  //handle quotes
  return handleQuotes(text, forumURL);
}

void BBCodeParser::addCode(BBCode* code)
{
  m_Codes.push_back(code);
}

void BBCodeParser::addSmilie(const Smilie& sm)
{
  m_Smilies.push_back(sm);
}

void BBCodeParser::clearCodes()
{
  m_Codes.clear();
}

void BBCodeParser::clearSmilies()
{
  m_Smilies.clear();
}