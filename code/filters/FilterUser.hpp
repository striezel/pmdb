/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2014, 2025  Dirk Stolle

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

#ifndef FILTERUSER_HPP
#define FILTERUSER_HPP

#include <string>
#include "Filter.hpp"

enum class UserType: bool
{
  /// recipient of the private message
  Recipient = false,

  /// sender of the private message
  Sender = true
};

enum class Match: bool
{
  PartialName = false,
  FullName = true
};


/// Filters PMs by user name.
class FilterUser : public Filter
{
  public:
    /** \brief Creates a new filter based on user name.
     *
     * \param username  name of the user to match
     * \param type      which type of user (sender or recipient) to match
     * \param match     whether to match full or partial name
     */
    FilterUser(const std::string& username, const UserType type, const Match match);

    /** \brief Checks, if a given PM matches the filter criterion.
     *
     * \param pm  reference to the private message
     * \return Returns true, if criterion is fulfilled. Returns false otherwise.
     */
    virtual bool match(const PrivateMessage& pm) const;
  private:
    std::string user;
    UserType type_of_user;
    Match match_kind;
}; // class FilterUser

#endif // FILTERUSER_HPP
