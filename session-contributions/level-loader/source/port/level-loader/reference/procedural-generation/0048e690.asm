
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048e690 <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE>:
  48e690: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  48e694: e5916000     	ldr	r6, [r1]
  48e698: e5913004     	ldr	r3, [r1, #0x4]
  48e69c: e1a04001     	mov	r4, r1
  48e6a0: e24dd00c     	sub	sp, sp, #12
  48e6a4: e1560003     	cmp	r6, r3
  48e6a8: e1a05000     	mov	r5, r0
  48e6ac: 01a04006     	moveq	r4, r6
  48e6b0: 0a000023     	beq	0x48e744 <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0xb4> @ imm = #0x8c
  48e6b4: e59f8100     	ldr	r8, [pc, #0x100]        @ 0x48e7bc <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x12c>
  48e6b8: e1a01006     	mov	r1, r6
  48e6bc: e08f8008     	add	r8, pc, r8
  48e6c0: e2887006     	add	r7, r8, #6
  48e6c4: e2888002     	add	r8, r8, #2
  48e6c8: ea000002     	b	0x48e6d8 <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x48> @ imm = #0x8
  48e6cc: e2811054     	add	r1, r1, #84
  48e6d0: e1510003     	cmp	r1, r3
  48e6d4: 0a000022     	beq	0x48e764 <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0xd4> @ imm = #0x88
  48e6d8: e5912000     	ldr	r2, [r1]
  48e6dc: e5922004     	ldr	r2, [r2, #0x4]
  48e6e0: e5920018     	ldr	r0, [r2, #0x18]
  48e6e4: e5922014     	ldr	r2, [r2, #0x14]
  48e6e8: e052c000     	subs	r12, r2, r0
  48e6ec: 0afffff6     	beq	0x48e6cc <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x3c> @ imm = #-0x28
  48e6f0: e35c0005     	cmp	r12, #5
  48e6f4: 9afffff4     	bls	0x48e6cc <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x3c> @ imm = #-0x30
  48e6f8: e1520000     	cmp	r2, r0
  48e6fc: 0a000006     	beq	0x48e71c <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x8c> @ imm = #0x18
  48e700: e280b001     	add	r11, r0, #1
  48e704: e15bc0d1     	ldrsb	r12, [r11, #-1]
  48e708: e35c005f     	cmp	r12, #95
  48e70c: 0a000017     	beq	0x48e770 <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0xe0> @ imm = #0x5c
  48e710: e152000b     	cmp	r2, r11
  48e714: e28bb001     	add	r11, r11, #1
  48e718: 1afffff9     	bne	0x48e704 <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x74> @ imm = #-0x1c
  48e71c: e1a0c002     	mov	r12, r2
  48e720: e152000c     	cmp	r2, r12
  48e724: 0affffe8     	beq	0x48e6cc <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x3c> @ imm = #-0x60
  48e728: e060000c     	rsb	r0, r0, r12
  48e72c: e3700001     	cmn	r0, #1
  48e730: 0affffe5     	beq	0x48e6cc <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x3c> @ imm = #-0x6c
  48e734: e1a00004     	mov	r0, r4
  48e738: e28d2004     	add	r2, sp, #4
  48e73c: ebfffccb     	bl	0x48da70 <_ZNSt6vectorISt4pairIPKN3rnd4ExitENS1_8ListElemEESaIS6_EE8_M_eraseEPS6_RKSt12__false_type> @ imm = #-0xcd4
  48e740: e8940050     	ldm	r4, {r4, r6}
  48e744: e5950004     	ldr	r0, [r5, #0x4]
  48e748: ebfff569     	bl	0x48bcf4 <_ZNK3rnd4Rule6GetAppEv> @ imm = #-0x2a5c
  48e74c: e1a01006     	mov	r1, r6
  48e750: e1a02000     	mov	r2, r0
  48e754: e1a00004     	mov	r0, r4
  48e758: ebffffb2     	bl	0x48e628 <_ZSt14random_shuffleIPSt4pairIPKN3rnd4ExitENS1_8ListElemEENS1_15RandomGeneratorEEvT_S9_RT0_> @ imm = #-0x138
  48e75c: e28dd00c     	add	sp, sp, #12
  48e760: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  48e764: e1a04006     	mov	r4, r6
  48e768: e1a06001     	mov	r6, r1
  48e76c: eafffff4     	b	0x48e744 <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0xb4> @ imm = #-0x30
  48e770: e15b0002     	cmp	r11, r2
  48e774: e1a0c00b     	mov	r12, r11
  48e778: 0affffe8     	beq	0x48e720 <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x90> @ imm = #-0x60
  48e77c: e1a0e008     	mov	lr, r8
  48e780: ea000004     	b	0x48e798 <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x108> @ imm = #0x10
  48e784: e15e0007     	cmp	lr, r7
  48e788: 0a000009     	beq	0x48e7b4 <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x124> @ imm = #0x24
  48e78c: e15c0002     	cmp	r12, r2
  48e790: e28ee001     	add	lr, lr, #1
  48e794: 0affffe1     	beq	0x48e720 <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x90> @ imm = #-0x7c
  48e798: e1dc90d0     	ldrsb	r9, [r12]
  48e79c: e15ea0d1     	ldrsb	r10, [lr, #-1]
  48e7a0: e28cc001     	add	r12, r12, #1
  48e7a4: e159000a     	cmp	r9, r10
  48e7a8: 0afffff5     	beq	0x48e784 <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0xf4> @ imm = #-0x2c
  48e7ac: e28bb001     	add	r11, r11, #1
  48e7b0: eaffffd3     	b	0x48e704 <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x74> @ imm = #-0xb4
  48e7b4: e24bc001     	sub	r12, r11, #1
  48e7b8: eaffffd8     	b	0x48e720 <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x90> @ imm = #-0xa0
  48e7bc: cc 67 44 00  	.word	0x004467cc
