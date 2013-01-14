/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2013  Thoronador

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
#include "MsgTemplate.h"
#include <map>

//forward declaration
class XMLNode;

class MessageDatabase
{
  public:
    /* constructor */
    MessageDatabase();

    /* adds a message to the database and returns true, if the message was
       added. If the message already exists in the database, the function will
       return false and leave the DB unchanged.
    */
    bool addMessage(PrivateMessage& pm);

    /* returns the number of messages that are in the database */
    unsigned int getNumberOfMessages() const;

    /* returns true, if the given message is in the database */
    bool hasMessage(PrivateMessage& pm) const;

    /* returns the message with the given digest, if it exists. Throws exception
       otherwise.
    */
    const PrivateMessage& getMessage(const SHA256::MessageDigest& digest) const;

    /* returns true, if there is a folder entry for a PM with the given digest

       parameters:
           pm_digest - SHA256 hash of the message
    */
    bool hasFolderEntry(const SHA256::MessageDigest& pm_digest) const;

    /* returns the name of the folder wherein the message with the given digest
       resides, if it has a folder entry. Throws exception otherwise.
    */
    const std::string& getFolderName(const SHA256::MessageDigest& pm_digest) const;

    /* tries to import messages from a XML file and returns true in case of
       success, false in case of error. In either case, readPMs will hold the
       number of messages that were successfully read from the file.

       parameters:
           fileName - path to the XML file
           readPMs  - will hold the number of PMs that were read from the file
           newPMs   - will hold the number of new PMs that were stored in the DB
    */
    bool importFromFile(const std::string& fileName, uint32_t& readPMs, uint32_t& newPMs);

    typedef std::map<SHA256::MessageDigest, PrivateMessage>::const_iterator Iterator;

    /* return iterator to the start of the DB's element list */
    Iterator getBegin() const;

    /* return iterator to the end of the DB's PM list */
    Iterator getEnd()   const;

    /* tries to save all messages in the database to the given directory and
       returns true in case of success
    */
    bool saveMessages(const std::string& directory) const;

    /* tries to load all messages in the given directory into the database and
       returns true in case of success
    */
    bool loadMessages(const std::string& directory, uint32_t& readPMs, uint32_t& newPMs);

    /* tries to save the folder map in the given directory and returns true in
       case of success
    */
    bool saveFolderMap(const std::string& directory) const;

    /* tries to load the folder map from the given directory and returns true
       in case of success
    */
    bool loadFolderMap(const std::string& directory);

    /* creates an index file (HTML) for all messages

       parameters:
           fileName - path to the file
    */
    bool saveIndexFile(const std::string& fileName, MsgTemplate index, MsgTemplate entry) const;
  private:
    bool processFolderNode(const XMLNode& node, uint32_t& readPMs, uint32_t& newPMs);
    bool processPrivateMessageNode(const XMLNode& node, uint32_t& readPMs, uint32_t& newPMs, const std::string& folder);

    //static std::string escapeFolderName(std::string fName);
    //static std::string unescapeFolderName(std::string rawName);

    std::map<SHA256::MessageDigest, PrivateMessage> m_Messages;
    std::map<SHA256::MessageDigest, std::string> m_FolderMap;
};//class

#endif // MESSAGEDATABASE_H
