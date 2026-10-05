
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f3b40 <Level::LoadFile(std::string const&, std::string const&)>:
  3f3b40: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3f3b44: e59f55b8     	ldr	r5, [pc, #0x5b8]        @ 0x3f4104 <Level::LoadFile(std::string const&, std::string const&)+0x5c4>
  3f3b48: e59fa5b8     	ldr	r10, [pc, #0x5b8]       @ 0x3f4108 <Level::LoadFile(std::string const&, std::string const&)+0x5c8>
  3f3b4c: e1a0b001     	mov	r11, r1
  3f3b50: e08f5005     	add	r5, pc, r5
  3f3b54: e795300a     	ldr	r3, [r5, r10]
  3f3b58: e59f15ac     	ldr	r1, [pc, #0x5ac]        @ 0x3f410c <Level::LoadFile(std::string const&, std::string const&)+0x5cc>
  3f3b5c: e24dd0a4     	sub	sp, sp, #164
  3f3b60: e5933000     	ldr	r3, [r3]
  3f3b64: e28d8084     	add	r8, sp, #132
  3f3b68: e1a07000     	mov	r7, r0
  3f3b6c: e1a04002     	mov	r4, r2
  3f3b70: e08f1001     	add	r1, pc, r1
  3f3b74: e1a00008     	mov	r0, r8
  3f3b78: e28d2050     	add	r2, sp, #80
  3f3b7c: e58d309c     	str	r3, [sp, #0x9c]
  3f3b80: ebfc8159     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xdfa9c
  3f3b84: e5976140     	ldr	r6, [r7, #0x140]
  3f3b88: e3560000     	cmp	r6, #0
  3f3b8c: 0a000039     	beq	0x3f3c78 <Level::LoadFile(std::string const&, std::string const&)+0x138> @ imm = #0xe4
  3f3b90: e5963038     	ldr	r3, [r6, #0x38]
  3f3b94: e3530000     	cmp	r3, #0
  3f3b98: 0a0000f3     	beq	0x3f3f6c <Level::LoadFile(std::string const&, std::string const&)+0x42c> @ imm = #0x3cc
  3f3b9c: e5961044     	ldr	r1, [r6, #0x44]
  3f3ba0: e3510000     	cmp	r1, #0
  3f3ba4: 0a000010     	beq	0x3f3bec <Level::LoadFile(std::string const&, std::string const&)+0xac> @ imm = #0x40
  3f3ba8: e1a00007     	mov	r0, r7
  3f3bac: ebfff185     	bl	0x3f01c8 <Level::_LoadFromXML(TiXmlElement*)> @ imm = #-0x39ec
  3f3bb0: e5974140     	ldr	r4, [r7, #0x140]
  3f3bb4: e3a09000     	mov	r9, #0
  3f3bb8: e5940044     	ldr	r0, [r4, #0x44]
  3f3bbc: eb048228     	bl	0x514464 <TiXmlNode::NextSiblingElement() const> @ imm = #0x1208a0
  3f3bc0: e5840044     	str	r0, [r4, #0x44]
  3f3bc4: e1a00008     	mov	r0, r8
  3f3bc8: ebfc7f77     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe0224
  3f3bcc: e795300a     	ldr	r3, [r5, r10]
  3f3bd0: e59d209c     	ldr	r2, [sp, #0x9c]
  3f3bd4: e1a00009     	mov	r0, r9
  3f3bd8: e5933000     	ldr	r3, [r3]
  3f3bdc: e1520003     	cmp	r2, r3
  3f3be0: 1a000140     	bne	0x3f40e8 <Level::LoadFile(std::string const&, std::string const&)+0x5a8> @ imm = #0x500
  3f3be4: e28dd0a4     	add	sp, sp, #164
  3f3be8: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3f3bec: e5961040     	ldr	r1, [r6, #0x40]
  3f3bf0: e3510000     	cmp	r1, #0
  3f3bf4: 0a000010     	beq	0x3f3c3c <Level::LoadFile(std::string const&, std::string const&)+0xfc> @ imm = #0x40
  3f3bf8: e596003c     	ldr	r0, [r6, #0x3c]
  3f3bfc: eb0481ff     	bl	0x514400 <TiXmlNode::IterateChildren(TiXmlNode const*) const> @ imm = #0x1207fc
  3f3c00: e5860040     	str	r0, [r6, #0x40]
  3f3c04: e5977140     	ldr	r7, [r7, #0x140]
  3f3c08: e5976040     	ldr	r6, [r7, #0x40]
  3f3c0c: e3560000     	cmp	r6, #0
  3f3c10: 0a000007     	beq	0x3f3c34 <Level::LoadFile(std::string const&, std::string const&)+0xf4> @ imm = #0x1c
  3f3c14: e5943010     	ldr	r3, [r4, #0x10]
  3f3c18: e5960034     	ldr	r0, [r6, #0x34]
  3f3c1c: e5962030     	ldr	r2, [r6, #0x30]
  3f3c20: e5941014     	ldr	r1, [r4, #0x14]
  3f3c24: e0602002     	rsb	r2, r0, r2
  3f3c28: e0613003     	rsb	r3, r1, r3
  3f3c2c: e1520003     	cmp	r2, r3
  3f3c30: 0a000110     	beq	0x3f4078 <Level::LoadFile(std::string const&, std::string const&)+0x538> @ imm = #0x440
  3f3c34: e3a09000     	mov	r9, #0
  3f3c38: eaffffe1     	b	0x3f3bc4 <Level::LoadFile(std::string const&, std::string const&)+0x84> @ imm = #-0x7c
  3f3c3c: e1a00003     	mov	r0, r3
  3f3c40: e5933000     	ldr	r3, [r3]
  3f3c44: e1a0e00f     	mov	lr, pc
  3f3c48: e593f004     	ldr	pc, [r3, #0x4]
  3f3c4c: e5973140     	ldr	r3, [r7, #0x140]
  3f3c50: e3530000     	cmp	r3, #0
  3f3c54: 0a000003     	beq	0x3f3c68 <Level::LoadFile(std::string const&, std::string const&)+0x128> @ imm = #0xc
  3f3c58: e1a00003     	mov	r0, r3
  3f3c5c: e5933000     	ldr	r3, [r3]
  3f3c60: e1a0e00f     	mov	lr, pc
  3f3c64: e593f004     	ldr	pc, [r3, #0x4]
  3f3c68: e3a03000     	mov	r3, #0
  3f3c6c: e5873140     	str	r3, [r7, #0x140]
  3f3c70: e3a09001     	mov	r9, #1
  3f3c74: eaffffd2     	b	0x3f3bc4 <Level::LoadFile(std::string const&, std::string const&)+0x84> @ imm = #-0xb8
  3f3c78: e28d106c     	add	r1, sp, #108
  3f3c7c: e1a00001     	mov	r0, r1
  3f3c80: e58d101c     	str	r1, [sp, #0x1c]
  3f3c84: e3a01010     	mov	r1, #16
  3f3c88: e58d007c     	str	r0, [sp, #0x7c]
  3f3c8c: e58d0080     	str	r0, [sp, #0x80]
  3f3c90: ebfc7679     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0xe261c
  3f3c94: e28d3054     	add	r3, sp, #84
  3f3c98: e58d3014     	str	r3, [sp, #0x14]
  3f3c9c: e59d307c     	ldr	r3, [sp, #0x7c]
  3f3ca0: e59f1468     	ldr	r1, [pc, #0x468]        @ 0x3f4110 <Level::LoadFile(std::string const&, std::string const&)+0x5d0>
  3f3ca4: e28d204c     	add	r2, sp, #76
  3f3ca8: e5c36000     	strb	r6, [r3]
  3f3cac: e08f1001     	add	r1, pc, r1
  3f3cb0: e59d0014     	ldr	r0, [sp, #0x14]
  3f3cb4: ebfc810c     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xdfbd0
  3f3cb8: e59f3454     	ldr	r3, [pc, #0x454]        @ 0x3f4114 <Level::LoadFile(std::string const&, std::string const&)+0x5d4>
  3f3cbc: e59f1454     	ldr	r1, [pc, #0x454]        @ 0x3f4118 <Level::LoadFile(std::string const&, std::string const&)+0x5d8>
  3f3cc0: e59f9454     	ldr	r9, [pc, #0x454]        @ 0x3f411c <Level::LoadFile(std::string const&, std::string const&)+0x5dc>
  3f3cc4: e58d3018     	str	r3, [sp, #0x18]
  3f3cc8: e59f3450     	ldr	r3, [pc, #0x450]        @ 0x3f4120 <Level::LoadFile(std::string const&, std::string const&)+0x5e0>
  3f3ccc: e58d1028     	str	r1, [sp, #0x28]
  3f3cd0: e59f244c     	ldr	r2, [pc, #0x44c]        @ 0x3f4124 <Level::LoadFile(std::string const&, std::string const&)+0x5e4>
  3f3cd4: e08f3003     	add	r3, pc, r3
  3f3cd8: e58d3020     	str	r3, [sp, #0x20]
  3f3cdc: e59d1020     	ldr	r1, [sp, #0x20]
  3f3ce0: e59f3440     	ldr	r3, [pc, #0x440]        @ 0x3f4128 <Level::LoadFile(std::string const&, std::string const&)+0x5e8>
  3f3ce4: e58d8038     	str	r8, [sp, #0x38]
  3f3ce8: e59d801c     	ldr	r8, [sp, #0x1c]
  3f3cec: e08f9009     	add	r9, pc, r9
  3f3cf0: e08f3003     	add	r3, pc, r3
  3f3cf4: e2811002     	add	r1, r1, #2
  3f3cf8: e58d703c     	str	r7, [sp, #0x3c]
  3f3cfc: e58d2024     	str	r2, [sp, #0x24]
  3f3d00: e58d3030     	str	r3, [sp, #0x30]
  3f3d04: e28990a0     	add	r9, r9, #160
  3f3d08: e58d102c     	str	r1, [sp, #0x2c]
  3f3d0c: e1a07005     	mov	r7, r5
  3f3d10: e58da034     	str	r10, [sp, #0x34]
  3f3d14: e59d2028     	ldr	r2, [sp, #0x28]
  3f3d18: e1a00008     	mov	r0, r8
  3f3d1c: e08f1002     	add	r1, pc, r2
  3f3d20: e1a02001     	mov	r2, r1
  3f3d24: ebfc732d     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0xe334c
  3f3d28: e59d1024     	ldr	r1, [sp, #0x24]
  3f3d2c: e08f3001     	add	r3, pc, r1
  3f3d30: e2833090     	add	r3, r3, #144
  3f3d34: e7934006     	ldr	r4, [r3, r6]
  3f3d38: e1a00004     	mov	r0, r4
  3f3d3c: ebfc6844     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0xe5ef0
  3f3d40: e1a01004     	mov	r1, r4
  3f3d44: e0842000     	add	r2, r4, r0
  3f3d48: e1a00008     	mov	r0, r8
  3f3d4c: ebfc72ac     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0xe3550
  3f3d50: e59b1014     	ldr	r1, [r11, #0x14]
  3f3d54: e59b2010     	ldr	r2, [r11, #0x10]
  3f3d58: e1a00008     	mov	r0, r8
  3f3d5c: ebfc72a8     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0xe3560
  3f3d60: e59d2018     	ldr	r2, [sp, #0x18]
  3f3d64: e59d1080     	ldr	r1, [sp, #0x80]
  3f3d68: e7974002     	ldr	r4, [r7, r2]
  3f3d6c: e1a00004     	mov	r0, r4
  3f3d70: ebfcb240     	bl	0x320678 <Application::IsUsingUncompiledData(char const*) const> @ imm = #-0xd3700
  3f3d74: e3500000     	cmp	r0, #0
  3f3d78: 1a000049     	bne	0x3f3ea4 <Level::LoadFile(std::string const&, std::string const&)+0x364> @ imm = #0x124
  3f3d7c: e59d307c     	ldr	r3, [sp, #0x7c]
  3f3d80: e3a04000     	mov	r4, #0
  3f3d84: e28da048     	add	r10, sp, #72
  3f3d88: e58d3010     	str	r3, [sp, #0x10]
  3f3d8c: e7995004     	ldr	r5, [r9, r4]
  3f3d90: e1a00005     	mov	r0, r5
  3f3d94: ebfc682e     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0xe5f48
  3f3d98: e59d1010     	ldr	r1, [sp, #0x10]
  3f3d9c: e1a02000     	mov	r2, r0
  3f3da0: e59d0080     	ldr	r0, [sp, #0x80]
  3f3da4: e0513000     	subs	r3, r1, r0
  3f3da8: 1a00002a     	bne	0x3f3e58 <Level::LoadFile(std::string const&, std::string const&)+0x318> @ imm = #0xa8
  3f3dac: e3520000     	cmp	r2, #0
  3f3db0: 1a000037     	bne	0x3f3e94 <Level::LoadFile(std::string const&, std::string const&)+0x354> @ imm = #0xdc
  3f3db4: e1a01002     	mov	r1, r2
  3f3db8: e59f336c     	ldr	r3, [pc, #0x36c]        @ 0x3f412c <Level::LoadFile(std::string const&, std::string const&)+0x5ec>
  3f3dbc: e3a0c000     	mov	r12, #0
  3f3dc0: e1a00008     	mov	r0, r8
  3f3dc4: e08f3003     	add	r3, pc, r3
  3f3dc8: e58dc000     	str	r12, [sp]
  3f3dcc: ebfff889     	bl	0x3f1ff8 <std::string::replace(unsigned int, unsigned int, char const*, unsigned int)> @ imm = #-0x1ddc
  3f3dd0: e59d1020     	ldr	r1, [sp, #0x20]
  3f3dd4: e59d202c     	ldr	r2, [sp, #0x2c]
  3f3dd8: e59d0014     	ldr	r0, [sp, #0x14]
  3f3ddc: ebfc7288     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0xe35e0
  3f3de0: e59d1080     	ldr	r1, [sp, #0x80]
  3f3de4: e59d207c     	ldr	r2, [sp, #0x7c]
  3f3de8: e59d0014     	ldr	r0, [sp, #0x14]
  3f3dec: ebfc7284     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0xe35f0
  3f3df0: e59d3018     	ldr	r3, [sp, #0x18]
  3f3df4: e3a02000     	mov	r2, #0
  3f3df8: e59d1080     	ldr	r1, [sp, #0x80]
  3f3dfc: e7974003     	ldr	r4, [r7, r3]
  3f3e00: e1a03002     	mov	r3, r2
  3f3e04: e5940010     	ldr	r0, [r4, #0x10]
  3f3e08: e590c034     	ldr	r12, [r0, #0x34]
  3f3e0c: e1a0000c     	mov	r0, r12
  3f3e10: e59cc000     	ldr	r12, [r12]
  3f3e14: e1a0e00f     	mov	lr, pc
  3f3e18: e59cf088     	ldr	pc, [r12, #0x88]
  3f3e1c: e3500000     	cmp	r0, #0
  3f3e20: e58d0040     	str	r0, [sp, #0x40]
  3f3e24: 1a0000a3     	bne	0x3f40b8 <Level::LoadFile(std::string const&, std::string const&)+0x578> @ imm = #0x28c
  3f3e28: e2866004     	add	r6, r6, #4
  3f3e2c: e3560010     	cmp	r6, #16
  3f3e30: 1affffb7     	bne	0x3f3d14 <Level::LoadFile(std::string const&, std::string const&)+0x1d4> @ imm = #-0x124
  3f3e34: e59da034     	ldr	r10, [sp, #0x34]
  3f3e38: e59d8038     	ldr	r8, [sp, #0x38]
  3f3e3c: e1a05007     	mov	r5, r7
  3f3e40: e3a09001     	mov	r9, #1
  3f3e44: e59d0014     	ldr	r0, [sp, #0x14]
  3f3e48: ebfc7ed7     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe04a4
  3f3e4c: e59d001c     	ldr	r0, [sp, #0x1c]
  3f3e50: ebfc7ed5     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe04ac
  3f3e54: eaffff5a     	b	0x3f3bc4 <Level::LoadFile(std::string const&, std::string const&)+0x84> @ imm = #-0x298
  3f3e58: e1520003     	cmp	r2, r3
  3f3e5c: 8a00000c     	bhi	0x3f3e94 <Level::LoadFile(std::string const&, std::string const&)+0x354> @ imm = #0x30
  3f3e60: e0853002     	add	r3, r5, r2
  3f3e64: e59d1010     	ldr	r1, [sp, #0x10]
  3f3e68: e1a02005     	mov	r2, r5
  3f3e6c: e58da000     	str	r10, [sp]
  3f3e70: ebfd6bad     	bl	0x34ed2c <char const* std::search<char const*, char const*, std::priv::_Eq_traits<std::char_traits<char>>>(char const*, char const*, char const*, char const*, std::priv::_Eq_traits<std::char_traits<char>>)> @ imm = #-0xa514c
  3f3e74: e59d207c     	ldr	r2, [sp, #0x7c]
  3f3e78: e1500002     	cmp	r0, r2
  3f3e7c: e58d2010     	str	r2, [sp, #0x10]
  3f3e80: 0a000003     	beq	0x3f3e94 <Level::LoadFile(std::string const&, std::string const&)+0x354> @ imm = #0xc
  3f3e84: e59d3080     	ldr	r3, [sp, #0x80]
  3f3e88: e0633000     	rsb	r3, r3, r0
  3f3e8c: e3730001     	cmn	r3, #1
  3f3e90: 1a000095     	bne	0x3f40ec <Level::LoadFile(std::string const&, std::string const&)+0x5ac> @ imm = #0x254
  3f3e94: e2844004     	add	r4, r4, #4
  3f3e98: e3540010     	cmp	r4, #16
  3f3e9c: 1affffba     	bne	0x3f3d8c <Level::LoadFile(std::string const&, std::string const&)+0x24c> @ imm = #-0x118
  3f3ea0: eaffffca     	b	0x3f3dd0 <Level::LoadFile(std::string const&, std::string const&)+0x290> @ imm = #-0xd8
  3f3ea4: e59d1030     	ldr	r1, [sp, #0x30]
  3f3ea8: e59d0014     	ldr	r0, [sp, #0x14]
  3f3eac: ebfff733     	bl	0x3f1b80 <std::string::append(char const*)> @ imm = #-0x2334
  3f3eb0: e59d1080     	ldr	r1, [sp, #0x80]
  3f3eb4: e59d207c     	ldr	r2, [sp, #0x7c]
  3f3eb8: e59d0014     	ldr	r0, [sp, #0x14]
  3f3ebc: ebfc7250     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0xe36c0
  3f3ec0: e5943010     	ldr	r3, [r4, #0x10]
  3f3ec4: e3a02000     	mov	r2, #0
  3f3ec8: e59d1080     	ldr	r1, [sp, #0x80]
  3f3ecc: e593c034     	ldr	r12, [r3, #0x34]
  3f3ed0: e1a03002     	mov	r3, r2
  3f3ed4: e1a0000c     	mov	r0, r12
  3f3ed8: e59cc000     	ldr	r12, [r12]
  3f3edc: e1a0e00f     	mov	lr, pc
  3f3ee0: e59cf088     	ldr	pc, [r12, #0x88]
  3f3ee4: e3500000     	cmp	r0, #0
  3f3ee8: e58d0044     	str	r0, [sp, #0x44]
  3f3eec: 0affffa2     	beq	0x3f3d7c <Level::LoadFile(std::string const&, std::string const&)+0x23c> @ imm = #-0x178
  3f3ef0: e3a01000     	mov	r1, #0
  3f3ef4: e3a00050     	mov	r0, #80
  3f3ef8: e1a05007     	mov	r5, r7
  3f3efc: e59da034     	ldr	r10, [sp, #0x34]
  3f3f00: e59d703c     	ldr	r7, [sp, #0x3c]
  3f3f04: e59d8038     	ldr	r8, [sp, #0x38]
  3f3f08: ebfc7198     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xe39a0
  3f3f0c: e28db0a0     	add	r11, sp, #160
  3f3f10: e59f3218     	ldr	r3, [pc, #0x218]        @ 0x3f4130 <Level::LoadFile(std::string const&, std::string const&)+0x5f0>
  3f3f14: e53b105c     	ldr	r1, [r11, #-0x5c]!
  3f3f18: e1a06000     	mov	r6, r0
  3f3f1c: e7953003     	ldr	r3, [r5, r3]
  3f3f20: e1a00006     	mov	r0, r6
  3f3f24: e3a09000     	mov	r9, #0
  3f3f28: e2833008     	add	r3, r3, #8
  3f3f2c: e4803008     	str	r3, [r0], #8
  3f3f30: ebfc8ce8     	bl	0x3172d8 <StreamBuffer::StreamBuffer(IStreamBase*)> @ imm = #-0xdcc60
  3f3f34: e5869038     	str	r9, [r6, #0x38]
  3f3f38: e586903c     	str	r9, [r6, #0x3c]
  3f3f3c: e5869040     	str	r9, [r6, #0x40]
  3f3f40: e5869044     	str	r9, [r6, #0x44]
  3f3f44: e5c69048     	strb	r9, [r6, #0x48]
  3f3f48: e5876140     	str	r6, [r7, #0x140]
  3f3f4c: e5943010     	ldr	r3, [r4, #0x10]
  3f3f50: e1a0100b     	mov	r1, r11
  3f3f54: e5933034     	ldr	r3, [r3, #0x34]
  3f3f58: e1a00003     	mov	r0, r3
  3f3f5c: e5933000     	ldr	r3, [r3]
  3f3f60: e1a0e00f     	mov	lr, pc
  3f3f64: e593f078     	ldr	pc, [r3, #0x78]
  3f3f68: eaffffb5     	b	0x3f3e44 <Level::LoadFile(std::string const&, std::string const&)+0x304> @ imm = #-0x12c
  3f3f6c: e1a01003     	mov	r1, r3
  3f3f70: e3a00070     	mov	r0, #112
  3f3f74: ebfc717d     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xe3a0c
  3f3f78: e1a06000     	mov	r6, r0
  3f3f7c: eb048bd0     	bl	0x516ec4 <TiXmlDocument::TiXmlDocument()> @ imm = #0x122f40
  3f3f80: e5973140     	ldr	r3, [r7, #0x140]
  3f3f84: e5836038     	str	r6, [r3, #0x38]
  3f3f88: e5976140     	ldr	r6, [r7, #0x140]
  3f3f8c: e5d63034     	ldrb	r3, [r6, #0x34]
  3f3f90: e5969038     	ldr	r9, [r6, #0x38]
  3f3f94: e3530000     	cmp	r3, #0
  3f3f98: 1a000016     	bne	0x3f3ff8 <Level::LoadFile(std::string const&, std::string const&)+0x4b8> @ imm = #0x58
  3f3f9c: e59f2190     	ldr	r2, [pc, #0x190]        @ 0x3f4134 <Level::LoadFile(std::string const&, std::string const&)+0x5f4>
  3f3fa0: e7952002     	ldr	r2, [r5, r2]
  3f3fa4: e5922000     	ldr	r2, [r2]
  3f3fa8: e3520002     	cmp	r2, #2
  3f3fac: 05833000     	streq	r3, [r3]
  3f3fb0: 01a02006     	moveq	r2, r6
  3f3fb4: 0a000010     	beq	0x3f3ffc <Level::LoadFile(std::string const&, std::string const&)+0x4bc> @ imm = #0x40
  3f3fb8: e3520001     	cmp	r2, #1
  3f3fbc: 1a00000d     	bne	0x3f3ff8 <Level::LoadFile(std::string const&, std::string const&)+0x4b8> @ imm = #0x34
  3f3fc0: e59f0170     	ldr	r0, [pc, #0x170]        @ 0x3f4138 <Level::LoadFile(std::string const&, std::string const&)+0x5f8>
  3f3fc4: e59f1170     	ldr	r1, [pc, #0x170]        @ 0x3f413c <Level::LoadFile(std::string const&, std::string const&)+0x5fc>
  3f3fc8: e59f2170     	ldr	r2, [pc, #0x170]        @ 0x3f4140 <Level::LoadFile(std::string const&, std::string const&)+0x600>
  3f3fcc: e7950000     	ldr	r0, [r5, r0]
  3f3fd0: e59f316c     	ldr	r3, [pc, #0x16c]        @ 0x3f4144 <Level::LoadFile(std::string const&, std::string const&)+0x604>
  3f3fd4: e08f2002     	add	r2, pc, r2
  3f3fd8: e3a0c082     	mov	r12, #130
  3f3fdc: e08f1001     	add	r1, pc, r1
  3f3fe0: e28000a8     	add	r0, r0, #168
  3f3fe4: e08f3003     	add	r3, pc, r3
  3f3fe8: e58dc000     	str	r12, [sp]
  3f3fec: ebfc6804     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xe5ff0
  3f3ff0: e5972140     	ldr	r2, [r7, #0x140]
  3f3ff4: ea000000     	b	0x3f3ffc <Level::LoadFile(std::string const&, std::string const&)+0x4bc> @ imm = #0x0
  3f3ff8: e1a02006     	mov	r2, r6
  3f3ffc: e5963024     	ldr	r3, [r6, #0x24]
  3f4000: e1a00009     	mov	r0, r9
  3f4004: e5922030     	ldr	r2, [r2, #0x30]
  3f4008: e5931000     	ldr	r1, [r3]
  3f400c: e3a03000     	mov	r3, #0
  3f4010: eb048924     	bl	0x5164a8 <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)> @ imm = #0x122490
  3f4014: e3500000     	cmp	r0, #0
  3f4018: 1a000020     	bne	0x3f40a0 <Level::LoadFile(std::string const&, std::string const&)+0x560> @ imm = #0x80
  3f401c: e59f3110     	ldr	r3, [pc, #0x110]        @ 0x3f4134 <Level::LoadFile(std::string const&, std::string const&)+0x5f4>
  3f4020: e7953003     	ldr	r3, [r5, r3]
  3f4024: e5934000     	ldr	r4, [r3]
  3f4028: e3540002     	cmp	r4, #2
  3f402c: 05800000     	streq	r0, [r0]
  3f4030: 03a09001     	moveq	r9, #1
  3f4034: 0afffee2     	beq	0x3f3bc4 <Level::LoadFile(std::string const&, std::string const&)+0x84> @ imm = #-0x478
  3f4038: e3540001     	cmp	r4, #1
  3f403c: 1affff0b     	bne	0x3f3c70 <Level::LoadFile(std::string const&, std::string const&)+0x130> @ imm = #-0x3d4
  3f4040: e59f00f0     	ldr	r0, [pc, #0xf0]         @ 0x3f4138 <Level::LoadFile(std::string const&, std::string const&)+0x5f8>
  3f4044: e59f10fc     	ldr	r1, [pc, #0xfc]         @ 0x3f4148 <Level::LoadFile(std::string const&, std::string const&)+0x608>
  3f4048: e59f20fc     	ldr	r2, [pc, #0xfc]         @ 0x3f414c <Level::LoadFile(std::string const&, std::string const&)+0x60c>
  3f404c: e7950000     	ldr	r0, [r5, r0]
  3f4050: e59f30f8     	ldr	r3, [pc, #0xf8]         @ 0x3f4150 <Level::LoadFile(std::string const&, std::string const&)+0x610>
  3f4054: e3a0ce8b     	mov	r12, #2224
  3f4058: e08f1001     	add	r1, pc, r1
  3f405c: e28000a8     	add	r0, r0, #168
  3f4060: e08f2002     	add	r2, pc, r2
  3f4064: e08f3003     	add	r3, pc, r3
  3f4068: e58dc000     	str	r12, [sp]
  3f406c: e1a09004     	mov	r9, r4
  3f4070: ebfc67e3     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xe6074
  3f4074: eafffed2     	b	0x3f3bc4 <Level::LoadFile(std::string const&, std::string const&)+0x84> @ imm = #-0x4b8
  3f4078: ebfc6958     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0xe5aa0
  3f407c: e2509000     	subs	r9, r0, #0
  3f4080: 1afffeeb     	bne	0x3f3c34 <Level::LoadFile(std::string const&, std::string const&)+0xf4> @ imm = #-0x454
  3f4084: e5963000     	ldr	r3, [r6]
  3f4088: e1a00006     	mov	r0, r6
  3f408c: e1a0e00f     	mov	lr, pc
  3f4090: e593f02c     	ldr	pc, [r3, #0x2c]
  3f4094: eb0480dd     	bl	0x514410 <TiXmlNode::FirstChildElement() const> @ imm = #0x120374
  3f4098: e5870044     	str	r0, [r7, #0x44]
  3f409c: eafffec8     	b	0x3f3bc4 <Level::LoadFile(std::string const&, std::string const&)+0x84> @ imm = #-0x4e0
  3f40a0: e5973140     	ldr	r3, [r7, #0x140]
  3f40a4: e3a01000     	mov	r1, #0
  3f40a8: e5932038     	ldr	r2, [r3, #0x38]
  3f40ac: e583203c     	str	r2, [r3, #0x3c]
  3f40b0: e5976140     	ldr	r6, [r7, #0x140]
  3f40b4: eafffecf     	b	0x3f3bf8 <Level::LoadFile(std::string const&, std::string const&)+0xb8> @ imm = #-0x4c4
  3f40b8: e3a01000     	mov	r1, #0
  3f40bc: e3a00050     	mov	r0, #80
  3f40c0: e1a05007     	mov	r5, r7
  3f40c4: e59da034     	ldr	r10, [sp, #0x34]
  3f40c8: e59d703c     	ldr	r7, [sp, #0x3c]
  3f40cc: e59d8038     	ldr	r8, [sp, #0x38]
  3f40d0: ebfc7126     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xe3b68
  3f40d4: e28db0a0     	add	r11, sp, #160
  3f40d8: e1a06000     	mov	r6, r0
  3f40dc: e59f304c     	ldr	r3, [pc, #0x4c]         @ 0x3f4130 <Level::LoadFile(std::string const&, std::string const&)+0x5f0>
  3f40e0: e53b1060     	ldr	r1, [r11, #-0x60]!
  3f40e4: eaffff8c     	b	0x3f3f1c <Level::LoadFile(std::string const&, std::string const&)+0x3dc> @ imm = #-0x1d0
  3f40e8: ebfc6888     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xe5de0
  3f40ec: e1a00005     	mov	r0, r5
  3f40f0: e58d300c     	str	r3, [sp, #0xc]
  3f40f4: ebfc6756     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0xe62a8
  3f40f8: e59d100c     	ldr	r1, [sp, #0xc]
  3f40fc: e1a02000     	mov	r2, r0
  3f4100: eaffff2c     	b	0x3f3db8 <Level::LoadFile(std::string const&, std::string const&)+0x278> @ imm = #-0x350
  3f4104: 40 0f 5a 00  	.word	0x005a0f40
  3f4108: ac 40 00 00  	.word	0x000040ac
  3f410c: 40 2c 4d 00  	.word	0x004d2c40
  3f4110: 5c 7b 4d 00  	.word	0x004d7b5c
  3f4114: f4 37 00 00  	.word	0x000037f4
  3f4118: ec 7a 4d 00  	.word	0x004d7aec
  3f411c: c0 2c 56 00  	.word	0x00562cc0
  3f4120: e4 2a 4d 00  	.word	0x004d2ae4
  3f4124: 80 2c 56 00  	.word	0x00562c80
  3f4128: c8 2a 4d 00  	.word	0x004d2ac8
  3f412c: 44 7a 4d 00  	.word	0x004d7a44
  3f4130: 20 22 00 00  	.word	0x00002220
  3f4134: c0 39 00 00  	.word	0x000039c0
  3f4138: c0 19 00 00  	.word	0x000019c0
  3f413c: fc a3 4c 00  	.word	0x004ca3fc
  3f4140: f4 a5 4c 00  	.word	0x004ca5f4
  3f4144: dc 27 4d 00  	.word	0x004d27dc
  3f4148: 80 a3 4c 00  	.word	0x004ca380
  3f414c: a0 27 4d 00  	.word	0x004d27a0
  3f4150: ac 24 4d 00  	.word	0x004d24ac
