# color24bppto32bpp
A finely tuned RGB/BGR to RGBA/BGRA converter use avx2 and avx512-vbmi1

非常快。无论是avx2还是avx512-vbmi1的版本，都能提供稳定高于编译器O3的表现。

该转换器接受一个24bpp bmp文件，自动添加alpha通道并默认填充0xff，转为32bpp bmp文件之后输出。

这个程序是为Windows设计，在最新的VS2022/VS2026上，你应该可以直接构建，无需任何修改。
