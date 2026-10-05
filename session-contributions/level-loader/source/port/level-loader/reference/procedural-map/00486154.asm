
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00486154 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)>:
  486154: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  486158: e5915004     	ldr	r5, [r1, #0x4]
  48615c: e24dd014     	sub	sp, sp, #20
  486160: e1a08001     	mov	r8, r1
  486164: e3550000     	cmp	r5, #0
  486168: e1a04000     	mov	r4, r0
  48616c: e1a07002     	mov	r7, r2
  486170: 01a05001     	moveq	r5, r1
  486174: 0a00001a     	beq	0x4861e4 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0x90> @ imm = #0x68
  486178: e2816014     	add	r6, r1, #20
  48617c: ea000000     	b	0x486184 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0x30> @ imm = #0x0
  486180: e1a05002     	mov	r5, r2
  486184: e5952010     	ldr	r2, [r5, #0x10]
  486188: e1a00006     	mov	r0, r6
  48618c: e5971000     	ldr	r1, [r7]
  486190: ebfa4615     	bl	0x3179ec <lstr::operator()(char const*, char const*) const> @ imm = #-0x16e7ac
  486194: e3500000     	cmp	r0, #0
  486198: 15952008     	ldrne	r2, [r5, #0x8]
  48619c: 0595200c     	ldreq	r2, [r5, #0xc]
  4861a0: e1a03005     	mov	r3, r5
  4861a4: e3520000     	cmp	r2, #0
  4861a8: 1afffff4     	bne	0x486180 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0x2c> @ imm = #-0x30
  4861ac: e3500000     	cmp	r0, #0
  4861b0: 01a0a005     	moveq	r10, r5
  4861b4: 1a00000a     	bne	0x4861e4 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0x90> @ imm = #0x28
  4861b8: e1a00006     	mov	r0, r6
  4861bc: e5931010     	ldr	r1, [r3, #0x10]
  4861c0: e5972000     	ldr	r2, [r7]
  4861c4: ebfa4608     	bl	0x3179ec <lstr::operator()(char const*, char const*) const> @ imm = #-0x16e7e0
  4861c8: e3500000     	cmp	r0, #0
  4861cc: 0584a000     	streq	r10, [r4]
  4861d0: 05c40004     	strbeq	r0, [r4, #0x4]
  4861d4: 1a00001e     	bne	0x486254 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0x100> @ imm = #0x78
  4861d8: e1a00004     	mov	r0, r4
  4861dc: e28dd014     	add	sp, sp, #20
  4861e0: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  4861e4: e5983008     	ldr	r3, [r8, #0x8]
  4861e8: e1550003     	cmp	r5, r3
  4861ec: 0a000036     	beq	0x4862cc <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0x178> @ imm = #0xd8
  4861f0: e5d53000     	ldrb	r3, [r5]
  4861f4: e3530000     	cmp	r3, #0
  4861f8: 1a000003     	bne	0x48620c <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0xb8> @ imm = #0xc
  4861fc: e5953004     	ldr	r3, [r5, #0x4]
  486200: e5933004     	ldr	r3, [r3, #0x4]
  486204: e1550003     	cmp	r5, r3
  486208: 0a00002b     	beq	0x4862bc <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0x168> @ imm = #0xac
  48620c: e5953008     	ldr	r3, [r5, #0x8]
  486210: e3530000     	cmp	r3, #0
  486214: 1a000001     	bne	0x486220 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0xcc> @ imm = #0x4
  486218: ea000019     	b	0x486284 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0x130> @ imm = #0x64
  48621c: e1a03002     	mov	r3, r2
  486220: e593200c     	ldr	r2, [r3, #0xc]
  486224: e3520000     	cmp	r2, #0
  486228: 1afffffb     	bne	0x48621c <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0xc8> @ imm = #-0x14
  48622c: e2886014     	add	r6, r8, #20
  486230: e1a00006     	mov	r0, r6
  486234: e5931010     	ldr	r1, [r3, #0x10]
  486238: e5972000     	ldr	r2, [r7]
  48623c: e1a0a003     	mov	r10, r3
  486240: ebfa45e9     	bl	0x3179ec <lstr::operator()(char const*, char const*) const> @ imm = #-0x16e85c
  486244: e3500000     	cmp	r0, #0
  486248: 0584a000     	streq	r10, [r4]
  48624c: 05c40004     	strbeq	r0, [r4, #0x4]
  486250: 0affffe0     	beq	0x4861d8 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0x84> @ imm = #-0x80
  486254: e1a02005     	mov	r2, r5
  486258: e1a03007     	mov	r3, r7
  48625c: e3a0c000     	mov	r12, #0
  486260: e1a01008     	mov	r1, r8
  486264: e28d0008     	add	r0, sp, #8
  486268: e58dc000     	str	r12, [sp]
  48626c: ebffff6b     	bl	0x486020 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::_M_insert(std::priv::_Rb_tree_node_base*, std::pair<char const* const, rnd::Block*> const&, std::priv::_Rb_tree_node_base*, std::priv::_Rb_tree_node_base*) (.clone.12)> @ imm = #-0x254
  486270: e59d3008     	ldr	r3, [sp, #0x8]
  486274: e3a02001     	mov	r2, #1
  486278: e5c42004     	strb	r2, [r4, #0x4]
  48627c: e5843000     	str	r3, [r4]
  486280: eaffffd4     	b	0x4861d8 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0x84> @ imm = #-0xb0
  486284: e5952004     	ldr	r2, [r5, #0x4]
  486288: e5923008     	ldr	r3, [r2, #0x8]
  48628c: e1550003     	cmp	r5, r3
  486290: 11a03002     	movne	r3, r2
  486294: 11a0a003     	movne	r10, r3
  486298: 12886014     	addne	r6, r8, #20
  48629c: 0a000001     	beq	0x4862a8 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0x154> @ imm = #0x4
  4862a0: eaffffc4     	b	0x4861b8 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0x64> @ imm = #-0xf0
  4862a4: e1a02003     	mov	r2, r3
  4862a8: e5923004     	ldr	r3, [r2, #0x4]
  4862ac: e5931008     	ldr	r1, [r3, #0x8]
  4862b0: e1510002     	cmp	r1, r2
  4862b4: 0afffffa     	beq	0x4862a4 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0x150> @ imm = #-0x18
  4862b8: eaffffdb     	b	0x48622c <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0xd8> @ imm = #-0x94
  4862bc: e595300c     	ldr	r3, [r5, #0xc]
  4862c0: e2886014     	add	r6, r8, #20
  4862c4: e1a0a003     	mov	r10, r3
  4862c8: eaffffba     	b	0x4861b8 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0x64> @ imm = #-0x118
  4862cc: e1a02005     	mov	r2, r5
  4862d0: e1a03007     	mov	r3, r7
  4862d4: e1a01008     	mov	r1, r8
  4862d8: e28d000c     	add	r0, sp, #12
  4862dc: e58d5000     	str	r5, [sp]
  4862e0: ebffff4e     	bl	0x486020 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::_M_insert(std::priv::_Rb_tree_node_base*, std::pair<char const* const, rnd::Block*> const&, std::priv::_Rb_tree_node_base*, std::priv::_Rb_tree_node_base*) (.clone.12)> @ imm = #-0x2c8
  4862e4: e59d300c     	ldr	r3, [sp, #0xc]
  4862e8: e3a02001     	mov	r2, #1
  4862ec: e5c42004     	strb	r2, [r4, #0x4]
  4862f0: e5843000     	str	r3, [r4]
  4862f4: eaffffb7     	b	0x4861d8 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)+0x84> @ imm = #-0x124
