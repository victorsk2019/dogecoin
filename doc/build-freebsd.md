<<<<<<< HEAD
Building on FreeBSD
--------------------

**Last tested with:** 1.14.5-dev (as of 18dbe32)
**Tested on:** FreeBSD 11.4

Clang is installed by default as `cc` compiler, this makes it easier to get
started than on other distros. Installing dependencies:

    pkg install autoconf automake libtool pkgconf
    pkg install boost-libs openssl libevent
    pkg install gmake

You need to use GNU make (`gmake`) instead of `make`.
(`libressl` instead of `openssl` will also work)

For the wallet (optional):

    pkg install db5

As of writing, the default hardening routines will fail on the scrypt code, so
currently, no hardened executables can be built, and the `--disable-hardening`
flag is needed for successful compilation.

Then build using:

```bash
  ./autogen.sh
  ./configure --disable-hardening MAKE="gmake" \
      CFLAGS="-I/usr/local/include" CXXFLAGS="-I/usr/local/include -I/usr/local/include/db5" \
      LDFLAGS="-L/usr/local/lib -L/usr/local/lib/db5"
  gmake
```

*Note on debugging*: The version of `gdb` installed by default is [ancient and considered harmful](https://wiki.freebsd.org/GdbRetirement).
It is not suitable for debugging a multi-threaded C++ program, not even for getting backtraces. Please install the package `gdb` and
use the versioned gdb command e.g. `gdb7111`.
=======
FreeBSD build guide
======================
(updated for FreeBSD 12.0)

This guide describes how to build bitcoind and command-line utilities on FreeBSD.

This guide does not contain instructions for building the GUI.

## Preparation

You will need the following dependencies, which can be installed as root via pkg:

```bash
pkg install autoconf automake boost-libs git gmake libevent libtool pkgconf

git clone https://github.com/bitcoin/bitcoin.git
```

In order to run the test suite (recommended), you will need to have Python 3 installed:

```bash
pkg install python3
```

See [dependencies.md](dependencies.md) for a complete overview.

### Building BerkeleyDB

BerkeleyDB is only necessary for the wallet functionality. To skip this, pass
`--disable-wallet` to `./configure` and skip to the next section.

```bash
./contrib/install_db5.sh `pwd`
export BDB_PREFIX="$PWD/db5"
```

## Building Bitcoin Core

**Important**: Use `gmake` (the non-GNU `make` will exit with an error).

With wallet:
```bash
./autogen.sh
./configure --with-gui=no \
    BDB_LIBS="-L${BDB_PREFIX}/lib -ldb_cxx-5.3" \
    BDB_CFLAGS="-I${BDB_PREFIX}/include" \
    MAKE=gmake
```

Without wallet:
```bash
./autogen.sh
./configure --with-gui=no --disable-wallet MAKE=gmake
```

followed by:

```bash
gmake # use -jX here for parallelism
gmake check # Run tests if Python 3 is available
```
>>>>>>> 1.21-dev
