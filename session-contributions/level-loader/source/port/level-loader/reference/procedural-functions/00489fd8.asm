
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00489fd8 <rnd::Block::~Block()>:
  489fd8: e59f3054     	ldr	r3, [pc, #0x54]         @ 0x48a034 <rnd::Block::~Block()+0x5c>
  489fdc: e59f2054     	ldr	r2, [pc, #0x54]         @ 0x48a038 <rnd::Block::~Block()+0x60>
  489fe0: e92d4070     	push	{r4, r5, r6, lr}
  489fe4: e08f3003     	add	r3, pc, r3
  489fe8: e7932002     	ldr	r2, [r3, r2]
  489fec: e1a05000     	mov	r5, r0
  489ff0: e1a06000     	mov	r6, r0
  489ff4: e2822008     	add	r2, r2, #8
  489ff8: e2804d27     	add	r4, r0, #2496
  489ffc: e4852060     	str	r2, [r5], #96
  48a000: e2444f4b     	sub	r4, r4, #300
  48a004: e284001c     	add	r0, r4, #28
  48a008: ebfa27c8     	bl	0x313f30 <std::vector<std::string, std::allocator<std::string>>::~vector()> @ imm = #-0x1760e0
  48a00c: e1540005     	cmp	r4, r5
  48a010: 1afffffa     	bne	0x48a000 <rnd::Block::~Block()+0x28> @ imm = #-0x18
  48a014: e2860034     	add	r0, r6, #52
  48a018: ebfa2663     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x176674
  48a01c: e286001c     	add	r0, r6, #28
  48a020: ebfa2661     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x17667c
  48a024: e2860004     	add	r0, r6, #4
  48a028: ebfa265f     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x176684
  48a02c: e1a00006     	mov	r0, r6
  48a030: e8bd8070     	pop	{r4, r5, r6, pc}
  48a034: ac aa 50 00  	.word	0x0050aaac
  48a038: 1c 3e 00 00  	.word	0x00003e1c
