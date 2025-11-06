# color24bppto32bpp
A finely tuned RGB/BGR to RGBA/BGRA converter use avx2 and avx512-vbmi1

非常快。无论是avx2还是avx512-vbmi1的版本，都能提供稳定高于编译器O3的表现。

该转换器接受一个24bpp bmp文件，自动添加alpha通道并默认填充0xff，转为32bpp bmp文件之后输出。

使用方法很简单，直接拖放你的24bpp文件到exe上即可。程序会自动重命名并输出32bpp版本。

这个程序是为Windows设计的，如果你希望自行编译，在最新的VS2022/VS2026上，你应该可以直接构建，无需任何修改。

C++部分未使用任何Windows专用特性，因此C++部分很容易在Linux上编译。汇编代码则是专门针对Windows设计的，如果希望在Linux上使用，你需要将masm格式改为Linux上接受的格式，同时**修改调用约定**（rcx/rdx/r8/r9 -> rdi/rsi/rdx/rcx/r8/r9）

另外隆重介绍__attribute__((target_clones()) 特性，这项功能允许在一个二进制文件中集成对某个函数的多种目标架构的代码实现。这项功能目前有gcc/clang提供支持，在这个程序中，clang的支持已通过测试并确定可以使用。gcc至少在Windows下，默认下载的工具包不能使用，Linux未知，但这里放弃支持。项目默认提供的二进制文件是使用基于clang21的icx20250200构建的，为从haswell开始一直到尚未发布的diamondrapids都提供了支持。甚至代码中已经预设好了对novalake和wildcatlake的支持，只要待编译器新版本发布后即可使用。

这个程序为可移植性做了大量工作。大量的预定义宏让这个程序同时易于使用msvc/clang/icx编译。gcc只要不使用__attribute__((target_clones())特性，一样易于编译。

下图提供循环测试30轮结果：

我使用基于rocketlake架构的CPU，处理一张15000x11264x24bpp的图片，循环处理30次。

可以看到，编译器版本(flag: -O3 -march=rocketlake)进行30轮循环处理耗时2349ms。手写汇编的版本耗时1518ms。快了54.74%。

![](https://raw.githubusercontent.com/copper187/color24bppto32bpp/refs/heads/main/2025-11-06%20193643.png)
