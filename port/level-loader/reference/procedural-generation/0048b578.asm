
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048b578 <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii>:
  48b578: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  48b57c: e5907000     	ldr	r7, [r0]
  48b580: e24dd08c     	sub	sp, sp, #140
  48b584: e1a04000     	mov	r4, r0
  48b588: e1570001     	cmp	r7, r1
  48b58c: e98d0006     	stmib	sp, {r1, r2}
  48b590: da000069     	ble	0x48b73c <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x1c4> @ imm = #0x1a4
  48b594: e5903008     	ldr	r3, [r0, #0x8]
  48b598: e0617007     	rsb	r7, r1, r7
  48b59c: e590500c     	ldr	r5, [r0, #0xc]
  48b5a0: e0833007     	add	r3, r3, r7
  48b5a4: e5803008     	str	r3, [r0, #0x8]
  48b5a8: e590301c     	ldr	r3, [r0, #0x1c]
  48b5ac: e5901018     	ldr	r1, [r0, #0x18]
  48b5b0: e3a0a000     	mov	r10, #0
  48b5b4: e1530005     	cmp	r3, r5
  48b5b8: e5909014     	ldr	r9, [r0, #0x14]
  48b5bc: e28db084     	add	r11, sp, #132
  48b5c0: e58d100c     	str	r1, [sp, #0xc]
  48b5c4: e1a0800a     	mov	r8, r10
  48b5c8: 0a000017     	beq	0x48b62c <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0xb4> @ imm = #0x5c
  48b5cc: e3570000     	cmp	r7, #0
  48b5d0: c3a06000     	movgt	r6, #0
  48b5d4: da00000c     	ble	0x48b60c <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x94> @ imm = #0x30
  48b5d8: e58d8084     	str	r8, [sp, #0x84]
  48b5dc: e5953000     	ldr	r3, [r5]
  48b5e0: e5952004     	ldr	r2, [r5, #0x4]
  48b5e4: e1530002     	cmp	r3, r2
  48b5e8: 0a00002a     	beq	0x48b698 <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x120> @ imm = #0xa8
  48b5ec: e503a004     	str	r10, [r3, #-0x4]
  48b5f0: e5953000     	ldr	r3, [r5]
  48b5f4: e2433004     	sub	r3, r3, #4
  48b5f8: e5853000     	str	r3, [r5]
  48b5fc: e2866001     	add	r6, r6, #1
  48b600: e1560007     	cmp	r6, r7
  48b604: 1afffff3     	bne	0x48b5d8 <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x60> @ imm = #-0x34
  48b608: e594301c     	ldr	r3, [r4, #0x1c]
  48b60c: e2855028     	add	r5, r5, #40
  48b610: e1590005     	cmp	r9, r5
  48b614: 059d200c     	ldreq	r2, [sp, #0xc]
  48b618: 05b25004     	ldreq	r5, [r2, #0x4]!
  48b61c: 058d200c     	streq	r2, [sp, #0xc]
  48b620: 02859078     	addeq	r9, r5, #120
  48b624: e1530005     	cmp	r3, r5
  48b628: 1affffe7     	bne	0x48b5cc <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x54> @ imm = #-0x64
  48b62c: e59d3004     	ldr	r3, [sp, #0x4]
  48b630: e5843000     	str	r3, [r4]
  48b634: e5947004     	ldr	r7, [r4, #0x4]
  48b638: e59d2008     	ldr	r2, [sp, #0x8]
  48b63c: e1570002     	cmp	r7, r2
  48b640: da000018     	ble	0x48b6a8 <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x130> @ imm = #0x60
  48b644: e0627007     	rsb	r7, r2, r7
  48b648: e3570000     	cmp	r7, #0
  48b64c: da00000d     	ble	0x48b688 <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x110> @ imm = #0x34
  48b650: e284800c     	add	r8, r4, #12
  48b654: e3a06000     	mov	r6, #0
  48b658: e28d5038     	add	r5, sp, #56
  48b65c: e5941008     	ldr	r1, [r4, #0x8]
  48b660: e1a00005     	mov	r0, r5
  48b664: ebfffef6     	bl	0x48b244 <_ZNSt5dequeIPN3rnd4TileESaIS2_EEC1Ej> @ imm = #-0x428
  48b668: e1a00008     	mov	r0, r8
  48b66c: e1a01005     	mov	r1, r5
  48b670: ebffffb2     	bl	0x48b540 <_ZNSt5dequeIS_IPN3rnd4TileESaIS2_EESaIS4_EE10push_frontERKS4_> @ imm = #-0x138
  48b674: e2866001     	add	r6, r6, #1
  48b678: e1a00005     	mov	r0, r5
  48b67c: ebffe6b9     	bl	0x485168 <_ZNSt4priv11_Deque_baseIPN3rnd4TileESaIS3_EED2Ev> @ imm = #-0x651c
  48b680: e1560007     	cmp	r6, r7
  48b684: 1afffff4     	bne	0x48b65c <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0xe4> @ imm = #-0x30
  48b688: e59d3008     	ldr	r3, [sp, #0x8]
  48b68c: e5843004     	str	r3, [r4, #0x4]
  48b690: e28dd08c     	add	sp, sp, #140
  48b694: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  48b698: e1a00005     	mov	r0, r5
  48b69c: e1a0100b     	mov	r1, r11
  48b6a0: ebfffecd     	bl	0x48b1dc <_ZNSt5dequeIPN3rnd4TileESaIS2_EE19_M_push_front_aux_vERKS2_> @ imm = #-0x4cc
  48b6a4: eaffffd4     	b	0x48b5fc <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x84> @ imm = #-0xb0
  48b6a8: e28dc070     	add	r12, sp, #112
  48b6ac: e284500c     	add	r5, r4, #12
  48b6b0: e284601c     	add	r6, r4, #28
  48b6b4: e895000f     	ldm	r5, {r0, r1, r2, r3}
  48b6b8: e88c000f     	stm	r12, {r0, r1, r2, r3}
  48b6bc: e1a0100c     	mov	r1, r12
  48b6c0: e1a00006     	mov	r0, r6
  48b6c4: ebffe17c     	bl	0x483cbc <_ZNKSt4priv20_Deque_iterator_baseISt5dequeIPN3rnd4TileESaIS4_EEE11_M_subtractERKS7_> @ imm = #-0x7a10
  48b6c8: e594c004     	ldr	r12, [r4, #0x4]
  48b6cc: e59d1008     	ldr	r1, [sp, #0x8]
  48b6d0: e080000c     	add	r0, r0, r12
  48b6d4: e1510000     	cmp	r1, r0
  48b6d8: baffffec     	blt	0x48b690 <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x118> @ imm = #-0x50
  48b6dc: e28de060     	add	lr, sp, #96
  48b6e0: e895000f     	ldm	r5, {r0, r1, r2, r3}
  48b6e4: e88e000f     	stm	lr, {r0, r1, r2, r3}
  48b6e8: e59d2008     	ldr	r2, [sp, #0x8]
  48b6ec: e1a00006     	mov	r0, r6
  48b6f0: e1a0100e     	mov	r1, lr
  48b6f4: e06c8002     	rsb	r8, r12, r2
  48b6f8: ebffe16f     	bl	0x483cbc <_ZNKSt4priv20_Deque_iterator_baseISt5dequeIPN3rnd4TileESaIS4_EEE11_M_subtractERKS7_> @ imm = #-0x7a44
  48b6fc: e0588000     	subs	r8, r8, r0
  48b700: 4affffe2     	bmi	0x48b690 <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x118> @ imm = #-0x78
  48b704: e3a07000     	mov	r7, #0
  48b708: e28d6010     	add	r6, sp, #16
  48b70c: e5941008     	ldr	r1, [r4, #0x8]
  48b710: e1a00006     	mov	r0, r6
  48b714: ebfffeca     	bl	0x48b244 <_ZNSt5dequeIPN3rnd4TileESaIS2_EEC1Ej> @ imm = #-0x4d8
  48b718: e1a00005     	mov	r0, r5
  48b71c: e1a01006     	mov	r1, r6
  48b720: ebffff5e     	bl	0x48b4a0 <_ZNSt5dequeIS_IPN3rnd4TileESaIS2_EESaIS4_EE9push_backERKS4_> @ imm = #-0x288
  48b724: e2877001     	add	r7, r7, #1
  48b728: e1a00006     	mov	r0, r6
  48b72c: ebffe68d     	bl	0x485168 <_ZNSt4priv11_Deque_baseIPN3rnd4TileESaIS3_EED2Ev> @ imm = #-0x65cc
  48b730: e1580007     	cmp	r8, r7
  48b734: aafffff4     	bge	0x48b70c <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x194> @ imm = #-0x30
  48b738: eaffffd4     	b	0x48b690 <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x118> @ imm = #-0xb0
  48b73c: e5903008     	ldr	r3, [r0, #0x8]
  48b740: e59d1004     	ldr	r1, [sp, #0x4]
  48b744: e0832007     	add	r2, r3, r7
  48b748: e1510002     	cmp	r1, r2
  48b74c: baffffb8     	blt	0x48b634 <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0xbc> @ imm = #-0x120
  48b750: e2677001     	rsb	r7, r7, #1
  48b754: e0637007     	rsb	r7, r3, r7
  48b758: e5902018     	ldr	r2, [r0, #0x18]
  48b75c: e0877001     	add	r7, r7, r1
  48b760: e0873003     	add	r3, r7, r3
  48b764: e3a0a000     	mov	r10, #0
  48b768: e5803008     	str	r3, [r0, #0x8]
  48b76c: e5909014     	ldr	r9, [r0, #0x14]
  48b770: e590500c     	ldr	r5, [r0, #0xc]
  48b774: e590301c     	ldr	r3, [r0, #0x1c]
  48b778: e28db080     	add	r11, sp, #128
  48b77c: e58d2004     	str	r2, [sp, #0x4]
  48b780: e1a0800a     	mov	r8, r10
  48b784: e1530005     	cmp	r3, r5
  48b788: 0affffa9     	beq	0x48b634 <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0xbc> @ imm = #-0x15c
  48b78c: e3570000     	cmp	r7, #0
  48b790: c3a06000     	movgt	r6, #0
  48b794: da00000d     	ble	0x48b7d0 <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x258> @ imm = #0x34
  48b798: e58d8080     	str	r8, [sp, #0x80]
  48b79c: e5952018     	ldr	r2, [r5, #0x18]
  48b7a0: e5953010     	ldr	r3, [r5, #0x10]
  48b7a4: e2422004     	sub	r2, r2, #4
  48b7a8: e1530002     	cmp	r3, r2
  48b7ac: 0a00000e     	beq	0x48b7ec <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x274> @ imm = #0x38
  48b7b0: e583a000     	str	r10, [r3]
  48b7b4: e5953010     	ldr	r3, [r5, #0x10]
  48b7b8: e2833004     	add	r3, r3, #4
  48b7bc: e5853010     	str	r3, [r5, #0x10]
  48b7c0: e2866001     	add	r6, r6, #1
  48b7c4: e1560007     	cmp	r6, r7
  48b7c8: 1afffff2     	bne	0x48b798 <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x220> @ imm = #-0x38
  48b7cc: e594301c     	ldr	r3, [r4, #0x1c]
  48b7d0: e2855028     	add	r5, r5, #40
  48b7d4: e1590005     	cmp	r9, r5
  48b7d8: 059d1004     	ldreq	r1, [sp, #0x4]
  48b7dc: 05b15004     	ldreq	r5, [r1, #0x4]!
  48b7e0: 058d1004     	streq	r1, [sp, #0x4]
  48b7e4: 02859078     	addeq	r9, r5, #120
  48b7e8: eaffffe5     	b	0x48b784 <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x20c> @ imm = #-0x6c
  48b7ec: e1a00005     	mov	r0, r5
  48b7f0: e1a0100b     	mov	r1, r11
  48b7f4: ebfffe5b     	bl	0x48b168 <_ZNSt5dequeIPN3rnd4TileESaIS2_EE18_M_push_back_aux_vERKS2_> @ imm = #-0x694
  48b7f8: eafffff0     	b	0x48b7c0 <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii+0x248> @ imm = #-0x40
