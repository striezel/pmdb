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

#ifndef COLOURMAP_HPP
#define COLOURMAP_HPP

#include <string>
#include <map>

/** \brief Holds the mappings of folder names to colours.
 */
class ColourMap
{
  public:
    /// Creates an empty colour map.
    ColourMap();


    /** \brief Tries to read the colour map from the given file name.
     *
     * \param fileName  path to the colour map file
     * \return Returns true in case of success, or false if an error occurred.
     */
    bool loadFromFile(const std::string& fileName);


    /** \brief Add an entry to the colour map (overwrites existing entry of same name).
     *
     * \param folder name of the folder
     * \param sequence  control sequence for the desired colour
     */
    void add(const std::string& folder, const std::string& sequence);


    /** \brief Gets the folder name in corresponding colour (i.e. with control sequences).
     *
     * \param folder name of the folder
     * \return Returns string with control sequences, where appropriate.
     */
    std::string colouredFolder(const std::string& folder) const;


    /** \brief character that starts comments line in colour map file */
    static const char cCommentCharacter;
  private:
    std::map<std::string, std::string> colours;
}; //class

#endif // COLOURMAP_HPP
