%define version 1.14.0
%define release 1

Name: xruskb
Summary: program for switching two keyboard layouts
Version: %{version}
Release: %{release}
Copyright: GNU GPL
Group: X11/Utilities
Source: ftp://ftp.yars.free.net/pub/software/unix/X11/xruskb-%{version}.tar.gz
BuildRoot: /tmp/xruskb-%{version}-root

%description
   Xrus - program for switching two keyboard layouts with a hot
key combination. It also serves as autolock.
   It can be  compiled with Motif, Lesstif, Xaw  or without any
widget library. In  the later case it won't show  its icon with
keyboard indicator.
   It gets all keyboard and mouse events; when a particular hot
key combination  is pressed,  it swaps 1,2  and 3,4  columns of
keyboard map. When a timeout  without keyboard and mouse events
passes, it starts a locker.


%prep
%setup

%build
%define __libtoolize true
%configure
make

%install
make install DESTDIR=$RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%files
%attr(755, root, root) %{_bindir}/*
%attr(644, root, man)  %{_mandir}/man*/*
%attr(755, root, root) %dir %{_datadir}/xruskb
%attr(644, root, root) %{_datadir}/xruskb/*

%attr(-, root, root) %doc README xrus.doc* NEWS TODO Xrus.ad Xrus.wmaker
%attr(-, root, root) %doc keymaps/README.keymaps icons/README.icons
%attr(-, root, root) %doc xmodmap/README.xrusmodmap README.WindowMaker
