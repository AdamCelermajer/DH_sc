
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048c31c <_ZSt14random_shuffleIPPKcN3rnd15RandomGeneratorEEvT_S5_RT0_>:
  48c31c: e1500001     	cmp	r0, r1
  48c320: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  48c324: e1a04000     	mov	r4, r0
  48c328: e1a05001     	mov	r5, r1
  48c32c: e1a08002     	mov	r8, r2
  48c330: 0a00000e     	beq	0x48c370 <_ZSt14random_shuffleIPPKcN3rnd15RandomGeneratorEEvT_S5_RT0_+0x54> @ imm = #0x38
  48c334: e2806004     	add	r6, r0, #4
  48c338: e1510006     	cmp	r1, r6
  48c33c: 0a00000b     	beq	0x48c370 <_ZSt14random_shuffleIPPKcN3rnd15RandomGeneratorEEvT_S5_RT0_+0x54> @ imm = #0x2c
  48c340: e3a07004     	mov	r7, #4
  48c344: e1a01147     	asr	r1, r7, #2
  48c348: e2811001     	add	r1, r1, #1
  48c34c: e1a00008     	mov	r0, r8
  48c350: ebffdde8     	bl	0x483af8 <_ZN3rnd15RandomGeneratorclEj> @ imm = #-0x8860
  48c354: e7942100     	ldr	r2, [r4, r0, lsl #2]
  48c358: e5963000     	ldr	r3, [r6]
  48c35c: e2877004     	add	r7, r7, #4
  48c360: e4862004     	str	r2, [r6], #4
  48c364: e1550006     	cmp	r5, r6
  48c368: e7843100     	str	r3, [r4, r0, lsl #2]
  48c36c: 1afffff4     	bne	0x48c344 <_ZSt14random_shuffleIPPKcN3rnd15RandomGeneratorEEvT_S5_RT0_+0x28> @ imm = #-0x30
  48c370: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
