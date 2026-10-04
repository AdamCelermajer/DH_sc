
R:\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0042efb8 <MenuManager::PostLoad()>:
  42efb8: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  42efbc: e59f9320     	ldr	r9, [pc, #0x320]        @ 0x42f2e4 <MenuManager::PostLoad()+0x32c>
  42efc0: e59f2320     	ldr	r2, [pc, #0x320]        @ 0x42f2e8 <MenuManager::PostLoad()+0x330>
  42efc4: e24dd084     	sub	sp, sp, #132
  42efc8: e08f9009     	add	r9, pc, r9
  42efcc: e7993002     	ldr	r3, [r9, r2]
  42efd0: e58d2020     	str	r2, [sp, #0x20]
  42efd4: e59f2310     	ldr	r2, [pc, #0x310]        @ 0x42f2ec <MenuManager::PostLoad()+0x334>
  42efd8: e5933000     	ldr	r3, [r3]
  42efdc: e58d0004     	str	r0, [sp, #0x4]
  42efe0: e58d2028     	str	r2, [sp, #0x28]
  42efe4: e58d307c     	str	r3, [sp, #0x7c]
  42efe8: e59f3300     	ldr	r3, [pc, #0x300]        @ 0x42f2f0 <MenuManager::PostLoad()+0x338>
  42efec: e08f3003     	add	r3, pc, r3
  42eff0: e58d3024     	str	r3, [sp, #0x24]
  42eff4: e59f32f8     	ldr	r3, [pc, #0x2f8]        @ 0x42f2f4 <MenuManager::PostLoad()+0x33c>
  42eff8: e08f3003     	add	r3, pc, r3
  42effc: e58d3008     	str	r3, [sp, #0x8]
  42f000: e59f32f0     	ldr	r3, [pc, #0x2f0]        @ 0x42f2f8 <MenuManager::PostLoad()+0x340>
  42f004: e08f3003     	add	r3, pc, r3
  42f008: e58d3014     	str	r3, [sp, #0x14]
  42f00c: e3a03000     	mov	r3, #0
  42f010: e58d3018     	str	r3, [sp, #0x18]
  42f014: e59d2004     	ldr	r2, [sp, #0x4]
  42f018: e59230f4     	ldr	r3, [r2, #0xf4]
  42f01c: e59d2018     	ldr	r2, [sp, #0x18]
  42f020: e0833102     	add	r3, r3, r2, lsl #2
  42f024: e5934134     	ldr	r4, [r3, #0x134]
  42f028: e3540000     	cmp	r4, #0
  42f02c: 0a000035     	beq	0x42f108 <MenuManager::PostLoad()+0x150> @ imm = #0xd4
  42f030: e1a00004     	mov	r0, r4
  42f034: eb0de319     	bl	0x7a7ca0 <RenderFX::GetFlashRoot() const> @ imm = #0x378c64
  42f038: e3a05000     	mov	r5, #0
  42f03c: e1a01000     	mov	r1, r0
  42f040: e59d2024     	ldr	r2, [sp, #0x24]
  42f044: e1a00004     	mov	r0, r4
  42f048: e1a03005     	mov	r3, r5
  42f04c: eb0de6ed     	bl	0x7a8c08 <RenderFX::FindCharacters(gameswf::character*, char const*, int)> @ imm = #0x379bb4
  42f050: e58d5034     	str	r5, [sp, #0x34]
  42f054: e58d5038     	str	r5, [sp, #0x38]
  42f058: e58d503c     	str	r5, [sp, #0x3c]
  42f05c: e5cd5040     	strb	r5, [sp, #0x40]
  42f060: e5906004     	ldr	r6, [r0, #0x4]
  42f064: e1a07000     	mov	r7, r0
  42f068: e1560005     	cmp	r6, r5
  42f06c: aa000032     	bge	0x42f13c <MenuManager::PostLoad()+0x184> @ imm = #0xc8
  42f070: e28d3034     	add	r3, sp, #52
  42f074: e58d6038     	str	r6, [sp, #0x38]
  42f078: e58d302c     	str	r3, [sp, #0x2c]
  42f07c: e1a00004     	mov	r0, r4
  42f080: eb0de306     	bl	0x7a7ca0 <RenderFX::GetFlashRoot() const> @ imm = #0x378c18
  42f084: e59d3028     	ldr	r3, [sp, #0x28]
  42f088: e1a01000     	mov	r1, r0
  42f08c: e1a00004     	mov	r0, r4
  42f090: e08f2003     	add	r2, pc, r3
  42f094: e3a03000     	mov	r3, #0
  42f098: eb0de6da     	bl	0x7a8c08 <RenderFX::FindCharacters(gameswf::character*, char const*, int)> @ imm = #0x379b68
  42f09c: e5903004     	ldr	r3, [r0, #0x4]
  42f0a0: e1a06000     	mov	r6, r0
  42f0a4: e3530000     	cmp	r3, #0
  42f0a8: da00000e     	ble	0x42f0e8 <MenuManager::PostLoad()+0x130> @ imm = #0x38
  42f0ac: e59f7248     	ldr	r7, [pc, #0x248]        @ 0x42f2fc <MenuManager::PostLoad()+0x344>
  42f0b0: e3a05000     	mov	r5, #0
  42f0b4: e5963000     	ldr	r3, [r6]
  42f0b8: e1a00004     	mov	r0, r4
  42f0bc: e7938105     	ldr	r8, [r3, r5, lsl #2]
  42f0c0: eb0de2f9     	bl	0x7a7cac <RenderFX::GetRoot() const> @ imm = #0x378be4
  42f0c4: e1a01008     	mov	r1, r8
  42f0c8: e1a03000     	mov	r3, r0
  42f0cc: e7992007     	ldr	r2, [r9, r7]
  42f0d0: e1a00004     	mov	r0, r4
  42f0d4: eb0de369     	bl	0x7a7e80 <RenderFX::RegisterDisplayCallback(gameswf::character*, void (*)(gameswf::render_state&, void*), void*)> @ imm = #0x378da4
  42f0d8: e5963004     	ldr	r3, [r6, #0x4]
  42f0dc: e2855001     	add	r5, r5, #1
  42f0e0: e1550003     	cmp	r5, r3
  42f0e4: bafffff2     	blt	0x42f0b4 <MenuManager::PostLoad()+0xfc> @ imm = #-0x38
  42f0e8: e59d3038     	ldr	r3, [sp, #0x38]
  42f0ec: e3530000     	cmp	r3, #0
  42f0f0: da000071     	ble	0x42f2bc <MenuManager::PostLoad()+0x304> @ imm = #0x1c4
  42f0f4: e3a03000     	mov	r3, #0
  42f0f8: e59d002c     	ldr	r0, [sp, #0x2c]
  42f0fc: e1a01003     	mov	r1, r3
  42f100: e58d3038     	str	r3, [sp, #0x38]
  42f104: ebff9342     	bl	0x413e14 <gameswf::array<gameswf::character*>::reserve(int)> @ imm = #-0x1b2f8
  42f108: e59d2018     	ldr	r2, [sp, #0x18]
  42f10c: e2822001     	add	r2, r2, #1
  42f110: e3520004     	cmp	r2, #4
  42f114: e58d2018     	str	r2, [sp, #0x18]
  42f118: 1affffbd     	bne	0x42f014 <MenuManager::PostLoad()+0x5c> @ imm = #-0x10c
  42f11c: e59d2020     	ldr	r2, [sp, #0x20]
  42f120: e7993002     	ldr	r3, [r9, r2]
  42f124: e59d207c     	ldr	r2, [sp, #0x7c]
  42f128: e5933000     	ldr	r3, [r3]
  42f12c: e1520003     	cmp	r2, r3
  42f130: 1a00006a     	bne	0x42f2e0 <MenuManager::PostLoad()+0x328> @ imm = #0x1a8
  42f134: e28dd084     	add	sp, sp, #132
  42f138: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  42f13c: 0affffcb     	beq	0x42f070 <MenuManager::PostLoad()+0xb8> @ imm = #-0xd4
  42f140: daffffca     	ble	0x42f070 <MenuManager::PostLoad()+0xb8> @ imm = #-0xd8
  42f144: e28d3034     	add	r3, sp, #52
  42f148: e1a00003     	mov	r0, r3
  42f14c: e08610c6     	add	r1, r6, r6, asr #1
  42f150: e58d302c     	str	r3, [sp, #0x2c]
  42f154: ebff932e     	bl	0x413e14 <gameswf::array<gameswf::character*>::reserve(int)> @ imm = #-0x1b348
  42f158: e1a02005     	mov	r2, r5
  42f15c: e59d3034     	ldr	r3, [sp, #0x34]
  42f160: e7832105     	str	r2, [r3, r5, lsl #2]
  42f164: e2855001     	add	r5, r5, #1
  42f168: e1550006     	cmp	r5, r6
  42f16c: 1afffffa     	bne	0x42f15c <MenuManager::PostLoad()+0x1a4> @ imm = #-0x18
  42f170: e58d5038     	str	r5, [sp, #0x38]
  42f174: e5973000     	ldr	r3, [r7]
  42f178: e7931102     	ldr	r1, [r3, r2, lsl #2]
  42f17c: e59d3034     	ldr	r3, [sp, #0x34]
  42f180: e7831102     	str	r1, [r3, r2, lsl #2]
  42f184: e59d3038     	ldr	r3, [sp, #0x38]
  42f188: e2822001     	add	r2, r2, #1
  42f18c: e1520003     	cmp	r2, r3
  42f190: bafffff7     	blt	0x42f174 <MenuManager::PostLoad()+0x1bc> @ imm = #-0x24
  42f194: e3530000     	cmp	r3, #0
  42f198: daffffb7     	ble	0x42f07c <MenuManager::PostLoad()+0xc4> @ imm = #-0x124
  42f19c: e59f215c     	ldr	r2, [pc, #0x15c]        @ 0x42f300 <MenuManager::PostLoad()+0x348>
  42f1a0: e28d3044     	add	r3, sp, #68
  42f1a4: e3a05000     	mov	r5, #0
  42f1a8: e58d200c     	str	r2, [sp, #0xc]
  42f1ac: e28d6064     	add	r6, sp, #100
  42f1b0: e799a002     	ldr	r10, [r9, r2]
  42f1b4: e28db048     	add	r11, sp, #72
  42f1b8: e28d804c     	add	r8, sp, #76
  42f1bc: e58d3010     	str	r3, [sp, #0x10]
  42f1c0: e58d401c     	str	r4, [sp, #0x1c]
  42f1c4: ea000010     	b	0x42f20c <MenuManager::PostLoad()+0x254> @ imm = #0x40
  42f1c8: e59d300c     	ldr	r3, [sp, #0xc]
  42f1cc: e2855001     	add	r5, r5, #1
  42f1d0: e7994003     	ldr	r4, [r9, r3]
  42f1d4: e1a00004     	mov	r0, r4
  42f1d8: ebfc21aa     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xf7958
  42f1dc: e59d2010     	ldr	r2, [sp, #0x10]
  42f1e0: e59d1014     	ldr	r1, [sp, #0x14]
  42f1e4: e1a00008     	mov	r0, r8
  42f1e8: ebfb93bf     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x11b104
  42f1ec: e1a01008     	mov	r1, r8
  42f1f0: e1a00004     	mov	r0, r4
  42f1f4: ebfc2223     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xf7774
  42f1f8: e1a00008     	mov	r0, r8
  42f1fc: ebfba414     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x116fb0
  42f200: e59d3038     	ldr	r3, [sp, #0x38]
  42f204: e1550003     	cmp	r5, r3
  42f208: aa000029     	bge	0x42f2b4 <MenuManager::PostLoad()+0x2fc> @ imm = #0xa4
  42f20c: e59d3034     	ldr	r3, [sp, #0x34]
  42f210: e1a0000a     	mov	r0, r10
  42f214: e7934105     	ldr	r4, [r3, r5, lsl #2]
  42f218: ebfc219a     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xf7998
  42f21c: e1a0200b     	mov	r2, r11
  42f220: e59d1008     	ldr	r1, [sp, #0x8]
  42f224: e1a00006     	mov	r0, r6
  42f228: ebfb93af     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x11b144
  42f22c: e1a01006     	mov	r1, r6
  42f230: e1a0000a     	mov	r0, r10
  42f234: ebfc2213     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xf77b4
  42f238: e1a00006     	mov	r0, r6
  42f23c: ebfba404     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x116ff0
  42f240: e5941044     	ldr	r1, [r4, #0x44]
  42f244: e59d0004     	ldr	r0, [sp, #0x4]
  42f248: e1d130d0     	ldrsb	r3, [r1]
  42f24c: e3730001     	cmn	r3, #1
  42f250: 12811001     	addne	r1, r1, #1
  42f254: 0591100c     	ldreq	r1, [r1, #0xc]
  42f258: ebfff7e4     	bl	0x42d1f0 <MenuManager::GetMenuByName(char const*)> @ imm = #-0x2070
  42f25c: e3500000     	cmp	r0, #0
  42f260: 1affffd8     	bne	0x42f1c8 <MenuManager::PostLoad()+0x210> @ imm = #-0xa0
  42f264: e5947044     	ldr	r7, [r4, #0x44]
  42f268: e3a01008     	mov	r1, #8
  42f26c: e3a000c4     	mov	r0, #196
  42f270: e1d730d0     	ldrsb	r3, [r7]
  42f274: e2855001     	add	r5, r5, #1
  42f278: e3730001     	cmn	r3, #1
  42f27c: 12877001     	addne	r7, r7, #1
  42f280: 0597700c     	ldreq	r7, [r7, #0xc]
  42f284: ebfb84b9     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x11ed1c
  42f288: e1a01007     	mov	r1, r7
  42f28c: e1a04000     	mov	r4, r0
  42f290: ebffddd1     	bl	0x4269dc <MenuBase::MenuBase(char const*)> @ imm = #-0x88bc
  42f294: e3a02001     	mov	r2, #1
  42f298: e5c4207d     	strb	r2, [r4, #0x7d]
  42f29c: ebfff5fa     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #-0x2818
  42f2a0: e1a01004     	mov	r1, r4
  42f2a4: ebfffefa     	bl	0x42ee94 <MenuManager::RegisterMenu(MenuBase*)> @ imm = #-0x418
  42f2a8: e59d3038     	ldr	r3, [sp, #0x38]
  42f2ac: e1550003     	cmp	r5, r3
  42f2b0: baffffd5     	blt	0x42f20c <MenuManager::PostLoad()+0x254> @ imm = #-0xac
  42f2b4: e59d401c     	ldr	r4, [sp, #0x1c]
  42f2b8: eaffff6f     	b	0x42f07c <MenuManager::PostLoad()+0xc4> @ imm = #-0x244
  42f2bc: aaffff8c     	bge	0x42f0f4 <MenuManager::PostLoad()+0x13c> @ imm = #-0x1d0
  42f2c0: e1a02103     	lsl	r2, r3, #2
  42f2c4: e3a00000     	mov	r0, #0
  42f2c8: e59d1034     	ldr	r1, [sp, #0x34]
  42f2cc: e2933001     	adds	r3, r3, #1
  42f2d0: e7810002     	str	r0, [r1, r2]
  42f2d4: e2822004     	add	r2, r2, #4
  42f2d8: 1afffffa     	bne	0x42f2c8 <MenuManager::PostLoad()+0x310> @ imm = #-0x18
  42f2dc: eaffff84     	b	0x42f0f4 <MenuManager::PostLoad()+0x13c> @ imm = #-0x1f0
  42f2e0: ebfb7c0a     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x120fd8
  42f2e4: c8 5a 56 00  	.word	0x00565ac8
  42f2e8: ac 40 00 00  	.word	0x000040ac
  42f2ec: 68 b1 49 00  	.word	0x0049b168
  42f2f0: 04 b2 49 00  	.word	0x0049b204
  42f2f4: e0 b0 49 00  	.word	0x0049b0e0
  42f2f8: d4 b0 49 00  	.word	0x0049b0d4
  42f2fc: 20 37 00 00  	.word	0x00003720
  42f300: 84 08 00 00  	.word	0x00000884
