
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040bd1c <LightPoint::~LightPoint()>:
  40bd1c: e59f203c     	ldr	r2, [pc, #0x3c]         @ 0x40bd60 <LightPoint::~LightPoint()+0x44>
  40bd20: e59f303c     	ldr	r3, [pc, #0x3c]         @ 0x40bd64 <LightPoint::~LightPoint()+0x48>
  40bd24: e92d4010     	push	{r4, lr}
  40bd28: e08f2002     	add	r2, pc, r2
  40bd2c: e7923003     	ldr	r3, [r2, r3]
  40bd30: e1a04000     	mov	r4, r0
  40bd34: e2800d06     	add	r0, r0, #384
  40bd38: e2832084     	add	r2, r3, #132
  40bd3c: e2831008     	add	r1, r3, #8
  40bd40: e2833078     	add	r3, r3, #120
  40bd44: e884000a     	stm	r4, {r1, r3}
  40bd48: e5842024     	str	r2, [r4, #0x24]
  40bd4c: ebfc1f16     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xf83a8
  40bd50: e1a00004     	mov	r0, r4
  40bd54: ebfffb76     	bl	0x40ab34 <LightBase::~LightBase()> @ imm = #-0x1228
  40bd58: e1a00004     	mov	r0, r4
  40bd5c: e8bd8010     	pop	{r4, pc}
  40bd60: 68 8d 58 00  	.word	0x00588d68
  40bd64: 10 25 00 00  	.word	0x00002510
