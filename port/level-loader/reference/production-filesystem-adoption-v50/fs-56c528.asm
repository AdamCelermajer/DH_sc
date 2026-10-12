
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0056c528 <glitch::io::CFileSystem::changeWorkingDirectoryTo(char const*)>:
  56c528: e92d4070     	push	{r4, r5, r6, lr}
  56c52c: e1a00001     	mov	r0, r1
  56c530: e1a05001     	mov	r5, r1
  56c534: ebf68a09     	bl	0x30ed60 <.plt+0xfec>   @ imm = #-0x25d7dc
  56c538: e59f3028     	ldr	r3, [pc, #0x28]         @ 0x56c568 <glitch::io::CFileSystem::changeWorkingDirectoryTo(char const*)+0x40>
  56c53c: e2704001     	rsbs	r4, r0, #1
  56c540: 33a04000     	movlo	r4, #0
  56c544: e3540000     	cmp	r4, #0
  56c548: e08f3003     	add	r3, pc, r3
  56c54c: 0a000003     	beq	0x56c560 <glitch::io::CFileSystem::changeWorkingDirectoryTo(char const*)+0x38> @ imm = #0xc
  56c550: e59f2014     	ldr	r2, [pc, #0x14]         @ 0x56c56c <glitch::io::CFileSystem::changeWorkingDirectoryTo(char const*)+0x44>
  56c554: e1a01005     	mov	r1, r5
  56c558: e7930002     	ldr	r0, [r3, r2]
  56c55c: ebf687ef     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0x25e044
  56c560: e1a00004     	mov	r0, r4
  56c564: e8bd8070     	pop	{r4, r5, r6, pc}
  56c568: 48 85 42 00  	.word	0x00428548
  56c56c: 68 0e 00 00  	.word	0x00000e68
