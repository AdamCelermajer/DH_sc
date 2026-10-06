
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f059c <Level::QuickSave(bool)>:
  3f059c: e59f30e4     	ldr	r3, [pc, #0xe4]         @ 0x3f0688 <Level::QuickSave(bool)+0xec>
  3f05a0: e59f20e4     	ldr	r2, [pc, #0xe4]         @ 0x3f068c <Level::QuickSave(bool)+0xf0>
  3f05a4: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3f05a8: e08f3003     	add	r3, pc, r3
  3f05ac: e7935002     	ldr	r5, [r3, r2]
  3f05b0: e1a04000     	mov	r4, r0
  3f05b4: e1a06001     	mov	r6, r1
  3f05b8: e3a02001     	mov	r2, #1
  3f05bc: e5950040     	ldr	r0, [r5, #0x40]
  3f05c0: e3a01000     	mov	r1, #0
  3f05c4: ebfdf7ab     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x82154
  3f05c8: e59430ec     	ldr	r3, [r4, #0xec]
  3f05cc: e5907660     	ldr	r7, [r0, #0x660]
  3f05d0: e3530000     	cmp	r3, #0
  3f05d4: 0a000002     	beq	0x3f05e4 <Level::QuickSave(bool)+0x48> @ imm = #0x8
  3f05d8: e5943130     	ldr	r3, [r4, #0x130]
  3f05dc: e3530026     	cmp	r3, #38
  3f05e0: 0a000000     	beq	0x3f05e8 <Level::QuickSave(bool)+0x4c> @ imm = #0x0
  3f05e4: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3f05e8: e3570000     	cmp	r7, #0
  3f05ec: 0afffffc     	beq	0x3f05e4 <Level::QuickSave(bool)+0x48> @ imm = #-0x10
  3f05f0: e5973000     	ldr	r3, [r7]
  3f05f4: e1a00007     	mov	r0, r7
  3f05f8: e1a0e00f     	mov	lr, pc
  3f05fc: e593f034     	ldr	pc, [r3, #0x34]
  3f0600: e3500000     	cmp	r0, #0
  3f0604: 1afffff6     	bne	0x3f05e4 <Level::QuickSave(bool)+0x48> @ imm = #-0x28
  3f0608: eb103461     	bl	0x7fd794 <GetOnline()>  @ imm = #0x40d184
  3f060c: e5d03005     	ldrb	r3, [r0, #0x5]
  3f0610: e3530000     	cmp	r3, #0
  3f0614: 1a000012     	bne	0x3f0664 <Level::QuickSave(bool)+0xc8> @ imm = #0x48
  3f0618: e5970168     	ldr	r0, [r7, #0x168]
  3f061c: e5971160     	ldr	r1, [r7, #0x160]
  3f0620: e5972164     	ldr	r2, [r7, #0x164]
  3f0624: e3013470     	movw	r3, #0x1470
  3f0628: e7870003     	str	r0, [r7, r3]
  3f062c: e3013468     	movw	r3, #0x1468
  3f0630: e7871003     	str	r1, [r7, r3]
  3f0634: e301346c     	movw	r3, #0x146c
  3f0638: e7872003     	str	r2, [r7, r3]
  3f063c: e59400ec     	ldr	r0, [r4, #0xec]
  3f0640: e3560000     	cmp	r6, #0
  3f0644: 13a03000     	movne	r3, #0
  3f0648: e5d05039     	ldrb	r5, [r0, #0x39]
  3f064c: 15c03039     	strbne	r3, [r0, #0x39]
  3f0650: 159400ec     	ldrne	r0, [r4, #0xec]
  3f0654: eb01c3e4     	bl	0x4615ec <LevelSavegame::Save()> @ imm = #0x70f90
  3f0658: e59430ec     	ldr	r3, [r4, #0xec]
  3f065c: e5c35039     	strb	r5, [r3, #0x39]
  3f0660: eaffffdf     	b	0x3f05e4 <Level::QuickSave(bool)+0x48> @ imm = #-0x84
  3f0664: e5950040     	ldr	r0, [r5, #0x40]
  3f0668: ebfdfa81     	bl	0x36f074 <PlayerManager::IsLocalPlayerHosting()> @ imm = #-0x815fc
  3f066c: e3500000     	cmp	r0, #0
  3f0670: 0affffdb     	beq	0x3f05e4 <Level::QuickSave(bool)+0x48> @ imm = #-0x94
  3f0674: e5953040     	ldr	r3, [r5, #0x40]
  3f0678: e5d33719     	ldrb	r3, [r3, #0x719]
  3f067c: e3530000     	cmp	r3, #0
  3f0680: 1affffd7     	bne	0x3f05e4 <Level::QuickSave(bool)+0x48> @ imm = #-0xa4
  3f0684: eaffffe3     	b	0x3f0618 <Level::QuickSave(bool)+0x7c> @ imm = #-0x74
  3f0688: e8 44 5a 00  	.word	0x005a44e8
  3f068c: f4 37 00 00  	.word	0x000037f4
