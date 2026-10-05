
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048dc08 <rnd::Rule::Rule(rnd::RootRule&, rnd::Rule*)>:
  48dc08: e59f3078     	ldr	r3, [pc, #0x78]         @ 0x48dc88 <rnd::Rule::Rule(rnd::RootRule&, rnd::Rule*)+0x80>
  48dc0c: e92d4070     	push	{r4, r5, r6, lr}
  48dc10: e59fe074     	ldr	lr, [pc, #0x74]         @ 0x48dc8c <rnd::Rule::Rule(rnd::RootRule&, rnd::Rule*)+0x84>
  48dc14: e08f3003     	add	r3, pc, r3
  48dc18: e1a04000     	mov	r4, r0
  48dc1c: e793e00e     	ldr	lr, [r3, lr]
  48dc20: e3a05000     	mov	r5, #0
  48dc24: e280c050     	add	r12, r0, #80
  48dc28: e28ee008     	add	lr, lr, #8
  48dc2c: e5801004     	str	r1, [r0, #0x4]
  48dc30: e580e000     	str	lr, [r0]
  48dc34: e5802008     	str	r2, [r0, #0x8]
  48dc38: e3a01010     	mov	r1, #16
  48dc3c: e1a0000c     	mov	r0, r12
  48dc40: e584500c     	str	r5, [r4, #0xc]
  48dc44: e584c060     	str	r12, [r4, #0x60]
  48dc48: e584c064     	str	r12, [r4, #0x64]
  48dc4c: ebfa0e8a     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x17c5d8
  48dc50: e5942060     	ldr	r2, [r4, #0x60]
  48dc54: e3a03001     	mov	r3, #1
  48dc58: e1a00004     	mov	r0, r4
  48dc5c: e5c25000     	strb	r5, [r2]
  48dc60: e3e02000     	mvn	r2, #0
  48dc64: e584206c     	str	r2, [r4, #0x6c]
  48dc68: e584507c     	str	r5, [r4, #0x7c]
  48dc6c: e5843084     	str	r3, [r4, #0x84]
  48dc70: e5845068     	str	r5, [r4, #0x68]
  48dc74: e5845070     	str	r5, [r4, #0x70]
  48dc78: e5845074     	str	r5, [r4, #0x74]
  48dc7c: e5845078     	str	r5, [r4, #0x78]
  48dc80: e5843080     	str	r3, [r4, #0x80]
  48dc84: e8bd8070     	pop	{r4, r5, r6, pc}
  48dc88: 7c 6e 50 00  	.word	0x00506e7c
  48dc8c: 2c 24 00 00  	.word	0x0000242c
