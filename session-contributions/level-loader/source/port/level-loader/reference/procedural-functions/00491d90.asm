
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00491d90 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)>:
  491d90: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  491d94: e59f34f0     	ldr	r3, [pc, #0x4f0]        @ 0x49228c <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x4fc>
  491d98: e59fc4f0     	ldr	r12, [pc, #0x4f0]       @ 0x492290 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x500>
  491d9c: e24ddd17     	sub	sp, sp, #1472
  491da0: e24dd00c     	sub	sp, sp, #12
  491da4: e08f3003     	add	r3, pc, r3
  491da8: e58d301c     	str	r3, [sp, #0x1c]
  491dac: e793300c     	ldr	r3, [r3, r12]
  491db0: e28d6038     	add	r6, sp, #56
  491db4: e59f94d8     	ldr	r9, [pc, #0x4d8]        @ 0x492294 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x504>
  491db8: e5933000     	ldr	r3, [r3]
  491dbc: e246e004     	sub	lr, r6, #4
  491dc0: e1a04000     	mov	r4, r0
  491dc4: e1a07001     	mov	r7, r1
  491dc8: e1a0000e     	mov	r0, lr
  491dcc: e3a01005     	mov	r1, #5
  491dd0: e58de020     	str	lr, [sp, #0x20]
  491dd4: e1a05002     	mov	r5, r2
  491dd8: e58dc024     	str	r12, [sp, #0x24]
  491ddc: e58d35c4     	str	r3, [sp, #0x5c4]
  491de0: e08f9009     	add	r9, pc, r9
  491de4: ebfbe06f     	bl	0x389fa8 <Module::Module(ObjectBase::GO_IDS)> @ imm = #-0x107e44
  491de8: e1a00006     	mov	r0, r6
  491dec: e58d9054     	str	r9, [sp, #0x54]
  491df0: eb0207e0     	bl	0x513d78 <PropertyMap::InitProperties()> @ imm = #0x81f80
  491df4: e1a00006     	mov	r0, r6
  491df8: eb02063b     	bl	0x5136ec <PropertyMap::LoadDefaultProperties()> @ imm = #0x818ec
  491dfc: e59f1494     	ldr	r1, [pc, #0x494]        @ 0x492298 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x508>
  491e00: e28dae51     	add	r10, sp, #1296
  491e04: e28aa008     	add	r10, r10, #8
  491e08: e28d8e5a     	add	r8, sp, #1440
  491e0c: e1a03005     	mov	r3, r5
  491e10: e3a0c000     	mov	r12, #0
  491e14: e5942000     	ldr	r2, [r4]
  491e18: e288800c     	add	r8, r8, #12
  491e1c: e08f1001     	add	r1, pc, r1
  491e20: e1a0000a     	mov	r0, r10
  491e24: e5cdc5ac     	strb	r12, [sp, #0x5ac]
  491e28: e58d85bc     	str	r8, [sp, #0x5bc]
  491e2c: e58d85c0     	str	r8, [sp, #0x5c0]
  491e30: ebf9f32b     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x183354
  491e34: e1a0000a     	mov	r0, r10
  491e38: ebf9f005     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x183fec
  491e3c: e1a0100a     	mov	r1, r10
  491e40: e08a2000     	add	r2, r10, r0
  491e44: e1a00008     	mov	r0, r8
  491e48: ebfa3aff     	bl	0x320a4c <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_append(char const*, char const*)> @ imm = #-0x171404
  491e4c: e59f1448     	ldr	r1, [pc, #0x448]        @ 0x49229c <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x50c>
  491e50: e1a00006     	mov	r0, r6
  491e54: e59d25c0     	ldr	r2, [sp, #0x5c0]
  491e58: e08f1001     	add	r1, pc, r1
  491e5c: eb020686     	bl	0x51387c <PropertyMap::SetProperty(char const*, char const*)> @ imm = #0x81a18
  491e60: e59f1438     	ldr	r1, [pc, #0x438]        @ 0x4922a0 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x510>
  491e64: e1a02009     	mov	r2, r9
  491e68: e1a00006     	mov	r0, r6
  491e6c: e08f1001     	add	r1, pc, r1
  491e70: eb020681     	bl	0x51387c <PropertyMap::SetProperty(char const*, char const*)> @ imm = #0x81a04
  491e74: e59f1428     	ldr	r1, [pc, #0x428]        @ 0x4922a4 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x514>
  491e78: e59fc428     	ldr	r12, [pc, #0x428]       @ 0x4922a8 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x518>
  491e7c: e1a00008     	mov	r0, r8
  491e80: e08f1001     	add	r1, pc, r1
  491e84: e58dc010     	str	r12, [sp, #0x10]
  491e88: ebffff8c     	bl	0x491cc0 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_assign(char const*, char const*) (.clone.6)> @ imm = #-0x1d0
  491e8c: e5940084     	ldr	r0, [r4, #0x84]
  491e90: ebf9f2b3     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x183534
  491e94: e594b030     	ldr	r11, [r4, #0x30]
  491e98: e1a09000     	mov	r9, r0
  491e9c: e59dc010     	ldr	r12, [sp, #0x10]
  491ea0: e59b0054     	ldr	r0, [r11, #0x54]
  491ea4: e28dae4b     	add	r10, sp, #1200
  491ea8: e08fc00c     	add	r12, pc, r12
  491eac: e2400001     	sub	r0, r0, #1
  491eb0: e58dc010     	str	r12, [sp, #0x10]
  491eb4: ebf9f2aa     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x183558
  491eb8: e3a0143f     	mov	r1, #1056964608
  491ebc: ebf9f3aa     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x183158
  491ec0: e1a01000     	mov	r1, r0
  491ec4: e1a00009     	mov	r0, r9
  491ec8: ebf9f335     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x18332c
  491ecc: e59b104c     	ldr	r1, [r11, #0x4c]
  491ed0: ebf9f3a5     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x18316c
  491ed4: e1a02000     	mov	r2, r0
  491ed8: e5940088     	ldr	r0, [r4, #0x88]
  491edc: e58d2014     	str	r2, [sp, #0x14]
  491ee0: ebf9f29f     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x183584
  491ee4: e1a03000     	mov	r3, r0
  491ee8: e59b0058     	ldr	r0, [r11, #0x58]
  491eec: e58d3018     	str	r3, [sp, #0x18]
  491ef0: e28aa004     	add	r10, r10, #4
  491ef4: e2400001     	sub	r0, r0, #1
  491ef8: ebf9f299     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x18359c
  491efc: e3a0143f     	mov	r1, #1056964608
  491f00: ebf9f399     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x18319c
  491f04: e59d3018     	ldr	r3, [sp, #0x18]
  491f08: e1a01000     	mov	r1, r0
  491f0c: e28d9e59     	add	r9, sp, #1424
  491f10: e1a00003     	mov	r0, r3
  491f14: ebf9f322     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x183378
  491f18: e59b1050     	ldr	r1, [r11, #0x50]
  491f1c: e2899004     	add	r9, r9, #4
  491f20: e2811102     	add	r1, r1, #-2147483648
  491f24: ebf9f390     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x1831c0
  491f28: e59d2014     	ldr	r2, [sp, #0x14]
  491f2c: e1a0b000     	mov	r11, r0
  491f30: e1a00002     	mov	r0, r2
  491f34: ebf9f25a     	bl	0x30e8a4 <.plt+0xb30>   @ imm = #-0x183698
  491f38: e1cd02f8     	strd	r0, r1, [sp, #40]
  491f3c: e1a0000b     	mov	r0, r11
  491f40: ebf9f257     	bl	0x30e8a4 <.plt+0xb30>   @ imm = #-0x1836a4
  491f44: e594308c     	ldr	r3, [r4, #0x8c]
  491f48: e1cd00f0     	strd	r0, r1, [sp]
  491f4c: e1a00003     	mov	r0, r3
  491f50: ebf9f253     	bl	0x30e8a4 <.plt+0xb30>   @ imm = #-0x1836b4
  491f54: e1cd22d8     	ldrd	r2, r3, [sp, #40]
  491f58: e59dc010     	ldr	r12, [sp, #0x10]
  491f5c: e1cd00f8     	strd	r0, r1, [sp, #8]
  491f60: e1a0100c     	mov	r1, r12
  491f64: e1a0000a     	mov	r0, r10
  491f68: ebf9f2dd     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x18348c
  491f6c: e1a0000a     	mov	r0, r10
  491f70: ebf9efb7     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x184124
  491f74: e1a0100a     	mov	r1, r10
  491f78: e08a2000     	add	r2, r10, r0
  491f7c: e1a00008     	mov	r0, r8
  491f80: ebfa3ab1     	bl	0x320a4c <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_append(char const*, char const*)> @ imm = #-0x17153c
  491f84: e59f1320     	ldr	r1, [pc, #0x320]        @ 0x4922ac <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x51c>
  491f88: e59d25c0     	ldr	r2, [sp, #0x5c0]
  491f8c: e1a00006     	mov	r0, r6
  491f90: e08f1001     	add	r1, pc, r1
  491f94: eb020638     	bl	0x51387c <PropertyMap::SetProperty(char const*, char const*)> @ imm = #0x818e0
  491f98: e1a00004     	mov	r0, r4
  491f9c: e59d1020     	ldr	r1, [sp, #0x20]
  491fa0: ebfffd5c     	bl	0x491518 <rnd::Tile::SetModuleMVXProperties(Module*)> @ imm = #-0xa90
  491fa4: e59f1304     	ldr	r1, [pc, #0x304]        @ 0x4922b0 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x520>
  491fa8: e2462008     	sub	r2, r6, #8
  491fac: e1a00009     	mov	r0, r9
  491fb0: e08f1001     	add	r1, pc, r1
  491fb4: ebfa084c     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x17ded0
  491fb8: e5943030     	ldr	r3, [r4, #0x30]
  491fbc: e1a00009     	mov	r0, r9
  491fc0: e5932044     	ldr	r2, [r3, #0x44]
  491fc4: e5931048     	ldr	r1, [r3, #0x48]
  491fc8: ebf9fa0d     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0x1817cc
  491fcc: e5943030     	ldr	r3, [r4, #0x30]
  491fd0: e5932030     	ldr	r2, [r3, #0x30]
  491fd4: e593302c     	ldr	r3, [r3, #0x2c]
  491fd8: e0531002     	subs	r1, r3, r2
  491fdc: 1a00007c     	bne	0x4921d4 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x444> @ imm = #0x1f0
  491fe0: e3a0b004     	mov	r11, #4
  491fe4: e59f12c8     	ldr	r1, [pc, #0x2c8]        @ 0x4922b4 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x524>
  491fe8: e1a00009     	mov	r0, r9
  491fec: e28dae57     	add	r10, sp, #1392
  491ff0: e08f1001     	add	r1, pc, r1
  491ff4: e2812001     	add	r2, r1, #1
  491ff8: ebf9fa01     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0x1817fc
  491ffc: e5943030     	ldr	r3, [r4, #0x30]
  492000: e28aa00c     	add	r10, r10, #12
  492004: e58da58c     	str	r10, [sp, #0x58c]
  492008: e58da590     	str	r10, [sp, #0x590]
  49200c: e593202c     	ldr	r2, [r3, #0x2c]
  492010: e5931030     	ldr	r1, [r3, #0x30]
  492014: e0612002     	rsb	r2, r1, r2
  492018: e15b0002     	cmp	r11, r2
  49201c: 8a000082     	bhi	0x49222c <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x49c> @ imm = #0x208
  492020: e0812002     	add	r2, r1, r2
  492024: e1a0000a     	mov	r0, r10
  492028: e081100b     	add	r1, r1, r11
  49202c: ebf9fdad     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0x18094c
  492030: e59d258c     	ldr	r2, [sp, #0x58c]
  492034: e59d1590     	ldr	r1, [sp, #0x590]
  492038: e1a00009     	mov	r0, r9
  49203c: ebf9f9f0     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0x181840
  492040: e1a0000a     	mov	r0, r10
  492044: ebfa0658     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x17e6a0
  492048: e5942064     	ldr	r2, [r4, #0x64]
  49204c: e5943060     	ldr	r3, [r4, #0x60]
  492050: e1520003     	cmp	r2, r3
  492054: 0a000018     	beq	0x4920bc <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x32c> @ imm = #0x60
  492058: e59f1258     	ldr	r1, [pc, #0x258]        @ 0x4922b8 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x528>
  49205c: e1a00008     	mov	r0, r8
  492060: e28dae45     	add	r10, sp, #1104
  492064: e08f1001     	add	r1, pc, r1
  492068: ebffff14     	bl	0x491cc0 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_assign(char const*, char const*) (.clone.6)> @ imm = #-0x3b0
  49206c: e59f1248     	ldr	r1, [pc, #0x248]        @ 0x4922bc <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x52c>
  492070: e59f3248     	ldr	r3, [pc, #0x248]        @ 0x4922c0 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x530>
  492074: e594c064     	ldr	r12, [r4, #0x64]
  492078: e08f1001     	add	r1, pc, r1
  49207c: e59d25a8     	ldr	r2, [sp, #0x5a8]
  492080: e08f3003     	add	r3, pc, r3
  492084: e1a0000a     	mov	r0, r10
  492088: e58dc000     	str	r12, [sp]
  49208c: ebf9f294     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x1835b0
  492090: e1a0000a     	mov	r0, r10
  492094: ebf9ef6e     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x184248
  492098: e1a0100a     	mov	r1, r10
  49209c: e08a2000     	add	r2, r10, r0
  4920a0: e1a00008     	mov	r0, r8
  4920a4: ebfa3a68     	bl	0x320a4c <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_append(char const*, char const*)> @ imm = #-0x171660
  4920a8: e59f1214     	ldr	r1, [pc, #0x214]        @ 0x4922c4 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x534>
  4920ac: e1a00006     	mov	r0, r6
  4920b0: e59d25c0     	ldr	r2, [sp, #0x5c0]
  4920b4: e08f1001     	add	r1, pc, r1
  4920b8: eb0205ef     	bl	0x51387c <PropertyMap::SetProperty(char const*, char const*)> @ imm = #0x817bc
  4920bc: e594207c     	ldr	r2, [r4, #0x7c]
  4920c0: e5943078     	ldr	r3, [r4, #0x78]
  4920c4: e1520003     	cmp	r2, r3
  4920c8: 0a000018     	beq	0x492130 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x3a0> @ imm = #0x60
  4920cc: e59f11f4     	ldr	r1, [pc, #0x1f4]        @ 0x4922c8 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x538>
  4920d0: e1a00008     	mov	r0, r8
  4920d4: e28dae45     	add	r10, sp, #1104
  4920d8: e08f1001     	add	r1, pc, r1
  4920dc: ebfffef7     	bl	0x491cc0 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_assign(char const*, char const*) (.clone.6)> @ imm = #-0x424
  4920e0: e59f11e4     	ldr	r1, [pc, #0x1e4]        @ 0x4922cc <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x53c>
  4920e4: e59f31e4     	ldr	r3, [pc, #0x1e4]        @ 0x4922d0 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x540>
  4920e8: e594c07c     	ldr	r12, [r4, #0x7c]
  4920ec: e08f1001     	add	r1, pc, r1
  4920f0: e59d25a8     	ldr	r2, [sp, #0x5a8]
  4920f4: e08f3003     	add	r3, pc, r3
  4920f8: e1a0000a     	mov	r0, r10
  4920fc: e58dc000     	str	r12, [sp]
  492100: ebf9f277     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x183624
  492104: e1a0000a     	mov	r0, r10
  492108: ebf9ef51     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x1842bc
  49210c: e1a0100a     	mov	r1, r10
  492110: e08a2000     	add	r2, r10, r0
  492114: e1a00008     	mov	r0, r8
  492118: ebfa3a4b     	bl	0x320a4c <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_append(char const*, char const*)> @ imm = #-0x1716d4
  49211c: e59f11b0     	ldr	r1, [pc, #0x1b0]        @ 0x4922d4 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x544>
  492120: e1a00006     	mov	r0, r6
  492124: e59d25c0     	ldr	r2, [sp, #0x5c0]
  492128: e08f1001     	add	r1, pc, r1
  49212c: eb0205d2     	bl	0x51387c <PropertyMap::SetProperty(char const*, char const*)> @ imm = #0x81748
  492130: e1a00006     	mov	r0, r6
  492134: e1a01007     	mov	r1, r7
  492138: e3a02000     	mov	r2, #0
  49213c: eb0204eb     	bl	0x5134f0 <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)> @ imm = #0x813ac
  492140: e594300c     	ldr	r3, [r4, #0xc]
  492144: e3530000     	cmp	r3, #0
  492148: da00000c     	ble	0x492180 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x3f0> @ imm = #0x30
  49214c: e1a0a004     	mov	r10, r4
  492150: e3a06000     	mov	r6, #0
  492154: e1a00005     	mov	r0, r5
  492158: e2802001     	add	r2, r0, #1
  49215c: e1a01007     	mov	r1, r7
  492160: e59a0010     	ldr	r0, [r10, #0x10]
  492164: ebffff09     	bl	0x491d90 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)> @ imm = #-0x3dc
  492168: e594300c     	ldr	r3, [r4, #0xc]
  49216c: e2866001     	add	r6, r6, #1
  492170: e28aa004     	add	r10, r10, #4
  492174: e1530006     	cmp	r3, r6
  492178: cafffff6     	bgt	0x492158 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x3c8> @ imm = #-0x28
  49217c: e1a05000     	mov	r5, r0
  492180: e1a00009     	mov	r0, r9
  492184: ebfa0608     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x17e7e0
  492188: e59d05c0     	ldr	r0, [sp, #0x5c0]
  49218c: e1500008     	cmp	r0, r8
  492190: 0a000002     	beq	0x4921a0 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x410> @ imm = #0x8
  492194: e3500000     	cmp	r0, #0
  492198: 0a000000     	beq	0x4921a0 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x410> @ imm = #0x0
  49219c: ebf9f8ab     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x181d54
  4921a0: e59d0020     	ldr	r0, [sp, #0x20]
  4921a4: ebfbdca6     	bl	0x389444 <Module::~Module()> @ imm = #-0x108d68
  4921a8: e59d201c     	ldr	r2, [sp, #0x1c]
  4921ac: e59d1024     	ldr	r1, [sp, #0x24]
  4921b0: e1a00005     	mov	r0, r5
  4921b4: e7923001     	ldr	r3, [r2, r1]
  4921b8: e59d25c4     	ldr	r2, [sp, #0x5c4]
  4921bc: e5933000     	ldr	r3, [r3]
  4921c0: e1520003     	cmp	r2, r3
  4921c4: 1a00002f     	bne	0x492288 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x4f8> @ imm = #0xbc
  4921c8: e28ddf73     	add	sp, sp, #460
  4921cc: e28ddb01     	add	sp, sp, #1024
  4921d0: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  4921d4: e3510004     	cmp	r1, #4
  4921d8: 9affff80     	bls	0x491fe0 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x250> @ imm = #-0x200
  4921dc: e1530002     	cmp	r3, r2
  4921e0: 0a00000b     	beq	0x492214 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x484> @ imm = #0x2c
  4921e4: e59f10ec     	ldr	r1, [pc, #0xec]         @ 0x4922d8 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x548>
  4921e8: e282a001     	add	r10, r2, #1
  4921ec: e08f1001     	add	r1, pc, r1
  4921f0: e281b005     	add	r11, r1, #5
  4921f4: e2811002     	add	r1, r1, #2
  4921f8: e58d1028     	str	r1, [sp, #0x28]
  4921fc: e15a10d1     	ldrsb	r1, [r10, #-1]
  492200: e3510064     	cmp	r1, #100
  492204: 0a00000c     	beq	0x49223c <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x4ac> @ imm = #0x30
  492208: e153000a     	cmp	r3, r10
  49220c: e28aa001     	add	r10, r10, #1
  492210: 1afffff9     	bne	0x4921fc <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x46c> @ imm = #-0x1c
  492214: e1a01003     	mov	r1, r3
  492218: e1530001     	cmp	r3, r1
  49221c: 10622001     	rsbne	r2, r2, r1
  492220: 1282b005     	addne	r11, r2, #5
  492224: 1affff6e     	bne	0x491fe4 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x254> @ imm = #-0x248
  492228: eaffff6c     	b	0x491fe0 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x250> @ imm = #-0x250
  49222c: e59f00a8     	ldr	r0, [pc, #0xa8]         @ 0x4922dc <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x54c>
  492230: e08f0000     	add	r0, pc, r0
  492234: eb09db1d     	bl	0x708eb0 <___ZSt24__stl_throw_out_of_rangePKc_veneer> @ imm = #0x276c74
  492238: eaffff7c     	b	0x492030 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x2a0> @ imm = #-0x210
  49223c: e153000a     	cmp	r3, r10
  492240: e1a0100a     	mov	r1, r10
  492244: 0afffff2     	beq	0x492214 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x484> @ imm = #-0x38
  492248: e59d0028     	ldr	r0, [sp, #0x28]
  49224c: ea000005     	b	0x492268 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x4d8> @ imm = #0x14
  492250: e150000b     	cmp	r0, r11
  492254: 0a000009     	beq	0x492280 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x4f0> @ imm = #0x24
  492258: e2811001     	add	r1, r1, #1
  49225c: e1510003     	cmp	r1, r3
  492260: e2800001     	add	r0, r0, #1
  492264: 0affffeb     	beq	0x492218 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x488> @ imm = #-0x54
  492268: e1d1e0d0     	ldrsb	lr, [r1]
  49226c: e150c0d1     	ldrsb	r12, [r0, #-1]
  492270: e15e000c     	cmp	lr, r12
  492274: 0afffff5     	beq	0x492250 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x4c0> @ imm = #-0x2c
  492278: e28aa001     	add	r10, r10, #1
  49227c: eaffffde     	b	0x4921fc <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x46c> @ imm = #-0x88
  492280: e24a1001     	sub	r1, r10, #1
  492284: eaffffe3     	b	0x492218 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)+0x488> @ imm = #-0x74
  492288: ebf9f020     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x183f80
  49228c: ec 2c 50 00  	.word	0x00502cec
  492290: ac 40 00 00  	.word	0x000040ac
  492294: 50 e6 42 00  	.word	0x0042e650
  492298: 8c 7a 43 00  	.word	0x00437a8c
  49229c: 90 f2 44 00  	.word	0x0044f290
  4922a0: 0c e3 42 00  	.word	0x0042e30c
  4922a4: 88 99 43 00  	.word	0x00439988
  4922a8: c8 c4 42 00  	.word	0x0042c4c8
  4922ac: 98 02 43 00  	.word	0x00430298
  4922b0: 20 4c 43 00  	.word	0x00434c20
  4922b4: 68 ec 42 00  	.word	0x0042ec68
  4922b8: a4 97 43 00  	.word	0x004397a4
  4922bc: 68 2f 44 00  	.word	0x00442f68
  4922c0: c0 47 43 00  	.word	0x004347c0
  4922c4: 5c 02 43 00  	.word	0x0043025c
  4922c8: 30 97 43 00  	.word	0x00439730
  4922cc: f4 2e 44 00  	.word	0x00442ef4
  4922d0: 54 47 43 00  	.word	0x00434754
  4922d4: f0 01 43 00  	.word	0x004301f0
  4922d8: e4 49 43 00  	.word	0x004349e4
  4922dc: 28 c2 42 00  	.word	0x0042c228
