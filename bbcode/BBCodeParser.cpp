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

std::string BBCodeParser::parse(std::string text, const std::string& forumURL, const bool isXHTML, const bool nl2br) const
{
  #ifndef NO_PREPROCESSORS_IN_PARSER
  //handle pre processors
  std::vector<TextProcessor*>::const_iterator pre_iter = m_PreProcs.begin();
  while (pre_iter!=m_PreProcs.end())
  {
    (*pre_iter)->applyToText(text);
    ++pre_iter;
  }//while
  #endif

  //handle line breaks
  if (nl2br)
  {
    std::string::size_type pos = text.find("\n");
    while (pos!=std::string::npos)
    {
      text.replace(pos, 1, isXHTML ? "<br />" : "<br>");
      pos = text.find("\n", pos+3);
    }//while
  }//if

  //handle bb codes
  std::vector<BBCode*>::const_iterator iter = m_Codes.begin();
  while (iter!=m_Codes.end())
  {
    (*iter)->applyToText(text);
    ++iter;
  }//while

  #ifndef NO_SMILIES_IN_PARSER
  //handle smilies
  std::vector<Smilie>::const_iterator sm_iter = m_Smilies.begin();
  while (sm_iter!=m_Smilies.end())
  {
    sm_iter->applyToText(text, forumURL, isXHTML);
    ++sm_iter;
  }//while
  #endif

  //handle quotes
  text = handleQuotes(text, forumURL);

  #ifndef NO_POSTPROCESSORS_IN_PARSER
  //handle post processors
  std::vector<TextProcessor*>::const_iterator post_iter = m_PostProcs.begin();
  while (post_iter!=m_PostProcs.end())
  {
    (*post_iter)->applyToText(text);
    ++post_iter;
  }//while
  #endif

  return text;
}

#ifndef NO_PREPROCESSORS_IN_PARSER
void BBCodeParser::addPreProcessor(TextProcessor* preProc)
{
  if (preProc!=NULL)
    m_PreProcs.push_back(preProc);
}
#endif

void BBCodeParser::addCode(BBCode* code)
{
  if (code!=NULL)
    m_Codes.push_back(code);
}

#ifndef NO_SMILIES_IN_PARSER
void BBCodeParser::addSmilie(const Smilie& sm)
{
  m_Smilies.push_back(sm);
}
#endif

#ifndef NO_POSTPROCESSORS_IN_PARSER
void BBCodeParser::addPostProcessor(TextProcessor* postProc)
{
  if (postProc!=NULL)
    m_PostProcs.push_back(postProc);
}
#endif

void BBCodeParser::clearCodes()
{
  m_Codes.clear();
}

#ifndef NO_SMILIES_IN_PARSER
void BBCodeParser::clearSmilies()
{
  m_Smilies.clear();
}
#endif
