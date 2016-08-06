#!/bin/bash

# This file is part of the test suite for Private Message Database.
# Copyright (C) 2015  Dirk Stolle
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


# check parameter
if [[ -z $1 ]]
then
  echo "Error: Expecting one parameter - executable for this test!"
  return 1
fi

# create base directory where the test shall take place
SAVE_DIR=`mktemp --directory --tmpdir test-mdb-save-loadXXXXXXXXXX`

# run test
$1 $SAVE_DIR
# ...and save its exit code
TEST_EXIT_CODE_FIRST=$?

# run test with slash at end of path
$1 $SAVE_DIR/
# ...and save its exit code
TEST_EXIT_CODE_SECOND=$?

TOTAL=0

if [[ $TEST_EXIT_CODE_FIRST -ne 0 ]]
then
  echo "ERROR: The first part of the MessageDatabase save/load test (path without slash) failed!"
  TOTAL=1
fi

if [[ $TEST_EXIT_CODE_SECOND -ne 0 ]]
then
  echo "ERROR: The second part of the MessageDatabase save/load test (path with slash) failed!"
  TOTAL=2
fi

# clean up test directory
rm -rf $SAVE_DIR

if [[ $TOTAL -eq 0 ]]
then
  exit 0
else
  exit 1
fi
