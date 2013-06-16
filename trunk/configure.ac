# Process this file with autoconf to create configure.

AC_INIT([chipsynth],[0.0.1])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([subdir-objects])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CPP


# Checks for languages.
AC_LANG([C++])

AC_CHECK_HEADERS([stdint.h])
#AS_IF([test "x${ac_cv_header_stdint_h}" != "xyes"],
#  [AC_MSG_ERROR([Required header stdint.h not found])]
#)

# Search for alsa library
PKG_CHECK_MODULES(ALSA, [alsa >= 1.0])

#####################################################
# Check for QT
#####################################################
PKG_CHECK_MODULES(QT4, [QtCore QtGui >= 4.4.0], [
  AC_PATH_PROGS(MOC, [moc-qt4 moc], moc,`eval $PKG_CONFIG --variable=exec_prefix QtCore`/bin)
  AC_PATH_PROG(RCC, rcc, rcc,`eval $PKG_CONFIG --variable=exec_prefix QtCore`/bin)
  AC_PATH_PROGS(UIC, [uic-qt4 uic], uic,`eval $PKG_CONFIG --variable=exec_prefix QtCore`/bin)
])

#####################################################
# Check for KDE
#####################################################
AC_MSG_CHECKING([whether KDEDIR environment variable is set])
if test "x$KDEDIR" = "x"; then
  AC_MSG_RESULT([no])
else
  AC_MSG_RESULT([$KDEDIR])
fi

AM_CONDITIONAL(HAVE_KDE, test "x$KDEDIR" != "x")

AC_ARG_VAR([KDE_CFLAGS], [Compiler flags for KDE])
AC_ARG_VAR([KDE_LIBS], [Linker flags for KDE])

KDE_CFLAGS="-I$KDEDIR/include"
KDE_LIBS="-L$KDEDIR/lib -lkdeui"

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
