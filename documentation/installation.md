# Installation

Currently, there is no easy way to install pmdb. It has to be
[compiled from the source code](https://github.com/striezel/pmdb?tab=readme-ov-file#building-from-source),
which is a bit tedious. In the future, pre-compiled versions will be available
on the [release page of the pmdb project](https://github.com/striezel/pmdb/releases).

## Windows

For Windows, a ZIP file containing all the required files is provided.

* __pmdb\_\<version\>.zip__ - binaries for 64-bit Windows systems, just extract
  to a location of your choice and run it from there

Just make sure to keep the `*.exe` and `*.dll` files in the same directory.
Otherwise the executable may not work.

## Linux

### Debian, Ubuntu and Debian-based systems

There are `*.deb` files for various versions of Debian and Ubuntu. If you are
using one of them or a system that is based on one of them, then you can use one
of those packages.

These files can be installed via `dpkg -i pmdb_*.deb` on a Debian system (or a
Debian-based system like Ubuntu). The packages may work on other distribution
versions than the ones mentioned, but they have not been tested on other
distributions.

* __pmdb\_\<version\>-trixie_amd64.deb__ - package to install the pmdb on
  __Debian 13 ("trixie")__ on amd64 architecture
* __pmdb\_\<version\>-trixie_arm64.deb__ - package to install the pmdb on
  __Debian 13 ("trixie")__ on arm64 / aarch64 architecture
* __pmdb\_\<version\>-bookworm_amd64.deb__ - package to install the pmdb on
  __Debian 12 ("bookworm")__ on amd64 architecture
* __pmdb\_\<version\>-bookworm_arm64.deb__ - package to install the pmdb on
  __Debian 12 ("bookworm")__ on arm64 / aarch64 architecture
* __pmdb\_\<version\>-noble_amd64.deb__ - package to install the pmdb on
  __Ubuntu 24.04 ("noble")__ on amd64 architecture
* __pmdb\_\<version\>-noble_arm64.deb__ - package to install the pmdb on
  __Ubuntu 24.04 ("noble")__ on arm64 / aarch64 architecture
* __pmdb\_\<version\>-jammy_amd64.deb__ - package to install the pmdb on
  __Ubuntu 22.04 ("jammy")__ on amd64 architecture
* __pmdb\_\<version\>-jammy_arm64.deb__ - package to install the pmdb on
  __Ubuntu 22.04 ("jammy")__ on arm64 / aarch64 architecture

### Other distributions

For other Linux distributions, there are generic packages:

* __pmdb\_\<version\>\_linux-amd64-generic.tar.gz__ - generic binaries for 64-bit
  Linux systems, should work on most recent Linux distributions using the amd64
  (64-bit AMD / Intel) architecture
* __pmdb\_\<version\>\_linux-arm64-generic.tar.gz__ - generic binaries for 64-bit
  Linux systems, should work on most recent Linux distributions using the arm64
  architecture (aarch64-linux-gnu / aarch64-linux-musl)

## macOS

Currently, there are no packages for macOS, because I don't have a macOS system
to test this on.

If you need ton run pmdb on macOS, you can either try to compile from source or
you could try to run the Windows binaries with the [Wine compatibility layer](https://www.winehq.org/).
