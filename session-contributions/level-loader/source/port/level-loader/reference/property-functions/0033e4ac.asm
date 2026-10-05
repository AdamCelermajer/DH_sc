
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0033e4ac <void PropertyMap::AddProperty<bool>(char const*, bool&, bool)>:
  33e4ac: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  33e4b0: e1a06000     	mov	r6, r0
  33e4b4: e24dd00c     	sub	sp, sp, #12
  33e4b8: e1a05001     	mov	r5, r1
  33e4bc: e3a00024     	mov	r0, #36
  33e4c0: e3a01000     	mov	r1, #0
  33e4c4: e1a0a003     	mov	r10, r3
  33e4c8: e1a07002     	mov	r7, r2
  33e4cc: ebff4827     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x2df64
  33e4d0: e59f4054     	ldr	r4, [pc, #0x54]         @ 0x33e52c <void PropertyMap::AddProperty<bool>(char const*, bool&, bool)+0x80>
  33e4d4: e59f3054     	ldr	r3, [pc, #0x54]         @ 0x33e530 <void PropertyMap::AddProperty<bool>(char const*, bool&, bool)+0x84>
  33e4d8: e1a08000     	mov	r8, r0
  33e4dc: e08f4004     	add	r4, pc, r4
  33e4e0: e7943003     	ldr	r3, [r4, r3]
  33e4e4: e1a01005     	mov	r1, r5
  33e4e8: e28d2004     	add	r2, sp, #4
  33e4ec: e2833008     	add	r3, r3, #8
  33e4f0: e4803008     	str	r3, [r0], #8
  33e4f4: ebff56fc     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x2a410
  33e4f8: e59f3034     	ldr	r3, [pc, #0x34]         @ 0x33e534 <void PropertyMap::AddProperty<bool>(char const*, bool&, bool)+0x88>
  33e4fc: e0667007     	rsb	r7, r6, r7
  33e500: e5887004     	str	r7, [r8, #0x4]
  33e504: e7943003     	ldr	r3, [r4, r3]
  33e508: e5c8a020     	strb	r10, [r8, #0x20]
  33e50c: e1a00006     	mov	r0, r6
  33e510: e2833008     	add	r3, r3, #8
  33e514: e5883000     	str	r3, [r8]
  33e518: e1a01005     	mov	r1, r5
  33e51c: e1a02008     	mov	r2, r8
  33e520: eb0755ef     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x1d57bc
  33e524: e28dd00c     	add	sp, sp, #12
  33e528: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  33e52c: b4 65 65 00  	.word	0x006565b4
  33e530: 30 23 00 00  	.word	0x00002330
  33e534: 4c 3e 00 00  	.word	0x00003e4c
