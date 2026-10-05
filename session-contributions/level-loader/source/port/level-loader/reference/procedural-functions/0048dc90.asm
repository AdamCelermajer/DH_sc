
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048dc90 <rnd::Rule::Rule(rnd::RootRule&, rnd::Rule*)>:
  48dc90: e59f3078     	ldr	r3, [pc, #0x78]         @ 0x48dd10 <rnd::Rule::Rule(rnd::RootRule&, rnd::Rule*)+0x80>
  48dc94: e92d4070     	push	{r4, r5, r6, lr}
  48dc98: e59fe074     	ldr	lr, [pc, #0x74]         @ 0x48dd14 <rnd::Rule::Rule(rnd::RootRule&, rnd::Rule*)+0x84>
  48dc9c: e08f3003     	add	r3, pc, r3
  48dca0: e1a04000     	mov	r4, r0
  48dca4: e793e00e     	ldr	lr, [r3, lr]
  48dca8: e3a05000     	mov	r5, #0
  48dcac: e280c050     	add	r12, r0, #80
  48dcb0: e28ee008     	add	lr, lr, #8
  48dcb4: e5801004     	str	r1, [r0, #0x4]
  48dcb8: e580e000     	str	lr, [r0]
  48dcbc: e5802008     	str	r2, [r0, #0x8]
  48dcc0: e3a01010     	mov	r1, #16
  48dcc4: e1a0000c     	mov	r0, r12
  48dcc8: e584500c     	str	r5, [r4, #0xc]
  48dccc: e584c060     	str	r12, [r4, #0x60]
  48dcd0: e584c064     	str	r12, [r4, #0x64]
  48dcd4: ebfa0e68     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x17c660
  48dcd8: e5942060     	ldr	r2, [r4, #0x60]
  48dcdc: e3a03001     	mov	r3, #1
  48dce0: e1a00004     	mov	r0, r4
  48dce4: e5c25000     	strb	r5, [r2]
  48dce8: e3e02000     	mvn	r2, #0
  48dcec: e584206c     	str	r2, [r4, #0x6c]
  48dcf0: e584507c     	str	r5, [r4, #0x7c]
  48dcf4: e5843084     	str	r3, [r4, #0x84]
  48dcf8: e5845068     	str	r5, [r4, #0x68]
  48dcfc: e5845070     	str	r5, [r4, #0x70]
  48dd00: e5845074     	str	r5, [r4, #0x74]
  48dd04: e5845078     	str	r5, [r4, #0x78]
  48dd08: e5843080     	str	r3, [r4, #0x80]
  48dd0c: e8bd8070     	pop	{r4, r5, r6, pc}
  48dd10: f4 6d 50 00  	.word	0x00506df4
  48dd14: 2c 24 00 00  	.word	0x0000242c
