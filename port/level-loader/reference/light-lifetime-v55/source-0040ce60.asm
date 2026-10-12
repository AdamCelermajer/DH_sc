
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040ce60 <LightSetManager::~LightSetManager()>:
  40ce60: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  40ce64: e1a05000     	mov	r5, r0
  40ce68: e5900190     	ldr	r0, [r0, #0x190]
  40ce6c: e59f7174     	ldr	r7, [pc, #0x174]        @ 0x40cfe8 <LightSetManager::~LightSetManager()+0x188>
  40ce70: e3500000     	cmp	r0, #0
  40ce74: e08f7007     	add	r7, pc, r7
  40ce78: 0a000000     	beq	0x40ce80 <LightSetManager::~LightSetManager()+0x20> @ imm = #0x0
  40ce7c: ebfc0d73     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0xfca34
  40ce80: e2954f47     	adds	r4, r5, #284
  40ce84: 0a000006     	beq	0x40cea4 <LightSetManager::~LightSetManager()+0x44> @ imm = #0x18
  40ce88: e2856f5b     	add	r6, r5, #364
  40ce8c: e5363014     	ldr	r3, [r6, #-0x14]!
  40ce90: e1a00006     	mov	r0, r6
  40ce94: e1a0e00f     	mov	lr, pc
  40ce98: e593f000     	ldr	pc, [r3]
  40ce9c: e1560004     	cmp	r6, r4
  40cea0: 1afffff9     	bne	0x40ce8c <LightSetManager::~LightSetManager()+0x2c> @ imm = #-0x1c
  40cea4: e29560dc     	adds	r6, r5, #220
  40cea8: 0a000005     	beq	0x40cec4 <LightSetManager::~LightSetManager()+0x64> @ imm = #0x14
  40ceac: e5343008     	ldr	r3, [r4, #-0x8]!
  40ceb0: e1a00004     	mov	r0, r4
  40ceb4: e1a0e00f     	mov	lr, pc
  40ceb8: e593f000     	ldr	pc, [r3]
  40cebc: e1540006     	cmp	r4, r6
  40cec0: 1afffff9     	bne	0x40ceac <LightSetManager::~LightSetManager()+0x4c> @ imm = #-0x1c
  40cec4: e29540c8     	adds	r4, r5, #200
  40cec8: 0a000016     	beq	0x40cf28 <LightSetManager::~LightSetManager()+0xc8> @ imm = #0x58
  40cecc: e59fa118     	ldr	r10, [pc, #0x118]       @ 0x40cfec <LightSetManager::~LightSetManager()+0x18c>
  40ced0: e3a08000     	mov	r8, #0
  40ced4: e5163004     	ldr	r3, [r6, #-0x4]
  40ced8: e3530000     	cmp	r3, #0
  40cedc: 0a00000e     	beq	0x40cf1c <LightSetManager::~LightSetManager()+0xbc> @ imm = #0x38
  40cee0: e5932000     	ldr	r2, [r3]
  40cee4: e1a00003     	mov	r0, r3
  40cee8: e2422001     	sub	r2, r2, #1
  40ceec: e3520000     	cmp	r2, #0
  40cef0: e5832000     	str	r2, [r3]
  40cef4: 1a000008     	bne	0x40cf1c <LightSetManager::~LightSetManager()+0xbc> @ imm = #0x20
  40cef8: e5d32054     	ldrb	r2, [r3, #0x54]
  40cefc: e3520000     	cmp	r2, #0
  40cf00: 0797200a     	ldreq	r2, [r7, r10]
  40cf04: 05931050     	ldreq	r1, [r3, #0x50]
  40cf08: 0592c000     	ldreq	r12, [r2]
  40cf0c: 0581c000     	streq	r12, [r1]
  40cf10: 05821000     	streq	r1, [r2]
  40cf14: e5838050     	str	r8, [r3, #0x50]
  40cf18: ebfc0d48     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xfcae0
  40cf1c: e2466004     	sub	r6, r6, #4
  40cf20: e1560004     	cmp	r6, r4
  40cf24: 1affffea     	bne	0x40ced4 <LightSetManager::~LightSetManager()+0x74> @ imm = #-0x58
  40cf28: e2956078     	adds	r6, r5, #120
  40cf2c: 0a000016     	beq	0x40cf8c <LightSetManager::~LightSetManager()+0x12c> @ imm = #0x58
  40cf30: e59fa0b4     	ldr	r10, [pc, #0xb4]        @ 0x40cfec <LightSetManager::~LightSetManager()+0x18c>
  40cf34: e3a08000     	mov	r8, #0
  40cf38: e5143004     	ldr	r3, [r4, #-0x4]
  40cf3c: e3530000     	cmp	r3, #0
  40cf40: 0a00000e     	beq	0x40cf80 <LightSetManager::~LightSetManager()+0x120> @ imm = #0x38
  40cf44: e5932000     	ldr	r2, [r3]
  40cf48: e1a00003     	mov	r0, r3
  40cf4c: e2422001     	sub	r2, r2, #1
  40cf50: e3520000     	cmp	r2, #0
  40cf54: e5832000     	str	r2, [r3]
  40cf58: 1a000008     	bne	0x40cf80 <LightSetManager::~LightSetManager()+0x120> @ imm = #0x20
  40cf5c: e5d32054     	ldrb	r2, [r3, #0x54]
  40cf60: e3520000     	cmp	r2, #0
  40cf64: 0797200a     	ldreq	r2, [r7, r10]
  40cf68: 05931050     	ldreq	r1, [r3, #0x50]
  40cf6c: 0592c000     	ldreq	r12, [r2]
  40cf70: 0581c000     	streq	r12, [r1]
  40cf74: 05821000     	streq	r1, [r2]
  40cf78: e5838050     	str	r8, [r3, #0x50]
  40cf7c: ebfc0d2f     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xfcb44
  40cf80: e2444004     	sub	r4, r4, #4
  40cf84: e1540006     	cmp	r4, r6
  40cf88: 1affffea     	bne	0x40cf38 <LightSetManager::~LightSetManager()+0xd8> @ imm = #-0x58
  40cf8c: e2956004     	adds	r6, r5, #4
  40cf90: 12854064     	addne	r4, r5, #100
  40cf94: 1a000003     	bne	0x40cfa8 <LightSetManager::~LightSetManager()+0x148> @ imm = #0xc
  40cf98: ea000010     	b	0x40cfe0 <LightSetManager::~LightSetManager()+0x180> @ imm = #0x40
  40cf9c: eb0befd7     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x2fbf5c
  40cfa0: e1540006     	cmp	r4, r6
  40cfa4: 0a00000d     	beq	0x40cfe0 <LightSetManager::~LightSetManager()+0x180> @ imm = #0x34
  40cfa8: e2444018     	sub	r4, r4, #24
  40cfac: e5943014     	ldr	r3, [r4, #0x14]
  40cfb0: e1530004     	cmp	r3, r4
  40cfb4: 0afffff9     	beq	0x40cfa0 <LightSetManager::~LightSetManager()+0x140> @ imm = #-0x1c
  40cfb8: e3530000     	cmp	r3, #0
  40cfbc: 0afffff7     	beq	0x40cfa0 <LightSetManager::~LightSetManager()+0x140> @ imm = #-0x24
  40cfc0: e5941000     	ldr	r1, [r4]
  40cfc4: e1a00003     	mov	r0, r3
  40cfc8: e0631001     	rsb	r1, r3, r1
  40cfcc: e3510080     	cmp	r1, #128
  40cfd0: 9afffff1     	bls	0x40cf9c <LightSetManager::~LightSetManager()+0x13c> @ imm = #-0x3c
  40cfd4: ebfc0d19     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xfcb9c
  40cfd8: e1540006     	cmp	r4, r6
  40cfdc: 1afffff1     	bne	0x40cfa8 <LightSetManager::~LightSetManager()+0x148> @ imm = #-0x3c
  40cfe0: e1a00005     	mov	r0, r5
  40cfe4: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  40cfe8: 1c 7c 58 00  	.word	0x00587c1c
  40cfec: c0 3c 00 00  	.word	0x00003cc0
