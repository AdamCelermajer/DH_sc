
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040bfc4 <LightPoint::DeclareProperties()>:
  40bfc4: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  40bfc8: e59f4188     	ldr	r4, [pc, #0x188]        @ 0x40c158 <LightPoint::DeclareProperties()+0x194>
  40bfcc: e59f3188     	ldr	r3, [pc, #0x188]        @ 0x40c15c <LightPoint::DeclareProperties()+0x198>
  40bfd0: e24dd04c     	sub	sp, sp, #76
  40bfd4: e08f4004     	add	r4, pc, r4
  40bfd8: e794c003     	ldr	r12, [r4, r3]
  40bfdc: e28d702c     	add	r7, sp, #44
  40bfe0: e1a0b000     	mov	r11, r0
  40bfe4: e59c3000     	ldr	r3, [r12]
  40bfe8: e58dc000     	str	r12, [sp]
  40bfec: e3a09000     	mov	r9, #0
  40bff0: e58d3044     	str	r3, [sp, #0x44]
  40bff4: ebfffb7d     	bl	0x40adf0 <LightBase::DeclareProperties()> @ imm = #-0x120c
  40bff8: e1a00007     	mov	r0, r7
  40bffc: e3a01010     	mov	r1, #16
  40c000: e58d703c     	str	r7, [sp, #0x3c]
  40c004: e58d7040     	str	r7, [sp, #0x40]
  40c008: ebfc159b     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0xfa994
  40c00c: e59d303c     	ldr	r3, [sp, #0x3c]
  40c010: e28d8014     	add	r8, sp, #20
  40c014: e1a00008     	mov	r0, r8
  40c018: e5c39000     	strb	r9, [r3]
  40c01c: e59d203c     	ldr	r2, [sp, #0x3c]
  40c020: e59d1040     	ldr	r1, [sp, #0x40]
  40c024: e58d8024     	str	r8, [sp, #0x24]
  40c028: e58d8028     	str	r8, [sp, #0x28]
  40c02c: ebfc15ad     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0xfa94c
  40c030: e1a01009     	mov	r1, r9
  40c034: e3a00038     	mov	r0, #56
  40c038: ebfc114c     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xfbad0
  40c03c: e59f311c     	ldr	r3, [pc, #0x11c]        @ 0x40c160 <LightPoint::DeclareProperties()+0x19c>
  40c040: e59fa11c     	ldr	r10, [pc, #0x11c]       @ 0x40c164 <LightPoint::DeclareProperties()+0x1a0>
  40c044: e1a05000     	mov	r5, r0
  40c048: e7943003     	ldr	r3, [r4, r3]
  40c04c: e08fa00a     	add	r10, pc, r10
  40c050: e1a0100a     	mov	r1, r10
  40c054: e2833008     	add	r3, r3, #8
  40c058: e4803008     	str	r3, [r0], #8
  40c05c: e28d2010     	add	r2, sp, #16
  40c060: e58d3004     	str	r3, [sp, #0x4]
  40c064: ebfc2020     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xf7f80
  40c068: e59f20f8     	ldr	r2, [pc, #0xf8]         @ 0x40c168 <LightPoint::DeclareProperties()+0x1a4>
  40c06c: e28b6004     	add	r6, r11, #4
  40c070: e28b1d06     	add	r1, r11, #384
  40c074: e7942002     	ldr	r2, [r4, r2]
  40c078: e1a00005     	mov	r0, r5
  40c07c: e0661001     	rsb	r1, r6, r1
  40c080: e2822008     	add	r2, r2, #8
  40c084: e5851004     	str	r1, [r5, #0x4]
  40c088: e4802020     	str	r2, [r0], #32
  40c08c: e5850030     	str	r0, [r5, #0x30]
  40c090: e5850034     	str	r0, [r5, #0x34]
  40c094: e59d1028     	ldr	r1, [sp, #0x28]
  40c098: e59d2024     	ldr	r2, [sp, #0x24]
  40c09c: ebfc1591     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0xfa9bc
  40c0a0: e1a0100a     	mov	r1, r10
  40c0a4: e1a02005     	mov	r2, r5
  40c0a8: e1a00006     	mov	r0, r6
  40c0ac: eb041f0c     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x107c30
  40c0b0: e1a00008     	mov	r0, r8
  40c0b4: ebfc1e3c     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xf8710
  40c0b8: e1a00007     	mov	r0, r7
  40c0bc: ebfc1e3a     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xf8718
  40c0c0: e59f20a4     	ldr	r2, [pc, #0xa4]         @ 0x40c16c <LightPoint::DeclareProperties()+0x1a8>
  40c0c4: e1a01009     	mov	r1, r9
  40c0c8: e3a0002c     	mov	r0, #44
  40c0cc: e7942002     	ldr	r2, [r4, r2]
  40c0d0: e59f7098     	ldr	r7, [pc, #0x98]         @ 0x40c170 <LightPoint::DeclareProperties()+0x1ac>
  40c0d4: e28bbf69     	add	r11, r11, #420
  40c0d8: e5929008     	ldr	r9, [r2, #0x8]
  40c0dc: e5928000     	ldr	r8, [r2]
  40c0e0: e592a004     	ldr	r10, [r2, #0x4]
  40c0e4: ebfc1121     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xfbb7c
  40c0e8: e59d3004     	ldr	r3, [sp, #0x4]
  40c0ec: e08f7007     	add	r7, pc, r7
  40c0f0: e1a05000     	mov	r5, r0
  40c0f4: e1a01007     	mov	r1, r7
  40c0f8: e4803008     	str	r3, [r0], #8
  40c0fc: e28d200c     	add	r2, sp, #12
  40c100: ebfc1ff9     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xf801c
  40c104: e59f3068     	ldr	r3, [pc, #0x68]         @ 0x40c174 <LightPoint::DeclareProperties()+0x1b0>
  40c108: e066b00b     	rsb	r11, r6, r11
  40c10c: e1a02005     	mov	r2, r5
  40c110: e7943003     	ldr	r3, [r4, r3]
  40c114: e585b004     	str	r11, [r5, #0x4]
  40c118: e5858020     	str	r8, [r5, #0x20]
  40c11c: e2833008     	add	r3, r3, #8
  40c120: e5853000     	str	r3, [r5]
  40c124: e585a024     	str	r10, [r5, #0x24]
  40c128: e5859028     	str	r9, [r5, #0x28]
  40c12c: e1a00006     	mov	r0, r6
  40c130: e1a01007     	mov	r1, r7
  40c134: eb041eea     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x107ba8
  40c138: e59dc000     	ldr	r12, [sp]
  40c13c: e59d2044     	ldr	r2, [sp, #0x44]
  40c140: e59c3000     	ldr	r3, [r12]
  40c144: e1520003     	cmp	r2, r3
  40c148: 1a000001     	bne	0x40c154 <LightPoint::DeclareProperties()+0x190> @ imm = #0x4
  40c14c: e28dd04c     	add	sp, sp, #76
  40c150: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  40c154: ebfc086d     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xfde4c
  40c158: bc 8a 58 00  	.word	0x00588abc
  40c15c: ac 40 00 00  	.word	0x000040ac
  40c160: 30 23 00 00  	.word	0x00002330
  40c164: dc bb 4b 00  	.word	0x004bbbdc
  40c168: 94 34 00 00  	.word	0x00003494
  40c16c: 2c 3f 00 00  	.word	0x00003f2c
  40c170: 4c bb 4b 00  	.word	0x004bbb4c
  40c174: 44 0b 00 00  	.word	0x00000b44
