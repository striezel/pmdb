#!/bin/sh

# Script to test wrong values of parameters.
#
#  Copyright (C) 2025  Dirk Stolle
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

# 1st parameter = executable path
if [ -z "$1" ]
then
  echo "First parameter must be executable file!"
  exit 1
fi
EXECUTABLE="$1"

# 2nd paramater = path to test-messages.xml
if [ -z "$2" ]
then
  echo "Second parameter must be XML file!"
  exit 1
fi
XML_FILE="$2"

# --xml: file does not exist
"$EXECUTABLE" --no-save --no-load-default --xml does-not-exist.xml
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 for non-existent XML file."
  exit 1
fi

# --xml: missing file name
"$EXECUTABLE" --no-save --no-load-default --xml
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when XML file name was missing."
  exit 1
fi

# --xml: same file given twice
"$EXECUTABLE" --no-save --no-load-default --xml "$XML_FILE" --xml "$XML_FILE"
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when same XML file was given twice."
  exit 1
fi

# --xml=file.xml: file does not exist
"$EXECUTABLE" --no-save --no-load-default --xml=does-not-exist.xml
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 for non-existent XML file."
  exit 1
fi

# --xml=file.xml: same file given twice
"$EXECUTABLE" --no-save --no-load-default "--xml=$XML_FILE" "--xml=$XML_FILE"
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when same XML file was given twice."
  exit 1
fi

# --save: parameter given twice
"$EXECUTABLE" --no-load-default --xml "$XML_FILE" --save --save
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --save was given twice."
  exit 1
fi

# --save and --no-save at the same time: conflict
"$EXECUTABLE" --no-load-default --xml "$XML_FILE" --save --no-save
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --save and --no-save were given."
  exit 1
fi

# --no-save: parameter given twice
"$EXECUTABLE" --no-load-default --xml "$XML_FILE" --no-save --no-save
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --no-save was given twice."
  exit 1
fi

# --html: parameter given twice
"$EXECUTABLE" --no-save --no-load-default --xml "$XML_FILE" --html --html
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --html was given twice."
  exit 1
fi

# --xhtml: parameter given twice
"$EXECUTABLE" --no-save --no-load-default --xml "$XML_FILE" --xhtml --xhtml
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --xhtml was given twice."
  exit 1
fi

# --html and --xhtml at the same time: conflict
"$EXECUTABLE" --no-save --no-load-default --xml "$XML_FILE" --html --xhtml
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --html and --xhtml were given."
  exit 1
fi

# --load-default: parameter given twice
"$EXECUTABLE" --no-save --xml "$XML_FILE" --load-default --load-default
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --load-default was given twice."
  exit 1
fi

# --no-load-default: parameter given twice
"$EXECUTABLE" --no-save --xml "$XML_FILE" --no-load-default --no-load-default
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --no-load-default was given twice."
  exit 1
fi

# --no-load-default and --load-default: conflict
"$EXECUTABLE" --no-save --xml "$XML_FILE" --no-load-default --load-default
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --no-load-default and --load-default were given."
  exit 1
fi

# --no-br: parameter given twice
"$EXECUTABLE" --no-save --no-load-default --xml "$XML_FILE" --no-br --no-br
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --no-br was given twice."
  exit 1
fi

# --no-list: parameter given twice
"$EXECUTABLE" --no-save --no-load-default --xml "$XML_FILE" --no-list --no-list
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --no-list was given twice."
  exit 1
fi

# --compress: parameter given twice
"$EXECUTABLE" --no-save --no-load-default --xml "$XML_FILE" --compress --compress
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --compress was given twice."
  exit 1
fi

echo Parameter tests succeeded.

exit 0
