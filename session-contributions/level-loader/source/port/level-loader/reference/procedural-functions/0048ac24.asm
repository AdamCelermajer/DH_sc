
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048ac24 <rnd::Block::Block()>:
  48ac24: e92d4070     	push	{r4, r5, r6, lr}
  48ac28: e59f60d8     	ldr	r6, [pc, #0xd8]         @ 0x48ad08 <rnd::Block::Block()+0xe4>
  48ac2c: e59f20d8     	ldr	r2, [pc, #0xd8]         @ 0x48ad0c <rnd::Block::Block()+0xe8>
  48ac30: e1a03000     	mov	r3, r0
  48ac34: e08f6006     	add	r6, pc, r6
  48ac38: e7962002     	ldr	r2, [r6, r2]
  48ac3c: e1a04000     	mov	r4, r0
  48ac40: e3a01010     	mov	r1, #16
  48ac44: e2822008     	add	r2, r2, #8
  48ac48: e4832004     	str	r2, [r3], #4
  48ac4c: e1a00003     	mov	r0, r3
  48ac50: e5843014     	str	r3, [r4, #0x14]
  48ac54: e5843018     	str	r3, [r4, #0x18]
  48ac58: ebfa1a87     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x1795e4
  48ac5c: e5942014     	ldr	r2, [r4, #0x14]
  48ac60: e3a05000     	mov	r5, #0
  48ac64: e284301c     	add	r3, r4, #28
  48ac68: e5c25000     	strb	r5, [r2]
  48ac6c: e1a00003     	mov	r0, r3
  48ac70: e584302c     	str	r3, [r4, #0x2c]
  48ac74: e5843030     	str	r3, [r4, #0x30]
  48ac78: e3a01010     	mov	r1, #16
  48ac7c: ebfa1a7e     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x179608
  48ac80: e594202c     	ldr	r2, [r4, #0x2c]
  48ac84: e2843034     	add	r3, r4, #52
  48ac88: e1a00003     	mov	r0, r3
  48ac8c: e5c25000     	strb	r5, [r2]
  48ac90: e3a01010     	mov	r1, #16
  48ac94: e5843044     	str	r3, [r4, #0x44]
  48ac98: e5843048     	str	r3, [r4, #0x48]
  48ac9c: ebfa1a76     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x179628
  48aca0: e59f3068     	ldr	r3, [pc, #0x68]         @ 0x48ad10 <rnd::Block::Block()+0xec>
  48aca4: e594c044     	ldr	r12, [r4, #0x44]
  48aca8: e3a01445     	mov	r1, #1157627904
  48acac: e7960003     	ldr	r0, [r6, r3]
  48acb0: e28119ee     	add	r1, r1, #3899392
  48acb4: e3a03001     	mov	r3, #1
  48acb8: e5cc5000     	strb	r5, [r12]
  48acbc: e1a02005     	mov	r2, r5
  48acc0: e5841050     	str	r1, [r4, #0x50]
  48acc4: e5843058     	str	r3, [r4, #0x58]
  48acc8: e584104c     	str	r1, [r4, #0x4c]
  48accc: e5843054     	str	r3, [r4, #0x54]
  48acd0: e584505c     	str	r5, [r4, #0x5c]
  48acd4: e2800040     	add	r0, r0, #64
  48acd8: e2843060     	add	r3, r4, #96
  48acdc: e1a01005     	mov	r1, r5
  48ace0: e2822f4b     	add	r2, r2, #300
  48ace4: e3520e96     	cmp	r2, #2400
  48ace8: e5830014     	str	r0, [r3, #0x14]
  48acec: e583101c     	str	r1, [r3, #0x1c]
  48acf0: e5831020     	str	r1, [r3, #0x20]
  48acf4: e5831024     	str	r1, [r3, #0x24]
  48acf8: e2833f4b     	add	r3, r3, #300
  48acfc: 1afffff7     	bne	0x48ace0 <rnd::Block::Block()+0xbc> @ imm = #-0x24
  48ad00: e1a00004     	mov	r0, r4
  48ad04: e8bd8070     	pop	{r4, r5, r6, pc}
  48ad08: 5c 9e 50 00  	.word	0x00509e5c
  48ad0c: 1c 3e 00 00  	.word	0x00003e1c
  48ad10: fc 43 00 00  	.word	0x000043fc
