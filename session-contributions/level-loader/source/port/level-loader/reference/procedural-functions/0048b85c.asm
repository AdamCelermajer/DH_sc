
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048b85c <rnd::MgxBlock::FitsInMap(Array2d<rnd::Tile*>&, int, int)>:
  48b85c: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  48b860: e1a04000     	mov	r4, r0
  48b864: e5900054     	ldr	r0, [r0, #0x54]
  48b868: e1a06002     	mov	r6, r2
  48b86c: e1a05001     	mov	r5, r1
  48b870: e0822000     	add	r2, r2, r0
  48b874: e1560002     	cmp	r6, r2
  48b878: e1a08003     	mov	r8, r3
  48b87c: aa000019     	bge	0x48b8e8 <rnd::MgxBlock::FitsInMap(Array2d<rnd::Tile*>&, int, int)+0x8c> @ imm = #0x64
  48b880: e5943058     	ldr	r3, [r4, #0x58]
  48b884: e1a0a006     	mov	r10, r6
  48b888: e0882003     	add	r2, r8, r3
  48b88c: e1580002     	cmp	r8, r2
  48b890: b1a07008     	movlt	r7, r8
  48b894: ba000004     	blt	0x48b8ac <rnd::MgxBlock::FitsInMap(Array2d<rnd::Tile*>&, int, int)+0x50> @ imm = #0x10
  48b898: ea00000e     	b	0x48b8d8 <rnd::MgxBlock::FitsInMap(Array2d<rnd::Tile*>&, int, int)+0x7c> @ imm = #0x38
  48b89c: e5943058     	ldr	r3, [r4, #0x58]
  48b8a0: e0882003     	add	r2, r8, r3
  48b8a4: e1520007     	cmp	r2, r7
  48b8a8: da000009     	ble	0x48b8d4 <rnd::MgxBlock::FitsInMap(Array2d<rnd::Tile*>&, int, int)+0x78> @ imm = #0x24
  48b8ac: e1a02007     	mov	r2, r7
  48b8b0: e1a00005     	mov	r0, r5
  48b8b4: e1a0100a     	mov	r1, r10
  48b8b8: ebffffcf     	bl	0x48b7fc <Array2d<rnd::Tile*>::operator()(int, int)> @ imm = #-0xc4
  48b8bc: e5903000     	ldr	r3, [r0]
  48b8c0: e2877001     	add	r7, r7, #1
  48b8c4: e3530000     	cmp	r3, #0
  48b8c8: 0afffff3     	beq	0x48b89c <rnd::MgxBlock::FitsInMap(Array2d<rnd::Tile*>&, int, int)+0x40> @ imm = #-0x34
  48b8cc: e3a00000     	mov	r0, #0
  48b8d0: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  48b8d4: e5940054     	ldr	r0, [r4, #0x54]
  48b8d8: e28aa001     	add	r10, r10, #1
  48b8dc: e0862000     	add	r2, r6, r0
  48b8e0: e152000a     	cmp	r2, r10
  48b8e4: caffffe7     	bgt	0x48b888 <rnd::MgxBlock::FitsInMap(Array2d<rnd::Tile*>&, int, int)+0x2c> @ imm = #-0x64
  48b8e8: e3a00001     	mov	r0, #1
  48b8ec: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
