
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00491518 <rnd::Tile::SetModuleMVXProperties(Module*)>:
  491518: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  49151c: e59f429c     	ldr	r4, [pc, #0x29c]        @ 0x4917c0 <rnd::Tile::SetModuleMVXProperties(Module*)+0x2a8>
  491520: e59f229c     	ldr	r2, [pc, #0x29c]        @ 0x4917c4 <rnd::Tile::SetModuleMVXProperties(Module*)+0x2ac>
  491524: e24dd0c4     	sub	sp, sp, #196
  491528: e08f4004     	add	r4, pc, r4
  49152c: e7943002     	ldr	r3, [r4, r2]
  491530: e58d2004     	str	r2, [sp, #0x4]
  491534: e1a05000     	mov	r5, r0
  491538: e59f2288     	ldr	r2, [pc, #0x288]        @ 0x4917c8 <rnd::Tile::SetModuleMVXProperties(Module*)+0x2b0>
  49153c: e5900030     	ldr	r0, [r0, #0x30]
  491540: e5933000     	ldr	r3, [r3]
  491544: e28db0a4     	add	r11, sp, #164
  491548: e08f2002     	add	r2, pc, r2
  49154c: e1a08001     	mov	r8, r1
  491550: e280101c     	add	r1, r0, #28
  491554: e1a0000b     	mov	r0, r11
  491558: e58d30bc     	str	r3, [sp, #0xbc]
  49155c: ebfa88da     	bl	0x3338cc <std::basic_string<char, std::char_traits<char>, std::allocator<char>> std::operator+<char, std::char_traits<char>, std::allocator<char>>(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, char const*)> @ imm = #-0x15dc98
  491560: e5951030     	ldr	r1, [r5, #0x30]
  491564: e59f2260     	ldr	r2, [pc, #0x260]        @ 0x4917cc <rnd::Tile::SetModuleMVXProperties(Module*)+0x2b4>
  491568: e28d508c     	add	r5, sp, #140
  49156c: e1a00005     	mov	r0, r5
  491570: e2811004     	add	r1, r1, #4
  491574: e08f2002     	add	r2, pc, r2
  491578: ebfa88d3     	bl	0x3338cc <std::basic_string<char, std::char_traits<char>, std::allocator<char>> std::operator+<char, std::char_traits<char>, std::allocator<char>>(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, char const*)> @ imm = #-0x15dcb4
  49157c: e59d10a0     	ldr	r1, [sp, #0xa0]
  491580: e59d209c     	ldr	r2, [sp, #0x9c]
  491584: e1a0000b     	mov	r0, r11
  491588: ebf9fc9d     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0x180d8c
  49158c: e1a00005     	mov	r0, r5
  491590: ebfa0905     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x17dbec
  491594: e59f3234     	ldr	r3, [pc, #0x234]        @ 0x4917d0 <rnd::Tile::SetModuleMVXProperties(Module*)+0x2b8>
  491598: e3a02000     	mov	r2, #0
  49159c: e59d10b8     	ldr	r1, [sp, #0xb8]
  4915a0: e7940003     	ldr	r0, [r4, r3]
  4915a4: e1a03002     	mov	r3, r2
  4915a8: e5900010     	ldr	r0, [r0, #0x10]
  4915ac: e5905034     	ldr	r5, [r0, #0x34]
  4915b0: e595c000     	ldr	r12, [r5]
  4915b4: e1a00005     	mov	r0, r5
  4915b8: e1a0e00f     	mov	lr, pc
  4915bc: e59cf088     	ldr	pc, [r12, #0x88]
  4915c0: e3500000     	cmp	r0, #0
  4915c4: e58d0018     	str	r0, [sp, #0x18]
  4915c8: 0a000078     	beq	0x4917b0 <rnd::Tile::SetModuleMVXProperties(Module*)+0x298> @ imm = #0x1e0
  4915cc: e5903000     	ldr	r3, [r0]
  4915d0: e1a0e00f     	mov	lr, pc
  4915d4: e593f008     	ldr	pc, [r3, #0x8]
  4915d8: e1a07000     	mov	r7, r0
  4915dc: ebf9f444     	bl	0x30e6f4 <.plt+0x980>   @ imm = #-0x182ef0
  4915e0: e28d60c0     	add	r6, sp, #192
  4915e4: e536c0a8     	ldr	r12, [r6, #-0xa8]!
  4915e8: e58d0000     	str	r0, [sp]
  4915ec: e1a02007     	mov	r2, r7
  4915f0: e1a03fc2     	asr	r3, r2, #31
  4915f4: e1a01000     	mov	r1, r0
  4915f8: e1a0000c     	mov	r0, r12
  4915fc: e59cc000     	ldr	r12, [r12]
  491600: e1a0e00f     	mov	lr, pc
  491604: e59cf018     	ldr	pc, [r12, #0x18]
  491608: e1a01006     	mov	r1, r6
  49160c: e5953000     	ldr	r3, [r5]
  491610: e1a00005     	mov	r0, r5
  491614: e28d601c     	add	r6, sp, #28
  491618: e1a0e00f     	mov	lr, pc
  49161c: e593f078     	ldr	pc, [r3, #0x78]
  491620: e1a00006     	mov	r0, r6
  491624: eb021626     	bl	0x516ec4 <TiXmlDocument::TiXmlDocument()> @ imm = #0x85898
  491628: e1a02007     	mov	r2, r7
  49162c: e59d1000     	ldr	r1, [sp]
  491630: e3a03000     	mov	r3, #0
  491634: e1a00006     	mov	r0, r6
  491638: eb02139a     	bl	0x5164a8 <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)> @ imm = #0x84e68
  49163c: e59f2190     	ldr	r2, [pc, #0x190]        @ 0x4917d4 <rnd::Tile::SetModuleMVXProperties(Module*)+0x2bc>
  491640: e28d7010     	add	r7, sp, #16
  491644: e1a00007     	mov	r0, r7
  491648: e08f2002     	add	r2, pc, r2
  49164c: e28d1014     	add	r1, sp, #20
  491650: e3a03000     	mov	r3, #0
  491654: e58d6014     	str	r6, [sp, #0x14]
  491658: eb020dde     	bl	0x514dd8 <TiXmlHandle::Child(char const*, int) const> @ imm = #0x83778
  49165c: e59f2174     	ldr	r2, [pc, #0x174]        @ 0x4917d8 <rnd::Tile::SetModuleMVXProperties(Module*)+0x2c0>
  491660: e28d500c     	add	r5, sp, #12
  491664: e1a01007     	mov	r1, r7
  491668: e08f2002     	add	r2, pc, r2
  49166c: e3a03000     	mov	r3, #0
  491670: e1a00005     	mov	r0, r5
  491674: eb020dd7     	bl	0x514dd8 <TiXmlHandle::Child(char const*, int) const> @ imm = #0x8375c
  491678: e1a00005     	mov	r0, r5
  49167c: ebffc8f1     	bl	0x483a48 <TiXmlHandle::ToElement() const> @ imm = #-0xdc3c
  491680: e2507000     	subs	r7, r0, #0
  491684: 0a000035     	beq	0x491760 <rnd::Tile::SetModuleMVXProperties(Module*)+0x248> @ imm = #0xd4
  491688: e59f914c     	ldr	r9, [pc, #0x14c]        @ 0x4917dc <rnd::Tile::SetModuleMVXProperties(Module*)+0x2c4>
  49168c: e59fa14c     	ldr	r10, [pc, #0x14c]       @ 0x4917e0 <rnd::Tile::SetModuleMVXProperties(Module*)+0x2c8>
  491690: e2885004     	add	r5, r8, #4
  491694: e08f9009     	add	r9, pc, r9
  491698: e1a01009     	mov	r1, r9
  49169c: eb020d73     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x835cc
  4916a0: e08fa00a     	add	r10, pc, r10
  4916a4: e1a02000     	mov	r2, r0
  4916a8: e1a01009     	mov	r1, r9
  4916ac: e1a00005     	mov	r0, r5
  4916b0: eb020871     	bl	0x51387c <PropertyMap::SetProperty(char const*, char const*)> @ imm = #0x821c4
  4916b4: e1a0100a     	mov	r1, r10
  4916b8: e1a00007     	mov	r0, r7
  4916bc: eb020d6b     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x835ac
  4916c0: e59f811c     	ldr	r8, [pc, #0x11c]        @ 0x4917e4 <rnd::Tile::SetModuleMVXProperties(Module*)+0x2cc>
  4916c4: e1a02000     	mov	r2, r0
  4916c8: e1a0100a     	mov	r1, r10
  4916cc: e08f8008     	add	r8, pc, r8
  4916d0: e1a00005     	mov	r0, r5
  4916d4: eb020868     	bl	0x51387c <PropertyMap::SetProperty(char const*, char const*)> @ imm = #0x821a0
  4916d8: e1a01008     	mov	r1, r8
  4916dc: e1a00007     	mov	r0, r7
  4916e0: eb020d62     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x83588
  4916e4: e59f90fc     	ldr	r9, [pc, #0xfc]         @ 0x4917e8 <rnd::Tile::SetModuleMVXProperties(Module*)+0x2d0>
  4916e8: e1a02000     	mov	r2, r0
  4916ec: e1a01008     	mov	r1, r8
  4916f0: e08f9009     	add	r9, pc, r9
  4916f4: e1a00005     	mov	r0, r5
  4916f8: eb02085f     	bl	0x51387c <PropertyMap::SetProperty(char const*, char const*)> @ imm = #0x8217c
  4916fc: e1a01009     	mov	r1, r9
  491700: e1a00007     	mov	r0, r7
  491704: eb020d59     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x83564
  491708: e59fa0dc     	ldr	r10, [pc, #0xdc]        @ 0x4917ec <rnd::Tile::SetModuleMVXProperties(Module*)+0x2d4>
  49170c: e1a02000     	mov	r2, r0
  491710: e1a01009     	mov	r1, r9
  491714: e08fa00a     	add	r10, pc, r10
  491718: e1a00005     	mov	r0, r5
  49171c: eb020856     	bl	0x51387c <PropertyMap::SetProperty(char const*, char const*)> @ imm = #0x82158
  491720: e1a0100a     	mov	r1, r10
  491724: e1a00007     	mov	r0, r7
  491728: eb020d50     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x83540
  49172c: e59f80bc     	ldr	r8, [pc, #0xbc]         @ 0x4917f0 <rnd::Tile::SetModuleMVXProperties(Module*)+0x2d8>
  491730: e1a02000     	mov	r2, r0
  491734: e1a0100a     	mov	r1, r10
  491738: e08f8008     	add	r8, pc, r8
  49173c: e1a00005     	mov	r0, r5
  491740: eb02084d     	bl	0x51387c <PropertyMap::SetProperty(char const*, char const*)> @ imm = #0x82134
  491744: e1a01008     	mov	r1, r8
  491748: e1a00007     	mov	r0, r7
  49174c: eb020d47     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x8351c
  491750: e1a01008     	mov	r1, r8
  491754: e1a02000     	mov	r2, r0
  491758: e1a00005     	mov	r0, r5
  49175c: eb020846     	bl	0x51387c <PropertyMap::SetProperty(char const*, char const*)> @ imm = #0x82118
  491760: e59d0000     	ldr	r0, [sp]
  491764: ebf9f1e1     	bl	0x30def0 <.plt+0x17c>   @ imm = #-0x18387c
  491768: e59f3084     	ldr	r3, [pc, #0x84]         @ 0x4917f4 <rnd::Tile::SetModuleMVXProperties(Module*)+0x2dc>
  49176c: e2860048     	add	r0, r6, #72
  491770: e7943003     	ldr	r3, [r4, r3]
  491774: e2833008     	add	r3, r3, #8
  491778: e58d301c     	str	r3, [sp, #0x1c]
  49177c: ebfa088a     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x17ddd8
  491780: e1a00006     	mov	r0, r6
  491784: eb020cca     	bl	0x514ab4 <TiXmlNode::~TiXmlNode()> @ imm = #0x83328
  491788: e1a0000b     	mov	r0, r11
  49178c: ebfa0886     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x17dde8
  491790: e59d2004     	ldr	r2, [sp, #0x4]
  491794: e7943002     	ldr	r3, [r4, r2]
  491798: e59d20bc     	ldr	r2, [sp, #0xbc]
  49179c: e5933000     	ldr	r3, [r3]
  4917a0: e1520003     	cmp	r2, r3
  4917a4: 1a000004     	bne	0x4917bc <rnd::Tile::SetModuleMVXProperties(Module*)+0x2a4> @ imm = #0x10
  4917a8: e28dd0c4     	add	sp, sp, #196
  4917ac: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  4917b0: e1a0000b     	mov	r0, r11
  4917b4: ebfa087c     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x17de10
  4917b8: eafffff4     	b	0x491790 <rnd::Tile::SetModuleMVXProperties(Module*)+0x278> @ imm = #-0x30
  4917bc: ebf9f2d3     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x1834b4
  4917c0: 68 35 50 00  	.word	0x00503568
  4917c4: ac 40 00 00  	.word	0x000040ac
  4917c8: e8 52 43 00  	.word	0x004352e8
  4917cc: c4 52 43 00  	.word	0x004352c4
  4917d0: f4 37 00 00  	.word	0x000037f4
  4917d4: e8 ed 42 00  	.word	0x0042ede8
  4917d8: 00 ec 42 00  	.word	0x0042ec00
  4917dc: ec 3a 45 00  	.word	0x00453aec
  4917e0: 08 0e 43 00  	.word	0x00430e08
  4917e4: cc 0d 43 00  	.word	0x00430dcc
  4917e8: a8 81 43 00  	.word	0x004381a8
  4917ec: 2c 0c 43 00  	.word	0x00430c2c
  4917f0: b8 0b 43 00  	.word	0x00430bb8
  4917f4: 30 09 00 00  	.word	0x00000930
