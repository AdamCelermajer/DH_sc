
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004892ec <rnd::RandomGenerator::LoadBlocks()>:
  4892ec: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  4892f0: e59f1200     	ldr	r1, [pc, #0x200]        @ 0x4894f8 <rnd::RandomGenerator::LoadBlocks()+0x20c>
  4892f4: e59f2200     	ldr	r2, [pc, #0x200]        @ 0x4894fc <rnd::RandomGenerator::LoadBlocks()+0x210>
  4892f8: e24dd054     	sub	sp, sp, #84
  4892fc: e08f1001     	add	r1, pc, r1
  489300: e7913002     	ldr	r3, [r1, r2]
  489304: e98d0006     	stmib	sp, {r1, r2}
  489308: e5902158     	ldr	r2, [r0, #0x158]
  48930c: e5901154     	ldr	r1, [r0, #0x154]
  489310: e5933000     	ldr	r3, [r3]
  489314: e28dc034     	add	r12, sp, #52
  489318: e0621001     	rsb	r1, r2, r1
  48931c: e1a04000     	mov	r4, r0
  489320: e2811011     	add	r1, r1, #17
  489324: e1a0000c     	mov	r0, r12
  489328: e58dc000     	str	r12, [sp]
  48932c: e58dc044     	str	r12, [sp, #0x44]
  489330: e58dc048     	str	r12, [sp, #0x48]
  489334: e58d304c     	str	r3, [sp, #0x4c]
  489338: ebfa20cf     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x177cc4
  48933c: e59d3044     	ldr	r3, [sp, #0x44]
  489340: e3a05000     	mov	r5, #0
  489344: e59d0000     	ldr	r0, [sp]
  489348: e5c35000     	strb	r5, [r3]
  48934c: e5941158     	ldr	r1, [r4, #0x158]
  489350: e5942154     	ldr	r2, [r4, #0x154]
  489354: ebfa1d2a     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0x178b58
  489358: e59f11a0     	ldr	r1, [pc, #0x1a0]        @ 0x489500 <rnd::RandomGenerator::LoadBlocks()+0x214>
  48935c: e28dc014     	add	r12, sp, #20
  489360: e28d3030     	add	r3, sp, #48
  489364: e08f1001     	add	r1, pc, r1
  489368: e2812010     	add	r2, r1, #16
  48936c: e59d0000     	ldr	r0, [sp]
  489370: e58dc00c     	str	r12, [sp, #0xc]
  489374: ebfa8504     	bl	0x32a78c <std::string& std::string::_M_appendT<char const*>(char const*, char const*, std::forward_iterator_tag const&)> @ imm = #-0x15ebf0
  489378: e1a00004     	mov	r0, r4
  48937c: e59d1048     	ldr	r1, [sp, #0x48]
  489380: e59d200c     	ldr	r2, [sp, #0xc]
  489384: e58d501c     	str	r5, [sp, #0x1c]
  489388: e58d5014     	str	r5, [sp, #0x14]
  48938c: e58d5018     	str	r5, [sp, #0x18]
  489390: ebfffd5b     	bl	0x488904 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)> @ imm = #-0xa94
  489394: e59d5014     	ldr	r5, [sp, #0x14]
  489398: e59db018     	ldr	r11, [sp, #0x18]
  48939c: e155000b     	cmp	r5, r11
  4893a0: 0284603c     	addeq	r6, r4, #60
  4893a4: 0a00001b     	beq	0x489418 <rnd::RandomGenerator::LoadBlocks()+0x12c> @ imm = #0x6c
  4893a8: e59f7154     	ldr	r7, [pc, #0x154]        @ 0x489504 <rnd::RandomGenerator::LoadBlocks()+0x218>
  4893ac: e284603c     	add	r6, r4, #60
  4893b0: e28da020     	add	r10, sp, #32
  4893b4: e08f7007     	add	r7, pc, r7
  4893b8: e28d8028     	add	r8, sp, #40
  4893bc: e5959014     	ldr	r9, [r5, #0x14]
  4893c0: e1a01007     	mov	r1, r7
  4893c4: e1a00009     	mov	r0, r9
  4893c8: ebfa1601     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x17a7fc
  4893cc: e3500000     	cmp	r0, #0
  4893d0: 0a00000d     	beq	0x48940c <rnd::RandomGenerator::LoadBlocks()+0x120> @ imm = #0x34
  4893d4: e1a02009     	mov	r2, r9
  4893d8: e5940158     	ldr	r0, [r4, #0x158]
  4893dc: e5941140     	ldr	r1, [r4, #0x140]
  4893e0: eb00065b     	bl	0x48ad54 <rnd::MgxBlock::FromFilename(char const*, char const*, char const*)> @ imm = #0x196c
  4893e4: e2503000     	subs	r3, r0, #0
  4893e8: 0a000006     	beq	0x489408 <rnd::RandomGenerator::LoadBlocks()+0x11c> @ imm = #0x18
  4893ec: e593c018     	ldr	r12, [r3, #0x18]
  4893f0: e1a0000a     	mov	r0, r10
  4893f4: e1a01006     	mov	r1, r6
  4893f8: e1a02008     	mov	r2, r8
  4893fc: e58dc028     	str	r12, [sp, #0x28]
  489400: e58d302c     	str	r3, [sp, #0x2c]
  489404: ebfff352     	bl	0x486154 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::insert_unique(std::pair<char const* const, rnd::Block*> const&)> @ imm = #-0x32b8
  489408: e59db018     	ldr	r11, [sp, #0x18]
  48940c: e2855018     	add	r5, r5, #24
  489410: e155000b     	cmp	r5, r11
  489414: 1affffe8     	bne	0x4893bc <rnd::RandomGenerator::LoadBlocks()+0xd0> @ imm = #-0x60
  489418: e5944044     	ldr	r4, [r4, #0x44]
  48941c: e1540006     	cmp	r4, r6
  489420: 0a00000d     	beq	0x48945c <rnd::RandomGenerator::LoadBlocks()+0x170> @ imm = #0x34
  489424: e5940014     	ldr	r0, [r4, #0x14]
  489428: e1a01006     	mov	r1, r6
  48942c: eb0001ae     	bl	0x489aec <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)> @ imm = #0x6b8
  489430: e594200c     	ldr	r2, [r4, #0xc]
  489434: e3520000     	cmp	r2, #0
  489438: 1a000001     	bne	0x489444 <rnd::RandomGenerator::LoadBlocks()+0x158> @ imm = #0x4
  48943c: ea00001d     	b	0x4894b8 <rnd::RandomGenerator::LoadBlocks()+0x1cc> @ imm = #0x74
  489440: e1a02003     	mov	r2, r3
  489444: e5923008     	ldr	r3, [r2, #0x8]
  489448: e3530000     	cmp	r3, #0
  48944c: 1afffffb     	bne	0x489440 <rnd::RandomGenerator::LoadBlocks()+0x154> @ imm = #-0x14
  489450: e1a04002     	mov	r4, r2
  489454: e1540006     	cmp	r4, r6
  489458: 1afffff1     	bne	0x489424 <rnd::RandomGenerator::LoadBlocks()+0x138> @ imm = #-0x3c
  48945c: e59d000c     	ldr	r0, [sp, #0xc]
  489460: ebfa2ab2     	bl	0x313f30 <std::vector<std::string, std::allocator<std::string>>::~vector()> @ imm = #-0x175538
  489464: e59d0048     	ldr	r0, [sp, #0x48]
  489468: e59d1000     	ldr	r1, [sp]
  48946c: e1500001     	cmp	r0, r1
  489470: 0a000006     	beq	0x489490 <rnd::RandomGenerator::LoadBlocks()+0x1a4> @ imm = #0x18
  489474: e3500000     	cmp	r0, #0
  489478: 0a000004     	beq	0x489490 <rnd::RandomGenerator::LoadBlocks()+0x1a4> @ imm = #0x10
  48947c: e59d1034     	ldr	r1, [sp, #0x34]
  489480: e0601001     	rsb	r1, r0, r1
  489484: e3510080     	cmp	r1, #128
  489488: 8a000017     	bhi	0x4894ec <rnd::RandomGenerator::LoadBlocks()+0x200> @ imm = #0x5c
  48948c: eb09fe9b     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27fa6c
  489490: e59d2008     	ldr	r2, [sp, #0x8]
  489494: e59dc004     	ldr	r12, [sp, #0x4]
  489498: e3a00001     	mov	r0, #1
  48949c: e79c3002     	ldr	r3, [r12, r2]
  4894a0: e59d204c     	ldr	r2, [sp, #0x4c]
  4894a4: e5933000     	ldr	r3, [r3]
  4894a8: e1520003     	cmp	r2, r3
  4894ac: 1a000010     	bne	0x4894f4 <rnd::RandomGenerator::LoadBlocks()+0x208> @ imm = #0x40
  4894b0: e28dd054     	add	sp, sp, #84
  4894b4: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  4894b8: e5943004     	ldr	r3, [r4, #0x4]
  4894bc: e593100c     	ldr	r1, [r3, #0xc]
  4894c0: e1540001     	cmp	r4, r1
  4894c4: 1a000005     	bne	0x4894e0 <rnd::RandomGenerator::LoadBlocks()+0x1f4> @ imm = #0x14
  4894c8: e1a04003     	mov	r4, r3
  4894cc: e5933004     	ldr	r3, [r3, #0x4]
  4894d0: e593200c     	ldr	r2, [r3, #0xc]
  4894d4: e1520004     	cmp	r2, r4
  4894d8: 0afffffa     	beq	0x4894c8 <rnd::RandomGenerator::LoadBlocks()+0x1dc> @ imm = #-0x18
  4894dc: e594200c     	ldr	r2, [r4, #0xc]
  4894e0: e1520003     	cmp	r2, r3
  4894e4: 11a04003     	movne	r4, r3
  4894e8: eaffffcb     	b	0x48941c <rnd::RandomGenerator::LoadBlocks()+0x130> @ imm = #-0xd4
  4894ec: ebfa1bd3     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1790b4
  4894f0: eaffffe6     	b	0x489490 <rnd::RandomGenerator::LoadBlocks()+0x1a4> @ imm = #-0x68
  4894f4: ebfa1385     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x17b1ec
  4894f8: 94 b7 50 00  	.word	0x0050b794
  4894fc: ac 40 00 00  	.word	0x000040ac
  489500: 0c b9 44 00  	.word	0x0044b90c
  489504: 74 d4 43 00  	.word	0x0043d474
