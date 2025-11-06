.data 

data2 SEGMENT align(1024)

; 00h, 01h, 02h, 80h, 03h, 04h, 05h, 80h, 06h, 07h, 08h, 80h, 09h, 10h, 11h, ...   another style
align 32
shuffle_imm0 BYTE 0, 1, 2, 128, 3, 4, 5, 128, 6, 7, 8, 128, 9, 10, 11, 128, 12, 13, 14, 128, 15, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128
shuffle_imm1 BYTE 8, 9, 10, 128, 11, 12, 13, 128, 14, 15, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 0, 1, 128, 2, 3, 4, 128, 5, 6, 7, 128
shuffle_imm2 BYTE 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 0, 128, 1, 2, 3, 128, 4, 5, 6, 128, 7, 8, 9, 128, 10, 11, 12, 128, 13, 14, 15, 128 
or_imm BYTE 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255
blendvb_imm0 BYTE 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 0, 128, 128, 128, 0, 128, 128, 128, 0
blendvb_imm1 BYTE 128, 128, 128, 0, 128, 128, 128, 0, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
align 64
vpermb_imm0 BYTE 0, 1, 2, 255, 3, 4, 5, 255, 6, 7, 8, 255, 9, 10, 11, 255, 12, 13, 14, 255, 15, 16, 17, 255, 18, 19, 20, 255, 21, 22, 23, 255
			BYTE 24, 25, 26, 255, 27, 28, 29, 255, 30, 31, 32, 255, 33, 34, 35, 255, 36, 37, 38, 255, 39, 40, 41, 255, 42, 43, 44, 255, 45, 46, 47, 255 
vpermb_imm1 BYTE 16, 17, 18, 255, 19, 20, 21, 255, 22, 23, 24, 255, 25, 26, 27, 255, 28, 29, 30, 255, 31, 32, 33, 255, 34, 35, 36, 255, 37, 38, 39, 255
			BYTE 40, 41, 42, 255, 43, 44, 45, 255, 46, 47, 48, 255, 49, 50, 51, 255, 52, 53, 54, 255, 55, 56, 57, 255, 58, 59, 60, 255, 61, 62, 63, 255
vpermt2b_imm0 BYTE 48, 49, 50, 255, 51, 52, 53, 255, 54, 55, 56, 255, 57, 58, 59, 255, 60, 61, 62, 255, 63, 64, 65, 255, 66, 67, 68, 255, 69, 70, 71, 255
			  BYTE 72, 73, 74, 255, 75, 76, 77, 255, 78, 79, 80, 255, 81, 82, 83, 255, 84, 85, 86, 255, 87, 88, 89, 255, 90, 91, 92, 255, 93, 94, 95, 255
vpermt2b_imm1 BYTE 32, 33, 34, 255, 35, 36, 37, 255, 38, 39, 40, 255, 41, 42, 43, 255, 44, 45, 46, 255, 47, 48, 49, 255, 50, 51, 52, 255, 53, 54, 55, 255
			  BYTE 56, 57, 58, 255, 59, 60, 61, 255, 62, 63, 64, 255, 65, 66, 67, 255, 68, 69, 70, 255, 71, 72, 73, 255, 74, 75, 76, 255, 77, 78, 79, 255
blendmb_imm BYTE 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255
			BYTE 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255
mask_imm QWORD 07777777777777777h
data2 ends



.code
align 16
; rcx = bpp24_addr, rdx = bpp32_addr, r8 = pixel_count
bpp24to32_avx2 proc
	sub rsp, 320
	vmovdqu xmmword ptr [rsp], xmm6
	vmovdqu xmmword ptr [rsp + 32], xmm7
	vmovdqu xmmword ptr [rsp + 64], xmm8
	vmovdqu xmmword ptr [rsp + 96], xmm9
	vmovdqu xmmword ptr [rsp + 128], xmm10
	vmovdqu xmmword ptr [rsp + 160], xmm11
	vmovdqu xmmword ptr [rsp + 192], xmm12
	vmovdqu xmmword ptr [rsp + 224], xmm13
	vmovdqu xmmword ptr [rsp + 256], xmm14
	vmovdqu xmmword ptr [rsp + 288], xmm15
	prefetcht0 byte ptr [rcx]
	vmovdqa ymm10, ymmword ptr [shuffle_imm0]
	vmovdqa ymm11, ymmword ptr [shuffle_imm1]
	vmovdqa ymm12, ymmword ptr [shuffle_imm2]
	vmovdqa ymm13, ymmword ptr [or_imm]
	vmovdqa ymm14, ymmword ptr [blendvb_imm0]
	vmovdqa ymm15, ymmword ptr [blendvb_imm1]
	align 16
	single_round:
		vbroadcasti128 ymm0, xmmword ptr [rcx]
		vbroadcasti128 ymm1, xmmword ptr [rcx + 16]
		vpshufb ymm0, ymm0, ymm10
		vpshufb ymm1, ymm1, ymm11   ; ymm1 use again at Tag 1
		prefetcht0 byte ptr [rcx + 64]
		vpor ymm0, ymm0, ymm13
		vbroadcasti128 ymm2, xmmword ptr [rcx + 32]
		vbroadcasti128 ymm3, xmmword ptr [rcx + 48]   ; ymm3 use at Tag 2
		vpblendvb ymm0, ymm0, ymm1, ymm14

		vpshufb ymm2, ymm2, ymm12
		vpshufb ymm3, ymm3, ymm10
		vbroadcasti128 ymm4, xmmword ptr [rcx + 64]
		vpor ymm2, ymm2, ymm13
		vbroadcasti128 ymm5, xmmword ptr [rcx + 80]
		vpblendvb ymm2, ymm2, ymm1, ymm15   ; <--- Tag 1 is here!

		vmovntdq ymmword ptr [rdx], ymm0
		vpshufb ymm4, ymm4, ymm11   ; ymm4 use again at Tag 3
		vpshufb ymm5, ymm5, ymm12
		prefetcht0 byte ptr [rcx + 128]
		vpor ymm5, ymm5, ymm13
		vbroadcasti128 ymm6, xmmword ptr [rcx + 96]
		vbroadcasti128 ymm7, xmmword ptr [rcx + 112]   ; ymm7 use again at Tag 4
		vpblendvb ymm3, ymm3, ymm4, ymm14   ; <--- Tag 2 is here!
		vmovntdq ymmword ptr [rdx + 32], ymm2
		vpshufb ymm6, ymm6, ymm10
		vpshufb ymm7, ymm7, ymm11
		vpblendvb ymm5, ymm5, ymm4, ymm15   ; <--- Tag 3 is here!

		; 96 Bytes complete!

		vpor ymm3, ymm3, ymm13
		vpor ymm6, ymm6, ymm13
		vmovntdq ymmword ptr [rdx + 64], ymm3
		vbroadcasti128 ymm8, xmmword ptr [rcx + 128]
		vbroadcasti128 ymm9, xmmword ptr [rcx + 144]   ; ymm9 use at Tag 5
		vpblendvb ymm6, ymm6, ymm7, ymm14

		vmovntdq ymmword ptr [rdx + 96], ymm5
		vpshufb ymm8, ymm8, ymm12
		vpshufb ymm9, ymm9, ymm10
		prefetcht0 byte ptr [rcx + 192]
		vpor ymm8, ymm8, ymm13
		vmovntdq ymmword ptr [rdx + 128], ymm6
		vpblendvb ymm8, ymm8, ymm7, ymm15   ; Tag 4 at here!

		vbroadcasti128 ymm0, xmmword ptr [rcx + 160]
		vbroadcasti128 ymm1, xmmword ptr [rcx + 176]
		vpor ymm9, ymm9, ymm13
		vpshufb ymm0, ymm0, ymm11
		vpshufb ymm1, ymm1, ymm12	
		vmovntdq ymmword ptr [rdx + 160], ymm8
		vpor ymm1, ymm1, ymm13
		add rcx, 192
		vpblendvb ymm9, ymm9, ymm0, ymm14   ; <--- Tag 5 is here!
		vpblendvb ymm1, ymm1, ymm0, ymm15

		vmovntdq ymmword ptr [rdx + 192], ymm9
		vmovntdq ymmword ptr [rdx + 224], ymm1
		add rdx, 256
		sub r8, 1
		jne single_round
	vmovdqu xmm15, xmmword ptr [rsp + 288]
	vmovdqu xmm14, xmmword ptr [rsp + 256]
	vmovdqu xmm13, xmmword ptr [rsp + 224]
	vmovdqu xmm12, xmmword ptr [rsp + 192]
	vmovdqu xmm11, xmmword ptr [rsp + 160]
	vmovdqu xmm10, xmmword ptr [rsp + 128]
	vmovdqu xmm9, xmmword ptr [rsp + 96]
	vmovdqu xmm8, xmmword ptr [rsp + 64]
	vmovdqu xmm7, xmmword ptr [rsp + 32]
	vmovdqu xmm6, xmmword ptr [rsp]
	add rsp, 320
	vzeroupper
	ret
bpp24to32_avx2 endp

align 16
bpp24to32_avx512vbmi1 proc
	sub rsp, 96
	vmovdqu xmmword ptr [rsp], xmm6
	vmovdqu xmmword ptr [rsp + 32], xmm7
	vmovdqu xmmword ptr [rsp + 64], xmm8
	prefetcht0 byte ptr [rcx]
	vmovdqa64 zmm4, zmmword ptr [vpermb_imm0]
	vmovdqa64 zmm5, zmmword ptr [vpermb_imm1]
	vmovdqa64 zmm6, zmmword ptr [vpermt2b_imm0]
	vmovdqa64 zmm7, zmmword ptr [vpermt2b_imm1]
	vmovdqa64 zmm8, zmmword ptr [blendmb_imm]
	kmovq k1, qword ptr [mask_imm]
	align 16
	single_round:		
		vmovdqa64 zmm0, zmmword ptr [rcx]
		vmovdqa64 zmm1, zmmword ptr [rcx + 64]
		vmovdqa64 zmm2, zmmword ptr [rcx + 128]
		vpermb zmm3, zmm4, zmm0
		vpermt2b zmm0{k1}{z}, zmm6, zmm1
		vpermt2b zmm1{k1}{z}, zmm7, zmm2
		vpermb zmm2, zmm5, zmm2
		prefetcht0 byte ptr [rcx + 192]
		vpblendmb zmm3{k1}, zmm8, zmm3
		vpblendmb zmm0{k1}, zmm8, zmm0
		vmovntdq zmmword ptr [rdx], zmm3
		vpblendmb zmm1{k1}, zmm8, zmm1
		vpblendmb zmm2{k1}, zmm8, zmm2
		vmovntdq zmmword ptr [rdx + 64], zmm0
		vmovntdq zmmword ptr [rdx + 128], zmm1
		add rcx, 192
		vmovntdq zmmword ptr [rdx + 192], zmm2
		add rdx, 256
		sub r8, 1
		jne single_round
	vmovdqu xmm8, xmmword ptr [rsp + 64]
	vmovdqu xmm7, xmmword ptr [rsp + 32]
	vmovdqu xmm6, xmmword ptr [rsp]
	add rsp, 96
	vzeroupper
	ret
bpp24to32_avx512vbmi1 endp

end
