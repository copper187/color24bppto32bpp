#include <iostream>
#include <stdint.h>
#include <malloc.h>
#include <ctime>
#include <windows.h>
#include <filesystem>

#define get_aligned_boundary(mem_size, aligned_boundary) (((mem_size) + (aligned_boundary - 1)) & ~(aligned_boundary - 1))   //取最近对齐边界，快速方法

extern "C"
{
    void bpp24to32_avx2(uint8_t* bpp24addr, uint8_t* bpp32addr, uint64_t round_count);
    void bpp24to32_avx512vbmi1(uint8_t* bpp24addr, uint8_t* bpp32addr, uint64_t round_count);
}

void bpp24to32_pure_c(uint8_t* bpp24addr, uint8_t* bpp32addr, uint64_t pixel_count)
{
    uint64_t counter = 0;
    for (;;)
    {
        *bpp32addr = *bpp24addr;
        *(bpp32addr + 1) = *(bpp24addr + 1);
        *(bpp32addr + 2) = *(bpp24addr + 2);
        *(bpp32addr + 3) = 0xff;
        bpp24addr += 3;
        bpp32addr += 4;
        counter++;
        if (counter == pixel_count)
        {
            break;
        }
    }
    return;
}

#if defined(_MSC_VER) && !defined(__clang__) && !defined(__GNUC__) && !defined(__INTEL_LLVM_COMPILER)
#include <intrin.h>
void bpp24to32_simd_init(uint8_t* bpp24addr, uint8_t* bpp32addr, uint64_t pixel_count)
{
    uint64_t round_count = pixel_count / 64;
    int32_t cpuInfo[4];
    __cpuidex(cpuInfo, 7, 0);
    if ((cpuInfo[2] & (1 << 1)) != 0)
    {
        bpp24to32_avx512vbmi1(bpp24addr, bpp32addr, round_count);
    }
    else
    {
        bpp24to32_avx2(bpp24addr, bpp32addr, round_count);
    }
    bpp24addr += round_count * 64 * 3;
    bpp32addr += round_count * 64 * 4;
    pixel_count -= round_count * 64;
    for (uint64_t i = 0; i < pixel_count; i++)
    {
        *bpp32addr = *bpp24addr;
        *(bpp32addr + 1) = *(bpp24addr + 1);
        *(bpp32addr + 2) = *(bpp24addr + 2);
        *(bpp32addr + 3) = 0xff;
        bpp24addr += 3;
        bpp32addr += 4;
    }
    return;
}

#endif

#if defined(__clang__) || defined(__GNUC__) || defined(__INTEL_LLVM_COMPILER)

__attribute__((target_clones("default", "arch=haswell", "arch=broadwell", "arch=skylake", "arch=skylake-avx512", "arch=cascadelake", "arch=cooperlake", "arch=alderlake", "arch=raptorlake", "arch=meteorlake", "arch=arrowlake", "arch=arrowlake-s", "arch=lunarlake", "arch=pantherlake", "arch=sierraforest", "arch=grandridge", "arch=clearwaterforest", "arch=bdver4", "arch=znver1", "arch=znver2", "arch=znver3")))
void bpp24to32_simd_init_avx2(uint8_t* bpp24addr, uint8_t* bpp32addr, uint64_t pixel_count)
{
    uint64_t round_count = pixel_count / 64;
    bpp24to32_avx2(bpp24addr, bpp32addr, round_count);
    bpp24addr += round_count * 64 * 3;
    bpp32addr += round_count * 64 * 4;
    pixel_count -= round_count * 64;
    for (uint64_t i = 0; i < pixel_count; i++)
    {
        *bpp32addr = *bpp24addr;
        *(bpp32addr + 1) = *(bpp24addr + 1);
        *(bpp32addr + 2) = *(bpp24addr + 2);
        *(bpp32addr + 3) = 0xff;
        bpp24addr += 3;
        bpp32addr += 4;
    }
    return;
}

#if (defined(__clang_major__) && (__clang_major__ == 21)) || (defined(__GNUC__) && (__GNUC__ == 15))
__attribute__((target_clones("default", "arch=cannonlake", "arch=icelake-client", "arch=rocketlake", "arch=icelake-server", "arch=tigerlake", "arch=sapphirerapids", "arch=graniterapids", "arch=graniterapids-d", "arch=emeraldrapids", "arch=diamondrapids", "arch=znver4", "arch=znver5")))
#elif (defined(__clang_major__) && (__clang_major__ <= 20)) || (defined(__GNUC__) && (__GNUC__ <= 14))
__attribute__((target_clones("default", "arch=cannonlake", "arch=icelake-client", "arch=rocketlake", "arch=icelake-server", "arch=tigerlake", "arch=sapphirerapids", "arch=graniterapids", "arch=graniterapids-d", "arch=emeraldrapids", "arch=znver4", "arch=znver5")))
#endif
void bpp24to32_simd_init_avx512vbmi1(uint8_t* bpp24addr, uint8_t* bpp32addr, uint64_t pixel_count)
{
    uint64_t round_count = pixel_count / 64;
    bpp24to32_avx512vbmi1(bpp24addr, bpp32addr, round_count);
    bpp24addr += round_count * 64 * 3;
    bpp32addr += round_count * 64 * 4;
    pixel_count -= round_count * 64;
    for (uint64_t i = 0; i < pixel_count; i++)
    {
        *bpp32addr = *bpp24addr;
        *(bpp32addr + 1) = *(bpp24addr + 1);
        *(bpp32addr + 2) = *(bpp24addr + 2);
        *(bpp32addr + 3) = 0xff;
        bpp24addr += 3;
        bpp32addr += 4;
    }
    return;
}

#if (defined(__clang_major__) && (__clang_major__ >= 22)) || (defined(__GNUC__) && (__GNUC__ >= 16))
__attribute__((target_clones("default", "arch=haswell", "arch=broadwell", "arch=skylake", "arch=skylake-avx512", "arch=cascadelake", "arch=cooperlake", "arch=cannonlake", "arch=icelake-client", "arch=rocketlake", "arch=icelake-server", "arch=tigerlake", "arch=sapphirerapids", "arch=alderlake", "arch=raptorlake", "arch=meteorlake", "arch=arrowlake", "arch=arrowlake-s", "arch=lunarlake", "arch=pantherlake", "arch=sierraforest", "arch=grandridge", "arch=graniterapids", "arch=graniterapids-d", "arch=emeraldrapids", "arch=clearwaterforest", "arch=diamondrapids", "arch=novalake", "arch=wildcatlake", "arch=bdver1", "arch=bdver2", "arch=bdver3", "arch=bdver4", "arch=znver1", "arch=znver2", "arch=znver3", "arch=znver4", "arch=znver5")))
#elif (defined(__clang_major__) && (__clang_major__ == 21)) || (defined(__GNUC__) && (__GNUC__ == 15))
__attribute__((target_clones("default", "arch=haswell", "arch=broadwell", "arch=skylake", "arch=skylake-avx512", "arch=cascadelake", "arch=cooperlake", "arch=cannonlake", "arch=icelake-client", "arch=rocketlake", "arch=icelake-server", "arch=tigerlake", "arch=sapphirerapids", "arch=alderlake", "arch=raptorlake", "arch=meteorlake", "arch=arrowlake", "arch=arrowlake-s", "arch=lunarlake", "arch=pantherlake", "arch=sierraforest", "arch=grandridge", "arch=graniterapids", "arch=graniterapids-d", "arch=emeraldrapids", "arch=clearwaterforest", "arch=diamondrapids", "arch=bdver1", "arch=bdver2", "arch=bdver3", "arch=bdver4", "arch=znver1", "arch=znver2", "arch=znver3", "arch=znver4", "arch=znver5")))
#elif (defined(__clang_major__) && (__clang_major__ <= 20)) || (defined(__GNUC__) && (__GNUC__ <= 14))
__attribute__((target_clones("default", "arch=haswell", "arch=broadwell", "arch=skylake", "arch=skylake-avx512", "arch=cascadelake", "arch=cooperlake", "arch=cannonlake", "arch=icelake-client", "arch=rocketlake", "arch=icelake-server", "arch=tigerlake", "arch=sapphirerapids", "arch=alderlake", "arch=raptorlake", "arch=meteorlake", "arch=arrowlake", "arch=arrowlake-s", "arch=lunarlake", "arch=pantherlake", "arch=sierraforest", "arch=grandridge", "arch=graniterapids", "arch=graniterapids-d", "arch=emeraldrapids", "arch=clearwaterforest", "arch=bdver1", "arch=bdver2", "arch=bdver3", "arch=bdver4", "arch=znver1", "arch=znver2", "arch=znver3", "arch=znver4", "arch=znver5")))
#endif
void bpp24to32_simd_init(uint8_t* bpp24addr, uint8_t* bpp32addr, uint64_t pixel_count)
{
    if (__builtin_cpu_supports("avx512vbmi"))
    {
        bpp24to32_simd_init_avx2(bpp24addr, bpp32addr, pixel_count);
    }
    else
    {
        bpp24to32_simd_init_avx512vbmi1(bpp24addr, bpp32addr, pixel_count);
    }
}
#endif

#pragma pack(push, 2)
struct bmpindex
{
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t zero1 = 0;
    uint16_t zero2 = 0;
    uint32_t bfOffBits;
    uint32_t bfdib;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biplane;
    uint16_t biBitCount;
    uint32_t zero3 = 0;
    uint32_t zero4 = 0;
    uint32_t zero5 = 0;
    uint32_t zero6 = 0;
    uint32_t zero7 = 0;
    uint32_t zero8 = 0;
};
#pragma pack(pop)

int main(int argc, char* argv[])
{
    printf("Compiler: ");
    uint8_t is_first = 1;
    uint8_t compiler_flags[6] = {0};
    uint8_t isa_flags[6] = {0};
#if defined(__CUDACC__)
    printf("nvcc %d.%d.%d", __CUDACC_VER_MAJOR__, __CUDACC_VER_MINOR__, __CUDACC_VER_BUILD__);
    is_first = 0;
#endif
#if defined(_MSC_VER)
    if (is_first == 0)
    {
        printf(" + ");
    }
    printf("msvc %d(full version %d)", _MSC_VER, _MSC_FULL_VER);
    is_first = 0;
#endif
#if defined(__clang__)
    if (is_first == 0)
    {
        printf(" + ");
    }
    printf("clang %d.%d.%d", __clang_major__, __clang_minor__, __clang_patchlevel__);
    is_first = 0;
#endif
#if defined(__INTEL_COMPILER)
    if (is_first == 0)
    {
        printf(" + ");
    }
    printf("icc %d", __INTEL_COMPILER);
    is_first = 0;
#endif
#if defined(__INTEL_LLVM_COMPILER)
    if (is_first == 0)
    {
        printf(" + ");
    }
    printf("icx %d", __INTEL_LLVM_COMPILER);
    is_first = 0;
#endif
#if defined(__GNUC__)
    if (is_first == 0)
    {
        printf(" + ");
    }
    printf("gcc %d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    is_first = 0;
#endif
    printf("\n");
#if defined(__i386__) || defined(_M_IX86)
    printf("Target Architecture: x86 (32-bit)\n");
#endif
#if defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64)
    printf("Target Architecture: x86_64 / AMD64\n");
#endif
#if defined(__arm__) || defined(_M_ARM)
    printf("Target Architecture: arm32 / aarch32\n");
#endif
#if defined(__aarch64__) || defined(_M_ARM64)
    printf("Target Architecture: arm64 / aarch64\n");
#endif
#if defined(__riscv)
    if (__riscv_xlen == 32)
    {
        printf("Target Architecture: riscv32\n");
    }
    if (__riscv_xlen == 64)
    {
        printf("Target Architecture: riscv64\n");
    }
#endif

    size_t b0, b1;
    b0 = clock();
    uint64_t aligned_mem_size = get_aligned_boundary(sizeof(bmpindex), 128);  //寻找对齐边界
#ifdef _WIN32
    bmpindex* bmp_index = (bmpindex*)_aligned_malloc(aligned_mem_size, 128);
#else
    bmpindex* bmp_index = (bmpindex*)aligned_alloc(128, aligned_mem_size);
#endif
    if (!bmp_index)
    {
        printf("alloc bmp_index fail! bmp_index == 0(NULL)\n");
        return 0;
    }
    memset(bmp_index, 0, aligned_mem_size);
    if (!argv[1])
    {
        printf("no input file!\n");
        return 0;
    }
    FILE* fin;
    fin = fopen(argv[1], "rb");
    if (!fin)
    {
        printf("open input file fail!\n");
        return 0;
    }
    std::filesystem::path p = argv[1];

    // 文件名
    std::cout << "Filename: " << p.filename() << "\n";

    fseek(fin, 0, SEEK_END);
    uint64_t bmpsize = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    aligned_mem_size = get_aligned_boundary(bmpsize, 128);
#ifdef _WIN32
    uint8_t* bpp24_pixel_buffer = (uint8_t*)_aligned_malloc(aligned_mem_size, 128);
#else
    uint8_t* bpp24_pixel_buffer = (uint8_t*)aligned_alloc(128, aligned_mem_size);
#endif
    if (!bpp24_pixel_buffer)
    {
        printf("alloc bmp_filestream fail! bmp_filestream == 0(NULL)\n");
        return 0;
    }
    if (bmpsize > aligned_mem_size)
    {
        printf("alloc bmp_filestream size smaller than bmpsize!\n");
        return 0;
    }

    fread(&bmp_index->bfType, 1, 2, fin);
    if (bmp_index->bfType != 0x4d42)
    {
        printf("Read BM Magic number Error!\n");
        return 0;
    }

    fread(&bmp_index->bfSize, 4, 1, fin);
    fseek(fin, 4, SEEK_CUR);
    fread(&bmp_index->bfOffBits, 4, 1, fin);
    fseek(fin, 4, SEEK_CUR);
    fread(&bmp_index->biWidth, 4, 1, fin);
    fread(&bmp_index->biHeight, 4, 1, fin);
    fseek(fin, 2, SEEK_CUR);
    fread(&bmp_index->biBitCount, 2, 1, fin);

    bmp_index->biBitCount &= 0x0000ffff;

    fseek(fin, bmp_index->bfOffBits, SEEK_SET);

    fread(bpp24_pixel_buffer, 1, bmpsize - (bmp_index->bfOffBits), fin);   //<--- The main read pixel function.


    uint64_t pixel_count = (bmpsize - bmp_index->bfOffBits) / 3;
    if (bmp_index->biBitCount != 24)   //Check valid 24bpp?
    {
        printf("not valid 24bpp bmp.\n");
        return 0;
    }
    aligned_mem_size = get_aligned_boundary(pixel_count << 2, 128);
#ifdef _WIN32
    uint8_t* bpp32_pixel_buffer = (uint8_t*)_aligned_malloc(aligned_mem_size, 128);
#else
    uint8_t* bpp32_pixel_buffer = (uint8_t*)aligned_alloc(128, aligned_mem_size);
#endif
    if (!bpp32_pixel_buffer)
    {
        printf("alloc bpp32_pixel_buffer fail! bpp32_pixel_buffer == 0(NULL)\n");
        return 0;
    }
    b1 = clock();

    printf("init OK...%lldms\n", b1 - b0);
    b0 = clock();
    bpp24to32_simd_init(bpp24_pixel_buffer, bpp32_pixel_buffer, pixel_count);
    b1 = clock();
    printf("process OK...%lldms\n", b1 - b0);
    b0 = clock();
    uint8_t zero1 = 0;
    uint16_t zero2 = 0;
    uint32_t zero4 = 0;
    uint32_t bfdib = 0x28;
    uint16_t biplane = 1;
    p.replace_filename(p.stem().string() + "_32bpp" + p.extension().string());
    FILE* fout;
    fout = fopen((char*)p.string().c_str(), "wb");
    bmp_index->bfSize = pixel_count << 2;
    bmp_index->bfSize += bmp_index->bfOffBits;
    bmp_index->biBitCount = 32;
    bmp_index->bfdib = 0x28;
    bmp_index->biplane = 1;
    fwrite(bmp_index, 2, 27, fout);
    b1 = clock();
    printf("write header time: %lld ms\n", b1 - b0);
    fwrite(bpp32_pixel_buffer, 4, pixel_count, fout);
    fclose(fout);

#ifdef _WIN32
    _aligned_free(bmp_index);
    _aligned_free(bpp24_pixel_buffer);
#else
    free(bmp_index);
    free(bpp24_pixel_buffer);
#endif
    b1 = clock();
    printf("write file time: %lld ms\n", b1 - b0);
}
