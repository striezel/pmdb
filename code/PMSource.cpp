/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014, 2016  Dirk Stolle

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

#include "PMSource.hpp"
#include <cstring>
#ifdef DEBUG
  #include <iostream>
#endif
#include <stdexcept>
#include "../libstriezel/common/StringUtils.hpp"
#include "../libstriezel/common/BufferStream.hpp"

namespace SHA256
{

PMSource::PMSource(const PrivateMessage& pm)
{
  const std::string uid_string = uintToString(pm.getFromUserID());
  const std::string::size_type origLen =
       pm.getDatestamp().length()+1
      +pm.getTitle().length()+1
      +pm.getFromUser().length()+1
      +uid_string.length()+1
      +pm.getToUser().length()+1
      +pm.getMessage().length()+1;
  m_PaddingBuffer = new uint8_t[origLen];
  //copy datestamp
  uint64_t done = pm.getDatestamp().length()+1;
  memcpy(&m_PaddingBuffer[0], pm.getDatestamp().c_str(), done);
  //copy title
  memcpy(&m_PaddingBuffer[done], pm.getTitle().c_str(), pm.getTitle().length()+1);
  done = done + pm.getTitle().length()+1;
  //copy fromUser
  memcpy(&m_PaddingBuffer[done], pm.getFromUser().c_str(), pm.getFromUser().length()+1);
  done = done + pm.getFromUser().length()+1;
  //copy fromUserID
  memcpy(&m_PaddingBuffer[done], uid_string.c_str(), uid_string.length()+1);
  done = done + uid_string.length()+1;
  //copy toUser
  memcpy(&m_PaddingBuffer[done], pm.getToUser().c_str(), pm.getToUser().length()+1);
  done = done + pm.getToUser().length()+1;
  //copy message
  memcpy(&m_PaddingBuffer[done], pm.getMessage().c_str(), pm.getMessage().length()+1);
  done = done + pm.getMessage().length()+1;
  #ifdef DEBUG
  if (done!=origLen)
  {
    std::cout << "Number of written bytes does not match expected number!\n";
    throw std::runtime_error("PMSource::PMSource(): Number of written bytes does not match expected number!");
  }
  #endif
  m_BufStream.buffer((const char*) m_PaddingBuffer, origLen);
  m_BitsRead = 0;
  //BufferStream takes care of pointer, so we set it to NULL here to avoid conflicts
  m_PaddingBuffer = NULL;
}

PMSource::~PMSource()
{
  //empty
}

bool PMSource::getNextMessageBlock(MessageBlock& mBlock)
{
  std::streamsize bytesRead = 0;
  switch (m_Status)
  {
    case psUnpadded:
         m_BufStream.read((char*) &(mBlock.words[0]), 64);
         bytesRead = m_BufStream.gcount();
         if (bytesRead==64)
         {
           m_BitsRead += 512;
           mBlock.reverseBlock();
           return true;
         }
         //not full block read
         m_BitsRead += (bytesRead * 8);
         //close file, we are done here with reading from file anyway
         //m_BufStream.close(); // no close, since this will take care of itself in destructor
         //add 1-bit (start of message padding)
         ((uint8_t*) &(mBlock.words[0]))[bytesRead] = 0x80;
         //zero out rest of message block
         memset(&(((uint8_t*) &(mBlock.words[0]))[bytesRead+1]), 0, 64 - (bytesRead+1));
         //pad size value in there, too, if possible
         if (bytesRead+1<=56)
         {
           #if BYTE_ORDER == LITTLE_ENDIAN
           reverse64(m_BitsRead, m_BitsRead);
           #endif
           memcpy(&(mBlock.words[14]), &m_BitsRead, 8);
           m_Status = psPaddedAndAllRead;
           mBlock.reverseBlock();
           return true;
         }
         m_Status = psPadded1024And512Read;
         mBlock.reverseBlock();
         return true;
         break;
    case psPadded1024And512Read:
         //fill all with zeros (padding)
         memset(&(mBlock.words[0]), 0, 64);
         //pad size value in there, too
         #if BYTE_ORDER == LITTLE_ENDIAN
         reverse64(m_BitsRead, m_BitsRead);
         #endif
         memcpy(&(mBlock.words[14]), &m_BitsRead, 8);
         m_Status = psPaddedAndAllRead;
         mBlock.reverseBlock();
         return true;
         break;
    case psPaddedAndAllRead:
         return false;
         break;
    case psPadded512:
    case psPadded1024:
         //We should never get to this point!
         throw std::logic_error("PMSource::getNextMessageBlock(): Code execution should never get to this point!");
         return false;
         break;
  }//swi
  //We should never get to this point either!
  throw std::logic_error("PMSource::getNextMessageBlock(): Code execution should never get to this point either!");
  return false;
}

} //namespace
