
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040bd8c <LightPoint::~LightPoint()>:
  40bd8c: e59f203c     	ldr	r2, [pc, #0x3c]         @ 0x40bdd0 <LightPoint::~LightPoint()+0x44>
  40bd90: e59f303c     	ldr	r3, [pc, #0x3c]         @ 0x40bdd4 <LightPoint::~LightPoint()+0x48>
  40bd94: e92d4010     	push	{r4, lr}
  40bd98: e08f2002     	add	r2, pc, r2
  40bd9c: e7923003     	ldr	r3, [r2, r3]
  40bda0: e1a04000     	mov	r4, r0
  40bda4: e2800d06     	add	r0, r0, #384
  40bda8: e2832084     	add	r2, r3, #132
  40bdac: e2831008     	add	r1, r3, #8
  40bdb0: e2833078     	add	r3, r3, #120
  40bdb4: e884000a     	stm	r4, {r1, r3}
  40bdb8: e5842024     	str	r2, [r4, #0x24]
  40bdbc: ebfc1efa     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xf8418
  40bdc0: e1a00004     	mov	r0, r4
  40bdc4: ebfffb5a     	bl	0x40ab34 <LightBase::~LightBase()> @ imm = #-0x1298
  40bdc8: e1a00004     	mov	r0, r4
  40bdcc: e8bd8010     	pop	{r4, pc}
  40bdd0: f8 8c 58 00  	.word	0x00588cf8
  40bdd4: 10 25 00 00  	.word	0x00002510
