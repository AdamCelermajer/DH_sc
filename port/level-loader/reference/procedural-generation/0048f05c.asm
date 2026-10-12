
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048f05c <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i>:
  48f05c: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  48f060: e59f41ec     	ldr	r4, [pc, #0x1ec]        @ 0x48f254 <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i+0x1f8>
  48f064: e59f61ec     	ldr	r6, [pc, #0x1ec]        @ 0x48f258 <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i+0x1fc>
  48f068: e1a07002     	mov	r7, r2
  48f06c: e08f4004     	add	r4, pc, r4
  48f070: e794c006     	ldr	r12, [r4, r6]
  48f074: e24ddf95     	sub	sp, sp, #596
  48f078: e28d5e25     	add	r5, sp, #592
  48f07c: e59c2000     	ldr	r2, [r12]
  48f080: e1a09000     	mov	r9, r0
  48f084: e58d3004     	str	r3, [sp, #0x4]
  48f088: e58d224c     	str	r2, [sp, #0x24c]
  48f08c: e3a02000     	mov	r2, #0
  48f090: e5252148     	str	r2, [r5, #-0x148]!
  48f094: e2858004     	add	r8, r5, #4
  48f098: e1a00008     	mov	r0, r8
  48f09c: e1a0a001     	mov	r10, r1
  48f0a0: ebfffbf6     	bl	0x48e080 <_ZN3rnd8ListElemC1Ev> @ imm = #-0x1028
  48f0a4: e5973004     	ldr	r3, [r7, #0x4]
  48f0a8: e5932054     	ldr	r2, [r3, #0x54]
  48f0ac: e3520001     	cmp	r2, #1
  48f0b0: 0a00000e     	beq	0x48f0f0 <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i+0x94> @ imm = #0x38
  48f0b4: e59d3108     	ldr	r3, [sp, #0x108]
  48f0b8: e3530000     	cmp	r3, #0
  48f0bc: 0a000002     	beq	0x48f0cc <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i+0x70> @ imm = #0x8
  48f0c0: e1a0000a     	mov	r0, r10
  48f0c4: e1a01005     	mov	r1, r5
  48f0c8: ebffff96     	bl	0x48ef28 <_ZNSt6vectorISt4pairIPKN3rnd4ExitENS1_8ListElemEESaIS6_EE9push_backERKS6_> @ imm = #-0x1a8
  48f0cc: e2850004     	add	r0, r5, #4
  48f0d0: ebffe43e     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x6f08
  48f0d4: e7943006     	ldr	r3, [r4, r6]
  48f0d8: e59d224c     	ldr	r2, [sp, #0x24c]
  48f0dc: e5933000     	ldr	r3, [r3]
  48f0e0: e1520003     	cmp	r2, r3
  48f0e4: 1a000059     	bne	0x48f250 <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i+0x1f4> @ imm = #0x164
  48f0e8: e28ddf95     	add	sp, sp, #596
  48f0ec: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  48f0f0: e5932058     	ldr	r2, [r3, #0x58]
  48f0f4: e3520001     	cmp	r2, #1
  48f0f8: 1affffed     	bne	0x48f0b4 <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i+0x58> @ imm = #-0x4c
  48f0fc: e1a00009     	mov	r0, r9
  48f100: e593b05c     	ldr	r11, [r3, #0x5c]
  48f104: ebfff300     	bl	0x48bd0c <_ZNK3rnd4Path4Impl9IsDeadEndEv> @ imm = #-0x3400
  48f108: e3500000     	cmp	r0, #0
  48f10c: 0a00001a     	beq	0x48f17c <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i+0x120> @ imm = #0x68
  48f110: e35b0001     	cmp	r11, #1
  48f114: 1affffe6     	bne	0x48f0b4 <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i+0x58> @ imm = #-0x68
  48f118: e5993004     	ldr	r3, [r9, #0x4]
  48f11c: e59d2004     	ldr	r2, [sp, #0x4]
  48f120: e28d9f7f     	add	r9, sp, #508
  48f124: e5933068     	ldr	r3, [r3, #0x68]
  48f128: e3a01050     	mov	r1, #80
  48f12c: e1a00009     	mov	r0, r9
  48f130: e593301c     	ldr	r3, [r3, #0x1c]
  48f134: e0213291     	mla	r1, r1, r2, r3
  48f138: ebfffc50     	bl	0x48e280 <_ZN3rnd8ListElemC1ERKS0_> @ imm = #-0xec0
  48f13c: e28d3e25     	add	r3, sp, #592
  48f140: e523719c     	str	r7, [r3, #-0x19c]!
  48f144: e2837004     	add	r7, r3, #4
  48f148: e1a01009     	mov	r1, r9
  48f14c: e1a00007     	mov	r0, r7
  48f150: ebfffc4a     	bl	0x48e280 <_ZN3rnd8ListElemC1ERKS0_> @ imm = #-0xed8
  48f154: e59d30b4     	ldr	r3, [sp, #0xb4]
  48f158: e1a00008     	mov	r0, r8
  48f15c: e1a01007     	mov	r1, r7
  48f160: e58d3108     	str	r3, [sp, #0x108]
  48f164: ebfff3d4     	bl	0x48c0bc <_ZN3rnd8ListElemaSERKS0_> @ imm = #-0x30b0
  48f168: e1a00007     	mov	r0, r7
  48f16c: ebffe417     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x6fa4
  48f170: e1a00009     	mov	r0, r9
  48f174: ebffe415     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x6fac
  48f178: eaffffcd     	b	0x48f0b4 <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i+0x58> @ imm = #-0xcc
  48f17c: e35b0002     	cmp	r11, #2
  48f180: 1affffcb     	bne	0x48f0b4 <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i+0x58> @ imm = #-0xd4
  48f184: e5993044     	ldr	r3, [r9, #0x44]
  48f188: e5d3308d     	ldrb	r3, [r3, #0x8d]
  48f18c: e3530000     	cmp	r3, #0
  48f190: 0a00001b     	beq	0x48f204 <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i+0x1a8> @ imm = #0x6c
  48f194: e5970004     	ldr	r0, [r7, #0x4]
  48f198: ebffe9fa     	bl	0x489988 <_ZNK3rnd5Block10IsStraightEv> @ imm = #-0x5818
  48f19c: e3500000     	cmp	r0, #0
  48f1a0: 0affffc3     	beq	0x48f0b4 <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i+0x58> @ imm = #-0xf4
  48f1a4: e5993004     	ldr	r3, [r9, #0x4]
  48f1a8: e59d2004     	ldr	r2, [sp, #0x4]
  48f1ac: e28d9f6b     	add	r9, sp, #428
  48f1b0: e5933068     	ldr	r3, [r3, #0x68]
  48f1b4: e3a01050     	mov	r1, #80
  48f1b8: e28db064     	add	r11, sp, #100
  48f1bc: e593301c     	ldr	r3, [r3, #0x1c]
  48f1c0: e1a00009     	mov	r0, r9
  48f1c4: e0213291     	mla	r1, r1, r2, r3
  48f1c8: ebfffc2c     	bl	0x48e280 <_ZN3rnd8ListElemC1ERKS0_> @ imm = #-0xf50
  48f1cc: e1a01009     	mov	r1, r9
  48f1d0: e1a0000b     	mov	r0, r11
  48f1d4: e58d7060     	str	r7, [sp, #0x60]
  48f1d8: ebfffc28     	bl	0x48e280 <_ZN3rnd8ListElemC1ERKS0_> @ imm = #-0xf60
  48f1dc: e59d3060     	ldr	r3, [sp, #0x60]
  48f1e0: e1a0100b     	mov	r1, r11
  48f1e4: e1a00008     	mov	r0, r8
  48f1e8: e58d3108     	str	r3, [sp, #0x108]
  48f1ec: ebfff3b2     	bl	0x48c0bc <_ZN3rnd8ListElemaSERKS0_> @ imm = #-0x3138
  48f1f0: e1a0000b     	mov	r0, r11
  48f1f4: ebffe3f5     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x702c
  48f1f8: e1a00009     	mov	r0, r9
  48f1fc: ebffe3f3     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x7034
  48f200: eaffffab     	b	0x48f0b4 <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i+0x58> @ imm = #-0x154
  48f204: e5993004     	ldr	r3, [r9, #0x4]
  48f208: e59d2004     	ldr	r2, [sp, #0x4]
  48f20c: e28d9f57     	add	r9, sp, #348
  48f210: e5933068     	ldr	r3, [r3, #0x68]
  48f214: e3a01050     	mov	r1, #80
  48f218: e1a00009     	mov	r0, r9
  48f21c: e593301c     	ldr	r3, [r3, #0x1c]
  48f220: e0213291     	mla	r1, r1, r2, r3
  48f224: ebfffc15     	bl	0x48e280 <_ZN3rnd8ListElemC1ERKS0_> @ imm = #-0xfac
  48f228: e28d3e25     	add	r3, sp, #592
  48f22c: e5237244     	str	r7, [r3, #-0x244]!
  48f230: e2837004     	add	r7, r3, #4
  48f234: e1a01009     	mov	r1, r9
  48f238: e1a00007     	mov	r0, r7
  48f23c: ebfffc0f     	bl	0x48e280 <_ZN3rnd8ListElemC1ERKS0_> @ imm = #-0xfc4
  48f240: e1a00008     	mov	r0, r8
  48f244: e1a01007     	mov	r1, r7
  48f248: e59d300c     	ldr	r3, [sp, #0xc]
  48f24c: eaffffc3     	b	0x48f160 <_ZN3rnd4Path4Impl7AddExitERSt6vectorISt4pairIPKNS_4ExitENS_8ListElemEESaIS8_EES6_i+0x104> @ imm = #-0xf4
  48f250: ebf9fc2e     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x180f48
  48f254: 24 5a 50 00  	.word	0x00505a24
  48f258: ac 40 00 00  	.word	0x000040ac
