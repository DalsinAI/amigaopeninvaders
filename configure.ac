dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(open-invaders, 0.3, http://www.jamyskis.net/invaders.php)

PACKAGE_DATA_DIR=open_invadersdatadir
AC_SUBST(PACKAGE_DATA_DIR)

AC_SUBST(PACKAGE_DATA_DIR)


AC_SUBST(PACKAGE)


AC_SUBST(open_invadersincdir)


AC_SUBST(open_invadersdatadir)

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CXX
AM_PROG_CC_STDC
AC_HEADER_STDC




AM_PATH_ALLEGRO(4.2.1, , AC_MSG_ERROR(The Allegro headers (>=4.2.1) must be installed for this game to work. Please install the Allegro headers.))

AM_PROG_LIBTOOL

AC_OUTPUT([
Makefile
data/Makefile
headers/Makefile
src/Makefile

])
