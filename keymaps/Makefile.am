pkgdata_DATA = jcuken-cp866.xmm jcuken-iso5.xmm yawerty-cyr.xmm\
 jcuken-cp1251.xmm jcuken-koi8.xmm yawerty-iso5.xmm jcuken-cyr.xmm\
 yawerty-cp866.xmm yawerty-koi8.xmm yawerty-cp1251.xmm

EXTRA_DIST = $(pkgdata_DATA) misc.m4 notice.m4 clear.m4 jcuken.m4 yawerty.m4\
 cp866.m4 cp1251.m4 iso5.m4 koi8.m4 README.keymaps jcuken-cp866.m4\
 jcuken-cp1251.m4 jcuken-cyr.m4 jcuken-iso5.m4 jcuken-koi8.m4 yawerty-cp866.m4\
 yawerty-cp1251.m4 yawerty-cyr.m4 yawerty-iso5.m4 yawerty-koi8.m4

SUFFIXES = .xmm .m4
.m4.xmm:
	$(M4) $< > $@

jcuken-cp866.xmm:   jcuken-cp866.m4   jcuken.m4  cp866.m4
jcuken-cp1251.xmm:  jcuken-cp1251.m4  jcuken.m4  cp1251.m4
jcuken-cyr.xmm:     jcuken-cyr.m4     jcuken.m4  
jcuken-iso5.xmm:    jcuken-iso5.m4    jcuken.m4  iso5.m4
jcuken-koi8.xmm:    jcuken-koi8.m4    jcuken.m4  koi8.m4
yawerty-cp866.xmm:  yawerty-cp866.m4  yawerty.m4 cp866.m4
yawerty-cp1251.xmm: yawerty-cp1251.m4 yawerty.m4 cp1251.m4
yawerty-cyr.xmm:    yawerty-cyr.m4    yawerty.m4
yawerty-iso5.xmm:   yawerty-iso5.m4   yawerty.m4 iso5.m4
yawerty-koi8.xmm:   yawerty-koi8.m4   yawerty.m4 koi8.m4  

$(pkgdata_DATA): misc.m4 notice.m4 clear.m4

MAINTAINERCLEANFILES = $(pkgdata_DATA)
