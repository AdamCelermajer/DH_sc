
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048f25c <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE>:
  48f25c: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  48f260: e24dd024     	sub	sp, sp, #36
  48f264: e58d0004     	str	r0, [sp, #0x4]
  48f268: e5903004     	ldr	r3, [r0, #0x4]
  48f26c: e3a00000     	mov	r0, #0
  48f270: e58d0014     	str	r0, [sp, #0x14]
  48f274: e58d0018     	str	r0, [sp, #0x18]
  48f278: e58d001c     	str	r0, [sp, #0x1c]
  48f27c: e5933068     	ldr	r3, [r3, #0x68]
  48f280: e1a0b001     	mov	r11, r1
  48f284: e58d200c     	str	r2, [sp, #0xc]
  48f288: e1530000     	cmp	r3, r0
  48f28c: 0a000075     	beq	0x48f468 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x20c> @ imm = #0x1d4
  48f290: e89100c0     	ldm	r1, {r6, r7}
  48f294: e1a03007     	mov	r3, r7
  48f298: e1560007     	cmp	r6, r7
  48f29c: 028d2014     	addeq	r2, sp, #20
  48f2a0: 058d2008     	streq	r2, [sp, #0x8]
  48f2a4: 0a00003f     	beq	0x48f3a8 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x14c> @ imm = #0xfc
  48f2a8: e28d3014     	add	r3, sp, #20
  48f2ac: e58d3008     	str	r3, [sp, #0x8]
  48f2b0: e3a09050     	mov	r9, #80
  48f2b4: e1a0a006     	mov	r10, r6
  48f2b8: e59a5000     	ldr	r5, [r10]
  48f2bc: e3550000     	cmp	r5, #0
  48f2c0: 0a000033     	beq	0x48f394 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x138> @ imm = #0xcc
  48f2c4: e5953004     	ldr	r3, [r5, #0x4]
  48f2c8: e3530000     	cmp	r3, #0
  48f2cc: 0a000030     	beq	0x48f394 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x138> @ imm = #0xc0
  48f2d0: e59d0004     	ldr	r0, [sp, #0x4]
  48f2d4: e5908004     	ldr	r8, [r0, #0x4]
  48f2d8: e598406c     	ldr	r4, [r8, #0x6c]
  48f2dc: e3740001     	cmn	r4, #1
  48f2e0: 1a00003f     	bne	0x48f3e4 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x188> @ imm = #0xfc
  48f2e4: e5982068     	ldr	r2, [r8, #0x68]
  48f2e8: e5921020     	ldr	r1, [r2, #0x20]
  48f2ec: e592201c     	ldr	r2, [r2, #0x1c]
  48f2f0: e0621001     	rsb	r1, r2, r1
  48f2f4: e1a01241     	asr	r1, r1, #4
  48f2f8: e0810081     	add	r0, r1, r1, lsl #1
  48f2fc: e0800200     	add	r0, r0, r0, lsl #4
  48f300: e0800400     	add	r0, r0, r0, lsl #8
  48f304: e0800800     	add	r0, r0, r0, lsl #16
  48f308: e0811100     	add	r1, r1, r0, lsl #2
  48f30c: e3510000     	cmp	r1, #0
  48f310: 0a00001f     	beq	0x48f394 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x138> @ imm = #0x7c
  48f314: e3a00000     	mov	r0, #0
  48f318: e1a04000     	mov	r4, r0
  48f31c: ea000000     	b	0x48f324 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0xc8> @ imm = #0x0
  48f320: e5953004     	ldr	r3, [r5, #0x4]
  48f324: e0222099     	mla	r2, r9, r0, r2
  48f328: e5937014     	ldr	r7, [r3, #0x14]
  48f32c: e5921018     	ldr	r1, [r2, #0x18]
  48f330: e5926014     	ldr	r6, [r2, #0x14]
  48f334: e5930018     	ldr	r0, [r3, #0x18]
  48f338: e0616006     	rsb	r6, r1, r6
  48f33c: e0607007     	rsb	r7, r0, r7
  48f340: e1560007     	cmp	r6, r7
  48f344: b1a02006     	movlt	r2, r6
  48f348: a1a02007     	movge	r2, r7
  48f34c: ebf9fca3     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0x180d74
  48f350: e3500000     	cmp	r0, #0
  48f354: 0a000039     	beq	0x48f440 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x1e4> @ imm = #0xe4
  48f358: e5982068     	ldr	r2, [r8, #0x68]
  48f35c: e2844001     	add	r4, r4, #1
  48f360: e1a00004     	mov	r0, r4
  48f364: e5923020     	ldr	r3, [r2, #0x20]
  48f368: e592201c     	ldr	r2, [r2, #0x1c]
  48f36c: e0623003     	rsb	r3, r2, r3
  48f370: e1a03243     	asr	r3, r3, #4
  48f374: e0831083     	add	r1, r3, r3, lsl #1
  48f378: e0811201     	add	r1, r1, r1, lsl #4
  48f37c: e0811401     	add	r1, r1, r1, lsl #8
  48f380: e0811801     	add	r1, r1, r1, lsl #16
  48f384: e0833101     	add	r3, r3, r1, lsl #2
  48f388: e1530004     	cmp	r3, r4
  48f38c: 8affffe3     	bhi	0x48f320 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0xc4> @ imm = #-0x74
  48f390: e59b7004     	ldr	r7, [r11, #0x4]
  48f394: e28aa054     	add	r10, r10, #84
  48f398: e15a0007     	cmp	r10, r7
  48f39c: 1affffc5     	bne	0x48f2b8 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x5c> @ imm = #-0xec
  48f3a0: e59b3000     	ldr	r3, [r11]
  48f3a4: e59d0014     	ldr	r0, [sp, #0x14]
  48f3a8: e59d1018     	ldr	r1, [sp, #0x18]
  48f3ac: e59d201c     	ldr	r2, [sp, #0x1c]
  48f3b0: e59bc008     	ldr	r12, [r11, #0x8]
  48f3b4: e88b0007     	stm	r11, {r0, r1, r2}
  48f3b8: e59d0004     	ldr	r0, [sp, #0x4]
  48f3bc: e1a0100b     	mov	r1, r11
  48f3c0: e59d200c     	ldr	r2, [sp, #0xc]
  48f3c4: e58d3014     	str	r3, [sp, #0x14]
  48f3c8: e58d7018     	str	r7, [sp, #0x18]
  48f3cc: e58dc01c     	str	r12, [sp, #0x1c]
  48f3d0: ebfffcae     	bl	0x48e690 <_ZN3rnd4Rule4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE> @ imm = #-0xd48
  48f3d4: e59d0008     	ldr	r0, [sp, #0x8]
  48f3d8: ebfff946     	bl	0x48d8f8 <_ZNSt6vectorISt4pairIPKN3rnd4ExitENS1_8ListElemEESaIS6_EED1Ev> @ imm = #-0x1ae8
  48f3dc: e28dd024     	add	sp, sp, #36
  48f3e0: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  48f3e4: e5982068     	ldr	r2, [r8, #0x68]
  48f3e8: e5930018     	ldr	r0, [r3, #0x18]
  48f3ec: e5938014     	ldr	r8, [r3, #0x14]
  48f3f0: e592301c     	ldr	r3, [r2, #0x1c]
  48f3f4: e0608008     	rsb	r8, r0, r8
  48f3f8: e0233499     	mla	r3, r9, r4, r3
  48f3fc: e5936014     	ldr	r6, [r3, #0x14]
  48f400: e5931018     	ldr	r1, [r3, #0x18]
  48f404: e0616006     	rsb	r6, r1, r6
  48f408: e1560008     	cmp	r6, r8
  48f40c: b1a02006     	movlt	r2, r6
  48f410: a1a02008     	movge	r2, r8
  48f414: ebf9fc71     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0x180e3c
  48f418: e3500000     	cmp	r0, #0
  48f41c: 1affffdc     	bne	0x48f394 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x138> @ imm = #-0x90
  48f420: e1580006     	cmp	r8, r6
  48f424: baffffda     	blt	0x48f394 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x138> @ imm = #-0x98
  48f428: caffffd9     	bgt	0x48f394 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x138> @ imm = #-0x9c
  48f42c: e1a02005     	mov	r2, r5
  48f430: e1a03004     	mov	r3, r4
  48f434: e99d0003     	ldmib	sp, {r0, r1}
  48f438: ebffff07     	bl	0x48f05c <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i> @ imm = #-0x3e4
  48f43c: eaffffd3     	b	0x48f390 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x134> @ imm = #-0xb4
  48f440: e1570006     	cmp	r7, r6
  48f444: baffffc3     	blt	0x48f358 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0xfc> @ imm = #-0xf4
  48f448: caffffc2     	bgt	0x48f358 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0xfc> @ imm = #-0xf8
  48f44c: e1a02005     	mov	r2, r5
  48f450: e99d0003     	ldmib	sp, {r0, r1}
  48f454: e1a03004     	mov	r3, r4
  48f458: ebfffeff     	bl	0x48f05c <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i> @ imm = #-0x404
  48f45c: e59d2004     	ldr	r2, [sp, #0x4]
  48f460: e5928004     	ldr	r8, [r2, #0x4]
  48f464: eaffffbb     	b	0x48f358 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0xfc> @ imm = #-0x114
  48f468: e28d2014     	add	r2, sp, #20
  48f46c: e1a00003     	mov	r0, r3
  48f470: e8910088     	ldm	r1, {r3, r7}
  48f474: e58d2008     	str	r2, [sp, #0x8]
  48f478: eaffffca     	b	0x48f3a8 <_ZN3rnd4Path4Impl11FilterExitsERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EERPNS_8ListRuleE+0x14c> @ imm = #-0xd8
