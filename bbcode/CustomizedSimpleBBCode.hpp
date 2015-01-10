/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014, 2015  Thoronador

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

#ifndef CUSTOMIZEDSIMPLEBBCODE_HPP
#define CUSTOMIZEDSIMPLEBBCODE_HPP

#include <string>
#include "BBCode.hpp"


/** \brief CustomizedSimpleBBCode:
       like SimpleBBCode, but with a custom replacement for opening and closing
       tags.
*/
struct CustomizedSimpleBBCode: public BBCode
{
  public:
    /** \brief constructor
     *
     * \param code     "name" of the code, i.e. "b" for [B]bold text[/B]
     * \param before   replacement for the opening code tag
     * \param after    replacements for the closing code tag
     */
    CustomizedSimpleBBCode(const std::string& code, const std::string& before, const std::string& after);


    /** destructor */
    virtual ~CustomizedSimpleBBCode() {}


    /** \brief "applies" the BB code to the given text, i.e. transforms the BB code
     * into its HTML representation
     *
     * \param text   the message text that (may) contain the BB code
     */
    virtual void applyToText(std::string& text) const;
  private:
    std::string m_Before, m_After;
};//struct

#endif // CUSTOMIZEDSIMPLEBBCODE_HPP
