# luadec v2.1 [r80](https://code.google.com/p/luadec/source/detail?r=80) by viruscamp #

基于 luadec.luaforge.net 和 luadec51.luaforge.net

http://code.google.com/p/luadec

viruscamp+luadec@gmail.com

如果 luadec.exe 崩溃了，试试 luadec\_memwatch.exe ，然后把导致崩溃的lua文件和memwatch.log 发给我。


---

## 改进: ##
  1. 完整table支持，NEWTABLE, SETLIST, SETTABLE
  1. 改进的各种循环结构支持(循环结构算法彻底改变)
  1. 改进的disassemble显示
  1. 直接读取lua源文件
  1. 提供参数用于反编译部分文件,跳过可能导致崩溃的部分
  1. 处理大于255个函数的文件
  1. 改进程序稳定性,减少程序崩溃和内存泄漏
  1. 输出255及以上连续的变量赋值时不再崩溃


---

## 用法: ##
  1. 反编译编译后的lua文件
> > luadec abc.luac
  1. 直接对源文件用，内部会编译后再反编译
> > luadec abc.lua
  1. 生成汇编代码
> > luadec -dis abc.lua
  1. -pn 选项用于显示代码结构
```
luadec -pn test.lua
0
 1 
  1_1
 2
```
  1. -nf 选项用于然后可以反编译指定的函数 (函数 1 包含 1\_1)
> > luadec -nf 1 test.lua
  1. -nf -dn 选项同时使用 反编译指定的函数但不包括下级函数(函数 1 不包含 1\_1)
> > luadec -dn -nf 1 test.lua


---

## 已知bug: ##
  1. "local a,b = ..., ..." 应该是 "local a,b=..."
  1. "local ,a" 应该是 "local a"
  1. if..else 有时会生成 "do return end" , 应该是 "else", 后续语句应该调整到else里
  1. 对复杂的逻辑表达式处理不好, 会产生错误的结果
  1. 对strip后的文件处理不好 -s 参数会进行strip后再反编译用于测试


---

## TODO: ##
  1. if 与 逻辑表达式处理
  1. strip 后的 local\_guess 先关注 setlist
  1. strip 后使用 -a 参数 先关注 setlist