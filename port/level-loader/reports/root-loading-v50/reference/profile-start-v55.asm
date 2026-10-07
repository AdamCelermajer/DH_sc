
.local-inputs/libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0043e0d0 <NativeStartGame(gameswf::fn_call const&)>:
  43e0d0: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  43e0d4: e59f4214     	ldr	r4, [pc, #0x214]        @ 0x43e2f0 <NativeStartGame(gameswf::fn_call const&)+0x220>
  43e0d8: e59f5214     	ldr	r5, [pc, #0x214]        @ 0x43e2f4 <NativeStartGame(gameswf::fn_call const&)+0x224>
  43e0dc: e59f6214     	ldr	r6, [pc, #0x214]        @ 0x43e2f8 <NativeStartGame(gameswf::fn_call const&)+0x228>
  43e0e0: e08f4004     	add	r4, pc, r4
  43e0e4: e7943005     	ldr	r3, [r4, r5]
  43e0e8: e24ddf71     	sub	sp, sp, #452
  43e0ec: e1a08000     	mov	r8, r0
  43e0f0: e5933000     	ldr	r3, [r3]
  43e0f4: e7940006     	ldr	r0, [r4, r6]
  43e0f8: e58d31bc     	str	r3, [sp, #0x1bc]
  43e0fc: ebfb8524     	bl	0x31f594 <Application::GetCurrentLevel() const> @ imm = #-0x11eb70
  43e100: e3500000     	cmp	r0, #0
  43e104: 0a000002     	beq	0x43e114 <NativeStartGame(gameswf::fn_call const&)+0x44> @ imm = #0x8
  43e108: e5903130     	ldr	r3, [r0, #0x130]
  43e10c: e3530026     	cmp	r3, #38
  43e110: 0a000042     	beq	0x43e220 <NativeStartGame(gameswf::fn_call const&)+0x150> @ imm = #0x108
  43e114: e7943006     	ldr	r3, [r4, r6]
  43e118: e3a01000     	mov	r1, #0
  43e11c: e1a02001     	mov	r2, r1
  43e120: e5930040     	ldr	r0, [r3, #0x40]
  43e124: ebfcc0d3     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0xcfcb4
  43e128: e5907664     	ldr	r7, [r0, #0x664]
  43e12c: e3570000     	cmp	r7, #0
  43e130: ba000000     	blt	0x43e138 <NativeStartGame(gameswf::fn_call const&)+0x68> @ imm = #0x0
  43e134: eb009fe3     	bl	0x4660c8 <PlayerSavegame::SG_GetNextFreeSlot()> @ imm = #0x27f8c
  43e138: e28da024     	add	r10, sp, #36
  43e13c: e3a03000     	mov	r3, #0
  43e140: e1a0000a     	mov	r0, r10
  43e144: e1a01007     	mov	r1, r7
  43e148: e3a02001     	mov	r2, #1
  43e14c: eb009d16     	bl	0x4655ac <PlayerSavegame::PlayerSavegame(unsigned int, int, bool)> @ imm = #0x27458
  43e150: e5983010     	ldr	r3, [r8, #0x10]
  43e154: e3530001     	cmp	r3, #1
  43e158: 0a00004c     	beq	0x43e290 <NativeStartGame(gameswf::fn_call const&)+0x1c0> @ imm = #0x130
  43e15c: e59f8198     	ldr	r8, [pc, #0x198]        @ 0x43e2fc <NativeStartGame(gameswf::fn_call const&)+0x22c>
  43e160: e3a0b000     	mov	r11, #0
  43e164: e7948008     	ldr	r8, [r4, r8]
  43e168: e28d0d07     	add	r0, sp, #448
  43e16c: e5983000     	ldr	r3, [r8]
  43e170: e0803103     	add	r3, r0, r3, lsl #2
  43e174: e513914c     	ldr	r9, [r3, #-0x14c]
  43e178: eb0efd85     	bl	0x7fd794 <GetOnline()>  @ imm = #0x3bf614
  43e17c: e5d03005     	ldrb	r3, [r0, #0x5]
  43e180: e3530000     	cmp	r3, #0
  43e184: 1a00002c     	bne	0x43e23c <NativeStartGame(gameswf::fn_call const&)+0x16c> @ imm = #0xb0
  43e188: e5981000     	ldr	r1, [r8]
  43e18c: e28d0d07     	add	r0, sp, #448
  43e190: e3790001     	cmn	r9, #1
  43e194: e0803001     	add	r3, r0, r1
  43e198: e0801101     	add	r1, r0, r1, lsl #2
  43e19c: e511215c     	ldr	r2, [r1, #-0x15c]
  43e1a0: e5531150     	ldrb	r1, [r3, #-0x150]
  43e1a4: e58d101c     	str	r1, [sp, #0x1c]
  43e1a8: 0a000033     	beq	0x43e27c <NativeStartGame(gameswf::fn_call const&)+0x1ac> @ imm = #0xcc
  43e1ac: e3a08000     	mov	r8, #0
  43e1b0: e2433f55     	sub	r3, r3, #340
  43e1b4: e5c38004     	strb	r8, [r3, #0x4]
  43e1b8: e1a0000a     	mov	r0, r10
  43e1bc: e58d2018     	str	r2, [sp, #0x18]
  43e1c0: eb009a59     	bl	0x464b2c <PlayerSavegame::SG_Save()> @ imm = #0x26964
  43e1c4: e59f3134     	ldr	r3, [pc, #0x134]        @ 0x43e300 <NativeStartGame(gameswf::fn_call const&)+0x230>
  43e1c8: e3a0c048     	mov	r12, #72
  43e1cc: e7940006     	ldr	r0, [r4, r6]
  43e1d0: e7943003     	ldr	r3, [r4, r3]
  43e1d4: e59d2018     	ldr	r2, [sp, #0x18]
  43e1d8: e5931000     	ldr	r1, [r3]
  43e1dc: e1a03007     	mov	r3, r7
  43e1e0: e029199c     	mla	r9, r12, r9, r1
  43e1e4: e3a0c001     	mov	r12, #1
  43e1e8: e5991020     	ldr	r1, [r9, #0x20]
  43e1ec: e58dc000     	str	r12, [sp]
  43e1f0: e59dc01c     	ldr	r12, [sp, #0x1c]
  43e1f4: e58db008     	str	r11, [sp, #0x8]
  43e1f8: e58d800c     	str	r8, [sp, #0xc]
  43e1fc: e58dc004     	str	r12, [sp, #0x4]
  43e200: e58d8010     	str	r8, [sp, #0x10]
  43e204: e58d8014     	str	r8, [sp, #0x14]
  43e208: ebfbb6ee     	bl	0x32bdc8 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)> @ imm = #-0x112448
  43e20c: e5dd31b8     	ldrb	r3, [sp, #0x1b8]
  43e210: e1a0000a     	mov	r0, r10
  43e214: e1530008     	cmp	r3, r8
  43e218: 15cd81b8     	strbne	r8, [sp, #0x1b8]
  43e21c: eb00955a     	bl	0x46378c <PlayerSavegame::~PlayerSavegame()> @ imm = #0x25568
  43e220: e7943005     	ldr	r3, [r4, r5]
  43e224: e59d21bc     	ldr	r2, [sp, #0x1bc]
  43e228: e5933000     	ldr	r3, [r3]
  43e22c: e1520003     	cmp	r2, r3
  43e230: 1a00002d     	bne	0x43e2ec <NativeStartGame(gameswf::fn_call const&)+0x21c> @ imm = #0xb4
  43e234: e28ddf71     	add	sp, sp, #452
  43e238: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  43e23c: e7943006     	ldr	r3, [r4, r6]
  43e240: e5930040     	ldr	r0, [r3, #0x40]
  43e244: ebfcc38a     	bl	0x36f074 <PlayerManager::IsLocalPlayerHosting()> @ imm = #-0xcf1d8
  43e248: e5981000     	ldr	r1, [r8]
  43e24c: e3500000     	cmp	r0, #0
  43e250: 128d2d07     	addne	r2, sp, #448
  43e254: 10823101     	addne	r3, r2, r1, lsl #2
  43e258: 13a00000     	movne	r0, #0
  43e25c: 1513215c     	ldrne	r2, [r3, #-0x15c]
  43e260: 058d001c     	streq	r0, [sp, #0x1c]
  43e264: 03a02001     	moveq	r2, #1
  43e268: 158d001c     	strne	r0, [sp, #0x1c]
  43e26c: e28dcd07     	add	r12, sp, #448
  43e270: e3790001     	cmn	r9, #1
  43e274: e08c3001     	add	r3, r12, r1
  43e278: 1affffcb     	bne	0x43e1ac <NativeStartGame(gameswf::fn_call const&)+0xdc> @ imm = #-0xd4
  43e27c: e59f1080     	ldr	r1, [pc, #0x80]         @ 0x43e304 <NativeStartGame(gameswf::fn_call const&)+0x234>
  43e280: e7941001     	ldr	r1, [r4, r1]
  43e284: e5911000     	ldr	r1, [r1]
  43e288: e5919024     	ldr	r9, [r1, #0x24]
  43e28c: eaffffc6     	b	0x43e1ac <NativeStartGame(gameswf::fn_call const&)+0xdc> @ imm = #-0xe8
  43e290: e598300c     	ldr	r3, [r8, #0xc]
  43e294: e5980014     	ldr	r0, [r8, #0x14]
  43e298: e3a0900c     	mov	r9, #12
  43e29c: e5933000     	ldr	r3, [r3]
  43e2a0: e0203099     	mla	r0, r9, r0, r3
  43e2a4: ebffeeb8     	bl	0x439d8c <gameswf::as_value::is_number() const> @ imm = #-0x4520
  43e2a8: e3500000     	cmp	r0, #0
  43e2ac: 0affffaa     	beq	0x43e15c <NativeStartGame(gameswf::fn_call const&)+0x8c> @ imm = #-0x158
  43e2b0: e598300c     	ldr	r3, [r8, #0xc]
  43e2b4: e5980014     	ldr	r0, [r8, #0x14]
  43e2b8: e59f803c     	ldr	r8, [pc, #0x3c]         @ 0x43e2fc <NativeStartGame(gameswf::fn_call const&)+0x22c>
  43e2bc: e5933000     	ldr	r3, [r3]
  43e2c0: e0203099     	mla	r0, r9, r0, r3
  43e2c4: eb0d65e2     	bl	0x797a54 <gameswf::as_value::to_number() const> @ imm = #0x359788
  43e2c8: ebfb41d5     	bl	0x30ea24 <.plt+0xcb0>   @ imm = #-0x12f8ac
  43e2cc: e59d3060     	ldr	r3, [sp, #0x60]
  43e2d0: e1a0b000     	mov	r11, r0
  43e2d4: e1530000     	cmp	r3, r0
  43e2d8: a7943008     	ldrge	r3, [r4, r8]
  43e2dc: a5830000     	strge	r0, [r3]
  43e2e0: e1a0000a     	mov	r0, r10
  43e2e4: eb009a10     	bl	0x464b2c <PlayerSavegame::SG_Save()> @ imm = #0x26840
  43e2e8: eaffff9d     	b	0x43e164 <NativeStartGame(gameswf::fn_call const&)+0x94> @ imm = #-0x18c
  43e2ec: ebfb4007     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x12ffe4
  43e2f0: b0 69 55 00  	.word	0x005569b0
  43e2f4: ac 40 00 00  	.word	0x000040ac
  43e2f8: f4 37 00 00  	.word	0x000037f4
  43e2fc: 9c 1a 00 00  	.word	0x00001a9c
  43e300: 74 08 00 00  	.word	0x00000874
  43e304: c8 32 00 00  	.word	0x000032c8
