
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00340ea0 <ObjectBase* GetNewInstance<TriggerZoneExitLevel>()>:
  340ea0: e92d4010     	push	{r4, lr}
  340ea4: e3a01000     	mov	r1, #0
  340ea8: e3a00e82     	mov	r0, #2080
  340eac: ebff3daf     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x30944
  340eb0: e3a0100e     	mov	r1, #14
  340eb4: e1a04000     	mov	r4, r0
  340eb8: eb016eb9     	bl	0x39c9a4 <TriggerZoneExitLevel::TriggerZoneExitLevel(ObjectBase::GO_IDS)> @ imm = #0x5bae4
  340ebc: e1a00004     	mov	r0, r4
  340ec0: e8bd8010     	pop	{r4, pc}


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039c8f8 <TriggerZoneExitLevel::DeclareProperties()>:
  39c8f8: e92d4070     	push	{r4, r5, r6, lr}
  39c8fc: e1a05000     	mov	r5, r0
  39c900: ebfffda7     	bl	0x39bfa4 <TriggerZone::DeclareProperties()> @ imm = #-0x964
  39c904: e59f1080     	ldr	r1, [pc, #0x80]         @ 0x39c98c <TriggerZoneExitLevel::DeclareProperties()+0x94>
  39c908: e2854004     	add	r4, r5, #4
  39c90c: e2856e7d     	add	r6, r5, #2000
  39c910: e1a00004     	mov	r0, r4
  39c914: e2862008     	add	r2, r6, #8
  39c918: e08f1001     	add	r1, pc, r1
  39c91c: ebffffc6     	bl	0x39c83c <void PropertyMap::AddProperty<int>(char const*, int&, int) (.clone.1)> @ imm = #-0xe8
  39c920: e59f1068     	ldr	r1, [pc, #0x68]         @ 0x39c990 <TriggerZoneExitLevel::DeclareProperties()+0x98>
  39c924: e286200c     	add	r2, r6, #12
  39c928: e1a00004     	mov	r0, r4
  39c92c: e08f1001     	add	r1, pc, r1
  39c930: ebfe8991     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x5d9bc
  39c934: e59f1058     	ldr	r1, [pc, #0x58]         @ 0x39c994 <TriggerZoneExitLevel::DeclareProperties()+0x9c>
  39c938: e2856e7f     	add	r6, r5, #2032
  39c93c: e1a00004     	mov	r0, r4
  39c940: e2862004     	add	r2, r6, #4
  39c944: e08f1001     	add	r1, pc, r1
  39c948: ebffffbb     	bl	0x39c83c <void PropertyMap::AddProperty<int>(char const*, int&, int) (.clone.1)> @ imm = #-0x114
  39c94c: e59f1044     	ldr	r1, [pc, #0x44]         @ 0x39c998 <TriggerZoneExitLevel::DeclareProperties()+0xa0>
  39c950: e1a00004     	mov	r0, r4
  39c954: e2862008     	add	r2, r6, #8
  39c958: e08f1001     	add	r1, pc, r1
  39c95c: ebffffb6     	bl	0x39c83c <void PropertyMap::AddProperty<int>(char const*, int&, int) (.clone.1)> @ imm = #-0x128
  39c960: e59f1034     	ldr	r1, [pc, #0x34]         @ 0x39c99c <TriggerZoneExitLevel::DeclareProperties()+0xa4>
  39c964: e286200c     	add	r2, r6, #12
  39c968: e1a00004     	mov	r0, r4
  39c96c: e08f1001     	add	r1, pc, r1
  39c970: ebffffb1     	bl	0x39c83c <void PropertyMap::AddProperty<int>(char const*, int&, int) (.clone.1)> @ imm = #-0x13c
  39c974: e59f1024     	ldr	r1, [pc, #0x24]         @ 0x39c9a0 <TriggerZoneExitLevel::DeclareProperties()+0xa8>
  39c978: e1a00004     	mov	r0, r4
  39c97c: e2852b02     	add	r2, r5, #2048
  39c980: e08f1001     	add	r1, pc, r1
  39c984: e8bd4070     	pop	{r4, r5, r6, lr}
  39c988: eafe897b     	b	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x5da14
  39c98c: 40 64 52 00  	.word	0x00526440
  39c990: 34 64 52 00  	.word	0x00526434
  39c994: 2c 64 52 00  	.word	0x0052642c
  39c998: 28 64 52 00  	.word	0x00526428
  39c99c: 1c 64 52 00  	.word	0x0052641c
  39c9a0: 18 64 52 00  	.word	0x00526418


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039c9a4 <TriggerZoneExitLevel::TriggerZoneExitLevel(ObjectBase::GO_IDS)>:
  39c9a4: e92d4070     	push	{r4, r5, r6, lr}
  39c9a8: e59f5088     	ldr	r5, [pc, #0x88]         @ 0x39ca38 <TriggerZoneExitLevel::TriggerZoneExitLevel(ObjectBase::GO_IDS)+0x94>
  39c9ac: e1a04000     	mov	r4, r0
  39c9b0: ebfffbf6     	bl	0x39b990 <TriggerZone::TriggerZone(ObjectBase::GO_IDS)> @ imm = #-0x1028
  39c9b4: e59f2080     	ldr	r2, [pc, #0x80]         @ 0x39ca3c <TriggerZoneExitLevel::TriggerZoneExitLevel(ObjectBase::GO_IDS)+0x98>
  39c9b8: e08f5005     	add	r5, pc, r5
  39c9bc: e2843e7d     	add	r3, r4, #2000
  39c9c0: e7952002     	ldr	r2, [r5, r2]
  39c9c4: e283300c     	add	r3, r3, #12
  39c9c8: e58437ec     	str	r3, [r4, #0x7ec]
  39c9cc: e28210f4     	add	r1, r2, #244
  39c9d0: e2820008     	add	r0, r2, #8
  39c9d4: e28220e8     	add	r2, r2, #232
  39c9d8: e5842004     	str	r2, [r4, #0x4]
  39c9dc: e3e02000     	mvn	r2, #0
  39c9e0: e5840000     	str	r0, [r4]
  39c9e4: e5841024     	str	r1, [r4, #0x24]
  39c9e8: e58427d4     	str	r2, [r4, #0x7d4]
  39c9ec: e1a00003     	mov	r0, r3
  39c9f0: e58437f0     	str	r3, [r4, #0x7f0]
  39c9f4: e3a01010     	mov	r1, #16
  39c9f8: ebfdd31f     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x8b384
  39c9fc: e59427ec     	ldr	r2, [r4, #0x7ec]
  39ca00: e2843b02     	add	r3, r4, #2048
  39ca04: e3a05000     	mov	r5, #0
  39ca08: e5c25000     	strb	r5, [r2]
  39ca0c: e1a00003     	mov	r0, r3
  39ca10: e5843810     	str	r3, [r4, #0x810]
  39ca14: e5843814     	str	r3, [r4, #0x814]
  39ca18: e3a01010     	mov	r1, #16
  39ca1c: ebfdd316     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x8b3a8
  39ca20: e5943810     	ldr	r3, [r4, #0x810]
  39ca24: e1a00004     	mov	r0, r4
  39ca28: e5c35000     	strb	r5, [r3]
  39ca2c: e5c45819     	strb	r5, [r4, #0x819]
  39ca30: e5c45818     	strb	r5, [r4, #0x818]
  39ca34: e8bd8070     	pop	{r4, r5, r6, pc}
  39ca38: d8 80 5f 00  	.word	0x005f80d8
  39ca3c: f4 4b 00 00  	.word	0x00004bf4


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039ca40 <TriggerZoneExitLevel::TriggerZoneExitLevel(ObjectBase::GO_IDS)>:
  39ca40: e92d4070     	push	{r4, r5, r6, lr}
  39ca44: e59f5088     	ldr	r5, [pc, #0x88]         @ 0x39cad4 <TriggerZoneExitLevel::TriggerZoneExitLevel(ObjectBase::GO_IDS)+0x94>
  39ca48: e1a04000     	mov	r4, r0
  39ca4c: ebfffbcf     	bl	0x39b990 <TriggerZone::TriggerZone(ObjectBase::GO_IDS)> @ imm = #-0x10c4
  39ca50: e59f2080     	ldr	r2, [pc, #0x80]         @ 0x39cad8 <TriggerZoneExitLevel::TriggerZoneExitLevel(ObjectBase::GO_IDS)+0x98>
  39ca54: e08f5005     	add	r5, pc, r5
  39ca58: e2843e7d     	add	r3, r4, #2000
  39ca5c: e7952002     	ldr	r2, [r5, r2]
  39ca60: e283300c     	add	r3, r3, #12
  39ca64: e58437ec     	str	r3, [r4, #0x7ec]
  39ca68: e28210f4     	add	r1, r2, #244
  39ca6c: e2820008     	add	r0, r2, #8
  39ca70: e28220e8     	add	r2, r2, #232
  39ca74: e5842004     	str	r2, [r4, #0x4]
  39ca78: e3e02000     	mvn	r2, #0
  39ca7c: e5840000     	str	r0, [r4]
  39ca80: e5841024     	str	r1, [r4, #0x24]
  39ca84: e58427d4     	str	r2, [r4, #0x7d4]
  39ca88: e1a00003     	mov	r0, r3
  39ca8c: e58437f0     	str	r3, [r4, #0x7f0]
  39ca90: e3a01010     	mov	r1, #16
  39ca94: ebfdd2f8     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x8b420
  39ca98: e59427ec     	ldr	r2, [r4, #0x7ec]
  39ca9c: e2843b02     	add	r3, r4, #2048
  39caa0: e3a05000     	mov	r5, #0
  39caa4: e5c25000     	strb	r5, [r2]
  39caa8: e1a00003     	mov	r0, r3
  39caac: e5843810     	str	r3, [r4, #0x810]
  39cab0: e5843814     	str	r3, [r4, #0x814]
  39cab4: e3a01010     	mov	r1, #16
  39cab8: ebfdd2ef     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x8b444
  39cabc: e5943810     	ldr	r3, [r4, #0x810]
  39cac0: e1a00004     	mov	r0, r4
  39cac4: e5c35000     	strb	r5, [r3]
  39cac8: e5c45819     	strb	r5, [r4, #0x819]
  39cacc: e5c45818     	strb	r5, [r4, #0x818]
  39cad0: e8bd8070     	pop	{r4, r5, r6, pc}
  39cad4: 3c 80 5f 00  	.word	0x005f803c
  39cad8: f4 4b 00 00  	.word	0x00004bf4


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039c8f0 <non-virtual thunk to TriggerZoneExitLevel::DeclareProperties()>:
  39c8f0: e2400004     	sub	r0, r0, #4
  39c8f4: eaffffff     	b	0x39c8f8 <TriggerZoneExitLevel::DeclareProperties()> @ imm = #-0x4
