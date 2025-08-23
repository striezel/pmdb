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
#include <algorithm>
#include <filesystem>
#include <fstream>
#include "../../code/ColourMap.hpp"
#include "../FileGuard.hpp"

// utility function to write a colour map file for testing
bool writeColourMap(const std::filesystem::path& path, const std::string& content)
{
  std::ofstream stream(path, std::ios::out | std::ios::binary);
  if (!stream.good())
    return false;
  if (!stream.write(content.c_str(), content.size()).good())
    return false;
  stream.close();
  return stream.good();
}

TEST_CASE("ColourMap")
{
  SECTION("add")
  {
    SECTION("empty folder name does not get added")
    {
      ColourMap cMap;
      cMap.add("", "\033[32m");
      REQUIRE( cMap.colouredFolder("") == "" );
    }

    SECTION("folder with empty control sequence does not get added")
    {
      ColourMap cMap;
      cMap.add("Inbox", "");
      REQUIRE( cMap.colouredFolder("Inbox") == "Inbox" );
    }

    SECTION("add folder with control sequence")
    {
      ColourMap cMap;
      cMap.add("Inbox", "\033[34m");
      REQUIRE( cMap.colouredFolder("Inbox") == "\033[34mInbox\033[0m" );
    }

    SECTION("add folder more than once overwrites previous control sequence")
    {
      ColourMap cMap;
      cMap.add("Inbox", "\033[34m");
      REQUIRE( cMap.colouredFolder("Inbox") == "\033[34mInbox\033[0m" );
      cMap.add("Inbox", "\033[32m");
      REQUIRE( cMap.colouredFolder("Inbox") == "\033[32mInbox\033[0m" );
    }
  }

  SECTION("colouredFolder")
  {
    SECTION("folder not in map returns unchanged")
    {
      ColourMap cMap;
      REQUIRE( cMap.colouredFolder("not in map") == "not in map" );
    }

    SECTION("folder in map returns name with control sequences")
    {
      ColourMap cMap;
      cMap.add("Inbox", "\033[32m");
      REQUIRE( cMap.colouredFolder("Inbox") == "\033[32mInbox\033[0m" );
      // Folder not in map is still unchanged.
      REQUIRE( cMap.colouredFolder("Outgoing") == "Outgoing" );
    }
  }
  SECTION("loadFromFile")
  {
    SECTION("file does not exist")
    {
      ColourMap cMap;
      REQUIRE_FALSE( cMap.loadFromFile("/this/does/not-exist.colourmap") );
    }

    SECTION("load empty colour map")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_empty.colourmap"};
      const std::string content = "";
      REQUIRE( writeColourMap(path, "") );
      FileGuard guard{path};

      ColourMap cMap;
      REQUIRE( cMap.loadFromFile(path.string()) );
    }

    SECTION("comments only")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "colourmap_comments_only.map"};
      const std::string content = R"map(# This is a colour map file for the test suite of pmdb.
# Colour maps can be used to give the folder names in the console output
# different colours to allow easier distinction.
#
# This file gets loaded during the test suite runs.
# For a better example see pmdb.colourmap.example in the repository's root
# directory.
#
# Supported colour names are:
#   default (default console font colour)
#   black
#   blue
#   cyan
#   green
#   magenta
#   red
#   white
#   yellow
#
# Basic notation for colour assignments is:
#
#   folder name=colour
#
)map";
      REQUIRE( writeColourMap(path, content) );
      FileGuard guard{path};

      ColourMap cMap;
      REQUIRE( cMap.loadFromFile(path.string()) );
    }

    SECTION("comments only with CRLF line terminators")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "colourmap_comments_only_crlf.map"};
      const std::string content = std::string("# This is a colour map file for the test suite of pmdb.\r\n")
        + "# Colour maps can be used to give the folder names in the console output\r\n"
        + "# different colours to allow easier distinction.\r\n"
        + "#\r\n"
        + "# This file gets loaded during the test suite runs.\r\n"
        + "# For a better example see pmdb.colourmap.example in the repository's root\r\n"
        + "# directory.\r\n"
        + "#\r\n"
        + "# Supported colour names are:\r\n"
        + "#   default (default console font colour)\r\n"
        + "#   black\r\n"
        + "#   blue\r\n"
        + "#   cyan\r\n"
        + "#   green\r\n"
        + "#   magenta\r\n"
        + "#   red\r\n"
        + "#   white\r\n"
        + "#   yellow\r\n"
        + "#\r\n"
        + "# Basic notation for colour assignments is:\r\n"
        + "#\r\n"
        + "#   folder name=colour\r\n"
        + "#\r\n";
      REQUIRE( writeColourMap(path, content) );
      FileGuard guard{path};

      ColourMap cMap;
      REQUIRE( cMap.loadFromFile(path.string()) );
    }

    SECTION("load map with empty line")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "colourmap-empty-line.map"};
      const std::string content = "Inbox=cyan\n\nOutgoing=red\n";
      REQUIRE( writeColourMap(path, content) );
      FileGuard guard{path};

      ColourMap cMap;
      REQUIRE( cMap.loadFromFile(path.string()) );
      REQUIRE( cMap.colouredFolder("Inbox") == "\033[36mInbox\033[0m" );
      REQUIRE( cMap.colouredFolder("Outgoing") == "\033[31mOutgoing\033[0m" );
    }

    SECTION("load map with empty line and CRLF line terminators")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "colourmap-empty-line-crlf.map"};
      const std::string content = "Inbox=cyan\r\n\r\nOutgoing=red\r\n";
      REQUIRE( writeColourMap(path, content) );
      FileGuard guard{path};

      ColourMap cMap;
      REQUIRE( cMap.loadFromFile(path.string()) );
      REQUIRE( cMap.colouredFolder("Inbox") == "\033[36mInbox\033[0m" );
      REQUIRE( cMap.colouredFolder("Outgoing") == "\033[31mOutgoing\033[0m" );
    }

    SECTION("all colours")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_colourmap_all_colours.map"};
      const std::string content = R"map(# This is a colour map file for the test suite of pmdb.
# Colour maps can be used to give the folder names in the console output
# different colours to allow easier distinction.
#
# This file gets loaded during the test suite runs.
# For a better example see pmdb.colourmap.example in the repository's root
# directory.
#
# Supported colour names are:
#   default (default console font colour)
#   black
#   blue
#   cyan
#   green
#   magenta
#   red
#   white
#   yellow
#
# Basic notation for colour assignments is:
#
#   folder name=colour
#

def=default
bck=black
blu=blue
cyn=cyan
grn=green
mgt=magenta
red=red
wht=white
ylw=yellow
)map";
      REQUIRE( writeColourMap(path, content) );
      FileGuard guard{path};

      ColourMap cMap;
      REQUIRE( cMap.loadFromFile(path.string()) );
      REQUIRE( cMap.colouredFolder("def") == "\033[0mdef\033[0m" );
      REQUIRE( cMap.colouredFolder("bck") == "\033[30mbck\033[0m" );
      REQUIRE( cMap.colouredFolder("blu") == "\033[34mblu\033[0m" );
      REQUIRE( cMap.colouredFolder("cyn") == "\033[36mcyn\033[0m" );
      REQUIRE( cMap.colouredFolder("grn") == "\033[32mgrn\033[0m" );
      REQUIRE( cMap.colouredFolder("mgt") == "\033[35mmgt\033[0m" );
      REQUIRE( cMap.colouredFolder("red") == "\033[31mred\033[0m" );
      REQUIRE( cMap.colouredFolder("wht") == "\033[37mwht\033[0m" );
      REQUIRE( cMap.colouredFolder("ylw") == "\033[33mylw\033[0m" );
    }

    SECTION("all colours with CRLF line terminators")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_colourmap_all_colours_crlf.map"};
      const std::string content = std::string("# This is a colour map file for the test suite of pmdb.\r\n")
        + "# Colour maps can be used to give the folder names in the console output\r\n"
        + "# different colours to allow easier distinction.\r\n"
        + "#\r\n"
        + "# This file gets loaded during the test suite runs.\r\n"
        + "# For a better example see pmdb.colourmap.example in the repository's root\r\n"
        + "# directory.\r\n"
        + "#\r\n"
        + "# Supported colour names are:\r\n"
        + "#   default (default console font colour)\r\n"
        + "#   black\r\n"
        + "#   blue\r\n"
        + "#   cyan\r\n"
        + "#   green\r\n"
        + "#   magenta\r\n"
        + "#   red\r\n"
        + "#   white\r\n"
        + "#   yellow\r\n"
        + "#\r\n"
        + "# Basic notation for colour assignments is:\r\n"
        + "#\r\n"
        + "#   folder name=colour\r\n"
        + "#\r\n"
        + "\r\n"
        + "def=default\r\n"
        + "bck=black\r\n"
        + "blu=blue\r\n"
        + "cyn=cyan\r\n"
        + "grn=green\r\n"
        + "mgt=magenta\r\n"
        + "red=red\r\n"
        + "wht=white\r\n"
        + "ylw=yellow\r\n";
      REQUIRE( writeColourMap(path, content) );
      FileGuard guard{path};

      ColourMap cMap;
      REQUIRE( cMap.loadFromFile(path.string()) );
      REQUIRE( cMap.colouredFolder("def") == "\033[0mdef\033[0m" );
      REQUIRE( cMap.colouredFolder("bck") == "\033[30mbck\033[0m" );
      REQUIRE( cMap.colouredFolder("blu") == "\033[34mblu\033[0m" );
      REQUIRE( cMap.colouredFolder("cyn") == "\033[36mcyn\033[0m" );
      REQUIRE( cMap.colouredFolder("grn") == "\033[32mgrn\033[0m" );
      REQUIRE( cMap.colouredFolder("mgt") == "\033[35mmgt\033[0m" );
      REQUIRE( cMap.colouredFolder("red") == "\033[31mred\033[0m" );
      REQUIRE( cMap.colouredFolder("wht") == "\033[37mwht\033[0m" );
      REQUIRE( cMap.colouredFolder("ylw") == "\033[33mylw\033[0m" );
    }

    SECTION("repeating folders")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_colourmap_repeat.map"};
      const std::string content = R"map(# This is a colour map file for the test suite of pmdb.
# Colour maps can be used to give the folder names in the console output
# different colours to allow easier distinction.
#
# This file gets loaded during the test suite runs.
# For a better example see pmdb.colourmap.example in the repository's root
# directory.
#
# Supported colour names are:
#   default (default console font colour)
#   black
#   blue
#   cyan
#   green
#   magenta
#   red
#   white
#   yellow
#
# Basic notation for colour assignments is:
#
#   folder name=colour
#

def=default
def=default
def=default
bck=black
bck=black
blu=blue
blu=blue
cyn=cyan
cyn=cyan
grn=green
grn=green
mgt=magenta
mgt=magenta
red=red
red=red
wht=white
wht=white
ylw=yellow
ylw=yellow
)map";
      REQUIRE( writeColourMap(path, content) );
      FileGuard guard{path};

      ColourMap cMap;
      REQUIRE( cMap.loadFromFile(path.string()) );
      REQUIRE( cMap.colouredFolder("def") == "\033[0mdef\033[0m" );
      REQUIRE( cMap.colouredFolder("bck") == "\033[30mbck\033[0m" );
      REQUIRE( cMap.colouredFolder("blu") == "\033[34mblu\033[0m" );
      REQUIRE( cMap.colouredFolder("cyn") == "\033[36mcyn\033[0m" );
      REQUIRE( cMap.colouredFolder("grn") == "\033[32mgrn\033[0m" );
      REQUIRE( cMap.colouredFolder("mgt") == "\033[35mmgt\033[0m" );
      REQUIRE( cMap.colouredFolder("red") == "\033[31mred\033[0m" );
      REQUIRE( cMap.colouredFolder("wht") == "\033[37mwht\033[0m" );
      REQUIRE( cMap.colouredFolder("ylw") == "\033[33mylw\033[0m" );
    }

    SECTION("repeating folders with CRLF line terminators")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_colourmap_repeat_crlf.map"};
      const std::string content = std::string("# This is a colour map file for the test suite of pmdb.\r\n")
        + "# Colour maps can be used to give the folder names in the console output\r\n"
        + "# different colours to allow easier distinction.\r\n"
        + "#\r\n"
        + "# This file gets loaded during the test suite runs.\r\n"
        + "# For a better example see pmdb.colourmap.example in the repository's root\r\n"
        + "# directory.\r\n"
        + "#\r\n"
        + "# Supported colour names are:\r\n"
        + "#   default (default console font colour)\r\n"
        + "#   black\r\n"
        + "#   blue\r\n"
        + "#   cyan\r\n"
        + "#   green\r\n"
        + "#   magenta\r\n"
        + "#   red\r\n"
        + "#   white\r\n"
        + "#   yellow\r\n"
        + "#\r\n"
        + "# Basic notation for colour assignments is:\r\n"
        + "#\r\n"
        + "#   folder name=colour\r\n"
        + "#\r\n"
        + "\r\n"
        + "def=default\r\n"
        + "def=default\r\n"
        + "def=default\r\n"
        + "bck=black\r\n"
        + "bck=black\r\n"
        + "blu=blue\r\n"
        + "blu=blue\r\n"
        + "cyn=cyan\r\n"
        + "cyn=cyan\r\n"
        + "grn=green\r\n"
        + "grn=green\r\n"
        + "mgt=magenta\r\n"
        + "mgt=magenta\r\n"
        + "red=red\r\n"
        + "red=red\r\n"
        + "wht=white\r\n"
        + "wht=white\r\n"
        + "ylw=yellow\r\n"
        + "ylw=yellow\r\n";
      REQUIRE( writeColourMap(path, content) );
      FileGuard guard{path};

      ColourMap cMap;
      REQUIRE( cMap.loadFromFile(path.string()) );
      REQUIRE( cMap.colouredFolder("def") == "\033[0mdef\033[0m" );
      REQUIRE( cMap.colouredFolder("bck") == "\033[30mbck\033[0m" );
      REQUIRE( cMap.colouredFolder("blu") == "\033[34mblu\033[0m" );
      REQUIRE( cMap.colouredFolder("cyn") == "\033[36mcyn\033[0m" );
      REQUIRE( cMap.colouredFolder("grn") == "\033[32mgrn\033[0m" );
      REQUIRE( cMap.colouredFolder("mgt") == "\033[35mmgt\033[0m" );
      REQUIRE( cMap.colouredFolder("red") == "\033[31mred\033[0m" );
      REQUIRE( cMap.colouredFolder("wht") == "\033[37mwht\033[0m" );
      REQUIRE( cMap.colouredFolder("ylw") == "\033[33mylw\033[0m" );
    }

    SECTION("invalid: colour map with missing '=' in line")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "colourmap-wrong-format.map"};
      const std::string content = "def:default\n";
      REQUIRE( writeColourMap(path, content) );
      FileGuard guard{path};

      ColourMap cMap;
      REQUIRE_FALSE( cMap.loadFromFile(path.string()) );
    }

    SECTION("invalid: empty folder name")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb-empty-folder-name.colourmap"};
      const std::string content = "=green\n";
      REQUIRE( writeColourMap(path, content) );
      FileGuard guard{path};

      ColourMap cMap;
      REQUIRE_FALSE( cMap.loadFromFile(path.string()) );
    }

    SECTION("invalid: empty colour name")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb-empty-colour-name.colourmap"};
      const std::string content = "Inbox=\n";
      REQUIRE( writeColourMap(path, content) );
      FileGuard guard{path};

      ColourMap cMap;
      REQUIRE_FALSE( cMap.loadFromFile(path.string()) );
    }

    SECTION("invalid: unrecognized colour name")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb-unrecognized-colour.colourmap"};
      const std::string content = "Inbox=foo\n";
      REQUIRE( writeColourMap(path, content) );
      FileGuard guard{path};

      ColourMap cMap;
      REQUIRE_FALSE( cMap.loadFromFile(path.string()) );
    }
  }
}
