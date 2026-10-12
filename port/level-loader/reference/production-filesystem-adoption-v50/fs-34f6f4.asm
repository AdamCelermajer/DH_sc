
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034f6f4 <FileSystemWin32::FileSystemWin32()>:
  34f6f4: e92d4070     	push	{r4, r5, r6, lr}
  34f6f8: e59f6080     	ldr	r6, [pc, #0x80]         @ 0x34f780 <FileSystemWin32::FileSystemWin32()+0x8c>
  34f6fc: e1a04000     	mov	r4, r0
  34f700: ebfffb8c     	bl	0x34e538 <FileSystemBase::FileSystemBase()> @ imm = #-0x11d0
  34f704: e59f3078     	ldr	r3, [pc, #0x78]         @ 0x34f784 <FileSystemWin32::FileSystemWin32()+0x90>
  34f708: e08f6006     	add	r6, pc, r6
  34f70c: e3a05f41     	mov	r5, #260
  34f710: e7963003     	ldr	r3, [r6, r3]
  34f714: e1a00004     	mov	r0, r4
  34f718: e1a02005     	mov	r2, r5
  34f71c: e2833008     	add	r3, r3, #8
  34f720: e480302c     	str	r3, [r0], #44
  34f724: e3a01000     	mov	r1, #0
  34f728: ebfefb4c     	bl	0x30e460 <.plt+0x6ec>   @ imm = #-0x412d0
  34f72c: e1a02005     	mov	r2, r5
  34f730: e3a01000     	mov	r1, #0
  34f734: e2840e13     	add	r0, r4, #304
  34f738: ebfefb48     	bl	0x30e460 <.plt+0x6ec>   @ imm = #-0x412e0
  34f73c: e1a02005     	mov	r2, r5
  34f740: e3a01000     	mov	r1, #0
  34f744: e2840f8d     	add	r0, r4, #564
  34f748: ebfefb44     	bl	0x30e460 <.plt+0x6ec>   @ imm = #-0x412f0
  34f74c: e1a02005     	mov	r2, r5
  34f750: e3a01000     	mov	r1, #0
  34f754: e2840fce     	add	r0, r4, #824
  34f758: ebfefb40     	bl	0x30e460 <.plt+0x6ec>   @ imm = #-0x41300
  34f75c: e2840e43     	add	r0, r4, #1072
  34f760: e1a02005     	mov	r2, r5
  34f764: e3a01000     	mov	r1, #0
  34f768: e280000c     	add	r0, r0, #12
  34f76c: ebfefb3b     	bl	0x30e460 <.plt+0x6ec>   @ imm = #-0x41314
  34f770: e1a00004     	mov	r0, r4
  34f774: ebfffe3b     	bl	0x34f068 <FileSystemWin32::_getPaths()> @ imm = #-0x714
  34f778: e1a00004     	mov	r0, r4
  34f77c: e8bd8070     	pop	{r4, r5, r6, pc}
  34f780: 88 53 64 00  	.word	0x00645388
  34f784: 54 16 00 00  	.word	0x00001654
