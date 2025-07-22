/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2013, 2025  Dirk Stolle

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

#include "BBCodeParser.hpp"
#include "quotes.hpp"

BBCodeParser::BBCodeParser()
:
  #ifndef NO_PREPROCESSORS_IN_PARSER
  m_PreProcs(std::vector<TextProcessor*>()),
  #endif
  #ifndef NO_SMILIES_IN_PARSER
  m_Smilies(std::vector<Smilie>()),
  #endif
  #ifndef NO_POSTPROCESSORS_IN_PARSER
  m_PostProcs(std::vector<TextProcessor*>()),
  #endif
  m_Codes(std::vector<const BBCode*>())
{
}

std::string BBCodeParser::parse(std::string text, const std::string& forumURL, const HTMLStandard standard, const bool nl2br) const
{
  #ifndef NO_PREPROCESSORS_IN_PARSER
  // handle pre processors
  for (const auto * pre_ptr: m_PreProcs)
  {
    pre_ptr->applyToText(text);
  }
  #endif

  // handle line breaks
  if (nl2br)
  {
    std::string::size_type pos = text.find("\n");
    const bool isXHTML = standard == HTMLStandard::XHTML;
    while (pos != std::string::npos)
    {
      text.replace(pos, 1, isXHTML ? "<br />\n" : "<br>\n");
      pos = text.find("\n", isXHTML ? pos + 7 : pos + 5);
    }
  } // if

  // handle bb codes
  for (const BBCode* code: m_Codes)
  {
    code->applyToText(text);
  }

  #ifndef NO_SMILIES_IN_PARSER
  // handle smilies
  for (const auto& sm: m_Smilies)
  {
    sm.applyToText(text, forumURL, standard);
  }
  #endif

  // handle quotes
  text = handleQuotes(text, forumURL);

  #ifndef NO_POSTPROCESSORS_IN_PARSER
  // handle post processors
  for (const auto* post_ptr: m_PostProcs)
  {
    post_ptr->applyToText(text);
  }
  #endif

  return text;
}

#ifndef NO_PREPROCESSORS_IN_PARSER
void BBCodeParser::addPreProcessor(TextProcessor* preProc)
{
  if (preProc != nullptr)
    m_PreProcs.push_back(preProc);
}
#endif

void BBCodeParser::addCode(const BBCode* code)
{
  if (code != nullptr)
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
  if (postProc != nullptr)
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
