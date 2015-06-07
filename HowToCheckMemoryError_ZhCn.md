# 如何检测C/C++内存错误 #


---

## 常见的内存错误 ##
  * unfree 未释放
  * double free 双重释放
  * wild pointer (野指针) 导致的 wild free (错误释放) 和 invalid access (无效访问)
  * uninitialised value 使用未初始化的内存
  * buffer overflow 缓存溢出


---

## 检测方案 ##
> 使用内存检测工具
  * vld 用于 vc ，编译时加入，可以检测 unfree ， 带call stack
  * memwatch 用于 vc mingw linux ，编译时加入，可以检测 unfree， double free， WILD free， 不带call stack
  * valgrind 用于 linux ，运行时加入，可以检测 unfree， double free， WILD free，invalid access， uninitialised value， 带 call stack


---

## 处理方案 ##

1. 使用 calloc 替代 malloc
  * 强制 calloc 第二个参数使用 sizeof
  * calloc 会将分配的空间置零

2. 就近原则
  * 尽量在同一程序块内 定义，分配，释放
  * 如果一个变量全局定义，或者在函数开始时定义，然后 分配释放 多次，考虑取消全局定义，在使用处定义，将使用处设定为程序块

3. 使用和释放前检查，释放后置零
```
if (ptr) {
    free(ptr);
    ptr = NULL;
}
```

4. 作为函数参数与返回值的指针， 选择性使用 const 修饰
```
// 内部 指 本函数 和 StringBuffer 相关的其他函数
// 外部 指 调用者 和 有可能传递参数和返回值的其他函数
typedef struct StringBuffer_ StringBuffer;
struct StringBuffer_ {
	int size;
	char* buff;
}

//返回的指针不可修改，不可free ，直接返回结构内部保存的指针
const char* getRef(StringBuffer* sb);

//返回的指针内部不能再保留，free 由外部负责
char* getCopy(StringBuffer* sb);

//函数内部不能修改释放 str，最好内部不要保留str的值 ，必须由调用者负责free(str)
StringBuffer* newStringBuffer(const char* str);

//函数内部可能修改 str，必须就内部是否free(str)，做好约定， 最好在命名上约定：
StringBuffer* makeStringBuffer(char* str); //内部修改 str ，但是不保留， 由外部负责free
StringBuffer* deleteString(char* str); //根据命名，此函数 free(str)， 外部负责 str=NULL
StringBuffer* makeSBfromStr(char* str); //根据命名，内部将保留str的地址和内容， 外部在此之后不要操作str
```

5. buffer overflow 缓存溢出
  * 确定缓存值的数量有上限，按 上限+1 分配
  * 如缓存值的数量无上限，改用链表或动态扩大缓存
  * 操作时检测栈顶指针是否超上限，用 assert 只能用于debug