
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048e03c <rnd::RootRule::RootRule(rnd::RandomGenerator&)>:
  48e03c: e92d4070     	push	{r4, r5, r6, lr}
  48e040: e3a02000     	mov	r2, #0
  48e044: e1a06001     	mov	r6, r1
  48e048: e59f5028     	ldr	r5, [pc, #0x28]         @ 0x48e078 <rnd::RootRule::RootRule(rnd::RandomGenerator&)+0x3c>
  48e04c: e1a01000     	mov	r1, r0
  48e050: e1a04000     	mov	r4, r0
  48e054: ebffff0d     	bl	0x48dc90 <rnd::Rule::Rule(rnd::RootRule&, rnd::Rule*)> @ imm = #-0x3cc
  48e058: e59f301c     	ldr	r3, [pc, #0x1c]         @ 0x48e07c <rnd::RootRule::RootRule(rnd::RandomGenerator&)+0x40>
  48e05c: e08f5005     	add	r5, pc, r5
  48e060: e584608c     	str	r6, [r4, #0x8c]
  48e064: e7953003     	ldr	r3, [r5, r3]
  48e068: e1a00004     	mov	r0, r4
  48e06c: e2833008     	add	r3, r3, #8
  48e070: e5843000     	str	r3, [r4]
  48e074: e8bd8070     	pop	{r4, r5, r6, pc}
  48e078: 34 6a 50 00  	.word	0x00506a34
  48e07c: a8 48 00 00  	.word	0x000048a8
