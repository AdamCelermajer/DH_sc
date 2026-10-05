
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0038cee8 <GameObject::DeclareProperties()>:
  38cee8: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  38ceec: e59f6208     	ldr	r6, [pc, #0x208]        @ 0x38d0fc <GameObject::DeclareProperties()+0x214>
  38cef0: e59f3208     	ldr	r3, [pc, #0x208]        @ 0x38d100 <GameObject::DeclareProperties()+0x218>
  38cef4: e24dd04c     	sub	sp, sp, #76
  38cef8: e08f6006     	add	r6, pc, r6
  38cefc: e7969003     	ldr	r9, [r6, r3]
  38cf00: e2805004     	add	r5, r0, #4
  38cf04: e1a04000     	mov	r4, r0
  38cf08: e5993000     	ldr	r3, [r9]
  38cf0c: e280bf9d     	add	r11, r0, #628
  38cf10: e59f81ec     	ldr	r8, [pc, #0x1ec]        @ 0x38d104 <GameObject::DeclareProperties()+0x21c>
  38cf14: e58d3044     	str	r3, [sp, #0x44]
  38cf18: ebfec83d     	bl	0x33f014 <ObjectBase::DeclareProperties()> @ imm = #-0x4df0c
  38cf1c: e59f31e4     	ldr	r3, [pc, #0x1e4]        @ 0x38d108 <GameObject::DeclareProperties()+0x220>
  38cf20: e59f11e4     	ldr	r1, [pc, #0x1e4]        @ 0x38d10c <GameObject::DeclareProperties()+0x224>
  38cf24: e1a00005     	mov	r0, r5
  38cf28: e7967003     	ldr	r7, [r6, r3]
  38cf2c: e08f1001     	add	r1, pc, r1
  38cf30: e2842e16     	add	r2, r4, #352
  38cf34: e597c000     	ldr	r12, [r7]
  38cf38: e597e004     	ldr	lr, [r7, #0x4]
  38cf3c: e597a008     	ldr	r10, [r7, #0x8]
  38cf40: e28d3018     	add	r3, sp, #24
  38cf44: e58dc018     	str	r12, [sp, #0x18]
  38cf48: e58de01c     	str	lr, [sp, #0x1c]
  38cf4c: e58da020     	str	r10, [sp, #0x20]
  38cf50: ebfff24f     	bl	0x389894 <void PropertyMap::AddProperty<Point3D<float>>(char const*, Point3D<float>&, Point3D<float>)> @ imm = #-0x36c4
  38cf54: e59f11b4     	ldr	r1, [pc, #0x1b4]        @ 0x38d110 <GameObject::DeclareProperties()+0x228>
  38cf58: e597e000     	ldr	lr, [r7]
  38cf5c: e597c004     	ldr	r12, [r7, #0x4]
  38cf60: e597a008     	ldr	r10, [r7, #0x8]
  38cf64: e28d300c     	add	r3, sp, #12
  38cf68: e08f1001     	add	r1, pc, r1
  38cf6c: e1a00005     	mov	r0, r5
  38cf70: e2842f5b     	add	r2, r4, #364
  38cf74: e58de00c     	str	lr, [sp, #0xc]
  38cf78: e58dc010     	str	r12, [sp, #0x10]
  38cf7c: e58da014     	str	r10, [sp, #0x14]
  38cf80: ebfff243     	bl	0x389894 <void PropertyMap::AddProperty<Point3D<float>>(char const*, Point3D<float>&, Point3D<float>)> @ imm = #-0x36f4
  38cf84: e59f1188     	ldr	r1, [pc, #0x188]        @ 0x38d114 <GameObject::DeclareProperties()+0x22c>
  38cf88: e1a00005     	mov	r0, r5
  38cf8c: e2842e29     	add	r2, r4, #656
  38cf90: e08f1001     	add	r1, pc, r1
  38cf94: ebfec7f8     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x4e020
  38cf98: e59f1178     	ldr	r1, [pc, #0x178]        @ 0x38d118 <GameObject::DeclareProperties()+0x230>
  38cf9c: e1a00005     	mov	r0, r5
  38cfa0: e2842faa     	add	r2, r4, #680
  38cfa4: e08f1001     	add	r1, pc, r1
  38cfa8: ebfec7f3     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x4e034
  38cfac: e59f1168     	ldr	r1, [pc, #0x168]        @ 0x38d11c <GameObject::DeclareProperties()+0x234>
  38cfb0: e1a00005     	mov	r0, r5
  38cfb4: e2842d0b     	add	r2, r4, #704
  38cfb8: e08f1001     	add	r1, pc, r1
  38cfbc: ebfec7ee     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x4e048
  38cfc0: e59f1158     	ldr	r1, [pc, #0x158]        @ 0x38d120 <GameObject::DeclareProperties()+0x238>
  38cfc4: e3a0c5fe     	mov	r12, #1065353216
  38cfc8: e1a00005     	mov	r0, r5
  38cfcc: e08f1001     	add	r1, pc, r1
  38cfd0: e2842e12     	add	r2, r4, #288
  38cfd4: e1a0300d     	mov	r3, sp
  38cfd8: e58dc008     	str	r12, [sp, #0x8]
  38cfdc: e58dc000     	str	r12, [sp]
  38cfe0: e58dc004     	str	r12, [sp, #0x4]
  38cfe4: ebfff22a     	bl	0x389894 <void PropertyMap::AddProperty<Point3D<float>>(char const*, Point3D<float>&, Point3D<float>)> @ imm = #-0x3758
  38cfe8: e59f1134     	ldr	r1, [pc, #0x134]        @ 0x38d124 <GameObject::DeclareProperties()+0x23c>
  38cfec: e2842fbb     	add	r2, r4, #748
  38cff0: e2822001     	add	r2, r2, #1
  38cff4: e08f1001     	add	r1, pc, r1
  38cff8: e1a00005     	mov	r0, r5
  38cffc: e3a03000     	mov	r3, #0
  38d000: ebfec529     	bl	0x33e4ac <void PropertyMap::AddProperty<bool>(char const*, bool&, bool)> @ imm = #-0x4eb5c
  38d004: e59f111c     	ldr	r1, [pc, #0x11c]        @ 0x38d128 <GameObject::DeclareProperties()+0x240>
  38d008: e3a03000     	mov	r3, #0
  38d00c: e2842f57     	add	r2, r4, #348
  38d010: e08f1001     	add	r1, pc, r1
  38d014: e1a00005     	mov	r0, r5
  38d018: ebfec523     	bl	0x33e4ac <void PropertyMap::AddProperty<bool>(char const*, bool&, bool)> @ imm = #-0x4eb74
  38d01c: e3a01000     	mov	r1, #0
  38d020: e3a00024     	mov	r0, #36
  38d024: ebfe0d51     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x7cabc
  38d028: e59f30fc     	ldr	r3, [pc, #0xfc]         @ 0x38d12c <GameObject::DeclareProperties()+0x244>
  38d02c: e08f8008     	add	r8, pc, r8
  38d030: e1a0a000     	mov	r10, r0
  38d034: e7963003     	ldr	r3, [r6, r3]
  38d038: e1a01008     	mov	r1, r8
  38d03c: e28d2024     	add	r2, sp, #36
  38d040: e2833008     	add	r3, r3, #8
  38d044: e4803008     	str	r3, [r0], #8
  38d048: ebfe1c27     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x78f64
  38d04c: e59f30dc     	ldr	r3, [pc, #0xdc]         @ 0x38d130 <GameObject::DeclareProperties()+0x248>
  38d050: e3a02064     	mov	r2, #100
  38d054: e065b00b     	rsb	r11, r5, r11
  38d058: e7963003     	ldr	r3, [r6, r3]
  38d05c: e58a2020     	str	r2, [r10, #0x20]
  38d060: e1a01008     	mov	r1, r8
  38d064: e2833008     	add	r3, r3, #8
  38d068: e58a3000     	str	r3, [r10]
  38d06c: e1a0200a     	mov	r2, r10
  38d070: e1a00005     	mov	r0, r5
  38d074: e58ab004     	str	r11, [r10, #0x4]
  38d078: eb061b19     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x186c64
  38d07c: e59f10b0     	ldr	r1, [pc, #0xb0]         @ 0x38d134 <GameObject::DeclareProperties()+0x24c>
  38d080: e28d702c     	add	r7, sp, #44
  38d084: e28d2028     	add	r2, sp, #40
  38d088: e08f1001     	add	r1, pc, r1
  38d08c: e1a00007     	mov	r0, r7
  38d090: ebfe1c15     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x78fac
  38d094: e59f109c     	ldr	r1, [pc, #0x9c]         @ 0x38d138 <GameObject::DeclareProperties()+0x250>
  38d098: e1a03007     	mov	r3, r7
  38d09c: e2842f9e     	add	r2, r4, #632
  38d0a0: e08f1001     	add	r1, pc, r1
  38d0a4: e1a00005     	mov	r0, r5
  38d0a8: ebfec4d5     	bl	0x33e404 <void PropertyMap::AddProperty<std::string>(char const*, std::string&, std::string)> @ imm = #-0x4ecac
  38d0ac: e1a00007     	mov	r0, r7
  38d0b0: ebfe2c67     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x74e64
  38d0b4: e59f1080     	ldr	r1, [pc, #0x80]         @ 0x38d13c <GameObject::DeclareProperties()+0x254>
  38d0b8: e1a00005     	mov	r0, r5
  38d0bc: e2842fd6     	add	r2, r4, #856
  38d0c0: e08f1001     	add	r1, pc, r1
  38d0c4: ebfec7ac     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x4e150
  38d0c8: e59f1070     	ldr	r1, [pc, #0x70]         @ 0x38d140 <GameObject::DeclareProperties()+0x258>
  38d0cc: e2842060     	add	r2, r4, #96
  38d0d0: e3a03001     	mov	r3, #1
  38d0d4: e1a00005     	mov	r0, r5
  38d0d8: e08f1001     	add	r1, pc, r1
  38d0dc: ebfec4f2     	bl	0x33e4ac <void PropertyMap::AddProperty<bool>(char const*, bool&, bool)> @ imm = #-0x4ec38
  38d0e0: e59d2044     	ldr	r2, [sp, #0x44]
  38d0e4: e5993000     	ldr	r3, [r9]
  38d0e8: e1520003     	cmp	r2, r3
  38d0ec: 1a000001     	bne	0x38d0f8 <GameObject::DeclareProperties()+0x210> @ imm = #0x4
  38d0f0: e28dd04c     	add	sp, sp, #76
  38d0f4: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  38d0f8: ebfe0484     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x7edf0
  38d0fc: 98 7b 60 00  	.word	0x00607b98
  38d100: ac 40 00 00  	.word	0x000040ac
  38d104: a4 54 53 00  	.word	0x005354a4
  38d108: 2c 3f 00 00  	.word	0x00003f2c
  38d10c: fc 52 53 00  	.word	0x005352fc
  38d110: 20 55 53 00  	.word	0x00535520
  38d114: 08 c9 53 00  	.word	0x0053c908
  38d118: f4 54 53 00  	.word	0x005354f4
  38d11c: f0 54 53 00  	.word	0x005354f0
  38d120: b4 81 55 00  	.word	0x005581b4
  38d124: bc 54 53 00  	.word	0x005354bc
  38d128: b0 54 53 00  	.word	0x005354b0
  38d12c: 30 23 00 00  	.word	0x00002330
  38d130: 90 25 00 00  	.word	0x00002590
  38d134: 58 54 53 00  	.word	0x00535458
  38d138: 50 54 53 00  	.word	0x00535450
  38d13c: 40 54 53 00  	.word	0x00535440
  38d140: 38 54 53 00  	.word	0x00535438
