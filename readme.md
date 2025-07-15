# pmdb

[![GitLab pipeline status](https://gitlab.com/striezel/pmdb/badges/master/pipeline.svg)](https://gitlab.com/striezel/pmdb/-/pipelines)
[![GitHub CI Clang status](https://github.com/striezel/pmdb/workflows/Clang/badge.svg)](https://github.com/striezel/pmdb/actions)
[![GitHub CI GCC status](https://github.com/striezel/pmdb/workflows/GCC/badge.svg)](https://github.com/striezel/pmdb/actions)
[![GitHub CI MSYS2 status](https://github.com/striezel/pmdb/workflows/MSYS2/badge.svg)](https://github.com/striezel/pmdb/actions)

pmdb is (or better: will be) a small command-line tool that can be used to
"manage" private messages that were imported from vBulletin via an XML file.

TODO: Add more information.

## Usage

```
pmdb

Manages private messages exported from vBulletin in XML format.

options:
  --help           - Displays this help message and quits.
  -?               - same as --help
  --version        - Displays the version of the program and quits.
  -v               - same as --version
  -xml FILENAME    - Sets the name of the XML file that contains the private
                     messages to FILENAME. Must not be omitted.
  --xml=FILENAME   - same as -xml
  --load           - Tries to load messages from the default directory.
  --load=DIR       - Tries to load all messages saved in the directory DIR.
                     This option can be given more than once, however the
                     directory has to be different every time.
  --save           - All messages will be saved after the XML files were read
                     and the messages from the load directories have been
                     loaded. Enabled by default.
  --no-save        - Prevents the program from saving any read messages.
                     Mutually exclusive with --save.
  --compress       - Save and load operations (see --save and --load) will use
                     compression, i.e. messages are compressed using zlib
                     before they are saved to files, and they will be decom-
                     pressed when they are loaded from files. By default,
                     messages will NOT be compressed for backwards compatibi-
                     lity with earlier pmdb versions.
  --no-save-check  - This option prevents the program from checking the
                     compression status of messages when saving to an existing
                     directory. Note that this is not recommended, because it
                     could result in a mixup where a directory contains both
                     compressed and uncompressed messages, making some of the
                     messages unreadable by the program.
  --html           - Creates HTML files for every message.
  --xhtml          - Like --html, but use XHTML instead of HTML.
  --no-br          - Do not convert new line characters to line breaks in
                     (X)HTML output.
  --no-list        - Do not parse [LIST] codes when creating HTML files.
  --table=CLASS    - Sets the class for grids in <table> to CLASS.
                     Must occur together with --row and --cell.
  --row=CLASS      - Sets the class for grids in <tr> to CLASS.
                     Must occur together with --table and --cell.
  --cell=CLASS     - Sets the class for grids in <td> to CLASS.
                     Must occur together with --table and --row.
  --std-classes    - Sets the 'standard' classes for the three class options.
                     This is equivalent to specifying all these parameters:
                         --table=grid_table
                         --row=grid_tr
                         --cell=grid_td
  --subset-check   - Search for messages with texts that are completely
                     contained in other messages, too.
  --list-from X    - List all messages that were sent by user X, where X stands
                     for the name of the user (not the numeric user id).
                     Can occur multiple times for more than one user.
  --list-to X      - List all messages that were sent to user X, where X stands
                     for the name of the user (not the numeric user id).
                     Can occur multiple times for more than one user.
```

## Building from source

### Prerequisites

To build pmdb from source you need a C++ compiler with support for C++17,
CMake 3.8 or later, the libxml2 library and the zlib library. Additionally, the
program uses Catch (C++ Automated Test Cases in Headers) to perform some tests.

It also helps to have Git, a distributed version control system, on your build
system to get the latest source code directly from the Git repository.

All that can usually be installed by typing

    apt-get install catch cmake g++ git libxml2-dev zlib1g-dev

or

    yum install catch cmake gcc-c++ git libxml2-devel zlib-devel

into a root terminal.

### Getting the source code

Get the source directly from Git by cloning the Git repository and change to
the directory after the repository is completely cloned:

    git clone https://gitlab.com/striezel/pmdb.git ./pmdb
    cd pmdb
    git submodule update --init --recursive

The last of the lines above initializes and updates the submodule that the
pmdb source code needs, too, to be build from source.

### Build process

The build process is relatively easy, because CMake does all the preparations.
Starting in the root directory of the source, you can do the following steps:

    mkdir build
    cd build
    cmake ../
    make pmdb -j2

Now the pmdb binary is built and ready for use.

## Building from source on Windows

For information how to build pmdb on Windows, see the
[MSYS2 build instructions](./documentation/msys2-build.md).

## Copyright and Licensing

Copyright 2012-2015 Dirk Stolle

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
