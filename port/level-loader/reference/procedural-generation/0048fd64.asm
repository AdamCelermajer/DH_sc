
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048fd64 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_>:
  48fd64: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  48fd68: e59fc584     	ldr	r12, [pc, #0x584]       @ 0x4902f4 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x590>
  48fd6c: e59fe584     	ldr	lr, [pc, #0x584]        @ 0x4902f8 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x594>
  48fd70: e24ddfa3     	sub	sp, sp, #652
  48fd74: e08fc00c     	add	r12, pc, r12
  48fd78: e58dc020     	str	r12, [sp, #0x20]
  48fd7c: e79cc00e     	ldr	r12, [r12, lr]
  48fd80: e58de030     	str	lr, [sp, #0x30]
  48fd84: e1a04000     	mov	r4, r0
  48fd88: e594e02c     	ldr	lr, [r4, #0x2c]
  48fd8c: e5900028     	ldr	r0, [r0, #0x28]
  48fd90: e59cc000     	ldr	r12, [r12]
  48fd94: e58d101c     	str	r1, [sp, #0x1c]
  48fd98: e150000e     	cmp	r0, lr
  48fd9c: e58dc284     	str	r12, [sp, #0x284]
  48fda0: e1a0b002     	mov	r11, r2
  48fda4: e58d3028     	str	r3, [sp, #0x28]
  48fda8: ba000012     	blt	0x48fdf8 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x94> @ imm = #0x48
  48fdac: e5913030     	ldr	r3, [r1, #0x30]
  48fdb0: e593205c     	ldr	r2, [r3, #0x5c]
  48fdb4: e3520002     	cmp	r2, #2
  48fdb8: 0a000127     	beq	0x49025c <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x4f8> @ imm = #0x49c
  48fdbc: e3a02000     	mov	r2, #0
  48fdc0: e1a00004     	mov	r0, r4
  48fdc4: e59d101c     	ldr	r1, [sp, #0x1c]
  48fdc8: ebfffee1     	bl	0x48f954 <_ZN3rnd4Rule4Impl4StepEPNS_4TileEPKNS_4ExitE> @ imm = #-0x47c
  48fdcc: e1a04000     	mov	r4, r0
  48fdd0: e59d0030     	ldr	r0, [sp, #0x30]
  48fdd4: e59d1020     	ldr	r1, [sp, #0x20]
  48fdd8: e59d2284     	ldr	r2, [sp, #0x284]
  48fddc: e7913000     	ldr	r3, [r1, r0]
  48fde0: e1a00004     	mov	r0, r4
  48fde4: e5933000     	ldr	r3, [r3]
  48fde8: e1520003     	cmp	r2, r3
  48fdec: 1a00013f     	bne	0x4902f0 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x58c> @ imm = #0x4fc
  48fdf0: e28ddfa3     	add	sp, sp, #652
  48fdf4: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  48fdf8: e5922014     	ldr	r2, [r2, #0x14]
  48fdfc: e3500000     	cmp	r0, #0
  48fe00: e59f34f4     	ldr	r3, [pc, #0x4f4]        @ 0x4902fc <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x598>
  48fe04: e59d0020     	ldr	r0, [sp, #0x20]
  48fe08: e5922000     	ldr	r2, [r2]
  48fe0c: e7903003     	ldr	r3, [r0, r3]
  48fe10: e7932102     	ldr	r2, [r3, r2, lsl #2]
  48fe14: e58d202c     	str	r2, [sp, #0x2c]
  48fe18: 15948044     	ldrne	r8, [r4, #0x44]
  48fe1c: 0a0000ea     	beq	0x4901cc <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x468> @ imm = #0x3a8
  48fe20: e59ba028     	ldr	r10, [r11, #0x28]
  48fe24: e3a05000     	mov	r5, #0
  48fe28: e58d5038     	str	r5, [sp, #0x38]
  48fe2c: e15a0005     	cmp	r10, r5
  48fe30: e58d503c     	str	r5, [sp, #0x3c]
  48fe34: e58d5040     	str	r5, [sp, #0x40]
  48fe38: da000121     	ble	0x4902c4 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x560> @ imm = #0x484
  48fe3c: e28d00f0     	add	r0, sp, #240
  48fe40: e28de09c     	add	lr, sp, #156
  48fe44: e3e01000     	mvn	r1, #0
  48fe48: e28d2038     	add	r2, sp, #56
  48fe4c: e28d3f8d     	add	r3, sp, #564
  48fe50: e280c004     	add	r12, r0, #4
  48fe54: e58de014     	str	lr, [sp, #0x14]
  48fe58: e58d0024     	str	r0, [sp, #0x24]
  48fe5c: e1a0600b     	mov	r6, r11
  48fe60: e58d1034     	str	r1, [sp, #0x34]
  48fe64: e58d2008     	str	r2, [sp, #0x8]
  48fe68: e28d7f79     	add	r7, sp, #484
  48fe6c: e28e9004     	add	r9, lr, #4
  48fe70: e58d3010     	str	r3, [sp, #0x10]
  48fe74: e58dc018     	str	r12, [sp, #0x18]
  48fe78: e58d400c     	str	r4, [sp, #0xc]
  48fe7c: ea000014     	b	0x48fed4 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x170> @ imm = #0x50
  48fe80: e596402c     	ldr	r4, [r6, #0x2c]
  48fe84: e1a00007     	mov	r0, r7
  48fe88: ebfff87c     	bl	0x48e080 <_ZN3rnd8ListElemC1Ev> @ imm = #-0x1e10
  48fe8c: e1a01007     	mov	r1, r7
  48fe90: e1a00009     	mov	r0, r9
  48fe94: e58d409c     	str	r4, [sp, #0x9c]
  48fe98: ebfff8f8     	bl	0x48e280 <_ZN3rnd8ListElemC1ERKS0_> @ imm = #-0x1c20
  48fe9c: e59d1014     	ldr	r1, [sp, #0x14]
  48fea0: e59d0008     	ldr	r0, [sp, #0x8]
  48fea4: ebfffc1f     	bl	0x48ef28 <_ZNSt6vectorISt4pairIPKN3rnd4ExitENS1_8ListElemEESaIS6_EE9push_backERKS6_> @ imm = #-0xf84
  48fea8: e1a00009     	mov	r0, r9
  48feac: ebffe0c7     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x7ce4
  48feb0: e1a00007     	mov	r0, r7
  48feb4: ebffe0c5     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x7cec
  48feb8: e59d000c     	ldr	r0, [sp, #0xc]
  48febc: e59ba028     	ldr	r10, [r11, #0x28]
  48fec0: e5908044     	ldr	r8, [r0, #0x44]
  48fec4: e2855001     	add	r5, r5, #1
  48fec8: e15a0005     	cmp	r10, r5
  48fecc: e2866004     	add	r6, r6, #4
  48fed0: da000022     	ble	0x48ff60 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x1fc> @ imm = #0x88
  48fed4: e598306c     	ldr	r3, [r8, #0x6c]
  48fed8: e3730001     	cmn	r3, #1
  48fedc: 1affffe7     	bne	0x48fe80 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x11c> @ imm = #-0x64
  48fee0: e596402c     	ldr	r4, [r6, #0x2c]
  48fee4: e59de01c     	ldr	lr, [sp, #0x1c]
  48fee8: e5941004     	ldr	r1, [r4, #0x4]
  48feec: e59e3030     	ldr	r3, [lr, #0x30]
  48fef0: e5932014     	ldr	r2, [r3, #0x14]
  48fef4: e5930018     	ldr	r0, [r3, #0x18]
  48fef8: e5913014     	ldr	r3, [r1, #0x14]
  48fefc: e5911018     	ldr	r1, [r1, #0x18]
  48ff00: e0602002     	rsb	r2, r0, r2
  48ff04: e0613003     	rsb	r3, r1, r3
  48ff08: e1520003     	cmp	r2, r3
  48ff0c: 0a0000a2     	beq	0x49019c <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x438> @ imm = #0x288
  48ff10: e59d0010     	ldr	r0, [sp, #0x10]
  48ff14: ebfff859     	bl	0x48e080 <_ZN3rnd8ListElemC1Ev> @ imm = #-0x1e9c
  48ff18: e59d1010     	ldr	r1, [sp, #0x10]
  48ff1c: e59d0018     	ldr	r0, [sp, #0x18]
  48ff20: e58d40f0     	str	r4, [sp, #0xf0]
  48ff24: ebfff8d5     	bl	0x48e280 <_ZN3rnd8ListElemC1ERKS0_> @ imm = #-0x1cac
  48ff28: e59d1024     	ldr	r1, [sp, #0x24]
  48ff2c: e59d0008     	ldr	r0, [sp, #0x8]
  48ff30: ebfffbfc     	bl	0x48ef28 <_ZNSt6vectorISt4pairIPKN3rnd4ExitENS1_8ListElemEESaIS6_EE9push_backERKS6_> @ imm = #-0x1010
  48ff34: e59d0018     	ldr	r0, [sp, #0x18]
  48ff38: ebffe0a4     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x7d70
  48ff3c: e59d0010     	ldr	r0, [sp, #0x10]
  48ff40: ebffe0a2     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x7d78
  48ff44: e59ba028     	ldr	r10, [r11, #0x28]
  48ff48: e59d200c     	ldr	r2, [sp, #0xc]
  48ff4c: e2855001     	add	r5, r5, #1
  48ff50: e15a0005     	cmp	r10, r5
  48ff54: e5928044     	ldr	r8, [r2, #0x44]
  48ff58: e2866004     	add	r6, r6, #4
  48ff5c: caffffdc     	bgt	0x48fed4 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x170> @ imm = #-0x90
  48ff60: e59d400c     	ldr	r4, [sp, #0xc]
  48ff64: e5981068     	ldr	r1, [r8, #0x68]
  48ff68: e28d2fa2     	add	r2, sp, #648
  48ff6c: e5943000     	ldr	r3, [r4]
  48ff70: e1a00004     	mov	r0, r4
  48ff74: e5221244     	str	r1, [r2, #-0x244]!
  48ff78: e59d1008     	ldr	r1, [sp, #0x8]
  48ff7c: e1a0e00f     	mov	lr, pc
  48ff80: e593f008     	ldr	pc, [r3, #0x8]
  48ff84: e59d1034     	ldr	r1, [sp, #0x34]
  48ff88: e3710001     	cmn	r1, #1
  48ff8c: 0a000036     	beq	0x49006c <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x308> @ imm = #0xd8
  48ff90: e59d7044     	ldr	r7, [sp, #0x44]
  48ff94: e597201c     	ldr	r2, [r7, #0x1c]
  48ff98: e5973020     	ldr	r3, [r7, #0x20]
  48ff9c: e0623003     	rsb	r3, r2, r3
  48ffa0: e1a03243     	asr	r3, r3, #4
  48ffa4: e0831083     	add	r1, r3, r3, lsl #1
  48ffa8: e0811201     	add	r1, r1, r1, lsl #4
  48ffac: e0811401     	add	r1, r1, r1, lsl #8
  48ffb0: e0811801     	add	r1, r1, r1, lsl #16
  48ffb4: e0833101     	add	r3, r3, r1, lsl #2
  48ffb8: e3530000     	cmp	r3, #0
  48ffbc: 0a00002a     	beq	0x49006c <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x308> @ imm = #0xa8
  48ffc0: e59dc034     	ldr	r12, [sp, #0x34]
  48ffc4: e3a06000     	mov	r6, #0
  48ffc8: e28def65     	add	lr, sp, #404
  48ffcc: e08b310c     	add	r3, r11, r12, lsl #2
  48ffd0: e28d0048     	add	r0, sp, #72
  48ffd4: e28d104c     	add	r1, sp, #76
  48ffd8: e58d400c     	str	r4, [sp, #0xc]
  48ffdc: e283902c     	add	r9, r3, #44
  48ffe0: e1a05006     	mov	r5, r6
  48ffe4: e58de010     	str	lr, [sp, #0x10]
  48ffe8: e58d0018     	str	r0, [sp, #0x18]
  48ffec: e58d1014     	str	r1, [sp, #0x14]
  48fff0: e1a04006     	mov	r4, r6
  48fff4: e599a000     	ldr	r10, [r9]
  48fff8: e3a03050     	mov	r3, #80
  48fffc: e0242493     	mla	r4, r3, r4, r2
  490000: e59a3004     	ldr	r3, [r10, #0x4]
  490004: e5940018     	ldr	r0, [r4, #0x18]
  490008: e5948014     	ldr	r8, [r4, #0x14]
  49000c: e5936014     	ldr	r6, [r3, #0x14]
  490010: e5931018     	ldr	r1, [r3, #0x18]
  490014: e0608008     	rsb	r8, r0, r8
  490018: e0616006     	rsb	r6, r1, r6
  49001c: e1560008     	cmp	r6, r8
  490020: b1a02006     	movlt	r2, r6
  490024: a1a02008     	movge	r2, r8
  490028: ebf9f96c     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0x181a50
  49002c: e3500000     	cmp	r0, #0
  490030: 0a000090     	beq	0x490278 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x514> @ imm = #0x240
  490034: e597201c     	ldr	r2, [r7, #0x1c]
  490038: e5973020     	ldr	r3, [r7, #0x20]
  49003c: e2855001     	add	r5, r5, #1
  490040: e1a04005     	mov	r4, r5
  490044: e0623003     	rsb	r3, r2, r3
  490048: e1a03243     	asr	r3, r3, #4
  49004c: e0831083     	add	r1, r3, r3, lsl #1
  490050: e0811201     	add	r1, r1, r1, lsl #4
  490054: e0811401     	add	r1, r1, r1, lsl #8
  490058: e0811801     	add	r1, r1, r1, lsl #16
  49005c: e0831101     	add	r1, r3, r1, lsl #2
  490060: e1510005     	cmp	r1, r5
  490064: 8affffe2     	bhi	0x48fff4 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x290> @ imm = #-0x78
  490068: e59d400c     	ldr	r4, [sp, #0xc]
  49006c: e59d5038     	ldr	r5, [sp, #0x38]
  490070: e59de03c     	ldr	lr, [sp, #0x3c]
  490074: e155000e     	cmp	r5, lr
  490078: 0a000043     	beq	0x49018c <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x428> @ imm = #0x10c
  49007c: e59dc02c     	ldr	r12, [sp, #0x2c]
  490080: e59fa278     	ldr	r10, [pc, #0x278]       @ 0x490300 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x59c>
  490084: e28d8f51     	add	r8, sp, #324
  490088: e1a0920c     	lsl	r9, r12, #4
  49008c: e5940048     	ldr	r0, [r4, #0x48]
  490090: e5956000     	ldr	r6, [r5]
  490094: e3500004     	cmp	r0, #4
  490098: 0a000011     	beq	0x4900e4 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x380> @ imm = #0x44
  49009c: e5962004     	ldr	r2, [r6, #0x4]
  4900a0: e592c05c     	ldr	r12, [r2, #0x5c]
  4900a4: e35c0000     	cmp	r12, #0
  4900a8: da00000d     	ble	0x4900e4 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x380> @ imm = #0x34
  4900ac: e5923074     	ldr	r3, [r2, #0x74]
  4900b0: e5933000     	ldr	r3, [r3]
  4900b4: e1530000     	cmp	r3, r0
  4900b8: 13a03000     	movne	r3, #0
  4900bc: 1a000005     	bne	0x4900d8 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x374> @ imm = #0x14
  4900c0: ea000053     	b	0x490214 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x4b0> @ imm = #0x14c
  4900c4: e59211a0     	ldr	r1, [r2, #0x1a0]
  4900c8: e2822f4b     	add	r2, r2, #300
  4900cc: e5911000     	ldr	r1, [r1]
  4900d0: e1510000     	cmp	r1, r0
  4900d4: 0a00004e     	beq	0x490214 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x4b0> @ imm = #0x138
  4900d8: e2833001     	add	r3, r3, #1
  4900dc: e153000c     	cmp	r3, r12
  4900e0: 1afffff7     	bne	0x4900c4 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x360> @ imm = #-0x24
  4900e4: e2851004     	add	r1, r5, #4
  4900e8: e1a00008     	mov	r0, r8
  4900ec: ebfff863     	bl	0x48e280 <_ZN3rnd8ListElemC1ERKS0_> @ imm = #-0x1e74
  4900f0: e59d001c     	ldr	r0, [sp, #0x1c]
  4900f4: e59d1028     	ldr	r1, [sp, #0x28]
  4900f8: e1a0200b     	mov	r2, r11
  4900fc: e1a03006     	mov	r3, r6
  490100: e58d8000     	str	r8, [sp]
  490104: eb000669     	bl	0x491ab0 <_ZN3rnd4Tile8TrySpawnEPKNS_4ExitES3_S3_RNS_8ListElemE> @ imm = #0x19a4
  490108: e2507000     	subs	r7, r0, #0
  49010c: 0a000018     	beq	0x490174 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x410> @ imm = #0x60
  490110: e5973030     	ldr	r3, [r7, #0x30]
  490114: e593305c     	ldr	r3, [r3, #0x5c]
  490118: e3530001     	cmp	r3, #1
  49011c: 0a00004a     	beq	0x49024c <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x4e8> @ imm = #0x128
  490120: e5943028     	ldr	r3, [r4, #0x28]
  490124: e2833001     	add	r3, r3, #1
  490128: e5843028     	str	r3, [r4, #0x28]
  49012c: e5973030     	ldr	r3, [r7, #0x30]
  490130: e593205c     	ldr	r2, [r3, #0x5c]
  490134: e3520002     	cmp	r2, #2
  490138: 0a00001c     	beq	0x4901b0 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x44c> @ imm = #0x70
  49013c: e3a02000     	mov	r2, #0
  490140: e1a03006     	mov	r3, r6
  490144: e594c000     	ldr	r12, [r4]
  490148: e1a00004     	mov	r0, r4
  49014c: e1a01007     	mov	r1, r7
  490150: e1a0e00f     	mov	lr, pc
  490154: e59cf00c     	ldr	pc, [r12, #0xc]
  490158: e3500000     	cmp	r0, #0
  49015c: 1a00003a     	bne	0x49024c <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x4e8> @ imm = #0xe8
  490160: e5943028     	ldr	r3, [r4, #0x28]
  490164: e1a00007     	mov	r0, r7
  490168: e2433001     	sub	r3, r3, #1
  49016c: e5843028     	str	r3, [r4, #0x28]
  490170: eb0005cc     	bl	0x4918a8 <_ZN3rnd4Tile7UnspawnEv> @ imm = #0x1730
  490174: e1a00008     	mov	r0, r8
  490178: ebffe014     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x7fb0
  49017c: e59de03c     	ldr	lr, [sp, #0x3c]
  490180: e2855054     	add	r5, r5, #84
  490184: e155000e     	cmp	r5, lr
  490188: 1affffbf     	bne	0x49008c <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x328> @ imm = #-0x104
  49018c: e3a04000     	mov	r4, #0
  490190: e59d0008     	ldr	r0, [sp, #0x8]
  490194: ebfff5d7     	bl	0x48d8f8 <_ZNSt6vectorISt4pairIPKN3rnd4ExitENS1_8ListElemEESaIS6_EED1Ev> @ imm = #-0x28a4
  490198: eaffff0c     	b	0x48fdd0 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x6c> @ imm = #-0x3d0
  49019c: ebf9f90f     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0x181bc4
  4901a0: e3500000     	cmp	r0, #0
  4901a4: 058d5034     	streq	r5, [sp, #0x34]
  4901a8: 1affff58     	bne	0x48ff10 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x1ac> @ imm = #-0x2a0
  4901ac: eaffff44     	b	0x48fec4 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x160> @ imm = #-0x2f0
  4901b0: e2832060     	add	r2, r3, #96
  4901b4: e1560002     	cmp	r6, r2
  4901b8: 1affffe0     	bne	0x490140 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x3dc> @ imm = #-0x80
  4901bc: e2832f63     	add	r2, r3, #396
  4901c0: e1560002     	cmp	r6, r2
  4901c4: 0affffdc     	beq	0x49013c <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x3d8> @ imm = #-0x90
  4901c8: eaffffdc     	b	0x490140 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x3dc> @ imm = #-0x90
  4901cc: e5948044     	ldr	r8, [r4, #0x44]
  4901d0: e5d8308c     	ldrb	r3, [r8, #0x8c]
  4901d4: e3530000     	cmp	r3, #0
  4901d8: 0affff10     	beq	0x48fe20 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0xbc> @ imm = #-0x3c0
  4901dc: e59d1020     	ldr	r1, [sp, #0x20]
  4901e0: e59f3118     	ldr	r3, [pc, #0x118]        @ 0x490300 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x59c>
  4901e4: e59dc02c     	ldr	r12, [sp, #0x2c]
  4901e8: e7912003     	ldr	r2, [r1, r3]
  4901ec: e082320c     	add	r3, r2, r12, lsl #4
  4901f0: e7d2020c     	ldrb	r0, [r2, r12, lsl #4]
  4901f4: e5d3c001     	ldrb	r12, [r3, #0x1]
  4901f8: e5d31002     	ldrb	r1, [r3, #0x2]
  4901fc: e5d32003     	ldrb	r2, [r3, #0x3]
  490200: e180340c     	orr	r3, r0, r12, lsl #8
  490204: e1833801     	orr	r3, r3, r1, lsl #16
  490208: e1833c02     	orr	r3, r3, r2, lsl #24
  49020c: e5843048     	str	r3, [r4, #0x48]
  490210: eaffff02     	b	0x48fe20 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0xbc> @ imm = #-0x3f8
  490214: e59d1020     	ldr	r1, [sp, #0x20]
  490218: e59dc02c     	ldr	r12, [sp, #0x2c]
  49021c: e791200a     	ldr	r2, [r1, r10]
  490220: e0823009     	add	r3, r2, r9
  490224: e7d2120c     	ldrb	r1, [r2, r12, lsl #4]
  490228: e5d3c001     	ldrb	r12, [r3, #0x1]
  49022c: e5d32002     	ldrb	r2, [r3, #0x2]
  490230: e5d33003     	ldrb	r3, [r3, #0x3]
  490234: e181140c     	orr	r1, r1, r12, lsl #8
  490238: e1811802     	orr	r1, r1, r2, lsl #16
  49023c: e1811c03     	orr	r1, r1, r3, lsl #24
  490240: e1510000     	cmp	r1, r0
  490244: 1affffcd     	bne	0x490180 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x41c> @ imm = #-0xcc
  490248: eaffffa5     	b	0x4900e4 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x380> @ imm = #-0x16c
  49024c: e1a00008     	mov	r0, r8
  490250: ebffdfde     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x8088
  490254: e3a04001     	mov	r4, #1
  490258: eaffffcc     	b	0x490190 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x42c> @ imm = #-0xd0
  49025c: e2832060     	add	r2, r3, #96
  490260: e15b0002     	cmp	r11, r2
  490264: 1afffed5     	bne	0x48fdc0 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x5c> @ imm = #-0x4ac
  490268: e2832f63     	add	r2, r3, #396
  49026c: e15b0002     	cmp	r11, r2
  490270: 0afffed1     	beq	0x48fdbc <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x58> @ imm = #-0x4bc
  490274: eafffed1     	b	0x48fdc0 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x5c> @ imm = #-0x4bc
  490278: e1580006     	cmp	r8, r6
  49027c: baffff6c     	blt	0x490034 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x2d0> @ imm = #-0x250
  490280: caffff6b     	bgt	0x490034 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x2d0> @ imm = #-0x254
  490284: e1a01004     	mov	r1, r4
  490288: e59d0010     	ldr	r0, [sp, #0x10]
  49028c: ebfff7fb     	bl	0x48e280 <_ZN3rnd8ListElemC1ERKS0_> @ imm = #-0x2014
  490290: e59d1010     	ldr	r1, [sp, #0x10]
  490294: e59d0014     	ldr	r0, [sp, #0x14]
  490298: e58da048     	str	r10, [sp, #0x48]
  49029c: ebfff7f7     	bl	0x48e280 <_ZN3rnd8ListElemC1ERKS0_> @ imm = #-0x2024
  4902a0: e59d1018     	ldr	r1, [sp, #0x18]
  4902a4: e59d0008     	ldr	r0, [sp, #0x8]
  4902a8: ebfffb1e     	bl	0x48ef28 <_ZNSt6vectorISt4pairIPKN3rnd4ExitENS1_8ListElemEESaIS6_EE9push_backERKS6_> @ imm = #-0x1388
  4902ac: e59d0014     	ldr	r0, [sp, #0x14]
  4902b0: ebffdfc6     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x80e8
  4902b4: e59d0010     	ldr	r0, [sp, #0x10]
  4902b8: ebffdfc4     	bl	0x4881d0 <_ZN3rnd8ListElemD1Ev> @ imm = #-0x80f0
  4902bc: e59d7044     	ldr	r7, [sp, #0x44]
  4902c0: eaffff5b     	b	0x490034 <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x2d0> @ imm = #-0x294
  4902c4: e5982068     	ldr	r2, [r8, #0x68]
  4902c8: e28d3038     	add	r3, sp, #56
  4902cc: e58d3008     	str	r3, [sp, #0x8]
  4902d0: e5943000     	ldr	r3, [r4]
  4902d4: e1a00004     	mov	r0, r4
  4902d8: e58d2044     	str	r2, [sp, #0x44]
  4902dc: e59d1008     	ldr	r1, [sp, #0x8]
  4902e0: e28d2044     	add	r2, sp, #68
  4902e4: e1a0e00f     	mov	lr, pc
  4902e8: e593f008     	ldr	pc, [r3, #0x8]
  4902ec: eaffff5e     	b	0x49006c <_ZN3rnd4Path4Impl7OneStepEPNS_4TileEPKNS_4ExitES6_+0x308> @ imm = #-0x288
  4902f0: ebf9f806     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x181fe8
  4902f4: 1c 4d 50 00  	.word	0x00504d1c
  4902f8: ac 40 00 00  	.word	0x000040ac
  4902fc: b8 1b 00 00  	.word	0x00001bb8
  490300: fc 43 00 00  	.word	0x000043fc
