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


dist-hook:
	@echo $(PACKAGE).spec updating...
	@rm -f $(distdir)/$(PACKAGE).spec
	-@sed "s/%define version .*/%define version $(VERSION)/" \
	    $(srcdir)/$(PACKAGE).spec > $(distdir)/$(PACKAGE).spec && \
	    cp $(distdir)/$(PACKAGE).spec $(srcdir)/$(PACKAGE).spec
	@echo $(PACKAGE).lsm updating...
	@rm -f $(distdir)/$(PACKAGE).lsm
	-@SIZE=`$(TAR) chof - $(distdir) | gzip $(GZIP_ENV) | wc -c`; SIZE=`expr \( $$SIZE + 512 \) / 1024`; echo Size: $$SIZE kB; \
	 DATE=`date +%Y-%m-%d`; echo Date: $$DATE; \
	 sed -e "s/Version:	.*/Version:	$(VERSION)/" \
	     -e "s/[0-9]* kB $(PACKAGE)-[0-9.]*\.tar\.gz/$$SIZE kB $(PACKAGE)-$(VERSION).tar.gz/" \
	     -e "s/Entered-date:	.*/Entered-date:	$$DATE/" \
	    $(srcdir)/$(PACKAGE).lsm > $(distdir)/$(PACKAGE).lsm && \
	    cp $(distdir)/$(PACKAGE).lsm $(srcdir)/$(PACKAGE).lsm

REL_DIR=/ftp/pub/software/unix/X11
release:
	cvs commit -m $(VERSION)
	make dist
	cvs commit -m $(VERSION)
	make distcheck
	mv $(PACKAGE)-$(VERSION).tar.gz $(REL_DIR)
	cp -p $(PACKAGE).lsm $(REL_DIR)/$(PACKAGE)-$(VERSION).lsm
	cd $(REL_DIR) && \
	    (addbz $(PACKAGE)-$(VERSION).tar.gz; \
	    rm -f $(PACKAGE).tar.gz && ln -s $(PACKAGE)-$(VERSION).tar.gz $(PACKAGE).tar.gz)
	mutt -s add lsm@execpc.com < $(PACKAGE).lsm
	cvs tag $(PACKAGE)-`echo $(VERSION) | sed 's/\./-/g'` .
