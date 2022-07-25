<<<<<<< HEAD
Dogecoin Core version 1.14.5 is now available from:

  <https://github.com/dogecoin/dogecoin/releases/tag/v1.14.5/>

This is a new minor version release, including important security updates and
changes to network policies. All Dogecoin Core users, miners, services, relay
operators and wallet users are strongly recommended to upgrade.
=======
0.21.2rc2 Release Notes
====================

Bitcoin Core version 0.21.2rc2 is now available from:

  <https://bitcoincore.org/bin/bitcoin-core-0.21.2/test.rc2/>

This minor release includes various bug fixes and performance
improvements, as well as updated translations.
>>>>>>> 1.21-dev

Please report bugs using the issue tracker at GitHub:

  <https://github.com/dogecoin/dogecoin/issues>

To receive security and update notifications, please watch reddit or Twitter:

  * https://www.reddit.com/r/dogecoindev/
  * @Dogecoin on Twitter for high priority announcements
  * @dogecoin\_devs on Twitter for updates on development work

How to Upgrade
==============

If you are running an older version, shut it down. Wait until it has completely
shut down (which might take a few minutes in some cases), then run the
installer (on Windows) or just copy over `/Applications/Bitcoin-Qt` (on Mac)
or `bitcoind`/`bitcoin-qt` (on Linux).

Upgrading directly from a version of Bitcoin Core that has reached its EOL is
possible, but it might take some time if the data directory needs to be migrated. Old
wallet versions of Bitcoin Core are generally supported.

Compatibility
==============

<<<<<<< HEAD
Dogecoin Core is extensively tested on Ubuntu Server LTS, Intel-based macOS
and Windows 10.

Dogecoin Core should also work on most other Unix-like systems but is not
frequently tested on them.

Notable changes
===============

Important Security Updates
--------------------------

This release contains fixes for 2 high severity vulnerabilities that affect
most Dogecoin Core users.

### Remote Code Execution in Dogecoin QT (CVE-2021-3401)

This release addresses CVE-2021-3401 that opened potential remote code execution
on QT (graphical user interface) wallets through malicious use of
`dogecoin:` URIs.

**Dogecoin QT users are urged to please update their installations to this
version immediately**, to prevent malicious actors from exploiting this
vulnerability.

### Sensitive Information Exposure on Unix platforms (CVE-2019-15947)

A fix for CVE-2019-15947 was back-ported from Bitcoin Core to prevent potential
leakage of sensitive information when Dogecoin Core crashes on Unix platforms.
The vulnerability is patched for systems that run a Linux kernel equal to or
higher than 3.4.

**Dogecoin Core wallet users on Linux platforms are urged to please update to
this version.**

Fee Reductions
--------------

This release finalizes a new minimum fee recommendation for all participants on
the Dogecoin network, following the reduction of relay and mining defaults in
1.14.4. The recommendation has been documented and can be found
[here](fee-recommendation.md). With this release, the minimum fees when creating
transactions are recommended to be as follows:

* the recommended minimum transaction fee is 0.01 DOGE/kb, and
* the recommended dust limit is 1 DOGE, and
* the recommended RBF increment is 0.001 DOGE.

### Wallet/UI Changes

* The user interface for selecting fees when transacting DOGE has been updated
  to give an idea of how much is being spent, rather than a block target. As
  Dogecoin blocks are not full, typically all transactions are mined in the next
  block, and therefore the target estimation does not makes sense for Dogecoin.
* Transaction sizes are no longer rounded up to the nearest kilobyte before
  calculating fees, which significantly simplifies fee calculation logic and
  makes it more similar to Bitcoin and Litecoin.
* The default minimum transaction fee is now 0.01 DOGE per kilobyte. Note that
  you may see transactions take longer to be confirmed while using these lower
  fees, until all miners have updated. The new fee slider can help with getting
  fast-confirming transactions by sliding it all the way to the maximum, or for
  both CLI and GUI wallet users, this can be made the default by setting
  `-paytxfee=5.21`.
* Introduce `-discardthreshold`, a wallet-specific, configurable dust limit that
  enables gradual implementation of the dust limit on the network side. Each
  transaction created with the wallet will adhere to this threshold
  rather than the dust limits used for relay, preventing stuck transactions. The
  wallet will discard any change to fee and reject output amounts that are lower
  than this limit. Until this release sees significant network adoption, the
  default dust limit is recommended to stay at 1 DOGE, as versions 1.14.2 until
  1.14.4 have a bug that rejects any transaction with an output under 1 DOGE.
* Derive minimum change from configurable wallet parameters `-discardthreshold`
  and `-mintxfee`: `minimum change = discard threshold + 2 * minimum fee`.

### Relay changes

* Split the dust limit into a hard and soft threshold, to reintroduce the
  economic disincentive for dust, rather than rejection introduced since 1.14.2
  * `-harddustlimit` is by default set at 0.001 DOGE and sets the value under
    which transactions will be rejected by nodes.
  * The dust limit parameter introduced with 1.14.4 (`-dustlimit`) is now the
    soft dust limit, enforcing the economic disincentive. Each output under this
    threshold will be accepted as long as the entire limit is added to fee.
* Change the default incremental fee used for RBF and mempool limiting to
  0.0001 DOGE.

BDB Updated to 5.3
------------------

The Berkley DB version used by Dogecoin Core has been updated to 5.3 (from 5.1)
as 5.3 is now standard on many Linux distributions. 5.1 and 5.3 wallet files
have been tested to be interchangeable.

Version display
---------------
=======
Bitcoin Core is supported and extensively tested on operating systems
using the Linux kernel, macOS 10.12+, and Windows 7 and newer.  Bitcoin
Core should also work on most other Unix-like systems but is not as
frequently tested on them.  It is not recommended to use Bitcoin Core on
unsupported systems.

From Bitcoin Core 0.20.0 onwards, macOS versions earlier than 10.12 are no
longer supported. Additionally, Bitcoin Core does not yet change appearance
when macOS "dark mode" is activated.


0.21.2rc2 change log
=================

### P2P protocol and network code

- #21644 use NetPermissions::HasFlag() in CConnman::Bind() (jonatack)
- #22569 Rate limit the processing of rumoured addresses (sipa)
>>>>>>> 1.21-dev

The version displayed on QT's overview page has been changed to display the
full version rather than just the major version part, because this was confusing
wallet users.

<<<<<<< HEAD
Key Derivation
--------------

The BIP32 hierarchical deterministic key derivation path contained the wrong
chain ID. Previously the chain ID 0 was used, it's now correctly set to 3 as
per [SLIP44](https://github.com/satoshilabs/slips/blob/master/slip-0044.md).

The wallet.dat files stay fully interoperable between versions. Wallets created
with 1.14.5 will benefit from greater interoperability with hardware wallets in
the future.

Namecoin-compatibile AuxPoW mining
----------------------------------
=======
- #21907 Do not iterate a directory if having an error while accessing it (hebasto)

### RPC

- #19361 Reset scantxoutset progress before inferring descriptors (prusnak)

### Build System

- #21932 depends: update Qt 5.9 source url (kittywhiskers)
- #22017 Update Windows code signing certificate (achow101)
- #22191 Use custom MacOS code signing tool (achow101)
- #22713 Fix build with Boost 1.77.0 (sizeofvoid)

### Tests and QA

- #20182 Build with --enable-werror by default, and document exceptions (hebasto)
- #20535 Fix intermittent feature_taproot issue (MarcoFalke)
- #21663 Fix macOS brew install command (hebasto)
- #22279 add missing ECCVerifyHandle to base_encode_decode (apoelstra)
- #22730 Run fuzzer task for the master branch only (hebasto)
>>>>>>> 1.21-dev

The `createauxblock` and `submitauxblock` commands have been reintroduced,
mimicking the same commands from Namecoin 0.17, allowing miners to separate
wallets from block producing nodes by specifying the address for their coinbase
transactions.

<<<<<<< HEAD
Two additional features on top of the Namecoin 0.17 API have been added:
=======
- #277 Do not use QClipboard::Selection on Windows and macOS. (hebasto)
- #280 Remove user input from URI error message (prayank23)
- #365 Draw "eye" sign at the beginning of watch-only addresses (hebasto)
>>>>>>> 1.21-dev

* The block caching mechanism has been enhanced to enable mining pools to use
  multiple wallet addresses if desired.
* By default the AuxPoW API methods provide the difficulty target in a field
  named `target`, however this can now be configured to be fully compatible with
  the Namecoin API (`_target`) by setting the `-rpcnamecoinapi` argument.

RPC API Changes
---------------

* Added `softdustlimit` and `harddustlimit` fields to `getnetworkinfo` to enable
  operators and third party scripts to query this information without having to
  search configuration files or hardcode defaults.
* Added `createauxblock` and `submitauxblock` methods
* Added `-rpcnamecoinapi` that allows miners to use Namecoin-compatible AuxPoW
  APIs, for both `getauxblock` and `createauxblock` methods.

Build System and CI Changes
---------------------------

The build system for dependencies, continuous integration and binary releases
has been upgraded from Ubuntu Trusty to Ubuntu Bionic, because the former was
fully end-of-life. Ubuntu Bionic extends the useful life of the 1.14 build
system to April 2023, by which time we expect to have switched to 1.21 as the
main version. With this change, the default gcc used for testing and releases
has been updated from version 4.8 to 7.

The CI environment has been extended to build and test aarch64 binaries, and to
perform additional checks that allow us to catch more potential issues early and
automatically.

Additionally, an experimental CI build environment has been introduced to enable
ongoing testing and maintenance of incubating features that are not yet ready
for release. Currently this contains the AVX2 features that aim to increase the
performance of cryptographic routines within Dogecoin Core.

Minor Changes
=============

* Fix compilation on FreeBSD, which was failing to compile the Scrypt code.
* Update the FreeBSD build docs, see `doc/build-freebsd.md`.
* Update default dependencies to OpenSSL 1.0.2u.
* Refresh translation files to simplify volunteer contributions to translations.
* Add xkbcommon 0.8.4 as a separate dependency to fix keyboard compatibility
  issues and resolve issues with inadvertently used build system libraries.
* Harden and expand the recommended systemd unit files in `contrib/init`.
* Make the Freetype dependency compile independent from build system libraries.
* Update the Univalue library to use the latest version maintained by the
  Bitcoin Core developers.
* Fix the pruning test suite.
* Correct the block download timeout for the regtest chain.
* Shut down when trying to use a corrupted block from disk.
* Add experimental AVX2 support, to improve the performance of SHA operations.
* Add a [getting started guide](getting-started.md)

- #22002 Fix crash when parsing command line with -noincludeconf=0 (MarcoFalke)
- #22137 util: Properly handle -noincludeconf on command line (take 2) (MarcoFalke)


Credits
=======

<<<<<<< HEAD
* AbcSxyZ
* Bertrand Jacquin
* Carl Dong
* cg
* CharesFang
* chromatic
* Chun Kuan Lee
* Cory Fields
* Dakoda Greaves
* Daksh Sharma
* Dan Raviv
* dogespacewizard
* Ed Tubbs
* Elvis Begović
* fanquake
* Hennadii Stepanov
* KabDeveloper
* leezhen
* Luke Dashjr
* Micael Malta
* Michi Lumin
* Patrick Lodder
* Ross Nicoll
* Ryan Crosby
* Suhas Daftuar
* Vasil Dimov
* W. J. van der Laan
* Xiao Yi
=======
Thanks to everyone who directly contributed to this release:

- Andrew Chow
- Andrew Poelstra
- fanquake
- Hennadii Stepanov
- Jon Atack
- Kittywhiskers Van Gogh
- Luke Dashjr
- MarcoFalke
- Pavol Rusnak
- Pieter Wuille
- prayank23
- Rafael Sadowski
- W. J. van der Laan


As well as to everyone that helped with translations on
[Transifex](https://www.transifex.com/bitcoin/bitcoin/).
>>>>>>> 1.21-dev
