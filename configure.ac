# Process this file with autoconf to create configure.

AC_INIT([ChipSynth],[0.0.1],[http://sourceforge.net/p/chipsynth/tickets/],[chipsynth],[http://sourceforge.net/projects/chipsynth/])
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

#####################################################
# Check for debug build
#####################################################
AC_MSG_CHECKING([for debugging])
AC_ARG_ENABLE(debug, [AS_HELP_STRING([--enable-debug], [compile for debugging])])
AC_MSG_RESULT([$enable_debug])

if [ test "x$enable_debug" = "xyes" ]; then
        debug_flags="-g -Wall -O0 -DDEBUG -D__RTMIDI_DEBUG__ -DRTAUDIO_DEBUG"
        CXXFLAGS=""
        CFLAGS=""
        if [ test "x$MINGW32" = "xyes" ]; then
                LDFLAGS="${LDFLAGS} -mconsole"
        else
                LDFLAGS=""
        fi
else
        debug_flags=-DNDEBUG
fi

AC_SUBST([debug_flags])


#####################################################
# Check for audio libs
#####################################################

rt_flags=""

PKG_CHECK_MODULES(ALSA,
    [alsa >= 1.0],
    [rt_flags="$rt_flags -D__LINUX_ALSA__"; pthread_flags="-lpthread"],
    [AC_MSG_WARN([$ALSA_PKG_ERRORS])]
)

PKG_CHECK_MODULES(JACK,
    [jack >= 0.121],
    [rt_flags="$rt_flags -D__UNIX_JACK__"; pthread_flags="-lpthread"],
    [AC_MSG_WARN([$JACK_PKG_ERRORS])]
)

PKG_CHECK_MODULES(PULSE,
    [libpulse-simple >= 1.0],
    [rt_flags="$rt_flags -D__LINUX_PULSE__"; pthread_flags="-lpthread"],
    [AC_MSG_WARN([$PULSE_PKG_ERRORS])]
)

AC_CHECK_HEADERS([mmsystem.h asio.h ksmedia.h dsound.h CoreMIDI/CoreMIDI.h])

if test "$ac_cv_header_dsound_h" == "yes"; then
    rt_flags="$rt_flags -D__WINDOWS_DS__"
    DS_LIBS = -ldsound
fi

#if test "$ac_cv_header_asio_h" == "yes"; then
#    rt_flags="$rt_flags -D__WINDOWS_ASIO__"
#    ASIO_LIBS = ????
#fi

if test "$ac_cv_header_mmsystem_h" == "yes"; then
    rt_flags="$rt_flags -D__WINDOWS_MM__"
    MM_LIBS = -lwinmm
fi

if test "$ac_cv_header_ksmedia_h" == "yes"; then
    rt_flags="$rt_flags -D__WINDOWS_KS__"
    KS_LIBS = -lsetupapi -lksuser
fi

if test "$ac_cv_CoreMIDI_CoreMIDI_h" == "yes"; then
    rt_flags="$rt_flags -D__MACOSX_CORE__"
    CORE_LIBS="-framework CoreMIDI -framework CoreAudio -framework CoreFoundation"
    pthread_flags="-lpthread"
fi


AC_ARG_VAR([MM_LIBS], [Linker flags for Windows MMSYSTEM])
AC_ARG_VAR([DS_LIBS], [Linker flags for Windows DirectSound])
AC_ARG_VAR([KS_LIBS], [Linker flags for Windows Kernel Streaming])
AC_ARG_VAR([CORE_LIBS], [Linker flags for MAC OSX])

AC_SUBST([pthread_flags])
AC_SUBST([rt_flags])

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
