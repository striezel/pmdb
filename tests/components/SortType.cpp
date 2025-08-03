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
#include "../../code/SortType.hpp"

TEST_CASE("SortType")
{
  SECTION("constructor")
  {
    SHA256::MessageDigest digest;
    REQUIRE( digest.fromHexString("affe00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
    SortType instance("2018-07-01 16:13", digest);

    REQUIRE( instance.datestamp == "2018-07-01 16:13" );
    REQUIRE( instance.md.toHexString() == "affe00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe" );
  }

  SECTION("constructor with changed parameter order")
  {
    SHA256::MessageDigest digest;
    REQUIRE( digest.fromHexString("affe00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
    SortType instance(digest, "2018-07-01 16:13");

    REQUIRE( instance.datestamp == "2018-07-01 16:13" );
    REQUIRE( instance.md.toHexString() == "affe00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe" );
  }

  SECTION("operator <")
  {
    SECTION("less than - different date stamp, different hash")
    {
      SHA256::MessageDigest digest_one;
      REQUIRE( digest_one.fromHexString("affe00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType a("2018-07-01 16:13", digest_one);

      SHA256::MessageDigest digest_two;
      REQUIRE( digest_two.fromHexString("ffff00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType b("2018-12-31 23:59", digest_two);

      REQUIRE( a < b );
      REQUIRE_FALSE( b < a );
    }

    SECTION("less than - same date stamp, different hash")
    {
      SHA256::MessageDigest digest_one;
      REQUIRE( digest_one.fromHexString("affe00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType a("2018-07-01 16:13", digest_one);

      SHA256::MessageDigest digest_two;
      REQUIRE( digest_two.fromHexString("ffff00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType b("2018-07-01 16:13", digest_two);

      REQUIRE( a < b );
      REQUIRE_FALSE( b < a );
    }

    SECTION("equal - same date stamp, same hash")
    {
      SHA256::MessageDigest digest;
      REQUIRE( digest.fromHexString("affe00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType a("2018-07-01 16:13", digest);
      const SortType b("2018-07-01 16:13", digest);

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );
    }

    SECTION("greater than - different date stamp, different hash")
    {
      SHA256::MessageDigest digest_one;
      REQUIRE( digest_one.fromHexString("affe00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType a("2018-12-31 23:59", digest_one);

      SHA256::MessageDigest digest_two;
      REQUIRE( digest_two.fromHexString("ffff00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType b("2018-07-01 16:13", digest_two);

      REQUIRE_FALSE( a < b );
      REQUIRE( b < a );
    }

    SECTION("greater than - same date stamp, different hash")
    {
      SHA256::MessageDigest digest_one;
      REQUIRE( digest_one.fromHexString("ffff00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType a("2025-01-01 01:01", digest_one);

      SHA256::MessageDigest digest_two;
      REQUIRE( digest_two.fromHexString("affe00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType b("2025-01-01 01:01", digest_two);

      REQUIRE_FALSE( a < b );
      REQUIRE( b < a );
    }
  }

  SECTION("ST_greater")
  {
    SECTION("less than - different date stamp, different hash")
    {
      SHA256::MessageDigest digest_one;
      REQUIRE( digest_one.fromHexString("affe00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType a("2018-07-01 16:13", digest_one);

      SHA256::MessageDigest digest_two;
      REQUIRE( digest_two.fromHexString("ffff00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType b("2018-12-31 23:59", digest_two);

      REQUIRE_FALSE( ST_greater(a, b) );
      REQUIRE( ST_greater(b, a) );
    }

    SECTION("less than - same date stamp, different hash")
    {
      SHA256::MessageDigest digest_one;
      REQUIRE( digest_one.fromHexString("affe00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType a("2018-07-01 16:13", digest_one);

      SHA256::MessageDigest digest_two;
      REQUIRE( digest_two.fromHexString("ffff00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType b("2018-07-01 16:13", digest_two);

      REQUIRE_FALSE( ST_greater(a, b) );
      REQUIRE( ST_greater(b, a) );
    }

    SECTION("equal - same date stamp, same hash")
    {
      SHA256::MessageDigest digest;
      REQUIRE( digest.fromHexString("affe00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType a("2018-07-01 16:13", digest);
      const SortType b("2018-07-01 16:13", digest);

      REQUIRE_FALSE( ST_greater(a, b) );
      REQUIRE_FALSE( ST_greater(b, a) );
    }

    SECTION("greater than - different date stamp, different hash")
    {
      SHA256::MessageDigest digest_one;
      REQUIRE( digest_one.fromHexString("affe00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType a("2018-12-31 23:59", digest_one);

      SHA256::MessageDigest digest_two;
      REQUIRE( digest_two.fromHexString("ffff00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType b("2018-07-01 16:13", digest_two);

      REQUIRE( ST_greater(a, b) );
      REQUIRE_FALSE( ST_greater(b, a) );
    }

    SECTION("greater than - same date stamp, different hash")
    {
      SHA256::MessageDigest digest_one;
      REQUIRE( digest_one.fromHexString("ffff00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType a("2025-01-01 01:01", digest_one);

      SHA256::MessageDigest digest_two;
      REQUIRE( digest_two.fromHexString("affe00affe01affe02affe03affe04affe05affe06affe07affe08affe09affe") );
      const SortType b("2025-01-01 01:01", digest_two);

      REQUIRE( ST_greater(a, b) );
      REQUIRE_FALSE( ST_greater(b, a) );
    }
  }
}
