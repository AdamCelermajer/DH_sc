
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004b8dec <Arrays::v2Conditions::read(IStreamBase*)>:
  4b8dec: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  4b8df0: e24dd008     	sub	sp, sp, #8
  4b8df4: e1a08000     	mov	r8, r0
  4b8df8: ebf96b24     	bl	0x313a90 <unsigned int StreamReader::readAs<unsigned int>(IStreamBase*)> @ imm = #-0x1a5370
  4b8dfc: e59f5114     	ldr	r5, [pc, #0x114]        @ 0x4b8f18 <Arrays::v2Conditions::read(IStreamBase*)+0x12c>
  4b8e00: e3a03001     	mov	r3, #1
  4b8e04: e3530000     	cmp	r3, #0
  4b8e08: e58d0004     	str	r0, [sp, #0x4]
  4b8e0c: e58d3000     	str	r3, [sp]
  4b8e10: e08f5005     	add	r5, pc, r5
  4b8e14: 1a000010     	bne	0x4b8e5c <Arrays::v2Conditions::read(IStreamBase*)+0x70> @ imm = #0x40
  4b8e18: e28d3004     	add	r3, sp, #4
  4b8e1c: e2832002     	add	r2, r3, #2
  4b8e20: e2833001     	add	r3, r3, #1
  4b8e24: e5d20001     	ldrb	r0, [r2, #0x1]
  4b8e28: e5531001     	ldrb	r1, [r3, #-0x1]
  4b8e2c: e1520003     	cmp	r2, r3
  4b8e30: e0201001     	eor	r1, r0, r1
  4b8e34: e5431001     	strb	r1, [r3, #-0x1]
  4b8e38: e5d20001     	ldrb	r0, [r2, #0x1]
  4b8e3c: e0211000     	eor	r1, r1, r0
  4b8e40: e5c21001     	strb	r1, [r2, #0x1]
  4b8e44: e5530001     	ldrb	r0, [r3, #-0x1]
  4b8e48: e2422001     	sub	r2, r2, #1
  4b8e4c: e0211000     	eor	r1, r1, r0
  4b8e50: e5431001     	strb	r1, [r3, #-0x1]
  4b8e54: e2833001     	add	r3, r3, #1
  4b8e58: 8afffff1     	bhi	0x4b8e24 <Arrays::v2Conditions::read(IStreamBase*)+0x38> @ imm = #-0x3c
  4b8e5c: e59f60b8     	ldr	r6, [pc, #0xb8]         @ 0x4b8f1c <Arrays::v2Conditions::read(IStreamBase*)+0x130>
  4b8e60: ebffaeaf     	bl	0x4a4924 <Arrays::v2Conditions::finalize()> @ imm = #-0x14544
  4b8e64: e59d4004     	ldr	r4, [sp, #0x4]
  4b8e68: e7953006     	ldr	r3, [r5, r6]
  4b8e6c: e3a01001     	mov	r1, #1
  4b8e70: e1a00204     	lsl	r0, r4, #4
  4b8e74: e5834000     	str	r4, [r3]
  4b8e78: e2800008     	add	r0, r0, #8
  4b8e7c: ebf95dba     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x1a8918
  4b8e80: e3a03010     	mov	r3, #16
  4b8e84: e3540000     	cmp	r4, #0
  4b8e88: e8800018     	stm	r0, {r3, r4}
  4b8e8c: e2803008     	add	r3, r0, #8
  4b8e90: 0a000009     	beq	0x4b8ebc <Arrays::v2Conditions::read(IStreamBase*)+0xd0> @ imm = #0x24
  4b8e94: e59f1084     	ldr	r1, [pc, #0x84]         @ 0x4b8f20 <Arrays::v2Conditions::read(IStreamBase*)+0x134>
  4b8e98: e3a02000     	mov	r2, #0
  4b8e9c: e1a0c002     	mov	r12, r2
  4b8ea0: e7951001     	ldr	r1, [r5, r1]
  4b8ea4: e2811008     	add	r1, r1, #8
  4b8ea8: e2822001     	add	r2, r2, #1
  4b8eac: e1520004     	cmp	r2, r4
  4b8eb0: e5801008     	str	r1, [r0, #0x8]
  4b8eb4: e5a0c010     	str	r12, [r0, #0x10]!
  4b8eb8: 1afffffa     	bne	0x4b8ea8 <Arrays::v2Conditions::read(IStreamBase*)+0xbc> @ imm = #-0x18
  4b8ebc: e7952006     	ldr	r2, [r5, r6]
  4b8ec0: e59f705c     	ldr	r7, [pc, #0x5c]         @ 0x4b8f24 <Arrays::v2Conditions::read(IStreamBase*)+0x138>
  4b8ec4: e5921000     	ldr	r1, [r2]
  4b8ec8: e7952007     	ldr	r2, [r5, r7]
  4b8ecc: e3510000     	cmp	r1, #0
  4b8ed0: e5823000     	str	r3, [r2]
  4b8ed4: 0a00000d     	beq	0x4b8f10 <Arrays::v2Conditions::read(IStreamBase*)+0x124> @ imm = #0x34
  4b8ed8: e3a04000     	mov	r4, #0
  4b8edc: ea000001     	b	0x4b8ee8 <Arrays::v2Conditions::read(IStreamBase*)+0xfc> @ imm = #0x4
  4b8ee0: e7953007     	ldr	r3, [r5, r7]
  4b8ee4: e5933000     	ldr	r3, [r3]
  4b8ee8: e0830204     	add	r0, r3, r4, lsl #4
  4b8eec: e1a01008     	mov	r1, r8
  4b8ef0: e7933204     	ldr	r3, [r3, r4, lsl #4]
  4b8ef4: e1a0e00f     	mov	lr, pc
  4b8ef8: e593f00c     	ldr	pc, [r3, #0xc]
  4b8efc: e7953006     	ldr	r3, [r5, r6]
  4b8f00: e2844001     	add	r4, r4, #1
  4b8f04: e5933000     	ldr	r3, [r3]
  4b8f08: e1530004     	cmp	r3, r4
  4b8f0c: 8afffff3     	bhi	0x4b8ee0 <Arrays::v2Conditions::read(IStreamBase*)+0xf4> @ imm = #-0x34
  4b8f10: e28dd008     	add	sp, sp, #8
  4b8f14: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  4b8f18: 80 bc 4d 00  	.word	0x004dbc80
  4b8f1c: 64 2f 00 00  	.word	0x00002f64
  4b8f20: 6c 2f 00 00  	.word	0x00002f6c
  4b8f24: 40 28 00 00  	.word	0x00002840
