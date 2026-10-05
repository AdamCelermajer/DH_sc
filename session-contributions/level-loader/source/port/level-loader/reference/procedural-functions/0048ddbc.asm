
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048ddbc <rnd::ForceBlock::ForceBlock(rnd::RootRule&, rnd::Rule*)>:
  48ddbc: e92d4070     	push	{r4, r5, r6, lr}
  48ddc0: e59f4028     	ldr	r4, [pc, #0x28]         @ 0x48ddf0 <rnd::ForceBlock::ForceBlock(rnd::RootRule&, rnd::Rule*)+0x34>
  48ddc4: e1a05000     	mov	r5, r0
  48ddc8: ebffffb0     	bl	0x48dc90 <rnd::Rule::Rule(rnd::RootRule&, rnd::Rule*)> @ imm = #-0x140
  48ddcc: e59f3020     	ldr	r3, [pc, #0x20]         @ 0x48ddf4 <rnd::ForceBlock::ForceBlock(rnd::RootRule&, rnd::Rule*)+0x38>
  48ddd0: e08f4004     	add	r4, pc, r4
  48ddd4: e3a02004     	mov	r2, #4
  48ddd8: e7943003     	ldr	r3, [r4, r3]
  48dddc: e5852088     	str	r2, [r5, #0x88]
  48dde0: e1a00005     	mov	r0, r5
  48dde4: e2833008     	add	r3, r3, #8
  48dde8: e5853000     	str	r3, [r5]
  48ddec: e8bd8070     	pop	{r4, r5, r6, pc}
  48ddf0: c0 6c 50 00  	.word	0x00506cc0
  48ddf4: 0c 09 00 00  	.word	0x0000090c
