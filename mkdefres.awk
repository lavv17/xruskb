BEGIN {
   print "#include <config.h>"
   print "const char *DefaultResources="
}
/^!#/ { sub("^!",""); print; }
/^$|^!/ { next }
{
   gsub("\\\\","\\\\");
   print "\"" $0 "\\n\"";
}
END {
   print "\"\";"
}
