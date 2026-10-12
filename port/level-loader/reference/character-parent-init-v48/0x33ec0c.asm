
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0033ec0c <ObjectBase::InitPost()>:
  33ec0c: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  33ec10: e1a08000     	mov	r8, r0
  33ec14: e280008c     	add	r0, r0, #140
  33ec18: ebffffc2     	bl	0x33eb28 <ConditionData::Init()> @ imm = #-0xf8
  33ec1c: e28800b0     	add	r0, r8, #176
  33ec20: ebffffc0     	bl	0x33eb28 <ConditionData::Init()> @ imm = #-0x100
  33ec24: e59850e8     	ldr	r5, [r8, #0xe8]
  33ec28: e59820e4     	ldr	r2, [r8, #0xe4]
  33ec2c: e59f3060     	ldr	r3, [pc, #0x60]         @ 0x33ec94 <ObjectBase::InitPost()+0x88>
  33ec30: e1550002     	cmp	r5, r2
  33ec34: e08f3003     	add	r3, pc, r3
  33ec38: 0a000012     	beq	0x33ec88 <ObjectBase::InitPost()+0x7c> @ imm = #0x48
  33ec3c: e59f2054     	ldr	r2, [pc, #0x54]         @ 0x33ec98 <ObjectBase::InitPost()+0x8c>
  33ec40: e7932002     	ldr	r2, [r3, r2]
  33ec44: e5926000     	ldr	r6, [r2]
  33ec48: e3560000     	cmp	r6, #0
  33ec4c: 0a00000e     	beq	0x33ec8c <ObjectBase::InitPost()+0x80> @ imm = #0x38
  33ec50: e59f2044     	ldr	r2, [pc, #0x44]         @ 0x33ec9c <ObjectBase::InitPost()+0x90>
  33ec54: e3a04000     	mov	r4, #0
  33ec58: e7933002     	ldr	r3, [r3, r2]
  33ec5c: e5937000     	ldr	r7, [r3]
  33ec60: ea000002     	b	0x33ec70 <ObjectBase::InitPost()+0x64> @ imm = #0x8
  33ec64: e2844001     	add	r4, r4, #1
  33ec68: e1540006     	cmp	r4, r6
  33ec6c: 0a000006     	beq	0x33ec8c <ObjectBase::InitPost()+0x80> @ imm = #0x18
  33ec70: e7971104     	ldr	r1, [r7, r4, lsl #2]
  33ec74: e1a00005     	mov	r0, r5
  33ec78: ebff3da7     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x30964
  33ec7c: e3500000     	cmp	r0, #0
  33ec80: 1afffff7     	bne	0x33ec64 <ObjectBase::InitPost()+0x58> @ imm = #-0x24
  33ec84: e58840ec     	str	r4, [r8, #0xec]
  33ec88: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  33ec8c: e3e04000     	mvn	r4, #0
  33ec90: eafffffb     	b	0x33ec84 <ObjectBase::InitPost()+0x78> @ imm = #-0x14
  33ec94: 5c 5e 65 00  	.word	0x00655e5c
  33ec98: 30 35 00 00  	.word	0x00003530
  33ec9c: 80 48 00 00  	.word	0x00004880
