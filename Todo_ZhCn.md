  * test\while\while.lua
  * test\while\while-if.lua
  * test\ifelseif.lua 不能正确跳出 else

  * ~~对每个jmp 插入空语句 label (void statement label) ::pc23::~~
    * ~~debug时 jmp 后用注释输出 跳到的 lable~~
    * ~~debug时 输出 lable~~
    * goto 规则， 不能从外面的block跳到里面的block
    * while 和 repeat 不在 loop 中体现

  * 反编译到 AST(Abstract syntax tree) 抽象语法树

  * 大概是没有删除 LogicExp 干净，导致的内存泄漏
    * ..\test\lua5.1-tests\constructs.lua
    * ..\test\Main.lua
    * ..\test\unluac-test\booleanassign02.lua
    * ..\test\unluac-test\combinebexpression04.lua

  * ~~lua5.1-tests 下 attrib.lua constructs.lua main.lua 无法运行的错误~~
> > | | attrib.lua | constructs.lua | main.lua | Main.lua(张松) |
|:|:-----------|:---------------|:---------|:-----------------|
> > | 76 | Y          | Y              | Y        | N                |
> > | 80 | Y          | N              | Y        | Y                |
> > | 84 | N          | N(死循环)   | N        | N                |
> > | 90 | Y          | Y              | Y        | Y                |

  * ~~F->tables 不应该用 List 看 CloseTable 880~~

  * ~~DeleteFunction 不应该 ClearVpend ，应该在 PrintFunctionCheck 里面检查 vpend是否为空~~