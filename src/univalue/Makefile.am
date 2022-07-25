include sources.mk
ACLOCAL_AMFLAGS = -I build-aux/m4
.PHONY: gen FORCE
.INTERMEDIATE: $(GENBIN)

include_HEADERS = $(UNIVALUE_DIST_HEADERS_INT)
noinst_HEADERS = $(UNIVALUE_LIB_HEADERS_INT)

lib_LTLIBRARIES = libunivalue.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = pc/libunivalue.pc

<<<<<<< HEAD
libunivalue_la_SOURCES = $(UNIVALUE_LIB_SOURCES_INT)
=======
libunivalue_la_SOURCES = \
	lib/univalue.cpp \
	lib/univalue_get.cpp \
	lib/univalue_read.cpp \
	lib/univalue_write.cpp
>>>>>>> 1.21-dev

libunivalue_la_LDFLAGS = \
	-version-info $(LIBUNIVALUE_CURRENT):$(LIBUNIVALUE_REVISION):$(LIBUNIVALUE_AGE) \
	-no-undefined
libunivalue_la_CXXFLAGS = -I$(top_srcdir)/include

TESTS = test/object test/unitester test/no_nul

GENBIN = gen/gen$(BUILD_EXEEXT)
GEN_SRCS = gen/gen.cpp

$(GENBIN): $(GEN_SRCS)
	@echo Building $@
	$(AM_V_at)c++ -I$(top_srcdir)/include -o $@ $<

gen: $(GENBIN) FORCE
	@echo Updating lib/univalue_escapes.h
	$(AM_V_at)$(GENBIN) > lib/univalue_escapes.h

noinst_PROGRAMS = $(TESTS) test/test_json

test_unitester_SOURCES = $(UNIVALUE_TEST_UNITESTER_INT)
test_unitester_LDADD = libunivalue.la
test_unitester_CXXFLAGS = -I$(top_srcdir)/include -DJSON_TEST_SRC=\"$(srcdir)/$(UNIVALUE_TEST_DATA_DIR_INT)\"
test_unitester_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

<<<<<<< HEAD
test_test_json_SOURCES = $(UNIVALUE_TEST_JSON_INT)
test_test_json_LDADD = libunivalue.la
test_test_json_CXXFLAGS = -I$(top_srcdir)/include
test_test_json_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)
=======
test_test_json_SOURCES = test/test_json.cpp
test_test_json_LDADD = libunivalue.la
test_test_json_CXXFLAGS = -I$(top_srcdir)/include
test_test_json_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

test_no_nul_SOURCES = test/no_nul.cpp
test_no_nul_LDADD = libunivalue.la
test_no_nul_CXXFLAGS = -I$(top_srcdir)/include
test_no_nul_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

test_object_SOURCES = test/object.cpp
test_object_LDADD = libunivalue.la
test_object_CXXFLAGS = -I$(top_srcdir)/include
test_object_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

TEST_FILES = \
	$(TEST_DATA_DIR)/fail10.json \
	$(TEST_DATA_DIR)/fail11.json \
	$(TEST_DATA_DIR)/fail12.json \
	$(TEST_DATA_DIR)/fail13.json \
	$(TEST_DATA_DIR)/fail14.json \
	$(TEST_DATA_DIR)/fail15.json \
	$(TEST_DATA_DIR)/fail16.json \
	$(TEST_DATA_DIR)/fail17.json \
	$(TEST_DATA_DIR)/fail18.json \
	$(TEST_DATA_DIR)/fail19.json \
	$(TEST_DATA_DIR)/fail1.json \
	$(TEST_DATA_DIR)/fail20.json \
	$(TEST_DATA_DIR)/fail21.json \
	$(TEST_DATA_DIR)/fail22.json \
	$(TEST_DATA_DIR)/fail23.json \
	$(TEST_DATA_DIR)/fail24.json \
	$(TEST_DATA_DIR)/fail25.json \
	$(TEST_DATA_DIR)/fail26.json \
	$(TEST_DATA_DIR)/fail27.json \
	$(TEST_DATA_DIR)/fail28.json \
	$(TEST_DATA_DIR)/fail29.json \
	$(TEST_DATA_DIR)/fail2.json \
	$(TEST_DATA_DIR)/fail30.json \
	$(TEST_DATA_DIR)/fail31.json \
	$(TEST_DATA_DIR)/fail32.json \
	$(TEST_DATA_DIR)/fail33.json \
	$(TEST_DATA_DIR)/fail34.json \
	$(TEST_DATA_DIR)/fail35.json \
	$(TEST_DATA_DIR)/fail36.json \
	$(TEST_DATA_DIR)/fail37.json \
	$(TEST_DATA_DIR)/fail38.json \
	$(TEST_DATA_DIR)/fail39.json \
	$(TEST_DATA_DIR)/fail40.json \
	$(TEST_DATA_DIR)/fail41.json \
	$(TEST_DATA_DIR)/fail42.json \
	$(TEST_DATA_DIR)/fail44.json \
	$(TEST_DATA_DIR)/fail45.json \
	$(TEST_DATA_DIR)/fail3.json \
	$(TEST_DATA_DIR)/fail4.json \
	$(TEST_DATA_DIR)/fail5.json \
	$(TEST_DATA_DIR)/fail6.json \
	$(TEST_DATA_DIR)/fail7.json \
	$(TEST_DATA_DIR)/fail8.json \
	$(TEST_DATA_DIR)/fail9.json \
	$(TEST_DATA_DIR)/pass1.json \
	$(TEST_DATA_DIR)/pass2.json \
	$(TEST_DATA_DIR)/pass3.json \
	$(TEST_DATA_DIR)/pass4.json \
	$(TEST_DATA_DIR)/round1.json \
	$(TEST_DATA_DIR)/round2.json \
	$(TEST_DATA_DIR)/round3.json \
	$(TEST_DATA_DIR)/round4.json \
	$(TEST_DATA_DIR)/round5.json \
	$(TEST_DATA_DIR)/round6.json \
	$(TEST_DATA_DIR)/round7.json
>>>>>>> 1.21-dev

test_no_nul_SOURCES = $(UNIVALUE_TEST_NO_NUL_INT)
test_no_nul_LDADD = libunivalue.la
test_no_nul_CXXFLAGS = -I$(top_srcdir)/include
test_no_nul_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

test_object_SOURCES = $(UNIVALUE_TEST_OBJECT_INT)
test_object_LDADD = libunivalue.la
test_object_CXXFLAGS = -I$(top_srcdir)/include
test_object_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

TEST_FILES = $(UNIVALUE_TEST_FILES_INT)

EXTRA_DIST=$(UNIVALUE_TEST_FILES_INT) $(GEN_SRCS)
