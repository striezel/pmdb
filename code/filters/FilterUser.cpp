/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2014  Dirk Stolle

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

FilterUser::FilterUser(const std::string& username, const bool isSender, const bool onlyFullName)
: user(username),
  sender(isSender),
  fullName(onlyFullName)
{ }

bool FilterUser::match(const PrivateMessage& pm) const
{
  if (fullName)
  {
    if (sender)
      //compare with name of person who sent the PM
      return pm.getFromUser() == user;
    //compare with name of recipient
    return pm.getToUser() == user;
  }//if
  //find partial matches
  if (sender)
    //search in name of person who sent the PM
    return pm.getFromUser().find(user) != std::string::npos;
  //search in name of recipient
  return pm.getToUser().find(user) != std::string::npos;
}
