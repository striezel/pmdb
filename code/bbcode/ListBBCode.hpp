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

#ifndef LISTBBCODE_HPP
#define LISTBBCODE_HPP

#include <string>
#include "BBCode.hpp"


/** \brief ListBBCode:
       struct for list BB code
*/
struct ListBBCode: public BBCode
{
  public:
    /** \brief constructor
     *
     * \param code        "name" of the code, i.e. "list"
                          for "[LIST][*]item 1[*]2nd item[/LIST]"
     * \param unordered   if set to true, this code will produce an unordered list.
                          Otherwise it will create an ordered list.
     */
    ListBBCode(const std::string& code, bool unordered=true);


    /** destructor */
    virtual ~ListBBCode() {}


    /** \brief "applies" the BB code to the given text, i.e. transforms the BB code
     * into its HTML representation
     *
     * \param text   the message text that (may) contain the BB code
     */
    virtual void applyToText(std::string& text) const;


    /** returns true, if the code creates an unordered list */
    inline bool createsUnordered() const
    {
      return m_Unordered;
    }
  private:
    bool actualApply(std::string& text, const std::string::size_type offset) const;
    bool m_Unordered;
};//struct

#endif // LISTBBCODE_HPP
