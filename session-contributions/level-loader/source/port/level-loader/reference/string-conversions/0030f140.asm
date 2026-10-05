
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0030f140 <StrToObj(char const*, Point3D<int>&)>:
  30f140: e92d40f0     	push	{r4, r5, r6, r7, lr}
  30f144: e24dd00c     	sub	sp, sp, #12
  30f148: e3a0302c     	mov	r3, #44
  30f14c: e1a07000     	mov	r7, r0
  30f150: e28d4008     	add	r4, sp, #8
  30f154: e3a00c01     	mov	r0, #256
  30f158: e16430b4     	strh	r3, [r4, #-4]!
  30f15c: e1a05001     	mov	r5, r1
  30f160: eb0004bb     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #0x12ec
  30f164: e1a01007     	mov	r1, r7
  30f168: e1a06000     	mov	r6, r0
  30f16c: ebfffceb     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0xc54
  30f170: e1a00006     	mov	r0, r6
  30f174: e1a01004     	mov	r1, r4
  30f178: ebfffbaa     	bl	0x30e028 <.plt+0x2b4>   @ imm = #-0x1158
  30f17c: e3500000     	cmp	r0, #0
  30f180: 0a000003     	beq	0x30f194 <StrToObj(char const*, Point3D<int>&)+0x54> @ imm = #0xc
  30f184: e3a01000     	mov	r1, #0
  30f188: ebfffd47     	bl	0x30e6ac <.plt+0x938>   @ imm = #-0xae4
  30f18c: ebfffe24     	bl	0x30ea24 <.plt+0xcb0>   @ imm = #-0x770
  30f190: e5850000     	str	r0, [r5]
  30f194: e3a00000     	mov	r0, #0
  30f198: e1a01004     	mov	r1, r4
  30f19c: ebfffba1     	bl	0x30e028 <.plt+0x2b4>   @ imm = #-0x117c
  30f1a0: e3500000     	cmp	r0, #0
  30f1a4: 0a000001     	beq	0x30f1b0 <StrToObj(char const*, Point3D<int>&)+0x70> @ imm = #0x4
  30f1a8: ebfffbb9     	bl	0x30e094 <.plt+0x320>   @ imm = #-0x111c
  30f1ac: e5850004     	str	r0, [r5, #0x4]
  30f1b0: e1a01004     	mov	r1, r4
  30f1b4: e3a00000     	mov	r0, #0
  30f1b8: ebfffb9a     	bl	0x30e028 <.plt+0x2b4>   @ imm = #-0x1198
  30f1bc: e3500000     	cmp	r0, #0
  30f1c0: 0a000001     	beq	0x30f1cc <StrToObj(char const*, Point3D<int>&)+0x8c> @ imm = #0x4
  30f1c4: ebfffbb2     	bl	0x30e094 <.plt+0x320>   @ imm = #-0x1138
  30f1c8: e5850008     	str	r0, [r5, #0x8]
  30f1cc: e1a00006     	mov	r0, r6
  30f1d0: eb00049a     	bl	0x310440 <CustomFree(void*)> @ imm = #0x1268
  30f1d4: e28dd00c     	add	sp, sp, #12
  30f1d8: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
