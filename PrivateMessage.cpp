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
#include <sstream>
#include "PMSource.h"

PrivateMessage::PrivateMessage()
{
  clear();
}

void PrivateMessage::clear()
{
  datestamp = "";
  title = "";
  fromUser = "";
  fromUserID = 0;
  toUser = "";
  message = "";
  m_NeedsHashUpdate = true;
  m_Hash.setToNull();
}

void PrivateMessage::normalise()
{
  std::string::size_type pos = message.find("\r\n");
  while (pos!=std::string::npos)
  {
    message.replace(pos, 2, "\n");
    m_NeedsHashUpdate = true;
    pos = message.find("\r\n");
  }//while
}

const SHA256::MessageDigest& PrivateMessage::getHash()
{
  if (m_NeedsHashUpdate)
  {
    SHA256::PMSource pms(*this);
    m_Hash = SHA256::computeFromSource(pms);
    m_NeedsHashUpdate = false;
  }
  return m_Hash;
}

void PrivateMessage::setDatestamp(const std::string& ds)
{
  datestamp = ds;
  m_NeedsHashUpdate = true;
}

void PrivateMessage::setTitle(const std::string& t)
{
  title = t;
  m_NeedsHashUpdate = true;
}

void PrivateMessage::setFromUser(const std::string& from)
{
  fromUser = from;
  m_NeedsHashUpdate = true;
}

void PrivateMessage::setFromUserID(const uint32_t uid)
{
  fromUserID = uid;
  m_NeedsHashUpdate = true;
}

void PrivateMessage::setToUser(const std::string& to)
{
  toUser = to;
  m_NeedsHashUpdate = true;
}

void PrivateMessage::setMessage(const std::string& msg)
{
  message = msg;
  m_NeedsHashUpdate = true;
}
