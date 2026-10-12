
C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039f910 <_ZN9Container8InitPostEv>:
  39f910: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  39f914: e24dd008     	sub	sp, sp, #8
  39f918: e1a04000     	mov	r4, r0
  39f91c: ebffb110     	bl	0x38bd64 <_ZN10GameObject21CheckSpawnProbabilityEv> @ imm = #-0x13bc0
  39f920: e5943274     	ldr	r3, [r4, #0x274]
  39f924: e59f5200     	ldr	r5, [pc, #0x200]        @ 0x39fb2c <_ZN9Container8InitPostEv+0x21c>
  39f928: e1500003     	cmp	r0, r3
  39f92c: e08f5005     	add	r5, pc, r5
  39f930: ba000001     	blt	0x39f93c <_ZN9Container8InitPostEv+0x2c> @ imm = #0x4
  39f934: e28dd008     	add	sp, sp, #8
  39f938: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  39f93c: e5943000     	ldr	r3, [r4]
  39f940: e1a00004     	mov	r0, r4
  39f944: e1a0e00f     	mov	lr, pc
  39f948: e593f0d0     	ldr	pc, [r3, #0xd0]
  39f94c: e5943000     	ldr	r3, [r4]
  39f950: e5840374     	str	r0, [r4, #0x374]
  39f954: e1a00004     	mov	r0, r4
  39f958: e1a0e00f     	mov	lr, pc
  39f95c: e593f0cc     	ldr	pc, [r3, #0xcc]
  39f960: e3700001     	cmn	r0, #1
  39f964: 0a00000e     	beq	0x39f9a4 <_ZN9Container8InitPostEv+0x94> @ imm = #0x38
  39f968: e5943374     	ldr	r3, [r4, #0x374]
  39f96c: e3730001     	cmn	r3, #1
  39f970: 0a00000b     	beq	0x39f9a4 <_ZN9Container8InitPostEv+0x94> @ imm = #0x2c
  39f974: e59f31b4     	ldr	r3, [pc, #0x1b4]        @ 0x39fb30 <_ZN9Container8InitPostEv+0x220>
  39f978: e3a0200c     	mov	r2, #12
  39f97c: e7953003     	ldr	r3, [r5, r3]
  39f980: e5933000     	ldr	r3, [r3]
  39f984: e0203092     	mla	r0, r2, r0, r3
  39f988: e5906008     	ldr	r6, [r0, #0x8]
  39f98c: e1a00006     	mov	r0, r6
  39f990: ebfdb92f     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x91b44
  39f994: e1a01006     	mov	r1, r6
  39f998: e0862000     	add	r2, r6, r0
  39f99c: e2840e29     	add	r0, r4, #656
  39f9a0: ebfdc40e     	bl	0x3109e0 <_ZNSs9_M_assignEPKcS0_> @ imm = #-0x8efc8
  39f9a4: e1a00004     	mov	r0, r4
  39f9a8: ebffb12b     	bl	0x38be5c <_ZN10GameObject8InitPostEv> @ imm = #-0x13b54
  39f9ac: e1a00004     	mov	r0, r4
  39f9b0: ebffac6a     	bl	0x38ab60 <_ZNK10GameObject13MeetConditionEv> @ imm = #-0x14e58
  39f9b4: e2501000     	subs	r1, r0, #0
  39f9b8: 0a00004e     	beq	0x39faf8 <_ZN9Container8InitPostEv+0x1e8> @ imm = #0x138
  39f9bc: e59482d8     	ldr	r8, [r4, #0x2d8]
  39f9c0: e3580000     	cmp	r8, #0
  39f9c4: 0a00001b     	beq	0x39fa38 <_ZN9Container8InitPostEv+0x128> @ imm = #0x6c
  39f9c8: e59f3164     	ldr	r3, [pc, #0x164]        @ 0x39fb34 <_ZN9Container8InitPostEv+0x224>
  39f9cc: e5986038     	ldr	r6, [r8, #0x38]
  39f9d0: e1a02004     	mov	r2, r4
  39f9d4: e7951003     	ldr	r1, [r5, r3]
  39f9d8: e59f3158     	ldr	r3, [pc, #0x158]        @ 0x39fb38 <_ZN9Container8InitPostEv+0x228>
  39f9dc: e596c000     	ldr	r12, [r6]
  39f9e0: e1a00006     	mov	r0, r6
  39f9e4: e7953003     	ldr	r3, [r5, r3]
  39f9e8: e58d4000     	str	r4, [sp]
  39f9ec: e1a0e00f     	mov	lr, pc
  39f9f0: e59cf02c     	ldr	pc, [r12, #0x2c]
  39f9f4: e59f1140     	ldr	r1, [pc, #0x140]        @ 0x39fb3c <_ZN9Container8InitPostEv+0x22c>
  39f9f8: e3a07000     	mov	r7, #0
  39f9fc: e596c000     	ldr	r12, [r6]
  39fa00: e08f1001     	add	r1, pc, r1
  39fa04: e58d7000     	str	r7, [sp]
  39fa08: e1a00006     	mov	r0, r6
  39fa0c: e1a02007     	mov	r2, r7
  39fa10: e1a03007     	mov	r3, r7
  39fa14: e1a0e00f     	mov	lr, pc
  39fa18: e59cf020     	ldr	pc, [r12, #0x20]
  39fa1c: e3500000     	cmp	r0, #0
  39fa20: 0a00001b     	beq	0x39fa94 <_ZN9Container8InitPostEv+0x184> @ imm = #0x6c
  39fa24: e1a01007     	mov	r1, r7
  39fa28: e1a00004     	mov	r0, r4
  39fa2c: ebfffe66     	bl	0x39f3cc <_ZN9Container8SetStateENS_5StateE> @ imm = #-0x668
  39fa30: e1a00008     	mov	r0, r8
  39fa34: eb034406     	bl	0x470a54 <_ZN12VisualObject12ApplyMeshBoxEv> @ imm = #0xd1018
  39fa38: e59f3100     	ldr	r3, [pc, #0x100]        @ 0x39fb40 <_ZN9Container8InitPostEv+0x230>
  39fa3c: e7953003     	ldr	r3, [r5, r3]
  39fa40: e5935000     	ldr	r5, [r3]
  39fa44: e3550000     	cmp	r5, #0
  39fa48: 0a000006     	beq	0x39fa68 <_ZN9Container8InitPostEv+0x158> @ imm = #0x18
  39fa4c: e5943000     	ldr	r3, [r4]
  39fa50: e1a00004     	mov	r0, r4
  39fa54: e1a0e00f     	mov	lr, pc
  39fa58: e593f0d8     	ldr	pc, [r3, #0xd8]
  39fa5c: e1a01000     	mov	r1, r0
  39fa60: e1a00005     	mov	r0, r5
  39fa64: ebff27e4     	bl	0x3699fc <_ZN15VoxSoundManager9LoadSoundEi> @ imm = #-0x36070
  39fa68: e5943000     	ldr	r3, [r4]
  39fa6c: e1a00004     	mov	r0, r4
  39fa70: e1a0e00f     	mov	lr, pc
  39fa74: e593f0c8     	ldr	pc, [r3, #0xc8]
  39fa78: e59f20c4     	ldr	r2, [pc, #0xc4]         @ 0x39fb44 <_ZN9Container8InitPostEv+0x234>
  39fa7c: e1a01000     	mov	r1, r0
  39fa80: e1a00004     	mov	r0, r4
  39fa84: e08f2002     	add	r2, pc, r2
  39fa88: e28dd008     	add	sp, sp, #8
  39fa8c: e8bd41f0     	pop	{r4, r5, r6, r7, r8, lr}
  39fa90: eaffbd32     	b	0x38ef60 <_ZN10GameObject18LoadExternalScriptEPKcS1_> @ imm = #-0x10b38
  39fa94: e59f10ac     	ldr	r1, [pc, #0xac]         @ 0x39fb48 <_ZN9Container8InitPostEv+0x238>
  39fa98: e1a02000     	mov	r2, r0
  39fa9c: e596c000     	ldr	r12, [r6]
  39faa0: e1a03002     	mov	r3, r2
  39faa4: e58d0000     	str	r0, [sp]
  39faa8: e08f1001     	add	r1, pc, r1
  39faac: e1a00006     	mov	r0, r6
  39fab0: e1a0e00f     	mov	lr, pc
  39fab4: e59cf020     	ldr	pc, [r12, #0x20]
  39fab8: e2503000     	subs	r3, r0, #0
  39fabc: 1a000016     	bne	0x39fb1c <_ZN9Container8InitPostEv+0x20c> @ imm = #0x58
  39fac0: e59f1084     	ldr	r1, [pc, #0x84]         @ 0x39fb4c <_ZN9Container8InitPostEv+0x23c>
  39fac4: e596c000     	ldr	r12, [r6]
  39fac8: e1a02003     	mov	r2, r3
  39facc: e1a00006     	mov	r0, r6
  39fad0: e08f1001     	add	r1, pc, r1
  39fad4: e58d3000     	str	r3, [sp]
  39fad8: e1a0e00f     	mov	lr, pc
  39fadc: e59cf020     	ldr	pc, [r12, #0x20]
  39fae0: e3500000     	cmp	r0, #0
  39fae4: 0affffd1     	beq	0x39fa30 <_ZN9Container8InitPostEv+0x120> @ imm = #-0xbc
  39fae8: e1a00004     	mov	r0, r4
  39faec: e3a01002     	mov	r1, #2
  39faf0: ebfffe35     	bl	0x39f3cc <_ZN9Container8SetStateENS_5StateE> @ imm = #-0x72c
  39faf4: eaffffcd     	b	0x39fa30 <_ZN9Container8InitPostEv+0x120> @ imm = #-0xcc
  39faf8: e1a00004     	mov	r0, r4
  39fafc: e5943000     	ldr	r3, [r4]
  39fb00: e1a0e00f     	mov	lr, pc
  39fb04: e593f040     	ldr	pc, [r3, #0x40]
  39fb08: e1a00004     	mov	r0, r4
  39fb0c: e3a01004     	mov	r1, #4
  39fb10: e28dd008     	add	sp, sp, #8
  39fb14: e8bd41f0     	pop	{r4, r5, r6, r7, r8, lr}
  39fb18: eafffe2b     	b	0x39f3cc <_ZN9Container8SetStateENS_5StateE> @ imm = #-0x754
  39fb1c: e1a00004     	mov	r0, r4
  39fb20: e3a01001     	mov	r1, #1
  39fb24: ebfffe28     	bl	0x39f3cc <_ZN9Container8SetStateENS_5StateE> @ imm = #-0x760
  39fb28: eaffffc0     	b	0x39fa30 <_ZN9Container8InitPostEv+0x120> @ imm = #-0x100
  39fb2c: 64 51 5f 00  	.word	0x005f5164
  39fb30: a8 1c 00 00  	.word	0x00001ca8
  39fb34: 00 4c 00 00  	.word	0x00004c00
  39fb38: 6c 1f 00 00  	.word	0x00001f6c
  39fb3c: 50 35 52 00  	.word	0x00523550
  39fb40: a4 0d 00 00  	.word	0x00000da4
  39fb44: fc 30 52 00  	.word	0x005230fc
  39fb48: 00 34 52 00  	.word	0x00523400
  39fb4c: e0 27 52 00  	.word	0x005227e0

0039fb50 <_GLOBAL__I_.._.._sources_Game_Objects_Containers_Container.cpp>:
  39fb50: e92d4070     	push	{r4, r5, r6, lr}
  39fb54: e59f40ac     	ldr	r4, [pc, #0xac]         @ 0x39fc08 <_GLOBAL__I_.._.._sources_Game_Objects_Containers_Container.cpp+0xb8>
  39fb58: e59f20ac     	ldr	r2, [pc, #0xac]         @ 0x39fc0c <_GLOBAL__I_.._.._sources_Game_Objects_Containers_Container.cpp+0xbc>
  39fb5c: e59f30ac     	ldr	r3, [pc, #0xac]         @ 0x39fc10 <_GLOBAL__I_.._.._sources_Game_Objects_Containers_Container.cpp+0xc0>
  39fb60: e08f4004     	add	r4, pc, r4
  39fb64: e7941002     	ldr	r1, [r4, r2]
  39fb68: e08f3003     	add	r3, pc, r3
  39fb6c: e3a0243f     	mov	r2, #1056964608
  39fb70: e5910000     	ldr	r0, [r1]
  39fb74: e5832008     	str	r2, [r3, #0x8]
  39fb78: e5832000     	str	r2, [r3]
