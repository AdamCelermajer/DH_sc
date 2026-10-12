
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00487bb0 <_ZN7Array2dIPN3rnd4TileEE5ClearEv>:
  487bb0: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  487bb4: e5903018     	ldr	r3, [r0, #0x18]
  487bb8: e24dd0f4     	sub	sp, sp, #244
  487bbc: e590a014     	ldr	r10, [r0, #0x14]
  487bc0: e590400c     	ldr	r4, [r0, #0xc]
  487bc4: e28dc05c     	add	r12, sp, #92
  487bc8: e58d301c     	str	r3, [sp, #0x1c]
  487bcc: e28d30dc     	add	r3, sp, #220
  487bd0: e1a07000     	mov	r7, r0
  487bd4: e28d80ac     	add	r8, sp, #172
  487bd8: e28d90bc     	add	r9, sp, #188
  487bdc: e28db0ec     	add	r11, sp, #236
  487be0: e58dc00c     	str	r12, [sp, #0xc]
  487be4: e58d3014     	str	r3, [sp, #0x14]
  487be8: e28dc0cc     	add	r12, sp, #204
  487bec: e28d3020     	add	r3, sp, #32
  487bf0: e3a06000     	mov	r6, #0
  487bf4: e58dc010     	str	r12, [sp, #0x10]
  487bf8: e58d3018     	str	r3, [sp, #0x18]
  487bfc: e58da004     	str	r10, [sp, #0x4]
  487c00: ea00000b     	b	0x487c34 <_ZN7Array2dIPN3rnd4TileEE5ClearEv+0x84> @ imm = #0x2c
  487c04: e2845010     	add	r5, r4, #16
  487c08: e8955020     	ldm	r5, {r5, r12, lr}
  487c0c: e594a01c     	ldr	r10, [r4, #0x1c]
  487c10: e58dc0c0     	str	r12, [sp, #0xc0]
  487c14: e58de0c4     	str	lr, [sp, #0xc4]
  487c18: e58da0c8     	str	r10, [sp, #0xc8]
  487c1c: e58d50bc     	str	r5, [sp, #0xbc]
  487c20: ebffffae     	bl	0x487ae0 <_ZNSt5dequeIPN3rnd4TileESaIS2_EE14_M_fill_insertENSt4priv15_Deque_iteratorIS2_St16_Nonconst_traitsIS2_EEEjRKS2_> @ imm = #-0x148
  487c24: e59dc004     	ldr	r12, [sp, #0x4]
  487c28: e2844028     	add	r4, r4, #40
  487c2c: e15c0004     	cmp	r12, r4
  487c30: 0a000024     	beq	0x487cc8 <_ZN7Array2dIPN3rnd4TileEE5ClearEv+0x118> @ imm = #0x90
  487c34: e597301c     	ldr	r3, [r7, #0x1c]
  487c38: e2845010     	add	r5, r4, #16
  487c3c: e1530004     	cmp	r3, r4
  487c40: 0a000025     	beq	0x487cdc <_ZN7Array2dIPN3rnd4TileEE5ClearEv+0x12c> @ imm = #0x94
  487c44: e58d60ec     	str	r6, [sp, #0xec]
  487c48: e894000f     	ldm	r4, {r0, r1, r2, r3}
  487c4c: e888000f     	stm	r8, {r0, r1, r2, r3}
  487c50: e1a01008     	mov	r1, r8
  487c54: e1a00005     	mov	r0, r5
  487c58: ebfff034     	bl	0x483d30 <_ZNKSt4priv20_Deque_iterator_baseIPN3rnd4TileEE11_M_subtractERKS4_> @ imm = #-0x3f30
  487c5c: e3500000     	cmp	r0, #0
  487c60: e1a01009     	mov	r1, r9
  487c64: e1a00004     	mov	r0, r4
  487c68: e1a02006     	mov	r2, r6
  487c6c: e1a0300b     	mov	r3, r11
  487c70: 0affffe3     	beq	0x487c04 <_ZN7Array2dIPN3rnd4TileEE5ClearEv+0x54> @ imm = #-0x74
  487c74: e59da00c     	ldr	r10, [sp, #0xc]
  487c78: e894000f     	ldm	r4, {r0, r1, r2, r3}
  487c7c: e88a000f     	stm	r10, {r0, r1, r2, r3}
  487c80: e1a0000a     	mov	r0, r10
  487c84: e1a01006     	mov	r1, r6
  487c88: ebfff039     	bl	0x483d74 <_ZNSt4priv20_Deque_iterator_baseIPN3rnd4TileEE10_M_advanceEi> @ imm = #-0x3f1c
  487c8c: e89a000f     	ldm	r10, {r0, r1, r2, r3}
  487c90: e59dc014     	ldr	r12, [sp, #0x14]
  487c94: e88c000f     	stm	r12, {r0, r1, r2, r3}
  487c98: e59da010     	ldr	r10, [sp, #0x10]
  487c9c: e895000f     	ldm	r5, {r0, r1, r2, r3}
  487ca0: e88a000f     	stm	r10, {r0, r1, r2, r3}
  487ca4: e1a01004     	mov	r1, r4
  487ca8: e59d0018     	ldr	r0, [sp, #0x18]
  487cac: e59d2014     	ldr	r2, [sp, #0x14]
  487cb0: e59d3010     	ldr	r3, [sp, #0x10]
  487cb4: ebfff714     	bl	0x48590c <_ZNSt5dequeIPN3rnd4TileESaIS2_EE5eraseENSt4priv15_Deque_iteratorIS2_St16_Nonconst_traitsIS2_EEES9_> @ imm = #-0x23b0
  487cb8: e59dc004     	ldr	r12, [sp, #0x4]
  487cbc: e2844028     	add	r4, r4, #40
  487cc0: e15c0004     	cmp	r12, r4
  487cc4: 1affffda     	bne	0x487c34 <_ZN7Array2dIPN3rnd4TileEE5ClearEv+0x84> @ imm = #-0x98
  487cc8: e59d301c     	ldr	r3, [sp, #0x1c]
  487ccc: e5b34004     	ldr	r4, [r3, #0x4]!
  487cd0: e58d301c     	str	r3, [sp, #0x1c]
  487cd4: e284a078     	add	r10, r4, #120
  487cd8: eaffffc2     	b	0x487be8 <_ZN7Array2dIPN3rnd4TileEE5ClearEv+0x38> @ imm = #-0xf8
  487cdc: e3a04000     	mov	r4, #0
  487ce0: e28d5034     	add	r5, sp, #52
  487ce4: e1a00005     	mov	r0, r5
  487ce8: e1a01004     	mov	r1, r4
  487cec: e58d4034     	str	r4, [sp, #0x34]
  487cf0: e58d4038     	str	r4, [sp, #0x38]
  487cf4: e58d403c     	str	r4, [sp, #0x3c]
  487cf8: e58d4040     	str	r4, [sp, #0x40]
  487cfc: e58d4044     	str	r4, [sp, #0x44]
  487d00: e58d4048     	str	r4, [sp, #0x48]
  487d04: e58d404c     	str	r4, [sp, #0x4c]
  487d08: e58d4050     	str	r4, [sp, #0x50]
  487d0c: e58d4054     	str	r4, [sp, #0x54]
  487d10: e58d4058     	str	r4, [sp, #0x58]
  487d14: e287800c     	add	r8, r7, #12
  487d18: ebfffa70     	bl	0x4866e0 <_ZNSt4priv11_Deque_baseIPN3rnd4TileESaIS3_EE17_M_initialize_mapEj> @ imm = #-0x1640
  487d1c: e898000f     	ldm	r8, {r0, r1, r2, r3}
  487d20: e28dc06c     	add	r12, sp, #108
  487d24: e287a01c     	add	r10, r7, #28
  487d28: e88c000f     	stm	r12, {r0, r1, r2, r3}
  487d2c: e1a0100c     	mov	r1, r12
  487d30: e1a0000a     	mov	r0, r10
  487d34: ebffefe0     	bl	0x483cbc <_ZNKSt4priv20_Deque_iterator_baseISt5dequeIPN3rnd4TileESaIS4_EEE11_M_subtractERKS7_> @ imm = #-0x4080
  487d38: e2502000     	subs	r2, r0, #0
  487d3c: 1a000013     	bne	0x487d90 <_ZN7Array2dIPN3rnd4TileEE5ClearEv+0x1e0> @ imm = #0x4c
  487d40: e597301c     	ldr	r3, [r7, #0x1c]
  487d44: e5974028     	ldr	r4, [r7, #0x28]
  487d48: e597e024     	ldr	lr, [r7, #0x24]
  487d4c: e597c020     	ldr	r12, [r7, #0x20]
  487d50: e28d10f0     	add	r1, sp, #240
  487d54: e5213074     	str	r3, [r1, #-0x74]!
  487d58: e1a00008     	mov	r0, r8
  487d5c: e1a03005     	mov	r3, r5
  487d60: e58d4088     	str	r4, [sp, #0x88]
  487d64: e58de084     	str	lr, [sp, #0x84]
  487d68: e58dc080     	str	r12, [sp, #0x80]
  487d6c: ebfffcb0     	bl	0x487034 <_ZNSt5dequeIS_IPN3rnd4TileESaIS2_EESaIS4_EE14_M_fill_insertENSt4priv15_Deque_iteratorIS4_St16_Nonconst_traitsIS4_EEEjRKS4_> @ imm = #-0xd40
  487d70: e1a00005     	mov	r0, r5
  487d74: ebfff4fb     	bl	0x485168 <_ZNSt4priv11_Deque_baseIPN3rnd4TileESaIS3_EED2Ev> @ imm = #-0x2c14
  487d78: e3a03000     	mov	r3, #0
  487d7c: e5873008     	str	r3, [r7, #0x8]
  487d80: e5873004     	str	r3, [r7, #0x4]
  487d84: e5873000     	str	r3, [r7]
  487d88: e28dd0f4     	add	sp, sp, #244
  487d8c: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  487d90: e28d605c     	add	r6, sp, #92
  487d94: e898000f     	ldm	r8, {r0, r1, r2, r3}
  487d98: e886000f     	stm	r6, {r0, r1, r2, r3}
  487d9c: e1a01004     	mov	r1, r4
  487da0: e1a00006     	mov	r0, r6
  487da4: ebfff02a     	bl	0x483e54 <_ZNSt4priv20_Deque_iterator_baseISt5dequeIPN3rnd4TileESaIS4_EEE10_M_advanceEi> @ imm = #-0x3f58
  487da8: e896000f     	ldm	r6, {r0, r1, r2, r3}
  487dac: e28de09c     	add	lr, sp, #156
  487db0: e88e000f     	stm	lr, {r0, r1, r2, r3}
  487db4: e28dc08c     	add	r12, sp, #140
  487db8: e89a000f     	ldm	r10, {r0, r1, r2, r3}
  487dbc: e88c000f     	stm	r12, {r0, r1, r2, r3}
  487dc0: e1a01008     	mov	r1, r8
  487dc4: e1a0200e     	mov	r2, lr
  487dc8: e1a0300c     	mov	r3, r12
  487dcc: e28d0020     	add	r0, sp, #32
  487dd0: ebfff861     	bl	0x485f5c <_ZNSt5dequeIS_IPN3rnd4TileESaIS2_EESaIS4_EE5eraseENSt4priv15_Deque_iteratorIS4_St16_Nonconst_traitsIS4_EEESB_> @ imm = #-0x1e7c
  487dd4: eaffffe5     	b	0x487d70 <_ZN7Array2dIPN3rnd4TileEE5ClearEv+0x1c0> @ imm = #-0x6c
