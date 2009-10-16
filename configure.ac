# This file is part of libfreespace.
#  
# Copyright (c) 2009 Hillcrest Laboratories, Inc. 
# 
# libfreespace is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

AC_INIT([libfreespace],
        [0.4rc1])
AC_CONFIG_AUX_DIR(config)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE()
AC_CONFIG_MACRO_DIR([m4])


AC_PROG_CC
AC_PROG_INSTALL

# Configure libtool
LT_INIT
AC_DISABLE_STATIC

AC_C_BIGENDIAN
if test "x$ac_cv_c_bigendian" = "xyes"; then
    CFLAGS="$CFLAGS -DFREESPACE_BIG_ENDIAN"
else
    CFLAGS="$CFLAGS -DFREESPACE_LITTLE_ENDIAN"
fi

# Check OS type and set a conditional
AC_MSG_CHECKING([operating system])
case $host in
*-linux*)
	AC_MSG_RESULT([Linux])
	backend="linux"
	;;
*-darwin*)
	AC_MSG_RESULT([Darwin/MacOS X])
	backend="darwin"
	;;
*)
	AC_MSG_ERROR([unsupported operating system])
esac

AM_CONDITIONAL([OS_LINUX], [test "x$backend" == "xlinux"])
AM_CONDITIONAL([OS_DARWIN], [test "x$backend" == "xdarwin"])


AC_STDC_HEADERS

# Check for libusb 1.0
AC_CHECK_HEADERS([libusb-1.0/libusb.h], [], [AC_MSG_FAILURE([*** libusb.h header not found.])])
AC_CHECK_LIB(usb-1.0, libusb_open, [], [AC_MSG_FAILURE([*** libusb_open() not found.])])
AC_CHECK_HEADERS([sys/time.h], [], [AC_MSG_FAILURE([*** sys/time.h required.])])

# Check for support needed for examples
AC_CHECK_HEADERS([poll.h])

AC_CONFIG_FILES([
   Makefile
   README
   doc/Doxyfile
   doc/mainpage.h
   doc/Makefile
   m4/Makefile
   linux/Makefile
   common/Makefile
   win32/Makefile
   win32/vs2005/Makefile
   win32/vs2008/Makefile
   include/Makefile
])

AC_OUTPUT
