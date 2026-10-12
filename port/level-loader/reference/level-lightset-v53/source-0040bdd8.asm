
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040bdd8 <LightPoint::LightPoint()>:
  40bdd8: e92d4070     	push	{r4, r5, r6, lr}
  40bddc: e3a01013     	mov	r1, #19
  40bde0: e59f506c     	ldr	r5, [pc, #0x6c]         @ 0x40be54 <LightPoint::LightPoint()+0x7c>
  40bde4: e1a04000     	mov	r4, r0
  40bde8: ebfffb9a     	bl	0x40ac58 <LightBase::LightBase(ObjectBase::GO_IDS)> @ imm = #-0x1198
  40bdec: e59f3064     	ldr	r3, [pc, #0x64]         @ 0x40be58 <LightPoint::LightPoint()+0x80>
  40bdf0: e08f5005     	add	r5, pc, r5
  40bdf4: e2842d06     	add	r2, r4, #384
  40bdf8: e7953003     	ldr	r3, [r5, r3]
  40bdfc: e1a00002     	mov	r0, r2
  40be00: e5842190     	str	r2, [r4, #0x190]
  40be04: e283c008     	add	r12, r3, #8
  40be08: e2831084     	add	r1, r3, #132
  40be0c: e2833078     	add	r3, r3, #120
  40be10: e584c000     	str	r12, [r4]
  40be14: e5843004     	str	r3, [r4, #0x4]
  40be18: e5841024     	str	r1, [r4, #0x24]
  40be1c: e5842194     	str	r2, [r4, #0x194]
  40be20: e3a01010     	mov	r1, #16
  40be24: ebfc1614     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0xfa7b0
  40be28: e5943190     	ldr	r3, [r4, #0x190]
  40be2c: e3a02000     	mov	r2, #0
  40be30: e2840f66     	add	r0, r4, #408
  40be34: e5c32000     	strb	r2, [r3]
  40be38: ebfccdb3     	bl	0x33f50c <ObjectHandle::ObjectHandle()> @ imm = #-0xcc934
  40be3c: e3a03000     	mov	r3, #0
  40be40: e1a00004     	mov	r0, r4
  40be44: e58431ac     	str	r3, [r4, #0x1ac]
  40be48: e58431a4     	str	r3, [r4, #0x1a4]
  40be4c: e58431a8     	str	r3, [r4, #0x1a8]
  40be50: e8bd8070     	pop	{r4, r5, r6, pc}
  40be54: a0 8c 58 00  	.word	0x00588ca0
  40be58: 10 25 00 00  	.word	0x00002510
