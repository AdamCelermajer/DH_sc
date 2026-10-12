
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0038ef60 <_ZN10GameObject18LoadExternalScriptEPKcS1_>:
  38ef60: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  38ef64: e5903300     	ldr	r3, [r0, #0x300]
  38ef68: e1a04000     	mov	r4, r0
  38ef6c: e1a05001     	mov	r5, r1
  38ef70: e3530000     	cmp	r3, #0
  38ef74: e1a06002     	mov	r6, r2
  38ef78: 0a000005     	beq	0x38ef94 <_ZN10GameObject18LoadExternalScriptEPKcS1_+0x34> @ imm = #0x14
  38ef7c: e1a00003     	mov	r0, r3
  38ef80: e5933000     	ldr	r3, [r3]
  38ef84: e1a0e00f     	mov	lr, pc
  38ef88: e593f004     	ldr	pc, [r3, #0x4]
  38ef8c: e3a03000     	mov	r3, #0
  38ef90: e5843300     	str	r3, [r4, #0x300]
  38ef94: e3550000     	cmp	r5, #0
  38ef98: 0a000002     	beq	0x38efa8 <_ZN10GameObject18LoadExternalScriptEPKcS1_+0x48> @ imm = #0x8
  38ef9c: e1d530d0     	ldrsb	r3, [r5]
  38efa0: e3530000     	cmp	r3, #0
  38efa4: 1a000000     	bne	0x38efac <_ZN10GameObject18LoadExternalScriptEPKcS1_+0x4c> @ imm = #0x0
  38efa8: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
