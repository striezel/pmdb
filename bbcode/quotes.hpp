/*
    This file is part of the Private Message Database.
    Copyright (C) 2008, 2011, 2012  Thoronador

    The PM XML Interpreter is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    The PM XML Interpreter is distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef QUOTES_HPP
#define QUOTES_HPP

#include "../libthoro/common/StringUtils.h"

/* ************************************

   Quote handling functions (non-regexp)

   ************************************ */

enum OpeningTagType {ottNone=0, ottNormal=1, ottNamed=2, ottSingleQuoted=4};

inline std::string::size_type getClosingQuoteTagPosition(const std::string& text, const std::string::size_type offset)
{
  return find_ci(text, "[/quote]", offset);
}//getClosingTagPosition


/** \brief holds name and post number of an author of a quote */
struct NameAndPost
{
  std::string name;
  std::string post;


  /** \brief constructor
   *
   * \param theName name of the quoted person
   * \param thePost post number
   */
  NameAndPost(const std::string& theName="", const std::string& thePost="")
  : name(theName), post(thePost)
  {
  }
};//struct

std::string handleQuotes(std::string msg_content, const std::string& forumURL);

#endif // QUOTES_HPP
