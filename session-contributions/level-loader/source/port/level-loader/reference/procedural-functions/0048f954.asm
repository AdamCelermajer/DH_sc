
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048f954 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)>:
  48f954: e59f33fc     	ldr	r3, [pc, #0x3fc]        @ 0x48fd58 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x404>
  48f958: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  48f95c: e59fc3f8     	ldr	r12, [pc, #0x3f8]       @ 0x48fd5c <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x408>
  48f960: e08f3003     	add	r3, pc, r3
  48f964: e1a08001     	mov	r8, r1
  48f968: e793100c     	ldr	r1, [r3, r12]
  48f96c: e24ddff5     	sub	sp, sp, #980
  48f970: e58d301c     	str	r3, [sp, #0x1c]
  48f974: e58dc028     	str	r12, [sp, #0x28]
  48f978: e5911000     	ldr	r1, [r1]
  48f97c: e5983030     	ldr	r3, [r8, #0x30]
  48f980: e3a05000     	mov	r5, #0
  48f984: e58d5224     	str	r5, [sp, #0x224]
  48f988: e58d5228     	str	r5, [sp, #0x228]
  48f98c: e58d13cc     	str	r1, [sp, #0x3cc]
  48f990: e58d522c     	str	r5, [sp, #0x22c]
  48f994: e593105c     	ldr	r1, [r3, #0x5c]
  48f998: e1a04000     	mov	r4, r0
  48f99c: e58d200c     	str	r2, [sp, #0xc]
  48f9a0: e1510005     	cmp	r1, r5
  48f9a4: d28d1f89     	addle	r1, sp, #548
  48f9a8: d58d1004     	strle	r1, [sp, #0x4]
  48f9ac: da000027     	ble	0x48fa50 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0xfc> @ imm = #0x9c
  48f9b0: e59d600c     	ldr	r6, [sp, #0xc]
  48f9b4: e28d2fa1     	add	r2, sp, #644
  48f9b8: e28dcf89     	add	r12, sp, #548
  48f9bc: e88d1004     	stm	sp, {r2, r12}
  48f9c0: e3a0bf4b     	mov	r11, #300
  48f9c4: e28dafdf     	add	r10, sp, #892
  48f9c8: e28d7fb7     	add	r7, sp, #732
  48f9cc: e2829004     	add	r9, r2, #4
  48f9d0: e58d0008     	str	r0, [sp, #0x8]
  48f9d4: e004059b     	mul	r4, r11, r5
  48f9d8: e2844060     	add	r4, r4, #96
  48f9dc: e0834004     	add	r4, r3, r4
  48f9e0: e1540006     	cmp	r4, r6
  48f9e4: 0a000014     	beq	0x48fa3c <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0xe8> @ imm = #0x50
  48f9e8: e1a0000a     	mov	r0, r10
  48f9ec: ebfff9a3     	bl	0x48e080 <rnd::ListElem::ListElem()> @ imm = #-0x1974
  48f9f0: e1a0100a     	mov	r1, r10
  48f9f4: e1a00007     	mov	r0, r7
  48f9f8: e58d42d8     	str	r4, [sp, #0x2d8]
  48f9fc: ebfffa1f     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x1784
  48fa00: e59d32d8     	ldr	r3, [sp, #0x2d8]
  48fa04: e1a01007     	mov	r1, r7
  48fa08: e1a00009     	mov	r0, r9
  48fa0c: e58d3284     	str	r3, [sp, #0x284]
  48fa10: ebfffa1a     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x1798
  48fa14: e59d1000     	ldr	r1, [sp]
  48fa18: e59d0004     	ldr	r0, [sp, #0x4]
  48fa1c: ebfffd41     	bl	0x48ef28 <std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>::push_back(std::pair<rnd::Exit const*, rnd::ListElem> const&)> @ imm = #-0xafc
  48fa20: e1a00009     	mov	r0, r9
  48fa24: ebffe1e9     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x785c
  48fa28: e1a00007     	mov	r0, r7
  48fa2c: ebffe1e7     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x7864
  48fa30: e1a0000a     	mov	r0, r10
  48fa34: ebffe1e5     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x786c
  48fa38: e5983030     	ldr	r3, [r8, #0x30]
  48fa3c: e593205c     	ldr	r2, [r3, #0x5c]
  48fa40: e2855001     	add	r5, r5, #1
  48fa44: e1520005     	cmp	r2, r5
  48fa48: caffffe1     	bgt	0x48f9d4 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x80> @ imm = #-0x7c
  48fa4c: e59d4008     	ldr	r4, [sp, #0x8]
  48fa50: e59d7228     	ldr	r7, [sp, #0x228]
  48fa54: e59da224     	ldr	r10, [sp, #0x224]
  48fa58: e5940004     	ldr	r0, [r4, #0x4]
  48fa5c: e30c3f3d     	movw	r3, #0xcf3d
  48fa60: e06a6007     	rsb	r6, r10, r7
  48fa64: e3433cf3     	movt	r3, #0x3cf3
  48fa68: e1a06146     	asr	r6, r6, #2
  48fa6c: e0060693     	mul	r6, r3, r6
  48fa70: e590500c     	ldr	r5, [r0, #0xc]
  48fa74: ebfff09e     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x3d88
  48fa78: e1a01007     	mov	r1, r7
  48fa7c: e1a02000     	mov	r2, r0
  48fa80: e1a0000a     	mov	r0, r10
  48fa84: ebfffae7     	bl	0x48e628 <void std::random_shuffle<std::pair<rnd::Exit const*, rnd::ListElem>*, rnd::RandomGenerator>(std::pair<rnd::Exit const*, rnd::ListElem>*, std::pair<rnd::Exit const*, rnd::ListElem>*, rnd::RandomGenerator&)> @ imm = #-0x1464
  48fa88: e3550000     	cmp	r5, #0
  48fa8c: 13560000     	cmpne	r6, #0
  48fa90: 13a07000     	movne	r7, #0
  48fa94: 03a07001     	moveq	r7, #1
  48fa98: 0a000084     	beq	0x48fcb0 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x35c> @ imm = #0x210
  48fa9c: e59f32bc     	ldr	r3, [pc, #0x2bc]        @ 0x48fd60 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x40c>
  48faa0: e59d101c     	ldr	r1, [sp, #0x1c]
  48faa4: e300a2d6     	movw	r10, #0x2d6
  48faa8: e301e104     	movw	lr, #0x1104
  48faac: e00a059a     	mul	r10, r10, r5
  48fab0: e00e069e     	mul	lr, lr, r6
  48fab4: e791c003     	ldr	r12, [r1, r3]
  48fab8: e08a300e     	add	r3, r10, lr
  48fabc: e19c20d3     	ldrsb	r2, [r12, r3]
  48fac0: e08c3003     	add	r3, r12, r3
  48fac4: e3520000     	cmp	r2, #0
  48fac8: ba000099     	blt	0x48fd34 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x3e0> @ imm = #0x264
  48facc: e28d5034     	add	r5, sp, #52
  48fad0: e3a00006     	mov	r0, #6
  48fad4: e022a790     	mla	r2, r0, r7, r10
  48fad8: e1f310d6     	ldrsb	r1, [r3, #6]!
  48fadc: e082200e     	add	r2, r2, lr
  48fae0: e082200c     	add	r2, r2, r12
  48fae4: e3510000     	cmp	r1, #0
  48fae8: e7852107     	str	r2, [r5, r7, lsl #2]
  48faec: e2877001     	add	r7, r7, #1
  48faf0: aafffff7     	bge	0x48fad4 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x180> @ imm = #-0x24
  48faf4: e5940004     	ldr	r0, [r4, #0x4]
  48faf8: e1a07107     	lsl	r7, r7, #2
  48fafc: e58d7010     	str	r7, [sp, #0x10]
  48fb00: ebfff07b     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x3e14
  48fb04: e0266696     	mla	r6, r6, r6, r6
  48fb08: e59d7010     	ldr	r7, [sp, #0x10]
  48fb0c: e1a02000     	mov	r2, r0
  48fb10: e0866fa6     	add	r6, r6, r6, lsr #31
  48fb14: e0851007     	add	r1, r5, r7
  48fb18: e1a00005     	mov	r0, r5
  48fb1c: ebfff1fe     	bl	0x48c31c <void std::random_shuffle<char const**, rnd::RandomGenerator>(char const**, char const**, rnd::RandomGenerator&)> @ imm = #-0x3808
  48fb20: e28d2e23     	add	r2, sp, #560
  48fb24: e28d3f86     	add	r3, sp, #536
  48fb28: e30caf3d     	movw	r10, #0xcf3d
  48fb2c: e1a060c6     	asr	r6, r6, #1
  48fb30: e28dcfcb     	add	r12, sp, #812
  48fb34: e2821004     	add	r1, r2, #4
  48fb38: e58d2024     	str	r2, [sp, #0x24]
  48fb3c: e58d6018     	str	r6, [sp, #0x18]
  48fb40: e343acf3     	movt	r10, #0x3cf3
  48fb44: e3a07000     	mov	r7, #0
  48fb48: e58d302c     	str	r3, [sp, #0x2c]
  48fb4c: e58dc008     	str	r12, [sp, #0x8]
  48fb50: e58d1020     	str	r1, [sp, #0x20]
  48fb54: e58d8000     	str	r8, [sp]
  48fb58: e58d5014     	str	r5, [sp, #0x14]
  48fb5c: e1a0b003     	mov	r11, r3
  48fb60: e1a0000b     	mov	r0, r11
  48fb64: e59d1004     	ldr	r1, [sp, #0x4]
  48fb68: ebfffa24     	bl	0x48e400 <std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>::vector(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>> const&)> @ imm = #-0x1770
  48fb6c: e59d3014     	ldr	r3, [sp, #0x14]
  48fb70: e59d2218     	ldr	r2, [sp, #0x218]
  48fb74: e7936007     	ldr	r6, [r3, r7]
  48fb78: e59d321c     	ldr	r3, [sp, #0x21c]
  48fb7c: e0623003     	rsb	r3, r2, r3
  48fb80: e1a03143     	asr	r3, r3, #2
  48fb84: e003039a     	mul	r3, r10, r3
  48fb88: e3530000     	cmp	r3, #0
  48fb8c: 0a000045     	beq	0x48fca8 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x354> @ imm = #0x114
  48fb90: e3a00000     	mov	r0, #0
  48fb94: e1a05000     	mov	r5, r0
  48fb98: e3a08054     	mov	r8, #84
  48fb9c: e1a09007     	mov	r9, r7
  48fba0: e19610d0     	ldrsb	r1, [r6, r0]
  48fba4: e5943004     	ldr	r3, [r4, #0x4]
  48fba8: e0000098     	mul	r0, r8, r0
  48fbac: e2811004     	add	r1, r1, #4
  48fbb0: e7933101     	ldr	r3, [r3, r1, lsl #2]
  48fbb4: e7927000     	ldr	r7, [r2, r0]
  48fbb8: e1a01004     	mov	r1, r4
  48fbbc: e1a00003     	mov	r0, r3
  48fbc0: e5933000     	ldr	r3, [r3]
  48fbc4: e1a0e00f     	mov	lr, pc
  48fbc8: e593f008     	ldr	pc, [r3, #0x8]
  48fbcc: e2503000     	subs	r3, r0, #0
  48fbd0: 0a00004c     	beq	0x48fd08 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x3b4> @ imm = #0x130
  48fbd4: e593203c     	ldr	r2, [r3, #0x3c]
  48fbd8: e3520000     	cmp	r2, #0
  48fbdc: 0a000049     	beq	0x48fd08 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x3b4> @ imm = #0x124
  48fbe0: e5971014     	ldr	r1, [r7, #0x14]
  48fbe4: e5922000     	ldr	r2, [r2]
  48fbe8: e5911000     	ldr	r1, [r1]
  48fbec: e1520001     	cmp	r2, r1
  48fbf0: 0a000044     	beq	0x48fd08 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x3b4> @ imm = #0x110
  48fbf4: e59d3218     	ldr	r3, [sp, #0x218]
  48fbf8: e59d221c     	ldr	r2, [sp, #0x21c]
  48fbfc: e59dc018     	ldr	r12, [sp, #0x18]
  48fc00: e0633002     	rsb	r3, r3, r2
  48fc04: e1a03143     	asr	r3, r3, #2
  48fc08: e003039a     	mul	r3, r10, r3
  48fc0c: e15c0003     	cmp	r12, r3
  48fc10: 8a00000e     	bhi	0x48fc50 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x2fc> @ imm = #0x38
  48fc14: e1a07009     	mov	r7, r9
  48fc18: e594600c     	ldr	r6, [r4, #0xc]
  48fc1c: e3560000     	cmp	r6, #0
  48fc20: 0a00002f     	beq	0x48fce4 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x390> @ imm = #0xbc
  48fc24: e2466001     	sub	r6, r6, #1
  48fc28: e584600c     	str	r6, [r4, #0xc]
  48fc2c: e2863004     	add	r3, r6, #4
  48fc30: e7943103     	ldr	r3, [r4, r3, lsl #2]
  48fc34: e3530000     	cmp	r3, #0
  48fc38: 0afffff7     	beq	0x48fc1c <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x2c8> @ imm = #-0x24
  48fc3c: e1a00003     	mov	r0, r3
  48fc40: e5933000     	ldr	r3, [r3]
  48fc44: e1a0e00f     	mov	lr, pc
  48fc48: e593f004     	ldr	pc, [r3, #0x4]
  48fc4c: eafffff1     	b	0x48fc18 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x2c4> @ imm = #-0x3c
  48fc50: e59d0008     	ldr	r0, [sp, #0x8]
  48fc54: ebfff909     	bl	0x48e080 <rnd::ListElem::ListElem()> @ imm = #-0x1bdc
  48fc58: e59d1008     	ldr	r1, [sp, #0x8]
  48fc5c: e59d0020     	ldr	r0, [sp, #0x20]
  48fc60: e58d7230     	str	r7, [sp, #0x230]
  48fc64: ebfff985     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x19ec
  48fc68: e1a0000b     	mov	r0, r11
  48fc6c: e59d1024     	ldr	r1, [sp, #0x24]
  48fc70: ebfffcac     	bl	0x48ef28 <std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>::push_back(std::pair<rnd::Exit const*, rnd::ListElem> const&)> @ imm = #-0xd50
  48fc74: e59d0020     	ldr	r0, [sp, #0x20]
  48fc78: ebffe154     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x7ab0
  48fc7c: e59d0008     	ldr	r0, [sp, #0x8]
  48fc80: ebffe152     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x7ab8
  48fc84: e59d2218     	ldr	r2, [sp, #0x218]
  48fc88: e59d321c     	ldr	r3, [sp, #0x21c]
  48fc8c: e2855001     	add	r5, r5, #1
  48fc90: e1a00005     	mov	r0, r5
  48fc94: e0623003     	rsb	r3, r2, r3
  48fc98: e1a03143     	asr	r3, r3, #2
  48fc9c: e003039a     	mul	r3, r10, r3
  48fca0: e1550003     	cmp	r5, r3
  48fca4: 3affffbd     	blo	0x48fba0 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x24c> @ imm = #-0x10c
  48fca8: e59d002c     	ldr	r0, [sp, #0x2c]
  48fcac: ebfff711     	bl	0x48d8f8 <std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>::~vector()> @ imm = #-0x23bc
  48fcb0: e3a06001     	mov	r6, #1
  48fcb4: e59d0004     	ldr	r0, [sp, #0x4]
  48fcb8: ebfff70e     	bl	0x48d8f8 <std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>::~vector()> @ imm = #-0x23c8
  48fcbc: e59d2028     	ldr	r2, [sp, #0x28]
  48fcc0: e59dc01c     	ldr	r12, [sp, #0x1c]
  48fcc4: e1a00006     	mov	r0, r6
  48fcc8: e79c3002     	ldr	r3, [r12, r2]
  48fccc: e59d23cc     	ldr	r2, [sp, #0x3cc]
  48fcd0: e5933000     	ldr	r3, [r3]
  48fcd4: e1520003     	cmp	r2, r3
  48fcd8: 1a00001d     	bne	0x48fd54 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x400> @ imm = #0x74
  48fcdc: e28ddff5     	add	sp, sp, #980
  48fce0: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  48fce4: e59d0000     	ldr	r0, [sp]
  48fce8: eb00070e     	bl	0x491928 <rnd::Tile::RemoveNeighbors()> @ imm = #0x1c38
  48fcec: e1a0000b     	mov	r0, r11
  48fcf0: ebfff700     	bl	0x48d8f8 <std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>::~vector()> @ imm = #-0x2400
  48fcf4: e59d1010     	ldr	r1, [sp, #0x10]
  48fcf8: e2877004     	add	r7, r7, #4
  48fcfc: e1510007     	cmp	r1, r7
  48fd00: 1affff96     	bne	0x48fb60 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x20c> @ imm = #-0x1a8
  48fd04: eaffffea     	b	0x48fcb4 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x360> @ imm = #-0x58
  48fd08: e1a00003     	mov	r0, r3
  48fd0c: e593c000     	ldr	r12, [r3]
  48fd10: e1a02007     	mov	r2, r7
  48fd14: e59d1000     	ldr	r1, [sp]
  48fd18: e59d300c     	ldr	r3, [sp, #0xc]
  48fd1c: e1a0e00f     	mov	lr, pc
  48fd20: e59cf00c     	ldr	pc, [r12, #0xc]
  48fd24: e3500000     	cmp	r0, #0
  48fd28: 1affffd5     	bne	0x48fc84 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x330> @ imm = #-0xac
  48fd2c: e1a07009     	mov	r7, r9
  48fd30: eaffffb8     	b	0x48fc18 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x2c4> @ imm = #-0x120
  48fd34: e5940004     	ldr	r0, [r4, #0x4]
  48fd38: ebffefed     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x404c
  48fd3c: e1a02000     	mov	r2, r0
  48fd40: e28d0034     	add	r0, sp, #52
  48fd44: e1a01000     	mov	r1, r0
  48fd48: ebfff173     	bl	0x48c31c <void std::random_shuffle<char const**, rnd::RandomGenerator>(char const**, char const**, rnd::RandomGenerator&)> @ imm = #-0x3a34
  48fd4c: e1a06007     	mov	r6, r7
  48fd50: eaffffd7     	b	0x48fcb4 <rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)+0x360> @ imm = #-0xa4
  48fd54: ebf9f96d     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x181a4c
  48fd58: 30 51 50 00  	.word	0x00505130
  48fd5c: ac 40 00 00  	.word	0x000040ac
  48fd60: 9c 23 00 00  	.word	0x0000239c
