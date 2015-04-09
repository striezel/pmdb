/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database test suite.
    Copyright (C) 2015  Thoronador

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

#include <iostream>
#include <utility>
#include <vector>
#include "../../../libthoro/encoding/StringConversion.hpp"
#include "../../../libthoro/filesystem/DirectoryFunctions.hpp"
#include "../../../code/MessageDatabase.hpp"

int main(int argc, char **argv)
{
  if ((argc < 2) || (argv == NULL))
  {
    std::cout << "Error: Missing argument!\n";
    return 1;
  }
  if (argv[1] == NULL)
  {
    std::cout << "Error: argv[1] is NULL!\n";
    return 1;
  }
  //get base path for XML test files from command line arguments
  const std::string baseXmlPath = libthoro::filesystem::slashify(std::string(argv[1]));
  if (baseXmlPath.empty() || (baseXmlPath == "/") || (baseXmlPath == "\\"))
  {
    std::cout << "Error: Invalid base XML path!\n";
    return 1;
  }

  //scope one for "single_message.xml"
  {
    MessageDatabase msgdb;
    FolderMap fm;
    uint32_t readPMs = 0;
    uint32_t newPMs = 0;

    try
    {
      if (!msgdb.importFromFile(baseXmlPath+"single_message.xml", readPMs, newPMs, fm))
      {
        std::cout << "Error: Could not load message from " + baseXmlPath+"single_message.xml!\n";
        return 1;
      }
    }
    catch (...)
    {
      std::cout << "Error: Caught exception while trying to import message from XML file!\n";
      return 1;
    }

    //check, if we read exactly one PM
    if ((readPMs != 1) || (newPMs != 1))
    {
      std::cout << "Error: Numbers of read/new PMs should be one each, but they are not!\n"
                << "Read: " << readPMs << "; new: " << newPMs << "\n";
      return 1;
    }
    //check, if DB reports proper number of messages
    if (msgdb.getNumberOfMessages() != 1)
    {
      std::cout << "Error: Number of messages should be one, but it is "
                << msgdb.getNumberOfMessages() << " instead!\n";
      return 1;
    }

    MessageDatabase::Iterator iter = msgdb.getBegin();
    const SHA256::MessageDigest sha256Hash = iter->first;
    std::cout << "Message hash of single_message.xml is " << sha256Hash.toHexString() << ".\n";
    PrivateMessage pm;
    try
    {
      pm = msgdb.getMessage(sha256Hash);
    }
    catch (...)
    {
      std::cout << "Error: Could not get private message from database!\n";
      return 1;
    }
    //check contents
    // ---- datestamp
    if (pm.getDatestamp() != "2007-06-14 12:34")
    {
      std::cout << "Error: Datestamp should be 2007-06-14 12:34 but is \""
                << pm.getDatestamp() << "\" instead.\n";
      return 1;
    }
    // ---- title
    if (pm.getTitle() != "This is the title")
    {
      std::cout << "Error: Title should be \"This is the title\" but is \""
                << pm.getTitle() << "\" instead.\n";
      return 1;
    }
    // ---- sender
    if (pm.getFromUser() != "Hermes")
    {
      std::cout << "Error: Sender should be \"Hermes\" but is \""
                << pm.getFromUser() << "\" instead.\n";
      return 1;
    }
    // ---- from user ID
    if (pm.getFromUserID() != 234)
    {
      std::cout << "Error: Sender ID should be 234 but is "
                << pm.getFromUserID() << " instead.\n";
      return 1;
    }
    // ---- to user
    if (pm.getToUser() != "Poseidon")
    {
      std::cout << "Error: Receiver should be \"Poseidon\" but is \""
                << pm.getToUser() << "\" instead.\n";
      return 1;
    }
    // ---- message
    if (pm.getMessage() != "This is a message.")
    {
      std::cout << "Error: Message should be \"This is a message.\" but is \""
                << pm.getMessage() << "\" instead.\n";
      return 1;
    }

    //check folder association
    // ---- presence check
    if (!fm.hasEntry(sha256Hash))
    {
      std::cout << "Error: Folder map has no entry for " << sha256Hash.toHexString() << "!\n";
      return 1;
    }
    // ---- folder name check
    if (fm.getFolderName(sha256Hash) != std::string("Sent"))
    {
      std::cout << "Error: Folder for " << sha256Hash.toHexString()
                << " should be \"Sent\", but it is \""
                << fm.getFolderName(sha256Hash) << "\" instead !\n";
      return 1;
    }
  } //end of scope for "single_message.xml"


  std::vector<std::pair<std::string, std::string> > charsets;
  charsets.push_back(std::make_pair("charset-iso-8859-1.xml", "ISO-8859-1"));
  charsets.push_back(std::make_pair("charset-ucs-2.xml",      "UCS-2"));
  charsets.push_back(std::make_pair("charset-ucs-4.xml",      "UCS-4"));
  charsets.push_back(std::make_pair("charset-utf-8.xml",      "UTF-8"));
  charsets.push_back(std::make_pair("charset-utf-16.xml",     "UTF-16"));
  charsets.push_back(std::make_pair("charset-utf-16-BE.xml",  "UTF-16 big endian"));
  charsets.push_back(std::make_pair("charset-utf-16-LE.xml",  "UTF-16 little endian"));

  //range-based for loop -> requires C++11
  for (const std::pair<std::string, std::string>& charsetPair: charsets)
  {
    /* Try to read a message that contains a variety of ISO-88559-1/UTF-8/UTF-16 characters. */
    MessageDatabase msgdb;
    FolderMap fm;
    uint32_t readPMs = 0;
    uint32_t newPMs = 0;

    try
    {
      if (!msgdb.importFromFile(baseXmlPath+charsetPair.first, readPMs, newPMs, fm))
      {
        std::cout << "Error: Could not load message from " + baseXmlPath+charsetPair.first+"!\n";
        return 1;
      }
    }
    catch (...)
    {
      std::cout << "Error: Caught exception while trying to import message from "+charsetPair.first+"!\n";
      return 1;
    }
    //check, if we read exactly one PM
    if ((readPMs != 1) || (newPMs != 1))
    {
      std::cout << "Error: Numbers of read/new PMs should be one each, but they are not!\n"
                << "Read: " << readPMs << "; new: " << newPMs << "\n";
      return 1;
    }
    //check, if DB reports proper number of messages
    if (msgdb.getNumberOfMessages() != 1)
    {
      std::cout << "Error: Number of messages should be one, but it is "
                << msgdb.getNumberOfMessages() << " instead!\n";
      return 1;
    }

    MessageDatabase::Iterator iter = msgdb.getBegin();
    const SHA256::MessageDigest sha256Hash = iter->first;
    std::cout << "Message hash of "+charsetPair.first+" is " << sha256Hash.toHexString() << ".\n";
    PrivateMessage pm;
    try
    {
      pm = msgdb.getMessage(sha256Hash);
    }
    catch (...)
    {
      std::cout << "Error: Could not get private message from database!\n";
      return 1;
    }
    //check contents
    // ---- Note: datestamp, title, sender, from user ID, to user get omitted,
    //      they are the same as in the previous file and should therefore be
    //      read without problems.
    // ---- message

    const std::string iso8859_1_message
        = std::string("This is a message.\n")
          + " !\"#$%&'()*+,-./\n"
          + "0123456789:;<=>?\n"
          + "@ABCDEFGHIJKLMNO\n"
          + "PQRSTUVWXYZ[\\]^_\n"
          + "`abcdefghijklmno\n"
          + "pqrstuvwxyz{|}~\n"
          + "\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\n"  // " ¡¢£¤¥¦§¨©ª«¬­®¯\n"
          + "\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\n"  // "°±²³´µ¶·¸¹º»¼½¾¿\n"
          + "\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\n"  // "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏ\n"
          + "\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf\n"  // "ÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞß\n"
          + "\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\n"  // "àáâãäåæçèéêëìíîï\n"
          + "\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff\n"; // "ðñòóôõö÷øùúûüýþÿ\n"

    std::string message("");
    //convert message
    if (!libthoro::encoding::utf8_to_iso8859_1(pm.getMessage(), message))
    {
      std::cout << "Conversion failed!\n";
      std::cout << "Message is now \""<<message<<"\".\n";
      return 1;
    }

    //compare converted message
    if (message != iso8859_1_message)
    {
      std::cout << "Error: Message should be \"" << iso8859_1_message
                << "\" but is \"" << message << "\" instead.\n";
      return 1;
    }
    std::cout << "Info: " << charsetPair.second << " message is:\n" << message
              << "\n---- END OF MESSAGE ----\n";
  } //end range-bases for ("foreach")

  std::cout << "Passed simple message database file import test!\n";
  return 0;
}
