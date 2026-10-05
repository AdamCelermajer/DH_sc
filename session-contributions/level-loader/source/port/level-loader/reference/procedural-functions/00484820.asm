
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00484820 <rnd::RandomGenerator::PrintMap()>:
  484820: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  484824: e24dd030     	sub	sp, sp, #48
  484828: e2809024     	add	r9, r0, #36
  48482c: e2805014     	add	r5, r0, #20
  484830: e1a0a00d     	mov	r10, sp
  484834: e895000f     	ldm	r5, {r0, r1, r2, r3}
  484838: e88a000f     	stm	r10, {r0, r1, r2, r3}
  48483c: e1a00009     	mov	r0, r9
  484840: e1a0100d     	mov	r1, sp
  484844: ebfffd1c     	bl	0x483cbc <std::priv::_Deque_iterator_base<std::deque<rnd::Tile*, std::allocator<rnd::Tile*>>>::_M_subtract(std::priv::_Deque_iterator_base<std::deque<rnd::Tile*, std::allocator<rnd::Tile*>>> const&) const> @ imm = #-0xb90
  484848: e3a08000     	mov	r8, #0
  48484c: e1580000     	cmp	r8, r0
  484850: e28d4020     	add	r4, sp, #32
  484854: e28d6010     	add	r6, sp, #16
  484858: aa000022     	bge	0x4848e8 <rnd::RandomGenerator::PrintMap()+0xc8> @ imm = #0x88
  48485c: e3a07000     	mov	r7, #0
  484860: ea00000b     	b	0x484894 <rnd::RandomGenerator::PrintMap()+0x74> @ imm = #0x2c
  484864: e895000f     	ldm	r5, {r0, r1, r2, r3}
  484868: e884000f     	stm	r4, {r0, r1, r2, r3}
  48486c: e1a00004     	mov	r0, r4
  484870: e1a01008     	mov	r1, r8
  484874: ebfffd76     	bl	0x483e54 <std::priv::_Deque_iterator_base<std::deque<rnd::Tile*, std::allocator<rnd::Tile*>>>::_M_advance(int)> @ imm = #-0xa28
  484878: e59d3020     	ldr	r3, [sp, #0x20]
  48487c: e893000f     	ldm	r3, {r0, r1, r2, r3}
  484880: e884000f     	stm	r4, {r0, r1, r2, r3}
  484884: e1a01007     	mov	r1, r7
  484888: e1a00004     	mov	r0, r4
  48488c: ebfffd38     	bl	0x483d74 <std::priv::_Deque_iterator_base<rnd::Tile*>::_M_advance(int)> @ imm = #-0xb20
  484890: e2877001     	add	r7, r7, #1
  484894: e895000f     	ldm	r5, {r0, r1, r2, r3}
  484898: e884000f     	stm	r4, {r0, r1, r2, r3}
  48489c: e1a00004     	mov	r0, r4
  4848a0: e3a01000     	mov	r1, #0
  4848a4: ebfffd6a     	bl	0x483e54 <std::priv::_Deque_iterator_base<std::deque<rnd::Tile*, std::allocator<rnd::Tile*>>>::_M_advance(int)> @ imm = #-0xa58
  4848a8: e59dc020     	ldr	r12, [sp, #0x20]
  4848ac: e89c000f     	ldm	r12, {r0, r1, r2, r3}
  4848b0: e886000f     	stm	r6, {r0, r1, r2, r3}
  4848b4: e28c0010     	add	r0, r12, #16
  4848b8: e1a01006     	mov	r1, r6
  4848bc: ebfffd1b     	bl	0x483d30 <std::priv::_Deque_iterator_base<rnd::Tile*>::_M_subtract(std::priv::_Deque_iterator_base<rnd::Tile*> const&) const> @ imm = #-0xb94
  4848c0: e1570000     	cmp	r7, r0
  4848c4: baffffe6     	blt	0x484864 <rnd::RandomGenerator::PrintMap()+0x44> @ imm = #-0x68
  4848c8: e895000f     	ldm	r5, {r0, r1, r2, r3}
  4848cc: e88a000f     	stm	r10, {r0, r1, r2, r3}
  4848d0: e1a00009     	mov	r0, r9
  4848d4: e1a0100d     	mov	r1, sp
  4848d8: ebfffcf7     	bl	0x483cbc <std::priv::_Deque_iterator_base<std::deque<rnd::Tile*, std::allocator<rnd::Tile*>>>::_M_subtract(std::priv::_Deque_iterator_base<std::deque<rnd::Tile*, std::allocator<rnd::Tile*>>> const&) const> @ imm = #-0xc24
  4848dc: e2888001     	add	r8, r8, #1
  4848e0: e1580000     	cmp	r8, r0
  4848e4: baffffdc     	blt	0x48485c <rnd::RandomGenerator::PrintMap()+0x3c> @ imm = #-0x90
  4848e8: e28dd030     	add	sp, sp, #48
  4848ec: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
