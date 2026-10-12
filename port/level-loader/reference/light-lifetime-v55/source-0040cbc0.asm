
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040cbc0 <LightSetManager::ResetLightSets()>:
  40cbc0: e92d4ff8     	push	{r3, r4, r5, r6, r7, r8, r9, r10, r11, lr}
  40cbc4: e59fa0fc     	ldr	r10, [pc, #0xfc]        @ 0x40ccc8 <LightSetManager::ResetLightSets()+0x108>
  40cbc8: e59f90fc     	ldr	r9, [pc, #0xfc]         @ 0x40cccc <LightSetManager::ResetLightSets()+0x10c>
  40cbcc: e3a08000     	mov	r8, #0
  40cbd0: e1a0b000     	mov	r11, r0
  40cbd4: e1a07000     	mov	r7, r0
  40cbd8: e1a05000     	mov	r5, r0
  40cbdc: e1a06008     	mov	r6, r8
  40cbe0: e08fa00a     	add	r10, pc, r10
  40cbe4: e3a04000     	mov	r4, #0
  40cbe8: e0852004     	add	r2, r5, r4
  40cbec: e5923078     	ldr	r3, [r2, #0x78]
  40cbf0: e2844014     	add	r4, r4, #20
  40cbf4: e5826078     	str	r6, [r2, #0x78]
  40cbf8: e3530000     	cmp	r3, #0
  40cbfc: e1a00003     	mov	r0, r3
  40cc00: 0a00000d     	beq	0x40cc3c <LightSetManager::ResetLightSets()+0x7c> @ imm = #0x34
  40cc04: e5932000     	ldr	r2, [r3]
  40cc08: e2422001     	sub	r2, r2, #1
  40cc0c: e3520000     	cmp	r2, #0
  40cc10: e5832000     	str	r2, [r3]
  40cc14: 1a000008     	bne	0x40cc3c <LightSetManager::ResetLightSets()+0x7c> @ imm = #0x20
  40cc18: e5d32054     	ldrb	r2, [r3, #0x54]
  40cc1c: e3520000     	cmp	r2, #0
  40cc20: 079a2009     	ldreq	r2, [r10, r9]
  40cc24: 05931050     	ldreq	r1, [r3, #0x50]
  40cc28: 0592c000     	ldreq	r12, [r2]
  40cc2c: 0581c000     	streq	r12, [r1]
  40cc30: 05821000     	streq	r1, [r2]
  40cc34: e5836050     	str	r6, [r3, #0x50]
  40cc38: ebfc0e00     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xfc800
  40cc3c: e3540050     	cmp	r4, #80
  40cc40: 1affffe8     	bne	0x40cbe8 <LightSetManager::ResetLightSets()+0x28> @ imm = #-0x60
  40cc44: e2888001     	add	r8, r8, #1
  40cc48: e3580005     	cmp	r8, #5
  40cc4c: e2855004     	add	r5, r5, #4
  40cc50: 1affffe3     	bne	0x40cbe4 <LightSetManager::ResetLightSets()+0x24> @ imm = #-0x74
  40cc54: e59f6070     	ldr	r6, [pc, #0x70]         @ 0x40cccc <LightSetManager::ResetLightSets()+0x10c>
  40cc58: e3a04000     	mov	r4, #0
  40cc5c: e1a05004     	mov	r5, r4
  40cc60: e59730c8     	ldr	r3, [r7, #0xc8]
  40cc64: e2844001     	add	r4, r4, #1
  40cc68: e58750c8     	str	r5, [r7, #0xc8]
  40cc6c: e3530000     	cmp	r3, #0
  40cc70: e1a00003     	mov	r0, r3
  40cc74: 0a00000d     	beq	0x40ccb0 <LightSetManager::ResetLightSets()+0xf0> @ imm = #0x34
  40cc78: e5932000     	ldr	r2, [r3]
  40cc7c: e2422001     	sub	r2, r2, #1
  40cc80: e3520000     	cmp	r2, #0
  40cc84: e5832000     	str	r2, [r3]
  40cc88: 1a000008     	bne	0x40ccb0 <LightSetManager::ResetLightSets()+0xf0> @ imm = #0x20
  40cc8c: e5d32054     	ldrb	r2, [r3, #0x54]
  40cc90: e3520000     	cmp	r2, #0
  40cc94: 079a2006     	ldreq	r2, [r10, r6]
  40cc98: 05931050     	ldreq	r1, [r3, #0x50]
  40cc9c: 0592c000     	ldreq	r12, [r2]
  40cca0: 0581c000     	streq	r12, [r1]
  40cca4: 05821000     	streq	r1, [r2]
  40cca8: e5835050     	str	r5, [r3, #0x50]
  40ccac: ebfc0de3     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xfc874
  40ccb0: e3540005     	cmp	r4, #5
  40ccb4: e2877004     	add	r7, r7, #4
  40ccb8: 1affffe8     	bne	0x40cc60 <LightSetManager::ResetLightSets()+0xa0> @ imm = #-0x60
  40ccbc: e3a03000     	mov	r3, #0
  40ccc0: e58b31a0     	str	r3, [r11, #0x1a0]
  40ccc4: e8bd8ff8     	pop	{r3, r4, r5, r6, r7, r8, r9, r10, r11, pc}
  40ccc8: b0 7e 58 00  	.word	0x00587eb0
  40cccc: c0 3c 00 00  	.word	0x00003cc0
