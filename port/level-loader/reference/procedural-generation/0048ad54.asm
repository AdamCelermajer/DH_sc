
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048ad54 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_>:
  48ad54: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  48ad58: e59f5380     	ldr	r5, [pc, #0x380]        @ 0x48b0e0 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x38c>
  48ad5c: e59fb380     	ldr	r11, [pc, #0x380]       @ 0x48b0e4 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x390>
  48ad60: e59f3380     	ldr	r3, [pc, #0x380]        @ 0x48b0e8 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x394>
  48ad64: e08f5005     	add	r5, pc, r5
  48ad68: e795c00b     	ldr	r12, [r5, r11]
  48ad6c: e7953003     	ldr	r3, [r5, r3]
  48ad70: e24ddf91     	sub	sp, sp, #580
  48ad74: e59cc000     	ldr	r12, [r12]
  48ad78: e5933010     	ldr	r3, [r3, #0x10]
  48ad7c: e1a0a001     	mov	r10, r1
  48ad80: e59f1364     	ldr	r1, [pc, #0x364]        @ 0x48b0ec <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x398>
  48ad84: e58dc23c     	str	r12, [sp, #0x23c]
  48ad88: e5936034     	ldr	r6, [r3, #0x34]
  48ad8c: e28d403c     	add	r4, sp, #60
  48ad90: e1a07002     	mov	r7, r2
  48ad94: e08f1001     	add	r1, pc, r1
  48ad98: e1a02000     	mov	r2, r0
  48ad9c: e1a03007     	mov	r3, r7
  48ada0: e1a08000     	mov	r8, r0
  48ada4: e1a00004     	mov	r0, r4
  48ada8: ebfa0f4d     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x17c2cc
  48adac: e3a02000     	mov	r2, #0
  48adb0: e1a01004     	mov	r1, r4
  48adb4: e596c000     	ldr	r12, [r6]
  48adb8: e1a00006     	mov	r0, r6
  48adbc: e1a03002     	mov	r3, r2
  48adc0: e1a0e00f     	mov	lr, pc
  48adc4: e59cf088     	ldr	pc, [r12, #0x88]
  48adc8: e2504000     	subs	r4, r0, #0
  48adcc: 0a00006a     	beq	0x48af7c <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x228> @ imm = #0x1a8
  48add0: e3a01000     	mov	r1, #0
  48add4: e30009c8     	movw	r0, #0x9c8
  48add8: e58d4034     	str	r4, [sp, #0x34]
  48addc: ebfa15e3     	bl	0x310570 <_Znwj15MemoryHintState> @ imm = #-0x17a874
  48ade0: e1a04000     	mov	r4, r0
  48ade4: ebffffca     	bl	0x48ad14 <_ZN3rnd8MgxBlockC1Ev> @ imm = #-0xd8
  48ade8: e1a00007     	mov	r0, r7
  48adec: ebfa0c18     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x17cfa0
  48adf0: e2841004     	add	r1, r4, #4
  48adf4: e58d1010     	str	r1, [sp, #0x10]
  48adf8: e0872000     	add	r2, r7, r0
  48adfc: e1a01007     	mov	r1, r7
  48ae00: e59d0010     	ldr	r0, [sp, #0x10]
  48ae04: ebfa16f5     	bl	0x3109e0 <_ZNSs9_M_assignEPKcS0_> @ imm = #-0x17a42c
  48ae08: e5947018     	ldr	r7, [r4, #0x18]
  48ae0c: e5949014     	ldr	r9, [r4, #0x14]
  48ae10: e0679009     	rsb	r9, r7, r9
  48ae14: e3590003     	cmp	r9, #3
  48ae18: 9a000041     	bls	0x48af24 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x1d0> @ imm = #0x104
  48ae1c: e59fe2cc     	ldr	lr, [pc, #0x2cc]        @ 0x48b0f0 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x39c>
  48ae20: e0879009     	add	r9, r7, r9
  48ae24: e28d0030     	add	r0, sp, #48
  48ae28: e08fe00e     	add	lr, pc, lr
  48ae2c: e28ec004     	add	r12, lr, #4
  48ae30: e58dc024     	str	r12, [sp, #0x24]
  48ae34: e28dc020     	add	r12, sp, #32
  48ae38: e58dc000     	str	r12, [sp]
  48ae3c: e28d102c     	add	r1, sp, #44
  48ae40: e28dc038     	add	r12, sp, #56
  48ae44: e28d2028     	add	r2, sp, #40
  48ae48: e28d3024     	add	r3, sp, #36
  48ae4c: e58dc004     	str	r12, [sp, #0x4]
  48ae50: e58de020     	str	lr, [sp, #0x20]
  48ae54: e58d902c     	str	r9, [sp, #0x2c]
  48ae58: e58d7028     	str	r7, [sp, #0x28]
  48ae5c: ebfd9108     	bl	0x3ef284 <_ZSt6searchISt16reverse_iteratorIPKcES3_NSt4priv10_Eq_traitsISt11char_traitsIcEEEET_S9_S9_T0_SA_T1_> @ imm = #-0x9bbe0
  48ae60: e59dc030     	ldr	r12, [sp, #0x30]
  48ae64: e157000c     	cmp	r7, r12
  48ae68: 01a0c009     	moveq	r12, r9
  48ae6c: 124cc004     	subne	r12, r12, #4
  48ae70: e159000c     	cmp	r9, r12
  48ae74: 0a00002a     	beq	0x48af24 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x1d0> @ imm = #0xa8
  48ae78: e5947018     	ldr	r7, [r4, #0x18]
  48ae7c: e067c00c     	rsb	r12, r7, r12
  48ae80: e37c0001     	cmn	r12, #1
  48ae84: 0a000026     	beq	0x48af24 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x1d0> @ imm = #0x98
  48ae88: e5949014     	ldr	r9, [r4, #0x14]
  48ae8c: e0679009     	rsb	r9, r7, r9
  48ae90: e15c0009     	cmp	r12, r9
  48ae94: 8a000076     	bhi	0x48b074 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x320> @ imm = #0x1d8
  48ae98: e59f3254     	ldr	r3, [pc, #0x254]        @ 0x48b0f4 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x3a0>
  48ae9c: e06c9009     	rsb	r9, r12, r9
  48aea0: e3590004     	cmp	r9, #4
  48aea4: 908c9009     	addls	r9, r12, r9
  48aea8: 828c9004     	addhi	r9, r12, #4
  48aeac: e08f3003     	add	r3, pc, r3
  48aeb0: e1570003     	cmp	r7, r3
  48aeb4: 83a02000     	movhi	r2, #0
  48aeb8: e0879009     	add	r9, r7, r9
  48aebc: e087c00c     	add	r12, r7, r12
  48aec0: 858d2018     	strhi	r2, [sp, #0x18]
  48aec4: 8a000004     	bhi	0x48aedc <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x188> @ imm = #0x10
  48aec8: e5942014     	ldr	r2, [r4, #0x14]
  48aecc: e1520003     	cmp	r2, r3
  48aed0: 93a02000     	movls	r2, #0
  48aed4: 83a02001     	movhi	r2, #1
  48aed8: e58d2018     	str	r2, [sp, #0x18]
  48aedc: e059300c     	subs	r3, r9, r12
  48aee0: e58d301c     	str	r3, [sp, #0x1c]
  48aee4: 4a000032     	bmi	0x48afb4 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x260> @ imm = #0xc8
  48aee8: e15c0009     	cmp	r12, r9
  48aeec: 0a00000c     	beq	0x48af24 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x1d0> @ imm = #0x30
  48aef0: e5943014     	ldr	r3, [r4, #0x14]
  48aef4: e2832001     	add	r2, r3, #1
  48aef8: e0522009     	subs	r2, r2, r9
  48aefc: 0a000005     	beq	0x48af18 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x1c4> @ imm = #0x14
  48af00: e1a0000c     	mov	r0, r12
  48af04: e1a01009     	mov	r1, r9
  48af08: e58dc00c     	str	r12, [sp, #0xc]
  48af0c: ebfa0c09     	bl	0x30df38 <.plt+0x1c4>   @ imm = #-0x17cfdc
  48af10: e5943014     	ldr	r3, [r4, #0x14]
  48af14: e59dc00c     	ldr	r12, [sp, #0xc]
  48af18: e069900c     	rsb	r9, r9, r12
  48af1c: e0833009     	add	r3, r3, r9
  48af20: e5843014     	str	r3, [r4, #0x14]
  48af24: e1a00008     	mov	r0, r8
  48af28: ebfa0bc9     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x17d0dc
  48af2c: e1a01008     	mov	r1, r8
  48af30: e0882000     	add	r2, r8, r0
  48af34: e284001c     	add	r0, r4, #28
  48af38: ebfa16a8     	bl	0x3109e0 <_ZNSs9_M_assignEPKcS0_> @ imm = #-0x17a560
  48af3c: e1a0000a     	mov	r0, r10
  48af40: ebfa0bc3     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x17d0f4
  48af44: e1a0100a     	mov	r1, r10
  48af48: e08a2000     	add	r2, r10, r0
  48af4c: e2840034     	add	r0, r4, #52
  48af50: ebfa16a2     	bl	0x3109e0 <_ZNSs9_M_assignEPKcS0_> @ imm = #-0x17a578
  48af54: e1a00004     	mov	r0, r4
  48af58: e59d1034     	ldr	r1, [sp, #0x34]
  48af5c: ebfffec8     	bl	0x48aa84 <_ZN3rnd8MgxBlock17LoadFromXmlStreamEP11IFileStream> @ imm = #-0x4e0
  48af60: e2507000     	subs	r7, r0, #0
  48af64: 0a00000c     	beq	0x48af9c <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x248> @ imm = #0x30
  48af68: e1a00006     	mov	r0, r6
  48af6c: e5963000     	ldr	r3, [r6]
  48af70: e28d1034     	add	r1, sp, #52
  48af74: e1a0e00f     	mov	lr, pc
  48af78: e593f078     	ldr	pc, [r3, #0x78]
  48af7c: e795300b     	ldr	r3, [r5, r11]
  48af80: e59d223c     	ldr	r2, [sp, #0x23c]
  48af84: e1a00004     	mov	r0, r4
  48af88: e5933000     	ldr	r3, [r3]
  48af8c: e1520003     	cmp	r2, r3
  48af90: 1a000051     	bne	0x48b0dc <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x388> @ imm = #0x144
  48af94: e28ddf91     	add	sp, sp, #580
  48af98: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  48af9c: e1a00004     	mov	r0, r4
  48afa0: e5943000     	ldr	r3, [r4]
  48afa4: e1a0e00f     	mov	lr, pc
  48afa8: e593f004     	ldr	pc, [r3, #0x4]
  48afac: e1a04007     	mov	r4, r7
  48afb0: eaffffec     	b	0x48af68 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x214> @ imm = #-0x50
  48afb4: e59fe13c     	ldr	lr, [pc, #0x13c]        @ 0x48b0f8 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x3a4>
  48afb8: e59d1018     	ldr	r1, [sp, #0x18]
  48afbc: e08fe00e     	add	lr, pc, lr
  48afc0: e3510000     	cmp	r1, #0
  48afc4: e58de014     	str	lr, [sp, #0x14]
  48afc8: e1a0200e     	mov	r2, lr
  48afcc: 1a000010     	bne	0x48b014 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x2c0> @ imm = #0x40
  48afd0: e59f1124     	ldr	r1, [pc, #0x124]        @ 0x48b0fc <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x3a8>
  48afd4: e59d301c     	ldr	r3, [sp, #0x1c]
  48afd8: e08f1001     	add	r1, pc, r1
  48afdc: e0837001     	add	r7, r3, r1
  48afe0: e0572002     	subs	r2, r7, r2
  48afe4: 0a000001     	beq	0x48aff0 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x29c> @ imm = #0x4
  48afe8: e1a0000c     	mov	r0, r12
  48afec: ebfa0e1d     	bl	0x30e868 <.plt+0xaf4>   @ imm = #-0x17c78c
  48aff0: e59f3108     	ldr	r3, [pc, #0x108]        @ 0x48b100 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x3ac>
  48aff4: e59dc018     	ldr	r12, [sp, #0x18]
  48aff8: e59d0010     	ldr	r0, [sp, #0x10]
  48affc: e1a01009     	mov	r1, r9
  48b000: e1a02007     	mov	r2, r7
  48b004: e08f3003     	add	r3, pc, r3
  48b008: e58dc000     	str	r12, [sp]
  48b00c: ebfd9ae5     	bl	0x3f1ba8 <_ZNSs9_M_insertEPcPKcS1_b> @ imm = #-0x9946c
  48b010: eaffffc3     	b	0x48af24 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x1d0> @ imm = #-0xf4
  48b014: e15c000e     	cmp	r12, lr
  48b018: 33a03000     	movlo	r3, #0
  48b01c: 23a03001     	movhs	r3, #1
  48b020: e159000e     	cmp	r9, lr
  48b024: 93833001     	orrls	r3, r3, #1
  48b028: e3530000     	cmp	r3, #0
  48b02c: 1affffe7     	bne	0x48afd0 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x27c> @ imm = #-0x64
  48b030: e59de014     	ldr	lr, [sp, #0x14]
  48b034: e15c000e     	cmp	r12, lr
  48b038: 8a000014     	bhi	0x48b090 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x33c> @ imm = #0x50
  48b03c: e59d1014     	ldr	r1, [sp, #0x14]
  48b040: e59d201c     	ldr	r2, [sp, #0x1c]
  48b044: e1a0000c     	mov	r0, r12
  48b048: ebfa0e06     	bl	0x30e868 <.plt+0xaf4>   @ imm = #-0x17c7e8
  48b04c: e59dc014     	ldr	r12, [sp, #0x14]
  48b050: e59d301c     	ldr	r3, [sp, #0x1c]
  48b054: e59d0010     	ldr	r0, [sp, #0x10]
  48b058: e1a01009     	mov	r1, r9
  48b05c: e083200c     	add	r2, r3, r12
  48b060: e1a0300c     	mov	r3, r12
  48b064: e3a0c001     	mov	r12, #1
  48b068: e58dc000     	str	r12, [sp]
  48b06c: ebfd9acd     	bl	0x3f1ba8 <_ZNSs9_M_insertEPcPKcS1_b> @ imm = #-0x994cc
  48b070: eaffffab     	b	0x48af24 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x1d0> @ imm = #-0x154
  48b074: e59f0088     	ldr	r0, [pc, #0x88]         @ 0x48b104 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x3b0>
  48b078: e58dc00c     	str	r12, [sp, #0xc]
  48b07c: e08f0000     	add	r0, pc, r0
  48b080: eb09f78a     	bl	0x708eb0 <___ZSt24__stl_throw_out_of_rangePKc_veneer> @ imm = #0x27de28
  48b084: e5947018     	ldr	r7, [r4, #0x18]
  48b088: e59dc00c     	ldr	r12, [sp, #0xc]
  48b08c: eaffff81     	b	0x48ae98 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x144> @ imm = #-0x1fc
  48b090: e59d301c     	ldr	r3, [sp, #0x1c]
  48b094: e59d0010     	ldr	r0, [sp, #0x10]
  48b098: e1a01009     	mov	r1, r9
  48b09c: e083200e     	add	r2, r3, lr
  48b0a0: e1a0300e     	mov	r3, lr
  48b0a4: e3a0e001     	mov	lr, #1
  48b0a8: e58de000     	str	lr, [sp]
  48b0ac: e58dc00c     	str	r12, [sp, #0xc]
  48b0b0: ebfd9abc     	bl	0x3f1ba8 <_ZNSs9_M_insertEPcPKcS1_b> @ imm = #-0x99510
  48b0b4: e59de014     	ldr	lr, [sp, #0x14]
  48b0b8: e59dc00c     	ldr	r12, [sp, #0xc]
  48b0bc: e5943018     	ldr	r3, [r4, #0x18]
  48b0c0: e067100e     	rsb	r1, r7, lr
  48b0c4: e067000c     	rsb	r0, r7, r12
  48b0c8: e0831001     	add	r1, r3, r1
  48b0cc: e0830000     	add	r0, r3, r0
  48b0d0: e59d201c     	ldr	r2, [sp, #0x1c]
  48b0d4: ebfa0b97     	bl	0x30df38 <.plt+0x1c4>   @ imm = #-0x17d1a4
  48b0d8: eaffff91     	b	0x48af24 <_ZN3rnd8MgxBlock12FromFilenameEPKcS2_S2_+0x1d0> @ imm = #-0x1bc
  48b0dc: ebfa0c8b     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x17cdd4
  48b0e0: 2c 9d 50 00  	.word	0x00509d2c
  48b0e4: ac 40 00 00  	.word	0x000040ac
  48b0e8: f4 37 00 00  	.word	0x000037f4
  48b0ec: cc 9f 44 00  	.word	0x00449fcc
  48b0f0: 00 ba 43 00  	.word	0x0043ba00
  48b0f4: 5c 09 44 00  	.word	0x0044095c
  48b0f8: 4c 08 44 00  	.word	0x0044084c
  48b0fc: 30 08 44 00  	.word	0x00440830
  48b100: 04 08 44 00  	.word	0x00440804
  48b104: dc 33 43 00  	.word	0x004333dc
