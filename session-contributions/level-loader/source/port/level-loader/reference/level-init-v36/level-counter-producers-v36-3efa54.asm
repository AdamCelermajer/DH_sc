
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003efa54 <Level::SG_SavePlayer(Character*, bool)>:
  3efa54: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3efa58: e2514000     	subs	r4, r1, #0
  3efa5c: e1a05000     	mov	r5, r0
  3efa60: e1a07002     	mov	r7, r2
  3efa64: 0a00001b     	beq	0x3efad8 <Level::SG_SavePlayer(Character*, bool)+0x84> @ imm = #0x6c
  3efa68: e5943000     	ldr	r3, [r4]
  3efa6c: e1a00004     	mov	r0, r4
  3efa70: e1a0e00f     	mov	lr, pc
  3efa74: e593f028     	ldr	pc, [r3, #0x28]
  3efa78: e3500000     	cmp	r0, #0
  3efa7c: 0a000015     	beq	0x3efad8 <Level::SG_SavePlayer(Character*, bool)+0x84> @ imm = #0x54
  3efa80: e1a00004     	mov	r0, r4
  3efa84: ebff2f3e     	bl	0x3bb784 <Character::SG_IsBlocked()> @ imm = #-0x34308
  3efa88: e3570000     	cmp	r7, #0
  3efa8c: e1a06000     	mov	r6, r0
  3efa90: 1a000011     	bne	0x3efadc <Level::SG_SavePlayer(Character*, bool)+0x88> @ imm = #0x44
  3efa94: e1a00004     	mov	r0, r4
  3efa98: ebff35a0     	bl	0x3bd120 <Character::GetLevel() const> @ imm = #-0x32980
  3efa9c: e1a01000     	mov	r1, r0
  3efaa0: e1a00004     	mov	r0, r4
  3efaa4: ebff2f65     	bl	0x3bb840 <Character::SG_SetPlayerLevel(int)> @ imm = #-0x3426c
  3efaa8: e1a00004     	mov	r0, r4
  3efaac: ebff3069     	bl	0x3bbc58 <Character::SG_SetSaveDate()> @ imm = #-0x33e5c
  3efab0: e5951110     	ldr	r1, [r5, #0x110]
  3efab4: e1a00004     	mov	r0, r4
  3efab8: e3e02000     	mvn	r2, #0
  3efabc: ebff2f76     	bl	0x3bb89c <Character::SG_SetLevelEntryPoint(int, int)> @ imm = #-0x34228
  3efac0: e1a00004     	mov	r0, r4
  3efac4: ebff3277     	bl	0x3bc4a8 <Character::SG_Save()> @ imm = #-0x33624
  3efac8: e1a00004     	mov	r0, r4
  3efacc: e1a01006     	mov	r1, r6
  3efad0: e8bd41f0     	pop	{r4, r5, r6, r7, r8, lr}
  3efad4: eaff2f25     	b	0x3bb770 <Character::SG_Block(bool)> @ imm = #-0x3436c
  3efad8: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3efadc: e1a00004     	mov	r0, r4
  3efae0: e3a01000     	mov	r1, #0
  3efae4: ebff2f21     	bl	0x3bb770 <Character::SG_Block(bool)> @ imm = #-0x3437c
  3efae8: eaffffe9     	b	0x3efa94 <Level::SG_SavePlayer(Character*, bool)+0x40> @ imm = #-0x5c
