
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003bb98c <Character::SG_GetCurrentFaerieId(int) const>:
  3bb98c: e30134e8     	movw	r3, #0x14e8
  3bb990: e7900003     	ldr	r0, [r0, r3]
  3bb994: e59f3034     	ldr	r3, [pc, #0x34]         @ 0x3bb9d0 <Character::SG_GetCurrentFaerieId(int) const+0x44>
  3bb998: e3500000     	cmp	r0, #0
  3bb99c: e08f3003     	add	r3, pc, r3
  3bb9a0: 012fff1e     	bxeq	lr
  3bb9a4: e3710001     	cmn	r1, #1
  3bb9a8: 0a000002     	beq	0x3bb9b8 <Character::SG_GetCurrentFaerieId(int) const+0x2c> @ imm = #0x8
  3bb9ac: e0800101     	add	r0, r0, r1, lsl #2
  3bb9b0: e59000ac     	ldr	r0, [r0, #0xac]
  3bb9b4: e12fff1e     	bx	lr
  3bb9b8: e59f2014     	ldr	r2, [pc, #0x14]         @ 0x3bb9d4 <Character::SG_GetCurrentFaerieId(int) const+0x48>
  3bb9bc: e7933002     	ldr	r3, [r3, r2]
  3bb9c0: e5933000     	ldr	r3, [r3]
  3bb9c4: e0800103     	add	r0, r0, r3, lsl #2
  3bb9c8: e59000ac     	ldr	r0, [r0, #0xac]
  3bb9cc: e12fff1e     	bx	lr
  3bb9d0: f4 90 5d 00  	.word	0x005d90f4
  3bb9d4: 9c 1a 00 00  	.word	0x00001a9c
