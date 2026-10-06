
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003efaec <Level::SG_SavePlayer(int, bool)>:
  3efaec: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3efaf0: e59f4120     	ldr	r4, [pc, #0x120]        @ 0x3efc18 <Level::SG_SavePlayer(int, bool)+0x12c>
  3efaf4: e59f3120     	ldr	r3, [pc, #0x120]        @ 0x3efc1c <Level::SG_SavePlayer(int, bool)+0x130>
  3efaf8: e2515000     	subs	r5, r1, #0
  3efafc: e08f4004     	add	r4, pc, r4
  3efb00: e7943003     	ldr	r3, [r4, r3]
  3efb04: e24dd008     	sub	sp, sp, #8
  3efb08: e1a08000     	mov	r8, r0
  3efb0c: e1a07002     	mov	r7, r2
  3efb10: e5936040     	ldr	r6, [r3, #0x40]
  3efb14: ba00001c     	blt	0x3efb8c <Level::SG_SavePlayer(int, bool)+0xa0> @ imm = #0x70
  3efb18: e59636c4     	ldr	r3, [r6, #0x6c4]
  3efb1c: e1550003     	cmp	r5, r3
  3efb20: ba000008     	blt	0x3efb48 <Level::SG_SavePlayer(int, bool)+0x5c> @ imm = #0x20
  3efb24: e59f30f4     	ldr	r3, [pc, #0xf4]         @ 0x3efc20 <Level::SG_SavePlayer(int, bool)+0x134>
  3efb28: e7943003     	ldr	r3, [r4, r3]
  3efb2c: e5933000     	ldr	r3, [r3]
  3efb30: e3530002     	cmp	r3, #2
  3efb34: 03a03000     	moveq	r3, #0
  3efb38: 05833000     	streq	r3, [r3]
  3efb3c: 0a000001     	beq	0x3efb48 <Level::SG_SavePlayer(int, bool)+0x5c> @ imm = #0x4
  3efb40: e3530001     	cmp	r3, #1
  3efb44: 0a000026     	beq	0x3efbe4 <Level::SG_SavePlayer(int, bool)+0xf8> @ imm = #0x98
  3efb48: e3550000     	cmp	r5, #0
  3efb4c: ba00000c     	blt	0x3efb84 <Level::SG_SavePlayer(int, bool)+0x98> @ imm = #0x30
  3efb50: e59636c4     	ldr	r3, [r6, #0x6c4]
  3efb54: e1550003     	cmp	r5, r3
  3efb58: aa000009     	bge	0x3efb84 <Level::SG_SavePlayer(int, bool)+0x98> @ imm = #0x24
  3efb5c: e1a01005     	mov	r1, r5
  3efb60: e1a00006     	mov	r0, r6
  3efb64: e3a02001     	mov	r2, #1
  3efb68: ebfdfaf5     	bl	0x36e744 <PlayerManager::GetPlayer(int, bool)> @ imm = #-0x8142c
  3efb6c: e5901660     	ldr	r1, [r0, #0x660]
  3efb70: e1a02007     	mov	r2, r7
  3efb74: e1a00008     	mov	r0, r8
  3efb78: e28dd008     	add	sp, sp, #8
  3efb7c: e8bd41f0     	pop	{r4, r5, r6, r7, r8, lr}
  3efb80: eaffffb3     	b	0x3efa54 <Level::SG_SavePlayer(Character*, bool)> @ imm = #-0x134
  3efb84: e28dd008     	add	sp, sp, #8
  3efb88: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3efb8c: e59f308c     	ldr	r3, [pc, #0x8c]         @ 0x3efc20 <Level::SG_SavePlayer(int, bool)+0x134>
  3efb90: e7943003     	ldr	r3, [r4, r3]
  3efb94: e5933000     	ldr	r3, [r3]
  3efb98: e3530002     	cmp	r3, #2
  3efb9c: 03a03000     	moveq	r3, #0
  3efba0: 05833000     	streq	r3, [r3]
  3efba4: 0affffdb     	beq	0x3efb18 <Level::SG_SavePlayer(int, bool)+0x2c> @ imm = #-0x94
  3efba8: e3530001     	cmp	r3, #1
  3efbac: 1affffd9     	bne	0x3efb18 <Level::SG_SavePlayer(int, bool)+0x2c> @ imm = #-0x9c
  3efbb0: e59f006c     	ldr	r0, [pc, #0x6c]         @ 0x3efc24 <Level::SG_SavePlayer(int, bool)+0x138>
  3efbb4: e59f106c     	ldr	r1, [pc, #0x6c]         @ 0x3efc28 <Level::SG_SavePlayer(int, bool)+0x13c>
  3efbb8: e59f206c     	ldr	r2, [pc, #0x6c]         @ 0x3efc2c <Level::SG_SavePlayer(int, bool)+0x140>
  3efbbc: e7940000     	ldr	r0, [r4, r0]
  3efbc0: e59f3068     	ldr	r3, [pc, #0x68]         @ 0x3efc30 <Level::SG_SavePlayer(int, bool)+0x144>
  3efbc4: e300cb31     	movw	r12, #0xb31
  3efbc8: e08f1001     	add	r1, pc, r1
  3efbcc: e08f2002     	add	r2, pc, r2
  3efbd0: e08f3003     	add	r3, pc, r3
  3efbd4: e28000a8     	add	r0, r0, #168
  3efbd8: e58dc000     	str	r12, [sp]
  3efbdc: ebfc7908     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xe1be0
  3efbe0: eaffffcc     	b	0x3efb18 <Level::SG_SavePlayer(int, bool)+0x2c> @ imm = #-0xd0
  3efbe4: e59f0038     	ldr	r0, [pc, #0x38]         @ 0x3efc24 <Level::SG_SavePlayer(int, bool)+0x138>
  3efbe8: e59f1044     	ldr	r1, [pc, #0x44]         @ 0x3efc34 <Level::SG_SavePlayer(int, bool)+0x148>
  3efbec: e59f2044     	ldr	r2, [pc, #0x44]         @ 0x3efc38 <Level::SG_SavePlayer(int, bool)+0x14c>
  3efbf0: e7940000     	ldr	r0, [r4, r0]
  3efbf4: e59f3040     	ldr	r3, [pc, #0x40]         @ 0x3efc3c <Level::SG_SavePlayer(int, bool)+0x150>
  3efbf8: e300cb32     	movw	r12, #0xb32
  3efbfc: e08f1001     	add	r1, pc, r1
  3efc00: e08f2002     	add	r2, pc, r2
  3efc04: e08f3003     	add	r3, pc, r3
  3efc08: e28000a8     	add	r0, r0, #168
  3efc0c: e58dc000     	str	r12, [sp]
  3efc10: ebfc78fb     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xe1c14
  3efc14: eaffffcb     	b	0x3efb48 <Level::SG_SavePlayer(int, bool)+0x5c> @ imm = #-0xd4
  3efc18: 94 4f 5a 00  	.word	0x005a4f94
  3efc1c: f4 37 00 00  	.word	0x000037f4
  3efc20: c0 39 00 00  	.word	0x000039c0
  3efc24: c0 19 00 00  	.word	0x000019c0
  3efc28: 10 e8 4c 00  	.word	0x004ce810
  3efc2c: 34 69 4d 00  	.word	0x004d6934
  3efc30: 40 69 4d 00  	.word	0x004d6940
  3efc34: dc e7 4c 00  	.word	0x004ce7dc
  3efc38: 50 69 4d 00  	.word	0x004d6950
  3efc3c: 0c 69 4d 00  	.word	0x004d690c
