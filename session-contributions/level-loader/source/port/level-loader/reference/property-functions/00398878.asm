
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00398878 <void PropertyMap::AddProperty<int>(char const*, int&, int)>:
  398878: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  39887c: e1a06000     	mov	r6, r0
  398880: e24dd00c     	sub	sp, sp, #12
  398884: e1a05001     	mov	r5, r1
  398888: e3a00024     	mov	r0, #36
  39888c: e3a01000     	mov	r1, #0
  398890: e1a0a003     	mov	r10, r3
  398894: e1a07002     	mov	r7, r2
  398898: ebfddf34     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x88330
  39889c: e59f4054     	ldr	r4, [pc, #0x54]         @ 0x3988f8 <void PropertyMap::AddProperty<int>(char const*, int&, int)+0x80>
  3988a0: e59f3054     	ldr	r3, [pc, #0x54]         @ 0x3988fc <void PropertyMap::AddProperty<int>(char const*, int&, int)+0x84>
  3988a4: e1a08000     	mov	r8, r0
  3988a8: e08f4004     	add	r4, pc, r4
  3988ac: e7943003     	ldr	r3, [r4, r3]
  3988b0: e1a01005     	mov	r1, r5
  3988b4: e28d2004     	add	r2, sp, #4
  3988b8: e2833008     	add	r3, r3, #8
  3988bc: e4803008     	str	r3, [r0], #8
  3988c0: ebfdee09     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x847dc
  3988c4: e59f3034     	ldr	r3, [pc, #0x34]         @ 0x398900 <void PropertyMap::AddProperty<int>(char const*, int&, int)+0x88>
  3988c8: e0667007     	rsb	r7, r6, r7
  3988cc: e5887004     	str	r7, [r8, #0x4]
  3988d0: e7943003     	ldr	r3, [r4, r3]
  3988d4: e588a020     	str	r10, [r8, #0x20]
  3988d8: e1a00006     	mov	r0, r6
  3988dc: e2833008     	add	r3, r3, #8
  3988e0: e5883000     	str	r3, [r8]
  3988e4: e1a01005     	mov	r1, r5
  3988e8: e1a02008     	mov	r2, r8
  3988ec: eb05ecfc     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x17b3f0
  3988f0: e28dd00c     	add	sp, sp, #12
  3988f4: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  3988f8: e8 c1 5f 00  	.word	0x005fc1e8
  3988fc: 30 23 00 00  	.word	0x00002330
  398900: 90 25 00 00  	.word	0x00002590
