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

#ifndef MSGTEMPLATE_HPP
#define MSGTEMPLATE_HPP

#include <string>
#include <map>


/** class for templates */
class MsgTemplate
{
  public:
    /** constructor */
    MsgTemplate();

    explicit MsgTemplate(const std::string& str1)
    : m_Tags(std::map<std::string, std::string>()), m_Template(str1)
    { }


    /** destructor */
    ~MsgTemplate();


    /** \brief loads template from a given file
     *
     * \param fileName   path to the template file
     * \return Returns true in case of success.
     */
    bool loadFromFile(const std::string& fileName);


    /** \brief "loads" template from a given string
     *
     * \param tplText   the template text
     * \return Returns true in case of success.
     */
    bool loadFromString(const std::string& tplText);


    /** \brief adds a new replacement for a certain tag
     *
     * \param tag           name of the tag that shall be replaced
     * \param replacement   replacement for the given tag
     * \param killHTML      if set to true, HTML code will be escaped in the
     *                      replacement string
     */
    void addReplacement(const std::string& tag, const std::string& replacement, const bool killHTML);


    /** clears the replacement list */
    void clearReplacements();


    /** returns a string that contains the final template with all replacement
       tags replaced by their proper content
    */
    std::string show() const;
  private:
    std::string prepareReplacement(std::string content, const bool killHTML);
    std::map<std::string, std::string> m_Tags;
    std::string m_Template;
}; //class

#endif // MSGTEMPLATE_HPP
