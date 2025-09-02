/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014, 2015, 2025  Dirk Stolle

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

#ifndef HORIZONTALRULEBBCODE_HPP
#define HORIZONTALRULEBBCODE_HPP

#include <string>
#include "BBCode.hpp"
#include "../HTMLStandard.hpp"

/** \brief HorizontalRuleBBCode:
       struct for hr BB code
*/
struct HorizontalRuleBBCode: public BBCode
{
  public:
    /** Creates a new instance.
     *
     * \param code      "name" of the code, i.e. "b" for [B]bold text[/B]
     * \param standard  the HTML standard to use during transformation (HTML or XHTML)
     */
    HorizontalRuleBBCode(const std::string& code, const HTMLStandard standard);


    /** \brief "Applies" the BB code to the given text, i.e. transforms the BB code
     * into its (X)HTML representation.
     *
     * \param text   the message text that (may) contain the BB code
     */
    virtual void applyToText(std::string& text) const;
  private:
    HTMLStandard m_standard;
}; // struct

#endif // HORIZONTALRULEBBCODE_HPP
