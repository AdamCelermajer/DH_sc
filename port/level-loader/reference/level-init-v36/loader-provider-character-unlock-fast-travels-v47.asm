
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003bba70 <Character::SG_UnlockAllFastTravels()>:
  3bba70: e30134e8     	movw	r3, #0x14e8
  3bba74: e7900003     	ldr	r0, [r0, r3]
  3bba78: e3500000     	cmp	r0, #0
  3bba7c: 012fff1e     	bxeq	lr
  3bba80: ea02aa3f     	b	0x466384 <PlayerSavegame::SG_UnlockAllFastTravels()> @ imm = #0xaa8fc

003bba84 <Character::SG_SetFastTravelIdUnlocked(char const*, bool, int)>:
  3bba84: e52d4004     	str	r4, [sp, #-0x4]!
  3bba88: e301c4e8     	movw	r12, #0x14e8
  3bba8c: e790000c     	ldr	r0, [r0, r12]
  3bba90: e59fc030     	ldr	r12, [pc, #0x30]        @ 0x3bbac8 <Character::SG_SetFastTravelIdUnlocked(char const*, bool, int)+0x44>
  3bba94: e1a04003     	mov	r4, r3
  3bba98: e3500000     	cmp	r0, #0
  3bba9c: e08fc00c     	add	r12, pc, r12
  3bbaa0: 0a000006     	beq	0x3bbac0 <Character::SG_SetFastTravelIdUnlocked(char const*, bool, int)+0x3c> @ imm = #0x18
  3bbaa4: e3730001     	cmn	r3, #1
  3bbaa8: 1a000002     	bne	0x3bbab8 <Character::SG_SetFastTravelIdUnlocked(char const*, bool, int)+0x34> @ imm = #0x8
  3bbaac: e59f3018     	ldr	r3, [pc, #0x18]         @ 0x3bbacc <Character::SG_SetFastTravelIdUnlocked(char const*, bool, int)+0x48>
  3bbab0: e79c3003     	ldr	r3, [r12, r3]
  3bbab4: e5933000     	ldr	r3, [r3]
  3bbab8: e8bd0010     	ldm	sp!, {r4}
  3bbabc: ea02aa4d     	b	0x4663f8 <PlayerSavegame::SG_SetFastTravelIdUnlocked(char const*, bool, int)> @ imm = #0xaa934
  3bbac0: e8bd0010     	ldm	sp!, {r4}
  3bbac4: e12fff1e     	bx	lr
  3bbac8: f4 8f 5d 00  	.word	0x005d8ff4
  3bbacc: 9c 1a 00 00  	.word	0x00001a9c

003bbad0 <Character::SG_GetFastTravelIdUnlocked(char const*, int)>:
  3bbad0: e30134e8     	movw	r3, #0x14e8
  3bbad4: e7900003     	ldr	r0, [r0, r3]
  3bbad8: e59f3020     	ldr	r3, [pc, #0x20]         @ 0x3bbb00 <Character::SG_GetFastTravelIdUnlocked(char const*, int)+0x30>
  3bbadc: e3500000     	cmp	r0, #0
  3bbae0: e08f3003     	add	r3, pc, r3
  3bbae4: 012fff1e     	bxeq	lr
  3bbae8: e3720001     	cmn	r2, #1
  3bbaec: 1a000002     	bne	0x3bbafc <Character::SG_GetFastTravelIdUnlocked(char const*, int)+0x2c> @ imm = #0x8
  3bbaf0: e59f200c     	ldr	r2, [pc, #0xc]          @ 0x3bbb04 <Character::SG_GetFastTravelIdUnlocked(char const*, int)+0x34>
  3bbaf4: e7933002     	ldr	r3, [r3, r2]
  3bbaf8: e5932000     	ldr	r2, [r3]
  3bbafc: ea02aa66     	b	0x46649c <PlayerSavegame::SG_GetFastTravelIdUnlocked(char const*, int)> @ imm = #0xaa998
  3bbb00: b0 8f 5d 00  	.word	0x005d8fb0
  3bbb04: 9c 1a 00 00  	.word	0x00001a9c

003bbb08 <Character::SG_GetFastTravelIdUnlocked(int, int)>:
  3bbb08: e30134e8     	movw	r3, #0x14e8
  3bbb0c: e7900003     	ldr	r0, [r0, r3]
  3bbb10: e59f3020     	ldr	r3, [pc, #0x20]         @ 0x3bbb38 <Character::SG_GetFastTravelIdUnlocked(int, int)+0x30>
  3bbb14: e3500000     	cmp	r0, #0
  3bbb18: e08f3003     	add	r3, pc, r3
