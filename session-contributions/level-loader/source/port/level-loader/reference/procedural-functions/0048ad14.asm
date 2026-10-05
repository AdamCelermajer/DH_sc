
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048ad14 <rnd::MgxBlock::MgxBlock()>:
  48ad14: e92d4070     	push	{r4, r5, r6, lr}
  48ad18: e59f502c     	ldr	r5, [pc, #0x2c]         @ 0x48ad4c <rnd::MgxBlock::MgxBlock()+0x38>
  48ad1c: e1a04000     	mov	r4, r0
  48ad20: ebffffbf     	bl	0x48ac24 <rnd::Block::Block()> @ imm = #-0x104
  48ad24: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x48ad50 <rnd::MgxBlock::MgxBlock()+0x3c>
  48ad28: e08f5005     	add	r5, pc, r5
  48ad2c: e3a02000     	mov	r2, #0
  48ad30: e7953003     	ldr	r3, [r5, r3]
  48ad34: e58429c4     	str	r2, [r4, #0x9c4]
  48ad38: e58429c0     	str	r2, [r4, #0x9c0]
  48ad3c: e2833008     	add	r3, r3, #8
  48ad40: e5843000     	str	r3, [r4]
  48ad44: e1a00004     	mov	r0, r4
  48ad48: e8bd8070     	pop	{r4, r5, r6, pc}
  48ad4c: 68 9d 50 00  	.word	0x00509d68
  48ad50: 78 19 00 00  	.word	0x00001978
