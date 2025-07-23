/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database test suite.
    Copyright (C) 2015, 2022, 2025  Dirk Stolle

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

#include "../locate_catch.hpp"
#include <filesystem>
#include <fstream>
#include <string_view>
#include "../../code/FolderMap.hpp"
#include "../FileGuard.hpp"

TEST_CASE("FolderMap")
{
  SECTION("empty map")
  {
    FolderMap fm;
    REQUIRE( fm.getPresentFolders().empty() );

    // create an example hash
    SHA256::MessageDigest digest;
    REQUIRE( digest.fromHexString("5fa118354fd6e1dec635608c1856949c46c13d8752b8ccd6d2b4f1eec9d36c06") );

    // should not have entry!
    REQUIRE_FALSE( fm.hasEntry(digest) );

    // try to get folder name of hash that is not in map -> should throw
    REQUIRE_THROWS( fm.getFolderName(digest) );
  }

  SECTION("map with one entry")
  {
    FolderMap fm;

    // create an example hash
    SHA256::MessageDigest digest;
    REQUIRE( digest.fromHexString("5fa118354fd6e1dec635608c1856949c46c13d8752b8ccd6d2b4f1eec9d36c06") );
    // add entry
    fm.add(digest, "A folder's name");

    // should have entry
    REQUIRE( fm.hasEntry(digest) );
    // try to get folder name of hash that is in map -> should NOT throw
    REQUIRE_NOTHROW( fm.getFolderName(digest) );
    const std::string name = fm.getFolderName(digest);
    // name should be the same that went in
    REQUIRE( name == "A folder's name" );

    // check present folders
    const std::set<std::string> folders = fm.getPresentFolders();
    REQUIRE( folders.size() == 1 );
    // check name of folder
    REQUIRE( *folders.begin() == "A folder's name" );
  }

  SECTION("two messages in same folder")
  {
    FolderMap fm;

    // create two example hashes
    SHA256::MessageDigest digest_one;
    REQUIRE( digest_one.fromHexString("5fa118354fd6e1dec635608c1856949c46c13d8752b8ccd6d2b4f1eec9d36c06") );
    SHA256::MessageDigest digest_two;
    REQUIRE( digest_two.fromHexString("925ba3061b4b14c6a7cb661500e182d218816053c78215efa2bf68925ac25f29") );

    // add entries
    fm.add(digest_one, "A folder's name");
    fm.add(digest_two, "A folder's name");

    // should have entries
    REQUIRE( fm.hasEntry(digest_one) );
    REQUIRE( fm.hasEntry(digest_two) );

    // try to get folder name of hash that is in map -> should NOT throw
    REQUIRE_NOTHROW( fm.getFolderName(digest_one) );
    const std::string name_one = fm.getFolderName(digest_one);
    REQUIRE_NOTHROW( fm.getFolderName(digest_two) );
    const std::string name_two = fm.getFolderName(digest_two);
    // name should be the same that went in
    REQUIRE( name_one == "A folder's name" );
    REQUIRE( name_two == "A folder's name" );

    // check present folders
    const std::set<std::string> folders = fm.getPresentFolders();
    REQUIRE( folders.size() == 1 );

    // check name of folder
    REQUIRE( *folders.begin() == "A folder's name" );
  }

  SECTION("two messages in different folders")
  {
    FolderMap fm;

    // create two example hashes
    SHA256::MessageDigest digest_one;
    REQUIRE( digest_one.fromHexString("2d95696c3c83910d9ee999cda9c6f23fba73dbb597390fdae6edc723163bf81e") );
    SHA256::MessageDigest digest_two;
    REQUIRE( digest_two.fromHexString("a3d17e5bb63d7f73a850265e95c1b7a3699058ff6ca55f8a1e2ee3eab7e3398e") );

    // add entries
    fm.add(digest_one, "A folder's name");
    fm.add(digest_two, "Folder #2");

    // should have entries
    REQUIRE( fm.hasEntry(digest_one) );
    REQUIRE( fm.hasEntry(digest_two) );

    // try to get folder name of hash that is in map -> should NOT throw
    REQUIRE_NOTHROW( fm.getFolderName(digest_one) );
    REQUIRE_NOTHROW( fm.getFolderName(digest_two) );
    const std::string name_one = fm.getFolderName(digest_one);
    const std::string name_two = fm.getFolderName(digest_two);
    // name should be the same that went in
    REQUIRE( name_one == "A folder's name" );
    REQUIRE( name_two == "Folder #2" );

    // check present folders
    const std::set<std::string> folders = fm.getPresentFolders();
    REQUIRE( folders.size() == 2 );

    // check name of folder
    REQUIRE( *folders.begin() == "A folder's name" );
    REQUIRE(*(++folders.begin()) == "Folder #2" );
  }

  SECTION("save + load")
  {
    namespace fs = std::filesystem;

    SECTION("save to non-existent directory")
    {
      FolderMap fm;

      // create two example hashes
      SHA256::MessageDigest digest_one;
      REQUIRE( digest_one.fromHexString("0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef") );
      SHA256::MessageDigest digest_two;
      REQUIRE( digest_two.fromHexString("abacab120d0aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") );

      // add entries
      fm.add(digest_one, "First folder");
      fm.add(digest_two, "Folder #2");

      const fs::path path{fs::temp_directory_path() / "does" / "not" / "exist"};
      REQUIRE_FALSE( fm.save(path.string()) );
    }

    SECTION("save and load again")
    {
      FolderMap fm;

      // create two example hashes
      SHA256::MessageDigest digest_one;
      REQUIRE( digest_one.fromHexString("0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef") );
      SHA256::MessageDigest digest_two;
      REQUIRE( digest_two.fromHexString("abacab120d0aabbccaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") );

      // add entries
      fm.add(digest_one, "First folder");
      fm.add(digest_two, "Folder #2");

      const fs::path path{fs::temp_directory_path() / "save-foldermap-test"};
      REQUIRE( fs::create_directory(path) );
      FileGuard guard{path};
      REQUIRE( fm.save(path.string()) );
      FileGuard guard2{path / "foldermap"};

      // loading should succeed. too
      FolderMap fm_load;
      REQUIRE( fm_load.load(path.string()) );
      // Both entries should be present.
      REQUIRE( fm_load.hasEntry(digest_one) );
      REQUIRE( fm_load.hasEntry(digest_two) );
      // ... and so should be the folder names.
      REQUIRE( fm_load.getFolderName(digest_one) == "First folder" );
      REQUIRE( fm_load.getFolderName(digest_two) == "Folder #2" );
    }

    SECTION("load from non-existent directory")
    {
      FolderMap fm;

      const fs::path path{fs::temp_directory_path() / "does" / "not" / "exist"};
      REQUIRE_FALSE( fm.load(path.string()) );
    }

    SECTION("load foldermap with invalid hash")
    {
      const fs::path path{fs::temp_directory_path() / "pmdb_foldermap_invalid_hash"};
      REQUIRE( fs::create_directory(path) );
      const FileGuard guard{path};

      {
        // Write raw foldermap file with invalid hash.
        using namespace std::string_view_literals;
        constexpr std::string_view data = "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef One Folder\n01234_not_a_sha-256_hash_9abcdef0123456789abcdef0123456789abcdef Fol derp\n"sv;
        std::ofstream stream(path / "foldermap", std::ios::out | std::ios::binary);
        REQUIRE( stream.good() );
        REQUIRE( stream.write(data.data(), data.size()).good() );
        stream.close();
        REQUIRE( stream.good() );
      }

      const FileGuard guard2{path / "foldermap"};

      // loading the foldermap file fails
      FolderMap fm;
      REQUIRE_FALSE( fm.load(path.string()) );
    }
  }
}
