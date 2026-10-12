
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048e628 <_ZSt14random_shuffleIPSt4pairIPKN3rnd4ExitENS1_8ListElemEENS1_15RandomGeneratorEEvT_S9_RT0_>:
  48e628: e1500001     	cmp	r0, r1
  48e62c: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  48e630: e1a04000     	mov	r4, r0
  48e634: e1a05001     	mov	r5, r1
  48e638: e1a0a002     	mov	r10, r2
  48e63c: 0a000012     	beq	0x48e68c <_ZSt14random_shuffleIPSt4pairIPKN3rnd4ExitENS1_8ListElemEENS1_15RandomGeneratorEEvT_S9_RT0_+0x64> @ imm = #0x48
  48e640: e2806054     	add	r6, r0, #84
  48e644: e1510006     	cmp	r1, r6
  48e648: 0a00000f     	beq	0x48e68c <_ZSt14random_shuffleIPSt4pairIPKN3rnd4ExitENS1_8ListElemEENS1_15RandomGeneratorEEvT_S9_RT0_+0x64> @ imm = #0x3c
  48e64c: e3a07054     	mov	r7, #84
  48e650: e30c8f3d     	movw	r8, #0xcf3d
  48e654: e3438cf3     	movt	r8, #0x3cf3
  48e658: e1a09007     	mov	r9, r7
  48e65c: e1a01147     	asr	r1, r7, #2
  48e660: e0010198     	mul	r1, r8, r1
  48e664: e1a0000a     	mov	r0, r10
  48e668: e2811001     	add	r1, r1, #1
  48e66c: ebffd521     	bl	0x483af8 <_ZN3rnd15RandomGeneratorclEj> @ imm = #-0xab7c
  48e670: e0214099     	mla	r1, r9, r0, r4
  48e674: e1a00006     	mov	r0, r6
  48e678: e2866054     	add	r6, r6, #84
  48e67c: ebffffc1     	bl	0x48e588 <_ZSt4swapISt4pairIPKN3rnd4ExitENS1_8ListElemEEEvRT_S8_> @ imm = #-0xfc
  48e680: e1550006     	cmp	r5, r6
  48e684: e2877054     	add	r7, r7, #84
  48e688: 1afffff3     	bne	0x48e65c <_ZSt14random_shuffleIPSt4pairIPKN3rnd4ExitENS1_8ListElemEENS1_15RandomGeneratorEEvT_S9_RT0_+0x34> @ imm = #-0x34
  48e68c: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
