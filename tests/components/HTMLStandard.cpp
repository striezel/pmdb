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
#include "../../code/HTMLStandard.hpp"

TEST_CASE("HTMLStandard")
{
  SECTION("doctype")
  {
    SECTION("HTML 4.01")
    {
      const auto doc = doctype(HTMLStandard::HTML4_01);
      REQUIRE( doc.find("<!DOCTYPE HTML") != std::string::npos );
      REQUIRE( doc.find("-//W3C//DTD HTML 4.01 Transitional//EN") != std::string::npos );
      REQUIRE( doc.find("http://www.w3.org/TR/html4/loose.dtd") != std::string::npos );
    }

    SECTION("XHTML")
    {
      const auto doc = doctype(HTMLStandard::XHTML);
      REQUIRE( doc.find("<!DOCTYPE ") != std::string::npos );
      REQUIRE( doc.find("-//W3C//DTD XHTML 1.0 Transitional//EN") != std::string::npos );
      REQUIRE( doc.find("http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd") != std::string::npos );
    }
  }
}
