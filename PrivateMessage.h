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

#ifndef PRIVATEMESSAGE_H
#define PRIVATEMESSAGE_H

#include <string>
#include <stdint.h>
#include "random-thoro/cpp/hashfunctions/sha-256.h"

class PrivateMessage
{
  public:
    /* constructor */
    PrivateMessage();

    /* clears/resets all data members */
    void clear();

    /* normalises the line breaks in the message text */
    void normalise();

    //access to data members
    inline const std::string& getDatestamp() const
    {
      return datestamp;
    }

    inline const std::string& getTitle() const
    {
      return title;
    }

    inline const std::string& getFromUser() const
    {
      return fromUser;
    }

    inline uint32_t getFromUserID() const
    {
      return fromUserID;
    }

    inline const std::string& getToUser() const
    {
      return toUser;
    }

    inline const std::string& getMessage() const
    {
      return message;
    }

    const SHA256::MessageDigest& getHash();

    //functions to set various data members
    void setDatestamp(const std::string& ds);

    void setTitle(const std::string& t);

    void setFromUser(const std::string& from);

    void setFromUserID(const uint32_t uid);

    void setToUser(const std::string& to);

    void setMessage(const std::string& msg);

    /* tries to save the message to the given file and returns true in case of
       success, or false if an error occured */
    bool saveToFile(const std::string& fileName) const;

    /* tries to load the message to the given file and returns true in case of
       success, or false if an error occured */
    bool loadFromFile(const std::string& fileName);
  private:
    std::string datestamp;
    std::string title;
    std::string fromUser;
    uint32_t fromUserID;
    std::string toUser;
    std::string message;
    bool m_NeedsHashUpdate;
    SHA256::MessageDigest m_Hash;
};//class

#endif // PRIVATEMESSAGE_H
