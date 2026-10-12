
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040adf0 <LightBase::DeclareProperties()>:
  40adf0: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  40adf4: e59f5288     	ldr	r5, [pc, #0x288]        @ 0x40b084 <LightBase::DeclareProperties()+0x294>
  40adf8: e59f3288     	ldr	r3, [pc, #0x288]        @ 0x40b088 <LightBase::DeclareProperties()+0x298>
  40adfc: e59f1288     	ldr	r1, [pc, #0x288]        @ 0x40b08c <LightBase::DeclareProperties()+0x29c>
  40ae00: e08f5005     	add	r5, pc, r5
  40ae04: e795c003     	ldr	r12, [r5, r3]
  40ae08: e59f3280     	ldr	r3, [pc, #0x280]        @ 0x40b090 <LightBase::DeclareProperties()+0x2a0>
  40ae0c: e24dd08c     	sub	sp, sp, #140
  40ae10: e59c7000     	ldr	r7, [r12]
  40ae14: e7953003     	ldr	r3, [r5, r3]
  40ae18: e2804004     	add	r4, r0, #4
  40ae1c: e1a06000     	mov	r6, r0
  40ae20: e593e000     	ldr	lr, [r3]
  40ae24: e593a008     	ldr	r10, [r3, #0x8]
  40ae28: e5938004     	ldr	r8, [r3, #0x4]
  40ae2c: e2802f49     	add	r2, r0, #292
  40ae30: e28d303c     	add	r3, sp, #60
  40ae34: e08f1001     	add	r1, pc, r1
  40ae38: e1a00004     	mov	r0, r4
  40ae3c: e58dc004     	str	r12, [sp, #0x4]
  40ae40: e58de03c     	str	lr, [sp, #0x3c]
  40ae44: e58d7084     	str	r7, [sp, #0x84]
  40ae48: e58d8040     	str	r8, [sp, #0x40]
  40ae4c: e58da044     	str	r10, [sp, #0x44]
  40ae50: ebfdfa8f     	bl	0x389894 <void PropertyMap::AddProperty<Point3D<float>>(char const*, Point3D<float>&, Point3D<float>)> @ imm = #-0x815c4
  40ae54: e3a01000     	mov	r1, #0
  40ae58: e3a00024     	mov	r0, #36
  40ae5c: ebfc15c3     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xfa8f4
  40ae60: e59fb22c     	ldr	r11, [pc, #0x22c]       @ 0x40b094 <LightBase::DeclareProperties()+0x2a4>
  40ae64: e59f822c     	ldr	r8, [pc, #0x22c]        @ 0x40b098 <LightBase::DeclareProperties()+0x2a8>
  40ae68: e1a07000     	mov	r7, r0
  40ae6c: e795b00b     	ldr	r11, [r5, r11]
  40ae70: e08f8008     	add	r8, pc, r8
  40ae74: e1a01008     	mov	r1, r8
  40ae78: e28bb008     	add	r11, r11, #8
  40ae7c: e28d2050     	add	r2, sp, #80
  40ae80: e480b008     	str	r11, [r0], #8
  40ae84: ebfc2498     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xf6da0
  40ae88: e59f320c     	ldr	r3, [pc, #0x20c]        @ 0x40b09c <LightBase::DeclareProperties()+0x2ac>
  40ae8c: e2862e13     	add	r2, r6, #304
  40ae90: e0642002     	rsb	r2, r4, r2
  40ae94: e7953003     	ldr	r3, [r5, r3]
  40ae98: e5872004     	str	r2, [r7, #0x4]
  40ae9c: e1a01008     	mov	r1, r8
  40aea0: e2833008     	add	r3, r3, #8
  40aea4: e5873000     	str	r3, [r7]
  40aea8: e3a03101     	mov	r3, #1073741824
  40aeac: e5873020     	str	r3, [r7, #0x20]
  40aeb0: e1a02007     	mov	r2, r7
  40aeb4: e1a00004     	mov	r0, r4
  40aeb8: eb042389     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x108e24
  40aebc: e59f11dc     	ldr	r1, [pc, #0x1dc]        @ 0x40b0a0 <LightBase::DeclareProperties()+0x2b0>
  40aec0: e3a07000     	mov	r7, #0
  40aec4: e2862f4d     	add	r2, r6, #308
  40aec8: e1a00004     	mov	r0, r4
  40aecc: e08f1001     	add	r1, pc, r1
  40aed0: e28d3030     	add	r3, sp, #48
  40aed4: e58d7030     	str	r7, [sp, #0x30]
  40aed8: e58d7034     	str	r7, [sp, #0x34]
  40aedc: e58d7038     	str	r7, [sp, #0x38]
  40aee0: ebfdfa6b     	bl	0x389894 <void PropertyMap::AddProperty<Point3D<float>>(char const*, Point3D<float>&, Point3D<float>)> @ imm = #-0x81654
  40aee4: e59f11b8     	ldr	r1, [pc, #0x1b8]        @ 0x40b0a4 <LightBase::DeclareProperties()+0x2b4>
  40aee8: e2862d05     	add	r2, r6, #320
  40aeec: e1a00004     	mov	r0, r4
  40aef0: e08f1001     	add	r1, pc, r1
  40aef4: e28d3024     	add	r3, sp, #36
  40aef8: e58d7024     	str	r7, [sp, #0x24]
  40aefc: e58d7028     	str	r7, [sp, #0x28]
  40af00: e58d702c     	str	r7, [sp, #0x2c]
  40af04: ebfdfa62     	bl	0x389894 <void PropertyMap::AddProperty<Point3D<float>>(char const*, Point3D<float>&, Point3D<float>)> @ imm = #-0x81678
  40af08: e59f1198     	ldr	r1, [pc, #0x198]        @ 0x40b0a8 <LightBase::DeclareProperties()+0x2b8>
  40af0c: e2862f53     	add	r2, r6, #332
  40af10: e1a00004     	mov	r0, r4
  40af14: e08f1001     	add	r1, pc, r1
  40af18: e28d3018     	add	r3, sp, #24
  40af1c: e58d7018     	str	r7, [sp, #0x18]
  40af20: e58d701c     	str	r7, [sp, #0x1c]
  40af24: e58d7020     	str	r7, [sp, #0x20]
  40af28: ebfdfa59     	bl	0x389894 <void PropertyMap::AddProperty<Point3D<float>>(char const*, Point3D<float>&, Point3D<float>)> @ imm = #-0x8169c
  40af2c: e59f1178     	ldr	r1, [pc, #0x178]        @ 0x40b0ac <LightBase::DeclareProperties()+0x2bc>
  40af30: e28d300c     	add	r3, sp, #12
  40af34: e2862f56     	add	r2, r6, #344
  40af38: e08f1001     	add	r1, pc, r1
  40af3c: e1a00004     	mov	r0, r4
  40af40: e58d7014     	str	r7, [sp, #0x14]
  40af44: e58d700c     	str	r7, [sp, #0xc]
  40af48: e58d7010     	str	r7, [sp, #0x10]
  40af4c: ebfdfa50     	bl	0x389894 <void PropertyMap::AddProperty<Point3D<float>>(char const*, Point3D<float>&, Point3D<float>)> @ imm = #-0x816c0
  40af50: e3a01000     	mov	r1, #0
  40af54: e3a00024     	mov	r0, #36
  40af58: ebfc1584     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xfa9f0
  40af5c: e59fa14c     	ldr	r10, [pc, #0x14c]       @ 0x40b0b0 <LightBase::DeclareProperties()+0x2c0>
  40af60: e1a08000     	mov	r8, r0
  40af64: e28d204c     	add	r2, sp, #76
  40af68: e08fa00a     	add	r10, pc, r10
  40af6c: e1a0100a     	mov	r1, r10
  40af70: e480b008     	str	r11, [r0], #8
  40af74: ebfc245c     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xf6e90
  40af78: e59f3134     	ldr	r3, [pc, #0x134]        @ 0x40b0b4 <LightBase::DeclareProperties()+0x2c4>
  40af7c: e2862f59     	add	r2, r6, #356
  40af80: e3a07000     	mov	r7, #0
  40af84: e7953003     	ldr	r3, [r5, r3]
  40af88: e0642002     	rsb	r2, r4, r2
  40af8c: e28d906c     	add	r9, sp, #108
  40af90: e2833008     	add	r3, r3, #8
  40af94: e5883000     	str	r3, [r8]
  40af98: e5882004     	str	r2, [r8, #0x4]
  40af9c: e1a0100a     	mov	r1, r10
  40afa0: e1a02008     	mov	r2, r8
  40afa4: e5c87020     	strb	r7, [r8, #0x20]
  40afa8: e1a00004     	mov	r0, r4
  40afac: eb04234c     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x108d30
  40afb0: e1a00009     	mov	r0, r9
  40afb4: e3a01010     	mov	r1, #16
  40afb8: e58d907c     	str	r9, [sp, #0x7c]
  40afbc: e58d9080     	str	r9, [sp, #0x80]
  40afc0: ebfc19ad     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0xf994c
  40afc4: e59d307c     	ldr	r3, [sp, #0x7c]
  40afc8: e28d8054     	add	r8, sp, #84
  40afcc: e1a00008     	mov	r0, r8
  40afd0: e5c37000     	strb	r7, [r3]
  40afd4: e59d207c     	ldr	r2, [sp, #0x7c]
  40afd8: e59d1080     	ldr	r1, [sp, #0x80]
  40afdc: e58d8064     	str	r8, [sp, #0x64]
  40afe0: e58d8068     	str	r8, [sp, #0x68]
  40afe4: ebfc19bf     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0xf9904
  40afe8: e1a01007     	mov	r1, r7
  40afec: e3a00038     	mov	r0, #56
  40aff0: ebfc155e     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xfaa88
  40aff4: e59fa0bc     	ldr	r10, [pc, #0xbc]        @ 0x40b0b8 <LightBase::DeclareProperties()+0x2c8>
  40aff8: e1a07000     	mov	r7, r0
  40affc: e28d2048     	add	r2, sp, #72
  40b000: e08fa00a     	add	r10, pc, r10
  40b004: e1a0100a     	mov	r1, r10
  40b008: e480b008     	str	r11, [r0], #8
  40b00c: ebfc2436     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xf6f28
  40b010: e59f30a4     	ldr	r3, [pc, #0xa4]         @ 0x40b0bc <LightBase::DeclareProperties()+0x2cc>
  40b014: e2866f5a     	add	r6, r6, #360
  40b018: e1a00007     	mov	r0, r7
  40b01c: e7953003     	ldr	r3, [r5, r3]
  40b020: e0646006     	rsb	r6, r4, r6
  40b024: e5876004     	str	r6, [r7, #0x4]
  40b028: e2833008     	add	r3, r3, #8
  40b02c: e4803020     	str	r3, [r0], #32
  40b030: e5870030     	str	r0, [r7, #0x30]
  40b034: e5870034     	str	r0, [r7, #0x34]
  40b038: e59d1068     	ldr	r1, [sp, #0x68]
  40b03c: e59d2064     	ldr	r2, [sp, #0x64]
  40b040: ebfc19a8     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0xf9960
  40b044: e1a02007     	mov	r2, r7
  40b048: e1a0100a     	mov	r1, r10
  40b04c: e1a00004     	mov	r0, r4
  40b050: eb042323     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x108c8c
  40b054: e1a00008     	mov	r0, r8
  40b058: ebfc2253     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xf76b4
  40b05c: e1a00009     	mov	r0, r9
  40b060: ebfc2251     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xf76bc
  40b064: e59dc004     	ldr	r12, [sp, #0x4]
  40b068: e59d2084     	ldr	r2, [sp, #0x84]
  40b06c: e59c3000     	ldr	r3, [r12]
  40b070: e1520003     	cmp	r2, r3
  40b074: 1a000001     	bne	0x40b080 <LightBase::DeclareProperties()+0x290> @ imm = #0x4
  40b078: e28dd08c     	add	sp, sp, #140
  40b07c: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  40b080: ebfc0ca2     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xfcd78
  40b084: 90 9c 58 00  	.word	0x00589c90
  40b088: ac 40 00 00  	.word	0x000040ac
  40b08c: f4 73 4b 00  	.word	0x004b73f4
  40b090: 2c 3f 00 00  	.word	0x00003f2c
  40b094: 30 23 00 00  	.word	0x00002330
  40b098: c8 cc 4b 00  	.word	0x004bccc8
  40b09c: bc 24 00 00  	.word	0x000024bc
  40b0a0: 74 cc 4b 00  	.word	0x004bcc74
  40b0a4: 18 ba 4b 00  	.word	0x004bba18
  40b0a8: 3c cc 4b 00  	.word	0x004bcc3c
  40b0ac: 28 cc 4b 00  	.word	0x004bcc28
  40b0b0: 08 cc 4b 00  	.word	0x004bcc08
  40b0b4: 4c 3e 00 00  	.word	0x00003e4c
  40b0b8: 98 e8 4b 00  	.word	0x004be898
  40b0bc: 94 34 00 00  	.word	0x00003494
