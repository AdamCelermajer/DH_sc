
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00466384 <PlayerSavegame::SG_UnlockAllFastTravels()>:
  466384: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  466388: e59f5060     	ldr	r5, [pc, #0x60]         @ 0x4663f0 <PlayerSavegame::SG_UnlockAllFastTravels()+0x6c>
  46638c: e59f7060     	ldr	r7, [pc, #0x60]         @ 0x4663f4 <PlayerSavegame::SG_UnlockAllFastTravels()+0x70>
  466390: e1a09000     	mov	r9, r0
  466394: e08f5005     	add	r5, pc, r5
  466398: e795a007     	ldr	r10, [r5, r7]
  46639c: e3a08000     	mov	r8, #0
  4663a0: e59a3000     	ldr	r3, [r10]
  4663a4: e3530000     	cmp	r3, #0
  4663a8: 0a00000c     	beq	0x4663e0 <PlayerSavegame::SG_UnlockAllFastTravels()+0x5c> @ imm = #0x30
  4663ac: e0896188     	add	r6, r9, r8, lsl #3
  4663b0: e3a01000     	mov	r1, #0
  4663b4: e2866f5f     	add	r6, r6, #380
  4663b8: e1a04001     	mov	r4, r1
  4663bc: e1a00006     	mov	r0, r6
  4663c0: e3a02001     	mov	r2, #1
  4663c4: ebffffd9     	bl	0x466330 <std::bitset<64u>::set(unsigned int, int)> @ imm = #-0x9c
  4663c8: e7953007     	ldr	r3, [r5, r7]
  4663cc: e2844001     	add	r4, r4, #1
  4663d0: e1a01004     	mov	r1, r4
  4663d4: e5933000     	ldr	r3, [r3]
  4663d8: e1540003     	cmp	r4, r3
  4663dc: 3afffff6     	blo	0x4663bc <PlayerSavegame::SG_UnlockAllFastTravels()+0x38> @ imm = #-0x28
  4663e0: e2888001     	add	r8, r8, #1
  4663e4: e3580003     	cmp	r8, #3
  4663e8: 1affffec     	bne	0x4663a0 <PlayerSavegame::SG_UnlockAllFastTravels()+0x1c> @ imm = #-0x50
  4663ec: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  4663f0: fc e6 52 00  	.word	0x0052e6fc
  4663f4: f4 45 00 00  	.word	0x000045f4
