
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004913d8 <_ZN3rnd4Tile9PlaceTileEiif>:
  4913d8: e92d4070     	push	{r4, r5, r6, lr}
  4913dc: e5904030     	ldr	r4, [r0, #0x30]
  4913e0: e5906004     	ldr	r6, [r0, #0x4]
  4913e4: e1a0c000     	mov	r12, r0
  4913e8: e580308c     	str	r3, [r0, #0x8c]
  4913ec: e1a05001     	mov	r5, r1
  4913f0: e1a0e002     	mov	lr, r2
  4913f4: e58c1084     	str	r1, [r12, #0x84]
  4913f8: e58c2088     	str	r2, [r12, #0x88]
  4913fc: e24dd008     	sub	sp, sp, #8
  491400: e1a0200c     	mov	r2, r12
  491404: e1a00004     	mov	r0, r4
  491408: e594c000     	ldr	r12, [r4]
  49140c: e2861008     	add	r1, r6, #8
  491410: e1a03005     	mov	r3, r5
  491414: e58de000     	str	lr, [sp]
  491418: e1a0e00f     	mov	lr, pc
  49141c: e59cf008     	ldr	pc, [r12, #0x8]
  491420: e28dd008     	add	sp, sp, #8
  491424: e8bd8070     	pop	{r4, r5, r6, pc}
