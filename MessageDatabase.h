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
  private:
    bool processFolderNode(const XMLNode& node, uint32_t& readPMs, uint32_t& newPMs);
    bool processPrivateMessageNode(const XMLNode& node, uint32_t& readPMs, uint32_t& newPMs);

    std::map<SHA256::MessageDigest, PrivateMessage> m_Messages;
};//class

#endif // MESSAGEDATABASE_H
