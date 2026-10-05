
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00389894 <void PropertyMap::AddProperty<Point3D<float>>(char const*, Point3D<float>&, Point3D<float>)>:
  389894: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  389898: e1a07000     	mov	r7, r0
  38989c: e24dd00c     	sub	sp, sp, #12
  3898a0: e1a06001     	mov	r6, r1
  3898a4: e3a0002c     	mov	r0, #44
  3898a8: e3a01000     	mov	r1, #0
  3898ac: e593b008     	ldr	r11, [r3, #0x8]
  3898b0: e5938000     	ldr	r8, [r3]
  3898b4: e5939004     	ldr	r9, [r3, #0x4]
  3898b8: e1a0a002     	mov	r10, r2
  3898bc: ebfe1b2b     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x79354
  3898c0: e59f505c     	ldr	r5, [pc, #0x5c]         @ 0x389924 <void PropertyMap::AddProperty<Point3D<float>>(char const*, Point3D<float>&, Point3D<float>)+0x90>
  3898c4: e59f305c     	ldr	r3, [pc, #0x5c]         @ 0x389928 <void PropertyMap::AddProperty<Point3D<float>>(char const*, Point3D<float>&, Point3D<float>)+0x94>
  3898c8: e1a04000     	mov	r4, r0
  3898cc: e08f5005     	add	r5, pc, r5
  3898d0: e7953003     	ldr	r3, [r5, r3]
  3898d4: e1a01006     	mov	r1, r6
  3898d8: e28d2004     	add	r2, sp, #4
  3898dc: e2833008     	add	r3, r3, #8
  3898e0: e4803008     	str	r3, [r0], #8
  3898e4: ebfe2a00     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x75800
  3898e8: e59f303c     	ldr	r3, [pc, #0x3c]         @ 0x38992c <void PropertyMap::AddProperty<Point3D<float>>(char const*, Point3D<float>&, Point3D<float>)+0x98>
  3898ec: e067a00a     	rsb	r10, r7, r10
  3898f0: e584a004     	str	r10, [r4, #0x4]
  3898f4: e7953003     	ldr	r3, [r5, r3]
  3898f8: e5848020     	str	r8, [r4, #0x20]
  3898fc: e5849024     	str	r9, [r4, #0x24]
  389900: e2833008     	add	r3, r3, #8
  389904: e5843000     	str	r3, [r4]
  389908: e584b028     	str	r11, [r4, #0x28]
  38990c: e1a00007     	mov	r0, r7
  389910: e1a01006     	mov	r1, r6
  389914: e1a02004     	mov	r2, r4
  389918: eb0628f1     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x18a3c4
  38991c: e28dd00c     	add	sp, sp, #12
  389920: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  389924: c4 b1 60 00  	.word	0x0060b1c4
  389928: 30 23 00 00  	.word	0x00002330
  38992c: 44 0b 00 00  	.word	0x00000b44
