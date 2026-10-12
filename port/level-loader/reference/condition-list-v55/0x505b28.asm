
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00505b28 <Structs::v2Condition::read(IStreamBase*)>:
  505b28: e92d4010     	push	{r4, lr}
  505b2c: e1a04000     	mov	r4, r0
  505b30: e24dd008     	sub	sp, sp, #8
  505b34: e1a00001     	mov	r0, r1
  505b38: e2841004     	add	r1, r4, #4
  505b3c: ebfd4d53     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0xacab4
  505b40: e3a03001     	mov	r3, #1
  505b44: e3530000     	cmp	r3, #0
  505b48: e58d3004     	str	r3, [sp, #0x4]
  505b4c: 1a00000f     	bne	0x505b90 <Structs::v2Condition::read(IStreamBase*)+0x68> @ imm = #0x3c
  505b50: e2843006     	add	r3, r4, #6
  505b54: e2844005     	add	r4, r4, #5
  505b58: e5d31001     	ldrb	r1, [r3, #0x1]
  505b5c: e5542001     	ldrb	r2, [r4, #-0x1]
  505b60: e1540003     	cmp	r4, r3
  505b64: e0212002     	eor	r2, r1, r2
  505b68: e5442001     	strb	r2, [r4, #-0x1]
  505b6c: e5d31001     	ldrb	r1, [r3, #0x1]
  505b70: e0222001     	eor	r2, r2, r1
  505b74: e5c32001     	strb	r2, [r3, #0x1]
  505b78: e5541001     	ldrb	r1, [r4, #-0x1]
  505b7c: e2433001     	sub	r3, r3, #1
  505b80: e0222001     	eor	r2, r2, r1
  505b84: e5442001     	strb	r2, [r4, #-0x1]
  505b88: e2844001     	add	r4, r4, #1
  505b8c: 3afffff1     	blo	0x505b58 <Structs::v2Condition::read(IStreamBase*)+0x30> @ imm = #-0x3c
  505b90: e28dd008     	add	sp, sp, #8
  505b94: e8bd8010     	pop	{r4, pc}
