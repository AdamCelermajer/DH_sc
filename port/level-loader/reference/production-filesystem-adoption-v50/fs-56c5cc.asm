
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0056c5cc <_GLOBAL__I_.._source_glitch_io_CFileSystem.cpp>:
  56c5cc: e59f1058     	ldr	r1, [pc, #0x58]         @ 0x56c62c <_GLOBAL__I_.._source_glitch_io_CFileSystem.cpp+0x60>
  56c5d0: e59fc058     	ldr	r12, [pc, #0x58]        @ 0x56c630 <_GLOBAL__I_.._source_glitch_io_CFileSystem.cpp+0x64>
  56c5d4: e3a0043f     	mov	r0, #1056964608
  56c5d8: e08f1001     	add	r1, pc, r1
  56c5dc: e59f3050     	ldr	r3, [pc, #0x50]         @ 0x56c634 <_GLOBAL__I_.._source_glitch_io_CFileSystem.cpp+0x68>
  56c5e0: e52d4004     	str	r4, [sp, #-0x4]!
  56c5e4: e59f204c     	ldr	r2, [pc, #0x4c]         @ 0x56c638 <_GLOBAL__I_.._source_glitch_io_CFileSystem.cpp+0x6c>
  56c5e8: e5810008     	str	r0, [r1, #0x8]
  56c5ec: e5810000     	str	r0, [r1]
  56c5f0: e5810004     	str	r0, [r1, #0x4]
  56c5f4: e59f1040     	ldr	r1, [pc, #0x40]         @ 0x56c63c <_GLOBAL__I_.._source_glitch_io_CFileSystem.cpp+0x70>
  56c5f8: e08fc00c     	add	r12, pc, r12
  56c5fc: e79c3003     	ldr	r3, [r12, r3]
  56c600: e79c2002     	ldr	r2, [r12, r2]
  56c604: e79c1001     	ldr	r1, [r12, r1]
  56c608: e3a04000     	mov	r4, #0
  56c60c: e1a00003     	mov	r0, r3
  56c610: e5834010     	str	r4, [r3, #0x10]
  56c614: e5834004     	str	r4, [r3, #0x4]
  56c618: e5c34000     	strb	r4, [r3]
  56c61c: e5833008     	str	r3, [r3, #0x8]
  56c620: e583300c     	str	r3, [r3, #0xc]
  56c624: e8bd0010     	ldm	sp!, {r4}
  56c628: eaf68735     	b	0x30e304 <.plt+0x590>   @ imm = #-0x25e32c
  56c62c: f8 a0 48 00  	.word	0x0048a0f8
  56c630: 98 84 42 00  	.word	0x00428498
  56c634: b4 47 00 00  	.word	0x000047b4
  56c638: 90 18 00 00  	.word	0x00001890
  56c63c: 84 38 00 00  	.word	0x00003884
