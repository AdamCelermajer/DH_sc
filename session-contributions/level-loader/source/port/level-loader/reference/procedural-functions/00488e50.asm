
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00488e50 <rnd::RandomGenerator::SaveModularLevelXml()>:
  488e50: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  488e54: e59f4448     	ldr	r4, [pc, #0x448]        @ 0x4892a4 <rnd::RandomGenerator::SaveModularLevelXml()+0x454>
  488e58: e59f2448     	ldr	r2, [pc, #0x448]        @ 0x4892a8 <rnd::RandomGenerator::SaveModularLevelXml()+0x458>
  488e5c: e24ddf4b     	sub	sp, sp, #300
  488e60: e08f4004     	add	r4, pc, r4
  488e64: e7943002     	ldr	r3, [r4, r2]
  488e68: e28d8020     	add	r8, sp, #32
  488e6c: e1a0a001     	mov	r10, r1
  488e70: e5933000     	ldr	r3, [r3]
  488e74: e58d000c     	str	r0, [sp, #0xc]
  488e78: e1a00008     	mov	r0, r8
  488e7c: e28d5f43     	add	r5, sp, #268
  488e80: e58d3124     	str	r3, [sp, #0x124]
  488e84: e58d2010     	str	r2, [sp, #0x10]
  488e88: ebfa37ab     	bl	0x316d3c <StreamBuffer::StreamBuffer()> @ imm = #-0x172154
  488e8c: e1a0000a     	mov	r0, r10
  488e90: e1a01008     	mov	r1, r8
  488e94: ebffff3a     	bl	0x488b84 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)> @ imm = #-0x318
  488e98: e59a1170     	ldr	r1, [r10, #0x170]
  488e9c: e59a216c     	ldr	r2, [r10, #0x16c]
  488ea0: e1a00005     	mov	r0, r5
  488ea4: e58d511c     	str	r5, [sp, #0x11c]
  488ea8: e58d5120     	str	r5, [sp, #0x120]
  488eac: ebfa220d     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0x1777cc
  488eb0: e59a1188     	ldr	r1, [r10, #0x188]
  488eb4: e59a2184     	ldr	r2, [r10, #0x184]
  488eb8: e1a00005     	mov	r0, r5
  488ebc: ebfa1e50     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0x1786c0
  488ec0: e59f13e4     	ldr	r1, [pc, #0x3e4]        @ 0x4892ac <rnd::RandomGenerator::SaveModularLevelXml()+0x45c>
  488ec4: e1a00005     	mov	r0, r5
  488ec8: e08f1001     	add	r1, pc, r1
  488ecc: ebffee15     	bl	0x484728 <std::string::find(char const*, unsigned int) const (.clone.1)> @ imm = #-0x47ac
  488ed0: e3700001     	cmn	r0, #1
  488ed4: e1a07000     	mov	r7, r0
  488ed8: 0a000091     	beq	0x489124 <rnd::RandomGenerator::SaveModularLevelXml()+0x2d4> @ imm = #0x244
  488edc: e28d605c     	add	r6, sp, #92
  488ee0: e2863048     	add	r3, r6, #72
  488ee4: e1a00003     	mov	r0, r3
  488ee8: e58d301c     	str	r3, [sp, #0x1c]
  488eec: eb0a0013     	bl	0x708f40 <___ZNSt8ios_baseC2Ev_veneer> @ imm = #0x28004c
  488ef0: e59f23b8     	ldr	r2, [pc, #0x3b8]        @ 0x4892b0 <rnd::RandomGenerator::SaveModularLevelXml()+0x460>
  488ef4: e59f33b8     	ldr	r3, [pc, #0x3b8]        @ 0x4892b4 <rnd::RandomGenerator::SaveModularLevelXml()+0x464>
  488ef8: e3a09000     	mov	r9, #0
  488efc: e794b002     	ldr	r11, [r4, r2]
  488f00: e7943003     	ldr	r3, [r4, r3]
  488f04: e5cd90e8     	strb	r9, [sp, #0xe8]
  488f08: e59b2008     	ldr	r2, [r11, #0x8]
  488f0c: e2833008     	add	r3, r3, #8
  488f10: e58d90ec     	str	r9, [sp, #0xec]
  488f14: e58d205c     	str	r2, [sp, #0x5c]
  488f18: e58d90f0     	str	r9, [sp, #0xf0]
  488f1c: e58d30a4     	str	r3, [sp, #0xa4]
  488f20: e512300c     	ldr	r3, [r2, #-0xc]
  488f24: e59b200c     	ldr	r2, [r11, #0xc]
  488f28: e2860008     	add	r0, r6, #8
  488f2c: e58d0014     	str	r0, [sp, #0x14]
  488f30: e7862003     	str	r2, [r6, r3]
  488f34: e59d305c     	ldr	r3, [sp, #0x5c]
  488f38: e28d20f4     	add	r2, sp, #244
  488f3c: e58d9060     	str	r9, [sp, #0x60]
  488f40: e58d2018     	str	r2, [sp, #0x18]
  488f44: e513000c     	ldr	r0, [r3, #-0xc]
  488f48: e1a01009     	mov	r1, r9
  488f4c: e0860000     	add	r0, r6, r0
  488f50: ebfa6853     	bl	0x3230a4 <std::basic_ios<char, std::char_traits<char>>::init(std::basic_streambuf<char, std::char_traits<char>>*)> @ imm = #-0x165eb4
  488f54: e59b3010     	ldr	r3, [r11, #0x10]
  488f58: e59b2014     	ldr	r2, [r11, #0x14]
  488f5c: e59d0014     	ldr	r0, [sp, #0x14]
  488f60: e58d3064     	str	r3, [sp, #0x64]
  488f64: e513300c     	ldr	r3, [r3, #-0xc]
  488f68: e1a01009     	mov	r1, r9
  488f6c: e7802003     	str	r2, [r0, r3]
  488f70: e59d3064     	ldr	r3, [sp, #0x64]
  488f74: e59d2014     	ldr	r2, [sp, #0x14]
  488f78: e513000c     	ldr	r0, [r3, #-0xc]
  488f7c: e0820000     	add	r0, r2, r0
  488f80: ebfa6847     	bl	0x3230a4 <std::basic_ios<char, std::char_traits<char>>::init(std::basic_streambuf<char, std::char_traits<char>>*)> @ imm = #-0x165ee4
  488f84: e59b3004     	ldr	r3, [r11, #0x4]
  488f88: e59b0018     	ldr	r0, [r11, #0x18]
  488f8c: e59b201c     	ldr	r2, [r11, #0x1c]
  488f90: e58d305c     	str	r3, [sp, #0x5c]
  488f94: e513300c     	ldr	r3, [r3, #-0xc]
  488f98: e1a01009     	mov	r1, r9
  488f9c: e7860003     	str	r0, [r6, r3]
  488fa0: e59d305c     	ldr	r3, [sp, #0x5c]
  488fa4: e58d2064     	str	r2, [sp, #0x64]
  488fa8: e513000c     	ldr	r0, [r3, #-0xc]
  488fac: e0860000     	add	r0, r6, r0
  488fb0: ebfa683b     	bl	0x3230a4 <std::basic_ios<char, std::char_traits<char>>::init(std::basic_streambuf<char, std::char_traits<char>>*)> @ imm = #-0x165f14
  488fb4: e59f32fc     	ldr	r3, [pc, #0x2fc]        @ 0x4892b8 <rnd::RandomGenerator::SaveModularLevelXml()+0x468>
  488fb8: e59f22fc     	ldr	r2, [pc, #0x2fc]        @ 0x4892bc <rnd::RandomGenerator::SaveModularLevelXml()+0x46c>
  488fbc: e2860028     	add	r0, r6, #40
  488fc0: e7943003     	ldr	r3, [r4, r3]
  488fc4: e7942002     	ldr	r2, [r4, r2]
  488fc8: e58d906c     	str	r9, [sp, #0x6c]
  488fcc: e283c020     	add	r12, r3, #32
  488fd0: e2821008     	add	r1, r2, #8
  488fd4: e283200c     	add	r2, r3, #12
  488fd8: e2833034     	add	r3, r3, #52
  488fdc: e58dc064     	str	r12, [sp, #0x64]
  488fe0: e58d205c     	str	r2, [sp, #0x5c]
  488fe4: e58d30a4     	str	r3, [sp, #0xa4]
  488fe8: e58d1068     	str	r1, [sp, #0x68]
  488fec: e58d9070     	str	r9, [sp, #0x70]
  488ff0: e58d9074     	str	r9, [sp, #0x74]
  488ff4: e58d9078     	str	r9, [sp, #0x78]
  488ff8: e58d907c     	str	r9, [sp, #0x7c]
  488ffc: e58d9080     	str	r9, [sp, #0x80]
  489000: eb09ff92     	bl	0x708e50 <___ZNSt6localeC1Ev_veneer> @ imm = #0x27fe48
  489004: e59f22b4     	ldr	r2, [pc, #0x2b4]        @ 0x4892c0 <rnd::RandomGenerator::SaveModularLevelXml()+0x470>
  489008: e2863030     	add	r3, r6, #48
  48900c: e1a00003     	mov	r0, r3
  489010: e7942002     	ldr	r2, [r4, r2]
  489014: e3a01010     	mov	r1, #16
  489018: e58d309c     	str	r3, [sp, #0x9c]
  48901c: e2822008     	add	r2, r2, #8
  489020: e58d2068     	str	r2, [sp, #0x68]
  489024: e3a02018     	mov	r2, #24
  489028: e58d2088     	str	r2, [sp, #0x88]
  48902c: e58d30a0     	str	r3, [sp, #0xa0]
  489030: ebfa2191     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x1779bc
  489034: e59d309c     	ldr	r3, [sp, #0x9c]
  489038: e59d001c     	ldr	r0, [sp, #0x1c]
  48903c: e286100c     	add	r1, r6, #12
  489040: e5c39000     	strb	r9, [r3]
  489044: ebfa6816     	bl	0x3230a4 <std::basic_ios<char, std::char_traits<char>>::init(std::basic_streambuf<char, std::char_traits<char>>*)> @ imm = #-0x165fa8
  489048: e59f1274     	ldr	r1, [pc, #0x274]        @ 0x4892c4 <rnd::RandomGenerator::SaveModularLevelXml()+0x474>
  48904c: e59d0014     	ldr	r0, [sp, #0x14]
  489050: e08f1001     	add	r1, pc, r1
  489054: ebfa284f     	bl	0x313198 <std::ostream::_M_put_nowiden(char const*)> @ imm = #-0x175ec4
  489058: e59a1004     	ldr	r1, [r10, #0x4]
  48905c: e59d0014     	ldr	r0, [sp, #0x14]
  489060: ebffeeb1     	bl	0x484b2c <std::basic_ostream<char, std::char_traits<char>>& std::priv::__put_num<char, std::char_traits<char>, unsigned long>(std::basic_ostream<char, std::char_traits<char>>&, unsigned long)> @ imm = #-0x453c
  489064: e59f125c     	ldr	r1, [pc, #0x25c]        @ 0x4892c8 <rnd::RandomGenerator::SaveModularLevelXml()+0x478>
  489068: e08f1001     	add	r1, pc, r1
  48906c: ebfa2849     	bl	0x313198 <std::ostream::_M_put_nowiden(char const*)> @ imm = #-0x175edc
  489070: e59d0018     	ldr	r0, [sp, #0x18]
  489074: e59d10a0     	ldr	r1, [sp, #0xa0]
  489078: e59d209c     	ldr	r2, [sp, #0x9c]
  48907c: e58d0104     	str	r0, [sp, #0x104]
  489080: e58d0108     	str	r0, [sp, #0x108]
  489084: ebfa2197     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0x1779a4
  489088: e59d3120     	ldr	r3, [sp, #0x120]
  48908c: e59d911c     	ldr	r9, [sp, #0x11c]
  489090: e0639009     	rsb	r9, r3, r9
  489094: e1570009     	cmp	r7, r9
  489098: 8a00007a     	bhi	0x489288 <rnd::RandomGenerator::SaveModularLevelXml()+0x438> @ imm = #0x1e8
  48909c: e59dc104     	ldr	r12, [sp, #0x104]
  4890a0: e59d3108     	ldr	r3, [sp, #0x108]
  4890a4: e30f2ffe     	movw	r2, #0xfffe
  4890a8: e34f2fff     	movt	r2, #0xffff
  4890ac: e067a009     	rsb	r10, r7, r9
  4890b0: e35a0004     	cmp	r10, #4
  4890b4: 23a0a004     	movhs	r10, #4
  4890b8: e0692002     	rsb	r2, r9, r2
  4890bc: e082200a     	add	r2, r2, r10
  4890c0: e063100c     	rsb	r1, r3, r12
  4890c4: e1510002     	cmp	r1, r2
  4890c8: 8a000068     	bhi	0x489270 <rnd::RandomGenerator::SaveModularLevelXml()+0x420> @ imm = #0x1a0
  4890cc: e59d1120     	ldr	r1, [sp, #0x120]
  4890d0: e08a2007     	add	r2, r10, r7
  4890d4: e58dc000     	str	r12, [sp]
  4890d8: e0812002     	add	r2, r1, r2
  4890dc: e3a0c000     	mov	r12, #0
  4890e0: e0811007     	add	r1, r1, r7
  4890e4: e1a00005     	mov	r0, r5
  4890e8: e58dc004     	str	r12, [sp, #0x4]
  4890ec: ebfda353     	bl	0x3f1e40 <std::string::_M_replace(char*, char*, char const*, char const*, bool)> @ imm = #-0x972b4
  4890f0: e59d0108     	ldr	r0, [sp, #0x108]
  4890f4: e59d2018     	ldr	r2, [sp, #0x18]
  4890f8: e1500002     	cmp	r0, r2
  4890fc: 0a000006     	beq	0x48911c <rnd::RandomGenerator::SaveModularLevelXml()+0x2cc> @ imm = #0x18
  489100: e3500000     	cmp	r0, #0
  489104: 0a000004     	beq	0x48911c <rnd::RandomGenerator::SaveModularLevelXml()+0x2cc> @ imm = #0x10
  489108: e59d10f4     	ldr	r1, [sp, #0xf4]
  48910c: e0601001     	rsb	r1, r0, r1
  489110: e3510080     	cmp	r1, #128
  489114: 8a00005f     	bhi	0x489298 <rnd::RandomGenerator::SaveModularLevelXml()+0x448> @ imm = #0x17c
  489118: eb09ff78     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27fde0
  48911c: e1a00006     	mov	r0, r6
  489120: ebfc0087     	bl	0x389344 <std::basic_stringstream<char, std::char_traits<char>, std::allocator<char>>::~basic_stringstream()> @ imm = #-0xffde4
  489124: e59f31a0     	ldr	r3, [pc, #0x1a0]        @ 0x4892cc <rnd::RandomGenerator::SaveModularLevelXml()+0x47c>
  489128: e59d1120     	ldr	r1, [sp, #0x120]
  48912c: e3a02001     	mov	r2, #1
  489130: e7943003     	ldr	r3, [r4, r3]
  489134: e5933010     	ldr	r3, [r3, #0x10]
  489138: e5936034     	ldr	r6, [r3, #0x34]
  48913c: e5963000     	ldr	r3, [r6]
  489140: e1a00006     	mov	r0, r6
  489144: e1a0e00f     	mov	lr, pc
  489148: e593f094     	ldr	pc, [r3, #0x94]
  48914c: e3500000     	cmp	r0, #0
  489150: e1a07000     	mov	r7, r0
  489154: e58d0054     	str	r0, [sp, #0x54]
  489158: 0a00000f     	beq	0x48919c <rnd::RandomGenerator::SaveModularLevelXml()+0x34c> @ imm = #0x3c
  48915c: e5dd304c     	ldrb	r3, [sp, #0x4c]
  489160: e5902000     	ldr	r2, [r0]
  489164: e3530000     	cmp	r3, #0
  489168: e592a01c     	ldr	r10, [r2, #0x1c]
  48916c: 0a000028     	beq	0x489214 <rnd::RandomGenerator::SaveModularLevelXml()+0x3c4> @ imm = #0xa0
  489170: e59d303c     	ldr	r3, [sp, #0x3c]
  489174: e1a00007     	mov	r0, r7
  489178: e59d2048     	ldr	r2, [sp, #0x48]
  48917c: e5931000     	ldr	r1, [r3]
  489180: e3a03000     	mov	r3, #0
  489184: e12fff3a     	blx	r10
  489188: e1a00006     	mov	r0, r6
  48918c: e5963000     	ldr	r3, [r6]
  489190: e28d1054     	add	r1, sp, #84
  489194: e1a0e00f     	mov	lr, pc
  489198: e593f078     	ldr	pc, [r3, #0x78]
  48919c: e1a00005     	mov	r0, r5
  4891a0: ebffed81     	bl	0x4847ac <std::string::rfind(char, unsigned int) const (.clone.10)> @ imm = #-0x49fc
  4891a4: e28dc058     	add	r12, sp, #88
  4891a8: e2802001     	add	r2, r0, #1
  4891ac: e1a01005     	mov	r1, r5
  4891b0: e59d000c     	ldr	r0, [sp, #0xc]
  4891b4: e3e03000     	mvn	r3, #0
  4891b8: e58dc000     	str	r12, [sp]
  4891bc: ebfe32c5     	bl	0x415cd8 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&, unsigned int, unsigned int, std::allocator<char> const&)> @ imm = #-0x734ec
  4891c0: e59d0120     	ldr	r0, [sp, #0x120]
  4891c4: e1500005     	cmp	r0, r5
  4891c8: 0a000006     	beq	0x4891e8 <rnd::RandomGenerator::SaveModularLevelXml()+0x398> @ imm = #0x18
  4891cc: e3500000     	cmp	r0, #0
  4891d0: 0a000004     	beq	0x4891e8 <rnd::RandomGenerator::SaveModularLevelXml()+0x398> @ imm = #0x10
  4891d4: e59d110c     	ldr	r1, [sp, #0x10c]
  4891d8: e0601001     	rsb	r1, r0, r1
  4891dc: e3510080     	cmp	r1, #128
  4891e0: 8a000020     	bhi	0x489268 <rnd::RandomGenerator::SaveModularLevelXml()+0x418> @ imm = #0x80
  4891e4: eb09ff45     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27fd14
  4891e8: e1a00008     	mov	r0, r8
  4891ec: ebfa35f4     	bl	0x3169c4 <StreamBuffer::~StreamBuffer()> @ imm = #-0x172830
  4891f0: e59d2010     	ldr	r2, [sp, #0x10]
  4891f4: e59d000c     	ldr	r0, [sp, #0xc]
  4891f8: e7943002     	ldr	r3, [r4, r2]
  4891fc: e59d2124     	ldr	r2, [sp, #0x124]
  489200: e5933000     	ldr	r3, [r3]
  489204: e1520003     	cmp	r2, r3
  489208: 1a000024     	bne	0x4892a0 <rnd::RandomGenerator::SaveModularLevelXml()+0x450> @ imm = #0x90
  48920c: e28ddf4b     	add	sp, sp, #300
  489210: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  489214: e59f20b4     	ldr	r2, [pc, #0xb4]         @ 0x4892d0 <rnd::RandomGenerator::SaveModularLevelXml()+0x480>
  489218: e7942002     	ldr	r2, [r4, r2]
  48921c: e5922000     	ldr	r2, [r2]
  489220: e3520002     	cmp	r2, #2
  489224: 05833000     	streq	r3, [r3]
  489228: 0affffd0     	beq	0x489170 <rnd::RandomGenerator::SaveModularLevelXml()+0x320> @ imm = #-0xc0
  48922c: e3520001     	cmp	r2, #1
  489230: 1affffce     	bne	0x489170 <rnd::RandomGenerator::SaveModularLevelXml()+0x320> @ imm = #-0xc8
  489234: e59f0098     	ldr	r0, [pc, #0x98]         @ 0x4892d4 <rnd::RandomGenerator::SaveModularLevelXml()+0x484>
  489238: e59f1098     	ldr	r1, [pc, #0x98]         @ 0x4892d8 <rnd::RandomGenerator::SaveModularLevelXml()+0x488>
  48923c: e59f2098     	ldr	r2, [pc, #0x98]         @ 0x4892dc <rnd::RandomGenerator::SaveModularLevelXml()+0x48c>
  489240: e7940000     	ldr	r0, [r4, r0]
  489244: e59f3094     	ldr	r3, [pc, #0x94]         @ 0x4892e0 <rnd::RandomGenerator::SaveModularLevelXml()+0x490>
  489248: e3a0c082     	mov	r12, #130
  48924c: e08f1001     	add	r1, pc, r1
  489250: e08f2002     	add	r2, pc, r2
  489254: e08f3003     	add	r3, pc, r3
  489258: e28000a8     	add	r0, r0, #168
  48925c: e58dc000     	str	r12, [sp]
  489260: ebfa1367     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x17b264
  489264: eaffffc1     	b	0x489170 <rnd::RandomGenerator::SaveModularLevelXml()+0x320> @ imm = #-0xfc
  489268: ebfa1c74     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x178e30
  48926c: eaffffdd     	b	0x4891e8 <rnd::RandomGenerator::SaveModularLevelXml()+0x398> @ imm = #-0x8c
  489270: e59f006c     	ldr	r0, [pc, #0x6c]         @ 0x4892e4 <rnd::RandomGenerator::SaveModularLevelXml()+0x494>
  489274: e08f0000     	add	r0, pc, r0
  489278: eb09fef0     	bl	0x708e40 <___ZSt24__stl_throw_length_errorPKc_veneer> @ imm = #0x27fbc0
  48927c: e59d3108     	ldr	r3, [sp, #0x108]
  489280: e59dc104     	ldr	r12, [sp, #0x104]
  489284: eaffff90     	b	0x4890cc <rnd::RandomGenerator::SaveModularLevelXml()+0x27c> @ imm = #-0x1c0
  489288: e59f0058     	ldr	r0, [pc, #0x58]         @ 0x4892e8 <rnd::RandomGenerator::SaveModularLevelXml()+0x498>
  48928c: e08f0000     	add	r0, pc, r0
  489290: eb09ff06     	bl	0x708eb0 <___ZSt24__stl_throw_out_of_rangePKc_veneer> @ imm = #0x27fc18
  489294: eaffff80     	b	0x48909c <rnd::RandomGenerator::SaveModularLevelXml()+0x24c> @ imm = #-0x200
  489298: ebfa1c68     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x178e60
  48929c: eaffff9e     	b	0x48911c <rnd::RandomGenerator::SaveModularLevelXml()+0x2cc> @ imm = #-0x188
  4892a0: ebfa141a     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x17af98
  4892a4: 30 bc 50 00  	.word	0x0050bc30
  4892a8: ac 40 00 00  	.word	0x000040ac
  4892ac: a0 68 43 00  	.word	0x004368a0
  4892b0: cc 38 00 00  	.word	0x000038cc
  4892b4: 30 37 00 00  	.word	0x00003730
  4892b8: 40 0e 00 00  	.word	0x00000e40
  4892bc: b4 07 00 00  	.word	0x000007b4
  4892c0: 50 4a 00 00  	.word	0x00004a50
  4892c4: d0 b4 43 00  	.word	0x0043b4d0
  4892c8: e8 66 43 00  	.word	0x004366e8
  4892cc: f4 37 00 00  	.word	0x000037f4
  4892d0: c0 39 00 00  	.word	0x000039c0
  4892d4: c0 19 00 00  	.word	0x000019c0
  4892d8: 8c 51 43 00  	.word	0x0043518c
  4892dc: 78 53 43 00  	.word	0x00435378
  4892e0: 6c d5 43 00  	.word	0x0043d56c
  4892e4: e4 51 43 00  	.word	0x004351e4
  4892e8: cc 51 43 00  	.word	0x004351cc
