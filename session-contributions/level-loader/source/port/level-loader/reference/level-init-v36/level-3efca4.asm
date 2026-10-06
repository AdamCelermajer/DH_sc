
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003efca4 <Level::_LoadFinalInit()>:
  3efca4: e59f30c0     	ldr	r3, [pc, #0xc0]         @ 0x3efd6c <Level::_LoadFinalInit()+0xc8>
  3efca8: e59f20c0     	ldr	r2, [pc, #0xc0]         @ 0x3efd70 <Level::_LoadFinalInit()+0xcc>
  3efcac: e92d4070     	push	{r4, r5, r6, lr}
  3efcb0: e08f3003     	add	r3, pc, r3
  3efcb4: e7932002     	ldr	r2, [r3, r2]
  3efcb8: e24dd010     	sub	sp, sp, #16
  3efcbc: e5926038     	ldr	r6, [r2, #0x38]
  3efcc0: e5964014     	ldr	r4, [r6, #0x14]
  3efcc4: e286600c     	add	r6, r6, #12
  3efcc8: e1540006     	cmp	r4, r6
  3efccc: 0a000017     	beq	0x3efd30 <Level::_LoadFinalInit()+0x8c> @ imm = #0x5c
  3efcd0: e28d5004     	add	r5, sp, #4
  3efcd4: e594102c     	ldr	r1, [r4, #0x2c]
  3efcd8: e3510000     	cmp	r1, #0
  3efcdc: 0a000008     	beq	0x3efd04 <Level::_LoadFinalInit()+0x60> @ imm = #0x20
  3efce0: e1a00005     	mov	r0, r5
  3efce4: ebfd3810     	bl	0x33dd2c <ObjectBase::GetHandle()> @ imm = #-0xb1fc0
  3efce8: e1a00005     	mov	r0, r5
  3efcec: ebfd407c     	bl	0x33fee4 <ObjectHandle::operator GameObject*()> @ imm = #-0xafe10
  3efcf0: e2503000     	subs	r3, r0, #0
  3efcf4: 0a000002     	beq	0x3efd04 <Level::_LoadFinalInit()+0x60> @ imm = #0x8
  3efcf8: e5933000     	ldr	r3, [r3]
  3efcfc: e1a0e00f     	mov	lr, pc
  3efd00: e593f058     	ldr	pc, [r3, #0x58]
  3efd04: e594200c     	ldr	r2, [r4, #0xc]
  3efd08: e3520000     	cmp	r2, #0
  3efd0c: 1a000001     	bne	0x3efd18 <Level::_LoadFinalInit()+0x74> @ imm = #0x4
  3efd10: ea000008     	b	0x3efd38 <Level::_LoadFinalInit()+0x94> @ imm = #0x20
  3efd14: e1a02003     	mov	r2, r3
  3efd18: e5923008     	ldr	r3, [r2, #0x8]
  3efd1c: e3530000     	cmp	r3, #0
  3efd20: 1afffffb     	bne	0x3efd14 <Level::_LoadFinalInit()+0x70> @ imm = #-0x14
  3efd24: e1a04002     	mov	r4, r2
  3efd28: e1560004     	cmp	r6, r4
  3efd2c: 1affffe8     	bne	0x3efcd4 <Level::_LoadFinalInit()+0x30> @ imm = #-0x60
  3efd30: e28dd010     	add	sp, sp, #16
  3efd34: e8bd8070     	pop	{r4, r5, r6, pc}
  3efd38: e5943004     	ldr	r3, [r4, #0x4]
  3efd3c: e593100c     	ldr	r1, [r3, #0xc]
  3efd40: e1540001     	cmp	r4, r1
  3efd44: 1a000005     	bne	0x3efd60 <Level::_LoadFinalInit()+0xbc> @ imm = #0x14
  3efd48: e1a04003     	mov	r4, r3
  3efd4c: e5933004     	ldr	r3, [r3, #0x4]
  3efd50: e593200c     	ldr	r2, [r3, #0xc]
  3efd54: e1520004     	cmp	r2, r4
  3efd58: 0afffffa     	beq	0x3efd48 <Level::_LoadFinalInit()+0xa4> @ imm = #-0x18
  3efd5c: e594200c     	ldr	r2, [r4, #0xc]
  3efd60: e1520003     	cmp	r2, r3
  3efd64: 11a04003     	movne	r4, r3
  3efd68: eaffffee     	b	0x3efd28 <Level::_LoadFinalInit()+0x84> @ imm = #-0x48
  3efd6c: e0 4d 5a 00  	.word	0x005a4de0
  3efd70: f4 37 00 00  	.word	0x000037f4
