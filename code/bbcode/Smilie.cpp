/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2025  Dirk Stolle

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

#include "Smilie.hpp"

Smilie::Smilie(const std::string& code, const std::string& url, const UrlType url_type)
: m_Code(code), m_URL(url), type_of_url(url_type)
{
}

void Smilie::applyToText(std::string& text, const std::string& forumURL, const bool isXHTML) const
{
  std::string::size_type pos = text.find(m_Code);

  const std::string replacement = "<img src=\""
                                + (type_of_url == UrlType::Relative ? forumURL + m_URL : m_URL) + "\" alt=\"" + m_Code
                                + (isXHTML ? "\" border=\"0\" />" : "\" border=\"0\">");
  while (pos != std::string::npos)
  {
    text.replace(pos, m_Code.length(), replacement);
    pos = text.find(m_Code, pos + replacement.length() - 1);
  }
}
