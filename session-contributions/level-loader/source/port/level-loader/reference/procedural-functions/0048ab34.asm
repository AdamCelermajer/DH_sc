
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048ab34 <rnd::Block::Block()>:
  48ab34: e92d4070     	push	{r4, r5, r6, lr}
  48ab38: e59f60d8     	ldr	r6, [pc, #0xd8]         @ 0x48ac18 <rnd::Block::Block()+0xe4>
  48ab3c: e59f20d8     	ldr	r2, [pc, #0xd8]         @ 0x48ac1c <rnd::Block::Block()+0xe8>
  48ab40: e1a03000     	mov	r3, r0
  48ab44: e08f6006     	add	r6, pc, r6
  48ab48: e7962002     	ldr	r2, [r6, r2]
  48ab4c: e1a04000     	mov	r4, r0
  48ab50: e3a01010     	mov	r1, #16
  48ab54: e2822008     	add	r2, r2, #8
  48ab58: e4832004     	str	r2, [r3], #4
  48ab5c: e1a00003     	mov	r0, r3
  48ab60: e5843014     	str	r3, [r4, #0x14]
  48ab64: e5843018     	str	r3, [r4, #0x18]
  48ab68: ebfa1ac3     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x1794f4
  48ab6c: e5942014     	ldr	r2, [r4, #0x14]
  48ab70: e3a05000     	mov	r5, #0
  48ab74: e284301c     	add	r3, r4, #28
  48ab78: e5c25000     	strb	r5, [r2]
  48ab7c: e1a00003     	mov	r0, r3
  48ab80: e584302c     	str	r3, [r4, #0x2c]
  48ab84: e5843030     	str	r3, [r4, #0x30]
  48ab88: e3a01010     	mov	r1, #16
  48ab8c: ebfa1aba     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x179518
  48ab90: e594202c     	ldr	r2, [r4, #0x2c]
  48ab94: e2843034     	add	r3, r4, #52
  48ab98: e1a00003     	mov	r0, r3
  48ab9c: e5c25000     	strb	r5, [r2]
  48aba0: e3a01010     	mov	r1, #16
  48aba4: e5843044     	str	r3, [r4, #0x44]
  48aba8: e5843048     	str	r3, [r4, #0x48]
  48abac: ebfa1ab2     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x179538
  48abb0: e59f3068     	ldr	r3, [pc, #0x68]         @ 0x48ac20 <rnd::Block::Block()+0xec>
  48abb4: e594c044     	ldr	r12, [r4, #0x44]
  48abb8: e3a01445     	mov	r1, #1157627904
  48abbc: e7960003     	ldr	r0, [r6, r3]
  48abc0: e28119ee     	add	r1, r1, #3899392
  48abc4: e3a03001     	mov	r3, #1
  48abc8: e5cc5000     	strb	r5, [r12]
  48abcc: e1a02005     	mov	r2, r5
  48abd0: e5841050     	str	r1, [r4, #0x50]
  48abd4: e5843058     	str	r3, [r4, #0x58]
  48abd8: e584104c     	str	r1, [r4, #0x4c]
  48abdc: e5843054     	str	r3, [r4, #0x54]
  48abe0: e584505c     	str	r5, [r4, #0x5c]
  48abe4: e2800040     	add	r0, r0, #64
  48abe8: e2843060     	add	r3, r4, #96
  48abec: e1a01005     	mov	r1, r5
  48abf0: e2822f4b     	add	r2, r2, #300
  48abf4: e3520e96     	cmp	r2, #2400
  48abf8: e5830014     	str	r0, [r3, #0x14]
  48abfc: e583101c     	str	r1, [r3, #0x1c]
  48ac00: e5831020     	str	r1, [r3, #0x20]
  48ac04: e5831024     	str	r1, [r3, #0x24]
  48ac08: e2833f4b     	add	r3, r3, #300
  48ac0c: 1afffff7     	bne	0x48abf0 <rnd::Block::Block()+0xbc> @ imm = #-0x24
  48ac10: e1a00004     	mov	r0, r4
  48ac14: e8bd8070     	pop	{r4, r5, r6, pc}
  48ac18: 4c 9f 50 00  	.word	0x00509f4c
  48ac1c: 1c 3e 00 00  	.word	0x00003e1c
  48ac20: fc 43 00 00  	.word	0x000043fc
