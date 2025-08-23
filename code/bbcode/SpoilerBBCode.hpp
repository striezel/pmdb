/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2025  Dirk Stolle

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

#ifndef SPOILERBBCODE_HPP
#define SPOILERBBCODE_HPP

#include "CustomizedSimpleBBCode.hpp"

/** \brief AdvancedTemplateBBCode:
       Handles the [SPOILER] code. Currently, it uses the old layout, i.e.
       the version without JavaScript. That may change in future versions.
*/
struct SpoilerBBCode: public CustomizedSimpleBBCode
{
  public:
    /** \brief constructor
     *
     * \param code    "name" of the code, i.e. "b" for [B]bold text[/B]
     */
    SpoilerBBCode(const std::string& code)
    : CustomizedSimpleBBCode(code,
                             R"(<div style="margin: 5px;"><span>Spoiler: (zum Lesen bitte Text markieren)</span><div style="background:#999; color: #999; border: 1px solid #ff0000; padding: 5px;"><div style=" border-color:#4F4F4F #CECECE #CECECE #4F4F4F; border-width: 1px; border-style: solid; margin: -2px;">)", "</div></div></div>")
    {
      /* HTML code would be something like:

        <div style="margin: 5px;"><span>Spoiler: (zum lesen bitte Text markieren)</span><div style="background:#999; color: #999; border: 1px solid #ff0000; padding: 5px;"><div style=" border-color:#4F4F4F #CECECE #CECECE #4F4F4F; border-width: 1px; border-style: solid; margin: -2px;">versteckter Text</div></div></div>

      */
    }
}; // struct

#endif // SPOILERBBCODE_HPP
