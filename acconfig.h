#ifndef _CONFIG_H
#define _CONFIG_H
@TOP@

/* the name of package */
#undef PACKAGE

/* the version of package */
#undef VERSION

/* define if you have Motif */
#undef HAVE_MOTIF

#undef HAVE_LIBXAW

#define TK_NONE   0
#define TK_MOTIF  1
#define TK_XAW    2

#undef TK

@BOTTOM@

#ifndef HAVE_STRERROR
  extern char *sys_errlist[];
# define strerror(e) (sys_errlist[e])
#endif

#ifndef HAVE_STRDUP
# define strdup(s) strcpy(malloc(strlen((s))+1),(s));
#endif

#define KEYMAP_DIR   PKGDATADIR

#ifndef TK
# if defined HAVE_MOTIF
#  define TK TK_MOTIF
# elif defined HAVE_LIBXAW
#  define TK TK_XAW
# else
#  define TK TK_NONE
# endif
#endif

#endif /* _CONFIG_H */
