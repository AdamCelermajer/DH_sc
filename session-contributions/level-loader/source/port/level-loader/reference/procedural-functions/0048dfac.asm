
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048dfac <rnd::Path::Path(rnd::RootRule&, rnd::Rule*)>:
  48dfac: e92d4070     	push	{r4, r5, r6, lr}
  48dfb0: e59f5038     	ldr	r5, [pc, #0x38]         @ 0x48dff0 <rnd::Path::Path(rnd::RootRule&, rnd::Rule*)+0x44>
  48dfb4: e1a04000     	mov	r4, r0
  48dfb8: ebffff34     	bl	0x48dc90 <rnd::Rule::Rule(rnd::RootRule&, rnd::Rule*)> @ imm = #-0x330
  48dfbc: e59f3030     	ldr	r3, [pc, #0x30]         @ 0x48dff4 <rnd::Path::Path(rnd::RootRule&, rnd::Rule*)+0x48>
  48dfc0: e08f5005     	add	r5, pc, r5
  48dfc4: e1a00004     	mov	r0, r4
  48dfc8: e7953003     	ldr	r3, [r5, r3]
  48dfcc: e2833008     	add	r3, r3, #8
  48dfd0: e5843000     	str	r3, [r4]
  48dfd4: e3a03001     	mov	r3, #1
  48dfd8: e5c4308c     	strb	r3, [r4, #0x8c]
  48dfdc: e3a03000     	mov	r3, #0
  48dfe0: e5c4308d     	strb	r3, [r4, #0x8d]
  48dfe4: e3a03004     	mov	r3, #4
  48dfe8: e5843088     	str	r3, [r4, #0x88]
  48dfec: e8bd8070     	pop	{r4, r5, r6, pc}
  48dff0: d0 6a 50 00  	.word	0x00506ad0
  48dff4: 34 2c 00 00  	.word	0x00002c34
