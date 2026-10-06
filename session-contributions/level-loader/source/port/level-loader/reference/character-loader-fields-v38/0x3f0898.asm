
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f0898 <Level::PlaceFaeryAndFollowers(Character*)>:
  3f0898: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3f089c: e59f7234     	ldr	r7, [pc, #0x234]        @ 0x3f0ad8 <Level::PlaceFaeryAndFollowers(Character*)+0x240>
  3f08a0: e59f8234     	ldr	r8, [pc, #0x234]        @ 0x3f0adc <Level::PlaceFaeryAndFollowers(Character*)+0x244>
  3f08a4: e24dd01c     	sub	sp, sp, #28
  3f08a8: e08f7007     	add	r7, pc, r7
  3f08ac: e7973008     	ldr	r3, [r7, r8]
  3f08b0: e58d1000     	str	r1, [sp]
  3f08b4: e28db00c     	add	r11, sp, #12
  3f08b8: e5939038     	ldr	r9, [r3, #0x38]
  3f08bc: e5b96060     	ldr	r6, [r9, #0x60]!
  3f08c0: e1590006     	cmp	r9, r6
  3f08c4: 0a000054     	beq	0x3f0a1c <Level::PlaceFaeryAndFollowers(Character*)+0x184> @ imm = #0x150
  3f08c8: e5965008     	ldr	r5, [r6, #0x8]
  3f08cc: e3550000     	cmp	r5, #0
  3f08d0: 0a00004e     	beq	0x3f0a10 <Level::PlaceFaeryAndFollowers(Character*)+0x178> @ imm = #0x138
  3f08d4: e1a00005     	mov	r0, r5
  3f08d8: ebfec9ed     	bl	0x3a3094 <Character::IsFaerie() const> @ imm = #-0x4d84c
  3f08dc: e3500000     	cmp	r0, #0
  3f08e0: 0a00004f     	beq	0x3f0a24 <Level::PlaceFaeryAndFollowers(Character*)+0x18c> @ imm = #0x13c
  3f08e4: e59d2000     	ldr	r2, [sp]
  3f08e8: e3a03000     	mov	r3, #0
  3f08ec: e58d300c     	str	r3, [sp, #0xc]
  3f08f0: e3520000     	cmp	r2, #0
  3f08f4: e58d3010     	str	r3, [sp, #0x10]
  3f08f8: e58d3014     	str	r3, [sp, #0x14]
  3f08fc: 11a0a002     	movne	r10, r2
  3f0900: 0a000057     	beq	0x3f0a64 <Level::PlaceFaeryAndFollowers(Character*)+0x1cc> @ imm = #0x15c
  3f0904: e1a0100b     	mov	r1, r11
  3f0908: e1a0000a     	mov	r0, r10
  3f090c: ebfe8c74     	bl	0x393ae4 <GameObject::GetLookAtVec(Point3D<float>&) const> @ imm = #-0x5ce30
  3f0910: e1a0000a     	mov	r0, r10
  3f0914: ebfe8b30     	bl	0x3935dc <GameObject::GetTargetPosition() const> @ imm = #-0x5d340
  3f0918: e5901000     	ldr	r1, [r0]
  3f091c: e1a04000     	mov	r4, r0
  3f0920: e59d000c     	ldr	r0, [sp, #0xc]
  3f0924: ebfc789e     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0xe1d88
  3f0928: e58d000c     	str	r0, [sp, #0xc]
  3f092c: e5941004     	ldr	r1, [r4, #0x4]
  3f0930: e59d0010     	ldr	r0, [sp, #0x10]
  3f0934: ebfc789a     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0xe1d98
  3f0938: e58d0010     	str	r0, [sp, #0x10]
  3f093c: e5941008     	ldr	r1, [r4, #0x8]
  3f0940: e59d0014     	ldr	r0, [sp, #0x14]
  3f0944: ebfc7896     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0xe1da8
  3f0948: e1a0100b     	mov	r1, r11
  3f094c: e3a02001     	mov	r2, #1
  3f0950: e58d0014     	str	r0, [sp, #0x14]
  3f0954: e1a00005     	mov	r0, r5
  3f0958: ebfe8d15     	bl	0x393db4 <GameObject::SetPosition(Point3D<float> const&, bool)> @ imm = #-0x5cbac
  3f095c: e1a00005     	mov	r0, r5
  3f0960: ebfe8d4a     	bl	0x393e90 <GameObject::ForceUpdatePosition()> @ imm = #-0x5cad8
  3f0964: e1a00005     	mov	r0, r5
  3f0968: ebfec9c9     	bl	0x3a3094 <Character::IsFaerie() const> @ imm = #-0x4d8dc
  3f096c: e3500000     	cmp	r0, #0
  3f0970: 0a000031     	beq	0x3f0a3c <Level::PlaceFaeryAndFollowers(Character*)+0x1a4> @ imm = #0xc4
  3f0974: e7973008     	ldr	r3, [r7, r8]
  3f0978: e5930040     	ldr	r0, [r3, #0x40]
  3f097c: e59036c4     	ldr	r3, [r0, #0x6c4]
  3f0980: e3530000     	cmp	r3, #0
  3f0984: da00000c     	ble	0x3f09bc <Level::PlaceFaeryAndFollowers(Character*)+0x124> @ imm = #0x30
  3f0988: e3a04000     	mov	r4, #0
  3f098c: e1a01004     	mov	r1, r4
  3f0990: e3a02000     	mov	r2, #0
  3f0994: ebfdf76a     	bl	0x36e744 <PlayerManager::GetPlayer(int, bool)> @ imm = #-0x82258
  3f0998: e5903660     	ldr	r3, [r0, #0x660]
  3f099c: e2844001     	add	r4, r4, #1
  3f09a0: e3530000     	cmp	r3, #0
  3f09a4: 15835420     	strne	r5, [r3, #0x420]
  3f09a8: e7973008     	ldr	r3, [r7, r8]
  3f09ac: e5930040     	ldr	r0, [r3, #0x40]
  3f09b0: e59036c4     	ldr	r3, [r0, #0x6c4]
  3f09b4: e1540003     	cmp	r4, r3
  3f09b8: bafffff3     	blt	0x3f098c <Level::PlaceFaeryAndFollowers(Character*)+0xf4> @ imm = #-0x34
  3f09bc: e5953418     	ldr	r3, [r5, #0x418]
  3f09c0: e3a01000     	mov	r1, #0
  3f09c4: e3a02001     	mov	r2, #1
  3f09c8: e58d3004     	str	r3, [sp, #0x4]
  3f09cc: ebfdf6a9     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x8255c
  3f09d0: e5904660     	ldr	r4, [r0, #0x660]
  3f09d4: e3540000     	cmp	r4, #0
  3f09d8: 0a00001b     	beq	0x3f0a4c <Level::PlaceFaeryAndFollowers(Character*)+0x1b4> @ imm = #0x6c
  3f09dc: e2855ff2     	add	r5, r5, #968
  3f09e0: e1a00005     	mov	r0, r5
  3f09e4: e1a01004     	mov	r1, r4
  3f09e8: ebff90e4     	bl	0x3d4d80 <CharAI::AI_SetMaster(Character*)> @ imm = #-0x1bc70
  3f09ec: e1a00004     	mov	r0, r4
  3f09f0: e3e01000     	mvn	r1, #0
  3f09f4: ebff2be4     	bl	0x3bb98c <Character::SG_GetCurrentFaerieId(int) const> @ imm = #-0x35070
  3f09f8: e1a01000     	mov	r1, r0
  3f09fc: e1a0000a     	mov	r0, r10
  3f0a00: ebfef7e5     	bl	0x3ae99c <Character::ChangeFaery(unsigned int)> @ imm = #-0x4206c
  3f0a04: e1a00005     	mov	r0, r5
  3f0a08: e59d1004     	ldr	r1, [sp, #0x4]
  3f0a0c: ebff90db     	bl	0x3d4d80 <CharAI::AI_SetMaster(Character*)> @ imm = #-0x1bc94
  3f0a10: e5966000     	ldr	r6, [r6]
  3f0a14: e1590006     	cmp	r9, r6
  3f0a18: 1affffaa     	bne	0x3f08c8 <Level::PlaceFaeryAndFollowers(Character*)+0x30> @ imm = #-0x158
  3f0a1c: e28dd01c     	add	sp, sp, #28
  3f0a20: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3f0a24: e1a00005     	mov	r0, r5
  3f0a28: ebfec993     	bl	0x3a307c <Character::IsFollower() const> @ imm = #-0x4d9b4
  3f0a2c: e3500000     	cmp	r0, #0
  3f0a30: 1affffab     	bne	0x3f08e4 <Level::PlaceFaeryAndFollowers(Character*)+0x4c> @ imm = #-0x154
  3f0a34: e5966000     	ldr	r6, [r6]
  3f0a38: eafffff5     	b	0x3f0a14 <Level::PlaceFaeryAndFollowers(Character*)+0x17c> @ imm = #-0x2c
  3f0a3c: e1a00005     	mov	r0, r5
  3f0a40: ebfe6eee     	bl	0x38c600 <GameObject::DisableZoning()> @ imm = #-0x64448
  3f0a44: e5966000     	ldr	r6, [r6]
  3f0a48: eafffff1     	b	0x3f0a14 <Level::PlaceFaeryAndFollowers(Character*)+0x17c> @ imm = #-0x3c
  3f0a4c: e2855ff2     	add	r5, r5, #968
  3f0a50: e1a00005     	mov	r0, r5
  3f0a54: e1a0100a     	mov	r1, r10
  3f0a58: ebff90c8     	bl	0x3d4d80 <CharAI::AI_SetMaster(Character*)> @ imm = #-0x1bce0
  3f0a5c: e1a0000a     	mov	r0, r10
  3f0a60: eaffffe2     	b	0x3f09f0 <Level::PlaceFaeryAndFollowers(Character*)+0x158> @ imm = #-0x78
  3f0a64: eb10334a     	bl	0x7fd794 <GetOnline()>  @ imm = #0x40cd28
  3f0a68: e5d03005     	ldrb	r3, [r0, #0x5]
  3f0a6c: e3530000     	cmp	r3, #0
  3f0a70: 0a00000e     	beq	0x3f0ab0 <Level::PlaceFaeryAndFollowers(Character*)+0x218> @ imm = #0x38
  3f0a74: e7974008     	ldr	r4, [r7, r8]
  3f0a78: e5940040     	ldr	r0, [r4, #0x40]
  3f0a7c: ebfdf586     	bl	0x36e09c <PlayerManager::GetHostingPlayer()> @ imm = #-0x829e8
  3f0a80: e590a660     	ldr	r10, [r0, #0x660]
  3f0a84: e5940040     	ldr	r0, [r4, #0x40]
  3f0a88: ebfdf979     	bl	0x36f074 <PlayerManager::IsLocalPlayerHosting()> @ imm = #-0x81a1c
  3f0a8c: e3500000     	cmp	r0, #0
  3f0a90: 1a00000c     	bne	0x3f0ac8 <Level::PlaceFaeryAndFollowers(Character*)+0x230> @ imm = #0x30
  3f0a94: e5940040     	ldr	r0, [r4, #0x40]
  3f0a98: ebfdf57f     	bl	0x36e09c <PlayerManager::GetHostingPlayer()> @ imm = #-0x82a04
  3f0a9c: e5903670     	ldr	r3, [r0, #0x670]
  3f0aa0: e3a02000     	mov	r2, #0
  3f0aa4: e5852114     	str	r2, [r5, #0x114]
  3f0aa8: e5853110     	str	r3, [r5, #0x110]
  3f0aac: ea000005     	b	0x3f0ac8 <Level::PlaceFaeryAndFollowers(Character*)+0x230> @ imm = #0x14
  3f0ab0: e7973008     	ldr	r3, [r7, r8]
  3f0ab4: e59d1000     	ldr	r1, [sp]
  3f0ab8: e3a02001     	mov	r2, #1
  3f0abc: e5930040     	ldr	r0, [r3, #0x40]
  3f0ac0: ebfdf66c     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x82650
  3f0ac4: e590a660     	ldr	r10, [r0, #0x660]
  3f0ac8: e35a0000     	cmp	r10, #0
  3f0acc: 1affff8c     	bne	0x3f0904 <Level::PlaceFaeryAndFollowers(Character*)+0x6c> @ imm = #-0x1d0
  3f0ad0: e5966000     	ldr	r6, [r6]
  3f0ad4: eaffffce     	b	0x3f0a14 <Level::PlaceFaeryAndFollowers(Character*)+0x17c> @ imm = #-0xc8
  3f0ad8: e8 41 5a 00  	.word	0x005a41e8
  3f0adc: f4 37 00 00  	.word	0x000037f4
