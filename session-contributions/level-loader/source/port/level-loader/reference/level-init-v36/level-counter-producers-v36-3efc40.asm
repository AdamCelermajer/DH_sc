
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003efc40 <Level::SG_SaveAllPlayer(bool)>:
  3efc40: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3efc44: e59f5050     	ldr	r5, [pc, #0x50]         @ 0x3efc9c <Level::SG_SaveAllPlayer(bool)+0x5c>
  3efc48: e59f6050     	ldr	r6, [pc, #0x50]         @ 0x3efca0 <Level::SG_SaveAllPlayer(bool)+0x60>
  3efc4c: e1a07000     	mov	r7, r0
  3efc50: e08f5005     	add	r5, pc, r5
  3efc54: e7953006     	ldr	r3, [r5, r6]
  3efc58: e1a08001     	mov	r8, r1
  3efc5c: e5933040     	ldr	r3, [r3, #0x40]
  3efc60: e59336c4     	ldr	r3, [r3, #0x6c4]
  3efc64: e3530000     	cmp	r3, #0
  3efc68: 0a00000a     	beq	0x3efc98 <Level::SG_SaveAllPlayer(bool)+0x58> @ imm = #0x28
  3efc6c: e3a04000     	mov	r4, #0
  3efc70: e1a01004     	mov	r1, r4
  3efc74: e1a00007     	mov	r0, r7
  3efc78: e1a02008     	mov	r2, r8
  3efc7c: ebffff9a     	bl	0x3efaec <Level::SG_SavePlayer(int, bool)> @ imm = #-0x198
  3efc80: e7953006     	ldr	r3, [r5, r6]
  3efc84: e2844001     	add	r4, r4, #1
  3efc88: e5933040     	ldr	r3, [r3, #0x40]
  3efc8c: e59336c4     	ldr	r3, [r3, #0x6c4]
  3efc90: e1530004     	cmp	r3, r4
  3efc94: 8afffff5     	bhi	0x3efc70 <Level::SG_SaveAllPlayer(bool)+0x30> @ imm = #-0x2c
  3efc98: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3efc9c: 40 4e 5a 00  	.word	0x005a4e40
  3efca0: f4 37 00 00  	.word	0x000037f4
