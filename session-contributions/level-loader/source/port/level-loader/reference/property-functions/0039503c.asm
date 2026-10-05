
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039503c <void PropertyMap::AddProperty<float>(char const*, float&, float)>:
  39503c: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  395040: e1a06000     	mov	r6, r0
  395044: e24dd00c     	sub	sp, sp, #12
  395048: e1a05001     	mov	r5, r1
  39504c: e3a00024     	mov	r0, #36
  395050: e3a01000     	mov	r1, #0
  395054: e1a0a003     	mov	r10, r3
  395058: e1a07002     	mov	r7, r2
  39505c: ebfded43     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x84af4
  395060: e59f4054     	ldr	r4, [pc, #0x54]         @ 0x3950bc <void PropertyMap::AddProperty<float>(char const*, float&, float)+0x80>
  395064: e59f3054     	ldr	r3, [pc, #0x54]         @ 0x3950c0 <void PropertyMap::AddProperty<float>(char const*, float&, float)+0x84>
  395068: e1a08000     	mov	r8, r0
  39506c: e08f4004     	add	r4, pc, r4
  395070: e7943003     	ldr	r3, [r4, r3]
  395074: e1a01005     	mov	r1, r5
  395078: e28d2004     	add	r2, sp, #4
  39507c: e2833008     	add	r3, r3, #8
  395080: e4803008     	str	r3, [r0], #8
  395084: ebfdfc18     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x80fa0
  395088: e59f3034     	ldr	r3, [pc, #0x34]         @ 0x3950c4 <void PropertyMap::AddProperty<float>(char const*, float&, float)+0x88>
  39508c: e0667007     	rsb	r7, r6, r7
  395090: e5887004     	str	r7, [r8, #0x4]
  395094: e7943003     	ldr	r3, [r4, r3]
  395098: e588a020     	str	r10, [r8, #0x20]
  39509c: e1a00006     	mov	r0, r6
  3950a0: e2833008     	add	r3, r3, #8
  3950a4: e5883000     	str	r3, [r8]
  3950a8: e1a01005     	mov	r1, r5
  3950ac: e1a02008     	mov	r2, r8
  3950b0: eb05fb0b     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x17ec2c
  3950b4: e28dd00c     	add	sp, sp, #12
  3950b8: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  3950bc: 24 fa 5f 00  	.word	0x005ffa24
  3950c0: 30 23 00 00  	.word	0x00002330
  3950c4: bc 24 00 00  	.word	0x000024bc
