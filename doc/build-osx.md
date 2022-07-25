# macOS Build Instructions and Notes

The commands in this guide should be executed in a Terminal application.
The built-in one is located in
```
/Applications/Utilities/Terminal.app
```

## Preparation
Install the macOS command line tools:

```shell
xcode-select --install
```

When the popup appears, click `Install`.

Then install [Homebrew](https://brew.sh).

## Dependencies
```shell
brew install automake libtool boost miniupnpc pkg-config python qt libevent qrencode
```

<<<<<<< HEAD
    brew install automake libtool boost miniupnpc openssl pkg-config protobuf qt5 libevent
    brew install berkeley-db # You need to make sure you install a version >= 5.3.28, but as close to 5.3.28 as possible. Check the homebrew docs to find out how to install older versions.

If you want to build the disk image with `make deploy` (.dmg / optional), you need RSVG
=======
If you run into issues, check [Homebrew's troubleshooting page](https://docs.brew.sh/Troubleshooting).
See [dependencies.md](dependencies.md) for a complete overview.

If you want to build the disk image with `make deploy` (.dmg / optional), you need RSVG:
```shell
brew install librsvg
```
>>>>>>> 1.21-dev

The wallet support requires one or both of the dependencies ([*SQLite*](#sqlite) and [*Berkeley DB*](#berkeley-db)) in the sections below.
To build Bitcoin Core without wallet, see [*Disable-wallet mode*](#disable-wallet-mode).

#### SQLite

<<<<<<< HEAD
Build Dogecoin Core
------------------------

1. Clone the dogecoin source code and cd into `dogecoin`

        git clone https://github.com/dogecoin/dogecoin
        cd dogecoin

2.  Build dogecoin:

    Configure and build the headless dogecoin binaries as well as the GUI (if Qt is found).
=======
Usually, macOS installation already has a suitable SQLite installation.
Also, the Homebrew package could be installed:

```shell
brew install sqlite
```

In that case the Homebrew package will prevail.

#### Berkeley DB

It is recommended to use Berkeley DB 5.3. If you have to build it yourself,
you can use [this](/contrib/install_db5.sh) script to install it
like so:

```shell
./contrib/install_db5.sh .
```

from the root of the repository.

## Build Bitcoin Core

1. Clone the Bitcoin Core source code:
    ```shell
    git clone https://github.com/bitcoin/bitcoin
    cd bitcoin
    ```

2.  Build Bitcoin Core:

    Configure and build the headless Bitcoin Core binaries as well as the GUI (if Qt is found).
>>>>>>> 1.21-dev

    You can disable the GUI build by passing `--without-gui` to configure.
    ```shell
    ./autogen.sh
    ./configure
    make
    ```

3.  It is recommended to build and run the unit tests:
    ```shell
    make check
    ```

4.  You can also create a  `.dmg` that contains the `.app` bundle (optional):
    ```shell
    make deploy
    ```

## Disable-wallet mode
When the intention is to run only a P2P node without a wallet, Bitcoin Core may be
compiled in disable-wallet mode with:
```shell
./configure --disable-wallet
```

In this case there is no dependency on [*Berkeley DB*](#berkeley-db) and [*SQLite*](#sqlite).

Mining is also possible in disable-wallet mode using the `getblocktemplate` RPC call.

<<<<<<< HEAD
Dogecoin Core is now available at `./src/dogecoind`
=======
## Running
Bitcoin Core is now available at `./src/bitcoind`
>>>>>>> 1.21-dev

Before running, you may create an empty configuration file:
```shell
mkdir -p "/Users/${USER}/Library/Application Support/Bitcoin"

<<<<<<< HEAD
    echo -e "rpcuser=dogecoinrpc\nrpcpassword=$(xxd -l 16 -p /dev/urandom)" > "/Users/${USER}/Library/Application Support/Dogecoin/dogecoin.conf"

    chmod 600 "/Users/${USER}/Library/Application Support/Dogecoin/dogecoin.conf"

The first time you run dogecoind, it will start downloading the blockchain. This process could take several hours.
=======
touch "/Users/${USER}/Library/Application Support/Bitcoin/bitcoin.conf"

chmod 600 "/Users/${USER}/Library/Application Support/Bitcoin/bitcoin.conf"
```

The first time you run bitcoind, it will start downloading the blockchain. This process could
take many hours, or even days on slower than average systems.
>>>>>>> 1.21-dev

You can monitor the download process by looking at the debug.log file:
```shell
tail -f $HOME/Library/Application\ Support/Bitcoin/debug.log
```

<<<<<<< HEAD
    tail -f $HOME/Library/Application\ Support/Dogecoin/debug.log

Other commands:
-------

    ./src/dogecoind -daemon # Starts the dogecoin daemon.
    ./src/dogecoin-cli --help # Outputs a list of command-line options.
    ./src/dogecoin-cli help # Outputs a list of RPC commands when the daemon is running.

Using Qt Creator as IDE
------------------------
You can use Qt Creator as an IDE, for dogecoin development.
Download and install the community edition of [Qt Creator](https://www.qt.io/download/).
Uncheck everything except Qt Creator during the installation process.

1. Make sure you installed everything through Homebrew mentioned above
2. Do a proper ./configure --enable-debug
3. In Qt Creator do "New Project" -> Import Project -> Import Existing Project
4. Enter "dogecoin-qt" as project name, enter src/qt as location
5. Leave the file selection as it is
6. Confirm the "summary page"
7. In the "Projects" tab select "Manage Kits..."
8. Select the default "Desktop" kit and select "Clang (x86 64bit in /usr/bin)" as compiler
9. Select LLDB as debugger (you might need to set the path to your installation)
10. Start debugging with Qt Creator

Notes
-----

* Tested on OS X 10.8 through 10.12 on 64-bit Intel processors only.

* Building with downloaded Qt binaries is not officially supported. See the notes in [#7714](https://github.com/dogecoin/dogecoin/issues/7714)
=======
## Other commands:
```shell
./src/bitcoind -daemon      # Starts the bitcoin daemon.
./src/bitcoin-cli --help    # Outputs a list of command-line options.
./src/bitcoin-cli help      # Outputs a list of RPC commands when the daemon is running.
```

## Notes
* Tested on OS X 10.12 Sierra through macOS 10.15 Catalina on 64-bit Intel
processors only.
* Building with downloaded Qt binaries is not officially supported. See the notes in [#7714](https://github.com/bitcoin/bitcoin/issues/7714).
>>>>>>> 1.21-dev
