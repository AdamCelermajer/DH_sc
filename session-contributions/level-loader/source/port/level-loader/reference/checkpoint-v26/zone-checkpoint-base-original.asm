
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00397df8 <Zone::DeclareProperties()>:
  397df8: e92d40f0     	push	{r4, r5, r6, r7, lr}
  397dfc: e24dd00c     	sub	sp, sp, #12
  397e00: e1a07000     	mov	r7, r0
  397e04: ebffd437     	bl	0x38cee8 <GameObject::DeclareProperties()> @ imm = #-0xaf24
  397e08: e3a01000     	mov	r1, #0
  397e0c: e3a0002c     	mov	r0, #44
  397e10: ebfde1d6     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x878a8
  397e14: e59f5070     	ldr	r5, [pc, #0x70]         @ 0x397e8c <Zone::DeclareProperties()+0x94>
  397e18: e59f3070     	ldr	r3, [pc, #0x70]         @ 0x397e90 <Zone::DeclareProperties()+0x98>
  397e1c: e59f6070     	ldr	r6, [pc, #0x70]         @ 0x397e94 <Zone::DeclareProperties()+0x9c>
  397e20: e08f5005     	add	r5, pc, r5
  397e24: e7953003     	ldr	r3, [r5, r3]
  397e28: e08f6006     	add	r6, pc, r6
  397e2c: e1a04000     	mov	r4, r0
  397e30: e2833008     	add	r3, r3, #8
  397e34: e1a01006     	mov	r1, r6
  397e38: e28d2004     	add	r2, sp, #4
  397e3c: e4803008     	str	r3, [r0], #8
  397e40: ebfdf0a9     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x83d5c
  397e44: e59f204c     	ldr	r2, [pc, #0x4c]         @ 0x397e98 <Zone::DeclareProperties()+0xa0>
  397e48: e2871fdd     	add	r1, r7, #884
  397e4c: e2870004     	add	r0, r7, #4
  397e50: e7952002     	ldr	r2, [r5, r2]
  397e54: e3a03443     	mov	r3, #1124073472
  397e58: e2833712     	add	r3, r3, #4718592
  397e5c: e0601001     	rsb	r1, r0, r1
  397e60: e2822008     	add	r2, r2, #8
  397e64: e5841004     	str	r1, [r4, #0x4]
  397e68: e5842000     	str	r2, [r4]
  397e6c: e5843028     	str	r3, [r4, #0x28]
  397e70: e5843020     	str	r3, [r4, #0x20]
  397e74: e5843024     	str	r3, [r4, #0x24]
  397e78: e1a01006     	mov	r1, r6
  397e7c: e1a02004     	mov	r2, r4
  397e80: eb05ef97     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x17be5c
  397e84: e28dd00c     	add	sp, sp, #12
  397e88: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  397e8c: 70 cc 5f 00  	.word	0x005fcc70
  397e90: 30 23 00 00  	.word	0x00002330
  397e94: b8 a4 52 00  	.word	0x0052a4b8
  397e98: 44 0b 00 00  	.word	0x00000b44


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00397c2c <Zone::Zone(ObjectBase::GO_IDS, bool, bool)>:
  397c2c: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  397c30: e59f4060     	ldr	r4, [pc, #0x60]         @ 0x397c98 <Zone::Zone(ObjectBase::GO_IDS, bool, bool)+0x6c>
  397c34: e1a06000     	mov	r6, r0
  397c38: e1a05002     	mov	r5, r2
  397c3c: e1a07003     	mov	r7, r3
  397c40: ebffd1d4     	bl	0x38c398 <GameObject::GameObject(ObjectBase::GO_IDS)> @ imm = #-0xb8b0
  397c44: e59f3050     	ldr	r3, [pc, #0x50]         @ 0x397c9c <Zone::Zone(ObjectBase::GO_IDS, bool, bool)+0x70>
  397c48: e08f4004     	add	r4, pc, r4
  397c4c: e3a02000     	mov	r2, #0
  397c50: e7943003     	ldr	r3, [r4, r3]
  397c54: e586237c     	str	r2, [r6, #0x37c]
  397c58: e5c65380     	strb	r5, [r6, #0x380]
  397c5c: e28310f4     	add	r1, r3, #244
  397c60: e2830008     	add	r0, r3, #8
  397c64: e28330e8     	add	r3, r3, #232
  397c68: e5863004     	str	r3, [r6, #0x4]
  397c6c: e3a03000     	mov	r3, #0
  397c70: e5863384     	str	r3, [r6, #0x384]
  397c74: e3a03001     	mov	r3, #1
  397c78: e5860000     	str	r0, [r6]
  397c7c: e5861024     	str	r1, [r6, #0x24]
  397c80: e5c67381     	strb	r7, [r6, #0x381]
  397c84: e5c63084     	strb	r3, [r6, #0x84]
  397c88: e5862374     	str	r2, [r6, #0x374]
  397c8c: e5862378     	str	r2, [r6, #0x378]
  397c90: e1a00006     	mov	r0, r6
  397c94: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  397c98: 48 ce 5f 00  	.word	0x005fce48
  397c9c: 34 0d 00 00  	.word	0x00000d34


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00397ca0 <Zone::Zone(ObjectBase::GO_IDS, bool, bool)>:
  397ca0: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  397ca4: e59f4060     	ldr	r4, [pc, #0x60]         @ 0x397d0c <Zone::Zone(ObjectBase::GO_IDS, bool, bool)+0x6c>
  397ca8: e1a06000     	mov	r6, r0
  397cac: e1a05002     	mov	r5, r2
  397cb0: e1a07003     	mov	r7, r3
  397cb4: ebffd1b7     	bl	0x38c398 <GameObject::GameObject(ObjectBase::GO_IDS)> @ imm = #-0xb924
  397cb8: e59f3050     	ldr	r3, [pc, #0x50]         @ 0x397d10 <Zone::Zone(ObjectBase::GO_IDS, bool, bool)+0x70>
  397cbc: e08f4004     	add	r4, pc, r4
  397cc0: e3a02000     	mov	r2, #0
  397cc4: e7943003     	ldr	r3, [r4, r3]
  397cc8: e586237c     	str	r2, [r6, #0x37c]
  397ccc: e5c65380     	strb	r5, [r6, #0x380]
  397cd0: e28310f4     	add	r1, r3, #244
  397cd4: e2830008     	add	r0, r3, #8
  397cd8: e28330e8     	add	r3, r3, #232
  397cdc: e5863004     	str	r3, [r6, #0x4]
  397ce0: e3a03000     	mov	r3, #0
  397ce4: e5863384     	str	r3, [r6, #0x384]
  397ce8: e3a03001     	mov	r3, #1
  397cec: e5860000     	str	r0, [r6]
  397cf0: e5861024     	str	r1, [r6, #0x24]
  397cf4: e5c67381     	strb	r7, [r6, #0x381]
  397cf8: e5c63084     	strb	r3, [r6, #0x84]
  397cfc: e5862374     	str	r2, [r6, #0x374]
  397d00: e5862378     	str	r2, [r6, #0x378]
  397d04: e1a00006     	mov	r0, r6
  397d08: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  397d0c: d4 cd 5f 00  	.word	0x005fcdd4
  397d10: 34 0d 00 00  	.word	0x00000d34


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00397df0 <non-virtual thunk to Zone::DeclareProperties()>:
  397df0: e2400004     	sub	r0, r0, #4
  397df4: eaffffff     	b	0x397df8 <Zone::DeclareProperties()> @ imm = #-0x4
