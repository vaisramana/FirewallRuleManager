AC_INIT([cm_demos], [0.0.0], [], [], [https://gitlabe1.ext.net.nokia.com/cm/configurationmanagement-demo])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([subdir-objects foreign tar-pax])
AM_SILENT_RULES([yes])
AC_PROG_CXX
AC_LANG([C++])
AM_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])
PKG_PROG_PKG_CONFIG
AC_ARG_WITH([systemdsystemunitdir],
     [AS_HELP_STRING([--with-systemdsystemunitdir=DIR], [Directory for systemd service files])],,
     [with_systemdsystemunitdir=auto])
AS_IF([test "x$with_systemdsystemunitdir" = "xyes" -o "x$with_systemdsystemunitdir" = "xauto"], [
     def_systemdsystemunitdir=$($PKG_CONFIG --variable=systemdsystemunitdir systemd)

     AS_IF([test "x$def_systemdsystemunitdir" = "x"],
         [AS_IF([test "x$with_systemdsystemunitdir" = "xyes"],
                [AC_MSG_ERROR([systemd support requested but pkg-config unable to query systemd package])])
          with_systemdsystemunitdir=no],
         [with_systemdsystemunitdir="$def_systemdsystemunitdir"])])
AS_IF([test "x$with_systemdsystemunitdir" != "xno"],
      [AC_SUBST([systemdsystemunitdir], [$with_systemdsystemunitdir])])
AM_CONDITIONAL([HAVE_SYSTEMD], [test "x$with_systemdsystemunitdir" != "xno"])
PKG_CHECK_MODULES([LIBCM], [libcm])
AC_ARG_WITH([cm-conf-dir],
    AS_HELP_STRING([--with-cm-conf-dir[=PATH]],
        [Install CM configuration files in this directory. @<:@default=yes@:>@]),
    [],
    [with_cm_conf_dir=yes])
if test "x$with_cm_conf_dir" = "xyes"; then
    CM_CONF_DIR="`pkg-config libcm --variable=confdir`"
else
    CM_CONF_DIR="$with_cm_conf_dir"
fi
AC_SUBST(CM_CONF_DIR)
AM_CONDITIONAL([ENABLE_CM_CONF],[test "x$with_cm_conf_dir" != "xno"])
PKG_CHECK_MODULES([LIBCMEXAMPLE1], [libcmexample1])
PKG_CHECK_MODULES([LIBCMEXAMPLE2], [libcmexample2])
AX_CXX_COMPILE_STDCXX(11, [noext], [mandatory])
AC_CONFIG_FILES([
 Makefile
 cmdemo1/Makefile
 cmdemo2/Makefile
])
AC_OUTPUT
