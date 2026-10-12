
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040ccd0 <LightSetManager::~LightSetManager()>:
  40ccd0: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  40ccd4: e1a05000     	mov	r5, r0
  40ccd8: e5900190     	ldr	r0, [r0, #0x190]
  40ccdc: e59f7174     	ldr	r7, [pc, #0x174]        @ 0x40ce58 <LightSetManager::~LightSetManager()+0x188>
  40cce0: e3500000     	cmp	r0, #0
  40cce4: e08f7007     	add	r7, pc, r7
  40cce8: 0a000000     	beq	0x40ccf0 <LightSetManager::~LightSetManager()+0x20> @ imm = #0x0
  40ccec: ebfc0dd7     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0xfc8a4
  40ccf0: e2954f47     	adds	r4, r5, #284
  40ccf4: 0a000006     	beq	0x40cd14 <LightSetManager::~LightSetManager()+0x44> @ imm = #0x18
  40ccf8: e2856f5b     	add	r6, r5, #364
  40ccfc: e5363014     	ldr	r3, [r6, #-0x14]!
  40cd00: e1a00006     	mov	r0, r6
  40cd04: e1a0e00f     	mov	lr, pc
  40cd08: e593f000     	ldr	pc, [r3]
  40cd0c: e1560004     	cmp	r6, r4
  40cd10: 1afffff9     	bne	0x40ccfc <LightSetManager::~LightSetManager()+0x2c> @ imm = #-0x1c
  40cd14: e29560dc     	adds	r6, r5, #220
  40cd18: 0a000005     	beq	0x40cd34 <LightSetManager::~LightSetManager()+0x64> @ imm = #0x14
  40cd1c: e5343008     	ldr	r3, [r4, #-0x8]!
  40cd20: e1a00004     	mov	r0, r4
  40cd24: e1a0e00f     	mov	lr, pc
  40cd28: e593f000     	ldr	pc, [r3]
  40cd2c: e1540006     	cmp	r4, r6
  40cd30: 1afffff9     	bne	0x40cd1c <LightSetManager::~LightSetManager()+0x4c> @ imm = #-0x1c
  40cd34: e29540c8     	adds	r4, r5, #200
  40cd38: 0a000016     	beq	0x40cd98 <LightSetManager::~LightSetManager()+0xc8> @ imm = #0x58
  40cd3c: e59fa118     	ldr	r10, [pc, #0x118]       @ 0x40ce5c <LightSetManager::~LightSetManager()+0x18c>
  40cd40: e3a08000     	mov	r8, #0
  40cd44: e5163004     	ldr	r3, [r6, #-0x4]
  40cd48: e3530000     	cmp	r3, #0
  40cd4c: 0a00000e     	beq	0x40cd8c <LightSetManager::~LightSetManager()+0xbc> @ imm = #0x38
  40cd50: e5932000     	ldr	r2, [r3]
  40cd54: e1a00003     	mov	r0, r3
  40cd58: e2422001     	sub	r2, r2, #1
  40cd5c: e3520000     	cmp	r2, #0
  40cd60: e5832000     	str	r2, [r3]
  40cd64: 1a000008     	bne	0x40cd8c <LightSetManager::~LightSetManager()+0xbc> @ imm = #0x20
  40cd68: e5d32054     	ldrb	r2, [r3, #0x54]
  40cd6c: e3520000     	cmp	r2, #0
  40cd70: 0797200a     	ldreq	r2, [r7, r10]
  40cd74: 05931050     	ldreq	r1, [r3, #0x50]
  40cd78: 0592c000     	ldreq	r12, [r2]
  40cd7c: 0581c000     	streq	r12, [r1]
  40cd80: 05821000     	streq	r1, [r2]
  40cd84: e5838050     	str	r8, [r3, #0x50]
  40cd88: ebfc0dac     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xfc950
  40cd8c: e2466004     	sub	r6, r6, #4
  40cd90: e1560004     	cmp	r6, r4
  40cd94: 1affffea     	bne	0x40cd44 <LightSetManager::~LightSetManager()+0x74> @ imm = #-0x58
  40cd98: e2956078     	adds	r6, r5, #120
  40cd9c: 0a000016     	beq	0x40cdfc <LightSetManager::~LightSetManager()+0x12c> @ imm = #0x58
  40cda0: e59fa0b4     	ldr	r10, [pc, #0xb4]        @ 0x40ce5c <LightSetManager::~LightSetManager()+0x18c>
  40cda4: e3a08000     	mov	r8, #0
  40cda8: e5143004     	ldr	r3, [r4, #-0x4]
  40cdac: e3530000     	cmp	r3, #0
  40cdb0: 0a00000e     	beq	0x40cdf0 <LightSetManager::~LightSetManager()+0x120> @ imm = #0x38
  40cdb4: e5932000     	ldr	r2, [r3]
  40cdb8: e1a00003     	mov	r0, r3
  40cdbc: e2422001     	sub	r2, r2, #1
  40cdc0: e3520000     	cmp	r2, #0
  40cdc4: e5832000     	str	r2, [r3]
  40cdc8: 1a000008     	bne	0x40cdf0 <LightSetManager::~LightSetManager()+0x120> @ imm = #0x20
  40cdcc: e5d32054     	ldrb	r2, [r3, #0x54]
  40cdd0: e3520000     	cmp	r2, #0
  40cdd4: 0797200a     	ldreq	r2, [r7, r10]
  40cdd8: 05931050     	ldreq	r1, [r3, #0x50]
  40cddc: 0592c000     	ldreq	r12, [r2]
  40cde0: 0581c000     	streq	r12, [r1]
  40cde4: 05821000     	streq	r1, [r2]
  40cde8: e5838050     	str	r8, [r3, #0x50]
  40cdec: ebfc0d93     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xfc9b4
  40cdf0: e2444004     	sub	r4, r4, #4
  40cdf4: e1540006     	cmp	r4, r6
  40cdf8: 1affffea     	bne	0x40cda8 <LightSetManager::~LightSetManager()+0xd8> @ imm = #-0x58
  40cdfc: e2956004     	adds	r6, r5, #4
  40ce00: 12854064     	addne	r4, r5, #100
  40ce04: 1a000003     	bne	0x40ce18 <LightSetManager::~LightSetManager()+0x148> @ imm = #0xc
  40ce08: ea000010     	b	0x40ce50 <LightSetManager::~LightSetManager()+0x180> @ imm = #0x40
  40ce0c: eb0bf03b     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x2fc0ec
  40ce10: e1540006     	cmp	r4, r6
  40ce14: 0a00000d     	beq	0x40ce50 <LightSetManager::~LightSetManager()+0x180> @ imm = #0x34
  40ce18: e2444018     	sub	r4, r4, #24
  40ce1c: e5943014     	ldr	r3, [r4, #0x14]
  40ce20: e1530004     	cmp	r3, r4
  40ce24: 0afffff9     	beq	0x40ce10 <LightSetManager::~LightSetManager()+0x140> @ imm = #-0x1c
  40ce28: e3530000     	cmp	r3, #0
  40ce2c: 0afffff7     	beq	0x40ce10 <LightSetManager::~LightSetManager()+0x140> @ imm = #-0x24
  40ce30: e5941000     	ldr	r1, [r4]
  40ce34: e1a00003     	mov	r0, r3
  40ce38: e0631001     	rsb	r1, r3, r1
  40ce3c: e3510080     	cmp	r1, #128
  40ce40: 9afffff1     	bls	0x40ce0c <LightSetManager::~LightSetManager()+0x13c> @ imm = #-0x3c
  40ce44: ebfc0d7d     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xfca0c
  40ce48: e1540006     	cmp	r4, r6
  40ce4c: 1afffff1     	bne	0x40ce18 <LightSetManager::~LightSetManager()+0x148> @ imm = #-0x3c
  40ce50: e1a00005     	mov	r0, r5
  40ce54: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  40ce58: ac 7d 58 00  	.word	0x00587dac
  40ce5c: c0 3c 00 00  	.word	0x00003cc0
