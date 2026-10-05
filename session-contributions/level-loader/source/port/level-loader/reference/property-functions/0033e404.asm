
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0033e404 <void PropertyMap::AddProperty<std::string>(char const*, std::string&, std::string)>:
  33e404: e59fc098     	ldr	r12, [pc, #0x98]        @ 0x33e4a4 <void PropertyMap::AddProperty<std::string>(char const*, std::string&, std::string)+0xa0>
  33e408: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  33e40c: e59fe094     	ldr	lr, [pc, #0x94]         @ 0x33e4a8 <void PropertyMap::AddProperty<std::string>(char const*, std::string&, std::string)+0xa4>
  33e410: e08fc00c     	add	r12, pc, r12
  33e414: e24dd02c     	sub	sp, sp, #44
  33e418: e79c500e     	ldr	r5, [r12, lr]
  33e41c: e28d400c     	add	r4, sp, #12
  33e420: e1a07000     	mov	r7, r0
  33e424: e595e000     	ldr	lr, [r5]
  33e428: e1a06001     	mov	r6, r1
  33e42c: e1a0a002     	mov	r10, r2
  33e430: e5931014     	ldr	r1, [r3, #0x14]
  33e434: e5932010     	ldr	r2, [r3, #0x10]
  33e438: e1a00004     	mov	r0, r4
  33e43c: e58de024     	str	lr, [sp, #0x24]
  33e440: e58d401c     	str	r4, [sp, #0x1c]
  33e444: e58d4020     	str	r4, [sp, #0x20]
  33e448: ebff4ca6     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0x2cd68
  33e44c: e3a01000     	mov	r1, #0
  33e450: e3a00038     	mov	r0, #56
  33e454: ebff4845     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x2deec
  33e458: e1a0300a     	mov	r3, r10
  33e45c: e1a08000     	mov	r8, r0
  33e460: e1a01007     	mov	r1, r7
  33e464: e1a02006     	mov	r2, r6
  33e468: e58d4000     	str	r4, [sp]
  33e46c: ebffffc3     	bl	0x33e380 <SimpleTypeProperty<std::string>::SimpleTypeProperty(void*, char const*, std::string*, std::string)> @ imm = #-0xf4
  33e470: e1a02008     	mov	r2, r8
  33e474: e1a00007     	mov	r0, r7
  33e478: e1a01006     	mov	r1, r6
  33e47c: eb075618     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x1d5860
  33e480: e1a00004     	mov	r0, r4
  33e484: ebff5548     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x2aae0
  33e488: e59d2024     	ldr	r2, [sp, #0x24]
  33e48c: e5953000     	ldr	r3, [r5]
  33e490: e1520003     	cmp	r2, r3
  33e494: 1a000001     	bne	0x33e4a0 <void PropertyMap::AddProperty<std::string>(char const*, std::string&, std::string)+0x9c> @ imm = #0x4
  33e498: e28dd02c     	add	sp, sp, #44
  33e49c: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  33e4a0: ebff3f9a     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x30198
  33e4a4: 80 66 65 00  	.word	0x00656680
  33e4a8: ac 40 00 00  	.word	0x000040ac
