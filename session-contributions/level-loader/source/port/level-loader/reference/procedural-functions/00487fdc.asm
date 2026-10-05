
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00487fdc <rnd::RandomGenerator::RandomGenerator()>:
  487fdc: e92d4030     	push	{r4, r5, lr}
  487fe0: e1a04000     	mov	r4, r0
  487fe4: e24dd00c     	sub	sp, sp, #12
  487fe8: e2800008     	add	r0, r0, #8
  487fec: e3a05000     	mov	r5, #0
  487ff0: ebffff78     	bl	0x487dd8 <Array2d<rnd::Tile*>::Array2d()> @ imm = #-0x220
  487ff4: e1a02004     	mov	r2, r4
  487ff8: e1a03004     	mov	r3, r4
  487ffc: e5845040     	str	r5, [r4, #0x40]
  488000: e5e2503c     	strb	r5, [r2, #0x3c]!
  488004: e5842048     	str	r2, [r4, #0x48]
  488008: e5842044     	str	r2, [r4, #0x44]
  48800c: e584504c     	str	r5, [r4, #0x4c]
  488010: e5845058     	str	r5, [r4, #0x58]
  488014: e5e35054     	strb	r5, [r3, #0x54]!
  488018: e5843060     	str	r3, [r4, #0x60]
  48801c: e584305c     	str	r3, [r4, #0x5c]
  488020: e5845064     	str	r5, [r4, #0x64]
  488024: e1a01004     	mov	r1, r4
  488028: e284006c     	add	r0, r4, #108
  48802c: eb0017f1     	bl	0x48dff8 <rnd::RootRule::RootRule(rnd::RandomGenerator&)> @ imm = #0x5fc4
  488030: e28430fc     	add	r3, r4, #252
  488034: e1a00003     	mov	r0, r3
  488038: e584310c     	str	r3, [r4, #0x10c]
  48803c: e5843110     	str	r3, [r4, #0x110]
  488040: e3a01010     	mov	r1, #16
  488044: ebfa258c     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x1769d0
  488048: e594210c     	ldr	r2, [r4, #0x10c]
  48804c: e2843f4b     	add	r3, r4, #300
  488050: e1a00003     	mov	r0, r3
  488054: e5c25000     	strb	r5, [r2]
  488058: e3a01010     	mov	r1, #16
  48805c: e5845114     	str	r5, [r4, #0x114]
  488060: e5845118     	str	r5, [r4, #0x118]
  488064: e584511c     	str	r5, [r4, #0x11c]
  488068: e5845120     	str	r5, [r4, #0x120]
  48806c: e5845124     	str	r5, [r4, #0x124]
  488070: e5845128     	str	r5, [r4, #0x128]
  488074: e584313c     	str	r3, [r4, #0x13c]
  488078: e5843140     	str	r3, [r4, #0x140]
  48807c: ebfa257e     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x176a08
  488080: e594213c     	ldr	r2, [r4, #0x13c]
  488084: e2843f51     	add	r3, r4, #324
  488088: e1a00003     	mov	r0, r3
  48808c: e5c25000     	strb	r5, [r2]
  488090: e3a01010     	mov	r1, #16
  488094: e5843154     	str	r3, [r4, #0x154]
  488098: e5843158     	str	r3, [r4, #0x158]
  48809c: ebfa2576     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x176a28
  4880a0: e5943154     	ldr	r3, [r4, #0x154]
  4880a4: e59f1080     	ldr	r1, [pc, #0x80]         @ 0x48812c <rnd::RandomGenerator::RandomGenerator()+0x150>
  4880a8: e28d2004     	add	r2, sp, #4
  4880ac: e5c35000     	strb	r5, [r3]
  4880b0: e08f1001     	add	r1, pc, r1
  4880b4: e2840f57     	add	r0, r4, #348
  4880b8: ebfa300b     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x173fd4
  4880bc: e2843f5d     	add	r3, r4, #372
  4880c0: e1a00003     	mov	r0, r3
  4880c4: e5843184     	str	r3, [r4, #0x184]
  4880c8: e5843188     	str	r3, [r4, #0x188]
  4880cc: e3a01010     	mov	r1, #16
  4880d0: ebfa2569     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x176a5c
  4880d4: e5943184     	ldr	r3, [r4, #0x184]
  4880d8: e1a01005     	mov	r1, r5
  4880dc: e3a00fc6     	mov	r0, #792
  4880e0: e5c35000     	strb	r5, [r3]
  4880e4: e584518c     	str	r5, [r4, #0x18c]
  4880e8: ebfa2120     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x177b80
  4880ec: e3a01004     	mov	r1, #4
  4880f0: e1a05000     	mov	r5, r0
  4880f4: ebfdb42b     	bl	0x3f51a8 <LevelConfig::LevelConfig(ObjectBase::GO_IDS)> @ imm = #-0x92f54
  4880f8: e59f3030     	ldr	r3, [pc, #0x30]         @ 0x488130 <rnd::RandomGenerator::RandomGenerator()+0x154>
  4880fc: e584518c     	str	r5, [r4, #0x18c]
  488100: e08f3003     	add	r3, pc, r3
  488104: e5853020     	str	r3, [r5, #0x20]
  488108: e594018c     	ldr	r0, [r4, #0x18c]
  48810c: e2800004     	add	r0, r0, #4
  488110: eb022f18     	bl	0x513d78 <PropertyMap::InitProperties()> @ imm = #0x8bc60
  488114: e594018c     	ldr	r0, [r4, #0x18c]
  488118: e2800004     	add	r0, r0, #4
  48811c: eb022d72     	bl	0x5136ec <PropertyMap::LoadDefaultProperties()> @ imm = #0x8b5c8
  488120: e1a00004     	mov	r0, r4
  488124: e28dd00c     	add	sp, sp, #12
  488128: e8bd8030     	pop	{r4, r5, pc}
  48812c: 28 eb 43 00  	.word	0x0043eb28
  488130: 70 83 43 00  	.word	0x00438370
