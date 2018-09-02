# ubuntu 18.04 c++环境搭建

实现的步骤如下

## 1. 环境准备

首先需要,安装ｇcc和ｇ++环境

安装之前查看是否有安装,使用命令:

```
gcc --version
g++ --version
```

如果没有安装使用如下命令进行安装:

```
# 版本安装：这里选择的式版本5
sudo apt-get install gcc-5
sudo apt-get install g++-5
```

安装后查看,如同所示:

![](https://raw.githubusercontent.com/zhi-z/other/master/ubuntu%20C%2B%2B%E5%BC%80%E5%8F%91%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/gcc.png)

## 2. 写源代码

可以通过任何文本编辑器创建一个.cpp的文件.在这里用创建一个helloWorld.cpp文件.并保存在一个文件中.测试的代码为:

```
#include <iostream>
using namespace std;
int main()
{
    cout << "Hello, world!" << endl;
    return 0;
}

```

## 3. 代码编译

打开终端使用如下命令进行编译:

```
// 命令格式:g++ 文件名
g++ helloWorld.cpp
```

- 注意:这里的文件名其实是包含文件路径的。要是不知道文件路径的话可以在敲完g++加上空格之后直接把文件拖进去，系统会自动添加文件路径。默认的输出位置是“主文件夹”，默认输出文件是a.out。可以使用以下格式指定输出的文件名：

```
g++ 文件名 -o 输出文件名
```

输出的路径仍然是默认路径，文件名应是*.out的格式。这里要注意的是使用-o命令必须指定输出文件名，否则会报错：g++: argument to '-o' missing。

- 对于g++的命令使用,可以通过:**man g++**命令查看

## 4. 执行并输出结果

通过以上编译得到*.out文件以后,可以直接把这个文件拖到Terminal上,通过按回车键运行,如图所示:

![](https://raw.githubusercontent.com/zhi-z/other/master/ubuntu%20C%2B%2B%E5%BC%80%E5%8F%91%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/image/run.png)

到这里就完成了ubuntu下c++环境的搭建.

对于从编译到运行的过程可以通过脚本来一步到位执行,后面有时间在搞.



## 参考文献

- [Ubuntu C++ 环境的搭建](https://blog.csdn.net/wangzhen_csdn/article/details/76358484)

注:大部是参考以上的文献进行操作.