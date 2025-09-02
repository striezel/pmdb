#!/bin/bash

#  create-package.sh - script to create *.deb package from latest Git commit
#
#  Copyright (C) 2019, 2022, 2025  Dirk Stolle
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

# Gets the latest tag.
TAG="${CI_COMMIT_TAG:-$(git describe | cut -d '-' -f 1)}"
# Version is the same as the tag, just without the leading 'v'.
VERSION=$(echo $TAG | cut -c '2-')
ARCH=$(dpkg --print-architecture)
COMMIT_HASH=$(git rev-parse HEAD)

echo "Info: Tag is $TAG."
echo "Info: Version is $VERSION."
echo "Info: Architecture is $ARCH."
echo "Info: Commit hash is $COMMIT_HASH."

# GitLab download:
# wget -O pmdb_${VERSION}.orig.tar.bz2 https://gitlab.com/striezel/pmdb/-/archive/${COMMIT_HASH}/pmdb-${COMMIT_HASH}.tar.bz2
# tar -x --bzip2 -f pmdb_${VERSION}.orig.tar.bz2
# GitHub download:
# wget -O pmdb_${VERSION}.orig.tar.gz https://github.com/striezel/pmdb/archive/${COMMIT_HASH}.tar.gz
# tar -x --gzip -f pmdb_${VERSION}.orig.tar.gz

# Downloads do not contain submodules, so we build an archive ourselves with
# the help of git archive and tar.
echo "Creating main archive ..."
git archive --verbose --prefix "pmdb-archive/" --format tar --output "/tmp/pmdb-main-archive.tar" $COMMIT_HASH
if [[ $? -ne 0 ]]
then
  echo "ERROR: Archive creation (main) failed!"
  exit 1
fi

echo "Creating submodule archive(s) ..."
git submodule foreach --recursive 'git archive --verbose --prefix=pmdb-archive/$path/ --format tar HEAD --output /tmp/pmdb-module-archive-$sha1.tar'
if [[ $? -ne 0 ]]
then
  echo "ERROR: Archive creation (submodule) failed!"
  exit 1
fi

# Delete empty directory.
tar --delete -f /tmp/pmdb-main-archive.tar pmdb-archive/libstriezel
if [[ $? -ne 0 ]]
then
  echo "ERROR: Directory could not be deleted from archive!"
  exit 1
fi

tar --concatenate --ignore-zeros --file /tmp/pmdb-main-archive.tar /tmp/pmdb-module-archive*.tar
if [[ $? -ne 0 ]]
then
  echo "ERROR: Archive concatenation failed!"
  exit 1
fi

echo "Info: Archive contents (pmdb-main-archive.tar):"
tar tf /tmp/pmdb-main-archive.tar

bzip2 --keep /tmp/pmdb-main-archive.tar && cp /tmp/pmdb-main-archive.tar.bz2 ./pmdb_${VERSION}.orig.tar.bz2
if [[ $? -ne 0 ]]
then
  echo "ERROR: Archive compression or copy failed!"
  exit 1
fi
# Archive creation done.

tar -x --bzip2 -f pmdb_${VERSION}.orig.tar.bz2
if [[ $? -ne 0 ]]
then
  echo "ERROR: Archive extraction failed!"
  exit 1
fi

mv pmdb-archive pmdb-${VERSION}
if [[ $? -ne 0 ]]
then
  echo "ERROR: Could not move extracted files!"
  exit 1
fi

cd pmdb-${VERSION}
if [[ $? -ne 0 ]]
then
  echo "ERROR: Could not change to extracted directory"
  exit 1
fi

# build the package
debuild -uc -us
if [[ $? -ne 0 ]]
then
  echo "ERROR: Package build failed!"
  exit 1
fi

cd ..
RELEASE=$(lsb_release -cs)
mv pmdb*.deb pmdb_${VERSION}-${RELEASE}_${ARCH}.deb
