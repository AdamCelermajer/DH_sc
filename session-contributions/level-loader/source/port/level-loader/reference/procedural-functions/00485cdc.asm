
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00485cdc <rnd::RandomGenerator::LoadRoomPools(TiXmlNode*)>:
  485cdc: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  485ce0: e59fa15c     	ldr	r10, [pc, #0x15c]       @ 0x485e44 <rnd::RandomGenerator::LoadRoomPools(TiXmlNode*)+0x168>
  485ce4: e24dd014     	sub	sp, sp, #20
  485ce8: e1a05000     	mov	r5, r0
  485cec: e08fa00a     	add	r10, pc, r10
  485cf0: e1a00001     	mov	r0, r1
  485cf4: e1a0100a     	mov	r1, r10
  485cf8: eb023c26     	bl	0x514d98 <TiXmlNode::FirstChild(char const*) const> @ imm = #0x8f098
  485cfc: e2508000     	subs	r8, r0, #0
  485d00: 0a00001b     	beq	0x485d74 <rnd::RandomGenerator::LoadRoomPools(TiXmlNode*)+0x98> @ imm = #0x6c
  485d04: e28d300c     	add	r3, sp, #12
  485d08: e285be12     	add	r11, r5, #288
  485d0c: e3a06000     	mov	r6, #0
  485d10: e58d3004     	str	r3, [sp, #0x4]
  485d14: e3a01000     	mov	r1, #0
  485d18: e3a00014     	mov	r0, #20
  485d1c: ebfa2a13     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x1757b4
  485d20: e1a01008     	mov	r1, r8
  485d24: e5806000     	str	r6, [r0]
  485d28: e5806004     	str	r6, [r0, #0x4]
  485d2c: e5806008     	str	r6, [r0, #0x8]
  485d30: e580600c     	str	r6, [r0, #0xc]
  485d34: e5805010     	str	r5, [r0, #0x10]
  485d38: e1a04000     	mov	r4, r0
  485d3c: eb001b5f     	bl	0x48cac0 <rnd::RoomPool::LoadFromXml(TiXmlNode*)> @ imm = #0x6d7c
  485d40: e595711c     	ldr	r7, [r5, #0x11c]
  485d44: e5953120     	ldr	r3, [r5, #0x120]
  485d48: e1570003     	cmp	r7, r3
  485d4c: 0a00000b     	beq	0x485d80 <rnd::RandomGenerator::LoadRoomPools(TiXmlNode*)+0xa4> @ imm = #0x2c
  485d50: e5874000     	str	r4, [r7]
  485d54: e595311c     	ldr	r3, [r5, #0x11c]
  485d58: e2833004     	add	r3, r3, #4
  485d5c: e585311c     	str	r3, [r5, #0x11c]
  485d60: e1a00008     	mov	r0, r8
  485d64: e1a0100a     	mov	r1, r10
  485d68: eb023bd6     	bl	0x514cc8 <TiXmlNode::NextSibling(char const*) const> @ imm = #0x8ef58
  485d6c: e2508000     	subs	r8, r0, #0
  485d70: 1affffe7     	bne	0x485d14 <rnd::RandomGenerator::LoadRoomPools(TiXmlNode*)+0x38> @ imm = #-0x64
  485d74: e3a00001     	mov	r0, #1
  485d78: e28dd014     	add	sp, sp, #20
  485d7c: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  485d80: e5952118     	ldr	r2, [r5, #0x118]
  485d84: e0622007     	rsb	r2, r2, r7
  485d88: e1a02142     	asr	r2, r2, #2
  485d8c: e3520001     	cmp	r2, #1
  485d90: 20823002     	addhs	r3, r2, r2
  485d94: 32823001     	addlo	r3, r2, #1
  485d98: e3730107     	cmn	r3, #-1073741823
  485d9c: 9a00001a     	bls	0x485e0c <rnd::RandomGenerator::LoadRoomPools(TiXmlNode*)+0x130> @ imm = #0x68
  485da0: e3e03103     	mvn	r3, #-1073741824
  485da4: e1a01003     	mov	r1, r3
  485da8: e1a0000b     	mov	r0, r11
  485dac: e59d2004     	ldr	r2, [sp, #0x4]
  485db0: e58d300c     	str	r3, [sp, #0xc]
  485db4: ebfffbcf     	bl	0x484cf8 <std::allocator<rnd::RoomPool*>::_M_allocate(unsigned int, unsigned int&)> @ imm = #-0x10c4
  485db8: e5951118     	ldr	r1, [r5, #0x118]
  485dbc: e1a09000     	mov	r9, r0
  485dc0: e0577001     	subs	r7, r7, r1
  485dc4: 01a07000     	moveq	r7, r0
  485dc8: 1a000012     	bne	0x485e18 <rnd::RandomGenerator::LoadRoomPools(TiXmlNode*)+0x13c> @ imm = #0x48
  485dcc: e4874004     	str	r4, [r7], #4
  485dd0: e5950118     	ldr	r0, [r5, #0x118]
  485dd4: e5951120     	ldr	r1, [r5, #0x120]
  485dd8: e3500000     	cmp	r0, #0
  485ddc: 0a000004     	beq	0x485df4 <rnd::RandomGenerator::LoadRoomPools(TiXmlNode*)+0x118> @ imm = #0x10
  485de0: e0601001     	rsb	r1, r0, r1
  485de4: e3c11003     	bic	r1, r1, #3
  485de8: e3510080     	cmp	r1, #128
  485dec: 8a00000d     	bhi	0x485e28 <rnd::RandomGenerator::LoadRoomPools(TiXmlNode*)+0x14c> @ imm = #0x34
  485df0: eb0a0c42     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x283108
  485df4: e59d300c     	ldr	r3, [sp, #0xc]
  485df8: e5859118     	str	r9, [r5, #0x118]
  485dfc: e585711c     	str	r7, [r5, #0x11c]
  485e00: e0899103     	add	r9, r9, r3, lsl #2
  485e04: e5859120     	str	r9, [r5, #0x120]
  485e08: eaffffd4     	b	0x485d60 <rnd::RandomGenerator::LoadRoomPools(TiXmlNode*)+0x84> @ imm = #-0xb0
  485e0c: e1520003     	cmp	r2, r3
  485e10: 9affffe3     	bls	0x485da4 <rnd::RandomGenerator::LoadRoomPools(TiXmlNode*)+0xc8> @ imm = #-0x74
  485e14: eaffffe1     	b	0x485da0 <rnd::RandomGenerator::LoadRoomPools(TiXmlNode*)+0xc4> @ imm = #-0x7c
  485e18: e1a02007     	mov	r2, r7
  485e1c: ebfa2045     	bl	0x30df38 <.plt+0x1c4>   @ imm = #-0x177eec
  485e20: e0807007     	add	r7, r0, r7
  485e24: eaffffe8     	b	0x485dcc <rnd::RandomGenerator::LoadRoomPools(TiXmlNode*)+0xf0> @ imm = #-0x60
  485e28: ebfa2984     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1759f0
  485e2c: e59d300c     	ldr	r3, [sp, #0xc]
  485e30: e5859118     	str	r9, [r5, #0x118]
  485e34: e585711c     	str	r7, [r5, #0x11c]
  485e38: e0899103     	add	r9, r9, r3, lsl #2
  485e3c: e5859120     	str	r9, [r5, #0x120]
  485e40: eaffffc6     	b	0x485d60 <rnd::RandomGenerator::LoadRoomPools(TiXmlNode*)+0x84> @ imm = #-0xe8
  485e44: f4 ee 44 00  	.word	0x0044eef4
