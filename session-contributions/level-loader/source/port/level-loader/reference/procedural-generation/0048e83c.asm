
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048e83c <_ZSt14random_shuffleIPN3rnd8ListElemENS0_15RandomGeneratorEEvT_S4_RT0_>:
  48e83c: e1500001     	cmp	r0, r1
  48e840: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  48e844: e1a04000     	mov	r4, r0
  48e848: e1a05001     	mov	r5, r1
  48e84c: e1a08002     	mov	r8, r2
  48e850: 0a000014     	beq	0x48e8a8 <_ZSt14random_shuffleIPN3rnd8ListElemENS0_15RandomGeneratorEEvT_S4_RT0_+0x6c> @ imm = #0x50
  48e854: e2806050     	add	r6, r0, #80
  48e858: e1510006     	cmp	r1, r6
  48e85c: 0a000011     	beq	0x48e8a8 <_ZSt14random_shuffleIPN3rnd8ListElemENS0_15RandomGeneratorEEvT_S4_RT0_+0x6c> @ imm = #0x44
  48e860: e3a07050     	mov	r7, #80
  48e864: e1a0a007     	mov	r10, r7
  48e868: e1a03247     	asr	r3, r7, #4
  48e86c: e1a00008     	mov	r0, r8
  48e870: e0832083     	add	r2, r3, r3, lsl #1
  48e874: e2877050     	add	r7, r7, #80
  48e878: e0822202     	add	r2, r2, r2, lsl #4
  48e87c: e0822402     	add	r2, r2, r2, lsl #8
  48e880: e0822802     	add	r2, r2, r2, lsl #16
  48e884: e0833102     	add	r3, r3, r2, lsl #2
  48e888: e2831001     	add	r1, r3, #1
  48e88c: ebffd499     	bl	0x483af8 <_ZN3rnd15RandomGeneratorclEj> @ imm = #-0xad9c
  48e890: e021409a     	mla	r1, r10, r0, r4
  48e894: e1a00006     	mov	r0, r6
  48e898: e2866050     	add	r6, r6, #80
  48e89c: ebffffc7     	bl	0x48e7c0 <_ZSt4swapIN3rnd8ListElemEEvRT_S3_> @ imm = #-0xe4
  48e8a0: e1550006     	cmp	r5, r6
  48e8a4: 1affffef     	bne	0x48e868 <_ZSt14random_shuffleIPN3rnd8ListElemENS0_15RandomGeneratorEEvT_S4_RT0_+0x2c> @ imm = #-0x44
  48e8a8: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
