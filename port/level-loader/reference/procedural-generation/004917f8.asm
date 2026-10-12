
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004917f8 <_ZN3rnd4TileC1ERNS_15RandomGeneratorERNS_5BlockEPS0_RNS_8ListElemE>:
  4917f8: e92d4070     	push	{r4, r5, r6, lr}
  4917fc: e2805034     	add	r5, r0, #52
  491800: e1a04000     	mov	r4, r0
  491804: e980000a     	stmib	r0, {r1, r3}
  491808: e5802030     	str	r2, [r0, #0x30]
  49180c: e1a00005     	mov	r0, r5
  491810: ebfff21a     	bl	0x48e080 <_ZN3rnd8ListElemC1Ev> @ imm = #-0x3798
  491814: e5940008     	ldr	r0, [r4, #0x8]
  491818: e3a03000     	mov	r3, #0
  49181c: e584300c     	str	r3, [r4, #0xc]
  491820: e1500003     	cmp	r0, r3
  491824: 0a000001     	beq	0x491830 <_ZN3rnd4TileC1ERNS_15RandomGeneratorERNS_5BlockEPS0_RNS_8ListElemE+0x38> @ imm = #0x4
  491828: e1a01004     	mov	r1, r4
  49182c: ebfffefd     	bl	0x491428 <_ZN3rnd4Tile8AddChildEPS0_> @ imm = #-0x40c
  491830: e5943030     	ldr	r3, [r4, #0x30]
  491834: e59d1010     	ldr	r1, [sp, #0x10]
  491838: e1a00005     	mov	r0, r5
  49183c: e5933018     	ldr	r3, [r3, #0x18]
  491840: e5843000     	str	r3, [r4]
  491844: ebffea1c     	bl	0x48c0bc <_ZN3rnd8ListElemaSERKS0_> @ imm = #-0x5790
  491848: e1a00004     	mov	r0, r4
  49184c: e8bd8070     	pop	{r4, r5, r6, pc}
