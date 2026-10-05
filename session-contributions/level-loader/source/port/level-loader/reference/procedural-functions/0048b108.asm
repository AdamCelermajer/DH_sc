
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048b108 <rnd::MgxBlock::MgxBlock()>:
  48b108: e92d4070     	push	{r4, r5, r6, lr}
  48b10c: e59f502c     	ldr	r5, [pc, #0x2c]         @ 0x48b140 <rnd::MgxBlock::MgxBlock()+0x38>
  48b110: e1a04000     	mov	r4, r0
  48b114: ebfffec2     	bl	0x48ac24 <rnd::Block::Block()> @ imm = #-0x4f8
  48b118: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x48b144 <rnd::MgxBlock::MgxBlock()+0x3c>
  48b11c: e08f5005     	add	r5, pc, r5
  48b120: e3a02000     	mov	r2, #0
  48b124: e7953003     	ldr	r3, [r5, r3]
  48b128: e58429c4     	str	r2, [r4, #0x9c4]
  48b12c: e58429c0     	str	r2, [r4, #0x9c0]
  48b130: e2833008     	add	r3, r3, #8
  48b134: e5843000     	str	r3, [r4]
  48b138: e1a00004     	mov	r0, r4
  48b13c: e8bd8070     	pop	{r4, r5, r6, pc}
  48b140: 74 99 50 00  	.word	0x00509974
  48b144: 78 19 00 00  	.word	0x00001978
