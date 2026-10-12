
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00478f20 <ConditionList::~ConditionList()>:
  478f20: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  478f24: e5902000     	ldr	r2, [r0]
  478f28: e1a06000     	mov	r6, r0
  478f2c: e3520000     	cmp	r2, #0
  478f30: d5905004     	ldrle	r5, [r0, #0x4]
  478f34: da00000e     	ble	0x478f74 <ConditionList::~ConditionList()+0x54> @ imm = #0x38
  478f38: e5905004     	ldr	r5, [r0, #0x4]
  478f3c: e3a04000     	mov	r4, #0
  478f40: e1a07004     	mov	r7, r4
  478f44: e7953104     	ldr	r3, [r5, r4, lsl #2]
  478f48: e3530000     	cmp	r3, #0
  478f4c: 0a000005     	beq	0x478f68 <ConditionList::~ConditionList()+0x48> @ imm = #0x14
  478f50: e1a00003     	mov	r0, r3
  478f54: e5933000     	ldr	r3, [r3]
  478f58: e1a0e00f     	mov	lr, pc
  478f5c: e593f004     	ldr	pc, [r3, #0x4]
  478f60: e7857104     	str	r7, [r5, r4, lsl #2]
  478f64: e8960024     	ldm	r6, {r2, r5}
  478f68: e2844001     	add	r4, r4, #1
  478f6c: e1520004     	cmp	r2, r4
  478f70: cafffff3     	bgt	0x478f44 <ConditionList::~ConditionList()+0x24> @ imm = #-0x34
  478f74: e3550000     	cmp	r5, #0
  478f78: 0a000003     	beq	0x478f8c <ConditionList::~ConditionList()+0x6c> @ imm = #0xc
  478f7c: e1a00005     	mov	r0, r5
  478f80: ebfa5d2e     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x168b48
  478f84: e3a03000     	mov	r3, #0
  478f88: e5863004     	str	r3, [r6, #0x4]
  478f8c: e1a00006     	mov	r0, r6
  478f90: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
