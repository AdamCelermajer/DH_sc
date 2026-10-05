
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00489f58 <rnd::Block::~Block()>:
  489f58: e59f3054     	ldr	r3, [pc, #0x54]         @ 0x489fb4 <rnd::Block::~Block()+0x5c>
  489f5c: e59f2054     	ldr	r2, [pc, #0x54]         @ 0x489fb8 <rnd::Block::~Block()+0x60>
  489f60: e92d4070     	push	{r4, r5, r6, lr}
  489f64: e08f3003     	add	r3, pc, r3
  489f68: e7932002     	ldr	r2, [r3, r2]
  489f6c: e1a05000     	mov	r5, r0
  489f70: e1a06000     	mov	r6, r0
  489f74: e2822008     	add	r2, r2, #8
  489f78: e2804d27     	add	r4, r0, #2496
  489f7c: e4852060     	str	r2, [r5], #96
  489f80: e2444f4b     	sub	r4, r4, #300
  489f84: e284001c     	add	r0, r4, #28
  489f88: ebfa27e8     	bl	0x313f30 <std::vector<std::string, std::allocator<std::string>>::~vector()> @ imm = #-0x176060
  489f8c: e1540005     	cmp	r4, r5
  489f90: 1afffffa     	bne	0x489f80 <rnd::Block::~Block()+0x28> @ imm = #-0x18
  489f94: e2860034     	add	r0, r6, #52
  489f98: ebfa2683     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x1765f4
  489f9c: e286001c     	add	r0, r6, #28
  489fa0: ebfa2681     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x1765fc
  489fa4: e2860004     	add	r0, r6, #4
  489fa8: ebfa267f     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x176604
  489fac: e1a00006     	mov	r0, r6
  489fb0: e8bd8070     	pop	{r4, r5, r6, pc}
  489fb4: 2c ab 50 00  	.word	0x0050ab2c
  489fb8: 1c 3e 00 00  	.word	0x00003e1c
