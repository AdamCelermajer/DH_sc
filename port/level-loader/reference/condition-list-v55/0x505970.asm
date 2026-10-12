
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00505970 <Structs::v2CondAnd::read(IStreamBase*)>:
  505970: e92d40f0     	push	{r4, r5, r6, r7, lr}
  505974: e1a05000     	mov	r5, r0
  505978: e24dd00c     	sub	sp, sp, #12
  50597c: e1a00001     	mov	r0, r1
  505980: e1a06001     	mov	r6, r1
  505984: e59f7194     	ldr	r7, [pc, #0x194]        @ 0x505b20 <Structs::v2CondAnd::read(IStreamBase*)+0x1b0>
  505988: e2851004     	add	r1, r5, #4
  50598c: ebfb6603     	bl	0x3df1a0 <void StreamReader::readAs<unsigned int>(IStreamBase*, unsigned int*)> @ imm = #-0x1267f4
  505990: e3a03001     	mov	r3, #1
  505994: e3530000     	cmp	r3, #0
  505998: e58d3004     	str	r3, [sp, #0x4]
  50599c: e08f7007     	add	r7, pc, r7
  5059a0: 1a00000f     	bne	0x5059e4 <Structs::v2CondAnd::read(IStreamBase*)+0x74> @ imm = #0x3c
  5059a4: e2853005     	add	r3, r5, #5
  5059a8: e2852006     	add	r2, r5, #6
  5059ac: e5d20001     	ldrb	r0, [r2, #0x1]
  5059b0: e5531001     	ldrb	r1, [r3, #-0x1]
  5059b4: e1520003     	cmp	r2, r3
  5059b8: e0201001     	eor	r1, r0, r1
  5059bc: e5431001     	strb	r1, [r3, #-0x1]
  5059c0: e5d20001     	ldrb	r0, [r2, #0x1]
  5059c4: e0211000     	eor	r1, r1, r0
  5059c8: e5c21001     	strb	r1, [r2, #0x1]
  5059cc: e5530001     	ldrb	r0, [r3, #-0x1]
  5059d0: e2422001     	sub	r2, r2, #1
  5059d4: e0211000     	eor	r1, r1, r0
  5059d8: e5431001     	strb	r1, [r3, #-0x1]
  5059dc: e2833001     	add	r3, r3, #1
  5059e0: 8afffff1     	bhi	0x5059ac <Structs::v2CondAnd::read(IStreamBase*)+0x3c> @ imm = #-0x3c
  5059e4: e5953008     	ldr	r3, [r5, #0x8]
  5059e8: e3530000     	cmp	r3, #0
  5059ec: 0a00000f     	beq	0x505a30 <Structs::v2CondAnd::read(IStreamBase*)+0xc0> @ imm = #0x3c
  5059f0: e5130004     	ldr	r0, [r3, #-0x4]
  5059f4: e0830200     	add	r0, r3, r0, lsl #4
  5059f8: e1530000     	cmp	r3, r0
  5059fc: 1a000001     	bne	0x505a08 <Structs::v2CondAnd::read(IStreamBase*)+0x98> @ imm = #0x4
  505a00: ea000008     	b	0x505a28 <Structs::v2CondAnd::read(IStreamBase*)+0xb8> @ imm = #0x20
  505a04: e1a00004     	mov	r0, r4
  505a08: e2404010     	sub	r4, r0, #16
  505a0c: e5103010     	ldr	r3, [r0, #-0x10]
  505a10: e1a00004     	mov	r0, r4
  505a14: e1a0e00f     	mov	lr, pc
  505a18: e593f000     	ldr	pc, [r3]
  505a1c: e5950008     	ldr	r0, [r5, #0x8]
  505a20: e1500004     	cmp	r0, r4
  505a24: 1afffff6     	bne	0x505a04 <Structs::v2CondAnd::read(IStreamBase*)+0x94> @ imm = #-0x28
  505a28: e2400008     	sub	r0, r0, #8
  505a2c: ebf82a83     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1f55f4
  505a30: e5954004     	ldr	r4, [r5, #0x4]
  505a34: e3a01001     	mov	r1, #1
  505a38: e1a00204     	lsl	r0, r4, #4
  505a3c: e2800008     	add	r0, r0, #8
  505a40: ebf82ac9     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x1f54dc
  505a44: e3a03010     	mov	r3, #16
  505a48: e3540000     	cmp	r4, #0
  505a4c: e8800018     	stm	r0, {r3, r4}
  505a50: e2803008     	add	r3, r0, #8
  505a54: 0a000008     	beq	0x505a7c <Structs::v2CondAnd::read(IStreamBase*)+0x10c> @ imm = #0x20
  505a58: e59f10c4     	ldr	r1, [pc, #0xc4]         @ 0x505b24 <Structs::v2CondAnd::read(IStreamBase*)+0x1b4>
  505a5c: e3a02000     	mov	r2, #0
  505a60: e7971001     	ldr	r1, [r7, r1]
  505a64: e2811008     	add	r1, r1, #8
  505a68: e2822001     	add	r2, r2, #1
  505a6c: e1520004     	cmp	r2, r4
  505a70: e5801008     	str	r1, [r0, #0x8]
  505a74: e2800010     	add	r0, r0, #16
  505a78: 1afffffa     	bne	0x505a68 <Structs::v2CondAnd::read(IStreamBase*)+0xf8> @ imm = #-0x18
  505a7c: e5952004     	ldr	r2, [r5, #0x4]
  505a80: e5853008     	str	r3, [r5, #0x8]
  505a84: e3520000     	cmp	r2, #0
  505a88: 0a00000b     	beq	0x505abc <Structs::v2CondAnd::read(IStreamBase*)+0x14c> @ imm = #0x2c
  505a8c: e3a04000     	mov	r4, #0
  505a90: ea000000     	b	0x505a98 <Structs::v2CondAnd::read(IStreamBase*)+0x128> @ imm = #0x0
  505a94: e5953008     	ldr	r3, [r5, #0x8]
  505a98: e0830204     	add	r0, r3, r4, lsl #4
  505a9c: e1a01006     	mov	r1, r6
  505aa0: e7933204     	ldr	r3, [r3, r4, lsl #4]
  505aa4: e1a0e00f     	mov	lr, pc
  505aa8: e593f00c     	ldr	pc, [r3, #0xc]
  505aac: e5953004     	ldr	r3, [r5, #0x4]
  505ab0: e2844001     	add	r4, r4, #1
  505ab4: e1530004     	cmp	r3, r4
  505ab8: 8afffff5     	bhi	0x505a94 <Structs::v2CondAnd::read(IStreamBase*)+0x124> @ imm = #-0x2c
  505abc: e1a00006     	mov	r0, r6
  505ac0: e285100c     	add	r1, r5, #12
  505ac4: ebfd4d71     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0xaca3c
  505ac8: e3a03001     	mov	r3, #1
  505acc: e3530000     	cmp	r3, #0
  505ad0: e58d3004     	str	r3, [sp, #0x4]
  505ad4: 1a00000f     	bne	0x505b18 <Structs::v2CondAnd::read(IStreamBase*)+0x1a8> @ imm = #0x3c
  505ad8: e285300e     	add	r3, r5, #14
  505adc: e285500d     	add	r5, r5, #13
  505ae0: e5d31001     	ldrb	r1, [r3, #0x1]
  505ae4: e5552001     	ldrb	r2, [r5, #-0x1]
  505ae8: e1550003     	cmp	r5, r3
  505aec: e0212002     	eor	r2, r1, r2
  505af0: e5452001     	strb	r2, [r5, #-0x1]
  505af4: e5d31001     	ldrb	r1, [r3, #0x1]
  505af8: e0222001     	eor	r2, r2, r1
  505afc: e5c32001     	strb	r2, [r3, #0x1]
  505b00: e5551001     	ldrb	r1, [r5, #-0x1]
  505b04: e2433001     	sub	r3, r3, #1
  505b08: e0222001     	eor	r2, r2, r1
  505b0c: e5452001     	strb	r2, [r5, #-0x1]
  505b10: e2855001     	add	r5, r5, #1
  505b14: 3afffff1     	blo	0x505ae0 <Structs::v2CondAnd::read(IStreamBase*)+0x170> @ imm = #-0x3c
  505b18: e28dd00c     	add	sp, sp, #12
  505b1c: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  505b20: f4 f0 48 00  	.word	0x0048f0f4
  505b24: 48 09 00 00  	.word	0x00000948

00505b28 <Structs::v2Condition::read(IStreamBase*)>:
  505b28: e92d4010     	push	{r4, lr}
  505b2c: e1a04000     	mov	r4, r0
  505b30: e24dd008     	sub	sp, sp, #8
  505b34: e1a00001     	mov	r0, r1
  505b38: e2841004     	add	r1, r4, #4
  505b3c: ebfd4d53     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0xacab4
  505b40: e3a03001     	mov	r3, #1
  505b44: e3530000     	cmp	r3, #0
  505b48: e58d3004     	str	r3, [sp, #0x4]
  505b4c: 1a00000f     	bne	0x505b90 <Structs::v2Condition::read(IStreamBase*)+0x68> @ imm = #0x3c
  505b50: e2843006     	add	r3, r4, #6
  505b54: e2844005     	add	r4, r4, #5
  505b58: e5d31001     	ldrb	r1, [r3, #0x1]
  505b5c: e5542001     	ldrb	r2, [r4, #-0x1]
  505b60: e1540003     	cmp	r4, r3
