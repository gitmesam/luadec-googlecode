# 说明 #

编译依赖于 lua-5.1.x

可以在 lua 支持的任何环境编译

测试过 gcc-4.2@linux-2.6 , windows mingw , windows vs2008 , windows vs2012


# 步骤 #

mingw32:
```
cd lua-5.1.4
cd src
mingw32-make mingw
cd luadec
//生成 release 版本
mingw32-make
//或者生成 debug 版本
mingw32-make debug
//或者生成带 memwatch 的 debug 版本
mingw32-make memwatch
```

linux:
```
cd lua-5.1.4
cd src
make linux
cd luadec
//生成 release 版本
make
//或者生成 debug 版本
make debug
//或者生成带 memwatch 的 debug 版本
make memwatch
```