
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0030f334 <StrToObj(char const*, glitch::core::vector3d<float>&)>:
  30f334: e92d40f0     	push	{r4, r5, r6, r7, lr}
  30f338: e24dd00c     	sub	sp, sp, #12
  30f33c: e3a0302c     	mov	r3, #44
  30f340: e1a07000     	mov	r7, r0
  30f344: e28d4008     	add	r4, sp, #8
  30f348: e3a00c01     	mov	r0, #256
  30f34c: e16430b4     	strh	r3, [r4, #-4]!
  30f350: e1a05001     	mov	r5, r1
  30f354: eb00043e     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #0x10f8
  30f358: e1a01007     	mov	r1, r7
  30f35c: e1a06000     	mov	r6, r0
  30f360: ebfffc6e     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0xe48
  30f364: e1a00006     	mov	r0, r6
  30f368: e1a01004     	mov	r1, r4
  30f36c: ebfffb2d     	bl	0x30e028 <.plt+0x2b4>   @ imm = #-0x134c
  30f370: e3500000     	cmp	r0, #0
  30f374: 0a000003     	beq	0x30f388 <StrToObj(char const*, glitch::core::vector3d<float>&)+0x54> @ imm = #0xc
  30f378: e3a01000     	mov	r1, #0
  30f37c: ebfffcca     	bl	0x30e6ac <.plt+0x938>   @ imm = #-0xcd8
  30f380: ebfffcc6     	bl	0x30e6a0 <.plt+0x92c>   @ imm = #-0xce8
  30f384: e5850000     	str	r0, [r5]
  30f388: e3a00000     	mov	r0, #0
  30f38c: e1a01004     	mov	r1, r4
  30f390: ebfffb24     	bl	0x30e028 <.plt+0x2b4>   @ imm = #-0x1370
  30f394: e3500000     	cmp	r0, #0
  30f398: 0a000003     	beq	0x30f3ac <StrToObj(char const*, glitch::core::vector3d<float>&)+0x78> @ imm = #0xc
  30f39c: e3a01000     	mov	r1, #0
  30f3a0: ebfffcc1     	bl	0x30e6ac <.plt+0x938>   @ imm = #-0xcfc
  30f3a4: ebfffcbd     	bl	0x30e6a0 <.plt+0x92c>   @ imm = #-0xd0c
  30f3a8: e5850004     	str	r0, [r5, #0x4]
  30f3ac: e1a01004     	mov	r1, r4
  30f3b0: e3a00000     	mov	r0, #0
  30f3b4: ebfffb1b     	bl	0x30e028 <.plt+0x2b4>   @ imm = #-0x1394
  30f3b8: e3500000     	cmp	r0, #0
  30f3bc: 0a000003     	beq	0x30f3d0 <StrToObj(char const*, glitch::core::vector3d<float>&)+0x9c> @ imm = #0xc
  30f3c0: e3a01000     	mov	r1, #0
  30f3c4: ebfffcb8     	bl	0x30e6ac <.plt+0x938>   @ imm = #-0xd20
  30f3c8: ebfffcb4     	bl	0x30e6a0 <.plt+0x92c>   @ imm = #-0xd30
  30f3cc: e5850008     	str	r0, [r5, #0x8]
  30f3d0: e1a00006     	mov	r0, r6
  30f3d4: eb000419     	bl	0x310440 <CustomFree(void*)> @ imm = #0x1064
  30f3d8: e28dd00c     	add	sp, sp, #12
  30f3dc: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
