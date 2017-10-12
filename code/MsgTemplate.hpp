/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014, 2017  Dirk Stolle

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

#ifndef MSGTEMPLATE_HPP
#define MSGTEMPLATE_HPP

#include <string>
#include <map>


/** class for templates */
class MsgTemplate
{
  public:
    /** Default constructor. */
    MsgTemplate();


    /** \brief Constructor with initial value for template text.
     *
     * \param tplText the template text
     */
    explicit MsgTemplate(const std::string& tplText);


    /** destructor */
    ~MsgTemplate();


    /** \brief Loads template from a given file.
     *
     * \param fileName   path to the template file
     * \return Returns true in case of success.
     */
    bool loadFromFile(const std::string& fileName);


    /** \brief Loads template from a given string.
     *
     * \param tplText   the template text
     * \return Returns true in case of success.
     */
    bool loadFromString(const std::string& tplText);


    /** \brief Adds a new replacement for a certain tag.
     *
     * \param tag           name of the tag that shall be replaced
     * \param replacement   replacement for the given tag
     * \param killHTML      if set to true, HTML code will be escaped in the
     *                      replacement string
     * \remarks If a tag with the given name already exists, it will be
     * overwritten.
     */
    void addReplacement(const std::string& tag, const std::string& replacement, const bool killHTML);


    /** Clears the replacement list. */
    void clearReplacements();


    /** Gets a string that contains the final template with all replacement
     * tags replaced by their proper content.
     *
     * \return template with all tags replaced by proper content
     */
    std::string show() const;
  private:
    /** \brief Prepares a replacement string by escaping HTML code (if specified
     * via @killHTML) and some other character sequences with special meaning.
     *
     * \param content    replacement string
     * \param killHTML   if set to true, HTML code will be escaped in the
     *                   replacement string
     * \return prepared version of @content
     */
    std::string prepareReplacement(std::string content, const bool killHTML);


    std::map<std::string, std::string> m_Tags; /**< tag map; key = name, value = replacement */
    std::string m_Template; /**< template text */
}; //class

#endif // MSGTEMPLATE_HPP
