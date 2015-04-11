/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014  Thoronador

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

#include "PrivateMessage.hpp"
#include <fstream>
#ifdef DEBUG
  #include <iostream>
#endif
#include <sstream>
#include "PMSource.hpp"
#include "../libthoro/common/StringUtils.h"

PrivateMessage::PrivateMessage()
: datestamp(""),
  title(""),
  fromUser(""),
  fromUserID(0),
  toUser(""),
  message(""),
  m_NeedsHashUpdate(true),
  m_Hash(SHA256::MessageDigest())
{
}

void PrivateMessage::clear()
{
  datestamp.clear();
  title.clear();
  fromUser.clear();
  fromUserID = 0;
  toUser.clear();
  message.clear();
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
    pos = message.find("\r\n", pos);
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

bool PrivateMessage::saveToFile(const std::string& fileName) const
{
  std::ofstream output;
  output.open(fileName.c_str(), std::ios_base::out | std::ios_base::binary);
  if (!output)
  {
    return false;
  }
  //write datestamp
  output.write(datestamp.c_str(), datestamp.length()+1);
  //write title
  output.write(title.c_str(), title.length()+1);
  //write fromUser
  output.write(fromUser.c_str(), fromUser.length()+1);
  //write fromUserID
  const std::string uid_string = uintToString(fromUserID);
  output.write(uid_string.c_str(), uid_string.length()+1);
  //write toUser
  output.write(toUser.c_str(), toUser.length()+1);
  //write message text
  output.write(message.c_str(), message.length()+1);
  return output.good();
}

bool PrivateMessage::loadFromFile(const std::string& fileName)
{
  std::ifstream input;
  input.open(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!input)
  {
    return false;
  }
  input.seekg(0, std::ios_base::end);
  const std::streamsize len = input.tellg();
  input.seekg(0, std::ios_base::beg);
  if (!input.good())
  {
    #ifdef DEBUG
    std::cout << "Error while reading private message: seek operation(s) failed!\n";
    #endif
    input.close();
    return false;
  }
  //We do not want PMs larger than 1 MB, to avoid excessive memory consumption.
  if (len>1024*1024)
  {
    #ifdef DEBUG
    std::cout << "Error while reading private message: unexpected file size!\n";
    #endif
    input.close();
    return false;
  }
  //assume worst case for buffer size: all file content should fit into it
  char * buffer = new char[len+1];

  //read datestamp
  input.getline(buffer, len, '\0');
  if (!input.good())
  {
    #ifdef DEBUG
    std::cout << "Error while reading private message's datestamp part!\n";
    #endif
    delete[] buffer;
    input.close();
    return false;
  }
  buffer[input.gcount()] = '\0';
  datestamp = std::string(buffer);
  m_NeedsHashUpdate = true;

  //read title
  input.getline(buffer, len, '\0');
  if (!input.good())
  {
    #ifdef DEBUG
    std::cout << "Error while reading private message's title part!\n";
    #endif
    delete[] buffer;
    input.close();
    return false;
  }
  buffer[input.gcount()] = '\0';
  title = std::string(buffer);

  //read fromUser
  input.getline(buffer, len, '\0');
  if (!input.good())
  {
    #ifdef DEBUG
    std::cout << "Error while reading private message's sender part!\n";
    #endif
    delete[] buffer;
    input.close();
    return false;
  }
  buffer[input.gcount()] = '\0';
  fromUser = std::string(buffer);

  //read fromUserID
  input.getline(buffer, len, '\0');
  if (!input.good())
  {
    #ifdef DEBUG
    std::cout << "Error while reading private message's user ID!\n";
    #endif
    delete[] buffer;
    input.close();
    return false;
  }
  buffer[input.gcount()] = '\0';
  const std::string tempStr = std::string(buffer);
  if (!(std::stringstream (tempStr) >> fromUserID))
  {
    #ifdef DEBUG
    std::cout << "Error while converting private message's user ID string to integer!\n";
    #endif
    delete[] buffer;
    input.close();
    return false;
  }

  //read toUser
  input.getline(buffer, len, '\0');
  if (!input.good())
  {
    #ifdef DEBUG
    std::cout << "Error while reading private message's receiver!\n";
    #endif
    delete[] buffer;
    input.close();
    return false;
  }
  buffer[input.gcount()] = '\0';
  toUser = std::string(buffer);

  //read message text
  input.getline(buffer, len, '\0');
  if (!input.good())
  {
    #ifdef DEBUG
    std::cout << "Error while reading private message's text!\n";
    #endif
    delete[] buffer;
    input.close();
    return false;
  }
  buffer[input.gcount()] = '\0';
  message = std::string(buffer);
  m_NeedsHashUpdate = true;
  delete[] buffer; //free previously allocated buffer

  input.close();
  return true;
}

bool PrivateMessage::operator==(const PrivateMessage& other) const
{
  return ((datestamp == other.datestamp) && (title == other.title)
      && (fromUser == other.fromUser) && (fromUserID == other.fromUserID)
      && (toUser == other.toUser) && (message == other.message));
}

bool PrivateMessage::operator!=(const PrivateMessage& other) const
{
  return ((datestamp != other.datestamp) or (title != other.title)
      or (fromUser != other.fromUser) or (fromUserID != other.fromUserID)
      or (toUser != other.toUser) or (message != other.message));
}
