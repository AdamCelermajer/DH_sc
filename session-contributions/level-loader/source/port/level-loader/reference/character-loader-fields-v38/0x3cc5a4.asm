
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003cc5a4 <CharAI::_UpdateMaster()>:
  3cc5a4: e92d4070     	push	{r4, r5, r6, lr}
  3cc5a8: e5903050     	ldr	r3, [r0, #0x50]
  3cc5ac: e1a04000     	mov	r4, r0
  3cc5b0: e3530000     	cmp	r3, #0
  3cc5b4: 0a000022     	beq	0x3cc644 <CharAI::_UpdateMaster()+0xa0> @ imm = #0x88
  3cc5b8: e5900004     	ldr	r0, [r0, #0x4]
  3cc5bc: ebff5a8a     	bl	0x3a2fec <Character::GetCharAIId() const> @ imm = #-0x295d8
  3cc5c0: e5943050     	ldr	r3, [r4, #0x50]
  3cc5c4: e1a00003     	mov	r0, r3
  3cc5c8: e5933000     	ldr	r3, [r3]
  3cc5cc: e1a0e00f     	mov	lr, pc
  3cc5d0: e593f034     	ldr	pc, [r3, #0x34]
  3cc5d4: e5d43054     	ldrb	r3, [r4, #0x54]
  3cc5d8: e2200001     	eor	r0, r0, #1
  3cc5dc: e6ef5070     	uxtb	r5, r0
  3cc5e0: e3530000     	cmp	r3, #0
  3cc5e4: 0a000017     	beq	0x3cc648 <CharAI::_UpdateMaster()+0xa4> @ imm = #0x5c
  3cc5e8: e3550000     	cmp	r5, #0
  3cc5ec: 0a000042     	beq	0x3cc6fc <CharAI::_UpdateMaster()+0x158> @ imm = #0x108
  3cc5f0: e5941050     	ldr	r1, [r4, #0x50]
  3cc5f4: e5c45054     	strb	r5, [r4, #0x54]
  3cc5f8: e3510000     	cmp	r1, #0
  3cc5fc: 0a000010     	beq	0x3cc644 <CharAI::_UpdateMaster()+0xa0> @ imm = #0x40
  3cc600: e1a00004     	mov	r0, r4
  3cc604: eb002233     	bl	0x3d4ed8 <CharAI::AI_IsInSight(GameObject const*) const> @ imm = #0x88cc
  3cc608: e5d43055     	ldrb	r3, [r4, #0x55]
  3cc60c: e1a05000     	mov	r5, r0
  3cc610: e3530000     	cmp	r3, #0
  3cc614: 1a000012     	bne	0x3cc664 <CharAI::_UpdateMaster()+0xc0> @ imm = #0x48
  3cc618: e3500000     	cmp	r0, #0
  3cc61c: 1a000031     	bne	0x3cc6e8 <CharAI::_UpdateMaster()+0x144> @ imm = #0xc4
  3cc620: e5943050     	ldr	r3, [r4, #0x50]
  3cc624: e5c45055     	strb	r5, [r4, #0x55]
  3cc628: e3530000     	cmp	r3, #0
  3cc62c: 0a000004     	beq	0x3cc644 <CharAI::_UpdateMaster()+0xa0> @ imm = #0x10
  3cc630: e5d43054     	ldrb	r3, [r4, #0x54]
  3cc634: e3530000     	cmp	r3, #0
  3cc638: 0a000001     	beq	0x3cc644 <CharAI::_UpdateMaster()+0xa0> @ imm = #0x4
  3cc63c: e3550000     	cmp	r5, #0
  3cc640: 1a00000e     	bne	0x3cc680 <CharAI::_UpdateMaster()+0xdc> @ imm = #0x38
  3cc644: e8bd8070     	pop	{r4, r5, r6, pc}
  3cc648: e3550000     	cmp	r5, #0
  3cc64c: 0affffe7     	beq	0x3cc5f0 <CharAI::_UpdateMaster()+0x4c> @ imm = #-0x64
  3cc650: e5940004     	ldr	r0, [r4, #0x4]
  3cc654: e3a01013     	mov	r1, #19
  3cc658: e5942050     	ldr	r2, [r4, #0x50]
  3cc65c: ebff61be     	bl	0x3a4d5c <Character::RaiseEvent(int, void*)> @ imm = #-0x27908
  3cc660: eaffffe2     	b	0x3cc5f0 <CharAI::_UpdateMaster()+0x4c> @ imm = #-0x78
  3cc664: e3500000     	cmp	r0, #0
  3cc668: 1affffec     	bne	0x3cc620 <CharAI::_UpdateMaster()+0x7c> @ imm = #-0x50
  3cc66c: e5940004     	ldr	r0, [r4, #0x4]
  3cc670: e3a01014     	mov	r1, #20
  3cc674: e5942050     	ldr	r2, [r4, #0x50]
  3cc678: ebff61b7     	bl	0x3a4d5c <Character::RaiseEvent(int, void*)> @ imm = #-0x27924
  3cc67c: eaffffe7     	b	0x3cc620 <CharAI::_UpdateMaster()+0x7c> @ imm = #-0x64
  3cc680: e1a00004     	mov	r0, r4
  3cc684: ebffff7e     	bl	0x3cc484 <CharAI::IsMyTurn(CharAI const*)> @ imm = #-0x208
  3cc688: e3500000     	cmp	r0, #0
  3cc68c: 0affffec     	beq	0x3cc644 <CharAI::_UpdateMaster()+0xa0> @ imm = #-0x50
  3cc690: e5943004     	ldr	r3, [r4, #0x4]
  3cc694: e1a00003     	mov	r0, r3
  3cc698: e5933000     	ldr	r3, [r3]
  3cc69c: e1a0e00f     	mov	lr, pc
  3cc6a0: e593f124     	ldr	pc, [r3, #0x124]
  3cc6a4: e3500000     	cmp	r0, #0
  3cc6a8: 0a000018     	beq	0x3cc710 <CharAI::_UpdateMaster()+0x16c> @ imm = #0x60
  3cc6ac: e1a00004     	mov	r0, r4
  3cc6b0: e5941050     	ldr	r1, [r4, #0x50]
  3cc6b4: eb002747     	bl	0x3d63d8 <CharAI::AI_IsInCloseRange(GameObject const*) const> @ imm = #0x9d1c
  3cc6b8: e3500000     	cmp	r0, #0
  3cc6bc: 1a00001d     	bne	0x3cc738 <CharAI::_UpdateMaster()+0x194> @ imm = #0x74
  3cc6c0: e1a00004     	mov	r0, r4
  3cc6c4: e5941050     	ldr	r1, [r4, #0x50]
  3cc6c8: eb0027cd     	bl	0x3d6604 <CharAI::AI_IsInRange(GameObject const*) const> @ imm = #0x9f34
  3cc6cc: e3500000     	cmp	r0, #0
  3cc6d0: 0a000013     	beq	0x3cc724 <CharAI::_UpdateMaster()+0x180> @ imm = #0x4c
  3cc6d4: e5942050     	ldr	r2, [r4, #0x50]
  3cc6d8: e5940004     	ldr	r0, [r4, #0x4]
  3cc6dc: e3a01017     	mov	r1, #23
  3cc6e0: e8bd4070     	pop	{r4, r5, r6, lr}
  3cc6e4: eaff619c     	b	0x3a4d5c <Character::RaiseEvent(int, void*)> @ imm = #-0x27990
  3cc6e8: e5940004     	ldr	r0, [r4, #0x4]
  3cc6ec: e3a01015     	mov	r1, #21
  3cc6f0: e5942050     	ldr	r2, [r4, #0x50]
  3cc6f4: ebff6198     	bl	0x3a4d5c <Character::RaiseEvent(int, void*)> @ imm = #-0x279a0
  3cc6f8: eaffffc8     	b	0x3cc620 <CharAI::_UpdateMaster()+0x7c> @ imm = #-0xe0
  3cc6fc: e5940004     	ldr	r0, [r4, #0x4]
  3cc700: e3a01012     	mov	r1, #18
  3cc704: e5942050     	ldr	r2, [r4, #0x50]
  3cc708: ebff6193     	bl	0x3a4d5c <Character::RaiseEvent(int, void*)> @ imm = #-0x279b4
  3cc70c: eaffffb7     	b	0x3cc5f0 <CharAI::_UpdateMaster()+0x4c> @ imm = #-0x124
  3cc710: e1a00004     	mov	r0, r4
  3cc714: e5941050     	ldr	r1, [r4, #0x50]
  3cc718: eb00269a     	bl	0x3d6188 <CharAI::AI_IsInMeleeRange(GameObject const*) const> @ imm = #0x9a68
  3cc71c: e3500000     	cmp	r0, #0
  3cc720: 1a000009     	bne	0x3cc74c <CharAI::_UpdateMaster()+0x1a8> @ imm = #0x24
  3cc724: e5942050     	ldr	r2, [r4, #0x50]
  3cc728: e5940004     	ldr	r0, [r4, #0x4]
  3cc72c: e3a01016     	mov	r1, #22
  3cc730: e8bd4070     	pop	{r4, r5, r6, lr}
  3cc734: eaff6188     	b	0x3a4d5c <Character::RaiseEvent(int, void*)> @ imm = #-0x279e0
  3cc738: e5942050     	ldr	r2, [r4, #0x50]
  3cc73c: e5940004     	ldr	r0, [r4, #0x4]
  3cc740: e3a01018     	mov	r1, #24
  3cc744: e8bd4070     	pop	{r4, r5, r6, lr}
  3cc748: eaff6183     	b	0x3a4d5c <Character::RaiseEvent(int, void*)> @ imm = #-0x279f4
  3cc74c: e5942050     	ldr	r2, [r4, #0x50]
  3cc750: e5940004     	ldr	r0, [r4, #0x4]
  3cc754: e3a01019     	mov	r1, #25
  3cc758: e8bd4070     	pop	{r4, r5, r6, lr}
  3cc75c: eaff617e     	b	0x3a4d5c <Character::RaiseEvent(int, void*)> @ imm = #-0x27a08
