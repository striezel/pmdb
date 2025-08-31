/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database test suite.
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
#include "../../code/html_generation.hpp"

TEST_CASE("HTML generation")
{
  SECTION("generateHtmlFiles")
  {
    FolderMap fm;
    MessageDatabase mdb;

    PrivateMessage pm;
    pm.setDatestamp("2007-06-14 12:34");
    pm.setTitle("This is the title");
    pm.setFromUser("Hermes");
    pm.setFromUserID(234);
    pm.setToUser("Poseidon");
    pm.setMessage("This is a [b]bold[/b] text.");

    fm.add(pm.getHash(), "Inbox");
    mdb.addMessage(pm);

    HTMLOptions options;
    options.standard = HTMLStandard::HTML4_01;

    std::filesystem::path html_path = std::filesystem::temp_directory_path() / "pmdb_test_html_directory";

    int exit_code = generateHtmlFiles(mdb, fm, options, html_path.string());
    REQUIRE( exit_code == 0 );

    auto message_path = html_path / (pm.getHash().toHexString() + ".html");
    REQUIRE( std::filesystem::is_regular_file(message_path) );

    auto folder_path = html_path / "folder_94835ea2fcf775cd77cb9c9cee01b5cbd9bc515467aab1215f48a5ade9ca5274.html";
    REQUIRE( std::filesystem::is_regular_file(message_path) );

    REQUIRE( std::filesystem::remove(message_path) );
    REQUIRE( std::filesystem::remove(folder_path) );
    REQUIRE( std::filesystem::remove(html_path) );
  }
}
