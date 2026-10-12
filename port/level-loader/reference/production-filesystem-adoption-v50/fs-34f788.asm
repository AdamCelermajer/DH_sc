
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034f788 <FileSystemWin32::FileSystemWin32()>:
  34f788: e92d4070     	push	{r4, r5, r6, lr}
  34f78c: e59f6080     	ldr	r6, [pc, #0x80]         @ 0x34f814 <FileSystemWin32::FileSystemWin32()+0x8c>
  34f790: e1a04000     	mov	r4, r0
  34f794: ebfffb67     	bl	0x34e538 <FileSystemBase::FileSystemBase()> @ imm = #-0x1264
  34f798: e59f3078     	ldr	r3, [pc, #0x78]         @ 0x34f818 <FileSystemWin32::FileSystemWin32()+0x90>
  34f79c: e08f6006     	add	r6, pc, r6
  34f7a0: e3a05f41     	mov	r5, #260
  34f7a4: e7963003     	ldr	r3, [r6, r3]
  34f7a8: e1a00004     	mov	r0, r4
  34f7ac: e1a02005     	mov	r2, r5
  34f7b0: e2833008     	add	r3, r3, #8
  34f7b4: e480302c     	str	r3, [r0], #44
  34f7b8: e3a01000     	mov	r1, #0
  34f7bc: ebfefb27     	bl	0x30e460 <.plt+0x6ec>   @ imm = #-0x41364
  34f7c0: e1a02005     	mov	r2, r5
  34f7c4: e3a01000     	mov	r1, #0
  34f7c8: e2840e13     	add	r0, r4, #304
  34f7cc: ebfefb23     	bl	0x30e460 <.plt+0x6ec>   @ imm = #-0x41374
  34f7d0: e1a02005     	mov	r2, r5
  34f7d4: e3a01000     	mov	r1, #0
  34f7d8: e2840f8d     	add	r0, r4, #564
  34f7dc: ebfefb1f     	bl	0x30e460 <.plt+0x6ec>   @ imm = #-0x41384
  34f7e0: e1a02005     	mov	r2, r5
  34f7e4: e3a01000     	mov	r1, #0
  34f7e8: e2840fce     	add	r0, r4, #824
  34f7ec: ebfefb1b     	bl	0x30e460 <.plt+0x6ec>   @ imm = #-0x41394
  34f7f0: e2840e43     	add	r0, r4, #1072
  34f7f4: e1a02005     	mov	r2, r5
  34f7f8: e3a01000     	mov	r1, #0
  34f7fc: e280000c     	add	r0, r0, #12
  34f800: ebfefb16     	bl	0x30e460 <.plt+0x6ec>   @ imm = #-0x413a8
  34f804: e1a00004     	mov	r0, r4
  34f808: ebfffe16     	bl	0x34f068 <FileSystemWin32::_getPaths()> @ imm = #-0x7a8
  34f80c: e1a00004     	mov	r0, r4
  34f810: e8bd8070     	pop	{r4, r5, r6, pc}
  34f814: f4 52 64 00  	.word	0x006452f4
  34f818: 54 16 00 00  	.word	0x00001654
