
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003596f8 <SceneManager::LoadScene(char const*, char const*, bool, bool)>:
  3596f8: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3596fc: e59f4308     	ldr	r4, [pc, #0x308]        @ 0x359a0c <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x314>
  359700: e59f7308     	ldr	r7, [pc, #0x308]        @ 0x359a10 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x318>
  359704: e2525000     	subs	r5, r2, #0
  359708: e08f4004     	add	r4, pc, r4
  35970c: e7942007     	ldr	r2, [r4, r7]
  359710: e24dd03c     	sub	sp, sp, #60
  359714: e58d0008     	str	r0, [sp, #0x8]
  359718: e5922000     	ldr	r2, [r2]
  35971c: e1a06001     	mov	r6, r1
  359720: e1a0b003     	mov	r11, r3
  359724: e5dd9060     	ldrb	r9, [sp, #0x60]
  359728: e58d2034     	str	r2, [sp, #0x34]
  35972c: 0a000002     	beq	0x35973c <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x44> @ imm = #0x8
  359730: e1d530d0     	ldrsb	r3, [r5]
  359734: e3530000     	cmp	r3, #0
  359738: 1a000094     	bne	0x359990 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x298> @ imm = #0x250
  35973c: e59f32d0     	ldr	r3, [pc, #0x2d0]        @ 0x359a14 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x31c>
  359740: e59f82d0     	ldr	r8, [pc, #0x2d0]        @ 0x359a18 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x320>
  359744: e1a01006     	mov	r1, r6
  359748: e7940003     	ldr	r0, [r4, r3]
  35974c: e3a02001     	mov	r2, #1
  359750: e7943008     	ldr	r3, [r4, r8]
  359754: e5900010     	ldr	r0, [r0, #0x10]
  359758: e5900010     	ldr	r0, [r0, #0x10]
  35975c: eb0b091c     	bl	0x61bbd4 <glitch::collada::CColladaDatabase::constructScene(glitch::video::IVideoDriver*, char const*, bool, glitch::collada::CColladaFactory*)> @ imm = #0x2c2470
  359760: e1a0a000     	mov	r10, r0
  359764: e35a0000     	cmp	r10, #0
  359768: 0a000010     	beq	0x3597b0 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0xb8> @ imm = #0x40
  35976c: e1a00006     	mov	r0, r6
  359770: ebfed1b7     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x4b924
  359774: e1a01006     	mov	r1, r6
  359778: e0862000     	add	r2, r6, r0
  35977c: e28a0f6f     	add	r0, r10, #444
  359780: ebfedc96     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x48da8
  359784: e3550000     	cmp	r5, #0
  359788: e28a3f75     	add	r3, r10, #468
  35978c: 0a000099     	beq	0x3599f8 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x300> @ imm = #0x264
  359790: e1a00005     	mov	r0, r5
  359794: e58d3004     	str	r3, [sp, #0x4]
  359798: ebfed1ad     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x4b94c
  35979c: e59d3004     	ldr	r3, [sp, #0x4]
  3597a0: e0852000     	add	r2, r5, r0
  3597a4: e1a01005     	mov	r1, r5
  3597a8: e1a00003     	mov	r0, r3
  3597ac: ebfedc8b     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x48dd4
  3597b0: e3590000     	cmp	r9, #0
  3597b4: 0a000003     	beq	0x3597c8 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0xd0> @ imm = #0xc
  3597b8: e35a0000     	cmp	r10, #0
  3597bc: 0a000001     	beq	0x3597c8 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0xd0> @ imm = #0x4
  3597c0: e1a0000a     	mov	r0, r10
  3597c4: eb000d10     	bl	0x35cc0c <RootSceneNode::ResetPositionFromFile()> @ imm = #0x3440
  3597c8: e2559000     	subs	r9, r5, #0
  3597cc: 13a09001     	movne	r9, #1
  3597d0: e35a0000     	cmp	r10, #0
  3597d4: 13550000     	cmpne	r5, #0
  3597d8: 1a00005d     	bne	0x359954 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x25c> @ imm = #0x174
  3597dc: e35a0000     	cmp	r10, #0
  3597e0: 0a00002e     	beq	0x3598a0 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x1a8> @ imm = #0xb8
  3597e4: e1a0000a     	mov	r0, r10
  3597e8: e3a01002     	mov	r1, #2
  3597ec: eb08f66a     	bl	0x59719c <glitch::scene::ISceneNode::setAutomaticCulling(glitch::scene::E_CULLING_TYPE)> @ imm = #0x23d9a8
  3597f0: e3590000     	cmp	r9, #0
  3597f4: 0a000027     	beq	0x359898 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x1a0> @ imm = #0x9c
  3597f8: e59a80f4     	ldr	r8, [r10, #0xf4]
  3597fc: e3580000     	cmp	r8, #0
  359800: 12488004     	subne	r8, r8, #4
  359804: e5983000     	ldr	r3, [r8]
  359808: e1a00008     	mov	r0, r8
  35980c: e1a0e00f     	mov	lr, pc
  359810: e593f024     	ldr	pc, [r3, #0x24]
  359814: e59f1200     	ldr	r1, [pc, #0x200]        @ 0x359a1c <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x324>
  359818: e08f1001     	add	r1, pc, r1
  35981c: ebfed4ec     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x4ac50
  359820: e3500000     	cmp	r0, #0
  359824: 0a00001b     	beq	0x359898 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x1a0> @ imm = #0x6c
  359828: e5b850f4     	ldr	r5, [r8, #0xf4]!
  35982c: e1550008     	cmp	r5, r8
  359830: 0a000018     	beq	0x359898 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x1a0> @ imm = #0x60
  359834: e59f31e4     	ldr	r3, [pc, #0x1e4]        @ 0x359a20 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x328>
  359838: e59f91e4     	ldr	r9, [pc, #0x1e4]        @ 0x359a24 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x32c>
  35983c: e08f3003     	add	r3, pc, r3
  359840: e58d300c     	str	r3, [sp, #0xc]
  359844: e59f31dc     	ldr	r3, [pc, #0x1dc]        @ 0x359a28 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x330>
  359848: e08f9009     	add	r9, pc, r9
  35984c: e08f3003     	add	r3, pc, r3
  359850: e58d3010     	str	r3, [sp, #0x10]
  359854: e59f31d0     	ldr	r3, [pc, #0x1d0]        @ 0x359a2c <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x334>
  359858: e08f3003     	add	r3, pc, r3
  35985c: e58d3014     	str	r3, [sp, #0x14]
  359860: e3550000     	cmp	r5, #0
  359864: 01a06005     	moveq	r6, r5
  359868: 12456004     	subne	r6, r5, #4
  35986c: e5963000     	ldr	r3, [r6]
  359870: e1a00006     	mov	r0, r6
  359874: e5955000     	ldr	r5, [r5]
  359878: e1a0e00f     	mov	lr, pc
  35987c: e593f024     	ldr	pc, [r3, #0x24]
  359880: e1a01009     	mov	r1, r9
  359884: ebfed4d2     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x4acb8
  359888: e3500000     	cmp	r0, #0
  35988c: 0a000013     	beq	0x3598e0 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x1e8> @ imm = #0x4c
  359890: e1580005     	cmp	r8, r5
  359894: 1afffff1     	bne	0x359860 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x168> @ imm = #-0x3c
  359898: e35b0000     	cmp	r11, #0
  35989c: 1a000007     	bne	0x3598c0 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x1c8> @ imm = #0x1c
  3598a0: e7943007     	ldr	r3, [r4, r7]
  3598a4: e59d2034     	ldr	r2, [sp, #0x34]
  3598a8: e1a0000a     	mov	r0, r10
  3598ac: e5933000     	ldr	r3, [r3]
  3598b0: e1520003     	cmp	r2, r3
  3598b4: 1a000053     	bne	0x359a08 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x310> @ imm = #0x14c
  3598b8: e28dd03c     	add	sp, sp, #60
  3598bc: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3598c0: e59d2008     	ldr	r2, [sp, #0x8]
  3598c4: e1a0100a     	mov	r1, r10
  3598c8: e5923004     	ldr	r3, [r2, #0x4]
  3598cc: e1a00003     	mov	r0, r3
  3598d0: e5933000     	ldr	r3, [r3]
  3598d4: e1a0e00f     	mov	lr, pc
  3598d8: e593f05c     	ldr	pc, [r3, #0x5c]
  3598dc: eaffffef     	b	0x3598a0 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x1a8> @ imm = #-0x44
  3598e0: e5963000     	ldr	r3, [r6]
  3598e4: e1a00006     	mov	r0, r6
  3598e8: e1a0e00f     	mov	lr, pc
  3598ec: e593f024     	ldr	pc, [r3, #0x24]
  3598f0: e59d100c     	ldr	r1, [sp, #0xc]
  3598f4: ebfed4b6     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x4ad28
  3598f8: e3500000     	cmp	r0, #0
  3598fc: 1affffe3     	bne	0x359890 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x198> @ imm = #-0x74
  359900: e5963000     	ldr	r3, [r6]
  359904: e1a00006     	mov	r0, r6
  359908: e1a0e00f     	mov	lr, pc
  35990c: e593f024     	ldr	pc, [r3, #0x24]
  359910: e59d1010     	ldr	r1, [sp, #0x10]
  359914: ebfed4ae     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x4ad48
  359918: e3500000     	cmp	r0, #0
  35991c: 1affffdb     	bne	0x359890 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x198> @ imm = #-0x94
  359920: e5963000     	ldr	r3, [r6]
  359924: e1a00006     	mov	r0, r6
  359928: e1a0e00f     	mov	lr, pc
  35992c: e593f024     	ldr	pc, [r3, #0x24]
  359930: e59d1014     	ldr	r1, [sp, #0x14]
  359934: ebfed4a6     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x4ad68
  359938: e3500000     	cmp	r0, #0
  35993c: 1affffd3     	bne	0x359890 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x198> @ imm = #-0xb4
  359940: e1a00006     	mov	r0, r6
  359944: e5963000     	ldr	r3, [r6]
  359948: e1a0e00f     	mov	lr, pc
  35994c: e593f068     	ldr	pc, [r3, #0x68]
  359950: eaffffce     	b	0x359890 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x198> @ imm = #-0xc8
  359954: e1a00006     	mov	r0, r6
  359958: e7941008     	ldr	r1, [r4, r8]
  35995c: eb0aff46     	bl	0x61967c <glitch::collada::CColladaDatabase::constructAnimator(char const*, glitch::collada::CColladaFactory*)> @ imm = #0x2bfd18
  359960: e2505000     	subs	r5, r0, #0
  359964: 0affff9c     	beq	0x3597dc <SceneManager::LoadScene(char const*, char const*, bool, bool)+0xe4> @ imm = #-0x190
  359968: e1a0000a     	mov	r0, r10
  35996c: e59a3000     	ldr	r3, [r10]
  359970: e1a01005     	mov	r1, r5
  359974: e1a0e00f     	mov	lr, pc
  359978: e593f06c     	ldr	pc, [r3, #0x6c]
  35997c: e5953000     	ldr	r3, [r5]
  359980: e513000c     	ldr	r0, [r3, #-0xc]
  359984: e0850000     	add	r0, r5, r0
  359988: ebff0efd     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x3c40c
  35998c: eaffff92     	b	0x3597dc <SceneManager::LoadScene(char const*, char const*, bool, bool)+0xe4> @ imm = #-0x1b8
  359990: e28dc01c     	add	r12, sp, #28
  359994: e1a01005     	mov	r1, r5
  359998: e28d2018     	add	r2, sp, #24
  35999c: e1a0000c     	mov	r0, r12
  3599a0: e58dc004     	str	r12, [sp, #0x4]
  3599a4: ebfee9d0     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x458c0
  3599a8: e59f1080     	ldr	r1, [pc, #0x80]         @ 0x359a30 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x338>
  3599ac: e59dc004     	ldr	r12, [sp, #0x4]
  3599b0: e59f8060     	ldr	r8, [pc, #0x60]         @ 0x359a18 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x320>
  3599b4: e08f1001     	add	r1, pc, r1
  3599b8: e1a0000c     	mov	r0, r12
  3599bc: e2812005     	add	r2, r1, #5
  3599c0: ebfedb8f     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0x491c4
  3599c4: e59f3048     	ldr	r3, [pc, #0x48]         @ 0x359a14 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x31c>
  3599c8: e1a01006     	mov	r1, r6
  3599cc: e59d2030     	ldr	r2, [sp, #0x30]
  3599d0: e7940003     	ldr	r0, [r4, r3]
  3599d4: e7943008     	ldr	r3, [r4, r8]
  3599d8: e5900010     	ldr	r0, [r0, #0x10]
  3599dc: e5900010     	ldr	r0, [r0, #0x10]
  3599e0: eb0b0ba4     	bl	0x61c878 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, char const*, char const*, glitch::collada::CColladaFactory*)> @ imm = #0x2c2e90
  3599e4: e59dc004     	ldr	r12, [sp, #0x4]
  3599e8: e1a0a000     	mov	r10, r0
  3599ec: e1a0000c     	mov	r0, r12
  3599f0: ebfefa17     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x417a4
  3599f4: eaffff5a     	b	0x359764 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x6c> @ imm = #-0x298
  3599f8: e59f2034     	ldr	r2, [pc, #0x34]         @ 0x359a34 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0x33c>
  3599fc: e08f2002     	add	r2, pc, r2
  359a00: e1a01002     	mov	r1, r2
  359a04: eaffff67     	b	0x3597a8 <SceneManager::LoadScene(char const*, char const*, bool, bool)+0xb0> @ imm = #-0x264
  359a08: ebfed240     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x4b700
  359a0c: 88 b3 63 00  	.word	0x0063b388
  359a10: ac 40 00 00  	.word	0x000040ac
  359a14: f4 37 00 00  	.word	0x000037f4
  359a18: 2c 0d 00 00  	.word	0x00000d2c
  359a1c: e0 73 56 00  	.word	0x005673e0
  359a20: d4 73 56 00  	.word	0x005673d4
  359a24: b8 73 56 00  	.word	0x005673b8
  359a28: cc 73 56 00  	.word	0x005673cc
  359a2c: c8 73 56 00  	.word	0x005673c8
  359a30: 3c 72 56 00  	.word	0x0056723c
  359a34: 0c 1e 57 00  	.word	0x00571e0c
