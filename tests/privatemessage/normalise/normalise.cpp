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
#include <vector>
#include <utility> //for std::pair
#include "../../../code/PrivateMessage.hpp"

int main()
{
  PrivateMessage pmOne;
  pmOne.setDatestamp("2007-06-14 12:34");
  pmOne.setTitle("This is the title");
  pmOne.setFromUser("Hermes");
  pmOne.setFromUserID(234);
  pmOne.setToUser("Poseidon");

  //list of tests:
  //    first element  = orig. message
  //    second element = expected normalised message
  std::vector<std::pair<std::string, std::string> > tests;
  //empty message
  tests.push_back(std::make_pair("", ""));
  // random message without line breaks
  tests.push_back(std::make_pair("This is some random text.", "This is some random text."));
  // just a normal Unix line break - should not change
  tests.push_back(std::make_pair("\n", "\n"));
  // normal Windows line break - should be replaced by new line only
  tests.push_back(std::make_pair("\r\n", "\n"));
  // text with both kinds of line breaks
  tests.push_back(std::make_pair("Foo.\nBar.\r\nBoom!", "Foo.\nBar.\nBoom!"));
  // multiple Unix line breaks - should not change
  tests.push_back(std::make_pair("\n\n\n\n\n\n\n\n\n\n", "\n\n\n\n\n\n\n\n\n\n"));
  // multiple Windows-style line breaks - should get converted to Unix style
  tests.push_back(std::make_pair("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n", "\n\n\n\n\n\n\n\n"));
  // example text with Unix line breaks
  tests.push_back(std::make_pair(std::string("Lorem ipsum dolor sit amet,\n")
                                +"consetetur sadipscing elitr,\n"
                                +"sed diam nonumy eirmod tempor\n"
                                +"invidunt ut labore et dolore magna aliquyam erat,\n"
                                +"sed diam voluptua.\n"
                                +"\n"
                                +"At vero eos et accusam et justo duo dolores et ea rebum.\n"
                                +"Stet clita kasd gubergren,\n"
                                +"no sea takimata sanctus est Lorem ipsum dolor sit amet.",

                                std::string("Lorem ipsum dolor sit amet,\n")
                                +"consetetur sadipscing elitr,\n"
                                +"sed diam nonumy eirmod tempor\n"
                                +"invidunt ut labore et dolore magna aliquyam erat,\n"
                                +"sed diam voluptua.\n"
                                +"\n"
                                +"At vero eos et accusam et justo duo dolores et ea rebum.\n"
                                +"Stet clita kasd gubergren,\n"
                                +"no sea takimata sanctus est Lorem ipsum dolor sit amet."));
  // example text with Windows line breaks
  tests.push_back(std::make_pair(std::string("Lorem ipsum dolor sit amet,\r\n")
                                +"consetetur sadipscing elitr,\r\n"
                                +"sed diam nonumy eirmod tempor\r\n"
                                +"invidunt ut labore et dolore magna aliquyam erat,\r\n"
                                +"sed diam voluptua.\r\n"
                                +"\r\n"
                                +"At vero eos et accusam et justo duo dolores et ea rebum.\r\n"
                                +"Stet clita kasd gubergren,\r\n"
                                +"no sea takimata sanctus est Lorem ipsum dolor sit amet.",

                                std::string("Lorem ipsum dolor sit amet,\n")
                                +"consetetur sadipscing elitr,\n"
                                +"sed diam nonumy eirmod tempor\n"
                                +"invidunt ut labore et dolore magna aliquyam erat,\n"
                                +"sed diam voluptua.\n"
                                +"\n"
                                +"At vero eos et accusam et justo duo dolores et ea rebum.\n"
                                +"Stet clita kasd gubergren,\n"
                                +"no sea takimata sanctus est Lorem ipsum dolor sit amet."));

  // Iterate through all examples and compare output with expected output.
  std::vector<std::pair<std::string, std::string> >::const_iterator iter = tests.begin();
  while (iter != tests.end())
  {
    pmOne.setMessage(iter->first);
    pmOne.normalise();
    if (pmOne.getMessage() != iter->second)
    {
      std::cout << "Error: normalise() did not produce the expected result!\n"
                << "Original message: \"" << iter->first << "\"\n"
                << "Expected message: \"" << iter->second << "\"\n"
                << "Actual message:   \"" << pmOne.getMessage() << "\"\n";
      return 1;
    }//if
    ++iter;
  } //while

  std::cout << "Info: Passed all tests for PrivateMessage::normalise()!\n";
  return 0;
}
