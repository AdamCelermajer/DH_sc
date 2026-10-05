
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048644c <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)>:
  48644c: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  486450: e5915004     	ldr	r5, [r1, #0x4]
  486454: e24dd014     	sub	sp, sp, #20
  486458: e1a08001     	mov	r8, r1
  48645c: e3550000     	cmp	r5, #0
  486460: e1a04000     	mov	r4, r0
  486464: e1a07002     	mov	r7, r2
  486468: 01a05001     	moveq	r5, r1
  48646c: 0a00001a     	beq	0x4864dc <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0x90> @ imm = #0x68
  486470: e2816014     	add	r6, r1, #20
  486474: ea000000     	b	0x48647c <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0x30> @ imm = #0x0
  486478: e1a05002     	mov	r5, r2
  48647c: e5952010     	ldr	r2, [r5, #0x10]
  486480: e1a00006     	mov	r0, r6
  486484: e5971000     	ldr	r1, [r7]
  486488: ebfa4557     	bl	0x3179ec <lstr::operator()(char const*, char const*) const> @ imm = #-0x16eaa4
  48648c: e3500000     	cmp	r0, #0
  486490: 15952008     	ldrne	r2, [r5, #0x8]
  486494: 0595200c     	ldreq	r2, [r5, #0xc]
  486498: e1a03005     	mov	r3, r5
  48649c: e3520000     	cmp	r2, #0
  4864a0: 1afffff4     	bne	0x486478 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0x2c> @ imm = #-0x30
  4864a4: e3500000     	cmp	r0, #0
  4864a8: 01a0a005     	moveq	r10, r5
  4864ac: 1a00000a     	bne	0x4864dc <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0x90> @ imm = #0x28
  4864b0: e1a00006     	mov	r0, r6
  4864b4: e5931010     	ldr	r1, [r3, #0x10]
  4864b8: e5972000     	ldr	r2, [r7]
  4864bc: ebfa454a     	bl	0x3179ec <lstr::operator()(char const*, char const*) const> @ imm = #-0x16ead8
  4864c0: e3500000     	cmp	r0, #0
  4864c4: 0584a000     	streq	r10, [r4]
  4864c8: 05c40004     	strbeq	r0, [r4, #0x4]
  4864cc: 1a00001e     	bne	0x48654c <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0x100> @ imm = #0x78
  4864d0: e1a00004     	mov	r0, r4
  4864d4: e28dd014     	add	sp, sp, #20
  4864d8: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  4864dc: e5983008     	ldr	r3, [r8, #0x8]
  4864e0: e1550003     	cmp	r5, r3
  4864e4: 0a000036     	beq	0x4865c4 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0x178> @ imm = #0xd8
  4864e8: e5d53000     	ldrb	r3, [r5]
  4864ec: e3530000     	cmp	r3, #0
  4864f0: 1a000003     	bne	0x486504 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0xb8> @ imm = #0xc
  4864f4: e5953004     	ldr	r3, [r5, #0x4]
  4864f8: e5933004     	ldr	r3, [r3, #0x4]
  4864fc: e1550003     	cmp	r5, r3
  486500: 0a00002b     	beq	0x4865b4 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0x168> @ imm = #0xac
  486504: e5953008     	ldr	r3, [r5, #0x8]
  486508: e3530000     	cmp	r3, #0
  48650c: 1a000001     	bne	0x486518 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0xcc> @ imm = #0x4
  486510: ea000019     	b	0x48657c <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0x130> @ imm = #0x64
  486514: e1a03002     	mov	r3, r2
  486518: e593200c     	ldr	r2, [r3, #0xc]
  48651c: e3520000     	cmp	r2, #0
  486520: 1afffffb     	bne	0x486514 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0xc8> @ imm = #-0x14
  486524: e2886014     	add	r6, r8, #20
  486528: e1a00006     	mov	r0, r6
  48652c: e5931010     	ldr	r1, [r3, #0x10]
  486530: e5972000     	ldr	r2, [r7]
  486534: e1a0a003     	mov	r10, r3
  486538: ebfa452b     	bl	0x3179ec <lstr::operator()(char const*, char const*) const> @ imm = #-0x16eb54
  48653c: e3500000     	cmp	r0, #0
  486540: 0584a000     	streq	r10, [r4]
  486544: 05c40004     	strbeq	r0, [r4, #0x4]
  486548: 0affffe0     	beq	0x4864d0 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0x84> @ imm = #-0x80
  48654c: e1a02005     	mov	r2, r5
  486550: e1a03007     	mov	r3, r7
  486554: e3a0c000     	mov	r12, #0
  486558: e1a01008     	mov	r1, r8
  48655c: e28d0008     	add	r0, sp, #8
  486560: e58dc000     	str	r12, [sp]
  486564: ebffff6b     	bl	0x486318 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::_M_insert(std::priv::_Rb_tree_node_base*, std::pair<char const* const, rnd::ListRule*> const&, std::priv::_Rb_tree_node_base*, std::priv::_Rb_tree_node_base*) (.clone.13)> @ imm = #-0x254
  486568: e59d3008     	ldr	r3, [sp, #0x8]
  48656c: e3a02001     	mov	r2, #1
  486570: e5c42004     	strb	r2, [r4, #0x4]
  486574: e5843000     	str	r3, [r4]
  486578: eaffffd4     	b	0x4864d0 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0x84> @ imm = #-0xb0
  48657c: e5952004     	ldr	r2, [r5, #0x4]
  486580: e5923008     	ldr	r3, [r2, #0x8]
  486584: e1550003     	cmp	r5, r3
  486588: 11a03002     	movne	r3, r2
  48658c: 11a0a003     	movne	r10, r3
  486590: 12886014     	addne	r6, r8, #20
  486594: 0a000001     	beq	0x4865a0 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0x154> @ imm = #0x4
  486598: eaffffc4     	b	0x4864b0 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0x64> @ imm = #-0xf0
  48659c: e1a02003     	mov	r2, r3
  4865a0: e5923004     	ldr	r3, [r2, #0x4]
  4865a4: e5931008     	ldr	r1, [r3, #0x8]
  4865a8: e1510002     	cmp	r1, r2
  4865ac: 0afffffa     	beq	0x48659c <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0x150> @ imm = #-0x18
  4865b0: eaffffdb     	b	0x486524 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0xd8> @ imm = #-0x94
  4865b4: e595300c     	ldr	r3, [r5, #0xc]
  4865b8: e2886014     	add	r6, r8, #20
  4865bc: e1a0a003     	mov	r10, r3
  4865c0: eaffffba     	b	0x4864b0 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0x64> @ imm = #-0x118
  4865c4: e1a02005     	mov	r2, r5
  4865c8: e1a03007     	mov	r3, r7
  4865cc: e1a01008     	mov	r1, r8
  4865d0: e28d000c     	add	r0, sp, #12
  4865d4: e58d5000     	str	r5, [sp]
  4865d8: ebffff4e     	bl	0x486318 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::_M_insert(std::priv::_Rb_tree_node_base*, std::pair<char const* const, rnd::ListRule*> const&, std::priv::_Rb_tree_node_base*, std::priv::_Rb_tree_node_base*) (.clone.13)> @ imm = #-0x2c8
  4865dc: e59d300c     	ldr	r3, [sp, #0xc]
  4865e0: e3a02001     	mov	r2, #1
  4865e4: e5c42004     	strb	r2, [r4, #0x4]
  4865e8: e5843000     	str	r3, [r4]
  4865ec: eaffffb7     	b	0x4864d0 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::insert_unique(std::pair<char const* const, rnd::ListRule*> const&)+0x84> @ imm = #-0x124
