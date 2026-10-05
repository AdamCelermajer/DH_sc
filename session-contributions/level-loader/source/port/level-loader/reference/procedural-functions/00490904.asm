
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00490904 <rnd::RoomPool::ComputeSizeOfRules()>:
  490904: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  490908: e1a05000     	mov	r5, r0
  49090c: e5901004     	ldr	r1, [r0, #0x4]
  490910: e5900008     	ldr	r0, [r0, #0x8]
  490914: e24dd014     	sub	sp, sp, #20
  490918: e0613000     	rsb	r3, r1, r0
  49091c: e1a031c3     	asr	r3, r3, #3
  490920: e0837103     	add	r7, r3, r3, lsl #2
  490924: e0877207     	add	r7, r7, r7, lsl #4
  490928: e0877407     	add	r7, r7, r7, lsl #8
  49092c: e0877807     	add	r7, r7, r7, lsl #16
  490930: e0937087     	adds	r7, r3, r7, lsl #1
  490934: 01a0c007     	moveq	r12, r7
  490938: 01a0400c     	moveq	r4, r12
  49093c: 0a00000d     	beq	0x490978 <rnd::RoomPool::ComputeSizeOfRules()+0x74> @ imm = #0x34
  490940: e3a02000     	mov	r2, #0
  490944: e1a03002     	mov	r3, r2
  490948: e1a0c002     	mov	r12, r2
  49094c: e1a04002     	mov	r4, r2
  490950: e3a08018     	mov	r8, #24
  490954: e0221298     	mla	r2, r8, r2, r1
  490958: e2833001     	add	r3, r3, #1
  49095c: e5926010     	ldr	r6, [r2, #0x10]
  490960: e592200c     	ldr	r2, [r2, #0xc]
  490964: e1530007     	cmp	r3, r7
  490968: e0844006     	add	r4, r4, r6
  49096c: e08cc002     	add	r12, r12, r2
  490970: e1a02003     	mov	r2, r3
  490974: 1afffff6     	bne	0x490954 <rnd::RoomPool::ComputeSizeOfRules()+0x50> @ imm = #-0x28
  490978: e5956000     	ldr	r6, [r5]
  49097c: e156000c     	cmp	r6, r12
  490980: ba000060     	blt	0x490b08 <rnd::RoomPool::ComputeSizeOfRules()+0x204> @ imm = #0x180
  490984: e1560004     	cmp	r6, r4
  490988: ca00005e     	bgt	0x490b08 <rnd::RoomPool::ComputeSizeOfRules()+0x204> @ imm = #0x178
  49098c: e06c6006     	rsb	r6, r12, r6
  490990: e3560000     	cmp	r6, #0
  490994: da00008c     	ble	0x490bcc <rnd::RoomPool::ComputeSizeOfRules()+0x2c8> @ imm = #0x230
  490998: e0613000     	rsb	r3, r1, r0
  49099c: e1a031c3     	asr	r3, r3, #3
  4909a0: e3a04000     	mov	r4, #0
  4909a4: e0832103     	add	r2, r3, r3, lsl #2
  4909a8: e58d4000     	str	r4, [sp]
  4909ac: e0822202     	add	r2, r2, r2, lsl #4
  4909b0: e58d4004     	str	r4, [sp, #0x4]
  4909b4: e0822402     	add	r2, r2, r2, lsl #8
  4909b8: e58d4008     	str	r4, [sp, #0x8]
  4909bc: e0822802     	add	r2, r2, r2, lsl #16
  4909c0: e0833082     	add	r3, r3, r2, lsl #1
  4909c4: e1530004     	cmp	r3, r4
  4909c8: 0a00001d     	beq	0x490a44 <rnd::RoomPool::ComputeSizeOfRules()+0x140> @ imm = #0x74
  4909cc: e1a0c004     	mov	r12, r4
  4909d0: e3a08018     	mov	r8, #24
  4909d4: e28da008     	add	r10, sp, #8
  4909d8: e28d900c     	add	r9, sp, #12
  4909dc: e0231c98     	mla	r3, r8, r12, r1
  4909e0: e5932010     	ldr	r2, [r3, #0x10]
  4909e4: e5933014     	ldr	r3, [r3, #0x14]
  4909e8: e1530002     	cmp	r3, r2
  4909ec: aa000009     	bge	0x490a18 <rnd::RoomPool::ComputeSizeOfRules()+0x114> @ imm = #0x24
  4909f0: e59d7004     	ldr	r7, [sp, #0x4]
  4909f4: e59d3008     	ldr	r3, [sp, #0x8]
  4909f8: e1570003     	cmp	r7, r3
  4909fc: 0a000047     	beq	0x490b20 <rnd::RoomPool::ComputeSizeOfRules()+0x21c> @ imm = #0x11c
  490a00: e5874000     	str	r4, [r7]
  490a04: e59d3004     	ldr	r3, [sp, #0x4]
  490a08: e5951004     	ldr	r1, [r5, #0x4]
  490a0c: e5950008     	ldr	r0, [r5, #0x8]
  490a10: e2833004     	add	r3, r3, #4
  490a14: e58d3004     	str	r3, [sp, #0x4]
  490a18: e0613000     	rsb	r3, r1, r0
  490a1c: e1a031c3     	asr	r3, r3, #3
  490a20: e2844001     	add	r4, r4, #1
  490a24: e0832103     	add	r2, r3, r3, lsl #2
  490a28: e1a0c004     	mov	r12, r4
  490a2c: e0822202     	add	r2, r2, r2, lsl #4
  490a30: e0822402     	add	r2, r2, r2, lsl #8
  490a34: e0822802     	add	r2, r2, r2, lsl #16
  490a38: e0833082     	add	r3, r3, r2, lsl #1
  490a3c: e1530004     	cmp	r3, r4
  490a40: 8affffe5     	bhi	0x4909dc <rnd::RoomPool::ComputeSizeOfRules()+0xd8> @ imm = #-0x6c
  490a44: e3a04018     	mov	r4, #24
  490a48: ea000001     	b	0x490a54 <rnd::RoomPool::ComputeSizeOfRules()+0x150> @ imm = #0x4
  490a4c: e2566001     	subs	r6, r6, #1
  490a50: 0a000021     	beq	0x490adc <rnd::RoomPool::ComputeSizeOfRules()+0x1d8> @ imm = #0x84
  490a54: ebf9f8d3     	bl	0x30eda8 <.plt+0x1034>  @ imm = #-0x181cb4
  490a58: e59d7000     	ldr	r7, [sp]
  490a5c: e59d1004     	ldr	r1, [sp, #0x4]
  490a60: e0671001     	rsb	r1, r7, r1
  490a64: e1a01141     	asr	r1, r1, #2
  490a68: ebf9f82f     	bl	0x30eb2c <.plt+0xdb8>   @ imm = #-0x181f44
  490a6c: e5953004     	ldr	r3, [r5, #0x4]
  490a70: e7972101     	ldr	r2, [r7, r1, lsl #2]
  490a74: e0233294     	mla	r3, r4, r2, r3
  490a78: e5932014     	ldr	r2, [r3, #0x14]
  490a7c: e2822001     	add	r2, r2, #1
  490a80: e5832014     	str	r2, [r3, #0x14]
  490a84: e59d0000     	ldr	r0, [sp]
  490a88: e5953004     	ldr	r3, [r5, #0x4]
  490a8c: e7902101     	ldr	r2, [r0, r1, lsl #2]
  490a90: e0800101     	add	r0, r0, r1, lsl #2
  490a94: e0233294     	mla	r3, r4, r2, r3
  490a98: e5932010     	ldr	r2, [r3, #0x10]
  490a9c: e5933014     	ldr	r3, [r3, #0x14]
  490aa0: e1530002     	cmp	r3, r2
  490aa4: 1affffe8     	bne	0x490a4c <rnd::RoomPool::ComputeSizeOfRules()+0x148> @ imm = #-0x60
  490aa8: e59d3004     	ldr	r3, [sp, #0x4]
  490aac: e2801004     	add	r1, r0, #4
  490ab0: e1510003     	cmp	r1, r3
  490ab4: 0a000004     	beq	0x490acc <rnd::RoomPool::ComputeSizeOfRules()+0x1c8> @ imm = #0x10
  490ab8: e0532001     	subs	r2, r3, r1
  490abc: 01a01003     	moveq	r1, r3
  490ac0: 0a000001     	beq	0x490acc <rnd::RoomPool::ComputeSizeOfRules()+0x1c8> @ imm = #0x4
  490ac4: ebf9f51b     	bl	0x30df38 <.plt+0x1c4>   @ imm = #-0x182b94
  490ac8: e59d1004     	ldr	r1, [sp, #0x4]
  490acc: e2411004     	sub	r1, r1, #4
  490ad0: e2566001     	subs	r6, r6, #1
  490ad4: e58d1004     	str	r1, [sp, #0x4]
  490ad8: 1affffdd     	bne	0x490a54 <rnd::RoomPool::ComputeSizeOfRules()+0x150> @ imm = #-0x8c
  490adc: e59d0000     	ldr	r0, [sp]
  490ae0: e3500000     	cmp	r0, #0
  490ae4: 0a000038     	beq	0x490bcc <rnd::RoomPool::ComputeSizeOfRules()+0x2c8> @ imm = #0xe0
  490ae8: e59d1008     	ldr	r1, [sp, #0x8]
  490aec: e0601001     	rsb	r1, r0, r1
  490af0: e3c11003     	bic	r1, r1, #3
  490af4: e3510080     	cmp	r1, #128
  490af8: 8a000005     	bhi	0x490b14 <rnd::RoomPool::ComputeSizeOfRules()+0x210> @ imm = #0x14
  490afc: eb09e0ff     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x2783fc
  490b00: e3a00001     	mov	r0, #1
  490b04: ea000000     	b	0x490b0c <rnd::RoomPool::ComputeSizeOfRules()+0x208> @ imm = #0x0
  490b08: e3a00000     	mov	r0, #0
  490b0c: e28dd014     	add	sp, sp, #20
  490b10: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  490b14: ebf9fe49     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1806dc
  490b18: e3a00001     	mov	r0, #1
  490b1c: eafffffa     	b	0x490b0c <rnd::RoomPool::ComputeSizeOfRules()+0x208> @ imm = #-0x18
  490b20: e59d2000     	ldr	r2, [sp]
  490b24: e0622007     	rsb	r2, r2, r7
  490b28: e1a02142     	asr	r2, r2, #2
  490b2c: e3520001     	cmp	r2, #1
  490b30: 20823002     	addhs	r3, r2, r2
  490b34: 32823001     	addlo	r3, r2, #1
  490b38: e3730107     	cmn	r3, #-1073741823
  490b3c: 8a000020     	bhi	0x490bc4 <rnd::RoomPool::ComputeSizeOfRules()+0x2c0> @ imm = #0x80
  490b40: e1520003     	cmp	r2, r3
  490b44: 8a00001e     	bhi	0x490bc4 <rnd::RoomPool::ComputeSizeOfRules()+0x2c0> @ imm = #0x78
  490b48: e1a01003     	mov	r1, r3
  490b4c: e1a0000a     	mov	r0, r10
  490b50: e1a02009     	mov	r2, r9
  490b54: e58d300c     	str	r3, [sp, #0xc]
  490b58: ebfb3c7f     	bl	0x35fd5c <std::allocator<int>::_M_allocate(unsigned int, unsigned int&)> @ imm = #-0x130e04
  490b5c: e59d1000     	ldr	r1, [sp]
  490b60: e1a0b000     	mov	r11, r0
  490b64: e0577001     	subs	r7, r7, r1
  490b68: 01a07000     	moveq	r7, r0
  490b6c: 0a000002     	beq	0x490b7c <rnd::RoomPool::ComputeSizeOfRules()+0x278> @ imm = #0x8
  490b70: e1a02007     	mov	r2, r7
  490b74: ebf9f4ef     	bl	0x30df38 <.plt+0x1c4>   @ imm = #-0x182c44
  490b78: e0807007     	add	r7, r0, r7
  490b7c: e4874004     	str	r4, [r7], #4
  490b80: e59d0000     	ldr	r0, [sp]
  490b84: e59d3008     	ldr	r3, [sp, #0x8]
  490b88: e3500000     	cmp	r0, #0
  490b8c: 0a000004     	beq	0x490ba4 <rnd::RoomPool::ComputeSizeOfRules()+0x2a0> @ imm = #0x10
  490b90: e0603003     	rsb	r3, r0, r3
  490b94: e3c31003     	bic	r1, r3, #3
  490b98: e3510080     	cmp	r1, #128
  490b9c: 8a00000c     	bhi	0x490bd4 <rnd::RoomPool::ComputeSizeOfRules()+0x2d0> @ imm = #0x30
  490ba0: eb09e0d6     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x278358
  490ba4: e59d300c     	ldr	r3, [sp, #0xc]
  490ba8: e5951004     	ldr	r1, [r5, #0x4]
  490bac: e5950008     	ldr	r0, [r5, #0x8]
  490bb0: e08b3103     	add	r3, r11, r3, lsl #2
  490bb4: e58db000     	str	r11, [sp]
  490bb8: e58d7004     	str	r7, [sp, #0x4]
  490bbc: e58d3008     	str	r3, [sp, #0x8]
  490bc0: eaffff94     	b	0x490a18 <rnd::RoomPool::ComputeSizeOfRules()+0x114> @ imm = #-0x1b0
  490bc4: e3e03103     	mvn	r3, #-1073741824
  490bc8: eaffffde     	b	0x490b48 <rnd::RoomPool::ComputeSizeOfRules()+0x244> @ imm = #-0x88
  490bcc: e3a00001     	mov	r0, #1
  490bd0: eaffffcd     	b	0x490b0c <rnd::RoomPool::ComputeSizeOfRules()+0x208> @ imm = #-0xcc
  490bd4: ebf9fe19     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x18079c
  490bd8: eafffff1     	b	0x490ba4 <rnd::RoomPool::ComputeSizeOfRules()+0x2a0> @ imm = #-0x3c
