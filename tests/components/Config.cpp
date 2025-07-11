/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
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
#include <algorithm>
#include <filesystem>
#include <fstream>
#include "../../code/Config.hpp"
#include "../FileGuard.hpp"

// utility function to write a configuration file for testing
bool writeConfiguration(const std::filesystem::path& path, const std::string& content)
{
  std::ofstream stream(path, std::ios::out | std::ios::binary);
  if (!stream.good())
    return false;
  if (!stream.write(content.c_str(), content.size()).good())
    return false;
  stream.close();
  return stream.good();
}

TEST_CASE("Config")
{
  SECTION("initial state")
  {
    Config conf;
    REQUIRE( conf.getForumURL().empty() );
    REQUIRE( conf.getSmilies().empty() );
  }

  SECTION("set / get URL")
  {
    Config conf;

    conf.setForumURL("http://www.example.com/");
    // forum URL should match value above
    REQUIRE( conf.getForumURL() == "http://www.example.com/" );
  }

  SECTION("loadFromFile")
  {
    SECTION("configuration file does not exist")
    {
      Config conf;
      REQUIRE_FALSE( conf.loadFromFile("/this/does/not-exist.conf") );
    }

    SECTION("load small configuration")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "small.conf"};
      const std::string content = "forum=https://forum.example.com/yay/\nsmilie=:)=https://example.com/smile.gif\nsmilie_r=:(=sad.gif\n";
      REQUIRE( writeConfiguration(path, content) );
      FileGuard guard{path};

      Config conf;
      REQUIRE( conf.loadFromFile(path.string()) );

      REQUIRE( conf.getForumURL() == "https://forum.example.com/yay/" );
      const auto& smilies = conf.getSmilies();
      REQUIRE( smilies.size() == 2 );

      auto iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ":)"; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "https://example.com/smile.gif" );
      REQUIRE( iter->type() == UrlType::Absolute );

      iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ":("; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "sad.gif" );
      REQUIRE( iter->type() == UrlType::Relative );
    }

    SECTION("load small configuration with CRLF line terminators")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "small-crlf.conf"};
      const std::string content = "forum=https://forum.example.com/crlf/\r\nsmilie=:)=https://example.com/smile.gif\r\nsmilie_r=:(=sad.gif\r\n";
      REQUIRE( writeConfiguration(path, content) );
      FileGuard guard{path};

      Config conf;
      REQUIRE( conf.loadFromFile(path.string()) );

      REQUIRE( conf.getForumURL() == "https://forum.example.com/crlf/" );
      const auto& smilies = conf.getSmilies();
      REQUIRE( smilies.size() == 2 );

      auto iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ":)"; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "https://example.com/smile.gif" );
      REQUIRE( iter->type() == UrlType::Absolute );

      iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ":("; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "sad.gif" );
      REQUIRE( iter->type() == UrlType::Relative );
    }

    SECTION("load configuration with empty line")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "empty-line.conf"};
      const std::string content = "forum=https://forum.example.com/yay/\n\nsmilie=:)=https://example.com/smile.gif\nsmilie_r=:(=sad.gif\n";
      REQUIRE( writeConfiguration(path, content) );
      FileGuard guard{path};

      Config conf;
      REQUIRE( conf.loadFromFile(path.string()) );

      REQUIRE( conf.getForumURL() == "https://forum.example.com/yay/" );
      const auto& smilies = conf.getSmilies();
      REQUIRE( smilies.size() == 2 );

      auto iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ":)"; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "https://example.com/smile.gif" );
      REQUIRE( iter->type() == UrlType::Absolute );

      iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ":("; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "sad.gif" );
      REQUIRE( iter->type() == UrlType::Relative );
    }

    SECTION("load configuration with empty line and CRLF line terminators")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "empty-line-crlf.conf"};
      const std::string content = "forum=https://forum.example.com/yay/\r\n\r\nsmilie=:)=https://example.com/smile.gif\r\nsmilie_r=:(=sad.gif\r\n";
      REQUIRE( writeConfiguration(path, content) );
      FileGuard guard{path};

      Config conf;
      REQUIRE( conf.loadFromFile(path.string()) );

      REQUIRE( conf.getForumURL() == "https://forum.example.com/yay/" );
      const auto& smilies = conf.getSmilies();
      REQUIRE( smilies.size() == 2 );

      auto iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ":)"; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "https://example.com/smile.gif" );
      REQUIRE( iter->type() == UrlType::Absolute );

      iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ":("; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "sad.gif" );
      REQUIRE( iter->type() == UrlType::Relative );
    }

    SECTION("load configuration with comments")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "comments.conf"};
      const std::string content = "# This is a comment.\nforum=https://forum.example.com/yay/\n# next comment\nsmilie=:)=https://example.com/smile.gif\nsmilie_r=:(=sad.gif\n";
      REQUIRE( writeConfiguration(path, content) );
      FileGuard guard{path};

      Config conf;
      REQUIRE( conf.loadFromFile(path.string()) );

      REQUIRE( conf.getForumURL() == "https://forum.example.com/yay/" );
      const auto& smilies = conf.getSmilies();
      REQUIRE( smilies.size() == 2 );

      auto iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ":)"; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "https://example.com/smile.gif" );
      REQUIRE( iter->type() == UrlType::Absolute );

      iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ":("; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "sad.gif" );
      REQUIRE( iter->type() == UrlType::Relative );
    }

    SECTION("load configuration with comments and CRLF line terminators")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "comments-crlf.conf"};
      const std::string content = "# This is a comment.\r\nforum=https://forum.example.com/yay/\r\n# next comment\r\nsmilie=:)=https://example.com/smile.gif\r\nsmilie_r=:(=sad.gif";
      REQUIRE( writeConfiguration(path, content) );
      FileGuard guard{path};

      Config conf;
      REQUIRE( conf.loadFromFile(path.string()) );

      REQUIRE( conf.getForumURL() == "https://forum.example.com/yay/" );
      const auto& smilies = conf.getSmilies();
      REQUIRE( smilies.size() == 2 );

      auto iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ":)"; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "https://example.com/smile.gif" );
      REQUIRE( iter->type() == UrlType::Absolute );

      iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ":("; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "sad.gif" );
      REQUIRE( iter->type() == UrlType::Relative );
    }

    SECTION("load configuration with more smilies")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "more-smilies.conf"};
      const std::string content = "forum=https://forum.example.com/yay/\nsmilie=:)=https://example.com/smile.gif\nsmilie_r=:(=sad.gif\nsmilie_r=:p=tongue.gif\nsmilie=;)=https://example.com/wink.gif";
      REQUIRE( writeConfiguration(path, content) );
      FileGuard guard{path};

      Config conf;
      REQUIRE( conf.loadFromFile(path.string()) );

      REQUIRE( conf.getForumURL() == "https://forum.example.com/yay/" );
      const auto& smilies = conf.getSmilies();
      REQUIRE( smilies.size() == 4 );

      auto iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ":)"; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "https://example.com/smile.gif" );
      REQUIRE( iter->type() == UrlType::Absolute );

      iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ":("; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "sad.gif" );
      REQUIRE( iter->type() == UrlType::Relative );

      iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ":p"; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "tongue.gif" );
      REQUIRE( iter->type() == UrlType::Relative );

      iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ";)"; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "https://example.com/wink.gif" );
      REQUIRE( iter->type() == UrlType::Absolute );
    }

    SECTION("load World of Players configuration example")
    {
      std::filesystem::path path{__FILE__};
      path.remove_filename();
      path = path / ".." / ".." / "documentation" / "pmdb.conf.example.worldofplayers";

      Config conf;
      REQUIRE( conf.loadFromFile(path.string()) );

      REQUIRE( conf.getForumURL() == "https://forum.worldofplayers.de/forum/" );
      const auto& smilies = conf.getSmilies();
      REQUIRE( smilies.size() > 70 );

      auto iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == ":)"; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "images/smilies/s_064.gif" );
      REQUIRE( iter->type() == UrlType::Relative );

      iter = std::find_if(smilies.begin(), smilies.end(), [](const Smilie& s) { return s.code() == "§hehe"; });
      REQUIRE( iter != smilies.end() );
      REQUIRE( iter->url() == "images/smilies/s_028.gif" );
      REQUIRE( iter->type() == UrlType::Relative );
    }

    SECTION("invalid: configuration with unknown setting name")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "unknown-name.conf"};
      const std::string content = "forum=https://forum.example.com/nay/\nfoo=bar\n";
      REQUIRE( writeConfiguration(path, content) );
      FileGuard guard{path};

      Config conf;
      REQUIRE_FALSE( conf.loadFromFile(path.string()) );
    }

    SECTION("invalid: configuration with missing '=' in setting line")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "wrong-format.conf"};
      const std::string content = "forumhttps://forum.example.com/nay/\n";
      REQUIRE( writeConfiguration(path, content) );
      FileGuard guard{path};

      Config conf;
      REQUIRE_FALSE( conf.loadFromFile(path.string()) );
    }

    SECTION("invalid: same smilie code occurs twice")
    {
      const std::filesystem::path path{std::filesystem::temp_directory_path() / "duplicate-smilie-code.conf"};
      const std::string content = "forum=https://forum.example.com/\nsmilie_r=:)=smile.png\nsmilie_r=:)=smile2.png\n";
      REQUIRE( writeConfiguration(path, content) );
      FileGuard guard{path};

      Config conf;
      REQUIRE_FALSE( conf.loadFromFile(path.string()) );
    }
  }
}
