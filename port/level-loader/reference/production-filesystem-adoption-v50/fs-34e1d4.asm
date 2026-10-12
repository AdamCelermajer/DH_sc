
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034e1d4 <FileSystemBase::changeWorkingDirectoryTo(char const*)>:
  34e1d4: e92d4070     	push	{r4, r5, r6, lr}
  34e1d8: e1a00001     	mov	r0, r1
  34e1dc: e1a05001     	mov	r5, r1
  34e1e0: ebff02de     	bl	0x30ed60 <.plt+0xfec>   @ imm = #-0x3f488
  34e1e4: e59f3028     	ldr	r3, [pc, #0x28]         @ 0x34e214 <FileSystemBase::changeWorkingDirectoryTo(char const*)+0x40>
  34e1e8: e2704001     	rsbs	r4, r0, #1
  34e1ec: 33a04000     	movlo	r4, #0
  34e1f0: e3540000     	cmp	r4, #0
  34e1f4: e08f3003     	add	r3, pc, r3
  34e1f8: 0a000003     	beq	0x34e20c <FileSystemBase::changeWorkingDirectoryTo(char const*)+0x38> @ imm = #0xc
  34e1fc: e59f2014     	ldr	r2, [pc, #0x14]         @ 0x34e218 <FileSystemBase::changeWorkingDirectoryTo(char const*)+0x44>
  34e200: e1a01005     	mov	r1, r5
  34e204: e7930002     	ldr	r0, [r3, r2]
  34e208: ebff00c4     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0x3fcf0
  34e20c: e1a00004     	mov	r0, r4
  34e210: e8bd8070     	pop	{r4, r5, r6, pc}
  34e214: 9c 68 64 00  	.word	0x0064689c
  34e218: 68 0e 00 00  	.word	0x00000e68
