
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00490304 <_ZN3rnd4Rule4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_>:
  490304: e59fc208     	ldr	r12, [pc, #0x208]       @ 0x490514 <_ZN3rnd4Rule4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x210>
  490308: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  49030c: e59fe204     	ldr	lr, [pc, #0x204]        @ 0x490518 <_ZN3rnd4Rule4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x214>
  490310: e08fc00c     	add	r12, pc, r12
  490314: e1a04002     	mov	r4, r2
  490318: e79c200e     	ldr	r2, [r12, lr]
  49031c: e24ddf53     	sub	sp, sp, #332
  490320: e58d0010     	str	r0, [sp, #0x10]
  490324: e5922000     	ldr	r2, [r2]
  490328: e58de018     	str	lr, [sp, #0x18]
  49032c: e58dc00c     	str	r12, [sp, #0xc]
  490330: e58d1014     	str	r1, [sp, #0x14]
  490334: e28d5020     	add	r5, sp, #32
  490338: e5941028     	ldr	r1, [r4, #0x28]
  49033c: e3a08000     	mov	r8, #0
  490340: e1a00005     	mov	r0, r5
  490344: e58d301c     	str	r3, [sp, #0x1c]
  490348: e58d2144     	str	r2, [sp, #0x144]
  49034c: e58d8020     	str	r8, [sp, #0x20]
  490350: e58d8024     	str	r8, [sp, #0x24]
  490354: e58d8028     	str	r8, [sp, #0x28]
  490358: ebfff7f4     	bl	0x48e330 <_ZNSt6vectorISt4pairIPKN3rnd4ExitENS1_8ListElemEESaIS6_EE7reserveEj> @ imm = #-0x2030
  49035c: e5943028     	ldr	r3, [r4, #0x28]
  490360: e1530008     	cmp	r3, r8
  490364: da000016     	ble	0x4903c4 <_ZN3rnd4Rule4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0xc0> @ imm = #0x58
  490368: e28db030     	add	r11, sp, #48
  49036c: e28d70d4     	add	r7, sp, #212
  490370: e1a06004     	mov	r6, r4
  490374: e28ba004     	add	r10, r11, #4
  490378: e596902c     	ldr	r9, [r6, #0x2c]
  49037c: e1a00007     	mov	r0, r7
  490380: ebfff73e     	bl	0x48e080 <_ZN3rnd8ListElemC1Ev> @ imm = #-0x2308
  490384: e1a01007     	mov	r1, r7
  490388: e1a0000a     	mov	r0, r10
  49038c: e58d9030     	str	r9, [sp, #0x30]
  490390: ebfff7ba     	bl	0x48e280 <_ZN3rnd8ListElemC1ERKS0_> @ imm = #-0x2118
  490394: e1a0100b     	mov	r1, r11
  490398: e1a00005     	mov	r0, r5
  49039c: ebfffae1     	bl	0x48ef28 <_ZNSt6vectorISt4pairIPKN3rnd4ExitENS1_8ListElemEESaIS6_EE9push_backERKS6_> @ imm = #-0x147c
  4903a0: e1a0000a     	mov	r0, r10
  4903a4: ebffdf89     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x81dc
  4903a8: e1a00007     	mov	r0, r7
  4903ac: ebffdf87     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x81e4
  4903b0: e5943028     	ldr	r3, [r4, #0x28]
  4903b4: e2888001     	add	r8, r8, #1
  4903b8: e2866004     	add	r6, r6, #4
  4903bc: e1530008     	cmp	r3, r8
  4903c0: caffffec     	bgt	0x490378 <_ZN3rnd4Rule4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x74> @ imm = #-0x50
  4903c4: e59d1010     	ldr	r1, [sp, #0x10]
  4903c8: e28d2f52     	add	r2, sp, #328
  4903cc: e3a03000     	mov	r3, #0
  4903d0: e522311c     	str	r3, [r2, #-0x11c]!
  4903d4: e5913000     	ldr	r3, [r1]
  4903d8: e1a00001     	mov	r0, r1
  4903dc: e1a01005     	mov	r1, r5
  4903e0: e1a0e00f     	mov	lr, pc
  4903e4: e593f008     	ldr	pc, [r3, #0x8]
  4903e8: e59d2024     	ldr	r2, [sp, #0x24]
  4903ec: e59d6020     	ldr	r6, [sp, #0x20]
  4903f0: e30c3f3d     	movw	r3, #0xcf3d
  4903f4: e3433cf3     	movt	r3, #0x3cf3
  4903f8: e0661002     	rsb	r1, r6, r2
  4903fc: e1a01141     	asr	r1, r1, #2
  490400: e0030193     	mul	r3, r3, r1
  490404: e3530000     	cmp	r3, #0
  490408: 0a000036     	beq	0x4904e8 <_ZN3rnd4Rule4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x1e4> @ imm = #0xd8
  49040c: e1520006     	cmp	r2, r6
  490410: 0a000022     	beq	0x4904a0 <_ZN3rnd4Rule4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x19c> @ imm = #0x88
  490414: e59db014     	ldr	r11, [sp, #0x14]
  490418: e59d901c     	ldr	r9, [sp, #0x1c]
  49041c: e58d5014     	str	r5, [sp, #0x14]
  490420: e59d5010     	ldr	r5, [sp, #0x10]
  490424: e28d7084     	add	r7, sp, #132
  490428: e1a08004     	mov	r8, r4
  49042c: e1a01006     	mov	r1, r6
  490430: e4914004     	ldr	r4, [r1], #4
  490434: e1a00007     	mov	r0, r7
  490438: ebfff790     	bl	0x48e280 <_ZN3rnd8ListElemC1ERKS0_> @ imm = #-0x21c0
  49043c: e1a0000b     	mov	r0, r11
  490440: e1a01009     	mov	r1, r9
  490444: e1a02008     	mov	r2, r8
  490448: e1a03004     	mov	r3, r4
  49044c: e58d7000     	str	r7, [sp]
  490450: eb000596     	bl	0x491ab0 <_ZN3rnd4Tile8TrySpawnEPKNS_4ExitES3_S3_RNS_8ListElemE> @ imm = #0x1658
  490454: e250a000     	subs	r10, r0, #0
  490458: 0a000009     	beq	0x490484 <_ZN3rnd4Rule4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x180> @ imm = #0x24
  49045c: e1a02004     	mov	r2, r4
  490460: e5953000     	ldr	r3, [r5]
  490464: e1a00005     	mov	r0, r5
  490468: e1a0100a     	mov	r1, r10
  49046c: e1a0e00f     	mov	lr, pc
  490470: e593f010     	ldr	pc, [r3, #0x10]
  490474: e3500000     	cmp	r0, #0
  490478: 1a000015     	bne	0x4904d4 <_ZN3rnd4Rule4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x1d0> @ imm = #0x54
  49047c: e1a0000a     	mov	r0, r10
  490480: eb000508     	bl	0x4918a8 <_ZN3rnd4Tile7UnspawnEv> @ imm = #0x1420
  490484: e1a00007     	mov	r0, r7
  490488: ebffdf50     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x82c0
  49048c: e59d3024     	ldr	r3, [sp, #0x24]
  490490: e2866054     	add	r6, r6, #84
  490494: e1560003     	cmp	r6, r3
  490498: 1affffe3     	bne	0x49042c <_ZN3rnd4Rule4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x128> @ imm = #-0x74
  49049c: e59d5014     	ldr	r5, [sp, #0x14]
  4904a0: e3a04000     	mov	r4, #0
  4904a4: e1a00005     	mov	r0, r5
  4904a8: ebfff512     	bl	0x48d8f8 <_ZNSt6vectorISt4pairIPKN3rnd4ExitENS1_8ListElemEESaIS6_EED1Ev> @ imm = #-0x2bb8
  4904ac: e59d100c     	ldr	r1, [sp, #0xc]
  4904b0: e59dc018     	ldr	r12, [sp, #0x18]
  4904b4: e59d2144     	ldr	r2, [sp, #0x144]
  4904b8: e1a00004     	mov	r0, r4
  4904bc: e791300c     	ldr	r3, [r1, r12]
  4904c0: e5933000     	ldr	r3, [r3]
  4904c4: e1520003     	cmp	r2, r3
  4904c8: 1a000010     	bne	0x490510 <_ZN3rnd4Rule4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x20c> @ imm = #0x40
  4904cc: e28ddf53     	add	sp, sp, #332
  4904d0: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  4904d4: e1a00007     	mov	r0, r7
  4904d8: e59d5014     	ldr	r5, [sp, #0x14]
  4904dc: e3a04001     	mov	r4, #1
  4904e0: ebffdf3a     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x8318
  4904e4: eaffffee     	b	0x4904a4 <_ZN3rnd4Rule4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x1a0> @ imm = #-0x48
  4904e8: e59d2010     	ldr	r2, [sp, #0x10]
  4904ec: e59f1028     	ldr	r1, [pc, #0x28]         @ 0x49051c <_ZN3rnd4Rule4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x218>
  4904f0: e28d0f49     	add	r0, sp, #292
  4904f4: e5923004     	ldr	r3, [r2, #0x4]
  4904f8: e08f1001     	add	r1, pc, r1
  4904fc: e593206c     	ldr	r2, [r3, #0x6c]
  490500: ebf9f977     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x181a24
  490504: e59d6020     	ldr	r6, [sp, #0x20]
  490508: e59d2024     	ldr	r2, [sp, #0x24]
  49050c: eaffffbe     	b	0x49040c <_ZN3rnd4Rule4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x108> @ imm = #-0x108
  490510: ebf9f77e     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x182208
  490514: 80 47 50 00  	.word	0x00504780
  490518: ac 40 00 00  	.word	0x000040ac
  49051c: f0 49 44 00  	.word	0x004449f0
