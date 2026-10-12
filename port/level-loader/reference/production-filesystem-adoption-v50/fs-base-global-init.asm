
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034e56c <_GLOBAL__I_.._.._sources_Core_Irrlicht_FileSystemBase.cpp>:
  34e56c: e92d4070     	push	{r4, r5, r6, lr}
  34e570: e59f4064     	ldr	r4, [pc, #0x64]         @ 0x34e5dc <_GLOBAL__I_.._.._sources_Core_Irrlicht_FileSystemBase.cpp+0x70>
  34e574: e59f2064     	ldr	r2, [pc, #0x64]         @ 0x34e5e0 <_GLOBAL__I_.._.._sources_Core_Irrlicht_FileSystemBase.cpp+0x74>
  34e578: e59f3064     	ldr	r3, [pc, #0x64]         @ 0x34e5e4 <_GLOBAL__I_.._.._sources_Core_Irrlicht_FileSystemBase.cpp+0x78>
  34e57c: e08f4004     	add	r4, pc, r4
  34e580: e7941002     	ldr	r1, [r4, r2]
  34e584: e08f3003     	add	r3, pc, r3
  34e588: e3a0243f     	mov	r2, #1056964608
  34e58c: e5910000     	ldr	r0, [r1]
  34e590: e5832008     	str	r2, [r3, #0x8]
  34e594: e5832000     	str	r2, [r3]
  34e598: e3100001     	tst	r0, #1
  34e59c: e5832004     	str	r2, [r3, #0x4]
  34e5a0: 0a000000     	beq	0x34e5a8 <_GLOBAL__I_.._.._sources_Core_Irrlicht_FileSystemBase.cpp+0x3c> @ imm = #0x0
  34e5a4: e8bd8070     	pop	{r4, r5, r6, pc}
  34e5a8: e3a03001     	mov	r3, #1
  34e5ac: e5813000     	str	r3, [r1]
  34e5b0: e59f3030     	ldr	r3, [pc, #0x30]         @ 0x34e5e8 <_GLOBAL__I_.._.._sources_Core_Irrlicht_FileSystemBase.cpp+0x7c>
  34e5b4: e7945003     	ldr	r5, [r4, r3]
  34e5b8: e1a00005     	mov	r0, r5
  34e5bc: ebff7c76     	bl	0x32d79c <Application::Application()> @ imm = #-0x20e28
  34e5c0: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x34e5ec <_GLOBAL__I_.._.._sources_Core_Irrlicht_FileSystemBase.cpp+0x80>
  34e5c4: e1a00005     	mov	r0, r5
  34e5c8: e7941003     	ldr	r1, [r4, r3]
  34e5cc: e59f301c     	ldr	r3, [pc, #0x1c]         @ 0x34e5f0 <_GLOBAL__I_.._.._sources_Core_Irrlicht_FileSystemBase.cpp+0x84>
  34e5d0: e7942003     	ldr	r2, [r4, r3]
  34e5d4: e8bd4070     	pop	{r4, r5, r6, lr}
  34e5d8: eafeff49     	b	0x30e304 <.plt+0x590>   @ imm = #-0x402dc
  34e5dc: 14 65 64 00  	.word	0x00646514
  34e5e0: ac 0f 00 00  	.word	0x00000fac
  34e5e4: 28 39 65 00  	.word	0x00653928
  34e5e8: f4 37 00 00  	.word	0x000037f4
  34e5ec: c0 08 00 00  	.word	0x000008c0
  34e5f0: 90 18 00 00  	.word	0x00001890
