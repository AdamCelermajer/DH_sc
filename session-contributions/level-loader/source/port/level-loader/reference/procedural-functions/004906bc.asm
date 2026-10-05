
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004906bc <rnd::RoomPool::Find(std::string)>:
  4906bc: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  4906c0: e59f4098     	ldr	r4, [pc, #0x98]         @ 0x490760 <rnd::RoomPool::Find(std::string)+0xa4>
  4906c4: e59f5098     	ldr	r5, [pc, #0x98]         @ 0x490764 <rnd::RoomPool::Find(std::string)+0xa8>
  4906c8: e24dd020     	sub	sp, sp, #32
  4906cc: e08f4004     	add	r4, pc, r4
  4906d0: e7943005     	ldr	r3, [r4, r5]
  4906d4: e5908010     	ldr	r8, [r0, #0x10]
  4906d8: e28d6004     	add	r6, sp, #4
  4906dc: e5933000     	ldr	r3, [r3]
  4906e0: e1a07000     	mov	r7, r0
  4906e4: e1a00006     	mov	r0, r6
  4906e8: e58d301c     	str	r3, [sp, #0x1c]
  4906ec: ebfa6c89     	bl	0x32b918 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&)> @ imm = #-0x164ddc
  4906f0: e59d1018     	ldr	r1, [sp, #0x18]
  4906f4: e1a00008     	mov	r0, r8
  4906f8: ebffcd05     	bl	0x483b14 <rnd::RandomGenerator::Hash(unsigned char*)> @ imm = #-0xcbec
  4906fc: e1a01000     	mov	r1, r0
  490700: e1a00007     	mov	r0, r7
  490704: ebffed45     	bl	0x48bc20 <rnd::RoomPool::Find(unsigned long)> @ imm = #-0x4aec
  490708: e1a07000     	mov	r7, r0
  49070c: e59d0018     	ldr	r0, [sp, #0x18]
  490710: e1500006     	cmp	r0, r6
  490714: 0a000006     	beq	0x490734 <rnd::RoomPool::Find(std::string)+0x78> @ imm = #0x18
  490718: e3500000     	cmp	r0, #0
  49071c: 0a000004     	beq	0x490734 <rnd::RoomPool::Find(std::string)+0x78> @ imm = #0x10
  490720: e59d1004     	ldr	r1, [sp, #0x4]
  490724: e0601001     	rsb	r1, r0, r1
  490728: e3510080     	cmp	r1, #128
  49072c: 8a000008     	bhi	0x490754 <rnd::RoomPool::Find(std::string)+0x98> @ imm = #0x20
  490730: eb09e1f2     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x2787c8
  490734: e7943005     	ldr	r3, [r4, r5]
  490738: e59d201c     	ldr	r2, [sp, #0x1c]
  49073c: e1a00007     	mov	r0, r7
  490740: e5933000     	ldr	r3, [r3]
  490744: e1520003     	cmp	r2, r3
  490748: 1a000003     	bne	0x49075c <rnd::RoomPool::Find(std::string)+0xa0> @ imm = #0xc
  49074c: e28dd020     	add	sp, sp, #32
  490750: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  490754: ebf9ff39     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x18031c
  490758: eafffff5     	b	0x490734 <rnd::RoomPool::Find(std::string)+0x78> @ imm = #-0x2c
  49075c: ebf9f6eb     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x182454
  490760: c4 43 50 00  	.word	0x005043c4
  490764: ac 40 00 00  	.word	0x000040ac
