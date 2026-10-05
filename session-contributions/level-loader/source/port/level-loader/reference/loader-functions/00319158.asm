
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00319158 <UserProperties::UserProperties(char const*)>:
  319158: e59f2048     	ldr	r2, [pc, #0x48]         @ 0x3191a8 <UserProperties::UserProperties(char const*)+0x50>
  31915c: e92d4070     	push	{r4, r5, r6, lr}
  319160: e59f5044     	ldr	r5, [pc, #0x44]         @ 0x3191ac <UserProperties::UserProperties(char const*)+0x54>
  319164: e08f2002     	add	r2, pc, r2
  319168: e3a0c000     	mov	r12, #0
  31916c: e7925005     	ldr	r5, [r2, r5]
  319170: e1a03000     	mov	r3, r0
  319174: e580c008     	str	r12, [r0, #0x8]
  319178: e2855008     	add	r5, r5, #8
  31917c: e5805000     	str	r5, [r0]
  319180: e151000c     	cmp	r1, r12
  319184: e5e3c004     	strb	r12, [r3, #0x4]!
  319188: e1a04000     	mov	r4, r0
  31918c: e5803010     	str	r3, [r0, #0x10]
  319190: e580c014     	str	r12, [r0, #0x14]
  319194: e580300c     	str	r3, [r0, #0xc]
  319198: 0a000000     	beq	0x3191a0 <UserProperties::UserProperties(char const*)+0x48> @ imm = #0x0
  31919c: ebffffa6     	bl	0x31903c <UserProperties::_ParseProperties(char const*)> @ imm = #-0x168
  3191a0: e1a00004     	mov	r0, r4
  3191a4: e8bd8070     	pop	{r4, r5, r6, pc}
  3191a8: 2c b9 67 00  	.word	0x0067b92c
  3191ac: 10 28 00 00  	.word	0x00002810
