
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00385f88 <GSLevel::~GSLevel()>:
  385f88: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x385fb4 <GSLevel::~GSLevel()+0x2c>
  385f8c: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x385fb8 <GSLevel::~GSLevel()+0x30>
  385f90: e92d4010     	push	{r4, lr}
  385f94: e08f3003     	add	r3, pc, r3
  385f98: e7932002     	ldr	r2, [r3, r2]
  385f9c: e1a04000     	mov	r4, r0
  385fa0: e2822008     	add	r2, r2, #8
  385fa4: e4802004     	str	r2, [r0], #4
  385fa8: ebfe367f     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x72604
  385fac: e1a00004     	mov	r0, r4
  385fb0: e8bd8010     	pop	{r4, pc}
  385fb4: fc ea 60 00  	.word	0x0060eafc
  385fb8: d8 0e 00 00  	.word	0x00000ed8
