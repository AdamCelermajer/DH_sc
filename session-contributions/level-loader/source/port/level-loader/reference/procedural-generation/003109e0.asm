
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003109e0 <_ZNSs9_M_assignEPKcS0_>:
  3109e0: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3109e4: e1a04000     	mov	r4, r0
  3109e8: e5903010     	ldr	r3, [r0, #0x10]
  3109ec: e5900014     	ldr	r0, [r0, #0x14]
  3109f0: e0615002     	rsb	r5, r1, r2
  3109f4: e1a06002     	mov	r6, r2
  3109f8: e0602003     	rsb	r2, r0, r3
  3109fc: e1550002     	cmp	r5, r2
  310a00: e1a07001     	mov	r7, r1
  310a04: 8a00000c     	bhi	0x310a3c <_ZNSs9_M_assignEPKcS0_+0x5c> @ imm = #0x30
  310a08: e3550000     	cmp	r5, #0
  310a0c: 1a000012     	bne	0x310a5c <_ZNSs9_M_assignEPKcS0_+0x7c> @ imm = #0x48
  310a10: e0802005     	add	r2, r0, r5
  310a14: e1520003     	cmp	r2, r3
  310a18: 0a000005     	beq	0x310a34 <_ZNSs9_M_assignEPKcS0_+0x54> @ imm = #0x14
  310a1c: e5d31000     	ldrb	r1, [r3]
  310a20: e0633002     	rsb	r3, r3, r2
  310a24: e7c01005     	strb	r1, [r0, r5]
  310a28: e5942010     	ldr	r2, [r4, #0x10]
  310a2c: e0823003     	add	r3, r2, r3
  310a30: e5843010     	str	r3, [r4, #0x10]
  310a34: e1a00004     	mov	r0, r4
  310a38: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  310a3c: e3520000     	cmp	r2, #0
  310a40: 1a00000d     	bne	0x310a7c <_ZNSs9_M_assignEPKcS0_+0x9c> @ imm = #0x34
  310a44: e0871002     	add	r1, r7, r2
  310a48: e1a00004     	mov	r0, r4
  310a4c: e1a02006     	mov	r2, r6
  310a50: ebffff6b     	bl	0x310804 <_ZNSs9_M_appendEPKcS0_> @ imm = #-0x254
  310a54: e1a00004     	mov	r0, r4
  310a58: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  310a5c: e1a02005     	mov	r2, r5
  310a60: ebfff780     	bl	0x30e868 <.plt+0xaf4>   @ imm = #-0x2200
  310a64: e5940014     	ldr	r0, [r4, #0x14]
  310a68: e5943010     	ldr	r3, [r4, #0x10]
  310a6c: e0802005     	add	r2, r0, r5
  310a70: e1520003     	cmp	r2, r3
  310a74: 1affffe8     	bne	0x310a1c <_ZNSs9_M_assignEPKcS0_+0x3c> @ imm = #-0x60
  310a78: eaffffed     	b	0x310a34 <_ZNSs9_M_assignEPKcS0_+0x54> @ imm = #-0x4c
  310a7c: ebfff779     	bl	0x30e868 <.plt+0xaf4>   @ imm = #-0x221c
  310a80: e5943014     	ldr	r3, [r4, #0x14]
  310a84: e5942010     	ldr	r2, [r4, #0x10]
  310a88: e1a00004     	mov	r0, r4
  310a8c: e0632002     	rsb	r2, r3, r2
  310a90: e0871002     	add	r1, r7, r2
  310a94: e1a02006     	mov	r2, r6
  310a98: ebffff59     	bl	0x310804 <_ZNSs9_M_appendEPKcS0_> @ imm = #-0x29c
  310a9c: eaffffec     	b	0x310a54 <_ZNSs9_M_assignEPKcS0_+0x74> @ imm = #-0x50
