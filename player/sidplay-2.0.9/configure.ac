dnl Process this file with autoconf to produce a configure script.
AC_INIT(Makefile.am)
AC_CONFIG_AUX_DIR(unix)
AM_CONFIG_HEADER(unix/config.h)

dnl Setup library CURRENT, REVISION and AGE
AM_INIT_AUTOMAKE(sidplay,2.0.9)

AC_CANONICAL_HOST

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX

dnl Initialise Libtool.
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN

dnl Checks what version of Unix we have and soundcard support
AC_CHECK_HEADERS(sys/ioctl.h linux/soundcard.h machine/soundcard.h  \
soundcard.h sys/audio.h sun/audioio.h sun/dbriio.h sys/audioio.h  \
audio.h dmedia/audio.h)

AH_TOP(
[/* Define supported audio driver */
#undef HAVE_HARDSID
#undef HAVE_ALSA
#undef HAVE_IRIX
#undef HAVE_HPUX
#undef HAVE_MMSYSTEM
#undef HAVE_OSS
#undef HAVE_SGI
#undef HAVE_SUNOS
#undef HAVE_WAV_ONLY
])

dnl Audio Subsystem Test
AC_MSG_CHECKING([which audio subsystem is available])
audiodrv_libadd=""
case "$host" in
    *cygwin*) AC_DEFINE(HAVE_MMSYSTEM)
        audiodrv_libadd="./audio/mmsystem/libmmsystem.a"
        AC_MSG_RESULT(mmsystem)
        AUDIO_LDADD=-lwinmm
        AC_SUBST(AUDIO_LDADD)
        ;;
    *freebsd*) AC_DEFINE(HAVE_OSS)
        audiodrv_libadd="./audio/oss/liboss.a"
        AC_MSG_RESULT(oss)
        ;;
    *hpux*) AC_DEFINE(HAVE_HPUX)
        audiodrv_libadd="./audio/hpux/libhpux.a"
        AC_MSG_RESULT(hpux)
        ;;
    *irix*) AC_DEFINE(HAVE_IRIX)
        audiodrv_libadd="./audio/irix/libirix.a"
        AC_MSG_RESULT(irix)
        AC_CHECK_LIB(audio, main, [AUDIO_LDADD=-laudio AC_SUBST(AUDIO_LDADD)])
        ;;
    *linux*) AC_DEFINE(HAVE_OSS)
        audiodrv_libadd="./audio/oss/liboss.a"
        AC_MSG_RESULT(oss)
        ;;
    *netbsd*) AC_DEFINE(HAVE_OSS)
        audiodrv_libadd="./audio/oss/liboss.a"
        AC_MSG_RESULT(oss)
        AC_CHECK_LIB(ossaudio, main, [AUDIO_LDADD=-lossaudio AC_SUBST(AUDIO_LDFLAGS)]) 
        ;;
    *qnx*) AC_DEFINE(HAVE_ALSA)
        audiodrv_libadd="./audio/alsa/libalsa.a"
        AC_MSG_RESULT(alsa)
        AC_CHECK_LIB(asound, main, [AUDIO_LDADD=-lasound AC_SUBST(AUDIO_LDFLAGS)])
        ;;
    *sgi*) AC_DEFINE(HAVE_SGI)
        AC_MSG_RESULT(sgi)
        ;;
    *solaris*) AC_DEFINE(HAVE_SUNOS)
        audiodrv_libadd="./audio/sunos/libsunos.a"
        AC_MSG_RESULT(sparc)
        ;;
    *sunos*) AC_DEFINE(HAVE_SUNOS)
        audiodrv_libadd="./audio/sunos/libsunos.a"
        AC_MSG_RESULT(sparc)
        ;;
    *) AC_DEFINE(HAVE_WAV_ONLY)
        AC_MSG_RESULT(wav only)
        ;;
esac

AUDIO_DRV="$audiodrv_libadd"
AC_SUBST(AUDIO_DRV)

dnl Use C++ for tests.
AC_LANG_CPLUSPLUS

dnl Check for nothrow allocator.
MY_CHECK_EXCEPTIONS

dnl Check ANSI C++ ios features
MY_CHECK_IOS_BIN
MY_CHECK_IOS_OPENMODE

dnl Check for libraries
SID2_LIB_CHECKS
SID2_FIND_LIBSIDPLAY2
SID2_FIND_LIBSIDUTILS

# Find sid builder emulations
SID2_FIND_BUILDERS

dnl Add absolute directory for header files
CXXFLAGS="$CXXFLAGS -I\$(top_srcdir)/include \
\$(LIBSIDPLAY2_CXXFLAGS) \$(LIBSIDUTILS_CXXFLAGS)"

ACLOCAL_AMFLAGS="-I unix"
AC_SUBST(ACLOCAL_AMFLAGS)

AC_OUTPUT(
Makefile \
doc/Makefile \
doc/en/Makefile \
src/Makefile \
src/audio/Makefile \
src/audio/alsa/Makefile \
src/audio/hpux/Makefile \
src/audio/irix/Makefile \
src/audio/mmsystem/Makefile \
src/audio/null/Makefile \
src/audio/oss/Makefile \
src/audio/sunos/Makefile \
src/audio/wav/Makefile \
unix/Makefile \
unix/sidplay2.spec \
)
