/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database test suite.
    Copyright (C) 2015  Thoronador

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

#include <iostream>
#include "../../../code/FolderMap.hpp"

int main()
{
  //scope for empty map
  {
    FolderMap fm;

    if (!fm.getPresentFolders().empty())
    {
      std::cout << "Error: Newly created FolderMap instance has non-empty folder list!\n";
      return 1;
    }

    //create an example hash
    SHA256::MessageDigest digest;
    if (!digest.fromHexString("5fa118354fd6e1dec635608c1856949c46c13d8752b8ccd6d2b4f1eec9d36c06"))
    {
      std::cout << "Error: Could not create SHA-256 hash from string!\n";
      return 1;
    }

    //should not have entry!
    if (fm.hasEntry(digest))
    {
      std::cout << "Error: FolderMap has an entry it should not have!\n";
      return 1;
    }

    //try to get folder name of hash that is not in map -> should throw
    try
    {
      const std::string name = fm.getFolderName(digest);
      std::cout << "Error: Could get folder name for hash that is not element of the map!\n";
      return 1;
    }
    catch (...)
    {
      //Caught exception, everything as expected.
      std::cout << "Info: Passed tests for empty folder map.\n";
    }
  } //end of scope for empty map

  //scope for map with one entry
  {
    FolderMap fm;

    //create an example hash
    SHA256::MessageDigest digest;
    if (!digest.fromHexString("5fa118354fd6e1dec635608c1856949c46c13d8752b8ccd6d2b4f1eec9d36c06"))
    {
      std::cout << "Error: Could not create SHA-256 hash from string!\n";
      return 1;
    }

    //add entry
    fm.add(digest, "A folder's name");

    //should have entry
    if (!fm.hasEntry(digest))
    {
      std::cout << "Error: FolderMap does not have the entry that was just added!\n";
      return 1;
    }

    //try to get folder name of hash that is in map -> should NOT throw
    try
    {
      const std::string name = fm.getFolderName(digest);
      //name should be the same that went in
      if (name != "A folder's name")
      {
        std::cout << "Error: Folder name that came out of map has changed!\n"
                  << "Original: A folder's name\n"
                  << "From map: " << name << "\n";
        return 1;
      }
    }
    catch (...)
    {
      //Caught an exception, something is screwed up!
      std::cout << "Error: Could not get folder name for hash "<< digest.toHexString() << "!\n";
      return 1;
    }

    //check present folders
    const std::set<std::string> folders = fm.getPresentFolders();
    if (folders.size() != 1)
    {
      std::cout << "Error: Number of present folders should be exactly one, but it is "
                << static_cast<int>(folders.size()) << " instead.\n";
      return 1;
    }

    //check name of folder+
    if (*folders.begin() != "A folder's name")
    {
      std::cout << "Error: Folder name should be \"A folder's name\", but it is \""
                << *folders.begin() << "\" instead.\n";
      return 1;
    }
    std::cout << "Info: Passed tests for single entry in folder map.\n";
  } //end of scope for one entry in map


  //scope for two messages
  {
    FolderMap fm;

    //create two example hashes
    SHA256::MessageDigest digest_one;
    if (!digest_one.fromHexString("5fa118354fd6e1dec635608c1856949c46c13d8752b8ccd6d2b4f1eec9d36c06"))
    {
      std::cout << "Error: Could not create SHA-256 hash from string!\n";
      return 1;
    }
    SHA256::MessageDigest digest_two;
    if (!digest_two.fromHexString("925ba3061b4b14c6a7cb661500e182d218816053c78215efa2bf68925ac25f29"))
    {
      std::cout << "Error: Could not create SHA-256 hash from string!\n";
      return 1;
    }

    //add entries
    fm.add(digest_one, "A folder's name");
    fm.add(digest_two, "A folder's name");

    //should have entries
    if (!fm.hasEntry(digest_one) || !fm.hasEntry(digest_two))
    {
      std::cout << "Error: FolderMap does not have the entry that was just added!\n";
      return 1;
    }

    //try to get folder name of hash that is in map -> should NOT throw
    try
    {
      const std::string name_one = fm.getFolderName(digest_one);
      const std::string name_two = fm.getFolderName(digest_two);
      //name should be the same that went in
      if (name_one != "A folder's name")
      {
        std::cout << "Error: Folder name that came out of map has changed!\n"
                  << "Original: A folder's name\n"
                  << "From map: " << name_one << "\n";
        return 1;
      }
      if (name_two != "A folder's name")
      {
        std::cout << "Error: Folder name that came out of map has changed!\n"
                  << "Original: A folder's name\n"
                  << "From map: " << name_two << "\n";
        return 1;
      }
    }
    catch (...)
    {
      //Caught an exception, something is screwed up!
      std::cout << "Error: Could not get folder name for hash "
                << digest_one.toHexString() << " or " << digest_two.toHexString()
                << "!\n";
      return 1;
    }

    //check present folders
    const std::set<std::string> folders = fm.getPresentFolders();
    if (folders.size() != 1)
    {
      std::cout << "Error: Number of present folders should be exactly one, but it is "
                << static_cast<int>(folders.size()) << " instead.\n";
      return 1;
    }

    //check name of folder+
    if (*folders.begin() != "A folder's name")
    {
      std::cout << "Error: Folder name should be \"A folder's name\", but it is \""
                << *folders.begin() << "\" instead.\n";
      return 1;
    }
    std::cout << "Info: Passed tests for folder map with two entries.\n";
  } //end of scope for two messages in same folder


  //scope for two messages in different folders
  {
    FolderMap fm;

    //create two example hashes
    SHA256::MessageDigest digest_one;
    if (!digest_one.fromHexString("2d95696c3c83910d9ee999cda9c6f23fba73dbb597390fdae6edc723163bf81e"))
    {
      std::cout << "Error: Could not create SHA-256 hash from string!\n";
      return 1;
    }
    SHA256::MessageDigest digest_two;
    if (!digest_two.fromHexString("a3d17e5bb63d7f73a850265e95c1b7a3699058ff6ca55f8a1e2ee3eab7e3398e"))
    {
      std::cout << "Error: Could not create SHA-256 hash from string!\n";
      return 1;
    }

    //add entries
    fm.add(digest_one, "A folder's name");
    fm.add(digest_two, "Folder #2");

    //should have entries
    if (!fm.hasEntry(digest_one) || !fm.hasEntry(digest_two))
    {
      std::cout << "Error: FolderMap does not have the entry that was just added!\n";
      return 1;
    }

    //try to get folder name of hash that is in map -> should NOT throw
    try
    {
      const std::string name_one = fm.getFolderName(digest_one);
      const std::string name_two = fm.getFolderName(digest_two);
      //name should be the same that went in
      if (name_one != "A folder's name")
      {
        std::cout << "Error: Folder name that came out of map has changed!\n"
                  << "Original: A folder's name\n"
                  << "From map: " << name_one << "\n";
        return 1;
      }
      if (name_two != "Folder #2")
      {
        std::cout << "Error: Folder name that came out of map has changed!\n"
                  << "Original: Folder #2\n"
                  << "From map: " << name_two << "\n";
        return 1;
      }
    }
    catch (...)
    {
      //Caught an exception, something is screwed up!
      std::cout << "Error: Could not get folder name for hash "
                << digest_one.toHexString() << " or " << digest_two.toHexString()
                << "!\n";
      return 1;
    }

    //check present folders
    const std::set<std::string> folders = fm.getPresentFolders();
    if (folders.size() != 2)
    {
      std::cout << "Error: Number of present folders should be exactly two, but it is "
                << static_cast<int>(folders.size()) << " instead.\n";
      return 1;
    }

    //check name of folder
    if (*folders.begin() != "A folder's name")
    {
      std::cout << "Error: Folder name should be \"A folder's name\", but it is \""
                << *folders.begin() << "\" instead.\n";
      return 1;
    }
    if (*(++folders.begin()) != "Folder #2")
    {
      std::cout << "Error: Folder name should be \"Folder #2\", but it is \""
                << *(++folders.begin()) << "\" instead.\n";
      return 1;
    }
    std::cout << "Info: Passed tests for folder map with two entries in different folders.\n";
  } //end of scope for two messages in different folders


  std::cout << "Info: Passed all FolderMap tests in add-get!\n";
  return 0;
}
