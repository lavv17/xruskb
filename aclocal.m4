dnl aclocal.m4 generated automatically by aclocal 1.4

dnl Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

dnl Functions for locating motif are taken from Lesstif distribution

AC_DEFUN(AC_PATH_MOTIF_DIRECT,
[test -z "$motif_direct_test_library" && motif_direct_test_library=Xm
test -z "$motif_direct_test_function" && motif_direct_test_function=XmCreatePushButton
test -z "$motif_direct_test_include" && motif_direct_test_include=Xm/Xm.h
  for ac_dir in               \
    /usr/include/Motif1.2     \
    /usr/Motif1.2/include     \
                              \
    /usr/X11R6/include        \
    /usr/X11R5/include        \
                              \
    /usr/include/X11R6        \
    /usr/include/X11R5        \
                              \
    /usr/local/X11R6/include  \
    /usr/local/X11R5/include  \
                              \
    /usr/local/include/X11R6  \
    /usr/local/include/X11R5  \
                              \
    /usr/X11/include          \
    /usr/include/X11          \
    /usr/local/X11/include    \
    /usr/local/include/X11    \
                              \
    /usr/X386/include         \
    /usr/x386/include         \
    /usr/XFree86/include/X11  \
                              \
    /usr/dt/include           \
                              \
    /usr/include              \
    /usr/local/include        \
    /usr/unsupported/include  \
    /usr/athena/include       \
    /usr/local/x11r5/include  \
    /usr/lpp/Xamples/include  \
    ; \
  do
    if test -r "$ac_dir/$motif_direct_test_include"; then
      no_motif= ac_motif_includes=$ac_dir
      break
    fi
  done

# Check for the libraries.
# See if we find them without any special options.
# Don't add to $LIBS permanently.
#ac_save_LIBS="$LIBS"
#LIBS="-l$motif_direct_test_library $LIBS"
# First see if replacing the include by lib works.
for ac_dir in `echo "$ac_motif_includes" | sed s/include/lib/` \
    /usr/lib/Motif1.2     \
    /usr/Motif1.2/lib     \
                          \
    /usr/X11R6/lib        \
    /usr/X11R5/lib        \
                          \
    /usr/lib/X11R6        \
    /usr/lib/X11R5        \
                          \
    /usr/local/X11R6/lib  \
    /usr/local/X11R5/lib  \
                          \
    /usr/local/lib/X11R6  \
    /usr/local/lib/X11R5  \
                          \
    /usr/X11/lib          \
    /usr/lib/X11          \
    /usr/local/X11/lib    \
                          \
    /usr/X386/lib         \
    /usr/x386/lib         \
    /usr/XFree86/lib/X11  \
                          \
    /usr/lib              \
    /usr/local/lib        \
    /usr/unsupported/lib  \
    /usr/athena/lib       \
    /usr/local/x11r5/lib  \
    /usr/lpp/Xamples/lib  \
    ; \
do
  for ac_extension in a so sl; do
    if test -r $ac_dir/lib${motif_direct_test_library}.$ac_extension; then
      no_motif= ac_motif_libraries=$ac_dir
      break 2
    fi
  done
done])

AC_DEFUN(AC_PATH_MOTIF,
[AC_REQUIRE_CPP()dnl

motif_includes=NONE
motif_libraries=NONE

AC_MSG_CHECKING(for Motif)
if test "x$with_motif" = xno; then
  no_motif=yes
else
  if test "x$motif_includes" != xNONE && test "x$motif_libraries" != xNONE; then
    no_motif=
  else
AC_CACHE_VAL(ac_cv_path_motif,
[# One or both of these vars are not set, and there is no cached value.
no_motif=yes
AC_PATH_MOTIF_DIRECT

if test "$no_motif" = yes; then
  ac_cv_path_motif="no_motif=yes"
else
  ac_cv_path_motif="no_motif= ac_motif_includes=$ac_motif_includes ac_motif_libraries=$ac_motif_libraries"
fi])dnl
  fi
  eval "$ac_cv_path_motif"
fi # with_motif != no

if test "$no_motif" = yes; then
  AC_MSG_RESULT(no)
else
  test "x$motif_includes" = xNONE && motif_includes=$ac_motif_includes
  test "x$motif_libraries" = xNONE && motif_libraries=$ac_motif_libraries
  ac_cv_path_motif="no_motif= ac_motif_includes=$motif_includes ac_motif_libraries=$motif_libraries"
  AC_MSG_RESULT([libraries $motif_libraries, headers $motif_includes])
fi
])

# Like AC_CONFIG_HEADER, but automatically create stamp file.

AC_DEFUN(AM_CONFIG_HEADER,
[AC_PREREQ([2.12])
AC_CONFIG_HEADER([$1])
dnl When config.status generates a header, we must update the stamp-h file.
dnl This file resides in the same directory as the config header
dnl that is generated.  We must strip everything past the first ":",
dnl and everything past the last "/".
AC_OUTPUT_COMMANDS(changequote(<<,>>)dnl
ifelse(patsubst(<<$1>>, <<[^ ]>>, <<>>), <<>>,
<<test -z "<<$>>CONFIG_HEADERS" || echo timestamp > patsubst(<<$1>>, <<^\([^:]*/\)?.*>>, <<\1>>)stamp-h<<>>dnl>>,
<<am_indx=1
for am_file in <<$1>>; do
  case " <<$>>CONFIG_HEADERS " in
  *" <<$>>am_file "*<<)>>
    echo timestamp > `echo <<$>>am_file | sed -e 's%:.*%%' -e 's%[^/]*$%%'`stamp-h$am_indx
    ;;
  esac
  am_indx=`expr "<<$>>am_indx" + 1`
done<<>>dnl>>)
changequote([,]))])

# Do all the work for Automake.  This macro actually does too much --
# some checks are only needed if your package does certain things.
# But this isn't really a big deal.

# serial 1

dnl Usage:
dnl AM_INIT_AUTOMAKE(package,version, [no-define])

AC_DEFUN(AM_INIT_AUTOMAKE,
[AC_REQUIRE([AC_PROG_INSTALL])
PACKAGE=[$1]
AC_SUBST(PACKAGE)
VERSION=[$2]
AC_SUBST(VERSION)
dnl test to see if srcdir already configured
if test "`cd $srcdir && pwd`" != "`pwd`" && test -f $srcdir/config.status; then
  AC_MSG_ERROR([source directory already configured; run "make distclean" there first])
fi
ifelse([$3],,
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Name of package])
AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Version number of package]))
AC_REQUIRE([AM_SANITY_CHECK])
AC_REQUIRE([AC_ARG_PROGRAM])
dnl FIXME This is truly gross.
missing_dir=`cd $ac_aux_dir && pwd`
AM_MISSING_PROG(ACLOCAL, aclocal, $missing_dir)
AM_MISSING_PROG(AUTOCONF, autoconf, $missing_dir)
AM_MISSING_PROG(AUTOMAKE, automake, $missing_dir)
AM_MISSING_PROG(AUTOHEADER, autoheader, $missing_dir)
AM_MISSING_PROG(MAKEINFO, makeinfo, $missing_dir)
AC_REQUIRE([AC_PROG_MAKE_SET])])

#
# Check to make sure that the build environment is sane.
#

AC_DEFUN(AM_SANITY_CHECK,
[AC_MSG_CHECKING([whether build environment is sane])
# Just in case
sleep 1
echo timestamp > conftestfile
# Do `set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
if (
   set X `ls -Lt $srcdir/configure conftestfile 2> /dev/null`
   if test "[$]*" = "X"; then
      # -L didn't work.
      set X `ls -t $srcdir/configure conftestfile`
   fi
   if test "[$]*" != "X $srcdir/configure conftestfile" \
      && test "[$]*" != "X conftestfile $srcdir/configure"; then

      # If neither matched, then we have a broken ls.  This can happen
      # if, for instance, CONFIG_SHELL is bash and it inherits a
      # broken ls alias from the environment.  This has actually
      # happened.  Such a system could not be considered "sane".
      AC_MSG_ERROR([ls -t appears to fail.  Make sure there is not a broken
alias in your environment])
   fi

   test "[$]2" = conftestfile
   )
then
   # Ok.
   :
else
   AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock])
fi
rm -f conftest*
AC_MSG_RESULT(yes)])

dnl AM_MISSING_PROG(NAME, PROGRAM, DIRECTORY)
dnl The program must properly implement --version.
AC_DEFUN(AM_MISSING_PROG,
[AC_MSG_CHECKING(for working $2)
# Run test in a subshell; some versions of sh will print an error if
# an executable is not found, even if stderr is redirected.
# Redirect stdin to placate older versions of autoconf.  Sigh.
if ($2 --version) < /dev/null > /dev/null 2>&1; then
   $1=$2
   AC_MSG_RESULT(found)
else
   $1="$3/missing $2"
   AC_MSG_RESULT(missing)
fi
AC_SUBST($1)])

