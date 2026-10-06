
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003b3d38 <Character::SafeGetCharPropsId()>:
  3b3d38: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  3b3d3c: e30163c8     	movw	r6, #0x13c8
  3b3d40: e19030f6     	ldrsh	r3, [r0, r6]
  3b3d44: e59f7294     	ldr	r7, [pc, #0x294]        @ 0x3b3fe0 <Character::SafeGetCharPropsId()+0x2a8>
  3b3d48: e1a04000     	mov	r4, r0
  3b3d4c: e3730001     	cmn	r3, #1
  3b3d50: e08f7007     	add	r7, pc, r7
  3b3d54: e24dd00c     	sub	sp, sp, #12
  3b3d58: 11a00003     	movne	r0, r3
  3b3d5c: 0a000001     	beq	0x3b3d68 <Character::SafeGetCharPropsId()+0x30> @ imm = #0x4
  3b3d60: e28dd00c     	add	sp, sp, #12
  3b3d64: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  3b3d68: e5943000     	ldr	r3, [r4]
  3b3d6c: e1a0e00f     	mov	lr, pc
  3b3d70: e593f028     	ldr	pc, [r3, #0x28]
  3b3d74: e250a000     	subs	r10, r0, #0
  3b3d78: 1a000013     	bne	0x3b3dcc <Character::SafeGetCharPropsId()+0x94> @ imm = #0x4c
  3b3d7c: e30133a8     	movw	r3, #0x13a8
  3b3d80: e7942003     	ldr	r2, [r4, r3]
  3b3d84: e30133ac     	movw	r3, #0x13ac
  3b3d88: e7943003     	ldr	r3, [r4, r3]
  3b3d8c: e1520003     	cmp	r2, r3
  3b3d90: 0a000062     	beq	0x3b3f20 <Character::SafeGetCharPropsId()+0x1e8> @ imm = #0x188
  3b3d94: e1a00004     	mov	r0, r4
  3b3d98: ebfffe53     	bl	0x3b36ec <Character::SafeGetCharPropsTemplateId()> @ imm = #-0x6b4
  3b3d9c: e3500000     	cmp	r0, #0
  3b3da0: ba000007     	blt	0x3b3dc4 <Character::SafeGetCharPropsId()+0x8c> @ imm = #0x1c
  3b3da4: e59f3238     	ldr	r3, [pc, #0x238]        @ 0x3b3fe4 <Character::SafeGetCharPropsId()+0x2ac>
  3b3da8: e3a0500c     	mov	r5, #12
  3b3dac: e7973003     	ldr	r3, [r7, r3]
  3b3db0: e5933000     	ldr	r3, [r3]
  3b3db4: e0253095     	mla	r5, r5, r0, r3
  3b3db8: e5951004     	ldr	r1, [r5, #0x4]
  3b3dbc: e3510000     	cmp	r1, #0
  3b3dc0: 1a000010     	bne	0x3b3e08 <Character::SafeGetCharPropsId()+0xd0> @ imm = #0x40
  3b3dc4: e19400f6     	ldrsh	r0, [r4, r6]
  3b3dc8: eaffffe4     	b	0x3b3d60 <Character::SafeGetCharPropsId()+0x28> @ imm = #-0x70
  3b3dcc: e3a01001     	mov	r1, #1
  3b3dd0: e1a00004     	mov	r0, r4
  3b3dd4: eb0021bd     	bl	0x3bc4d0 <Character::SG_Load(int)> @ imm = #0x86f4
  3b3dd8: e1a00004     	mov	r0, r4
  3b3ddc: eb001e86     	bl	0x3bb7fc <Character::SG_GetPlayerClass() const> @ imm = #0x7a18
  3b3de0: e6ff0070     	uxth	r0, r0
  3b3de4: e6bf1070     	sxth	r1, r0
  3b3de8: e3710001     	cmn	r1, #1
  3b3dec: e18400b6     	strh	r0, [r4, r6]
  3b3df0: 0a000031     	beq	0x3b3ebc <Character::SafeGetCharPropsId()+0x184> @ imm = #0xc4
  3b3df4: e1a00004     	mov	r0, r4
  3b3df8: eb001e85     	bl	0x3bb814 <Character::SG_SetPlayerClass(int)> @ imm = #0x7a14
  3b3dfc: e30133c8     	movw	r3, #0x13c8
  3b3e00: e19400f3     	ldrsh	r0, [r4, r3]
  3b3e04: eaffffd5     	b	0x3b3d60 <Character::SafeGetCharPropsId()+0x28> @ imm = #-0xac
  3b3e08: e59f21d8     	ldr	r2, [pc, #0x1d8]        @ 0x3b3fe8 <Character::SafeGetCharPropsId()+0x2b0>
  3b3e0c: e30ee6ab     	movw	lr, #0xe6ab
  3b3e10: e30d3b17     	movw	r3, #0xdb17
  3b3e14: e7972002     	ldr	r2, [r7, r2]
  3b3e18: e3423b52     	movt	r3, #0x2b52
  3b3e1c: e30fc26b     	movw	r12, #0xf26b
  3b3e20: e5920000     	ldr	r0, [r2]
  3b3e24: e340c0da     	movt	r12, #0xda
  3b3e28: e000009e     	mul	r0, lr, r0
  3b3e2c: e2800a2b     	add	r0, r0, #176128
  3b3e30: e2800fff     	add	r0, r0, #1020
  3b3e34: e2800001     	add	r0, r0, #1
  3b3e38: e083e093     	umull	lr, r3, r3, r0
  3b3e3c: e063e000     	rsb	lr, r3, r0
  3b3e40: e08330ae     	add	r3, r3, lr, lsr #1
  3b3e44: e1a03ba3     	lsr	r3, r3, #23
  3b3e48: e063039c     	mls	r3, r12, r3, r0
  3b3e4c: e5823000     	str	r3, [r2]
  3b3e50: e1a00003     	mov	r0, r3
  3b3e54: ebfd6b34     	bl	0x30eb2c <.plt+0xdb8>   @ imm = #-0xa5330
  3b3e58: e59f318c     	ldr	r3, [pc, #0x18c]        @ 0x3b3fec <Character::SafeGetCharPropsId()+0x2b4>
  3b3e5c: e0216fc1     	eor	r6, r1, r1, asr #31
  3b3e60: e0466fc1     	sub	r6, r6, r1, asr #31
  3b3e64: e7973003     	ldr	r3, [r7, r3]
  3b3e68: e5932000     	ldr	r2, [r3]
  3b3e6c: e2822001     	add	r2, r2, #1
  3b3e70: e5832000     	str	r2, [r3]
  3b3e74: e5953004     	ldr	r3, [r5, #0x4]
  3b3e78: e1530006     	cmp	r3, r6
  3b3e7c: ca000007     	bgt	0x3b3ea0 <Character::SafeGetCharPropsId()+0x168> @ imm = #0x1c
  3b3e80: e59f3168     	ldr	r3, [pc, #0x168]        @ 0x3b3ff0 <Character::SafeGetCharPropsId()+0x2b8>
  3b3e84: e7973003     	ldr	r3, [r7, r3]
  3b3e88: e5933000     	ldr	r3, [r3]
  3b3e8c: e3530002     	cmp	r3, #2
  3b3e90: 058aa000     	streq	r10, [r10]
  3b3e94: 0a000001     	beq	0x3b3ea0 <Character::SafeGetCharPropsId()+0x168> @ imm = #0x4
  3b3e98: e3530001     	cmp	r3, #1
  3b3e9c: 0a000042     	beq	0x3b3fac <Character::SafeGetCharPropsId()+0x274> @ imm = #0x108
  3b3ea0: e5953008     	ldr	r3, [r5, #0x8]
  3b3ea4: e0836186     	add	r6, r3, r6, lsl #3
  3b3ea8: e1d600b4     	ldrh	r0, [r6, #4]
  3b3eac: e30133c8     	movw	r3, #0x13c8
  3b3eb0: e18400b3     	strh	r0, [r4, r3]
  3b3eb4: e6bf0070     	sxth	r0, r0
  3b3eb8: eaffffa8     	b	0x3b3d60 <Character::SafeGetCharPropsId()+0x28> @ imm = #-0x160
  3b3ebc: e59f3130     	ldr	r3, [pc, #0x130]        @ 0x3b3ff4 <Character::SafeGetCharPropsId()+0x2bc>
  3b3ec0: e7973003     	ldr	r3, [r7, r3]
  3b3ec4: e5936000     	ldr	r6, [r3]
  3b3ec8: e3560000     	cmp	r6, #0
  3b3ecc: 0a000033     	beq	0x3b3fa0 <Character::SafeGetCharPropsId()+0x268> @ imm = #0xcc
  3b3ed0: e59f3120     	ldr	r3, [pc, #0x120]        @ 0x3b3ff8 <Character::SafeGetCharPropsId()+0x2c0>
  3b3ed4: e59f8120     	ldr	r8, [pc, #0x120]        @ 0x3b3ffc <Character::SafeGetCharPropsId()+0x2c4>
  3b3ed8: e3a05000     	mov	r5, #0
  3b3edc: e7973003     	ldr	r3, [r7, r3]
  3b3ee0: e08f8008     	add	r8, pc, r8
  3b3ee4: e5937000     	ldr	r7, [r3]
  3b3ee8: ea000002     	b	0x3b3ef8 <Character::SafeGetCharPropsId()+0x1c0> @ imm = #0x8
  3b3eec: e2855001     	add	r5, r5, #1
  3b3ef0: e1550006     	cmp	r5, r6
  3b3ef4: 0a000029     	beq	0x3b3fa0 <Character::SafeGetCharPropsId()+0x268> @ imm = #0xa4
  3b3ef8: e7971105     	ldr	r1, [r7, r5, lsl #2]
  3b3efc: e1a00008     	mov	r0, r8
  3b3f00: ebfd6905     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xa5bec
  3b3f04: e3500000     	cmp	r0, #0
  3b3f08: 1afffff7     	bne	0x3b3eec <Character::SafeGetCharPropsId()+0x1b4> @ imm = #-0x24
  3b3f0c: e6ff5075     	uxth	r5, r5
  3b3f10: e6bf1075     	sxth	r1, r5
  3b3f14: e30133c8     	movw	r3, #0x13c8
  3b3f18: e18450b3     	strh	r5, [r4, r3]
  3b3f1c: eaffffb4     	b	0x3b3df4 <Character::SafeGetCharPropsId()+0xbc> @ imm = #-0x130
  3b3f20: e30133c4     	movw	r3, #0x13c4
  3b3f24: e7948003     	ldr	r8, [r4, r3]
  3b3f28: e3a03d4f     	mov	r3, #5056
  3b3f2c: e7943003     	ldr	r3, [r4, r3]
  3b3f30: e1530008     	cmp	r3, r8
  3b3f34: 0affffa2     	beq	0x3b3dc4 <Character::SafeGetCharPropsId()+0x8c> @ imm = #-0x178
  3b3f38: e59f30b4     	ldr	r3, [pc, #0xb4]         @ 0x3b3ff4 <Character::SafeGetCharPropsId()+0x2bc>
  3b3f3c: e7973003     	ldr	r3, [r7, r3]
  3b3f40: e5936000     	ldr	r6, [r3]
  3b3f44: e3560000     	cmp	r6, #0
  3b3f48: 0a000011     	beq	0x3b3f94 <Character::SafeGetCharPropsId()+0x25c> @ imm = #0x44
  3b3f4c: e59f30a4     	ldr	r3, [pc, #0xa4]         @ 0x3b3ff8 <Character::SafeGetCharPropsId()+0x2c0>
  3b3f50: e1a0500a     	mov	r5, r10
  3b3f54: e7973003     	ldr	r3, [r7, r3]
  3b3f58: e5937000     	ldr	r7, [r3]
  3b3f5c: ea000002     	b	0x3b3f6c <Character::SafeGetCharPropsId()+0x234> @ imm = #0x8
  3b3f60: e2855001     	add	r5, r5, #1
  3b3f64: e1550006     	cmp	r5, r6
  3b3f68: 0a000009     	beq	0x3b3f94 <Character::SafeGetCharPropsId()+0x25c> @ imm = #0x24
  3b3f6c: e7971105     	ldr	r1, [r7, r5, lsl #2]
  3b3f70: e1a00008     	mov	r0, r8
  3b3f74: ebfd68e8     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xa5c60
  3b3f78: e3500000     	cmp	r0, #0
  3b3f7c: 1afffff7     	bne	0x3b3f60 <Character::SafeGetCharPropsId()+0x228> @ imm = #-0x24
  3b3f80: e6ff5075     	uxth	r5, r5
  3b3f84: e6bf0075     	sxth	r0, r5
  3b3f88: e30133c8     	movw	r3, #0x13c8
  3b3f8c: e18450b3     	strh	r5, [r4, r3]
  3b3f90: eaffff72     	b	0x3b3d60 <Character::SafeGetCharPropsId()+0x28> @ imm = #-0x238
  3b3f94: e3e00000     	mvn	r0, #0
  3b3f98: e30f5fff     	movw	r5, #0xffff
  3b3f9c: eafffff9     	b	0x3b3f88 <Character::SafeGetCharPropsId()+0x250> @ imm = #-0x1c
  3b3fa0: e3e01000     	mvn	r1, #0
  3b3fa4: e30f5fff     	movw	r5, #0xffff
  3b3fa8: eaffffd9     	b	0x3b3f14 <Character::SafeGetCharPropsId()+0x1dc> @ imm = #-0x9c
  3b3fac: e59f004c     	ldr	r0, [pc, #0x4c]         @ 0x3b4000 <Character::SafeGetCharPropsId()+0x2c8>
  3b3fb0: e59f104c     	ldr	r1, [pc, #0x4c]         @ 0x3b4004 <Character::SafeGetCharPropsId()+0x2cc>
  3b3fb4: e59f204c     	ldr	r2, [pc, #0x4c]         @ 0x3b4008 <Character::SafeGetCharPropsId()+0x2d0>
  3b3fb8: e7970000     	ldr	r0, [r7, r0]
  3b3fbc: e59f3048     	ldr	r3, [pc, #0x48]         @ 0x3b400c <Character::SafeGetCharPropsId()+0x2d4>
  3b3fc0: e3a0cfbd     	mov	r12, #756
  3b3fc4: e08f1001     	add	r1, pc, r1
  3b3fc8: e08f2002     	add	r2, pc, r2
  3b3fcc: e08f3003     	add	r3, pc, r3
  3b3fd0: e28000a8     	add	r0, r0, #168
  3b3fd4: e58dc000     	str	r12, [sp]
  3b3fd8: ebfd6809     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xa5fdc
  3b3fdc: eaffffaf     	b	0x3b3ea0 <Character::SafeGetCharPropsId()+0x168> @ imm = #-0x144
  3b3fe0: 40 0d 5e 00  	.word	0x005e0d40
  3b3fe4: b8 47 00 00  	.word	0x000047b8
  3b3fe8: 94 0c 00 00  	.word	0x00000c94
  3b3fec: 88 10 00 00  	.word	0x00001088
  3b3ff0: c0 39 00 00  	.word	0x000039c0
  3b3ff4: 04 42 00 00  	.word	0x00004204
  3b3ff8: 08 3c 00 00  	.word	0x00003c08
  3b3ffc: 80 fe 50 00  	.word	0x0050fe80
  3b4000: c0 19 00 00  	.word	0x000019c0
  3b4004: 14 a4 50 00  	.word	0x0050a414
  3b4008: b0 fd 50 00  	.word	0x0050fdb0
  3b400c: e4 fd 50 00  	.word	0x0050fde4
