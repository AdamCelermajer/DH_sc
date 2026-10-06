
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003ae7d4 <Character::SetFaeryState(unsigned int, int)>:
  3ae7d4: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3ae7d8: e24dd00c     	sub	sp, sp, #12
  3ae7dc: e1a05001     	mov	r5, r1
  3ae7e0: e1a06000     	mov	r6, r0
  3ae7e4: e1a07002     	mov	r7, r2
  3ae7e8: eb00343d     	bl	0x3bb8e4 <Character::SG_GetGameDifficulty()> @ imm = #0xd0f4
  3ae7ec: e1a08000     	mov	r8, r0
  3ae7f0: e1a01008     	mov	r1, r8
  3ae7f4: e1a00006     	mov	r0, r6
  3ae7f8: eb003488     	bl	0x3bba20 <Character::SG_GetFaerieCount(int)> @ imm = #0xd220
  3ae7fc: e59f4124     	ldr	r4, [pc, #0x124]        @ 0x3ae928 <Character::SetFaeryState(unsigned int, int)+0x154>
  3ae800: e1500005     	cmp	r0, r5
  3ae804: e08f4004     	add	r4, pc, r4
  3ae808: 8a000008     	bhi	0x3ae830 <Character::SetFaeryState(unsigned int, int)+0x5c> @ imm = #0x20
  3ae80c: e59f3118     	ldr	r3, [pc, #0x118]        @ 0x3ae92c <Character::SetFaeryState(unsigned int, int)+0x158>
  3ae810: e7943003     	ldr	r3, [r4, r3]
  3ae814: e5933000     	ldr	r3, [r3]
  3ae818: e3530002     	cmp	r3, #2
  3ae81c: 03a03000     	moveq	r3, #0
  3ae820: 05833000     	streq	r3, [r3]
  3ae824: 0a000001     	beq	0x3ae830 <Character::SetFaeryState(unsigned int, int)+0x5c> @ imm = #0x4
  3ae828: e3530001     	cmp	r3, #1
  3ae82c: 0a000030     	beq	0x3ae8f4 <Character::SetFaeryState(unsigned int, int)+0x120> @ imm = #0xc0
  3ae830: e3570001     	cmp	r7, #1
  3ae834: 13a03000     	movne	r3, #0
  3ae838: 03a03001     	moveq	r3, #1
  3ae83c: e3550000     	cmp	r5, #0
  3ae840: 03a03000     	moveq	r3, #0
  3ae844: e3530000     	cmp	r3, #0
  3ae848: 1a000006     	bne	0x3ae868 <Character::SetFaeryState(unsigned int, int)+0x94> @ imm = #0x18
  3ae84c: e1a00006     	mov	r0, r6
  3ae850: e1a01005     	mov	r1, r5
  3ae854: e1a02007     	mov	r2, r7
  3ae858: e1a03008     	mov	r3, r8
  3ae85c: e28dd00c     	add	sp, sp, #12
  3ae860: e8bd4ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3ae864: ea0034b7     	b	0x3bbb48 <Character::SG_SetFaerieState(unsigned int, int, int)> @ imm = #0xd2dc
  3ae868: eb113bc9     	bl	0x7fd794 <GetOnline()>  @ imm = #0x44ef24
  3ae86c: e5d03005     	ldrb	r3, [r0, #0x5]
  3ae870: e3530000     	cmp	r3, #0
  3ae874: 1afffff4     	bne	0x3ae84c <Character::SetFaeryState(unsigned int, int)+0x78> @ imm = #-0x30
  3ae878: e1a00006     	mov	r0, r6
  3ae87c: eb003418     	bl	0x3bb8e4 <Character::SG_GetGameDifficulty()> @ imm = #0xd060
  3ae880: e2509000     	subs	r9, r0, #0
  3ae884: 1afffff0     	bne	0x3ae84c <Character::SetFaeryState(unsigned int, int)+0x78> @ imm = #-0x40
  3ae888: e59fa0a0     	ldr	r10, [pc, #0xa0]        @ 0x3ae930 <Character::SetFaeryState(unsigned int, int)+0x15c>
  3ae88c: e794300a     	ldr	r3, [r4, r10]
  3ae890: e593304c     	ldr	r3, [r3, #0x4c]
  3ae894: e5d3302b     	ldrb	r3, [r3, #0x2b]
  3ae898: e3530000     	cmp	r3, #0
  3ae89c: 0affffea     	beq	0x3ae84c <Character::SetFaeryState(unsigned int, int)+0x78> @ imm = #-0x58
  3ae8a0: e59f308c     	ldr	r3, [pc, #0x8c]         @ 0x3ae934 <Character::SetFaeryState(unsigned int, int)+0x160>
  3ae8a4: e59f108c     	ldr	r1, [pc, #0x8c]         @ 0x3ae938 <Character::SetFaeryState(unsigned int, int)+0x164>
  3ae8a8: e3a02001     	mov	r2, #1
  3ae8ac: e794b003     	ldr	r11, [r4, r3]
  3ae8b0: e08f1001     	add	r1, pc, r1
  3ae8b4: e1a0000b     	mov	r0, r11
  3ae8b8: eb02aa4c     	bl	0x4591f0 <ScriptManager::GetIDFromName(char const*, bool) const> @ imm = #0xaa930
  3ae8bc: e3700001     	cmn	r0, #1
  3ae8c0: e1a01000     	mov	r1, r0
  3ae8c4: 0a000003     	beq	0x3ae8d8 <Character::SetFaeryState(unsigned int, int)+0x104> @ imm = #0xc
  3ae8c8: e1a0000b     	mov	r0, r11
  3ae8cc: e1a03009     	mov	r3, r9
  3ae8d0: e3e02000     	mvn	r2, #0
  3ae8d4: eb02c739     	bl	0x4605c0 <ScriptManager::StartScript(int, int, bool)> @ imm = #0xb1ce4
  3ae8d8: e794300a     	ldr	r3, [r4, r10]
  3ae8dc: e3a01000     	mov	r1, #0
  3ae8e0: e593204c     	ldr	r2, [r3, #0x4c]
  3ae8e4: e5c2102b     	strb	r1, [r2, #0x2b]
  3ae8e8: e593004c     	ldr	r0, [r3, #0x4c]
  3ae8ec: eb02f890     	bl	0x46cb34 <SavegameManager::saveSettings()> @ imm = #0xbe240
  3ae8f0: eaffffd5     	b	0x3ae84c <Character::SetFaeryState(unsigned int, int)+0x78> @ imm = #-0xac
  3ae8f4: e59f0040     	ldr	r0, [pc, #0x40]         @ 0x3ae93c <Character::SetFaeryState(unsigned int, int)+0x168>
  3ae8f8: e59f1040     	ldr	r1, [pc, #0x40]         @ 0x3ae940 <Character::SetFaeryState(unsigned int, int)+0x16c>
  3ae8fc: e59f2040     	ldr	r2, [pc, #0x40]         @ 0x3ae944 <Character::SetFaeryState(unsigned int, int)+0x170>
  3ae900: e7940000     	ldr	r0, [r4, r0]
  3ae904: e59f303c     	ldr	r3, [pc, #0x3c]         @ 0x3ae948 <Character::SetFaeryState(unsigned int, int)+0x174>
  3ae908: e3a0c087     	mov	r12, #135
  3ae90c: e08f1001     	add	r1, pc, r1
  3ae910: e08f2002     	add	r2, pc, r2
  3ae914: e08f3003     	add	r3, pc, r3
  3ae918: e28000a8     	add	r0, r0, #168
  3ae91c: e58dc000     	str	r12, [sp]
  3ae920: ebfd7db7     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xa0924
  3ae924: eaffffc1     	b	0x3ae830 <Character::SetFaeryState(unsigned int, int)+0x5c> @ imm = #-0xfc
  3ae928: 8c 62 5e 00  	.word	0x005e628c
  3ae92c: c0 39 00 00  	.word	0x000039c0
  3ae930: f4 37 00 00  	.word	0x000037f4
  3ae934: 20 1a 00 00  	.word	0x00001a20
  3ae938: b0 4f 51 00  	.word	0x00514fb0
  3ae93c: c0 19 00 00  	.word	0x000019c0
  3ae940: cc fa 50 00  	.word	0x0050facc
  3ae944: c0 4e 51 00  	.word	0x00514ec0
  3ae948: e4 4e 51 00  	.word	0x00514ee4
