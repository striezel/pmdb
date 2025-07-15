/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2025  Dirk Stolle

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
#include "../../code/CompressionDetection.hpp"
#include "../../code/PrivateMessage.hpp"
#include "../FileGuard.hpp"

PrivateMessage getExampleMessage()
{
  PrivateMessage pm;
  pm.setDatestamp("2007-06-14 12:34");
  pm.setTitle("This is the title");
  pm.setFromUser("Hermes");
  pm.setFromUserID(234);
  pm.setToUser("Poseidon");
  pm.setMessage("This is a test message.");
  return pm;
}

TEST_CASE("detect_compression")
{
  namespace fs = std::filesystem;

  SECTION("non-existent directory")
  {
    const fs::path path{fs::temp_directory_path() / "does" / "not" / "exist"};
    const auto detected = detect_compression(path.string());
    REQUIRE_FALSE( detected.has_value() );
  }

  SECTION("empty directory")
  {
    const fs::path path{fs::temp_directory_path() / "pmdb_dir_is_empty"};
    REQUIRE( fs::create_directories(path) );
    FileGuard guard{path};

    const auto detected = detect_compression(path.string());
    REQUIRE_FALSE( detected.has_value() );
  }

  SECTION("directory with uncompressed private message")
  {
    const fs::path path{fs::temp_directory_path() / "pm_uncompressed_directory"};
    REQUIRE( fs::create_directory(path) );
    FileGuard guard{path};

    auto pm = getExampleMessage();
    const auto hash = pm.getHash().toHexString();
    REQUIRE( pm.saveToFile((path / hash).string(), Compression::none) );

    const auto detected = detect_compression(path.string());
    REQUIRE( detected.has_value() );
    REQUIRE( detected.value() == Compression::none );
    REQUIRE( fs::remove(path / hash) );
    REQUIRE( fs::remove(path) );
  }

  SECTION("directory with compressed private message")
  {
    const fs::path path{fs::temp_directory_path() / "pm_zlib_directory"};
    REQUIRE( fs::create_directory(path) );

    auto pm = getExampleMessage();
    const auto hash = pm.getHash().toHexString();
    REQUIRE( pm.saveToFile((path / hash).string(), Compression::zlib) );

    const auto detected = detect_compression(path.string());
    REQUIRE( detected.has_value() );
    REQUIRE( detected.value() == Compression::zlib );
    REQUIRE( fs::remove(path / hash) );
    REQUIRE( fs::remove(path) );
  }
}
