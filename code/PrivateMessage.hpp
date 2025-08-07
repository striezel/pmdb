/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014, 2015, 2025  Dirk Stolle

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

#ifndef PRIVATEMESSAGE_HPP
#define PRIVATEMESSAGE_HPP

#include <cstdint>
#include <string>
#include "Compression.hpp"
#include "../libstriezel/hash/sha256/sha256.hpp"

/** Holds information about a private message. */
class PrivateMessage
{
  public:
    PrivateMessage();


    /** Normalises the line breaks in the message text. */
    void normalise();


    /** \brief Gets PM's datestamp.
     *
     * \return datestamp (string)
     */
    inline const std::string& getDatestamp() const
    {
      return datestamp;
    }


    /** \brief Gets title of the PM.
     *
     * \return PM title
     */
    inline const std::string& getTitle() const
    {
      return title;
    }


    /** \brief Gets the name of the person who sent the PM.
     *
     * \return name of the sender
     */
    inline const std::string& getFromUser() const
    {
      return fromUser;
    }


    /** \brief Gets the user ID of the person who sent the PM.
     *
     * \return Returns user ID of the sender.
     *         A value of zero indicates that no value has been set.
     */
    inline uint32_t getFromUserID() const
    {
      return fromUserID;
    }


    /** \brief Gets the name of the person who received the PM.
     *
     * \return name of the receiver
     */
    inline const std::string& getToUser() const
    {
      return toUser;
    }


    /** \brief Gets the text of the PM.
     *
     * \return message text
     */
    inline const std::string& getMessage() const
    {
      return message;
    }


    /** \brief Calculates the SHA-256 hash of the PM.
     *
     * \return Returns SHA-256 hash of the PM.
     */
    const SHA256::MessageDigest& getHash();


    /** \brief Sets new datestamp.
     *
     * \param ds  new value
     */
    void setDatestamp(const std::string& ds);


    /** \brief Sets new PM title.
     *
     * \param t  the new title
     */
    void setTitle(const std::string& t);


    /** \brief Sets new sender name.
     *
     * \param from  the new sender's name
     */
    void setFromUser(const std::string& from);


    /** \brief Sets new user ID of sender.
     *
     * \param uid  the new user ID
     */
    void setFromUserID(const uint32_t uid);


    /** \brief Sets new name of the receiver.
     *
     * \param to  the new name
     */
    void setToUser(const std::string& to);


    /** \brief Sets new PM message text.
     *
     * \param msg  the new message text
     */
    void setMessage(const std::string& msg);


    /** \brief Determines the size of this PM, if it were saved to a file.
     *
     * \return Returns size of uncompressed saved PM in bytes.
     */
    std::string::size_type getSaveSize() const;


    /** \brief Tries to save the message to the given file.
     *
     * \param fileName  the file that shall be used to save the message
     * \param compression   if set to Compression::zlib, the saved file will be compressed with zlib
     * \return Returns true in case of success, or false if an error occurred.
     */
    bool saveToFile(const std::string& fileName, const Compression compression) const;


    /** \brief Tries to load the message from the given file.
     *
     * \param fileName file that shall be used to load the message
     * \param compression  Set this to Compression::zlib to indicate that the file contains a zlib-compressed PM.
     * \return Returns true in case of success, or false if an error occurred.
     */
    bool loadFromFile(const std::string& fileName, const Compression compression);


    /** \brief equality operator for PrivateMessage class
     *
     * \param other   the other PrivateMessage instance
     * \return Returns true, if both instances have the same content.
     *         Hashes or hash update state are NOT considered.
     */
    bool operator==(const PrivateMessage& other) const;


    /** \brief inequality operator for PrivateMessage class
     *
     * \param other   the other PrivateMessage instance
     * \return Returns true, if both instances have different content.
     *         Hashes or hash update state are NOT considered.
     */
    bool operator!=(const PrivateMessage& other) const;
  private:
    /** \brief Tries to save the PM contents to an output stream.
     *
     * \param outputStream  stream that shall be used to save the PM;
     *                      the stream shall be opened and ready for writing
     *                      data to it when passed to the function.
     * \return Returns true, if the data was written to the stream.
     *         Returns false, if the write operation failed.
     */
    bool saveToStream(std::ostream& outputStream) const;


    /** \brief Tries to load the PM contents from an input stream.
     *
     * \param inputStream  stream that shall be used to load the PM;
     *                     the stream shall be opened and ready for reading
     *                     data from it when passed to the function.
     * \return Returns true, if the data was loaded from the stream.
     *         Returns false, if the read operation failed.
     */
    bool loadFromStream(std::istream& inputStream);


    std::string datestamp;  /**< date and time the PM was sent */
    std::string title;  /**< title of the PM */
    std::string fromUser;  /**< name of the sender */
    uint32_t fromUserID;  /**< numeric ID of the sender */
    std::string toUser;  /**< name of the recipients  */
    std::string message;  /**< the message text */
    bool m_NeedsHashUpdate;  /**< tracks whether hash is not up to date */
    SHA256::MessageDigest m_Hash;  /**< hash of the message */
}; // class

#endif // PRIVATEMESSAGE_HPP
