
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004b8c9c <Arrays::v2Events::read(IStreamBase*)>:
  4b8c9c: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  4b8ca0: e24dd00c     	sub	sp, sp, #12
  4b8ca4: e1a0a000     	mov	r10, r0
  4b8ca8: ebf96b78     	bl	0x313a90 <unsigned int StreamReader::readAs<unsigned int>(IStreamBase*)> @ imm = #-0x1a5220
  4b8cac: e59f6128     	ldr	r6, [pc, #0x128]        @ 0x4b8ddc <Arrays::v2Events::read(IStreamBase*)+0x140>
  4b8cb0: e3a03001     	mov	r3, #1
  4b8cb4: e3530000     	cmp	r3, #0
  4b8cb8: e58d0004     	str	r0, [sp, #0x4]
  4b8cbc: e58d3000     	str	r3, [sp]
  4b8cc0: e08f6006     	add	r6, pc, r6
  4b8cc4: 1a000010     	bne	0x4b8d0c <Arrays::v2Events::read(IStreamBase*)+0x70> @ imm = #0x40
  4b8cc8: e28d3004     	add	r3, sp, #4
  4b8ccc: e2832002     	add	r2, r3, #2
  4b8cd0: e2833001     	add	r3, r3, #1
  4b8cd4: e5d20001     	ldrb	r0, [r2, #0x1]
  4b8cd8: e5531001     	ldrb	r1, [r3, #-0x1]
  4b8cdc: e1520003     	cmp	r2, r3
  4b8ce0: e0201001     	eor	r1, r0, r1
  4b8ce4: e5431001     	strb	r1, [r3, #-0x1]
  4b8ce8: e5d20001     	ldrb	r0, [r2, #0x1]
  4b8cec: e0211000     	eor	r1, r1, r0
  4b8cf0: e5c21001     	strb	r1, [r2, #0x1]
  4b8cf4: e5530001     	ldrb	r0, [r3, #-0x1]
  4b8cf8: e2422001     	sub	r2, r2, #1
  4b8cfc: e0211000     	eor	r1, r1, r0
  4b8d00: e5431001     	strb	r1, [r3, #-0x1]
  4b8d04: e2833001     	add	r3, r3, #1
  4b8d08: 8afffff1     	bhi	0x4b8cd4 <Arrays::v2Events::read(IStreamBase*)+0x38> @ imm = #-0x3c
  4b8d0c: e59f70cc     	ldr	r7, [pc, #0xcc]         @ 0x4b8de0 <Arrays::v2Events::read(IStreamBase*)+0x144>
  4b8d10: ebffaea3     	bl	0x4a47a4 <Arrays::v2Events::finalize()> @ imm = #-0x14574
  4b8d14: e59d4004     	ldr	r4, [sp, #0x4]
  4b8d18: e7963007     	ldr	r3, [r6, r7]
  4b8d1c: e3a01001     	mov	r1, #1
  4b8d20: e0840084     	add	r0, r4, r4, lsl #1
  4b8d24: e0800001     	add	r0, r0, r1
  4b8d28: e5834000     	str	r4, [r3]
  4b8d2c: e1a00180     	lsl	r0, r0, #3
  4b8d30: ebf95e0d     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x1a87cc
  4b8d34: e3a03018     	mov	r3, #24
  4b8d38: e3540000     	cmp	r4, #0
  4b8d3c: e8800018     	stm	r0, {r3, r4}
  4b8d40: e2803008     	add	r3, r0, #8
  4b8d44: 0a00000b     	beq	0x4b8d78 <Arrays::v2Events::read(IStreamBase*)+0xdc> @ imm = #0x2c
  4b8d48: e59f1094     	ldr	r1, [pc, #0x94]         @ 0x4b8de4 <Arrays::v2Events::read(IStreamBase*)+0x148>
  4b8d4c: e3a02000     	mov	r2, #0
  4b8d50: e796c001     	ldr	r12, [r6, r1]
  4b8d54: e1a01002     	mov	r1, r2
  4b8d58: e28cc008     	add	r12, r12, #8
  4b8d5c: e2822001     	add	r2, r2, #1
  4b8d60: e1520004     	cmp	r2, r4
  4b8d64: e580c008     	str	r12, [r0, #0x8]
  4b8d68: e5801014     	str	r1, [r0, #0x14]
  4b8d6c: e580101c     	str	r1, [r0, #0x1c]
  4b8d70: e2800018     	add	r0, r0, #24
  4b8d74: 1afffff8     	bne	0x4b8d5c <Arrays::v2Events::read(IStreamBase*)+0xc0> @ imm = #-0x20
  4b8d78: e7962007     	ldr	r2, [r6, r7]
  4b8d7c: e59f8064     	ldr	r8, [pc, #0x64]         @ 0x4b8de8 <Arrays::v2Events::read(IStreamBase*)+0x14c>
  4b8d80: e5921000     	ldr	r1, [r2]
  4b8d84: e7962008     	ldr	r2, [r6, r8]
  4b8d88: e3510000     	cmp	r1, #0
  4b8d8c: e5823000     	str	r3, [r2]
  4b8d90: 0a00000f     	beq	0x4b8dd4 <Arrays::v2Events::read(IStreamBase*)+0x138> @ imm = #0x3c
  4b8d94: e3a04000     	mov	r4, #0
  4b8d98: e1a05004     	mov	r5, r4
  4b8d9c: ea000001     	b	0x4b8da8 <Arrays::v2Events::read(IStreamBase*)+0x10c> @ imm = #0x4
  4b8da0: e7963008     	ldr	r3, [r6, r8]
  4b8da4: e5933000     	ldr	r3, [r3]
  4b8da8: e0830004     	add	r0, r3, r4
  4b8dac: e1a0100a     	mov	r1, r10
  4b8db0: e7933004     	ldr	r3, [r3, r4]
  4b8db4: e1a0e00f     	mov	lr, pc
  4b8db8: e593f00c     	ldr	pc, [r3, #0xc]
  4b8dbc: e7963007     	ldr	r3, [r6, r7]
  4b8dc0: e2855001     	add	r5, r5, #1
  4b8dc4: e2844018     	add	r4, r4, #24
  4b8dc8: e5933000     	ldr	r3, [r3]
  4b8dcc: e1530005     	cmp	r3, r5
  4b8dd0: 8afffff2     	bhi	0x4b8da0 <Arrays::v2Events::read(IStreamBase*)+0x104> @ imm = #-0x38
  4b8dd4: e28dd00c     	add	sp, sp, #12
  4b8dd8: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  4b8ddc: d0 bd 4d 00  	.word	0x004dbdd0
  4b8de0: 60 08 00 00  	.word	0x00000860
  4b8de4: 88 34 00 00  	.word	0x00003488
  4b8de8: 98 1c 00 00  	.word	0x00001c98

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
