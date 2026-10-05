
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00483a94 <rnd::RandomGenerator::NextInt()>:
  483a94: e92d4010     	push	{r4, lr}
  483a98: e1a04000     	mov	r4, r0
  483a9c: e5900000     	ldr	r0, [r0]
  483aa0: e30630c1     	movw	r3, #0x60c1
  483aa4: e34130a8     	movt	r3, #0x10a8
  483aa8: e2800001     	add	r0, r0, #1
  483aac: e0810390     	umull	r0, r1, r0, r3
  483ab0: e3e02004     	mvn	r2, #4
  483ab4: e3a03000     	mov	r3, #0
  483ab8: ebfa2b85     	bl	0x30e8d4 <.plt+0xb60>   @ imm = #-0x1751ec
  483abc: e5842000     	str	r2, [r4]
  483ac0: e1a00002     	mov	r0, r2
  483ac4: e8bd8010     	pop	{r4, pc}
