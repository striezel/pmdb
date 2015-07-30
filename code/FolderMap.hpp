/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2014  Thoronador

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

#ifndef FOLDERMAP_HPP
#define FOLDERMAP_HPP

#include <map>
#include <set>
#include <string>
#include "../libthoro/hash/sha256/sha256.hpp"

/** \brief FolderMap - class that maps private messages (represented by their hash) to their corresponding folder
 */
class FolderMap
{
  public:
    ///constructor
    FolderMap();


    /** \brief adds an entry to the folder map
     *
     * \param pm_digest SHA256 hash of the message
     * \param folder name of the folder that the message is placed in
     */
    void add(const SHA256::MessageDigest& pm_digest, const std::string& folder);

    /** \brief determines whether there is a folder entry for the PM with the given digest
     *
     * \param pm_digest SHA256 hash of the message
     * \return Returns true, if there is a folder entry for a PM with the given digest.
     *         Returns false otherwise.
     */
    bool hasEntry(const SHA256::MessageDigest& pm_digest) const;


    /** \brief returns the name of the folder wherein the message with the given
     *  digest resides, if it has a folder entry. Throws exception otherwise.
     *
     * \param pm_digest hash of the private message
     * \return Returns name of the folder, if message is listed in map.
     *         Throws exception otherwise.
     */
    const std::string& getFolderName(const SHA256::MessageDigest& pm_digest) const;


    /** \brief tries to save the folder map in the given directory
     *
     * \param directory directory from which the folder map shall be loaded
     * \return returns true in case of success, or false if function failed
     */
    bool save(const std::string& directory) const;


    /** \brief tries to load the folder map from the given directory
     *
     * \param directory directory from which the folder map shall be loaded
     * \return returns true in case of success, or false if function failed
     */
    bool load(const std::string& directory);


    /** \brief get the names of the distinct folders in the folder map / DB
     *
     * \return Returns a set that contains the names of the distinct folders in the map.
     */
    std::set<std::string> getPresentFolders() const;
  private:
    std::map<SHA256::MessageDigest, std::string> m_FolderMap;
}; //class

#endif // FOLDERMAP_HPP
