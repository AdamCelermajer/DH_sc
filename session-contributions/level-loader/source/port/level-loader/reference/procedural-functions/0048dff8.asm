
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048dff8 <rnd::RootRule::RootRule(rnd::RandomGenerator&)>:
  48dff8: e92d4070     	push	{r4, r5, r6, lr}
  48dffc: e3a02000     	mov	r2, #0
  48e000: e1a06001     	mov	r6, r1
  48e004: e59f5028     	ldr	r5, [pc, #0x28]         @ 0x48e034 <rnd::RootRule::RootRule(rnd::RandomGenerator&)+0x3c>
  48e008: e1a01000     	mov	r1, r0
  48e00c: e1a04000     	mov	r4, r0
  48e010: ebffff1e     	bl	0x48dc90 <rnd::Rule::Rule(rnd::RootRule&, rnd::Rule*)> @ imm = #-0x388
  48e014: e59f301c     	ldr	r3, [pc, #0x1c]         @ 0x48e038 <rnd::RootRule::RootRule(rnd::RandomGenerator&)+0x40>
  48e018: e08f5005     	add	r5, pc, r5
  48e01c: e584608c     	str	r6, [r4, #0x8c]
  48e020: e7953003     	ldr	r3, [r5, r3]
  48e024: e1a00004     	mov	r0, r4
  48e028: e2833008     	add	r3, r3, #8
  48e02c: e5843000     	str	r3, [r4]
  48e030: e8bd8070     	pop	{r4, r5, r6, pc}
  48e034: 78 6a 50 00  	.word	0x00506a78
  48e038: a8 48 00 00  	.word	0x000048a8
