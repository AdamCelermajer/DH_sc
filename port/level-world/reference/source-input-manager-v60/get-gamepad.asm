
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034dbb4 <InputManagerWin32::GetGamepad(int)>:
  34dbb4: e92d4030     	push	{r4, r5, lr}
  34dbb8: e59f3084     	ldr	r3, [pc, #0x84]         @ 0x34dc44 <InputManagerWin32::GetGamepad(int)+0x90>
  34dbbc: e3510003     	cmp	r1, #3
  34dbc0: e24dd00c     	sub	sp, sp, #12
  34dbc4: e1a04001     	mov	r4, r1
  34dbc8: e08f3003     	add	r3, pc, r3
  34dbcc: e1a05000     	mov	r5, r0
  34dbd0: da000008     	ble	0x34dbf8 <InputManagerWin32::GetGamepad(int)+0x44> @ imm = #0x20
  34dbd4: e59f206c     	ldr	r2, [pc, #0x6c]         @ 0x34dc48 <InputManagerWin32::GetGamepad(int)+0x94>
  34dbd8: e7932002     	ldr	r2, [r3, r2]
  34dbdc: e5922000     	ldr	r2, [r2]
  34dbe0: e3520002     	cmp	r2, #2
  34dbe4: 03a03000     	moveq	r3, #0
  34dbe8: 05833000     	streq	r3, [r3]
  34dbec: 0a000001     	beq	0x34dbf8 <InputManagerWin32::GetGamepad(int)+0x44> @ imm = #0x4
  34dbf0: e3520001     	cmp	r2, #1
  34dbf4: 0a000005     	beq	0x34dc10 <InputManagerWin32::GetGamepad(int)+0x5c> @ imm = #0x14
  34dbf8: e300075c     	movw	r0, #0x75c
  34dbfc: e0040490     	mul	r4, r0, r4
  34dc00: e2840ee5     	add	r0, r4, #3664
  34dc04: e0850000     	add	r0, r5, r0
  34dc08: e28dd00c     	add	sp, sp, #12
  34dc0c: e8bd8030     	pop	{r4, r5, pc}
  34dc10: e59f0034     	ldr	r0, [pc, #0x34]         @ 0x34dc4c <InputManagerWin32::GetGamepad(int)+0x98>
  34dc14: e59f1034     	ldr	r1, [pc, #0x34]         @ 0x34dc50 <InputManagerWin32::GetGamepad(int)+0x9c>
  34dc18: e59f2034     	ldr	r2, [pc, #0x34]         @ 0x34dc54 <InputManagerWin32::GetGamepad(int)+0xa0>
  34dc1c: e7930000     	ldr	r0, [r3, r0]
  34dc20: e59f3030     	ldr	r3, [pc, #0x30]         @ 0x34dc58 <InputManagerWin32::GetGamepad(int)+0xa4>
  34dc24: e3a0cd0a     	mov	r12, #640
  34dc28: e08f1001     	add	r1, pc, r1
  34dc2c: e08f2002     	add	r2, pc, r2
  34dc30: e08f3003     	add	r3, pc, r3
  34dc34: e28000a8     	add	r0, r0, #168
  34dc38: e58dc000     	str	r12, [sp]
  34dc3c: ebff00f0     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x3fc40
  34dc40: eaffffec     	b	0x34dbf8 <InputManagerWin32::GetGamepad(int)+0x44> @ imm = #-0x50
  34dc44: c8 6e 64 00  	.word	0x00646ec8
  34dc48: c0 39 00 00  	.word	0x000039c0
  34dc4c: c0 19 00 00  	.word	0x000019c0
  34dc50: b0 07 57 00  	.word	0x005707b0
  34dc54: 34 2a 57 00  	.word	0x00572a34
  34dc58: 38 2a 57 00  	.word	0x00572a38
