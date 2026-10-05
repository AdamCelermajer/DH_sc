
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048b8f0 <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)>:
  48b8f0: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  48b8f4: e1a06000     	mov	r6, r0
  48b8f8: e5900058     	ldr	r0, [r0, #0x58]
  48b8fc: e59f813c     	ldr	r8, [pc, #0x13c]        @ 0x48ba40 <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x150>
  48b900: e24dd024     	sub	sp, sp, #36
  48b904: e3500000     	cmp	r0, #0
  48b908: e08f8008     	add	r8, pc, r8
  48b90c: e1a07001     	mov	r7, r1
  48b910: e58d2008     	str	r2, [sp, #0x8]
  48b914: e1a0b003     	mov	r11, r3
  48b918: da00003e     	ble	0x48ba18 <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x128> @ imm = #0xf8
  48b91c: e59f3120     	ldr	r3, [pc, #0x120]        @ 0x48ba44 <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x154>
  48b920: e3a02000     	mov	r2, #0
  48b924: e58d200c     	str	r2, [sp, #0xc]
  48b928: e08f3003     	add	r3, pc, r3
  48b92c: e58d3014     	str	r3, [sp, #0x14]
  48b930: e59f3110     	ldr	r3, [pc, #0x110]        @ 0x48ba48 <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x158>
  48b934: e08f3003     	add	r3, pc, r3
  48b938: e58d3018     	str	r3, [sp, #0x18]
  48b93c: e59f3108     	ldr	r3, [pc, #0x108]        @ 0x48ba4c <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x15c>
  48b940: e08f3003     	add	r3, pc, r3
  48b944: e58d301c     	str	r3, [sp, #0x1c]
  48b948: e5963054     	ldr	r3, [r6, #0x54]
  48b94c: e3530000     	cmp	r3, #0
  48b950: da00002b     	ble	0x48ba04 <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x114> @ imm = #0xac
  48b954: e59d300c     	ldr	r3, [sp, #0xc]
  48b958: e59dc048     	ldr	r12, [sp, #0x48]
  48b95c: e59f20ec     	ldr	r2, [pc, #0xec]         @ 0x48ba50 <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x160>
  48b960: e3a04000     	mov	r4, #0
  48b964: e083500c     	add	r5, r3, r12
  48b968: e59f90e4     	ldr	r9, [pc, #0xe4]         @ 0x48ba54 <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x164>
  48b96c: e58d2010     	str	r2, [sp, #0x10]
  48b970: e1a0a005     	mov	r10, r5
  48b974: ea000003     	b	0x48b988 <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x98> @ imm = #0xc
  48b978: e5963054     	ldr	r3, [r6, #0x54]
  48b97c: e2844001     	add	r4, r4, #1
  48b980: e1530004     	cmp	r3, r4
  48b984: da00001d     	ble	0x48ba00 <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x110> @ imm = #0x74
  48b988: e084500b     	add	r5, r4, r11
  48b98c: e1a01005     	mov	r1, r5
  48b990: e1a0200a     	mov	r2, r10
  48b994: e1a00007     	mov	r0, r7
  48b998: ebffff97     	bl	0x48b7fc <Array2d<rnd::Tile*>::operator()(int, int)> @ imm = #-0x1a4
  48b99c: e59dc008     	ldr	r12, [sp, #0x8]
  48b9a0: e5903000     	ldr	r3, [r0]
  48b9a4: e153000c     	cmp	r3, r12
  48b9a8: 0a00001d     	beq	0x48ba24 <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x134> @ imm = #0x74
  48b9ac: e7983009     	ldr	r3, [r8, r9]
  48b9b0: e5933000     	ldr	r3, [r3]
  48b9b4: e3530002     	cmp	r3, #2
  48b9b8: 03a03000     	moveq	r3, #0
  48b9bc: 05833000     	streq	r3, [r3]
  48b9c0: 0affffec     	beq	0x48b978 <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x88> @ imm = #-0x50
  48b9c4: e3530001     	cmp	r3, #1
  48b9c8: 1affffea     	bne	0x48b978 <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x88> @ imm = #-0x58
  48b9cc: e59d2010     	ldr	r2, [sp, #0x10]
  48b9d0: e59d301c     	ldr	r3, [sp, #0x1c]
  48b9d4: e3a0cd05     	mov	r12, #320
  48b9d8: e7980002     	ldr	r0, [r8, r2]
  48b9dc: e59d1014     	ldr	r1, [sp, #0x14]
  48b9e0: e59d2018     	ldr	r2, [sp, #0x18]
  48b9e4: e28000a8     	add	r0, r0, #168
  48b9e8: e58dc000     	str	r12, [sp]
  48b9ec: ebfa0984     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x17d9f0
  48b9f0: e5963054     	ldr	r3, [r6, #0x54]
  48b9f4: e2844001     	add	r4, r4, #1
  48b9f8: e1530004     	cmp	r3, r4
  48b9fc: caffffe1     	bgt	0x48b988 <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x98> @ imm = #-0x7c
  48ba00: e5960058     	ldr	r0, [r6, #0x58]
  48ba04: e59d200c     	ldr	r2, [sp, #0xc]
  48ba08: e2822001     	add	r2, r2, #1
  48ba0c: e1500002     	cmp	r0, r2
  48ba10: e58d200c     	str	r2, [sp, #0xc]
  48ba14: caffffcc     	bgt	0x48b94c <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x5c> @ imm = #-0xd0
  48ba18: e3a00001     	mov	r0, #1
  48ba1c: e28dd024     	add	sp, sp, #36
  48ba20: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  48ba24: e1a01005     	mov	r1, r5
  48ba28: e1a00007     	mov	r0, r7
  48ba2c: e1a0200a     	mov	r2, r10
  48ba30: ebffff71     	bl	0x48b7fc <Array2d<rnd::Tile*>::operator()(int, int)> @ imm = #-0x23c
  48ba34: e3a03000     	mov	r3, #0
  48ba38: e5803000     	str	r3, [r0]
  48ba3c: eaffffcd     	b	0x48b978 <rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&, rnd::Tile*, int, int)+0x88> @ imm = #-0xcc
  48ba40: 88 91 50 00  	.word	0x00509188
  48ba44: b0 2a 43 00  	.word	0x00432ab0
  48ba48: 34 2c 43 00  	.word	0x00432c34
  48ba4c: 30 94 44 00  	.word	0x00449430
  48ba50: c0 19 00 00  	.word	0x000019c0
  48ba54: c0 39 00 00  	.word	0x000039c0
