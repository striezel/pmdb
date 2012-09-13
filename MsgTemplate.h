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

#ifndef MSGTEMPLATE_H
#define MSGTEMPLATE_H

#include <string>
#include <map>

class MsgTemplate
{
  public:
    /* constructor */
    MsgTemplate();

    /* destructor */
    ~MsgTemplate();

    /* loads template from a given file */
    bool loadFromFile(const std::string& fileName);

    void addReplacement(const std::string& tag, const std::string& replacement);

    void clearReplacements();

    std::string show() const;
  private:
    std::string prepareReplacement(std::string content);
    std::map<std::string, std::string> m_Tags;
    std::string m_Template;
}; //class

#endif // MSGTEMPLATE_H
