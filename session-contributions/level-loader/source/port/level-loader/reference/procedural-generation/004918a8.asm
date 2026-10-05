
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004918a8 <_ZN3rnd4Tile7UnspawnEv>:
  4918a8: e92d4010     	push	{r4, lr}
  4918ac: e1a04000     	mov	r4, r0
  4918b0: e5900008     	ldr	r0, [r0, #0x8]
  4918b4: e24dd008     	sub	sp, sp, #8
  4918b8: e3500000     	cmp	r0, #0
  4918bc: 0a000001     	beq	0x4918c8 <_ZN3rnd4Tile7UnspawnEv+0x20> @ imm = #0x4
  4918c0: e1a01004     	mov	r1, r4
  4918c4: ebfffedd     	bl	0x491440 <_ZN3rnd4Tile11RemoveChildEPS0_> @ imm = #-0x48c
  4918c8: e5942030     	ldr	r2, [r4, #0x30]
  4918cc: e5941004     	ldr	r1, [r4, #0x4]
  4918d0: e594e088     	ldr	lr, [r4, #0x88]
  4918d4: e592c000     	ldr	r12, [r2]
  4918d8: e5943084     	ldr	r3, [r4, #0x84]
  4918dc: e2811008     	add	r1, r1, #8
  4918e0: e1a00002     	mov	r0, r2
  4918e4: e58de000     	str	lr, [sp]
  4918e8: e1a02004     	mov	r2, r4
  4918ec: e1a0e00f     	mov	lr, pc
  4918f0: e59cf00c     	ldr	pc, [r12, #0xc]
  4918f4: e1a00004     	mov	r0, r4
  4918f8: eb00000a     	bl	0x491928 <_ZN3rnd4Tile15RemoveNeighborsEv> @ imm = #0x28
  4918fc: e5940034     	ldr	r0, [r4, #0x34]
  491900: e3500000     	cmp	r0, #0
  491904: 0a000001     	beq	0x491910 <_ZN3rnd4Tile7UnspawnEv+0x68> @ imm = #0x4
  491908: e2841034     	add	r1, r4, #52
  49190c: ebfff4d8     	bl	0x48ec74 <_ZN3rnd8ListRule7ReplaceERNS_8ListElemE> @ imm = #-0x2ca0
  491910: e1a00004     	mov	r0, r4
  491914: ebfffef3     	bl	0x4914e8 <_ZN3rnd4TileD1Ev> @ imm = #-0x434
  491918: e1a00004     	mov	r0, r4
  49191c: e28dd008     	add	sp, sp, #8
  491920: e8bd4010     	pop	{r4, lr}
  491924: eaf9fac5     	b	0x310440 <_Z10CustomFreePv> @ imm = #-0x1814ec
