# RedisTest
redis demo

hiredis的下载目录：https://github.com/microsoft/hiredis

在msvcx选择编译器，配置vs编译环境(hiredis的默认C++->代码生成->运行库:多线程DLL(/MT))

使用中，在某些情况下会提示:RtlGenRandom重复定义错误；可以改在对应头文件中加入RtlGenRandomFunc RtlGenRandom;或者屏蔽win32fixes.h头文件，加入系统的头文件。

