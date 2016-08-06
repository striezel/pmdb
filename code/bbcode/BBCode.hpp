/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2013, 2014  Dirk Stolle

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

#ifndef BBCODE_HPP
#define BBCODE_HPP

#include <string>
#include "../MsgTemplate.hpp"

/** \brief BBCode: basic interface for BB code structs/classes */
struct BBCode
{
  public:
    /** \brief constructor
     *
     * \param code   "name" of the code, i.e. "b" for [B]bold text[/B]
     */
    BBCode(const std::string& code)
    : m_Name(code)
    { }


    /** destructor */
    virtual ~BBCode() {}


    /** \brief returns the code's "name"
     */
    inline const std::string& getName() const
    {
      return m_Name;
    }

    /** \brief "applies" the BB code to the given text, i.e. transforms the BB code
     * into its HTML representation
     *
     * \param text   the message text that (may) contain the BB code
     */
    virtual void applyToText(std::string& text) const = 0;

    #ifndef NO_BBCODE_NOTIFY
    template<typename notifier>
    void notify(const std::string& msg) const
    {
      notifier::put(msg);
    }
    #endif
  private:
    std::string m_Name;
};//struct

#endif // BBCODE_HPP
