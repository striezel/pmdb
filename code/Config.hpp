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

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#ifndef NO_SMILIES_IN_PARSER
#include "bbcode/Smilie.hpp"
#endif

/** \brief class that holds the configuration settings
 */
class Config
{
  public:
    /// constructor
    Config();


    /** \brief returns the forum's base URL
     *
     * \return returns the forum's base URL. Returns an empty string,
     *         if no URL was set/read yet.
     */
    const std::string& getForumURL() const;


    /** \brief sets the forum's base URL
     *
     * \param newURL the new URL
     */
    void setForumURL(const std::string& newURL);


    /** \brief path of the message template file
     *
     * \return returns the path of the message template file
     */
    const std::string& getTPL() const;


    /** \brief sets the message template file path
     *
     * \param newTPL the new message template file path
     */
    void setTPLFile(const std::string& newTPL);


    #ifndef NO_SMILIES_IN_PARSER
    /** \brief retrieve vector of smilie configs
     *
     * \return Returns the list of smilie entries found in the configuration file.
     */
    const std::vector<Smilie>& getSmilies() const;
    #endif


    /** \brief clears/resets existing values
     */
    void clear();


    /** \brief tries to read the configuration file from the given file name.
     *
     * \param fileName path to the configuration file
     * \return Returns true in case of success, or false if an error occurred.
     *
     */
    bool loadFromFile(const std::string& fileName);
  private:
    std::string forumURL;
    std::string tplFile;
    #ifndef NO_SMILIES_IN_PARSER
    std::vector<Smilie> smilies;
    #endif
}; //class

#endif // CONFIG_HPP
