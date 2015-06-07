# Introduction #

Must be compiled with lua 5.1.x .

Should be compiled in any environment that lua supports.

I have tested compiling in gcc-4.2@linux-2.6, windows mingw, windows vs2008, windows vs2012.


# Details #

mingw32:
```
cd lua-5.1.4
cd src
mingw32-make mingw
cd luadec
//release version
mingw32-make
//debug version
mingw32-make debug
//debug version with memwatch
mingw32-make memwatch
```

linux:
```
cd lua-5.1.4
cd src
make linux
cd luadec
//release version
mingw32-make
//debug version
mingw32-make debug
//debug version with memwatch
mingw32-make memwatch
```