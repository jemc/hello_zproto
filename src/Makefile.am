lib_LTLIBRARIES = libhello_zproto.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libhello_zproto.pc

include_HEADERS = \
    ../include/hello_zproto.h \
    ../include/hello_zproto_msg.h \
    ../include/citizen_msg.h

libhello_zproto_la_SOURCES = \
    platform.h \
    hello_zproto_msg.c \
    citizen_msg.c \
    citizen_server.c

AM_CFLAGS = -g
AM_CPPFLAGS = -I$(top_srcdir)/include
bin_PROGRAMS = hello_zproto_selftest
hello_zproto_selftest_LDADD = libhello_zproto.la
hello_zproto_selftest_SOURCES = hello_zproto_selftest.c
libhello_zproto_la_LDFLAGS = -version-info @LTVER@
TESTS = hello_zproto_selftest

# Produce generated models; do this manually in src directory
code:
	gsl -q hello_zproto_msg.xml \
         citizen_msg.xml citizen_server.xml

