
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00490bdc <rnd::Rule::LoadFromXml(TiXmlNode*)>:
  490bdc: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  490be0: e59f869c     	ldr	r8, [pc, #0x69c]        @ 0x491284 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6a8>
  490be4: e59fb69c     	ldr	r11, [pc, #0x69c]       @ 0x491288 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6ac>
  490be8: e24ddf4d     	sub	sp, sp, #308
  490bec: e08f8008     	add	r8, pc, r8
  490bf0: e798300b     	ldr	r3, [r8, r11]
  490bf4: e2517000     	subs	r7, r1, #0
  490bf8: e1a04000     	mov	r4, r0
  490bfc: e5933000     	ldr	r3, [r3]
  490c00: e58d312c     	str	r3, [sp, #0x12c]
  490c04: 0a0000ce     	beq	0x490f44 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x368> @ imm = #0x338
  490c08: e5973000     	ldr	r3, [r7]
  490c0c: e1a00007     	mov	r0, r7
  490c10: e1a0e00f     	mov	lr, pc
  490c14: e593f02c     	ldr	pc, [r3, #0x2c]
  490c18: e59f166c     	ldr	r1, [pc, #0x66c]        @ 0x49128c <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6b0>
  490c1c: e08f1001     	add	r1, pc, r1
  490c20: eb021012     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x84048
  490c24: e2506000     	subs	r6, r0, #0
  490c28: 0a0000c5     	beq	0x490f44 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x368> @ imm = #0x314
  490c2c: e28d2f45     	add	r2, sp, #276
  490c30: e58d2008     	str	r2, [sp, #0x8]
  490c34: e1a00002     	mov	r0, r2
  490c38: e1a01006     	mov	r1, r6
  490c3c: e28d2050     	add	r2, sp, #80
  490c40: ebfa0d29     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x17cb5c
  490c44: e59d0128     	ldr	r0, [sp, #0x128]
  490c48: e1d030d0     	ldrsb	r3, [r0]
  490c4c: e3530023     	cmp	r3, #35
  490c50: 0a0000c6     	beq	0x490f70 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x394> @ imm = #0x318
  490c54: e59f3634     	ldr	r3, [pc, #0x634]        @ 0x491290 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6b4>
  490c58: e59f2634     	ldr	r2, [pc, #0x634]        @ 0x491294 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6b8>
  490c5c: e1a00006     	mov	r0, r6
  490c60: e58d300c     	str	r3, [sp, #0xc]
  490c64: e59f362c     	ldr	r3, [pc, #0x62c]        @ 0x491298 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6bc>
  490c68: e3a0102c     	mov	r1, #44
  490c6c: e58d2010     	str	r2, [sp, #0x10]
  490c70: e08f3003     	add	r3, pc, r3
  490c74: e58d3014     	str	r3, [sp, #0x14]
  490c78: e59f361c     	ldr	r3, [pc, #0x61c]        @ 0x49129c <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6c0>
  490c7c: e1a0a007     	mov	r10, r7
  490c80: e2849070     	add	r9, r4, #112
  490c84: e08f3003     	add	r3, pc, r3
  490c88: e58d3018     	str	r3, [sp, #0x18]
  490c8c: e59f360c     	ldr	r3, [pc, #0x60c]        @ 0x4912a0 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6c4>
  490c90: e28d5084     	add	r5, sp, #132
  490c94: e08f3003     	add	r3, pc, r3
  490c98: e58d301c     	str	r3, [sp, #0x1c]
  490c9c: ebf9f7e1     	bl	0x30ec28 <.plt+0xeb4>   @ imm = #-0x18207c
  490ca0: e2507000     	subs	r7, r0, #0
  490ca4: 0a000026     	beq	0x490d44 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x168> @ imm = #0x98
  490ca8: e1a01006     	mov	r1, r6
  490cac: e1a00005     	mov	r0, r5
  490cb0: e1a02007     	mov	r2, r7
  490cb4: e58d5094     	str	r5, [sp, #0x94]
  490cb8: e58d5098     	str	r5, [sp, #0x98]
  490cbc: ebfa0289     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0x17f5dc
  490cc0: e5943004     	ldr	r3, [r4, #0x4]
  490cc4: e59d1098     	ldr	r1, [sp, #0x98]
  490cc8: e593008c     	ldr	r0, [r3, #0x8c]
  490ccc: ebffce3f     	bl	0x4845d0 <rnd::RandomGenerator::ValidBlock(char const*)> @ imm = #-0xc704
  490cd0: e3500000     	cmp	r0, #0
  490cd4: 1a000007     	bne	0x490cf8 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x11c> @ imm = #0x1c
  490cd8: e59d200c     	ldr	r2, [sp, #0xc]
  490cdc: e7983002     	ldr	r3, [r8, r2]
  490ce0: e5933000     	ldr	r3, [r3]
  490ce4: e3530002     	cmp	r3, #2
  490ce8: 05800000     	streq	r0, [r0]
  490cec: 0a000001     	beq	0x490cf8 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x11c> @ imm = #0x4
  490cf0: e3530001     	cmp	r3, #1
  490cf4: 0a00010c     	beq	0x49112c <rnd::Rule::LoadFromXml(TiXmlNode*)+0x550> @ imm = #0x430
  490cf8: e1a00009     	mov	r0, r9
  490cfc: e1a01005     	mov	r1, r5
  490d00: ebfa6b74     	bl	0x32bad8 <std::vector<std::string, std::allocator<std::string>>::push_back(std::string const&)> @ imm = #-0x165230
  490d04: e59d0098     	ldr	r0, [sp, #0x98]
  490d08: e1500005     	cmp	r0, r5
  490d0c: 0a000006     	beq	0x490d2c <rnd::Rule::LoadFromXml(TiXmlNode*)+0x150> @ imm = #0x18
  490d10: e3500000     	cmp	r0, #0
  490d14: 0a000004     	beq	0x490d2c <rnd::Rule::LoadFromXml(TiXmlNode*)+0x150> @ imm = #0x10
  490d18: e59d1084     	ldr	r1, [sp, #0x84]
  490d1c: e0601001     	rsb	r1, r0, r1
  490d20: e3510080     	cmp	r1, #128
  490d24: 8a00008e     	bhi	0x490f64 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x388> @ imm = #0x238
  490d28: eb09e074     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x2781d0
  490d2c: e1a06007     	mov	r6, r7
  490d30: e1a00006     	mov	r0, r6
  490d34: e3a0102c     	mov	r1, #44
  490d38: ebf9f7ba     	bl	0x30ec28 <.plt+0xeb4>   @ imm = #-0x182118
  490d3c: e2507000     	subs	r7, r0, #0
  490d40: 1affffd8     	bne	0x490ca8 <rnd::Rule::LoadFromXml(TiXmlNode*)+0xcc> @ imm = #-0xa0
  490d44: e5943004     	ldr	r3, [r4, #0x4]
  490d48: e1a01006     	mov	r1, r6
  490d4c: e1a0700a     	mov	r7, r10
  490d50: e593008c     	ldr	r0, [r3, #0x8c]
  490d54: ebffce1d     	bl	0x4845d0 <rnd::RandomGenerator::ValidBlock(char const*)> @ imm = #-0xc78c
  490d58: e3500000     	cmp	r0, #0
  490d5c: 0a0000dd     	beq	0x4910d8 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x4fc> @ imm = #0x374
  490d60: e28d506c     	add	r5, sp, #108
  490d64: e1a01006     	mov	r1, r6
  490d68: e28d204c     	add	r2, sp, #76
  490d6c: e1a00005     	mov	r0, r5
  490d70: ebfa0cdd     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x17cc8c
  490d74: e1a00009     	mov	r0, r9
  490d78: e1a01005     	mov	r1, r5
  490d7c: ebfa6b55     	bl	0x32bad8 <std::vector<std::string, std::allocator<std::string>>::push_back(std::string const&)> @ imm = #-0x1652ac
  490d80: e1a00005     	mov	r0, r5
  490d84: ebfa1d32     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x178b38
  490d88: e59d0128     	ldr	r0, [sp, #0x128]
  490d8c: e59d2008     	ldr	r2, [sp, #0x8]
  490d90: e1500002     	cmp	r0, r2
  490d94: 0a000006     	beq	0x490db4 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x1d8> @ imm = #0x18
  490d98: e3500000     	cmp	r0, #0
  490d9c: 0a000004     	beq	0x490db4 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x1d8> @ imm = #0x10
  490da0: e59d1114     	ldr	r1, [sp, #0x114]
  490da4: e0601001     	rsb	r1, r0, r1
  490da8: e3510080     	cmp	r1, #128
  490dac: 8a0000e8     	bhi	0x491154 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x578> @ imm = #0x3a0
  490db0: eb09e052     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x278148
  490db4: e5973000     	ldr	r3, [r7]
  490db8: e1a00007     	mov	r0, r7
  490dbc: e1a0e00f     	mov	lr, pc
  490dc0: e593f02c     	ldr	pc, [r3, #0x2c]
  490dc4: e59f14d8     	ldr	r1, [pc, #0x4d8]        @ 0x4912a4 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6c8>
  490dc8: e08f1001     	add	r1, pc, r1
  490dcc: eb020fa7     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x83e9c
  490dd0: e2505000     	subs	r5, r0, #0
  490dd4: 0a000004     	beq	0x490dec <rnd::Rule::LoadFromXml(TiXmlNode*)+0x210> @ imm = #0x10
  490dd8: ebf9f41d     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x182f8c
  490ddc: e1a01005     	mov	r1, r5
  490de0: e0852000     	add	r2, r5, r0
  490de4: e2840050     	add	r0, r4, #80
  490de8: ebf9fefc     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x180410
  490dec: e5973000     	ldr	r3, [r7]
  490df0: e1a00007     	mov	r0, r7
  490df4: e1a0e00f     	mov	lr, pc
  490df8: e593f02c     	ldr	pc, [r3, #0x2c]
  490dfc: e59f14a4     	ldr	r1, [pc, #0x4a4]        @ 0x4912a8 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6cc>
  490e00: e08f1001     	add	r1, pc, r1
  490e04: eb020f99     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x83e64
  490e08: e250a000     	subs	r10, r0, #0
  490e0c: 0a000016     	beq	0x490e6c <rnd::Rule::LoadFromXml(TiXmlNode*)+0x290> @ imm = #0x58
  490e10: e5940004     	ldr	r0, [r4, #0x4]
  490e14: ebffebb6     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x5128
  490e18: e28d5054     	add	r5, sp, #84
  490e1c: e1a06000     	mov	r6, r0
  490e20: e1a0100a     	mov	r1, r10
  490e24: e28d2048     	add	r2, sp, #72
  490e28: e1a00005     	mov	r0, r5
  490e2c: ebfa0cae     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x17cd48
  490e30: e1a00006     	mov	r0, r6
  490e34: e59d1068     	ldr	r1, [sp, #0x68]
  490e38: ebffcb35     	bl	0x483b14 <rnd::RandomGenerator::Hash(unsigned char*)> @ imm = #-0xd32c
  490e3c: e59d3068     	ldr	r3, [sp, #0x68]
  490e40: e584007c     	str	r0, [r4, #0x7c]
  490e44: e1530005     	cmp	r3, r5
  490e48: 0a000007     	beq	0x490e6c <rnd::Rule::LoadFromXml(TiXmlNode*)+0x290> @ imm = #0x1c
  490e4c: e3530000     	cmp	r3, #0
  490e50: 0a000005     	beq	0x490e6c <rnd::Rule::LoadFromXml(TiXmlNode*)+0x290> @ imm = #0x14
  490e54: e59d1054     	ldr	r1, [sp, #0x54]
  490e58: e0631001     	rsb	r1, r3, r1
  490e5c: e3510080     	cmp	r1, #128
  490e60: 8a000099     	bhi	0x4910cc <rnd::Rule::LoadFromXml(TiXmlNode*)+0x4f0> @ imm = #0x264
  490e64: e1a00003     	mov	r0, r3
  490e68: eb09e024     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x278090
  490e6c: e5975018     	ldr	r5, [r7, #0x18]
  490e70: e3550000     	cmp	r5, #0
  490e74: 0a000012     	beq	0x490ec4 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x2e8> @ imm = #0x48
  490e78: e3a07001     	mov	r7, #1
  490e7c: e1a01005     	mov	r1, r5
  490e80: e1a00004     	mov	r0, r4
  490e84: e594600c     	ldr	r6, [r4, #0xc]
  490e88: ebfff3ed     	bl	0x48de44 <rnd::Rule::NewRule(TiXmlNode*)> @ imm = #-0x304c
  490e8c: e2862001     	add	r2, r6, #1
  490e90: e2866004     	add	r6, r6, #4
  490e94: e7840106     	str	r0, [r4, r6, lsl #2]
  490e98: e584200c     	str	r2, [r4, #0xc]
  490e9c: e1a01005     	mov	r1, r5
  490ea0: e5903000     	ldr	r3, [r0]
  490ea4: e1a0e00f     	mov	lr, pc
  490ea8: e593f00c     	ldr	pc, [r3, #0xc]
  490eac: e595503c     	ldr	r5, [r5, #0x3c]
  490eb0: e0007007     	and	r7, r0, r7
  490eb4: e3550000     	cmp	r5, #0
  490eb8: 1affffef     	bne	0x490e7c <rnd::Rule::LoadFromXml(TiXmlNode*)+0x2a0> @ imm = #-0x44
  490ebc: e3570000     	cmp	r7, #0
  490ec0: 0a00001f     	beq	0x490f44 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x368> @ imm = #0x7c
  490ec4: e594307c     	ldr	r3, [r4, #0x7c]
  490ec8: e3530000     	cmp	r3, #0
  490ecc: 13a05000     	movne	r5, #0
  490ed0: 0a000019     	beq	0x490f3c <rnd::Rule::LoadFromXml(TiXmlNode*)+0x360> @ imm = #0x64
  490ed4: e1a00004     	mov	r0, r4
  490ed8: ebffeb85     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x51ec
  490edc: e5903118     	ldr	r3, [r0, #0x118]
  490ee0: e590211c     	ldr	r2, [r0, #0x11c]
  490ee4: e0633002     	rsb	r3, r3, r2
  490ee8: e1550143     	cmp	r5, r3, asr #2
  490eec: 2a000012     	bhs	0x490f3c <rnd::Rule::LoadFromXml(TiXmlNode*)+0x360> @ imm = #0x48
  490ef0: e1a00004     	mov	r0, r4
  490ef4: ebffeb7e     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x5208
  490ef8: e5903118     	ldr	r3, [r0, #0x118]
  490efc: e594107c     	ldr	r1, [r4, #0x7c]
  490f00: e7930105     	ldr	r0, [r3, r5, lsl #2]
  490f04: ebffeb45     	bl	0x48bc20 <rnd::RoomPool::Find(unsigned long)> @ imm = #-0x52ec
  490f08: e3500000     	cmp	r0, #0
  490f0c: 0a000002     	beq	0x490f1c <rnd::Rule::LoadFromXml(TiXmlNode*)+0x340> @ imm = #0x8
  490f10: e5941080     	ldr	r1, [r4, #0x80]
  490f14: e5942084     	ldr	r2, [r4, #0x84]
  490f18: ebffeb38     	bl	0x48bc00 <rnd::RPElem::FillSizes(int, int)> @ imm = #-0x5320
  490f1c: e1a00004     	mov	r0, r4
  490f20: ebffeb73     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x5234
  490f24: e5903118     	ldr	r3, [r0, #0x118]
  490f28: e590211c     	ldr	r2, [r0, #0x11c]
  490f2c: e2855001     	add	r5, r5, #1
  490f30: e0633002     	rsb	r3, r3, r2
  490f34: e1550143     	cmp	r5, r3, asr #2
  490f38: 3affffec     	blo	0x490ef0 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x314> @ imm = #-0x50
  490f3c: e3a00001     	mov	r0, #1
  490f40: ea000000     	b	0x490f48 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x36c> @ imm = #0x0
  490f44: e3a00000     	mov	r0, #0
  490f48: e798300b     	ldr	r3, [r8, r11]
  490f4c: e59d212c     	ldr	r2, [sp, #0x12c]
  490f50: e5933000     	ldr	r3, [r3]
  490f54: e1520003     	cmp	r2, r3
  490f58: 1a0000c8     	bne	0x491280 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6a4> @ imm = #0x320
  490f5c: e28ddf4d     	add	sp, sp, #308
  490f60: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  490f64: ebf9fd35     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x180b2c
  490f68: e1a06007     	mov	r6, r7
  490f6c: eaffff6f     	b	0x490d30 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x154> @ imm = #-0x244
  490f70: e59d1124     	ldr	r1, [sp, #0x124]
  490f74: e1500001     	cmp	r0, r1
  490f78: 0a000086     	beq	0x491198 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x5bc> @ imm = #0x218
  490f7c: e28d2e13     	add	r2, sp, #304
  490f80: e3a0305b     	mov	r3, #91
  490f84: e56230f0     	strb	r3, [r2, #-0xf0]!
  490f88: e28d3044     	add	r3, sp, #68
  490f8c: ebfaf71c     	bl	0x34ec04 <char const* std::priv::__find_if<char const*, std::priv::_Eq_char_bound<std::char_traits<char>>>(char const*, char const*, std::priv::_Eq_char_bound<std::char_traits<char>>, std::random_access_iterator_tag const&)> @ imm = #-0x142390
  490f90: e59d1124     	ldr	r1, [sp, #0x124]
  490f94: e1500001     	cmp	r0, r1
  490f98: 0a00007e     	beq	0x491198 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x5bc> @ imm = #0x1f8
  490f9c: e59d3128     	ldr	r3, [sp, #0x128]
  490fa0: e0635000     	rsb	r5, r3, r0
  490fa4: e3750001     	cmn	r5, #1
  490fa8: 0a00007a     	beq	0x491198 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x5bc> @ imm = #0x1e8
  490fac: e1510003     	cmp	r1, r3
  490fb0: 0a000076     	beq	0x491190 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x5b4> @ imm = #0x1d8
  490fb4: e28d2e13     	add	r2, sp, #304
  490fb8: e3a0005d     	mov	r0, #93
  490fbc: e56200f8     	strb	r0, [r2, #-0xf8]!
  490fc0: e1a00003     	mov	r0, r3
  490fc4: e28d303c     	add	r3, sp, #60
  490fc8: ebfaf70d     	bl	0x34ec04 <char const* std::priv::__find_if<char const*, std::priv::_Eq_char_bound<std::char_traits<char>>>(char const*, char const*, std::priv::_Eq_char_bound<std::char_traits<char>>, std::random_access_iterator_tag const&)> @ imm = #-0x1423cc
  490fcc: e59d3124     	ldr	r3, [sp, #0x124]
  490fd0: e1500003     	cmp	r0, r3
  490fd4: 0a00006d     	beq	0x491190 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x5b4> @ imm = #0x1b4
  490fd8: e59d3128     	ldr	r3, [sp, #0x128]
  490fdc: e0630000     	rsb	r0, r3, r0
  490fe0: e28d60fc     	add	r6, sp, #252
  490fe4: e2653001     	rsb	r3, r5, #1
  490fe8: e0833000     	add	r3, r3, r0
  490fec: e2852001     	add	r2, r5, #1
  490ff0: e28dc034     	add	r12, sp, #52
  490ff4: e59d1008     	ldr	r1, [sp, #0x8]
  490ff8: e1a00006     	mov	r0, r6
  490ffc: e58dc000     	str	r12, [sp]
  491000: ebfe1334     	bl	0x415cd8 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&, unsigned int, unsigned int, std::allocator<char> const&)> @ imm = #-0x7b330
  491004: e59d0110     	ldr	r0, [sp, #0x110]
  491008: ebf9f421     	bl	0x30e094 <.plt+0x320>   @ imm = #-0x182f7c
  49100c: e584006c     	str	r0, [r4, #0x6c]
  491010: e1a00006     	mov	r0, r6
  491014: ebfa1c8e     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x178dc8
  491018: e5943004     	ldr	r3, [r4, #0x4]
  49101c: e2456001     	sub	r6, r5, #1
  491020: e28d50e4     	add	r5, sp, #228
  491024: e593a08c     	ldr	r10, [r3, #0x8c]
  491028: e28dc030     	add	r12, sp, #48
  49102c: e3a02001     	mov	r2, #1
  491030: e1a03006     	mov	r3, r6
  491034: e59d1008     	ldr	r1, [sp, #0x8]
  491038: e1a00005     	mov	r0, r5
  49103c: e58dc000     	str	r12, [sp]
  491040: ebfe1324     	bl	0x415cd8 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&, unsigned int, unsigned int, std::allocator<char> const&)> @ imm = #-0x7b370
  491044: e1a0000a     	mov	r0, r10
  491048: e59d10f8     	ldr	r1, [sp, #0xf8]
  49104c: ebffcd40     	bl	0x484554 <rnd::RandomGenerator::ValidList(char const*)> @ imm = #-0xcb00
  491050: e1a0a000     	mov	r10, r0
  491054: e1a00005     	mov	r0, r5
  491058: ebfa1c7d     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x178e0c
  49105c: e35a0000     	cmp	r10, #0
  491060: 1a000007     	bne	0x491084 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x4a8> @ imm = #0x1c
  491064: e59f3224     	ldr	r3, [pc, #0x224]        @ 0x491290 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6b4>
  491068: e7983003     	ldr	r3, [r8, r3]
  49106c: e5933000     	ldr	r3, [r3]
  491070: e3530002     	cmp	r3, #2
  491074: 058aa000     	streq	r10, [r10]
  491078: 0a000001     	beq	0x491084 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x4a8> @ imm = #0x4
  49107c: e3530001     	cmp	r3, #1
  491080: 0a000035     	beq	0x49115c <rnd::Rule::LoadFromXml(TiXmlNode*)+0x580> @ imm = #0xd4
  491084: e5940004     	ldr	r0, [r4, #0x4]
  491088: ebffeb19     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x539c
  49108c: e28d50cc     	add	r5, sp, #204
  491090: e28dc02c     	add	r12, sp, #44
  491094: e1a0a000     	mov	r10, r0
  491098: e1a03006     	mov	r3, r6
  49109c: e59d1008     	ldr	r1, [sp, #0x8]
  4910a0: e3a02001     	mov	r2, #1
  4910a4: e1a00005     	mov	r0, r5
  4910a8: e58dc000     	str	r12, [sp]
  4910ac: ebfe1309     	bl	0x415cd8 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&, unsigned int, unsigned int, std::allocator<char> const&)> @ imm = #-0x7b3dc
  4910b0: e1a0000a     	mov	r0, r10
  4910b4: e59d10e0     	ldr	r1, [sp, #0xe0]
  4910b8: ebffccde     	bl	0x484438 <rnd::RandomGenerator::GetList(char const*) const> @ imm = #-0xcc88
  4910bc: e5840068     	str	r0, [r4, #0x68]
  4910c0: e1a00005     	mov	r0, r5
  4910c4: ebfa1c62     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x178e78
  4910c8: eaffff2e     	b	0x490d88 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x1ac> @ imm = #-0x348
  4910cc: e1a00003     	mov	r0, r3
  4910d0: ebf9fcda     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x180c98
  4910d4: eaffff64     	b	0x490e6c <rnd::Rule::LoadFromXml(TiXmlNode*)+0x290> @ imm = #-0x270
  4910d8: e59f31b0     	ldr	r3, [pc, #0x1b0]        @ 0x491290 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6b4>
  4910dc: e7983003     	ldr	r3, [r8, r3]
  4910e0: e5933000     	ldr	r3, [r3]
  4910e4: e3530002     	cmp	r3, #2
  4910e8: 05800000     	streq	r0, [r0]
  4910ec: 0affff1b     	beq	0x490d60 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x184> @ imm = #-0x394
  4910f0: e3530001     	cmp	r3, #1
  4910f4: 1affff19     	bne	0x490d60 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x184> @ imm = #-0x39c
  4910f8: e59f0194     	ldr	r0, [pc, #0x194]        @ 0x491294 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6b8>
  4910fc: e59f11a8     	ldr	r1, [pc, #0x1a8]        @ 0x4912ac <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6d0>
  491100: e59f21a8     	ldr	r2, [pc, #0x1a8]        @ 0x4912b0 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6d4>
  491104: e7980000     	ldr	r0, [r8, r0]
  491108: e59f31a4     	ldr	r3, [pc, #0x1a4]        @ 0x4912b4 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6d8>
  49110c: e300c151     	movw	r12, #0x151
  491110: e08f1001     	add	r1, pc, r1
  491114: e08f2002     	add	r2, pc, r2
  491118: e08f3003     	add	r3, pc, r3
  49111c: e28000a8     	add	r0, r0, #168
  491120: e58dc000     	str	r12, [sp]
  491124: ebf9f3b6     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x183128
  491128: eaffff0c     	b	0x490d60 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x184> @ imm = #-0x3d0
  49112c: e59d3010     	ldr	r3, [sp, #0x10]
  491130: e300c14d     	movw	r12, #0x14d
  491134: e59d1014     	ldr	r1, [sp, #0x14]
  491138: e7980003     	ldr	r0, [r8, r3]
  49113c: e59d2018     	ldr	r2, [sp, #0x18]
  491140: e59d301c     	ldr	r3, [sp, #0x1c]
  491144: e28000a8     	add	r0, r0, #168
  491148: e58dc000     	str	r12, [sp]
  49114c: ebf9f3ac     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x183150
  491150: eafffee8     	b	0x490cf8 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x11c> @ imm = #-0x460
  491154: ebf9fcb9     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x180d1c
  491158: eaffff15     	b	0x490db4 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x1d8> @ imm = #-0x3ac
  49115c: e59f0130     	ldr	r0, [pc, #0x130]        @ 0x491294 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6b8>
  491160: e59f1150     	ldr	r1, [pc, #0x150]        @ 0x4912b8 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6dc>
  491164: e59f2150     	ldr	r2, [pc, #0x150]        @ 0x4912bc <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6e0>
  491168: e7980000     	ldr	r0, [r8, r0]
  49116c: e59f314c     	ldr	r3, [pc, #0x14c]        @ 0x4912c0 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6e4>
  491170: e300c13b     	movw	r12, #0x13b
  491174: e08f1001     	add	r1, pc, r1
  491178: e08f2002     	add	r2, pc, r2
  49117c: e08f3003     	add	r3, pc, r3
  491180: e28000a8     	add	r0, r0, #168
  491184: e58dc000     	str	r12, [sp]
  491188: ebf9f39d     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x18318c
  49118c: eaffffbc     	b	0x491084 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x4a8> @ imm = #-0x110
  491190: e3e00000     	mvn	r0, #0
  491194: eaffff91     	b	0x490fe0 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x404> @ imm = #-0x1bc
  491198: e5942004     	ldr	r2, [r4, #0x4]
  49119c: e3e03000     	mvn	r3, #0
  4911a0: e584306c     	str	r3, [r4, #0x6c]
  4911a4: e592608c     	ldr	r6, [r2, #0x8c]
  4911a8: e28d50b4     	add	r5, sp, #180
  4911ac: e28dc028     	add	r12, sp, #40
  4911b0: e3a02001     	mov	r2, #1
  4911b4: e59d1008     	ldr	r1, [sp, #0x8]
  4911b8: e1a00005     	mov	r0, r5
  4911bc: e58dc000     	str	r12, [sp]
  4911c0: ebfe12c4     	bl	0x415cd8 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&, unsigned int, unsigned int, std::allocator<char> const&)> @ imm = #-0x7b4f0
  4911c4: e1a00006     	mov	r0, r6
  4911c8: e59d10c8     	ldr	r1, [sp, #0xc8]
  4911cc: ebffcce0     	bl	0x484554 <rnd::RandomGenerator::ValidList(char const*)> @ imm = #-0xcc80
  4911d0: e1a06000     	mov	r6, r0
  4911d4: e1a00005     	mov	r0, r5
  4911d8: ebfa1c1d     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x178f8c
  4911dc: e3560000     	cmp	r6, #0
  4911e0: 1a000007     	bne	0x491204 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x628> @ imm = #0x1c
  4911e4: e59f30a4     	ldr	r3, [pc, #0xa4]         @ 0x491290 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6b4>
  4911e8: e7983003     	ldr	r3, [r8, r3]
  4911ec: e5933000     	ldr	r3, [r3]
  4911f0: e3530002     	cmp	r3, #2
  4911f4: 05866000     	streq	r6, [r6]
  4911f8: 0a000001     	beq	0x491204 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x628> @ imm = #0x4
  4911fc: e3530001     	cmp	r3, #1
  491200: 0a000011     	beq	0x49124c <rnd::Rule::LoadFromXml(TiXmlNode*)+0x670> @ imm = #0x44
  491204: e5940004     	ldr	r0, [r4, #0x4]
  491208: ebffeab9     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x551c
  49120c: e28d509c     	add	r5, sp, #156
  491210: e28dc024     	add	r12, sp, #36
  491214: e1a06000     	mov	r6, r0
  491218: e59d1008     	ldr	r1, [sp, #0x8]
  49121c: e3a02001     	mov	r2, #1
  491220: e3e03000     	mvn	r3, #0
  491224: e1a00005     	mov	r0, r5
  491228: e58dc000     	str	r12, [sp]
  49122c: ebfe12a9     	bl	0x415cd8 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&, unsigned int, unsigned int, std::allocator<char> const&)> @ imm = #-0x7b55c
  491230: e1a00006     	mov	r0, r6
  491234: e59d10b0     	ldr	r1, [sp, #0xb0]
  491238: ebffcc7e     	bl	0x484438 <rnd::RandomGenerator::GetList(char const*) const> @ imm = #-0xce08
  49123c: e5840068     	str	r0, [r4, #0x68]
  491240: e1a00005     	mov	r0, r5
  491244: ebfa1c02     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x178ff8
  491248: eafffece     	b	0x490d88 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x1ac> @ imm = #-0x4c8
  49124c: e59f0040     	ldr	r0, [pc, #0x40]         @ 0x491294 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6b8>
  491250: e59f106c     	ldr	r1, [pc, #0x6c]         @ 0x4912c4 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6e8>
  491254: e59f206c     	ldr	r2, [pc, #0x6c]         @ 0x4912c8 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6ec>
  491258: e7980000     	ldr	r0, [r8, r0]
  49125c: e59f3068     	ldr	r3, [pc, #0x68]         @ 0x4912cc <rnd::Rule::LoadFromXml(TiXmlNode*)+0x6f0>
  491260: e300c142     	movw	r12, #0x142
  491264: e08f1001     	add	r1, pc, r1
  491268: e08f2002     	add	r2, pc, r2
  49126c: e08f3003     	add	r3, pc, r3
  491270: e28000a8     	add	r0, r0, #168
  491274: e58dc000     	str	r12, [sp]
  491278: ebf9f361     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x18327c
  49127c: eaffffe0     	b	0x491204 <rnd::Rule::LoadFromXml(TiXmlNode*)+0x628> @ imm = #-0x80
  491280: ebf9f422     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x182f78
  491284: a4 3e 50 00  	.word	0x00503ea4
  491288: ac 40 00 00  	.word	0x000040ac
  49128c: cc 04 45 00  	.word	0x004504cc
  491290: c0 39 00 00  	.word	0x000039c0
  491294: c0 19 00 00  	.word	0x000019c0
  491298: 68 d7 42 00  	.word	0x0042d768
  49129c: e4 42 44 00  	.word	0x004442e4
  4912a0: 94 41 44 00  	.word	0x00444194
  4912a4: f0 41 44 00  	.word	0x004441f0
  4912a8: 90 a8 47 00  	.word	0x0047a890
  4912ac: c8 d2 42 00  	.word	0x0042d2c8
  4912b0: 7c 3e 44 00  	.word	0x00443e7c
  4912b4: 10 3d 44 00  	.word	0x00443d10
  4912b8: 64 d2 42 00  	.word	0x0042d264
  4912bc: 78 3d 44 00  	.word	0x00443d78
  4912c0: ac 3c 44 00  	.word	0x00443cac
  4912c4: 74 d1 42 00  	.word	0x0042d174
  4912c8: c8 3c 44 00  	.word	0x00443cc8
  4912cc: bc 3b 44 00  	.word	0x00443bbc
