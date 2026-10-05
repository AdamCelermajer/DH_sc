
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004919b0 <rnd::Tile::Spawn(rnd::Block&, int, int, float, rnd::ListElem&)>:
  4919b0: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  4919b4: e1a06000     	mov	r6, r0
  4919b8: e24dd014     	sub	sp, sp, #20
  4919bc: e1a07001     	mov	r7, r1
  4919c0: e3a00090     	mov	r0, #144
  4919c4: e3a01000     	mov	r1, #0
  4919c8: e59d403c     	ldr	r4, [sp, #0x3c]
  4919cc: e1a0b002     	mov	r11, r2
  4919d0: e1a09003     	mov	r9, r3
  4919d4: ebf9fae5     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x18146c
  4919d8: e5961004     	ldr	r1, [r6, #0x4]
  4919dc: e1a02007     	mov	r2, r7
  4919e0: e1a03006     	mov	r3, r6
  4919e4: e1a05000     	mov	r5, r0
  4919e8: e58d4000     	str	r4, [sp]
  4919ec: ebffff81     	bl	0x4917f8 <rnd::Tile::Tile(rnd::RandomGenerator&, rnd::Block&, rnd::Tile*, rnd::ListElem&)> @ imm = #-0x1fc
  4919f0: e594a000     	ldr	r10, [r4]
  4919f4: e35a0000     	cmp	r10, #0
  4919f8: 0a000024     	beq	0x491a90 <rnd::Tile::Spawn(rnd::Block&, int, int, float, rnd::ListElem&)+0xe0> @ imm = #0x90
  4919fc: e59a601c     	ldr	r6, [r10, #0x1c]
  491a00: e59a8020     	ldr	r8, [r10, #0x20]
  491a04: e1560008     	cmp	r6, r8
  491a08: 0a000020     	beq	0x491a90 <rnd::Tile::Spawn(rnd::Block&, int, int, float, rnd::ListElem&)+0xe0> @ imm = #0x80
  491a0c: e5da7018     	ldrb	r7, [r10, #0x18]
  491a10: ea000002     	b	0x491a20 <rnd::Tile::Spawn(rnd::Block&, int, int, float, rnd::ListElem&)+0x70> @ imm = #0x8
  491a14: e2866050     	add	r6, r6, #80
  491a18: e1560008     	cmp	r6, r8
  491a1c: 0a00001b     	beq	0x491a90 <rnd::Tile::Spawn(rnd::Block&, int, int, float, rnd::ListElem&)+0xe0> @ imm = #0x6c
  491a20: e3570000     	cmp	r7, #0
  491a24: 1afffffa     	bne	0x491a14 <rnd::Tile::Spawn(rnd::Block&, int, int, float, rnd::ListElem&)+0x64> @ imm = #-0x18
  491a28: e5960030     	ldr	r0, [r6, #0x30]
  491a2c: e596202c     	ldr	r2, [r6, #0x2c]
  491a30: e5941030     	ldr	r1, [r4, #0x30]
  491a34: e594302c     	ldr	r3, [r4, #0x2c]
  491a38: e0602002     	rsb	r2, r0, r2
  491a3c: e0613003     	rsb	r3, r1, r3
  491a40: e1520003     	cmp	r2, r3
  491a44: 1afffff2     	bne	0x491a14 <rnd::Tile::Spawn(rnd::Block&, int, int, float, rnd::ListElem&)+0x64> @ imm = #-0x38
  491a48: ebf9f2e4     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0x183470
  491a4c: e3500000     	cmp	r0, #0
  491a50: 1affffef     	bne	0x491a14 <rnd::Tile::Spawn(rnd::Block&, int, int, float, rnd::ListElem&)+0x64> @ imm = #-0x44
  491a54: e5960018     	ldr	r0, [r6, #0x18]
  491a58: e5962014     	ldr	r2, [r6, #0x14]
  491a5c: e5941018     	ldr	r1, [r4, #0x18]
  491a60: e5943014     	ldr	r3, [r4, #0x14]
  491a64: e0602002     	rsb	r2, r0, r2
  491a68: e0613003     	rsb	r3, r1, r3
  491a6c: e1520003     	cmp	r2, r3
  491a70: 1affffe7     	bne	0x491a14 <rnd::Tile::Spawn(rnd::Block&, int, int, float, rnd::ListElem&)+0x64> @ imm = #-0x64
  491a74: ebf9f2d9     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0x18349c
  491a78: e3500000     	cmp	r0, #0
  491a7c: 1affffe4     	bne	0x491a14 <rnd::Tile::Spawn(rnd::Block&, int, int, float, rnd::ListElem&)+0x64> @ imm = #-0x70
  491a80: e28a001c     	add	r0, r10, #28
  491a84: e1a01006     	mov	r1, r6
  491a88: e28d200c     	add	r2, sp, #12
  491a8c: ebfff015     	bl	0x48dae8 <std::vector<rnd::ListElem, std::allocator<rnd::ListElem>>::_M_erase(rnd::ListElem*, std::__false_type const&)> @ imm = #-0x3fac
  491a90: e1a00005     	mov	r0, r5
  491a94: e1a0100b     	mov	r1, r11
  491a98: e1a02009     	mov	r2, r9
  491a9c: e59d3038     	ldr	r3, [sp, #0x38]
  491aa0: ebfffe4c     	bl	0x4913d8 <rnd::Tile::PlaceTile(int, int, float)> @ imm = #-0x6d0
  491aa4: e1a00005     	mov	r0, r5
  491aa8: e28dd014     	add	sp, sp, #20
  491aac: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
