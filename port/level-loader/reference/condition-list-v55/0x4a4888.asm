
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004a4888 <Arrays::v2Conditions::finalizeNames()>:
  4a4888: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  4a488c: e59f5084     	ldr	r5, [pc, #0x84]         @ 0x4a4918 <Arrays::v2Conditions::finalizeNames()+0x90>
  4a4890: e59f6084     	ldr	r6, [pc, #0x84]         @ 0x4a491c <Arrays::v2Conditions::finalizeNames()+0x94>
  4a4894: e08f5005     	add	r5, pc, r5
  4a4898: e7953006     	ldr	r3, [r5, r6]
  4a489c: e5933000     	ldr	r3, [r3]
  4a48a0: e3530000     	cmp	r3, #0
  4a48a4: 0a00001a     	beq	0x4a4914 <Arrays::v2Conditions::finalizeNames()+0x8c> @ imm = #0x68
  4a48a8: e59f7070     	ldr	r7, [pc, #0x70]         @ 0x4a4920 <Arrays::v2Conditions::finalizeNames()+0x98>
  4a48ac: e7952007     	ldr	r2, [r5, r7]
  4a48b0: e5922000     	ldr	r2, [r2]
  4a48b4: e3520000     	cmp	r2, #0
  4a48b8: 0a000010     	beq	0x4a4900 <Arrays::v2Conditions::finalizeNames()+0x78> @ imm = #0x40
  4a48bc: e3a04000     	mov	r4, #0
  4a48c0: ea000001     	b	0x4a48cc <Arrays::v2Conditions::finalizeNames()+0x44> @ imm = #0x4
  4a48c4: e7953006     	ldr	r3, [r5, r6]
  4a48c8: e5933000     	ldr	r3, [r3]
  4a48cc: e7930104     	ldr	r0, [r3, r4, lsl #2]
  4a48d0: e2844001     	add	r4, r4, #1
  4a48d4: e3500000     	cmp	r0, #0
  4a48d8: 0a000002     	beq	0x4a48e8 <Arrays::v2Conditions::finalizeNames()+0x60> @ imm = #0x8
  4a48dc: ebf9aed7     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1944a4
  4a48e0: e7953006     	ldr	r3, [r5, r6]
  4a48e4: e5933000     	ldr	r3, [r3]
  4a48e8: e7952007     	ldr	r2, [r5, r7]
  4a48ec: e5922000     	ldr	r2, [r2]
  4a48f0: e1520004     	cmp	r2, r4
  4a48f4: 8afffff2     	bhi	0x4a48c4 <Arrays::v2Conditions::finalizeNames()+0x3c> @ imm = #-0x38
  4a48f8: e3530000     	cmp	r3, #0
  4a48fc: 0a000001     	beq	0x4a4908 <Arrays::v2Conditions::finalizeNames()+0x80> @ imm = #0x4
  4a4900: e1a00003     	mov	r0, r3
  4a4904: ebf9aecd     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1944cc
  4a4908: e7953006     	ldr	r3, [r5, r6]
  4a490c: e3a02000     	mov	r2, #0
  4a4910: e5832000     	str	r2, [r3]
  4a4914: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  4a4918: fc 01 4f 00  	.word	0x004f01fc
  4a491c: 54 35 00 00  	.word	0x00003554
  4a4920: 64 2f 00 00  	.word	0x00002f64
