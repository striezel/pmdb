/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database test suite.
    Copyright (C) 2015, 2025  Dirk Stolle

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

#include <exception>
#include <filesystem>
#include <fstream>
#include <string_view>
#include "../locate_catch.hpp"
#include "../FileGuard.hpp"
#include "../../code/MessageDatabase.hpp"

bool writeMessage(const std::filesystem::path& path, const std::string_view content)
{
  std::ofstream stream(path, std::ios::out | std::ios::binary);
  if (!stream.good())
    return false;
  if (!stream.write(content.data(), content.size()).good())
    return false;
  stream.close();
  return stream.good();
}

// tests for MessageDatabase::addMessage()

TEST_CASE("MessageDatabase")
{
  SECTION("empty database")
  {
    MessageDatabase mdb;

    REQUIRE( mdb.getNumberOfMessages() == 0 );
  }

  SECTION("single message")
  {
    MessageDatabase mdb;

    // some random content
    PrivateMessage pm;
    pm.setDatestamp("2007-06-14 12:34");
    pm.setTitle("This is the title");
    pm.setFromUser("Hermes");
    pm.setFromUserID(234);
    pm.setToUser("Poseidon");
    pm.setMessage("This is a message.");

    // database should accept message
    REQUIRE( mdb.addMessage(pm) );
    // But database should not accept same message twice!
    REQUIRE_FALSE( mdb.addMessage(pm) );
    // DB should say it has that message.
    REQUIRE( mdb.hasMessage(pm) );
    REQUIRE( mdb.getNumberOfMessages() == 1 );
  }

  SECTION("more than one message")
  {
    MessageDatabase mdb;

    // some random content
    PrivateMessage pm;
    pm.setDatestamp("2007-06-14 12:34");
    pm.setTitle("This is the title");
    pm.setFromUser("Hermes");
    pm.setFromUserID(234);
    pm.setToUser("Poseidon");
    pm.setMessage("This is a message.");

    PrivateMessage secondPM;
    secondPM.setDatestamp("2007-06-14 12:34");
    secondPM.setTitle("This is the title");
    secondPM.setFromUser("Mr. A");
    secondPM.setFromUserID(567890);
    secondPM.setToUser("Mrs. B");
    secondPM.setMessage("This is another message.");

    // database should accept message
    REQUIRE( mdb.addMessage(pm) );
    // database should accept 2nd message
    REQUIRE( mdb.addMessage(secondPM) );

    // DB should say it has the first message.
    REQUIRE( mdb.hasMessage(pm) );
    // DB should say it has the second message.
    REQUIRE( mdb.hasMessage(secondPM) );

    REQUIRE( mdb.getNumberOfMessages() == 2 );
  }

  SECTION("adding multiple PMs")
  {
    MessageDatabase mdb;

    // some random content
    PrivateMessage pm;
    pm.setDatestamp("2007-06-14 12:34");
    pm.setTitle("This is the title");
    pm.setFromUser("Hermes");
    pm.setFromUserID(234);
    pm.setToUser("Poseidon");
    pm.setMessage("This is a message.");

    constexpr uint32_t limit = 200000;

    for (uint32_t i = 1; i <= limit; ++i)
    {
      // change user ID to get a "different" message
      pm.setFromUserID(i);
      // database should accept message
      REQUIRE( mdb.addMessage(pm) );
      // database should know it has that message
      REQUIRE( mdb.hasMessage(pm) );
      // Database should NOT accept the message twice.
      REQUIRE_FALSE( mdb.addMessage(pm) );
    } //for
    REQUIRE( mdb.getNumberOfMessages() == limit );
  } //end of scope for multiple PMs

  SECTION("getMessage")
  {
    SECTION("no message found")
    {
      MessageDatabase mdb;

      SHA256::MessageDigest digest;
      REQUIRE( digest.fromHexString("abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789") );
      REQUIRE_THROWS( mdb.getMessage(digest) );
    }

    SECTION("message found")
    {
      MessageDatabase mdb;

      PrivateMessage pm;
      pm.setDatestamp("2007-06-14 12:34");
      pm.setTitle("This is the title");
      pm.setFromUser("Hermes");
      pm.setFromUserID(234);
      pm.setToUser("Poseidon");
      pm.setMessage("This is a message.");

      REQUIRE( mdb.addMessage(pm) );

      REQUIRE_NOTHROW( mdb.getMessage(pm.getHash()) );

      const auto& db_pm = mdb.getMessage(pm.getHash());

      REQUIRE( pm.getDatestamp() == db_pm.getDatestamp() );
      REQUIRE( pm.getTitle() == db_pm.getTitle() );
      REQUIRE( pm.getFromUser() == db_pm.getFromUser() );
      REQUIRE( pm.getFromUserID() == db_pm.getFromUserID() );
      REQUIRE( pm.getToUser() == db_pm.getToUser() );
      REQUIRE( pm.getMessage() == db_pm.getMessage() );
    }
  }

  SECTION("importFromFile")
  {
    uint32_t read_messages = 0;
    uint32_t new_messages = 0;

    SECTION("import single PM from file")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Postausgang">
                          <privatemessage>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title>This is a new title.</title>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>2345</fromuserid>
                                  <touser>Bob</touser>
                                  <message><![CDATA[This is a message.]]></message>
                          </privatemessage>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_single_message.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      read_messages = 0;
      new_messages = 0;
      REQUIRE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
      REQUIRE( read_messages == 1 );

      const auto pm = mdb.getBegin()->second;

      REQUIRE( pm.getDatestamp() == "2007-06-14 12:34" );
      REQUIRE( pm.getTitle() == "This is a new title." );
      REQUIRE( pm.getFromUser() == "Alice" );
      REQUIRE( pm.getFromUserID() == 2345 );
      REQUIRE( pm.getToUser() == "Bob" );
      REQUIRE( pm.getMessage() == "This is a message." );

      const auto hash = mdb.getBegin()->first;

      REQUIRE( fm.hasEntry(hash) );
      REQUIRE( fm.getFolderName(hash) == "Postausgang" );
    }

    SECTION("failure: XML cannot be parsed")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemess)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_unparsable_file.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: empty XML document")
    {
      constexpr std::string_view content = "";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_empty.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: wrong name for root element")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <fail>
                  <folder name="Postausgang">
                          <privatemessage>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title>This is a new title.</title>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>2345</fromuserid>
                                  <touser>Bob</touser>
                                  <message><![CDATA[This is a message.]]></message>
                          </privatemessage>
                  </folder>
          </fail>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_wrong_root_element.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: root element has no child nodes")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages></privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_no_child_nodes.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: wrong name for folder element")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <fail name="Postausgang">
                          <privatemessage>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title>This is a new title.</title>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>2345</fromuserid>
                                  <touser>Bob</touser>
                                  <message><![CDATA[This is a message.]]></message>
                          </privatemessage>
                  </fail>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_wrong_folder_element.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: wrong name for privatemessage element")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Postausgang">
                          <privatefail>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title>This is a new title.</title>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>2345</fromuserid>
                                  <touser>Bob</touser>
                                  <message><![CDATA[This is a message.]]></message>
                          </privatefail>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_wrong_privatemessage_element.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: privatemessage element has no child elements")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Whatever">
                          <privatemessage></privatemessage>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_privatemessage_without_child_elements.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: message has more than one datestamp")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Postausgang">
                          <privatemessage>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title>This is a new title.</title>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>2345</fromuserid>
                                  <touser>Bob</touser>
                                  <message><![CDATA[This is a message.]]></message>
                          </privatemessage>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_multiple_datestamps.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: message has more than one title")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Postausgang">
                          <privatemessage>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title>This is a new title.</title>
                                  <title>This is a new title.</title>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>2345</fromuserid>
                                  <touser>Bob</touser>
                                  <message><![CDATA[This is a message.]]></message>
                          </privatemessage>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_multiple_titles.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: message has more than one fromuser")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Postausgang">
                          <privatemessage>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title>This is a new title.</title>
                                  <fromuser>Alice</fromuser>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>2345</fromuserid>
                                  <touser>Bob</touser>
                                  <message><![CDATA[This is a message.]]></message>
                          </privatemessage>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_multiple_fromusers.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: message has more than one fromuserid")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Postausgang">
                          <privatemessage>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title>This is a new title.</title>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>2345</fromuserid>
                                  <fromuserid>2345</fromuserid>
                                  <touser>Bob</touser>
                                  <message><![CDATA[This is a message.]]></message>
                          </privatemessage>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_multiple_fromuserids.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: fromuserid is not numeric")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Postausgang">
                          <privatemessage>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title>This is a new title.</title>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>not_a_number</fromuserid>
                                  <touser>Bob</touser>
                                  <message><![CDATA[This is a message.]]></message>
                          </privatemessage>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_multiple_fromuserid_not_an_integer.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: fromuserid is only partially numeric")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Postausgang">
                          <privatemessage>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title>This is a new title.</title>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>23 hehe 45</fromuserid>
                                  <touser>Bob</touser>
                                  <message><![CDATA[This is a message.]]></message>
                          </privatemessage>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_multiple_fromuserid_partially_numeric.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: message has more than one touser")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Postausgang">
                          <privatemessage>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title>This is a new title.</title>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>2345</fromuserid>
                                  <touser>Bob</touser>
                                  <touser>Bob</touser>
                                  <message><![CDATA[This is a message.]]></message>
                          </privatemessage>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_multiple_tousers.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: message has more than one message node")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Postausgang">
                          <privatemessage>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title>This is a new title.</title>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>2345</fromuserid>
                                  <touser>Bob</touser>
                                  <message><![CDATA[This is a message.]]></message>
                                  <message><![CDATA[This is another message.]]></message>
                          </privatemessage>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_multiple_message_nodes.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: unknown node name")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Postausgang">
                          <privatemessage>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title>This is a new title.</title>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>2345</fromuserid>
                                  <touser>Bob</touser>
                                  <message><![CDATA[This is a message.]]></message>
                                  <not_recognized>Fails here.</not_recognized>
                          </privatemessage>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_unknown_node_name.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: empty message")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Postausgang">
                          <privatemessage>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title>This is a new title.</title>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>2345</fromuserid>
                                  <touser>Bob</touser>
                                  <message><![CDATA[]]></message>
                          </privatemessage>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_empty_message.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: empty datestamp")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Postausgang">
                          <privatemessage>
                                  <datestamp></datestamp>
                                  <title>Some title</title>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>2345</fromuserid>
                                  <touser>Bob</touser>
                                  <message><![CDATA[This is a message.]]></message>
                          </privatemessage>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_empty_datestamp.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: empty title")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Postausgang">
                          <privatemessage>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title></title>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>2345</fromuserid>
                                  <touser>Bob</touser>
                                  <message><![CDATA[This is a message.]]></message>
                          </privatemessage>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_empty_title.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: empty fromuser")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Postausgang">
                          <privatemessage>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title>No sender here</title>
                                  <fromuser></fromuser>
                                  <fromuserid>2345</fromuserid>
                                  <touser>Bob</touser>
                                  <message><![CDATA[This is a message.]]></message>
                          </privatemessage>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_empty_fromuser.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }

    SECTION("failure: empty touser")
    {
      constexpr std::string_view content = R"(<?xml version="1.0" encoding="ISO-8859-1"?>
          <privatemessages>
                  <folder name="Postausgang">
                          <privatemessage>
                                  <datestamp>2007-06-14 12:34</datestamp>
                                  <title>Fancy title</title>
                                  <fromuser>Alice</fromuser>
                                  <fromuserid>2345</fromuserid>
                                  <touser></touser>
                                  <message><![CDATA[This is a message.]]></message>
                          </privatemessage>
                  </folder>
          </privatemessages>)";

      const std::filesystem::path path{std::filesystem::temp_directory_path() / "pmdb_empty_touser.xml"};
      REQUIRE( writeMessage(path, content) );
      const FileGuard guard{path};

      MessageDatabase mdb;
      FolderMap fm;

      REQUIRE_FALSE( mdb.importFromFile(path.string(), read_messages, new_messages, fm) );
    }
  }
}
