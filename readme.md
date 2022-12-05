# pmdb

[![GitLab pipeline status](https://gitlab.com/striezel/pmdb/badges/master/pipeline.svg)](https://gitlab.com/striezel/pmdb/-/pipelines)
[![GitHub CI Clang status](https://github.com/striezel/pmdb/workflows/Clang/badge.svg)](https://github.com/striezel/pmdb/actions)
[![GitHub CI GCC status](https://github.com/striezel/pmdb/workflows/GCC/badge.svg)](https://github.com/striezel/pmdb/actions)

pmdb is (or better: will be) a small command-line tool that can be used to
"manage" private messages that were imported from vBulletin via an XML file.

TODO: Add more information.

## Building from source

### Prerequisites

To build pmdb from source you need a C++ compiler with support for C++17,
CMake 3.8 or later, the libxml2 library and the zlib library.

It also helps to have Git, a distributed version control system, on your build
system to get the latest source code directly from the Git repository.

All that can usually be installed be typing

    apt-get install cmake g++ git libxml2-dev zlib1g-dev

or

    yum install cmake gcc-c++ git libxml2-devel zlib-devel

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
