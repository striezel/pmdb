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

#ifndef MESSAGEDATABASE_H
#define MESSAGEDATABASE_H

#include "PrivateMessage.h"
#include <vector>

class MessageDatabase
{
  public:
    /* constructor */
    MessageDatabase();

    /* adds a message to the database */
    void addMessage(const PrivateMessage& pm);

    /* returns the number of messages that are in the database */
    unsigned int getNumberOfMessages() const;

  //private:
      std::vector<PrivateMessage> m_Messages;
};//class

#endif // MESSAGEDATABASE_H
