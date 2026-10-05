
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048dd4c <rnd::EndPath::EndPath(rnd::RootRule&, rnd::Rule*)>:
  48dd4c: e92d4070     	push	{r4, r5, r6, lr}
  48dd50: e59f4020     	ldr	r4, [pc, #0x20]         @ 0x48dd78 <rnd::EndPath::EndPath(rnd::RootRule&, rnd::Rule*)+0x2c>
  48dd54: e1a05000     	mov	r5, r0
  48dd58: ebffffcc     	bl	0x48dc90 <rnd::Rule::Rule(rnd::RootRule&, rnd::Rule*)> @ imm = #-0xd0
  48dd5c: e59f3018     	ldr	r3, [pc, #0x18]         @ 0x48dd7c <rnd::EndPath::EndPath(rnd::RootRule&, rnd::Rule*)+0x30>
  48dd60: e08f4004     	add	r4, pc, r4
  48dd64: e1a00005     	mov	r0, r5
  48dd68: e7943003     	ldr	r3, [r4, r3]
  48dd6c: e2833008     	add	r3, r3, #8
  48dd70: e5853000     	str	r3, [r5]
  48dd74: e8bd8070     	pop	{r4, r5, r6, pc}
  48dd78: 30 6d 50 00  	.word	0x00506d30
  48dd7c: f4 2e 00 00  	.word	0x00002ef4
