
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048e8ac <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)>:
  48e8ac: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  48e8b0: e59f5290     	ldr	r5, [pc, #0x290]        @ 0x48eb48 <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0x29c>
  48e8b4: e59fb290     	ldr	r11, [pc, #0x290]       @ 0x48eb4c <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0x2a0>
  48e8b8: e5903044     	ldr	r3, [r0, #0x44]
  48e8bc: e08f5005     	add	r5, pc, r5
  48e8c0: e795200b     	ldr	r2, [r5, r11]
  48e8c4: e24ddf81     	sub	sp, sp, #516
  48e8c8: e1a04000     	mov	r4, r0
  48e8cc: e5922000     	ldr	r2, [r2]
  48e8d0: e58d21fc     	str	r2, [sp, #0x1fc]
  48e8d4: e593c068     	ldr	r12, [r3, #0x68]
  48e8d8: e35c0000     	cmp	r12, #0
  48e8dc: 0a00005f     	beq	0x48ea60 <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0x1b4> @ imm = #0x17c
  48e8e0: e593306c     	ldr	r3, [r3, #0x6c]
  48e8e4: e3730001     	cmn	r3, #1
  48e8e8: 0a000029     	beq	0x48e994 <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0xe8> @ imm = #0xa4
  48e8ec: e59c101c     	ldr	r1, [r12, #0x1c]
  48e8f0: e59c2020     	ldr	r2, [r12, #0x20]
  48e8f4: e0612002     	rsb	r2, r1, r2
  48e8f8: e1a02242     	asr	r2, r2, #4
  48e8fc: e0820082     	add	r0, r2, r2, lsl #1
  48e900: e0800200     	add	r0, r0, r0, lsl #4
  48e904: e0800400     	add	r0, r0, r0, lsl #8
  48e908: e0800800     	add	r0, r0, r0, lsl #16
  48e90c: e0822100     	add	r2, r2, r0, lsl #2
  48e910: e1530002     	cmp	r3, r2
  48e914: 2a00001e     	bhs	0x48e994 <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0xe8> @ imm = #0x78
  48e918: e3a02050     	mov	r2, #80
  48e91c: e28d6f6b     	add	r6, sp, #428
  48e920: e0211392     	mla	r1, r2, r3, r1
  48e924: e1a00006     	mov	r0, r6
  48e928: ebfffe54     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x6b0
  48e92c: e5940004     	ldr	r0, [r4, #0x4]
  48e930: e59d11c4     	ldr	r1, [sp, #0x1c4]
  48e934: ebfffe4e     	bl	0x48e274 <rnd::Rule::GetBlock(char const*) const> @ imm = #-0x6c8
  48e938: e2509000     	subs	r9, r0, #0
  48e93c: 0a00000a     	beq	0x48e96c <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0xc0> @ imm = #0x28
  48e940: e28d7f57     	add	r7, sp, #348
  48e944: e1a01006     	mov	r1, r6
  48e948: e1a00007     	mov	r0, r7
  48e94c: ebfffe4b     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x6d4
  48e950: e1a01009     	mov	r1, r9
  48e954: e1a00004     	mov	r0, r4
  48e958: e1a02007     	mov	r2, r7
  48e95c: ebfff684     	bl	0x48c374 <rnd::RootRule::Impl::PlaceRootTile(rnd::Block*, rnd::ListElem)> @ imm = #-0x25f0
  48e960: e1a09000     	mov	r9, r0
  48e964: e1a00007     	mov	r0, r7
  48e968: ebffe618     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x67a0
  48e96c: e1a00006     	mov	r0, r6
  48e970: ebffe616     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x67a8
  48e974: e795300b     	ldr	r3, [r5, r11]
  48e978: e59d21fc     	ldr	r2, [sp, #0x1fc]
  48e97c: e1a00009     	mov	r0, r9
  48e980: e5933000     	ldr	r3, [r3]
  48e984: e1520003     	cmp	r2, r3
  48e988: 1a00006d     	bne	0x48eb44 <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0x298> @ imm = #0x1b4
  48e98c: e28ddf81     	add	sp, sp, #516
  48e990: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  48e994: e28d300c     	add	r3, sp, #12
  48e998: e28c101c     	add	r1, r12, #28
  48e99c: e1a00003     	mov	r0, r3
  48e9a0: e58d3004     	str	r3, [sp, #0x4]
  48e9a4: ebfffec5     	bl	0x48e4c0 <std::vector<rnd::ListElem, std::allocator<rnd::ListElem>>::vector(std::vector<rnd::ListElem, std::allocator<rnd::ListElem>> const&)> @ imm = #-0x4ec
  48e9a8: e5940004     	ldr	r0, [r4, #0x4]
  48e9ac: e59d6010     	ldr	r6, [sp, #0x10]
  48e9b0: e59d700c     	ldr	r7, [sp, #0xc]
  48e9b4: ebfff4ce     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x2cc8
  48e9b8: e1a01006     	mov	r1, r6
  48e9bc: e1a02000     	mov	r2, r0
  48e9c0: e1a00007     	mov	r0, r7
  48e9c4: ebffff9c     	bl	0x48e83c <void std::random_shuffle<rnd::ListElem*, rnd::RandomGenerator>(rnd::ListElem*, rnd::ListElem*, rnd::RandomGenerator&)> @ imm = #-0x190
  48e9c8: e59d600c     	ldr	r6, [sp, #0xc]
  48e9cc: e59da010     	ldr	r10, [sp, #0x10]
  48e9d0: e156000a     	cmp	r6, r10
  48e9d4: 0a00001a     	beq	0x48ea44 <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0x198> @ imm = #0x68
  48e9d8: e28d7f43     	add	r7, sp, #268
  48e9dc: e28d80bc     	add	r8, sp, #188
  48e9e0: e1a01006     	mov	r1, r6
  48e9e4: e1a00007     	mov	r0, r7
  48e9e8: ebfffe24     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x770
  48e9ec: e5940004     	ldr	r0, [r4, #0x4]
  48e9f0: e59d1124     	ldr	r1, [sp, #0x124]
  48e9f4: ebfffe1e     	bl	0x48e274 <rnd::Rule::GetBlock(char const*) const> @ imm = #-0x788
  48e9f8: e2509000     	subs	r9, r0, #0
  48e9fc: 0a00000b     	beq	0x48ea30 <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0x184> @ imm = #0x2c
  48ea00: e1a01007     	mov	r1, r7
  48ea04: e1a00008     	mov	r0, r8
  48ea08: ebfffe1c     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x790
  48ea0c: e1a01009     	mov	r1, r9
  48ea10: e1a02008     	mov	r2, r8
  48ea14: e1a00004     	mov	r0, r4
  48ea18: ebfff655     	bl	0x48c374 <rnd::RootRule::Impl::PlaceRootTile(rnd::Block*, rnd::ListElem)> @ imm = #-0x26ac
  48ea1c: e1a09000     	mov	r9, r0
  48ea20: e1a00008     	mov	r0, r8
  48ea24: ebffe5e9     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x685c
  48ea28: e3590000     	cmp	r9, #0
  48ea2c: 1a000008     	bne	0x48ea54 <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0x1a8> @ imm = #0x20
  48ea30: e2866050     	add	r6, r6, #80
  48ea34: e1a00007     	mov	r0, r7
  48ea38: ebffe5e4     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x6870
  48ea3c: e156000a     	cmp	r6, r10
  48ea40: 1affffe6     	bne	0x48e9e0 <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0x134> @ imm = #-0x68
  48ea44: e3a09000     	mov	r9, #0
  48ea48: e59d0004     	ldr	r0, [sp, #0x4]
  48ea4c: ebffe60b     	bl	0x488280 <std::vector<rnd::ListElem, std::allocator<rnd::ListElem>>::~vector()> @ imm = #-0x67d4
  48ea50: eaffffc7     	b	0x48e974 <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0xc8> @ imm = #-0xe4
  48ea54: e1a00007     	mov	r0, r7
  48ea58: ebffe5dc     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x6890
  48ea5c: eafffff9     	b	0x48ea48 <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0x19c> @ imm = #-0x1c
  48ea60: e28d300c     	add	r3, sp, #12
  48ea64: e5901030     	ldr	r1, [r0, #0x30]
  48ea68: e5902034     	ldr	r2, [r0, #0x34]
  48ea6c: e58d3004     	str	r3, [sp, #0x4]
  48ea70: e59d0004     	ldr	r0, [sp, #0x4]
  48ea74: e28d3018     	add	r3, sp, #24
  48ea78: e58dc014     	str	r12, [sp, #0x14]
  48ea7c: e58dc00c     	str	r12, [sp, #0xc]
  48ea80: e58dc010     	str	r12, [sp, #0x10]
  48ea84: ebfff7a0     	bl	0x48c90c <void std::vector<char const*, std::allocator<char const*>>::_M_range_initialize<char const**>(char const**, char const**, std::forward_iterator_tag const&)> @ imm = #-0x2180
  48ea88: e5940004     	ldr	r0, [r4, #0x4]
  48ea8c: e59d6010     	ldr	r6, [sp, #0x10]
  48ea90: e59d700c     	ldr	r7, [sp, #0xc]
  48ea94: ebfff496     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x2da8
  48ea98: e1a01006     	mov	r1, r6
  48ea9c: e1a02000     	mov	r2, r0
  48eaa0: e1a00007     	mov	r0, r7
  48eaa4: ebfff61c     	bl	0x48c31c <void std::random_shuffle<char const**, rnd::RandomGenerator>(char const**, char const**, rnd::RandomGenerator&)> @ imm = #-0x2790
  48eaa8: e59d600c     	ldr	r6, [sp, #0xc]
  48eaac: e59d3010     	ldr	r3, [sp, #0x10]
  48eab0: e1560003     	cmp	r6, r3
  48eab4: 0a00001b     	beq	0x48eb28 <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0x27c> @ imm = #0x6c
  48eab8: e28d706c     	add	r7, sp, #108
  48eabc: e28d801c     	add	r8, sp, #28
  48eac0: e596a000     	ldr	r10, [r6]
  48eac4: e1a00007     	mov	r0, r7
  48eac8: ebfffd6c     	bl	0x48e080 <rnd::ListElem::ListElem()> @ imm = #-0xa50
  48eacc: e1a0100a     	mov	r1, r10
  48ead0: e5940004     	ldr	r0, [r4, #0x4]
  48ead4: ebfffde6     	bl	0x48e274 <rnd::Rule::GetBlock(char const*) const> @ imm = #-0x868
  48ead8: e250a000     	subs	r10, r0, #0
  48eadc: 0a00000b     	beq	0x48eb10 <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0x264> @ imm = #0x2c
  48eae0: e1a01007     	mov	r1, r7
  48eae4: e1a00008     	mov	r0, r8
  48eae8: ebfffde4     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x870
  48eaec: e1a0100a     	mov	r1, r10
  48eaf0: e1a02008     	mov	r2, r8
  48eaf4: e1a00004     	mov	r0, r4
  48eaf8: ebfff61d     	bl	0x48c374 <rnd::RootRule::Impl::PlaceRootTile(rnd::Block*, rnd::ListElem)> @ imm = #-0x278c
  48eafc: e1a09000     	mov	r9, r0
  48eb00: e1a00008     	mov	r0, r8
  48eb04: ebffe5b1     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x693c
  48eb08: e3590000     	cmp	r9, #0
  48eb0c: 1a000009     	bne	0x48eb38 <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0x28c> @ imm = #0x24
  48eb10: e1a00007     	mov	r0, r7
  48eb14: ebffe5ad     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x694c
  48eb18: e59d3010     	ldr	r3, [sp, #0x10]
  48eb1c: e2866004     	add	r6, r6, #4
  48eb20: e1560003     	cmp	r6, r3
  48eb24: 1affffe5     	bne	0x48eac0 <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0x214> @ imm = #-0x6c
  48eb28: e3a09000     	mov	r9, #0
  48eb2c: e59d0004     	ldr	r0, [sp, #0x4]
  48eb30: ebfff7d2     	bl	0x48ca80 <std::vector<char const*, std::allocator<char const*>>::~vector()> @ imm = #-0x20b8
  48eb34: eaffff8e     	b	0x48e974 <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0xc8> @ imm = #-0x1c8
  48eb38: e1a00007     	mov	r0, r7
  48eb3c: ebffe5a3     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x6974
  48eb40: eafffff9     	b	0x48eb2c <rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)+0x280> @ imm = #-0x1c
  48eb44: ebf9fdf1     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x18083c
  48eb48: d4 61 50 00  	.word	0x005061d4
  48eb4c: ac 40 00 00  	.word	0x000040ac
