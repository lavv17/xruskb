pkgdata_DATA = jcuken-cp866.xmm jcuken-iso5.xmm yawerty-cyr.xmm\
 jcuken-cp1251.xmm jcuken-koi8.xmm yawerty-iso5.xmm jcuken-cyr.xmm\
 yawerty-cp866.xmm yawerty-koi8.xmm yawerty-cp1251.xmm\
 jcuken-deru-cp866.xmm jcuken-deru-iso5.xmm yawerty-deru-cyr.xmm\
 jcuken-deru-cp1251.xmm jcuken-deru-koi8.xmm yawerty-deru-iso5.xmm\
 jcuken-deru-cyr.xmm yawerty-deru-cp866.xmm yawerty-deru-koi8.xmm\
 yawerty-deru-cp1251.xmm hebrew.xmm hebrew-iso8.xmm\
 jcuken-frru-cp866.xmm jcuken-frru-iso5.xmm jcuken-frru-cp1251.xmm\
 jcuken-frru-koi8.xmm jcuken-frru-cyr.xmm\
 yawerty-ua.xmm yawerty-ua-koi8.xmm

EXTRA_DIST = $(pkgdata_DATA) README.keymaps\
 misc.m4 notice.m4 clear.m4\
 jcuken.m4 yawerty.m4 yawerty-ua.m4 jcuken-deru.m4 yawerty-deru.m4 jcuken-frru.m4 hebrew.m4\
 cp866.m4 cp1251.m4 iso5.m4 koi8.m4 iso8.m4

SUFFIXES = .xmm .m4
.m4.xmm:
	$(M4) $< > $@

jcuken-cp866.xmm: jcuken.m4 cp866.m4
	echo "include(\`misc.m4')incall(\`cp866.m4',\`clear.m4',\`jcuken.m4')dnl" | $(M4) > $@
jcuken-cp1251.xmm: jcuken.m4 cp1251.m4
	echo "include(\`misc.m4')incall(\`cp1251.m4',\`clear.m4',\`jcuken.m4')dnl" | $(M4) > $@
jcuken-cyr.xmm: jcuken.m4
	echo "include(\`misc.m4')incall(\`clear.m4',\`jcuken.m4')dnl" | $(M4) > $@
jcuken-iso5.xmm: jcuken.m4 iso5.m4
	echo "include(\`misc.m4')incall(\`iso5.m4',\`clear.m4',\`jcuken.m4')dnl" | $(M4) > $@
jcuken-koi8.xmm: jcuken.m4 koi8.m4
	echo "include(\`misc.m4')incall(\`koi8.m4',\`clear.m4',\`jcuken.m4')dnl" | $(M4) > $@
yawerty-cp866.xmm: yawerty.m4 cp866.m4
	echo "include(\`misc.m4')incall(\`cp866.m4',\`clear.m4',\`yawerty.m4')dnl" | $(M4) > $@
yawerty-cp1251.xmm: yawerty.m4 cp1251.m4
	echo "include(\`misc.m4')incall(\`cp1251.m4',\`clear.m4',\`yawerty.m4')dnl" | $(M4) > $@
yawerty-cyr.xmm: yawerty.m4
	echo "include(\`misc.m4')incall(\`clear.m4',\`yawerty.m4')dnl" | $(M4) > $@
yawerty-iso5.xmm: yawerty.m4 iso5.m4
	echo "include(\`misc.m4')incall(\`iso5.m4',\`clear.m4',\`yawerty.m4')dnl" | $(M4) > $@
yawerty-koi8.xmm: yawerty.m4 koi8.m4
	echo "include(\`misc.m4')incall(\`koi8.m4',\`clear.m4',\`yawerty.m4')dnl" | $(M4) > $@

jcuken-deru-cp866.xmm: jcuken-deru.m4 cp866.m4
	echo "include(\`misc.m4')incall(\`cp866.m4',\`clear.m4',\`jcuken-deru.m4')dnl" | $(M4) > $@
jcuken-deru-cp1251.xmm: jcuken-deru.m4 cp1251.m4
	echo "include(\`misc.m4')incall(\`cp1251.m4',\`clear.m4',\`jcuken-deru.m4')dnl" | $(M4) > $@
jcuken-deru-cyr.xmm: jcuken-deru.m4
	echo "include(\`misc.m4')incall(\`clear.m4',\`jcuken-deru.m4')dnl" | $(M4) > $@
jcuken-deru-iso5.xmm: jcuken-deru.m4 iso5.m4
	echo "include(\`misc.m4')incall(\`iso5.m4',\`clear.m4',\`jcuken-deru.m4')dnl" | $(M4) > $@
jcuken-deru-koi8.xmm: jcuken-deru.m4 koi8.m4
	echo "include(\`misc.m4')incall(\`koi8.m4',\`clear.m4',\`jcuken-deru.m4')dnl" | $(M4) > $@
yawerty-deru-cp866.xmm: yawerty-deru.m4 cp866.m4
	echo "include(\`misc.m4')incall(\`cp866.m4',\`clear.m4',\`yawerty-deru.m4')dnl" | $(M4) > $@
yawerty-deru-cp1251.xmm: yawerty-deru.m4 cp1251.m4
	echo "include(\`misc.m4')incall(\`cp1251.m4',\`clear.m4',\`yawerty-deru.m4')dnl" | $(M4) > $@
yawerty-deru-cyr.xmm: yawerty-deru.m4
	echo "include(\`misc.m4')incall(\`clear.m4',\`yawerty-deru.m4')dnl" | $(M4) > $@
yawerty-deru-iso5.xmm: yawerty-deru.m4 iso5.m4
	echo "include(\`misc.m4')incall(\`iso5.m4',\`clear.m4',\`yawerty-deru.m4')dnl" | $(M4) > $@
yawerty-deru-koi8.xmm: yawerty-deru.m4 koi8.m4
	echo "include(\`misc.m4')incall(\`koi8.m4',\`clear.m4',\`yawerty-deru.m4')dnl" | $(M4) > $@

jcuken-frru-cp866.xmm: jcuken-frru.m4 cp866.m4
	echo "include(\`misc.m4')incall(\`cp866.m4',\`clear.m4',\`jcuken-frru.m4')dnl" | $(M4) > $@
jcuken-frru-cp1251.xmm: jcuken-frru.m4 cp1251.m4
	echo "include(\`misc.m4')incall(\`cp1251.m4',\`clear.m4',\`jcuken-frru.m4')dnl" | $(M4) > $@
jcuken-frru-cyr.xmm: jcuken-frru.m4
	echo "include(\`misc.m4')incall(\`clear.m4',\`jcuken-frru.m4')dnl" | $(M4) > $@
jcuken-frru-iso5.xmm: jcuken-frru.m4 iso5.m4
	echo "include(\`misc.m4')incall(\`iso5.m4',\`clear.m4',\`jcuken-frru.m4')dnl" | $(M4) > $@
jcuken-frru-koi8.xmm: jcuken-frru.m4 koi8.m4
	echo "include(\`misc.m4')incall(\`koi8.m4',\`clear.m4',\`jcuken-frru.m4')dnl" | $(M4) > $@

hebrew.xmm: hebrew.m4
	echo "include(\`misc.m4')incall(\`clear.m4',\`hebrew.m4')dnl" | $(M4) > $@
hebrew-iso8.xmm: hebrew.m4 iso8.m4
	echo "include(\`misc.m4')incall(\`iso8.m4',\`clear.m4',\`hebrew.m4')dnl" | $(M4) > $@

yawerty-ua.xmm: yawerty-ua.m4
	echo "include(\`misc.m4')incall(\`clear.m4',\`yawerty-ua.m4')dnl" | $(M4) > $@
yawerty-ua-koi8.xmm: yawerty-ua.m4 koi8.m4
	echo "include(\`misc.m4')incall(\`koi8.m4',\`clear.m4',\`yawerty-ua.m4')dnl" | $(M4) > $@

$(pkgdata_DATA): misc.m4 notice.m4 clear.m4

MAINTAINERCLEANFILES = $(pkgdata_DATA)
