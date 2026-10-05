
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048dd80 <rnd::ForceBlock::ForceBlock(rnd::RootRule&, rnd::Rule*)>:
  48dd80: e92d4070     	push	{r4, r5, r6, lr}
  48dd84: e59f4028     	ldr	r4, [pc, #0x28]         @ 0x48ddb4 <rnd::ForceBlock::ForceBlock(rnd::RootRule&, rnd::Rule*)+0x34>
  48dd88: e1a05000     	mov	r5, r0
  48dd8c: ebffffbf     	bl	0x48dc90 <rnd::Rule::Rule(rnd::RootRule&, rnd::Rule*)> @ imm = #-0x104
  48dd90: e59f3020     	ldr	r3, [pc, #0x20]         @ 0x48ddb8 <rnd::ForceBlock::ForceBlock(rnd::RootRule&, rnd::Rule*)+0x38>
  48dd94: e08f4004     	add	r4, pc, r4
  48dd98: e3a02004     	mov	r2, #4
  48dd9c: e7943003     	ldr	r3, [r4, r3]
  48dda0: e5852088     	str	r2, [r5, #0x88]
  48dda4: e1a00005     	mov	r0, r5
  48dda8: e2833008     	add	r3, r3, #8
  48ddac: e5853000     	str	r3, [r5]
  48ddb0: e8bd8070     	pop	{r4, r5, r6, pc}
  48ddb4: fc 6c 50 00  	.word	0x00506cfc
  48ddb8: 0c 09 00 00  	.word	0x0000090c
