define(`_incall',`ifelse($#,0,,$#,1,`include($1)',dnl
`include($1)_incall(shift($@))')')dnl
dnl
define(`incall',`dnl
! This file is automatically generated from the following files:
!	misc.m4,$@
! DO NOT EDIT DIRECTLY.
!
_incall($@)')dnl
