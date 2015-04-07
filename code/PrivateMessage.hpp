/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014, 2015  Thoronador

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

#include <string>
#include <stdint.h>
#include "../libthoro/hash/sha-256.h"

/** holds information about a private message */
class PrivateMessage
{
  public:
    /** constructor */
    PrivateMessage();


    /** clears/resets all data members */
    void clear();


    /** normalises the line breaks in the message text */
    void normalise();


    //access to data members
    /** \brief access PM's datestamp
     *
     * \return datestamp (string)
     */
    inline const std::string& getDatestamp() const
    {
      return datestamp;
    }


    /** \brief access current title of the PM
     *
     * \return PM title
     */
    inline const std::string& getTitle() const
    {
      return title;
    }


    /** \brief provides access to the name of the person who sent the PM
     *
     * \return name of the sender
     */
    inline const std::string& getFromUser() const
    {
      return fromUser;
    }


    /** \brief provides access to the user ID of the person who sent the PM
     *
     * \return Returns user ID of the sender.
     *         A value of zero indicates that no value has been set.
     */
    inline uint32_t getFromUserID() const
    {
      return fromUserID;
    }


    /** \brief provides access to the name of the person who received the PM
     *
     * \return name of the receiver
     */
    inline const std::string& getToUser() const
    {
      return toUser;
    }


    /** \brief provides access to the text of the PM
     *
     * \return message text
     */
    inline const std::string& getMessage() const
    {
      return message;
    }


    /** \brief calculates the SHA-256 hash of the PM
     *
     * \return returns SHA-256 hash of the PM
     */
    const SHA256::MessageDigest& getHash();


    //functions to set various data members
    /** \brief set new datestamp
     *
     * \param ds  new value
     */
    void setDatestamp(const std::string& ds);


    /** \brief set new PM title
     *
     * \param t  the new title
     */
    void setTitle(const std::string& t);


    /** \brief set new sender name
     *
     * \param from  the new sender's name
     */
    void setFromUser(const std::string& from);


    /** \brief set new user ID of sender
     *
     * \param uid  the new user ID
     */
    void setFromUserID(const uint32_t uid);


    /** \brief set new name of the receiver
     *
     * \param to  the new name
     */
    void setToUser(const std::string& to);


    /** \brief set new PM message text
     *
     * \param msg  the new message text
     */
    void setMessage(const std::string& msg);


    /** \brief tries to save the message to the given file
     *
     * \param fileName the file that shall be used to save the message
     * \return Returns true in case of success, or false if an error occured.
     */
    bool saveToFile(const std::string& fileName) const;


    /** \brief tries to load the message from the given file
     *
     * \param fileName file that shall be used to load the message
     * \return Returns true in case of success, or false if an error occured.
     */
    bool loadFromFile(const std::string& fileName);
  private:
    std::string datestamp;
    std::string title;
    std::string fromUser;
    uint32_t fromUserID;
    std::string toUser;
    std::string message;
    bool m_NeedsHashUpdate;
    SHA256::MessageDigest m_Hash;
};//class

#endif // PRIVATEMESSAGE_HPP
