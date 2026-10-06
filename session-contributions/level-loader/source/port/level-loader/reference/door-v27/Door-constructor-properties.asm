
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00340824 <ObjectBase* GetNewInstance<Door>()>:
  340824: e92d4010     	push	{r4, lr}
  340828: e3a01000     	mov	r1, #0
  34082c: e3a00e6d     	mov	r0, #1744
  340830: ebff3f4e     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x302c8
  340834: e3a01002     	mov	r1, #2
  340838: e1a04000     	mov	r4, r0
  34083c: eb029e8c     	bl	0x3e8274 <Door::Door(ObjectBase::GO_IDS)> @ imm = #0xa7a30
  340840: e1a00004     	mov	r0, r4
  340844: e8bd8010     	pop	{r4, pc}


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003e8990 <Door::DeclareProperties()>:
  3e8990: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3e8994: e59f4184     	ldr	r4, [pc, #0x184]        @ 0x3e8b20 <Door::DeclareProperties()+0x190>
  3e8998: e59fb184     	ldr	r11, [pc, #0x184]       @ 0x3e8b24 <Door::DeclareProperties()+0x194>
  3e899c: e24dd03c     	sub	sp, sp, #60
  3e89a0: e08f4004     	add	r4, pc, r4
  3e89a4: e794300b     	ldr	r3, [r4, r11]
  3e89a8: e28d701c     	add	r7, sp, #28
  3e89ac: e1a09000     	mov	r9, r0
  3e89b0: e5933000     	ldr	r3, [r3]
  3e89b4: e3a05000     	mov	r5, #0
  3e89b8: e28d8004     	add	r8, sp, #4
  3e89bc: e58d3034     	str	r3, [sp, #0x34]
  3e89c0: ebfebd0c     	bl	0x397df8 <Zone::DeclareProperties()> @ imm = #-0x50bd0
  3e89c4: e1a00007     	mov	r0, r7
  3e89c8: e3a01010     	mov	r1, #16
  3e89cc: e58d702c     	str	r7, [sp, #0x2c]
  3e89d0: e58d7030     	str	r7, [sp, #0x30]
  3e89d4: ebfca328     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0xd7360
  3e89d8: e59d302c     	ldr	r3, [sp, #0x2c]
  3e89dc: e1a00008     	mov	r0, r8
  3e89e0: e59fa140     	ldr	r10, [pc, #0x140]       @ 0x3e8b28 <Door::DeclareProperties()+0x198>
  3e89e4: e5c35000     	strb	r5, [r3]
  3e89e8: e59d202c     	ldr	r2, [sp, #0x2c]
  3e89ec: e59d1030     	ldr	r1, [sp, #0x30]
  3e89f0: e58d8014     	str	r8, [sp, #0x14]
  3e89f4: e58d8018     	str	r8, [sp, #0x18]
  3e89f8: ebfca33a     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0xd7318
  3e89fc: e1a01005     	mov	r1, r5
  3e8a00: e3a00038     	mov	r0, #56
  3e8a04: ebfc9ed9     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xd849c
  3e8a08: e59f311c     	ldr	r3, [pc, #0x11c]        @ 0x3e8b2c <Door::DeclareProperties()+0x19c>
  3e8a0c: e08fa00a     	add	r10, pc, r10
  3e8a10: e1a05000     	mov	r5, r0
  3e8a14: e7943003     	ldr	r3, [r4, r3]
  3e8a18: e1a0100a     	mov	r1, r10
  3e8a1c: e1a0200d     	mov	r2, sp
  3e8a20: e2833008     	add	r3, r3, #8
  3e8a24: e4803008     	str	r3, [r0], #8
  3e8a28: ebfcadaf     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xd4944
  3e8a2c: e59f30fc     	ldr	r3, [pc, #0xfc]         @ 0x3e8b30 <Door::DeclareProperties()+0x1a0>
  3e8a30: e2896004     	add	r6, r9, #4
  3e8a34: e2892fe2     	add	r2, r9, #904
  3e8a38: e7943003     	ldr	r3, [r4, r3]
  3e8a3c: e1a00005     	mov	r0, r5
  3e8a40: e0662002     	rsb	r2, r6, r2
  3e8a44: e2833008     	add	r3, r3, #8
  3e8a48: e5852004     	str	r2, [r5, #0x4]
  3e8a4c: e4803020     	str	r3, [r0], #32
  3e8a50: e5850030     	str	r0, [r5, #0x30]
  3e8a54: e5850034     	str	r0, [r5, #0x34]
  3e8a58: e59d1018     	ldr	r1, [sp, #0x18]
  3e8a5c: e59d2014     	ldr	r2, [sp, #0x14]
  3e8a60: ebfca320     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0xd7380
  3e8a64: e1a00006     	mov	r0, r6
  3e8a68: e1a0100a     	mov	r1, r10
  3e8a6c: e1a02005     	mov	r2, r5
  3e8a70: eb04ac9b     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x12b26c
  3e8a74: e59d0018     	ldr	r0, [sp, #0x18]
  3e8a78: e1500008     	cmp	r0, r8
  3e8a7c: 0a000006     	beq	0x3e8a9c <Door::DeclareProperties()+0x10c> @ imm = #0x18
  3e8a80: e3500000     	cmp	r0, #0
  3e8a84: 0a000004     	beq	0x3e8a9c <Door::DeclareProperties()+0x10c> @ imm = #0x10
  3e8a88: e59d1004     	ldr	r1, [sp, #0x4]
  3e8a8c: e0601001     	rsb	r1, r0, r1
  3e8a90: e3510080     	cmp	r1, #128
  3e8a94: 8a00001e     	bhi	0x3e8b14 <Door::DeclareProperties()+0x184> @ imm = #0x78
  3e8a98: eb0c8118     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x320460
  3e8a9c: e59d0030     	ldr	r0, [sp, #0x30]
  3e8aa0: e1500007     	cmp	r0, r7
  3e8aa4: 0a000006     	beq	0x3e8ac4 <Door::DeclareProperties()+0x134> @ imm = #0x18
  3e8aa8: e3500000     	cmp	r0, #0
  3e8aac: 0a000004     	beq	0x3e8ac4 <Door::DeclareProperties()+0x134> @ imm = #0x10
  3e8ab0: e59d101c     	ldr	r1, [sp, #0x1c]
  3e8ab4: e0601001     	rsb	r1, r0, r1
  3e8ab8: e3510080     	cmp	r1, #128
  3e8abc: 8a000012     	bhi	0x3e8b0c <Door::DeclareProperties()+0x17c> @ imm = #0x48
  3e8ac0: eb0c810e     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x320438
  3e8ac4: e59f1068     	ldr	r1, [pc, #0x68]         @ 0x3e8b34 <Door::DeclareProperties()+0x1a4>
  3e8ac8: e2899fe9     	add	r9, r9, #932
  3e8acc: e1a00006     	mov	r0, r6
  3e8ad0: e08f1001     	add	r1, pc, r1
  3e8ad4: e1a02009     	mov	r2, r9
  3e8ad8: ebfffd47     	bl	0x3e7ffc <void PropertyMap::AddProperty<bool>(char const*, bool&, bool) (.clone.5)> @ imm = #-0xae4
  3e8adc: e59f1054     	ldr	r1, [pc, #0x54]         @ 0x3e8b38 <Door::DeclareProperties()+0x1a8>
  3e8ae0: e2892001     	add	r2, r9, #1
  3e8ae4: e1a00006     	mov	r0, r6
  3e8ae8: e08f1001     	add	r1, pc, r1
  3e8aec: ebfffd42     	bl	0x3e7ffc <void PropertyMap::AddProperty<bool>(char const*, bool&, bool) (.clone.5)> @ imm = #-0xaf8
  3e8af0: e794300b     	ldr	r3, [r4, r11]
  3e8af4: e59d2034     	ldr	r2, [sp, #0x34]
  3e8af8: e5933000     	ldr	r3, [r3]
  3e8afc: e1520003     	cmp	r2, r3
  3e8b00: 1a000005     	bne	0x3e8b1c <Door::DeclareProperties()+0x18c> @ imm = #0x14
  3e8b04: e28dd03c     	add	sp, sp, #60
  3e8b08: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3e8b0c: ebfc9e4b     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xd86d4
  3e8b10: eaffffeb     	b	0x3e8ac4 <Door::DeclareProperties()+0x134> @ imm = #-0x54
  3e8b14: ebfc9e49     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xd86dc
  3e8b18: eaffffdf     	b	0x3e8a9c <Door::DeclareProperties()+0x10c> @ imm = #-0x84
  3e8b1c: ebfc95fb     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xda814
  3e8b20: f0 c0 5a 00  	.word	0x005ac0f0
  3e8b24: ac 40 00 00  	.word	0x000040ac
  3e8b28: 4c a1 4d 00  	.word	0x004da14c
  3e8b2c: 30 23 00 00  	.word	0x00002330
  3e8b30: 94 34 00 00  	.word	0x00003494
  3e8b34: b8 a4 4d 00  	.word	0x004da4b8
  3e8b38: 10 d6 4d 00  	.word	0x004dd610


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003e8274 <Door::Door(ObjectBase::GO_IDS)>:
  3e8274: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3e8278: e3a02000     	mov	r2, #0
  3e827c: e3a03001     	mov	r3, #1
  3e8280: e59f5094     	ldr	r5, [pc, #0x94]         @ 0x3e831c <Door::Door(ObjectBase::GO_IDS)+0xa8>
  3e8284: e1a04000     	mov	r4, r0
  3e8288: ebfebe84     	bl	0x397ca0 <Zone::Zone(ObjectBase::GO_IDS, bool, bool)> @ imm = #-0x505f0
  3e828c: e59f308c     	ldr	r3, [pc, #0x8c]         @ 0x3e8320 <Door::Door(ObjectBase::GO_IDS)+0xac>
  3e8290: e08f5005     	add	r5, pc, r5
  3e8294: e2842fe2     	add	r2, r4, #904
  3e8298: e7953003     	ldr	r3, [r5, r3]
  3e829c: e1a00002     	mov	r0, r2
  3e82a0: e5842398     	str	r2, [r4, #0x398]
  3e82a4: e283c008     	add	r12, r3, #8
  3e82a8: e28310f4     	add	r1, r3, #244
  3e82ac: e28330e8     	add	r3, r3, #232
  3e82b0: e584c000     	str	r12, [r4]
  3e82b4: e5843004     	str	r3, [r4, #0x4]
  3e82b8: e5841024     	str	r1, [r4, #0x24]
  3e82bc: e584239c     	str	r2, [r4, #0x39c]
  3e82c0: e3a01010     	mov	r1, #16
  3e82c4: ebfca4ec     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0xd6c50
  3e82c8: e5942398     	ldr	r2, [r4, #0x398]
  3e82cc: e3a03000     	mov	r3, #0
  3e82d0: e3a07001     	mov	r7, #1
  3e82d4: e2846e3b     	add	r6, r4, #944
  3e82d8: e5c23000     	strb	r3, [r2]
  3e82dc: e2845d15     	add	r5, r4, #1344
  3e82e0: e5c433ac     	strb	r3, [r4, #0x3ac]
  3e82e4: e5c433a4     	strb	r3, [r4, #0x3a4]
  3e82e8: e58433a8     	str	r3, [r4, #0x3a8]
  3e82ec: e5c473a5     	strb	r7, [r4, #0x3a5]
  3e82f0: e1a00006     	mov	r0, r6
  3e82f4: ebffff76     	bl	0x3e80d4 <Door::NetStructDoor::NetStructDoor()> @ imm = #-0x228
  3e82f8: e1a00005     	mov	r0, r5
  3e82fc: ebffff74     	bl	0x3e80d4 <Door::NetStructDoor::NetStructDoor()> @ imm = #-0x230
  3e8300: e3a03003     	mov	r3, #3
  3e8304: e5c47028     	strb	r7, [r4, #0x28]
  3e8308: e5846100     	str	r6, [r4, #0x100]
  3e830c: e5845104     	str	r5, [r4, #0x104]
  3e8310: e5c430f8     	strb	r3, [r4, #0xf8]
  3e8314: e1a00004     	mov	r0, r4
  3e8318: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3e831c: 00 c8 5a 00  	.word	0x005ac800
  3e8320: 84 49 00 00  	.word	0x00004984


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003e8324 <Door::Door(ObjectBase::GO_IDS)>:
  3e8324: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3e8328: e3a02000     	mov	r2, #0
  3e832c: e3a03001     	mov	r3, #1
  3e8330: e59f5094     	ldr	r5, [pc, #0x94]         @ 0x3e83cc <Door::Door(ObjectBase::GO_IDS)+0xa8>
  3e8334: e1a04000     	mov	r4, r0
  3e8338: ebfebe58     	bl	0x397ca0 <Zone::Zone(ObjectBase::GO_IDS, bool, bool)> @ imm = #-0x506a0
  3e833c: e59f308c     	ldr	r3, [pc, #0x8c]         @ 0x3e83d0 <Door::Door(ObjectBase::GO_IDS)+0xac>
  3e8340: e08f5005     	add	r5, pc, r5
  3e8344: e2842fe2     	add	r2, r4, #904
  3e8348: e7953003     	ldr	r3, [r5, r3]
  3e834c: e1a00002     	mov	r0, r2
  3e8350: e5842398     	str	r2, [r4, #0x398]
  3e8354: e283c008     	add	r12, r3, #8
  3e8358: e28310f4     	add	r1, r3, #244
  3e835c: e28330e8     	add	r3, r3, #232
  3e8360: e584c000     	str	r12, [r4]
  3e8364: e5843004     	str	r3, [r4, #0x4]
  3e8368: e5841024     	str	r1, [r4, #0x24]
  3e836c: e584239c     	str	r2, [r4, #0x39c]
  3e8370: e3a01010     	mov	r1, #16
  3e8374: ebfca4c0     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0xd6d00
  3e8378: e5942398     	ldr	r2, [r4, #0x398]
  3e837c: e3a03000     	mov	r3, #0
  3e8380: e3a07001     	mov	r7, #1
  3e8384: e2846e3b     	add	r6, r4, #944
  3e8388: e5c23000     	strb	r3, [r2]
  3e838c: e2845d15     	add	r5, r4, #1344
  3e8390: e5c433ac     	strb	r3, [r4, #0x3ac]
  3e8394: e5c433a4     	strb	r3, [r4, #0x3a4]
  3e8398: e58433a8     	str	r3, [r4, #0x3a8]
  3e839c: e5c473a5     	strb	r7, [r4, #0x3a5]
  3e83a0: e1a00006     	mov	r0, r6
  3e83a4: ebffff4a     	bl	0x3e80d4 <Door::NetStructDoor::NetStructDoor()> @ imm = #-0x2d8
  3e83a8: e1a00005     	mov	r0, r5
  3e83ac: ebffff48     	bl	0x3e80d4 <Door::NetStructDoor::NetStructDoor()> @ imm = #-0x2e0
  3e83b0: e3a03003     	mov	r3, #3
  3e83b4: e5c47028     	strb	r7, [r4, #0x28]
  3e83b8: e5846100     	str	r6, [r4, #0x100]
  3e83bc: e5845104     	str	r5, [r4, #0x104]
  3e83c0: e5c430f8     	strb	r3, [r4, #0xf8]
  3e83c4: e1a00004     	mov	r0, r4
  3e83c8: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3e83cc: 50 c7 5a 00  	.word	0x005ac750
  3e83d0: 84 49 00 00  	.word	0x00004984


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003e8988 <non-virtual thunk to Door::DeclareProperties()>:
  3e8988: e2400004     	sub	r0, r0, #4
  3e898c: eaffffff     	b	0x3e8990 <Door::DeclareProperties()> @ imm = #-0x4
