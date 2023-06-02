#!/usr/bin/env bash
#
# Copyright (c) 2019-2020 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C.UTF-8

export HOST=arm64-apple-darwin
#x86_64-apple-darwin22
export PIP_PACKAGES="zmq"
export GOAL="install"
export BDB_PREFIX="${BASE_ROOT_DIR}/db5"

##
export CPPFLAGS="-I/opt/homebrew/Cellar/libevent/2.1.12/include"
export LDFLAGS="-L/opt/homebrew/Cellar/boost/1.81.0_1/lib"
export BITCOIN_CONFIG="--with-boost='/opt/homebrew/Cellar/boost/1.81.0_1' --with-boost-process --with-gui --disable-wallet --enable-reduce-exports"
#export BITCOIN_CONFIG="--with-gui --enable-reduce-exports --with-boost='/opt/homebrew/Cellar/boost/1.81.0_1' --with-boost-process BDB_LIBS='-L${BDB_PREFIX}/lib -ldb_cxx-5.3' BDB_CFLAGS='-I${BDB_PREFIX}/include'"
#export BITCOIN_CONFIG="--enable-reduce-exports=no --with-gui=no BDB_LIBS='-L${BDB_PREFIX}/lib -ldb_cxx-5.3' BDB_CFLAGS='-I${BDB_PREFIX}/include'"
export CI_OS_NAME="macos"
export NO_DEPENDS=1
export OSX_SDK=""
export CCACHE_SIZE=300M

export RUN_SECURITY_TESTS="true"

# Compiler for Mac native warns on C99 in dependencies.
export NO_WERROR=1
