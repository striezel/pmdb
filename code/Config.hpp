/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014, 2025  Dirk Stolle

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

/** \brief Holds the configuration settings, mainly for HTML generation.
 */
class Config
{
  public:
    Config();


    /** \brief Returns the forum's base URL.
     *
     * \return Returns the forum's base URL. Returns an empty string,
     *         if no URL was set/read yet.
     */
    const std::string& getForumURL() const;


    /** \brief Sets the forum's base URL.
     *
     * \param newURL the new URL
     */
    void setForumURL(const std::string& newURL);


    #ifndef NO_SMILIES_IN_PARSER
    /** \brief Retrieve smilie entries.
     *
     * \return Returns the list of smilie entries found in the configuration file.
     */
    const std::vector<Smilie>& getSmilies() const;
    #endif


    /** \brief Clears/resets existing values.
     */
    void clear();


    /** \brief Tries to read the configuration file from the given file name.
     *
     * \param fileName path to the configuration file
     * \return Returns true in case of success, or false if an error occurred.
     */
    bool loadFromFile(const std::string& fileName);
  private:
    std::string forumURL;
    #ifndef NO_SMILIES_IN_PARSER
    std::vector<Smilie> smilies;
    #endif
}; // class

#endif // CONFIG_HPP
