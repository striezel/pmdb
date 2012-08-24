/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012  Thoronador

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

#include "PrivateMessage.h"

PrivateMessage::PrivateMessage()
{
  datestamp = "";
  title = "";
  fromUser = "";
  fromUserID = 0;
  toUser = "";
  message = "";
}

void PrivateMessage::normalise()
{
  std::string::size_type pos = message.find("\r\n");
  while (pos!=std::string::npos)
  {
    message.replace(pos, 2, "\n");
    pos = message.find("\r\n");
  }//while
}
