# Q and A：

Q：这个项目的用途
A：该项目依赖于baselib库和frame库，这两个库组成了一个UI的完整SDK；可以允许编程人员根据自己的需求编写定制化的UI界面，也可以作为其他项目的UI部分。

Q：如何使用这个项目编译可执行程序
A：按照如下操作编写并编译可执行程序
    1.将最新baselib库和frame库放置在library目录下
    2.根据需求修改或完善UIWork的内容（UIWork中的例子展示了如何实现自己的form、stage、actor并加载他们，以及在它们中加载图片、文言、xml布局等资源）
    4.执行build.sh脚本生成可执行程序

Q:找不到<ft2build.h>
A:默认的ft2build.h文件和freetype文件夹在/usr/include/freetype2/文件夹内，请将它们放在/usr/include/目录下，因为这是默认的系统目录。
如果您的系统中没有freetype2库,请在网络上下载并安装。freetype库依赖libpng和zstd等开源库,请记得先下载安装它们。

Q：baselib库更新方式
A：去github下载最新baselib库代码[代码地址](git@github.com:haosk1/baselib.git)，按照readme说明生成新的baselib库。
完成后将生成的.so和Include文件夹分别拷贝到library/baselib目录即完成库更新。

Q：frame库更新方式
A：去github下载最新frame库代码[代码地址](git@github.com:haosk1/frame.git)，按照readme说明生成新的frame库。
完成后将生成的.so和Include文件夹分别拷贝到library/baselib目录即完成库更新。

git add .
git commit
git push --set-upstream origin main


## gdb常用调试命令
1.run:用于开始程序调试
2.break:用于设置断点，可以是源代码的行号、函数名或地址
3.clear:用于清除断点，可以是源代码的行号、函数名或地址
4.continue:用于让程序继续执行，直到遇到断点
5.stepi:单步运行，每次运行一条汇编指令
6.step:单步运行,每次运行一行C源代码
7.nexti:运行至次汇编指令，不进入函数内部
8.next:运行至次行C源代码,不进入函数内部
9.jump:任意跳转到某行源代码运行
10.until:从断点处运行到某行源代码，不包括该行
11.finish:运行到当前函数的返回处
12.info breakpoints:显示所有的断点
13.delete:删除断点
14.watch:设置变量的某种变化条件
15.list:显示源代码
16.disassemble:反汇编指令
17.x:以十六进制及字符串查看内存值
18.print:打印数据、表达式及变量
19.set:设置变量值
20.backtrace:显示堆栈调用过程