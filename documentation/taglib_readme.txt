
To build taglib you need gcc 2.95/libstdc++.r4 by Oliver Tappe:

	http://dl.hirschkaefer.de/gcc/

You may also need autoconf 2.54.

	http://www.bebits.com/app/2971

cd to  projects/src/taglib/  
and run  configure;  make;  make

For now you need to copy libtag.so manually from 
project/src/taglib/taglib/.libs/  to  project/lib/ 

To clean it up, cd to projects/src/taglib/  and type 'make distclean'.

To make the taglib example binaries, run 'make examples' in the taglib folder.

test
