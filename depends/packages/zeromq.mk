package=zeromq
<<<<<<< HEAD
$(package)_version=4.3.4
$(package)_download_path=https://github.com/zeromq/libzmq/releases/download/v$($(package)_version)/
$(package)_file_name=$(package)-$($(package)_version).tar.gz
$(package)_sha256_hash=c593001a89f5a85dd2ddf564805deb860e02471171b3f204944857336295c3e5
$(package)_patches=remove_libstd_link.patch clock-unused-nsecs.patch 0002-disable-pthread_set_name_np.patch
=======
$(package)_version=4.3.1
$(package)_download_path=https://github.com/zeromq/libzmq/releases/download/v$($(package)_version)/
$(package)_file_name=$(package)-$($(package)_version).tar.gz
$(package)_sha256_hash=bcbabe1e2c7d0eec4ed612e10b94b112dd5f06fcefa994a0c79a45d835cd21eb
$(package)_patches=remove_libstd_link.patch
>>>>>>> 1.21-dev

define $(package)_set_vars
  $(package)_config_opts=--without-docs --disable-shared --disable-curve --disable-curve-keygen --disable-perf
  $(package)_config_opts += --without-libsodium --without-libgssapi_krb5 --without-pgm --without-norm --without-vmci
  $(package)_config_opts += --disable-libunwind --disable-radix-tree --without-gcov --disable-dependency-tracking
  $(package)_config_opts += --disable-Werror --disable-drafts --enable-option-checking
  $(package)_config_opts_linux=--with-pic
  $(package)_config_opts_android=--with-pic
  $(package)_cxxflags=-std=c++11
endef

define $(package)_preprocess_cmds
<<<<<<< HEAD
  patch -p1 < $($(package)_patch_dir)/clock-unused-nsecs.patch && \
  patch -p1 < $($(package)_patch_dir)/remove_libstd_link.patch && \
  patch -p1 < $($(package)_patch_dir)/0002-disable-pthread_set_name_np.patch && \
=======
  patch -p1 < $($(package)_patch_dir)/remove_libstd_link.patch && \
>>>>>>> 1.21-dev
  cp -f $(BASEDIR)/config.guess $(BASEDIR)/config.sub config
endef

define $(package)_config_cmds
  $($(package)_autoconf)
endef

define $(package)_build_cmds
<<<<<<< HEAD
  $(MAKE) ./src/libzmq.la
=======
  $(MAKE) src/libzmq.la
>>>>>>> 1.21-dev
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install-libLTLIBRARIES install-includeHEADERS install-pkgconfigDATA
endef

define $(package)_postprocess_cmds
  rm -rf bin share lib/*.la
endef
