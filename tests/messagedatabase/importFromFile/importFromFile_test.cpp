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
#include "../../../libthoro/filesystem/DirectoryFunctions.hpp"
#include "../../../code/Encoding.hpp"
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


  //scope one for "charset-iso-8859-1.xml"
  {
    /* Try to read a message that contains a variety of ISO-88559-1 characters
       with ordinal values above 127. */
    MessageDatabase msgdb;
    FolderMap fm;
    uint32_t readPMs = 0;
    uint32_t newPMs = 0;

    try
    {
      if (!msgdb.importFromFile(baseXmlPath+"charset-iso-8859-1.xml", readPMs, newPMs, fm))
      {
        std::cout << "Error: Could not load message from " + baseXmlPath+"charset-iso-8859-1.xml!\n";
        return 1;
      }
    }
    catch (...)
    {
      std::cout << "Error: Caught exception while trying to import message from charset-iso-8859-1.xml!\n";
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
    std::cout << "Message hash of charset-iso-8859-1.xml is " << sha256Hash.toHexString() << ".\n";
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

    const std::string iso8859_1_message = std::string("This is a message.\n")
                                        + " !\"#$%&'()*+,-./\n"
                                        + "0123456789:;<=>?\n"
                                        + "@ABCDEFGHIJKLMNO\n"
                                        + "PQRSTUVWXYZ[\\]^_\n"
                                        + "`abcdefghijklmno\n"
                                        + "pqrstuvwxyz{|}~\n"
                                        + " ¡¢£¤¥¦§¨©ª«¬­®¯\n"
                                        + "°±²³´µ¶·¸¹º»¼½¾¿\n"
                                        + "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏ\n"
                                        + "ĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß\n"
                                        + "àáâãäåæçèéêëìíîï\n"
                                        + "ğñòóôõö÷øùúûüışÿ\n";

    std::string message("");
    //convert message
    if (!Encoding::UTF8_to_ISO_8859_1(pm.getMessage(), message))
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
    std::cout << "Info: ISO-8859-1 message is:\n" << message
              << "\n---- END OF MESSAGE ----\n";
  } //end of scope for "charset-iso-8859-1.xml"

  std::cout << "Passed simple message database file import test!\n";
  return 0;
}
