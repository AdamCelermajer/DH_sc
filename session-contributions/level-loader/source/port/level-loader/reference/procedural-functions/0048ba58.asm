
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048ba58 <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)>:
  48ba58: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  48ba5c: e1a06000     	mov	r6, r0
  48ba60: e5900058     	ldr	r0, [r0, #0x58]
  48ba64: e59f8138     	ldr	r8, [pc, #0x138]        @ 0x48bba4 <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x14c>
  48ba68: e24dd024     	sub	sp, sp, #36
  48ba6c: e3500000     	cmp	r0, #0
  48ba70: e08f8008     	add	r8, pc, r8
  48ba74: e1a07001     	mov	r7, r1
  48ba78: e58d201c     	str	r2, [sp, #0x1c]
  48ba7c: e1a0b003     	mov	r11, r3
  48ba80: da00003d     	ble	0x48bb7c <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x124> @ imm = #0xf4
  48ba84: e59f311c     	ldr	r3, [pc, #0x11c]        @ 0x48bba8 <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x150>
  48ba88: e3a02000     	mov	r2, #0
  48ba8c: e58d2008     	str	r2, [sp, #0x8]
  48ba90: e08f3003     	add	r3, pc, r3
  48ba94: e58d3010     	str	r3, [sp, #0x10]
  48ba98: e59f310c     	ldr	r3, [pc, #0x10c]        @ 0x48bbac <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x154>
  48ba9c: e08f3003     	add	r3, pc, r3
  48baa0: e58d3014     	str	r3, [sp, #0x14]
  48baa4: e59f3104     	ldr	r3, [pc, #0x104]        @ 0x48bbb0 <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x158>
  48baa8: e08f3003     	add	r3, pc, r3
  48baac: e58d3018     	str	r3, [sp, #0x18]
  48bab0: e5963054     	ldr	r3, [r6, #0x54]
  48bab4: e3530000     	cmp	r3, #0
  48bab8: da00002a     	ble	0x48bb68 <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x110> @ imm = #0xa8
  48babc: e59d3008     	ldr	r3, [sp, #0x8]
  48bac0: e59dc048     	ldr	r12, [sp, #0x48]
  48bac4: e59f20e8     	ldr	r2, [pc, #0xe8]         @ 0x48bbb4 <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x15c>
  48bac8: e3a04000     	mov	r4, #0
  48bacc: e083500c     	add	r5, r3, r12
  48bad0: e59f90e0     	ldr	r9, [pc, #0xe0]         @ 0x48bbb8 <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x160>
  48bad4: e58d200c     	str	r2, [sp, #0xc]
  48bad8: e1a0a005     	mov	r10, r5
  48badc: ea000003     	b	0x48baf0 <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x98> @ imm = #0xc
  48bae0: e5963054     	ldr	r3, [r6, #0x54]
  48bae4: e2844001     	add	r4, r4, #1
  48bae8: e1530004     	cmp	r3, r4
  48baec: da00001c     	ble	0x48bb64 <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x10c> @ imm = #0x70
  48baf0: e084500b     	add	r5, r4, r11
  48baf4: e1a01005     	mov	r1, r5
  48baf8: e1a0200a     	mov	r2, r10
  48bafc: e1a00007     	mov	r0, r7
  48bb00: ebffff3d     	bl	0x48b7fc <Array2d<rnd::Tile*>::operator()(int, int)> @ imm = #-0x30c
  48bb04: e5903000     	ldr	r3, [r0]
  48bb08: e3530000     	cmp	r3, #0
  48bb0c: 0a00001d     	beq	0x48bb88 <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x130> @ imm = #0x74
  48bb10: e7983009     	ldr	r3, [r8, r9]
  48bb14: e5933000     	ldr	r3, [r3]
  48bb18: e3530002     	cmp	r3, #2
  48bb1c: 03a03000     	moveq	r3, #0
  48bb20: 05833000     	streq	r3, [r3]
  48bb24: 0affffed     	beq	0x48bae0 <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x88> @ imm = #-0x4c
  48bb28: e3530001     	cmp	r3, #1
  48bb2c: 1affffeb     	bne	0x48bae0 <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x88> @ imm = #-0x54
  48bb30: e59dc00c     	ldr	r12, [sp, #0xc]
  48bb34: e59d3018     	ldr	r3, [sp, #0x18]
  48bb38: e59d1010     	ldr	r1, [sp, #0x10]
  48bb3c: e798000c     	ldr	r0, [r8, r12]
  48bb40: e59d2014     	ldr	r2, [sp, #0x14]
  48bb44: e300c12a     	movw	r12, #0x12a
  48bb48: e28000a8     	add	r0, r0, #168
  48bb4c: e58dc000     	str	r12, [sp]
  48bb50: ebfa092b     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x17db54
  48bb54: e5963054     	ldr	r3, [r6, #0x54]
  48bb58: e2844001     	add	r4, r4, #1
  48bb5c: e1530004     	cmp	r3, r4
  48bb60: caffffe2     	bgt	0x48baf0 <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x98> @ imm = #-0x78
  48bb64: e5960058     	ldr	r0, [r6, #0x58]
  48bb68: e59d2008     	ldr	r2, [sp, #0x8]
  48bb6c: e2822001     	add	r2, r2, #1
  48bb70: e1500002     	cmp	r0, r2
  48bb74: e58d2008     	str	r2, [sp, #0x8]
  48bb78: caffffcd     	bgt	0x48bab4 <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x5c> @ imm = #-0xcc
  48bb7c: e3a00001     	mov	r0, #1
  48bb80: e28dd024     	add	sp, sp, #36
  48bb84: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  48bb88: e1a01005     	mov	r1, r5
  48bb8c: e1a00007     	mov	r0, r7
  48bb90: e1a0200a     	mov	r2, r10
  48bb94: ebffff18     	bl	0x48b7fc <Array2d<rnd::Tile*>::operator()(int, int)> @ imm = #-0x3a0
  48bb98: e59d301c     	ldr	r3, [sp, #0x1c]
  48bb9c: e5803000     	str	r3, [r0]
  48bba0: eaffffce     	b	0x48bae0 <rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x88> @ imm = #-0xc8
  48bba4: 20 90 50 00  	.word	0x00509020
  48bba8: 48 29 43 00  	.word	0x00432948
  48bbac: cc 2a 43 00  	.word	0x00432acc
  48bbb0: c8 92 44 00  	.word	0x004492c8
  48bbb4: c0 19 00 00  	.word	0x000019c0
  48bbb8: c0 39 00 00  	.word	0x000039c0
