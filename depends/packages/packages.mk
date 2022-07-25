packages:=boost libevent

qt_packages = zlib

<<<<<<< HEAD
qt_x86_64_linux_packages:=qt expat dbus libxcb xcb_proto libXau xproto freetype fontconfig libX11 xextproto libXext xtrans libxkbcommon
qt_i686_linux_packages:=$(qt_x86_64_linux_packages)
=======
qrencode_packages = qrencode

qt_linux_packages:=qt expat libxcb xcb_proto libXau xproto freetype fontconfig
qt_android_packages=qt
>>>>>>> 1.21-dev

qt_darwin_packages=qt
qt_mingw32_packages=qt

bdb_packages=bdb
sqlite_packages=sqlite

zmq_packages=zeromq

upnp_packages=miniupnpc

<<<<<<< HEAD
avx2_native_packages:=native_nasm
avx2_x86_64_linux_packages:=intel-ipsec-mb
=======
multiprocess_packages = libmultiprocess capnp
multiprocess_native_packages = native_libmultiprocess native_capnp
>>>>>>> 1.21-dev

darwin_native_packages = native_biplist native_ds_store native_mac_alias

ifneq ($(build_os),darwin)
darwin_native_packages += native_cctools native_cdrkit native_libdmg-hfsplus
endif
