SUBDIRS = icons keymaps xmodmap

bin_PROGRAMS = xrus

xrus_SOURCES = xrus.c props.c menu.c menu.h xrus.h props.h keycomb.c keycomb.h\
 altmap.c altmap.h xalloca.h kbdstate.c kbdstate.h

xrus_LDADD = $(TOOLKIT_LIBS)\
 -lXmu -lXt -lXext -lX11 $(X_PRE_LIBS) $(X_LIBS) $(X_EXTRA_LIBS)\
 defres.o @ALLOCA@

CPPFLAGS = $(X_CFLAGS)
INCLUDES = $(TOOLKIT_INCLUDE)

defres.c: Xrus.ad mkdefres.awk
	$(AWK) -f $(srcdir)/mkdefres.awk < $(srcdir)/Xrus.ad > $@

CLEANFILES = defres.c

man_MANS = xrus.1

EXTRA_DIST = INSTALL-koi8 INSTALL-english xrus.doc-koi8 xrus.doc-english\
 Xrus.ad mkdefres.awk $(man_MANS) xruskb.spec xruskb.lsm FAQ\
 README.WindowMaker Xrus.wmaker

AUTOMAKE_OPTIONS = foreign

confpaths.h: Makefile
	echo "#define BINDIR     \"$(bindir)\""      > $@
	echo "#define PKGDATADIR \"$(pkgdatadir)\"" >> $@

DISTCLEANFILES = confpaths.h

xrus.o: confpaths.h
