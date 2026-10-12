
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00385fd8 <GSLevel::~GSLevel()>:
  385fd8: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x386004 <GSLevel::~GSLevel()+0x2c>
  385fdc: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x386008 <GSLevel::~GSLevel()+0x30>
  385fe0: e92d4010     	push	{r4, lr}
  385fe4: e08f3003     	add	r3, pc, r3
  385fe8: e7932002     	ldr	r2, [r3, r2]
  385fec: e1a04000     	mov	r4, r0
  385ff0: e2822008     	add	r2, r2, #8
  385ff4: e4802004     	str	r2, [r0], #4
  385ff8: ebfe366b     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x72654
  385ffc: e1a00004     	mov	r0, r4
  386000: e8bd8010     	pop	{r4, pc}
  386004: ac ea 60 00  	.word	0x0060eaac
  386008: d8 0e 00 00  	.word	0x00000ed8
