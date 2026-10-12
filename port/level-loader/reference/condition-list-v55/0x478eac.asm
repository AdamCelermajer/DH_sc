
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00478eac <ConditionList::~ConditionList()>:
  478eac: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  478eb0: e5902000     	ldr	r2, [r0]
  478eb4: e1a06000     	mov	r6, r0
  478eb8: e3520000     	cmp	r2, #0
  478ebc: d5905004     	ldrle	r5, [r0, #0x4]
  478ec0: da00000e     	ble	0x478f00 <ConditionList::~ConditionList()+0x54> @ imm = #0x38
  478ec4: e5905004     	ldr	r5, [r0, #0x4]
  478ec8: e3a04000     	mov	r4, #0
  478ecc: e1a07004     	mov	r7, r4
  478ed0: e7953104     	ldr	r3, [r5, r4, lsl #2]
  478ed4: e3530000     	cmp	r3, #0
  478ed8: 0a000005     	beq	0x478ef4 <ConditionList::~ConditionList()+0x48> @ imm = #0x14
  478edc: e1a00003     	mov	r0, r3
  478ee0: e5933000     	ldr	r3, [r3]
  478ee4: e1a0e00f     	mov	lr, pc
  478ee8: e593f004     	ldr	pc, [r3, #0x4]
  478eec: e7857104     	str	r7, [r5, r4, lsl #2]
  478ef0: e8960024     	ldm	r6, {r2, r5}
  478ef4: e2844001     	add	r4, r4, #1
  478ef8: e1520004     	cmp	r2, r4
  478efc: cafffff3     	bgt	0x478ed0 <ConditionList::~ConditionList()+0x24> @ imm = #-0x34
  478f00: e3550000     	cmp	r5, #0
  478f04: 0a000003     	beq	0x478f18 <ConditionList::~ConditionList()+0x6c> @ imm = #0xc
  478f08: e1a00005     	mov	r0, r5
  478f0c: ebfa5d4b     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x168ad4
  478f10: e3a03000     	mov	r3, #0
  478f14: e5863004     	str	r3, [r6, #0x4]
  478f18: e1a00006     	mov	r0, r6
  478f1c: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
