/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014, 2015, 2016  Dirk Stolle

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
#include <limits>
#include <sstream>
#include "PMSource.hpp"
#include "../libstriezel/common/StringUtils.hpp"
#ifndef NO_PM_COMPRESSION
#include "../libstriezel/zlib/CompressionFunctions.hpp"
#endif

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
  while (pos != std::string::npos)
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

std::string::size_type PrivateMessage::getSaveSize() const
{
  const std::string uid_string = uintToString(getFromUserID());
  return getDatestamp().length() + 1
       + getTitle().length() + 1
       + getFromUser().length() + 1
       + uid_string.length() + 1
       + getToUser().length() + 1
       + getMessage().length() + 1;
}

bool PrivateMessage::saveToStream(std::ostream& outputStream) const
{
  //write datestamp
  outputStream.write(datestamp.c_str(), datestamp.length() + 1);
  //write title
  outputStream.write(title.c_str(), title.length() + 1);
  //write fromUser
  outputStream.write(fromUser.c_str(), fromUser.length() + 1);
  //write fromUserID
  const std::string uid_string = uintToString(fromUserID);
  outputStream.write(uid_string.c_str(), uid_string.length() + 1);
  //write toUser
  outputStream.write(toUser.c_str(), toUser.length() + 1);
  //write message text
  outputStream.write(message.c_str(), message.length() + 1);
  return outputStream.good();
}

bool PrivateMessage::saveToFile(const std::string& fileName, const bool compressed) const
{
  if (!compressed)
  {
    std::ofstream output;
    output.open(fileName.c_str(), std::ios_base::out | std::ios_base::binary);
    if (!output)
    {
      return false;
    }
    const bool result = saveToStream(output);
    output.close();
    return result;
  } //if not compressed
  else
  {
    #ifdef NO_PM_COMPRESSION
    #ifdef DEBUG
    std::cout << "Error while saving compressed private message: compression is disabled for this build!\n";
    #endif
    return false;
    #else
    const std::string::size_type bufLen = getSaveSize();
    uint8_t * buffer = NULL;
    try
    {
      buffer = new uint8_t[bufLen];
    }
    catch (...)
    {
      #ifdef DEBUG
      std::cout << "Error while saving private message: could not allocate buffer for compression!\n";
      #endif
      if (buffer != NULL)
        delete [] buffer;
      return false;
    } //catch

    libstriezel::OutBufferStream bufferStream(reinterpret_cast<char*>(buffer), bufLen);
    const bool result = saveToStream(bufferStream);
    if (!result)
    {
      #ifdef DEBUG
      std::cout << "Error while saving private message: could not write data to buffer stream!\n";
      #endif
      delete[] buffer;
      buffer = NULL;
      return false;
    } //if

    uint32_t compSize = 0;
    if (bufLen > std::numeric_limits<uint32_t>::max())
      compSize = std::numeric_limits<uint32_t>::max();
    else
      compSize = bufLen;
    libstriezel::zlib::CompressPointer compressedData = new uint8_t[compSize];
    uint32_t usedSize = 0;
    if (!libstriezel::zlib::compress(buffer, bufLen, compressedData, compSize, usedSize, 9))
    {
      #ifdef DEBUG
      std::cout << "Error while saving compressed message: Compression via zlib failed!\n";
      #endif
      delete[] buffer; buffer = NULL;
      delete[] compressedData; compressedData = NULL;
      return false;
    }
    std::ofstream output;
    output.open(fileName.c_str(), std::ios_base::out | std::ios_base::binary);
    if (!output)
    {
      delete[] buffer; buffer = NULL;
      delete[] compressedData; compressedData = NULL;
      return false;
    }
    //Write original length value, so we can use it to allocate proper buffer size for decompression
    const uint32_t origLen = bufLen;
    output.write(reinterpret_cast<const char*>(&origLen), sizeof(uint32_t));
    //write compressed data
    output.write(reinterpret_cast<const char*>(compressedData), usedSize);
    const bool success = output.good();
    output.close();
    delete[] buffer; buffer = NULL;
    delete[] compressedData; compressedData = NULL;
    return success;
    #endif // end of NO_PM_COMPRESSION is not defined
  } //else (i.e. shall save compressed PM data)
}

bool PrivateMessage::loadFromStream(std::istream& inputStream)
{
  inputStream.seekg(0, std::ios_base::end);
  const std::streamsize len = inputStream.tellg();
  inputStream.seekg(0, std::ios_base::beg);
  if (!inputStream.good())
  {
    #ifdef DEBUG
    std::cout << "Error while reading private message: seek operation(s) failed!\n";
    #endif
    return false;
  }
  //We do not want PMs larger than 1 MB, to avoid excessive memory consumption.
  if (len>1024*1024)
  {
    #ifdef DEBUG
    std::cout << "Error while reading private message: unexpected file size!\n";
    #endif
    return false;
  }
  //assume worst case for buffer size: all file content should fit into it
  char * buffer = new char[len+1];

  //read datestamp
  inputStream.getline(buffer, len, '\0');
  if (!inputStream.good())
  {
    #ifdef DEBUG
    std::cout << "Error while reading private message's datestamp part!\n";
    #endif
    delete[] buffer;
    return false;
  }
  buffer[inputStream.gcount()] = '\0';
  datestamp = std::string(buffer);
  m_NeedsHashUpdate = true;

  //read title
  inputStream.getline(buffer, len, '\0');
  if (!inputStream.good())
  {
    #ifdef DEBUG
    std::cout << "Error while reading private message's title part!\n";
    #endif
    delete[] buffer;
    return false;
  }
  buffer[inputStream.gcount()] = '\0';
  title = std::string(buffer);

  //read fromUser
  inputStream.getline(buffer, len, '\0');
  if (!inputStream.good())
  {
    #ifdef DEBUG
    std::cout << "Error while reading private message's sender part!\n";
    #endif
    delete[] buffer;
    return false;
  }
  buffer[inputStream.gcount()] = '\0';
  fromUser = std::string(buffer);

  //read fromUserID
  inputStream.getline(buffer, len, '\0');
  if (!inputStream.good())
  {
    #ifdef DEBUG
    std::cout << "Error while reading private message's user ID!\n";
    #endif
    delete[] buffer;
    return false;
  }
  buffer[inputStream.gcount()] = '\0';
  const std::string tempStr = std::string(buffer);
  if (!(std::stringstream (tempStr) >> fromUserID))
  {
    #ifdef DEBUG
    std::cout << "Error while converting private message's user ID string to integer!\n";
    #endif
    delete[] buffer;
    return false;
  }

  //read toUser
  inputStream.getline(buffer, len, '\0');
  if (!inputStream.good())
  {
    #ifdef DEBUG
    std::cout << "Error while reading private message's receiver!\n";
    #endif
    delete[] buffer;
    return false;
  }
  buffer[inputStream.gcount()] = '\0';
  toUser = std::string(buffer);

  //read message text
  inputStream.getline(buffer, len, '\0');
  if (!inputStream.good())
  {
    #ifdef DEBUG
    std::cout << "Error while reading private message's text!\n";
    #endif
    delete[] buffer;
    return false;
  }
  buffer[inputStream.gcount()] = '\0';
  message = std::string(buffer);
  m_NeedsHashUpdate = true;
  delete[] buffer; //free previously allocated buffer

  return true;
}

bool PrivateMessage::loadFromFile(const std::string& fileName, const bool isCompressed)
{
  if (isCompressed)
  {
    #ifdef NO_PM_COMPRESSION
    #ifdef DEBUG
    std::cout << "Error while loading compressed private message: (de-)compression is disabled for this build!\n";
    #endif //DEBUG
    return false;
    #else
    std::ifstream input;
    input.open(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
    if (!input)
    {
      return false;
    }

    uint32_t decompressedSize = 0;
    input.read(reinterpret_cast<char*>(&decompressedSize), sizeof(uint32_t));
    if (!input.good() or (input.gcount() != sizeof(uint32_t)))
    {
      input.close();
      #ifdef DEBUG
      std::cout << "Error while reading private message: Could not read size value!\n";
      #endif
      return false;
    }

    /* Check for size to avoid allocating an excessive amount of memory.
       Size should not be zero (empty buffer is useless), and it should not be
       more than 1 MB, which is more than enough for a PM.
    */
    if ((decompressedSize == 0) or (decompressedSize > 1024*1024))
    {
      input.close();
      #ifdef DEBUG
      std::cout << "Error while reading private message: Encountered invalid decompression size value of "
                << decompressedSize << " bytes! Size should be in [1;" << 1024*1024 << "].\n";
      #endif
      return false;
    }

    //get total length of file
    input.seekg(0, std::ios_base::end);
    const std::streamsize len = input.tellg();
    //reset stream pointer to fifth byte (four have already been read)
    input.seekg(sizeof(uint32_t), std::ios_base::beg);
    if (!input.good())
    {
      input.close();
      #ifdef DEBUG
      std::cout << "Error while reading private message: seek operation(s) failed!\n";
      #endif
      return false;
    }

    const std::streamsize compressedBufferSize = len - 4;
    if ((compressedBufferSize > 1024*1024) or (compressedBufferSize <= 0))
    {
      input.close();
      #ifdef DEBUG
      std::cout << "Error while reading private message: Encountered invalid compression buffer size value of "
                << compressedBufferSize << " bytes! Size should be in [1;" << 1024*1024 << "].\n";
      #endif
      return false;
    }

    //allocate buffer for compressed data
    uint8_t * compressedBuffer = new uint8_t[compressedBufferSize];
    //read all data into buffer
    input.read(reinterpret_cast<char*> (compressedBuffer), compressedBufferSize);
    if (!input.good() or (input.gcount() != compressedBufferSize))
    {
      input.close();
      delete[] compressedBuffer;
      #ifdef DEBUG
      std::cout << "Error while reading private message: Could not read all compressed data!\n";
      #endif
      return false;
    }
    //We can close the input stream now, because all data was read from the stream.
    input.close();

    //allocate buffer for decompressed data
    uint8_t * decompressedBuffer = new uint8_t[decompressedSize];

    //decompress all the stuff
    if (!libstriezel::zlib::decompress(compressedBuffer, compressedBufferSize, decompressedBuffer, decompressedSize))
    {
      delete[] compressedBuffer;
      delete[] decompressedBuffer;
      #ifdef DEBUG
      std::cout << "Error while reading private message: Decompression failed!\n";
      #endif
      return false;
    } //if decompress failed

    //Compression succeeded, we can delete compressed buffer.
    delete[] compressedBuffer;
    compressedBuffer = NULL;

    //create buffer stream
    libstriezel::InBufferStream bufferStream(reinterpret_cast<const char*>(decompressedBuffer), decompressedSize);
    const bool success = loadFromStream(bufferStream);
    bufferStream.buffer(NULL, 0);
    delete[] decompressedBuffer;
    decompressedBuffer = NULL;

    return success;
    #endif // NO_PM_COMPRESSION is not defined
  } //if compressed
  else
  {
    std::ifstream input;
    input.open(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
    if (!input)
    {
      return false;
    }
    const bool success = loadFromStream(input);
    input.close();
    return success;
  } //else (i.e. uncompressed)
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
