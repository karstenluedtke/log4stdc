AC_PREREQ([2.69])
AC_INIT([log4stdc], [1.0.11], [karsten.luedtke@posteo.de])
AC_ARG_ENABLE([bfclog4cxx],
[  --enable-bfclog4cxx    Build replacement library for log4cxx],
[case "${enableval}" in
  yes) bfclog4cxx=true ;;
  no)  bfclog4cxx=false ;;
  auto)bfclog4cxx=auto ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-bfclog4cxx]) ;;
esac],[bfclog4cxx=false])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AM_PROG_AR
AC_PROG_RANLIB
AC_PROG_INSTALL

# Checks for libraries.
AC_CHECK_LIB([rt], [clock_gettime])
AC_CHECK_LIB([pthread], [pthread_mutexattr_settype])
AC_CHECK_LIB([log], [__android_log_print])

AC_CHECK_HEADER([winsock2.h],
    [AC_DEFINE([HAVE_LIBWS2_32], [1], [Define if libws2_32 exists.]) LIBS="-lws2_32 $LIBS"],
    [],
    [#include <windows.h>])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h inttypes.h malloc.h stddef.h stdint.h stdlib.h string.h sys/time.h sys/socket.h unistd.h signal.h pthread.h wchar.h log4cxx/log4cxx.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_INT64_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_CHECK_TYPES([struct timeval], [], [], [[
#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
]])
AC_CHECK_TYPES([struct timespec], [], [], [[
#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
]])

AC_CHECK_MEMBER([struct tm.tm_gmtoff], [AC_DEFINE([HAVE_TM_TM_GMTOFF],1,
    [Define to 1 if the system has the tm_gmtoff field in struct tm])],
    [], [[
#define _BSD_SOURCE 1
#include <time.h>
]])
AC_CHECK_MEMBER([struct tm.__tm_gmtoff], [AC_DEFINE([HAVE_TM___TM_GMTOFF],1,
    [Define to 1 if the system has the __tm_gmtoff field in struct tm])],
    [], [[#include <time.h>]])

AC_CHECK_MEMBER([struct tm.tm_zone], [AC_DEFINE([HAVE_TM_TM_ZONE],1,
    [Define to 1 if the system has the tm_zone field in struct tm])],
    [], [[
#define _BSD_SOURCE 1
#include <time.h>
]])
AC_CHECK_MEMBER([struct tm.__tm_zone], [AC_DEFINE([HAVE_TM___TM_ZONE],1,
    [Define to 1 if the system has the __tm_zone field in struct tm])],
    [], [[#include <time.h>]])

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([getcwd clock_gettime gettimeofday localtime_r _localtime64_s _localtime64 memchr memset strchr strerror strncasecmp strtoul snprintf vsnprintf pthread_mutex_init pthread_mutexattr_settype])

AM_CONDITIONAL([BFCLOG4CXX], [test '(' x$bfclog4cxx = xtrue ')' -o '(' '(' x$bfclog4cxx = xauto ')' -a '(' x$ac_cv_header_log4cxx_log4cxx_h = xyes ')' ')' ])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
