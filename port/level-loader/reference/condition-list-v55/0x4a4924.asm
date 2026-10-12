
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004a4924 <Arrays::v2Conditions::finalize()>:
  4a4924: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  4a4928: e59f50c0     	ldr	r5, [pc, #0xc0]         @ 0x4a49f0 <Arrays::v2Conditions::finalize()+0xcc>
  4a492c: e59f60c0     	ldr	r6, [pc, #0xc0]         @ 0x4a49f4 <Arrays::v2Conditions::finalize()+0xd0>
  4a4930: e08f5005     	add	r5, pc, r5
  4a4934: e7953006     	ldr	r3, [r5, r6]
  4a4938: e5933000     	ldr	r3, [r3]
  4a493c: e3530000     	cmp	r3, #0
  4a4940: 0a000029     	beq	0x4a49ec <Arrays::v2Conditions::finalize()+0xc8> @ imm = #0xa4
  4a4944: e59f70ac     	ldr	r7, [pc, #0xac]         @ 0x4a49f8 <Arrays::v2Conditions::finalize()+0xd4>
  4a4948: e7952007     	ldr	r2, [r5, r7]
  4a494c: e5922000     	ldr	r2, [r2]
  4a4950: e3520000     	cmp	r2, #0
  4a4954: 0a000010     	beq	0x4a499c <Arrays::v2Conditions::finalize()+0x78> @ imm = #0x40
  4a4958: e3a04000     	mov	r4, #0
  4a495c: ea000001     	b	0x4a4968 <Arrays::v2Conditions::finalize()+0x44> @ imm = #0x4
  4a4960: e7953006     	ldr	r3, [r5, r6]
  4a4964: e5933000     	ldr	r3, [r3]
  4a4968: e0830204     	add	r0, r3, r4, lsl #4
  4a496c: e7933204     	ldr	r3, [r3, r4, lsl #4]
  4a4970: e1a0e00f     	mov	lr, pc
  4a4974: e593f008     	ldr	pc, [r3, #0x8]
  4a4978: e7953007     	ldr	r3, [r5, r7]
  4a497c: e2844001     	add	r4, r4, #1
  4a4980: e5933000     	ldr	r3, [r3]
  4a4984: e1530004     	cmp	r3, r4
  4a4988: 8afffff4     	bhi	0x4a4960 <Arrays::v2Conditions::finalize()+0x3c> @ imm = #-0x30
  4a498c: e7953006     	ldr	r3, [r5, r6]
  4a4990: e5933000     	ldr	r3, [r3]
  4a4994: e3530000     	cmp	r3, #0
  4a4998: 0a000010     	beq	0x4a49e0 <Arrays::v2Conditions::finalize()+0xbc> @ imm = #0x40
  4a499c: e5130004     	ldr	r0, [r3, #-0x4]
  4a49a0: e0830200     	add	r0, r3, r0, lsl #4
  4a49a4: e1530000     	cmp	r3, r0
  4a49a8: 1a000001     	bne	0x4a49b4 <Arrays::v2Conditions::finalize()+0x90> @ imm = #0x4
  4a49ac: ea000009     	b	0x4a49d8 <Arrays::v2Conditions::finalize()+0xb4> @ imm = #0x24
  4a49b0: e1a00004     	mov	r0, r4
  4a49b4: e2404010     	sub	r4, r0, #16
  4a49b8: e5103010     	ldr	r3, [r0, #-0x10]
  4a49bc: e1a00004     	mov	r0, r4
  4a49c0: e1a0e00f     	mov	lr, pc
  4a49c4: e593f000     	ldr	pc, [r3]
  4a49c8: e7953006     	ldr	r3, [r5, r6]
  4a49cc: e5930000     	ldr	r0, [r3]
  4a49d0: e1500004     	cmp	r0, r4
  4a49d4: 1afffff5     	bne	0x4a49b0 <Arrays::v2Conditions::finalize()+0x8c> @ imm = #-0x2c
  4a49d8: e2400008     	sub	r0, r0, #8
  4a49dc: ebf9ae97     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1945a4
  4a49e0: e7953006     	ldr	r3, [r5, r6]
  4a49e4: e3a02000     	mov	r2, #0
  4a49e8: e5832000     	str	r2, [r3]
  4a49ec: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  4a49f0: 60 01 4f 00  	.word	0x004f0160
  4a49f4: 40 28 00 00  	.word	0x00002840
  4a49f8: 64 2f 00 00  	.word	0x00002f64
