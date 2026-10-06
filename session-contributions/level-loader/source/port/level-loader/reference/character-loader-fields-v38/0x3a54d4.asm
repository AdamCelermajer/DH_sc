
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003a54d4 <Character::GetCharModelName() const>:
  3a54d4: e92d4070     	push	{r4, r5, r6, lr}
  3a54d8: e24dd008     	sub	sp, sp, #8
  3a54dc: e1a06000     	mov	r6, r0
  3a54e0: ebfff740     	bl	0x3a31e8 <Character::GetCharModelId() const> @ imm = #-0x2300
  3a54e4: e59f4194     	ldr	r4, [pc, #0x194]        @ 0x3a5680 <Character::GetCharModelName() const+0x1ac>
  3a54e8: e3700001     	cmn	r0, #1
  3a54ec: e1a05000     	mov	r5, r0
  3a54f0: e08f4004     	add	r4, pc, r4
  3a54f4: 0a00000e     	beq	0x3a5534 <Character::GetCharModelName() const+0x60> @ imm = #0x38
  3a54f8: e1a00006     	mov	r0, r6
  3a54fc: ebfff6e4     	bl	0x3a3094 <Character::IsFaerie() const> @ imm = #-0x2470
  3a5500: e3500000     	cmp	r0, #0
  3a5504: 1a00000d     	bne	0x3a5540 <Character::GetCharModelName() const+0x6c> @ imm = #0x34
  3a5508: e5963000     	ldr	r3, [r6]
  3a550c: e1a00006     	mov	r0, r6
  3a5510: e1a0e00f     	mov	lr, pc
  3a5514: e593f028     	ldr	pc, [r3, #0x28]
  3a5518: e3500000     	cmp	r0, #0
  3a551c: 0a000002     	beq	0x3a552c <Character::GetCharModelName() const+0x58> @ imm = #0x8
  3a5520: ebff7089     	bl	0x38174c <Device::IsHighPerformance()> @ imm = #-0x23ddc
  3a5524: e3500000     	cmp	r0, #0
  3a5528: 0a00001b     	beq	0x3a559c <Character::GetCharModelName() const+0xc8> @ imm = #0x6c
  3a552c: e3550000     	cmp	r5, #0
  3a5530: aa00000d     	bge	0x3a556c <Character::GetCharModelName() const+0x98> @ imm = #0x34
  3a5534: e3a00000     	mov	r0, #0
  3a5538: e28dd008     	add	sp, sp, #8
  3a553c: e8bd8070     	pop	{r4, r5, r6, pc}
  3a5540: e5966418     	ldr	r6, [r6, #0x418]
  3a5544: e3560000     	cmp	r6, #0
  3a5548: 0afffff7     	beq	0x3a552c <Character::GetCharModelName() const+0x58> @ imm = #-0x24
  3a554c: e3e01000     	mvn	r1, #0
  3a5550: e1a00006     	mov	r0, r6
  3a5554: eb00590c     	bl	0x3bb98c <Character::SG_GetCurrentFaerieId(int) const> @ imm = #0x16430
  3a5558: e1a01000     	mov	r1, r0
  3a555c: e1a00006     	mov	r0, r6
  3a5560: eb002556     	bl	0x3aeac0 <Character::GetCharFaery(int) const> @ imm = #0x9558
  3a5564: e590500c     	ldr	r5, [r0, #0xc]
  3a5568: eaffffef     	b	0x3a552c <Character::GetCharModelName() const+0x58> @ imm = #-0x44
  3a556c: e59f3110     	ldr	r3, [pc, #0x110]        @ 0x3a5684 <Character::GetCharModelName() const+0x1b0>
  3a5570: e7943003     	ldr	r3, [r4, r3]
  3a5574: e5933000     	ldr	r3, [r3]
  3a5578: e1550003     	cmp	r5, r3
  3a557c: aaffffec     	bge	0x3a5534 <Character::GetCharModelName() const+0x60> @ imm = #-0x50
  3a5580: e59f3100     	ldr	r3, [pc, #0x100]        @ 0x3a5688 <Character::GetCharModelName() const+0x1b4>
  3a5584: e3a0200c     	mov	r2, #12
  3a5588: e7943003     	ldr	r3, [r4, r3]
  3a558c: e5933000     	ldr	r3, [r3]
  3a5590: e0253592     	mla	r5, r2, r5, r3
  3a5594: e5950008     	ldr	r0, [r5, #0x8]
  3a5598: eaffffe6     	b	0x3a5538 <Character::GetCharModelName() const+0x64> @ imm = #-0x68
  3a559c: e59f30e8     	ldr	r3, [pc, #0xe8]         @ 0x3a568c <Character::GetCharModelName() const+0x1b8>
  3a55a0: e1a01006     	mov	r1, r6
  3a55a4: e7943003     	ldr	r3, [r4, r3]
  3a55a8: e5930040     	ldr	r0, [r3, #0x40]
  3a55ac: ebff2692     	bl	0x36effc <PlayerManager::IsLocalPlayer(Character const*)> @ imm = #-0x365b8
  3a55b0: e3500000     	cmp	r0, #0
  3a55b4: 1affffdc     	bne	0x3a552c <Character::GetCharModelName() const+0x58> @ imm = #-0x90
  3a55b8: e30133c8     	movw	r3, #0x13c8
  3a55bc: e19620f3     	ldrsh	r2, [r6, r3]
  3a55c0: e2423e12     	sub	r3, r2, #288
  3a55c4: e2433002     	sub	r3, r3, #2
  3a55c8: e3530002     	cmp	r3, #2
  3a55cc: 9a00000c     	bls	0x3a5604 <Character::GetCharModelName() const+0x130> @ imm = #0x30
  3a55d0: e2423f51     	sub	r3, r2, #324
  3a55d4: e2433001     	sub	r3, r3, #1
  3a55d8: e3530002     	cmp	r3, #2
  3a55dc: 9a00000d     	bls	0x3a5618 <Character::GetCharModelName() const+0x144> @ imm = #0x34
  3a55e0: e2422f41     	sub	r2, r2, #260
  3a55e4: e2422003     	sub	r2, r2, #3
  3a55e8: e3520002     	cmp	r2, #2
  3a55ec: 8a00000e     	bhi	0x3a562c <Character::GetCharModelName() const+0x158> @ imm = #0x38
  3a55f0: e59f3090     	ldr	r3, [pc, #0x90]         @ 0x3a5688 <Character::GetCharModelName() const+0x1b4>
  3a55f4: e7943003     	ldr	r3, [r4, r3]
  3a55f8: e5933000     	ldr	r3, [r3]
  3a55fc: e59303a4     	ldr	r0, [r3, #0x3a4]
  3a5600: eaffffcc     	b	0x3a5538 <Character::GetCharModelName() const+0x64> @ imm = #-0xd0
  3a5604: e59f307c     	ldr	r3, [pc, #0x7c]         @ 0x3a5688 <Character::GetCharModelName() const+0x1b4>
  3a5608: e7943003     	ldr	r3, [r4, r3]
  3a560c: e5933000     	ldr	r3, [r3]
  3a5610: e593038c     	ldr	r0, [r3, #0x38c]
  3a5614: eaffffc7     	b	0x3a5538 <Character::GetCharModelName() const+0x64> @ imm = #-0xe4
  3a5618: e59f3068     	ldr	r3, [pc, #0x68]         @ 0x3a5688 <Character::GetCharModelName() const+0x1b4>
  3a561c: e7943003     	ldr	r3, [r4, r3]
  3a5620: e5933000     	ldr	r3, [r3]
  3a5624: e5930398     	ldr	r0, [r3, #0x398]
  3a5628: eaffffc2     	b	0x3a5538 <Character::GetCharModelName() const+0x64> @ imm = #-0xf8
  3a562c: e59f305c     	ldr	r3, [pc, #0x5c]         @ 0x3a5690 <Character::GetCharModelName() const+0x1bc>
  3a5630: e7943003     	ldr	r3, [r4, r3]
  3a5634: e5933000     	ldr	r3, [r3]
  3a5638: e3530002     	cmp	r3, #2
  3a563c: 05800000     	streq	r0, [r0]
  3a5640: 0affffb9     	beq	0x3a552c <Character::GetCharModelName() const+0x58> @ imm = #-0x11c
  3a5644: e3530001     	cmp	r3, #1
  3a5648: 1affffb7     	bne	0x3a552c <Character::GetCharModelName() const+0x58> @ imm = #-0x124
  3a564c: e59f0040     	ldr	r0, [pc, #0x40]         @ 0x3a5694 <Character::GetCharModelName() const+0x1c0>
  3a5650: e59f1040     	ldr	r1, [pc, #0x40]         @ 0x3a5698 <Character::GetCharModelName() const+0x1c4>
  3a5654: e59f2040     	ldr	r2, [pc, #0x40]         @ 0x3a569c <Character::GetCharModelName() const+0x1c8>
  3a5658: e7940000     	ldr	r0, [r4, r0]
  3a565c: e59f303c     	ldr	r3, [pc, #0x3c]         @ 0x3a56a0 <Character::GetCharModelName() const+0x1cc>
  3a5660: e300c4a7     	movw	r12, #0x4a7
  3a5664: e08f1001     	add	r1, pc, r1
  3a5668: e08f2002     	add	r2, pc, r2
  3a566c: e08f3003     	add	r3, pc, r3
  3a5670: e28000a8     	add	r0, r0, #168
  3a5674: e58dc000     	str	r12, [sp]
  3a5678: ebfda261     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x9767c
  3a567c: eaffffaa     	b	0x3a552c <Character::GetCharModelName() const+0x58> @ imm = #-0x158
  3a5680: a0 f5 5e 00  	.word	0x005ef5a0
  3a5684: 0c 3c 00 00  	.word	0x00003c0c
  3a5688: 44 43 00 00  	.word	0x00004344
  3a568c: f4 37 00 00  	.word	0x000037f4
  3a5690: c0 39 00 00  	.word	0x000039c0
  3a5694: c0 19 00 00  	.word	0x000019c0
  3a5698: 74 8d 51 00  	.word	0x00518d74
  3a569c: 00 8f 51 00  	.word	0x00518f00
  3a56a0: 34 db 51 00  	.word	0x0051db34
