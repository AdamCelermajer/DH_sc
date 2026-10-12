
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040be5c <LightPoint::LightPoint()>:
  40be5c: e92d4070     	push	{r4, r5, r6, lr}
  40be60: e3a01013     	mov	r1, #19
  40be64: e59f506c     	ldr	r5, [pc, #0x6c]         @ 0x40bed8 <LightPoint::LightPoint()+0x7c>
  40be68: e1a04000     	mov	r4, r0
  40be6c: ebfffb79     	bl	0x40ac58 <LightBase::LightBase(ObjectBase::GO_IDS)> @ imm = #-0x121c
  40be70: e59f3064     	ldr	r3, [pc, #0x64]         @ 0x40bedc <LightPoint::LightPoint()+0x80>
  40be74: e08f5005     	add	r5, pc, r5
  40be78: e2842d06     	add	r2, r4, #384
  40be7c: e7953003     	ldr	r3, [r5, r3]
  40be80: e1a00002     	mov	r0, r2
  40be84: e5842190     	str	r2, [r4, #0x190]
  40be88: e283c008     	add	r12, r3, #8
  40be8c: e2831084     	add	r1, r3, #132
  40be90: e2833078     	add	r3, r3, #120
  40be94: e584c000     	str	r12, [r4]
  40be98: e5843004     	str	r3, [r4, #0x4]
  40be9c: e5841024     	str	r1, [r4, #0x24]
  40bea0: e5842194     	str	r2, [r4, #0x194]
  40bea4: e3a01010     	mov	r1, #16
  40bea8: ebfc15f3     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0xfa834
  40beac: e5943190     	ldr	r3, [r4, #0x190]
  40beb0: e3a02000     	mov	r2, #0
  40beb4: e2840f66     	add	r0, r4, #408
  40beb8: e5c32000     	strb	r2, [r3]
  40bebc: ebfccd92     	bl	0x33f50c <ObjectHandle::ObjectHandle()> @ imm = #-0xcc9b8
  40bec0: e3a03000     	mov	r3, #0
  40bec4: e1a00004     	mov	r0, r4
  40bec8: e58431ac     	str	r3, [r4, #0x1ac]
  40becc: e58431a4     	str	r3, [r4, #0x1a4]
  40bed0: e58431a8     	str	r3, [r4, #0x1a8]
  40bed4: e8bd8070     	pop	{r4, r5, r6, pc}
  40bed8: 1c 8c 58 00  	.word	0x00588c1c
  40bedc: 10 25 00 00  	.word	0x00002510
