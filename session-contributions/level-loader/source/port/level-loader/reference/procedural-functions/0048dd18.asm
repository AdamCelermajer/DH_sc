
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048dd18 <rnd::EndPath::EndPath(rnd::RootRule&, rnd::Rule*)>:
  48dd18: e92d4070     	push	{r4, r5, r6, lr}
  48dd1c: e59f4020     	ldr	r4, [pc, #0x20]         @ 0x48dd44 <rnd::EndPath::EndPath(rnd::RootRule&, rnd::Rule*)+0x2c>
  48dd20: e1a05000     	mov	r5, r0
  48dd24: ebffffd9     	bl	0x48dc90 <rnd::Rule::Rule(rnd::RootRule&, rnd::Rule*)> @ imm = #-0x9c
  48dd28: e59f3018     	ldr	r3, [pc, #0x18]         @ 0x48dd48 <rnd::EndPath::EndPath(rnd::RootRule&, rnd::Rule*)+0x30>
  48dd2c: e08f4004     	add	r4, pc, r4
  48dd30: e1a00005     	mov	r0, r5
  48dd34: e7943003     	ldr	r3, [r4, r3]
  48dd38: e2833008     	add	r3, r3, #8
  48dd3c: e5853000     	str	r3, [r5]
  48dd40: e8bd8070     	pop	{r4, r5, r6, pc}
  48dd44: 64 6d 50 00  	.word	0x00506d64
  48dd48: f4 2e 00 00  	.word	0x00002ef4
