/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2013, 2014  Thoronador

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

#ifndef MESSAGEDATABASE_HPP
#define MESSAGEDATABASE_HPP

#include <map>
#include <vector>
#include "PrivateMessage.hpp"
#include "MsgTemplate.hpp"
#include "FolderMap.hpp"
#include "SortType.hpp"

//forward declaration of XMLNode
class XMLNode;

class MessageDatabase
{
  public:
    /* constructor */
    MessageDatabase();


    /** \brief adds a message to the database
     *
     * \param pm the message that shall be added
     * \return Returns true, if the message was added.
     *         If the message already exists in the database, the function will
     *         return false and leave the DB unchanged.
     */
    bool addMessage(PrivateMessage& pm);


    /** \brief returns the number of messages that are in the database
     *
     * \return returns the number of messages that are in the database
     */
    unsigned int getNumberOfMessages() const;


    /** \brief determines whether a given message is in the database.
     *
     * \param pm the message
     * \return Returns true, if the given message is in the database.
     *         Returns false otherwise.
     */
    bool hasMessage(PrivateMessage& pm) const;


    /** \brief Returns a single message from the database.
     *
     * \param digest the SHA-256 hash of the required message
     * \return Returns the message with the given digest, if it exists.
     *         Throws exception otherwise.
     */
    const PrivateMessage& getMessage(const SHA256::MessageDigest& digest) const;


    /** \brief tries to import messages from a XML file
     *
     * \param fileName path to the XML file
     * \param readPMs  will hold the number of PMs that were read from the file
     * \param newPMs   will hold the number of new PMs that were stored in the DB
     * \param fm       reference to the folder map that will save folder information
     * \return Returns true in case of success, false in case of error.
     *         In either case, readPMs will hold the number of messages that
     *         were successfully read from the file.
     */
    bool importFromFile(const std::string& fileName, uint32_t& readPMs, uint32_t& newPMs, FolderMap& fm);


    typedef std::map<SHA256::MessageDigest, PrivateMessage>::const_iterator Iterator;


    /** \brief return iterator to the start of the DB's element list */
    Iterator getBegin() const;

    /** \brief return iterator to the end of the DB's PM list */
    Iterator getEnd()   const;


    /** \brief tries to save all messages in the database to the given directory
     *
     * \param directory directory where the messages shall be saved
     * \return Returns true in case of success, or false otherwise.
     */
    bool saveMessages(const std::string& directory) const;


    /** \brief tries to load all messages in the given directory into the database
     *
     * \param directory the directory from which the messages shall be loaded
     * \param readPMs   will hold the number of PMs that were read from the file
     * \param newPMs    will hold the number of new PMs that were stored in the DB
     * \return Returns true in case of success, or false otherwise.
     */
    bool loadMessages(const std::string& directory, uint32_t& readPMs, uint32_t& newPMs);


    /** \brief creates index files (HTML) for all message folders
     *
     * \param directory   path to the directory where the files are saved
     * \param index       template for index file
     * \param entry       template for message entry
     * \param folderList  template for the folder list
     * \param folderEntry template for a folder entry in the list
     * \param fm          the current folder map
     * \return Returns true, if file was created successfully.
     */
    bool saveIndexFiles(const std::string& directory, MsgTemplate index, MsgTemplate entry, MsgTemplate folderList, MsgTemplate folderEntry, const FolderMap& fm) const;


    /** \brief finds messages whose texts "overlap"
     *
     * \return Returns a map where a hash-datestamp struct for a message is
     *         mapped to a vector of hash-datestamp structs of messages whose
     *         texts are contained in the key message.
     */
    std::map<md_date, std::vector<md_date> > getTextSubsets() const;
  private:
    /** \brief processes a <folder> XML node
     *
     * \param node    the XML node
     * \param readPMs will hold the number of PMs that were read from the node
     * \param newPMs  will hold the number of new PMs that were stored in the DB
     * \param fm      FolderMap that will be used to store folder information
     * \return Returns true, if the node could be processed successfully.
     *         Returns false, if an error occurred.
     */
    bool processFolderNode(const XMLNode& node, uint32_t& readPMs, uint32_t& newPMs, FolderMap& fm);


    /** \brief processes a <privatemessage> XML node
     *
     * \param node    the XML node
     * \param readPMs will hold the number of PMs that were read from the node
     * \param newPMs  will hold the number of new PMs that were stored in the DB
     * \param folder  name of the containing folder (or empty for no folder)
     * \param fm      FolderMap that will be used to store folder information
     * \return Returns true, if the node was processed successfully.
     *         Returns false, if an error occurred.
     */
    bool processPrivateMessageNode(const XMLNode& node, uint32_t& readPMs, uint32_t& newPMs, const std::string& folder, FolderMap& fm);

    std::map<SHA256::MessageDigest, PrivateMessage> m_Messages; /**< map that holds the messages */
};//class

#endif // MESSAGEDATABASE_HPP
