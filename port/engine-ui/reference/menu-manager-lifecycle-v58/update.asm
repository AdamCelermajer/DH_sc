
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0042ea04 <_ZN11MenuManager6UpdateEb>:
  42ea04: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  42ea08: e59f4430     	ldr	r4, [pc, #0x430]        @ 0x42ee40 <_ZN11MenuManager6UpdateEb+0x43c>
  42ea0c: e59f3430     	ldr	r3, [pc, #0x430]        @ 0x42ee44 <_ZN11MenuManager6UpdateEb+0x440>
  42ea10: e59f9430     	ldr	r9, [pc, #0x430]        @ 0x42ee48 <_ZN11MenuManager6UpdateEb+0x444>
  42ea14: e08f4004     	add	r4, pc, r4
  42ea18: e7942003     	ldr	r2, [r4, r3]
  42ea1c: e7943009     	ldr	r3, [r4, r9]
  42ea20: e24dd084     	sub	sp, sp, #132
  42ea24: e5d22000     	ldrb	r2, [r2]
  42ea28: e5933000     	ldr	r3, [r3]
  42ea2c: e1a05000     	mov	r5, r0
  42ea30: e3520000     	cmp	r2, #0
  42ea34: e1a0b001     	mov	r11, r1
  42ea38: e58d307c     	str	r3, [sp, #0x7c]
  42ea3c: 1a00007b     	bne	0x42ec30 <_ZN11MenuManager6UpdateEb+0x22c> @ imm = #0x1ec
  42ea40: e59f0404     	ldr	r0, [pc, #0x404]        @ 0x42ee4c <_ZN11MenuManager6UpdateEb+0x448>
  42ea44: e59f6404     	ldr	r6, [pc, #0x404]        @ 0x42ee50 <_ZN11MenuManager6UpdateEb+0x44c>
  42ea48: e08f0000     	add	r0, pc, r0
  42ea4c: ebfb9318     	bl	0x3136b4 <_Z20PushProfilingContextPKc> @ imm = #-0x11b3a0
  42ea50: e7940006     	ldr	r0, [r4, r6]
  42ea54: ebfbc2ce     	bl	0x31f594 <_ZNK11Application15GetCurrentLevelEv> @ imm = #-0x10f4c8
  42ea58: e3500000     	cmp	r0, #0
  42ea5c: 0a000009     	beq	0x42ea88 <_ZN11MenuManager6UpdateEb+0x84> @ imm = #0x24
  42ea60: e59f73ec     	ldr	r7, [pc, #0x3ec]        @ 0x42ee54 <_ZN11MenuManager6UpdateEb+0x450>
  42ea64: e08f7007     	add	r7, pc, r7
  42ea68: e1a00007     	mov	r0, r7
  42ea6c: ebfb9310     	bl	0x3136b4 <_Z20PushProfilingContextPKc> @ imm = #-0x11b3c0
  42ea70: ebffeea1     	bl	0x42a4fc <_ZN12MenuDebugHUD11GetInstanceEv> @ imm = #-0x457c
  42ea74: e5903000     	ldr	r3, [r0]
  42ea78: e1a0e00f     	mov	lr, pc
  42ea7c: e593f01c     	ldr	pc, [r3, #0x1c]
  42ea80: e1a00007     	mov	r0, r7
  42ea84: ebfb930b     	bl	0x3136b8 <_Z19PopProfilingContextPKc> @ imm = #-0x11b3d4
  42ea88: e7943006     	ldr	r3, [r4, r6]
  42ea8c: e3a01000     	mov	r1, #0
  42ea90: e3a02001     	mov	r2, #1
  42ea94: e5930040     	ldr	r0, [r3, #0x40]
  42ea98: ebfcfe76     	bl	0x36e478 <_ZN13PlayerManager14GetLocalPlayerEib> @ imm = #-0xc0628
  42ea9c: e5907660     	ldr	r7, [r0, #0x660]
  42eaa0: e59f03b0     	ldr	r0, [pc, #0x3b0]        @ 0x42ee58 <_ZN11MenuManager6UpdateEb+0x454>
  42eaa4: e08f0000     	add	r0, pc, r0
  42eaa8: ebfb9301     	bl	0x3136b4 <_Z20PushProfilingContextPKc> @ imm = #-0x11b3fc
  42eaac: e3570000     	cmp	r7, #0
  42eab0: 0a00002d     	beq	0x42eb6c <_ZN11MenuManager6UpdateEb+0x168> @ imm = #0xb4
  42eab4: e30134a8     	movw	r3, #0x14a8
  42eab8: e19730d3     	ldrsb	r3, [r7, r3]
  42eabc: e353000a     	cmp	r3, #10
  42eac0: 83a07005     	movhi	r7, #5
  42eac4: 8a000002     	bhi	0x42ead4 <_ZN11MenuManager6UpdateEb+0xd0> @ imm = #0x8
  42eac8: e59f238c     	ldr	r2, [pc, #0x38c]        @ 0x42ee5c <_ZN11MenuManager6UpdateEb+0x458>
  42eacc: e08f2002     	add	r2, pc, r2
  42ead0: e7927103     	ldr	r7, [r2, r3, lsl #2]
  42ead4: e5953108     	ldr	r3, [r5, #0x108]
  42ead8: e1530007     	cmp	r3, r7
  42eadc: 0a000022     	beq	0x42eb6c <_ZN11MenuManager6UpdateEb+0x168> @ imm = #0x88
  42eae0: e1a00005     	mov	r0, r5
  42eae4: ebfff828     	bl	0x42cb8c <_ZN11MenuManager10GetHUDRootEv> @ imm = #-0x1f60
  42eae8: e3500000     	cmp	r0, #0
  42eaec: 0a00001e     	beq	0x42eb6c <_ZN11MenuManager6UpdateEb+0x168> @ imm = #0x78
  42eaf0: e5857108     	str	r7, [r5, #0x108]
  42eaf4: e1a00005     	mov	r0, r5
  42eaf8: ebfff823     	bl	0x42cb8c <_ZN11MenuManager10GetHUDRootEv> @ imm = #-0x1f74
  42eafc: e1a0a000     	mov	r10, r0
  42eb00: e1a00005     	mov	r0, r5
  42eb04: ebfff820     	bl	0x42cb8c <_ZN11MenuManager10GetHUDRootEv> @ imm = #-0x1f80
  42eb08: eb0de467     	bl	0x7a7cac <_ZNK8RenderFX7GetRootEv> @ imm = #0x37919c
  42eb0c: eb0d1590     	bl	0x774154 <_ZN7gameswf4root14get_root_movieEv> @ imm = #0x345640
  42eb10: e3a03000     	mov	r3, #0
  42eb14: e1a08000     	mov	r8, r0
  42eb18: e5950108     	ldr	r0, [r5, #0x108]
  42eb1c: e5cd3014     	strb	r3, [sp, #0x14]
  42eb20: e3a03002     	mov	r3, #2
  42eb24: e5cd3015     	strb	r3, [sp, #0x15]
  42eb28: ebfb8080     	bl	0x30ed30 <.plt+0xfbc>   @ imm = #-0x11fe00
  42eb2c: e1cd02f0     	strd	r0, r1, [sp, #32]
  42eb30: e59dc020     	ldr	r12, [sp, #0x20]
  42eb34: e59f2324     	ldr	r2, [pc, #0x324]        @ 0x42ee60 <_ZN11MenuManager6UpdateEb+0x45c>
  42eb38: e28d7014     	add	r7, sp, #20
  42eb3c: e58dc018     	str	r12, [sp, #0x18]
  42eb40: e59dc024     	ldr	r12, [sp, #0x24]
  42eb44: e1a0000a     	mov	r0, r10
  42eb48: e1a01008     	mov	r1, r8
  42eb4c: e587c008     	str	r12, [r7, #0x8]
  42eb50: e08f2002     	add	r2, pc, r2
  42eb54: e3a0c001     	mov	r12, #1
  42eb58: e1a03007     	mov	r3, r7
  42eb5c: e58dc000     	str	r12, [sp]
  42eb60: eb0df4a9     	bl	0x7abe0c <_ZN8RenderFX16InvokeASCallbackEPN7gameswf9characterEPKcPKNS0_8as_valueEi> @ imm = #0x37d2a4
  42eb64: e1a00007     	mov	r0, r7
  42eb68: eb0da16d     	bl	0x797124 <_ZN7gameswf8as_value9drop_refsEv> @ imm = #0x3685b4
  42eb6c: e59f02f0     	ldr	r0, [pc, #0x2f0]        @ 0x42ee64 <_ZN11MenuManager6UpdateEb+0x460>
  42eb70: e59f82f0     	ldr	r8, [pc, #0x2f0]        @ 0x42ee68 <_ZN11MenuManager6UpdateEb+0x464>
  42eb74: e28d7064     	add	r7, sp, #100
  42eb78: e08f0000     	add	r0, pc, r0
  42eb7c: ebfb92cd     	bl	0x3136b8 <_Z19PopProfilingContextPKc> @ imm = #-0x11b4cc
  42eb80: e3a00000     	mov	r0, #0
  42eb84: eb0de42a     	bl	0x7a7c34 <_ZN8RenderFX12SetWireFrameEb> @ imm = #0x3790a8
  42eb88: e794a008     	ldr	r10, [r4, r8]
  42eb8c: e1a0000a     	mov	r0, r10
  42eb90: ebfc233c     	bl	0x337888 <_ZN13DebugSwitches4loadEv> @ imm = #-0xf7310
  42eb94: e59f12d0     	ldr	r1, [pc, #0x2d0]        @ 0x42ee6c <_ZN11MenuManager6UpdateEb+0x468>
  42eb98: e28d2030     	add	r2, sp, #48
  42eb9c: e1a00007     	mov	r0, r7
  42eba0: e08f1001     	add	r1, pc, r1
  42eba4: ebfb9550     	bl	0x3140ec <_ZNSsC1EPKcRKSaIcE> @ imm = #-0x11aac0
  42eba8: e1a0000a     	mov	r0, r10
  42ebac: e1a01007     	mov	r1, r7
  42ebb0: ebfc23b4     	bl	0x337a88 <_ZN13DebugSwitches9GetSwitchERKSs> @ imm = #-0xf7130
  42ebb4: e1a0a000     	mov	r10, r0
  42ebb8: e1a00007     	mov	r0, r7
  42ebbc: ebfba5a4     	bl	0x318254 <_ZNSsD1Ev>    @ imm = #-0x116970
  42ebc0: e35a0000     	cmp	r10, #0
  42ebc4: 1a000085     	bne	0x42ede0 <_ZN11MenuManager6UpdateEb+0x3dc> @ imm = #0x214
  42ebc8: e7948008     	ldr	r8, [r4, r8]
  42ebcc: e28d704c     	add	r7, sp, #76
  42ebd0: e1a00008     	mov	r0, r8
  42ebd4: ebfc232b     	bl	0x337888 <_ZN13DebugSwitches4loadEv> @ imm = #-0xf7354
  42ebd8: e59f1290     	ldr	r1, [pc, #0x290]        @ 0x42ee70 <_ZN11MenuManager6UpdateEb+0x46c>
  42ebdc: e28d202c     	add	r2, sp, #44
  42ebe0: e1a00007     	mov	r0, r7
  42ebe4: e08f1001     	add	r1, pc, r1
  42ebe8: ebfb953f     	bl	0x3140ec <_ZNSsC1EPKcRKSaIcE> @ imm = #-0x11ab04
  42ebec: e1a00008     	mov	r0, r8
  42ebf0: e1a01007     	mov	r1, r7
  42ebf4: ebfc23a3     	bl	0x337a88 <_ZN13DebugSwitches9GetSwitchERKSs> @ imm = #-0xf7174
  42ebf8: e3500000     	cmp	r0, #0
  42ebfc: 0a00000d     	beq	0x42ec38 <_ZN11MenuManager6UpdateEb+0x234> @ imm = #0x34
  42ec00: e1a00007     	mov	r0, r7
  42ec04: ebfba592     	bl	0x318254 <_ZNSsD1Ev>    @ imm = #-0x1169b8
  42ec08: e59f0264     	ldr	r0, [pc, #0x264]        @ 0x42ee74 <_ZN11MenuManager6UpdateEb+0x470>
  42ec0c: e08f0000     	add	r0, pc, r0
  42ec10: ebfb92a8     	bl	0x3136b8 <_Z19PopProfilingContextPKc> @ imm = #-0x11b560
  42ec14: e7943009     	ldr	r3, [r4, r9]
  42ec18: e59d207c     	ldr	r2, [sp, #0x7c]
  42ec1c: e5933000     	ldr	r3, [r3]
  42ec20: e1520003     	cmp	r2, r3
  42ec24: 1a000084     	bne	0x42ee3c <_ZN11MenuManager6UpdateEb+0x438> @ imm = #0x210
  42ec28: e28dd084     	add	sp, sp, #132
  42ec2c: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  42ec30: eb004c65     	bl	0x441dcc <_Z15fillLeaderBoardv> @ imm = #0x13194
  42ec34: eaffff81     	b	0x42ea40 <_ZN11MenuManager6UpdateEb+0x3c> @ imm = #-0x1fc
  42ec38: e1a00008     	mov	r0, r8
  42ec3c: ebfc2311     	bl	0x337888 <_ZN13DebugSwitches4loadEv> @ imm = #-0xf73bc
  42ec40: e59f1230     	ldr	r1, [pc, #0x230]        @ 0x42ee78 <_ZN11MenuManager6UpdateEb+0x474>
  42ec44: e28da034     	add	r10, sp, #52
  42ec48: e28d2028     	add	r2, sp, #40
  42ec4c: e08f1001     	add	r1, pc, r1
  42ec50: e1a0000a     	mov	r0, r10
  42ec54: ebfb9524     	bl	0x3140ec <_ZNSsC1EPKcRKSaIcE> @ imm = #-0x11ab70
  42ec58: e1a0100a     	mov	r1, r10
  42ec5c: e1a00008     	mov	r0, r8
  42ec60: ebfc2388     	bl	0x337a88 <_ZN13DebugSwitches9GetSwitchERKSs> @ imm = #-0xf71e0
  42ec64: e1a08000     	mov	r8, r0
  42ec68: e1a0000a     	mov	r0, r10
  42ec6c: ebfba578     	bl	0x318254 <_ZNSsD1Ev>    @ imm = #-0x116a20
  42ec70: e1a00007     	mov	r0, r7
  42ec74: ebfba576     	bl	0x318254 <_ZNSsD1Ev>    @ imm = #-0x116a28
  42ec78: e3580000     	cmp	r8, #0
  42ec7c: 1affffe1     	bne	0x42ec08 <_ZN11MenuManager6UpdateEb+0x204> @ imm = #-0x7c
  42ec80: e7940006     	ldr	r0, [r4, r6]
  42ec84: ebfbc278     	bl	0x31f66c <_ZN11Application5GetDtEv> @ imm = #-0x10f620
  42ec88: e5d53088     	ldrb	r3, [r5, #0x88]
  42ec8c: e1a0a000     	mov	r10, r0
  42ec90: e3530000     	cmp	r3, #0
  42ec94: 0a000004     	beq	0x42ecac <_ZN11MenuManager6UpdateEb+0x2a8> @ imm = #0x10
  42ec98: e5953080     	ldr	r3, [r5, #0x80]
  42ec9c: e3530000     	cmp	r3, #0
  42eca0: 1a000051     	bne	0x42edec <_ZN11MenuManager6UpdateEb+0x3e8> @ imm = #0x144
  42eca4: e3a03000     	mov	r3, #0
  42eca8: e5c53088     	strb	r3, [r5, #0x88]
  42ecac: e59f81c8     	ldr	r8, [pc, #0x1c8]        @ 0x42ee7c <_ZN11MenuManager6UpdateEb+0x478>
  42ecb0: e35b0000     	cmp	r11, #0
  42ecb4: 01a0700b     	moveq	r7, r11
  42ecb8: 13a07003     	movne	r7, #3
  42ecbc: e08f8008     	add	r8, pc, r8
  42ecc0: e59530f4     	ldr	r3, [r5, #0xf4]
  42ecc4: e0833107     	add	r3, r3, r7, lsl #2
  42ecc8: e593b134     	ldr	r11, [r3, #0x134]
  42eccc: e35b0000     	cmp	r11, #0
  42ecd0: 0a000012     	beq	0x42ed20 <_ZN11MenuManager6UpdateEb+0x31c> @ imm = #0x48
  42ecd4: e7943006     	ldr	r3, [r4, r6]
  42ecd8: e1a00003     	mov	r0, r3
  42ecdc: e58d300c     	str	r3, [sp, #0xc]
  42ece0: ebfbc22b     	bl	0x31f594 <_ZNK11Application15GetCurrentLevelEv> @ imm = #-0x10f754
  42ece4: e3500000     	cmp	r0, #0
  42ece8: e59d300c     	ldr	r3, [sp, #0xc]
  42ecec: 0a000001     	beq	0x42ecf8 <_ZN11MenuManager6UpdateEb+0x2f4> @ imm = #0x4
  42ecf0: e3570003     	cmp	r7, #3
  42ecf4: 0a000045     	beq	0x42ee10 <_ZN11MenuManager6UpdateEb+0x40c> @ imm = #0x114
  42ecf8: e1a00008     	mov	r0, r8
  42ecfc: ebfb926c     	bl	0x3136b4 <_Z20PushProfilingContextPKc> @ imm = #-0x11b650
  42ed00: e1a0000b     	mov	r0, r11
  42ed04: e59b3000     	ldr	r3, [r11]
  42ed08: e1a0100a     	mov	r1, r10
  42ed0c: e3a02000     	mov	r2, #0
  42ed10: e1a0e00f     	mov	lr, pc
  42ed14: e593f010     	ldr	pc, [r3, #0x10]
  42ed18: e1a00008     	mov	r0, r8
  42ed1c: ebfb9265     	bl	0x3136b8 <_Z19PopProfilingContextPKc> @ imm = #-0x11b66c
  42ed20: e2877001     	add	r7, r7, #1
  42ed24: e3570004     	cmp	r7, #4
  42ed28: 1affffe4     	bne	0x42ecc0 <_ZN11MenuManager6UpdateEb+0x2bc> @ imm = #-0x70
  42ed2c: e59f014c     	ldr	r0, [pc, #0x14c]        @ 0x42ee80 <_ZN11MenuManager6UpdateEb+0x47c>
  42ed30: e08f0000     	add	r0, pc, r0
  42ed34: ebfb925e     	bl	0x3136b4 <_Z20PushProfilingContextPKc> @ imm = #-0x11b688
  42ed38: e7940006     	ldr	r0, [r4, r6]
  42ed3c: ebfbc250     	bl	0x31f684 <_ZNK11Application21IsCurrentlyInGameViewEv> @ imm = #-0x10f6c0
  42ed40: e3500000     	cmp	r0, #0
  42ed44: 1a000037     	bne	0x42ee28 <_ZN11MenuManager6UpdateEb+0x424> @ imm = #0xdc
  42ed48: ebff9450     	bl	0x413e90 <_ZN16FlashAnimManager11GetInstanceEv> @ imm = #-0x1aec0
  42ed4c: ebff92d6     	bl	0x4138ac <_ZN16FlashAnimManager6UpdateEv> @ imm = #-0x1b4a8
  42ed50: e59f012c     	ldr	r0, [pc, #0x12c]        @ 0x42ee84 <_ZN11MenuManager6UpdateEb+0x480>
  42ed54: e08f0000     	add	r0, pc, r0
  42ed58: ebfb9256     	bl	0x3136b8 <_Z19PopProfilingContextPKc> @ imm = #-0x11b6a8
  42ed5c: e59f0124     	ldr	r0, [pc, #0x124]        @ 0x42ee88 <_ZN11MenuManager6UpdateEb+0x484>
  42ed60: e08f0000     	add	r0, pc, r0
  42ed64: ebfb9252     	bl	0x3136b4 <_Z20PushProfilingContextPKc> @ imm = #-0x11b6b8
  42ed68: e1a00005     	mov	r0, r5
  42ed6c: ebfff771     	bl	0x42cb38 <_ZNK11MenuManager11GetNumMenusEv> @ imm = #-0x223c
  42ed70: e2507000     	subs	r7, r0, #0
  42ed74: da000012     	ble	0x42edc4 <_ZN11MenuManager6UpdateEb+0x3c0> @ imm = #0x48
  42ed78: e3a06000     	mov	r6, #0
  42ed7c: ea000002     	b	0x42ed8c <_ZN11MenuManager6UpdateEb+0x388> @ imm = #0x8
  42ed80: e2866001     	add	r6, r6, #1
  42ed84: e1570006     	cmp	r7, r6
  42ed88: 0a00000d     	beq	0x42edc4 <_ZN11MenuManager6UpdateEb+0x3c0> @ imm = #0x34
  42ed8c: e5953064     	ldr	r3, [r5, #0x64]
  42ed90: e7930106     	ldr	r0, [r3, r6, lsl #2]
  42ed94: ebffc196     	bl	0x41f3f4 <_ZNK8MenuBase9IsVisibleEv> @ imm = #-0xf9a8
  42ed98: e3500000     	cmp	r0, #0
  42ed9c: 0afffff7     	beq	0x42ed80 <_ZN11MenuManager6UpdateEb+0x37c> @ imm = #-0x24
  42eda0: e5953064     	ldr	r3, [r5, #0x64]
  42eda4: e7933106     	ldr	r3, [r3, r6, lsl #2]
  42eda8: e2866001     	add	r6, r6, #1
  42edac: e1a00003     	mov	r0, r3
  42edb0: e5933000     	ldr	r3, [r3]
  42edb4: e1a0e00f     	mov	lr, pc
  42edb8: e593f01c     	ldr	pc, [r3, #0x1c]
  42edbc: e1570006     	cmp	r7, r6
  42edc0: 1afffff1     	bne	0x42ed8c <_ZN11MenuManager6UpdateEb+0x388> @ imm = #-0x3c
  42edc4: e59f00c0     	ldr	r0, [pc, #0xc0]         @ 0x42ee8c <_ZN11MenuManager6UpdateEb+0x488>
  42edc8: e08f0000     	add	r0, pc, r0
  42edcc: ebfb9239     	bl	0x3136b8 <_Z19PopProfilingContextPKc> @ imm = #-0x11b71c
  42edd0: e59f00b8     	ldr	r0, [pc, #0xb8]         @ 0x42ee90 <_ZN11MenuManager6UpdateEb+0x48c>
  42edd4: e08f0000     	add	r0, pc, r0
  42edd8: ebfb9236     	bl	0x3136b8 <_Z19PopProfilingContextPKc> @ imm = #-0x11b728
  42eddc: eaffff8c     	b	0x42ec14 <_ZN11MenuManager6UpdateEb+0x210> @ imm = #-0x1d0
  42ede0: e3a00001     	mov	r0, #1
  42ede4: eb0de392     	bl	0x7a7c34 <_ZN8RenderFX12SetWireFrameEb> @ imm = #0x378e48
  42ede8: eaffff76     	b	0x42ebc8 <_ZN11MenuManager6UpdateEb+0x1c4> @ imm = #-0x228
  42edec: e2857070     	add	r7, r5, #112
  42edf0: e1a00007     	mov	r0, r7
  42edf4: e5951074     	ldr	r1, [r5, #0x74]
  42edf8: ebfffb18     	bl	0x42da60 <_ZNSt4priv8_Rb_treeIP8MenuBaseSt4lessIS2_ESt4pairIKS2_bENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE8_M_eraseEPNS_18_Rb_tree_node_baseE> @ imm = #-0x13a0
  42edfc: e585707c     	str	r7, [r5, #0x7c]
  42ee00: e5858080     	str	r8, [r5, #0x80]
  42ee04: e5857078     	str	r7, [r5, #0x78]
  42ee08: e5858074     	str	r8, [r5, #0x74]
  42ee0c: eaffffa4     	b	0x42eca4 <_ZN11MenuManager6UpdateEb+0x2a0> @ imm = #-0x170
  42ee10: e1a00003     	mov	r0, r3
  42ee14: ebfbc1de     	bl	0x31f594 <_ZNK11Application15GetCurrentLevelEv> @ imm = #-0x10f888
  42ee18: e5d03198     	ldrb	r3, [r0, #0x198]
  42ee1c: e3530000     	cmp	r3, #0
  42ee20: 0affffc1     	beq	0x42ed2c <_ZN11MenuManager6UpdateEb+0x328> @ imm = #-0xfc
  42ee24: eaffffb3     	b	0x42ecf8 <_ZN11MenuManager6UpdateEb+0x2f4> @ imm = #-0x134
  42ee28: ebffbfe8     	bl	0x41edd0 <_ZN14InfoHUDManager11GetInstanceEv> @ imm = #-0x10060
  42ee2c: ebffbf73     	bl	0x41ec00 <_ZN14InfoHUDManager6UpdateEv> @ imm = #-0x10234
  42ee30: ebffb0b9     	bl	0x41b11c <_ZN11HUDControls11GetInstanceEv> @ imm = #-0x13d1c
  42ee34: ebffae51     	bl	0x41a780 <_ZN11HUDControls6UpdateEv> @ imm = #-0x146bc
  42ee38: eaffffc2     	b	0x42ed48 <_ZN11MenuManager6UpdateEb+0x344> @ imm = #-0xf8
  42ee3c: ebfb7d33     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x120b34
