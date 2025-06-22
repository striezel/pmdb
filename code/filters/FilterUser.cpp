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

#include "FilterUser.hpp"

FilterUser::FilterUser(const std::string& username, const UserType type, const Match match)
: user(username),
  type_of_user(type),
  match_kind(match)
{
}

bool FilterUser::match(const PrivateMessage& pm) const
{
  if (match_kind == Match::FullName)
  {
    if (type_of_user == UserType::Sender)
    {
      // compare with name of person who sent the PM
      return pm.getFromUser() == user;
    }
    // compare with name of recipient
    return pm.getToUser() == user;
  }
  // find partial matches
  if (type_of_user == UserType::Sender)
  {
    // search in name of person who sent the PM
    return pm.getFromUser().find(user) != std::string::npos;
  }
  // search in name of recipient
  return pm.getToUser().find(user) != std::string::npos;
}
