
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00487e84 <rnd::RandomGenerator::RandomGenerator()>:
  487e84: e92d4030     	push	{r4, r5, lr}
  487e88: e1a04000     	mov	r4, r0
  487e8c: e24dd00c     	sub	sp, sp, #12
  487e90: e2800008     	add	r0, r0, #8
  487e94: e3a05000     	mov	r5, #0
  487e98: ebffffce     	bl	0x487dd8 <Array2d<rnd::Tile*>::Array2d()> @ imm = #-0xc8
  487e9c: e1a02004     	mov	r2, r4
  487ea0: e1a03004     	mov	r3, r4
  487ea4: e5845040     	str	r5, [r4, #0x40]
  487ea8: e5e2503c     	strb	r5, [r2, #0x3c]!
  487eac: e5842048     	str	r2, [r4, #0x48]
  487eb0: e5842044     	str	r2, [r4, #0x44]
  487eb4: e584504c     	str	r5, [r4, #0x4c]
  487eb8: e5845058     	str	r5, [r4, #0x58]
  487ebc: e5e35054     	strb	r5, [r3, #0x54]!
  487ec0: e5843060     	str	r3, [r4, #0x60]
  487ec4: e584305c     	str	r3, [r4, #0x5c]
  487ec8: e5845064     	str	r5, [r4, #0x64]
  487ecc: e1a01004     	mov	r1, r4
  487ed0: e284006c     	add	r0, r4, #108
  487ed4: eb001847     	bl	0x48dff8 <rnd::RootRule::RootRule(rnd::RandomGenerator&)> @ imm = #0x611c
  487ed8: e28430fc     	add	r3, r4, #252
  487edc: e1a00003     	mov	r0, r3
  487ee0: e584310c     	str	r3, [r4, #0x10c]
  487ee4: e5843110     	str	r3, [r4, #0x110]
  487ee8: e3a01010     	mov	r1, #16
  487eec: ebfa25e2     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x176878
  487ef0: e594210c     	ldr	r2, [r4, #0x10c]
  487ef4: e2843f4b     	add	r3, r4, #300
  487ef8: e1a00003     	mov	r0, r3
  487efc: e5c25000     	strb	r5, [r2]
  487f00: e3a01010     	mov	r1, #16
  487f04: e5845114     	str	r5, [r4, #0x114]
  487f08: e5845118     	str	r5, [r4, #0x118]
  487f0c: e584511c     	str	r5, [r4, #0x11c]
  487f10: e5845120     	str	r5, [r4, #0x120]
  487f14: e5845124     	str	r5, [r4, #0x124]
  487f18: e5845128     	str	r5, [r4, #0x128]
  487f1c: e584313c     	str	r3, [r4, #0x13c]
  487f20: e5843140     	str	r3, [r4, #0x140]
  487f24: ebfa25d4     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x1768b0
  487f28: e594213c     	ldr	r2, [r4, #0x13c]
  487f2c: e2843f51     	add	r3, r4, #324
  487f30: e1a00003     	mov	r0, r3
  487f34: e5c25000     	strb	r5, [r2]
  487f38: e3a01010     	mov	r1, #16
  487f3c: e5843154     	str	r3, [r4, #0x154]
  487f40: e5843158     	str	r3, [r4, #0x158]
  487f44: ebfa25cc     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x1768d0
  487f48: e5943154     	ldr	r3, [r4, #0x154]
  487f4c: e59f1080     	ldr	r1, [pc, #0x80]         @ 0x487fd4 <rnd::RandomGenerator::RandomGenerator()+0x150>
  487f50: e28d2004     	add	r2, sp, #4
  487f54: e5c35000     	strb	r5, [r3]
  487f58: e08f1001     	add	r1, pc, r1
  487f5c: e2840f57     	add	r0, r4, #348
  487f60: ebfa3061     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x173e7c
  487f64: e2843f5d     	add	r3, r4, #372
  487f68: e1a00003     	mov	r0, r3
  487f6c: e5843184     	str	r3, [r4, #0x184]
  487f70: e5843188     	str	r3, [r4, #0x188]
  487f74: e3a01010     	mov	r1, #16
  487f78: ebfa25bf     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x176904
  487f7c: e5943184     	ldr	r3, [r4, #0x184]
  487f80: e1a01005     	mov	r1, r5
  487f84: e3a00fc6     	mov	r0, #792
  487f88: e5c35000     	strb	r5, [r3]
  487f8c: e584518c     	str	r5, [r4, #0x18c]
  487f90: ebfa2176     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x177a28
  487f94: e3a01004     	mov	r1, #4
  487f98: e1a05000     	mov	r5, r0
  487f9c: ebfdb481     	bl	0x3f51a8 <LevelConfig::LevelConfig(ObjectBase::GO_IDS)> @ imm = #-0x92dfc
  487fa0: e59f3030     	ldr	r3, [pc, #0x30]         @ 0x487fd8 <rnd::RandomGenerator::RandomGenerator()+0x154>
  487fa4: e584518c     	str	r5, [r4, #0x18c]
  487fa8: e08f3003     	add	r3, pc, r3
  487fac: e5853020     	str	r3, [r5, #0x20]
  487fb0: e594018c     	ldr	r0, [r4, #0x18c]
  487fb4: e2800004     	add	r0, r0, #4
  487fb8: eb022f6e     	bl	0x513d78 <PropertyMap::InitProperties()> @ imm = #0x8bdb8
  487fbc: e594018c     	ldr	r0, [r4, #0x18c]
  487fc0: e2800004     	add	r0, r0, #4
  487fc4: eb022dc8     	bl	0x5136ec <PropertyMap::LoadDefaultProperties()> @ imm = #0x8b720
  487fc8: e1a00004     	mov	r0, r4
  487fcc: e28dd00c     	add	sp, sp, #12
  487fd0: e8bd8030     	pop	{r4, r5, pc}
  487fd4: 80 ec 43 00  	.word	0x0043ec80
  487fd8: c8 84 43 00  	.word	0x004384c8
