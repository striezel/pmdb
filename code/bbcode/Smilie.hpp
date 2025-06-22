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

#ifndef SMILIE_HPP
#define SMILIE_HPP

#include <string>
#include "HTMLStandard.hpp"

enum class UrlType: bool
{
  Absolute = false,
  Relative = true
};

/** \brief Represents a smilie code and its replacement image.
 */
struct Smilie
{
  public:
    /** \brief Creates a new smilie code.
     *
     * \param code       code for that smilie, e. g. ":)"
     * \param url        URL of the smilie image
     * \param url_type   set to UrlType::Relative, if the URL is relative to the forum URL
     */
    Smilie(const std::string& code, const std::string& url, const UrlType url_type);


    /** \brief "Applies" the smilie to the given text, i.e. transforms the smilie code
     *         into its HTML representation.
     *
     * \param text       the message text that (may) contain the smilie code
     * \param forumURL   base URL of the forum
     * \param standard   Indicates the HTML standard to use for the replacement (HTML or XHTML).
     */
    void applyToText(std::string& text, const std::string& forumURL, const HTMLStandard standard) const;
  private:
    std::string m_Code;
    std::string m_URL;
    UrlType type_of_url;
}; // struct

#endif // SMILIE_HPP
