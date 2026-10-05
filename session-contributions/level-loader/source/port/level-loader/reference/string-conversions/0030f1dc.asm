
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0030f1dc <StrToObj(char const*, Point3D<float>&)>:
  30f1dc: e92d40f0     	push	{r4, r5, r6, r7, lr}
  30f1e0: e24dd00c     	sub	sp, sp, #12
  30f1e4: e3a0302c     	mov	r3, #44
  30f1e8: e1a07000     	mov	r7, r0
  30f1ec: e28d4008     	add	r4, sp, #8
  30f1f0: e3a00c01     	mov	r0, #256
  30f1f4: e16430b4     	strh	r3, [r4, #-4]!
  30f1f8: e1a05001     	mov	r5, r1
  30f1fc: eb000494     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #0x1250
  30f200: e1a01007     	mov	r1, r7
  30f204: e1a06000     	mov	r6, r0
  30f208: ebfffcc4     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0xcf0
  30f20c: e1a00006     	mov	r0, r6
  30f210: e1a01004     	mov	r1, r4
  30f214: ebfffb83     	bl	0x30e028 <.plt+0x2b4>   @ imm = #-0x11f4
  30f218: e3500000     	cmp	r0, #0
  30f21c: 0a000003     	beq	0x30f230 <StrToObj(char const*, Point3D<float>&)+0x54> @ imm = #0xc
  30f220: e3a01000     	mov	r1, #0
  30f224: ebfffd20     	bl	0x30e6ac <.plt+0x938>   @ imm = #-0xb80
  30f228: ebfffd1c     	bl	0x30e6a0 <.plt+0x92c>   @ imm = #-0xb90
  30f22c: e5850000     	str	r0, [r5]
  30f230: e3a00000     	mov	r0, #0
  30f234: e1a01004     	mov	r1, r4
  30f238: ebfffb7a     	bl	0x30e028 <.plt+0x2b4>   @ imm = #-0x1218
  30f23c: e3500000     	cmp	r0, #0
  30f240: 0a000003     	beq	0x30f254 <StrToObj(char const*, Point3D<float>&)+0x78> @ imm = #0xc
  30f244: e3a01000     	mov	r1, #0
  30f248: ebfffd17     	bl	0x30e6ac <.plt+0x938>   @ imm = #-0xba4
  30f24c: ebfffd13     	bl	0x30e6a0 <.plt+0x92c>   @ imm = #-0xbb4
  30f250: e5850004     	str	r0, [r5, #0x4]
  30f254: e1a01004     	mov	r1, r4
  30f258: e3a00000     	mov	r0, #0
  30f25c: ebfffb71     	bl	0x30e028 <.plt+0x2b4>   @ imm = #-0x123c
  30f260: e3500000     	cmp	r0, #0
  30f264: 0a000003     	beq	0x30f278 <StrToObj(char const*, Point3D<float>&)+0x9c> @ imm = #0xc
  30f268: e3a01000     	mov	r1, #0
  30f26c: ebfffd0e     	bl	0x30e6ac <.plt+0x938>   @ imm = #-0xbc8
  30f270: ebfffd0a     	bl	0x30e6a0 <.plt+0x92c>   @ imm = #-0xbd8
  30f274: e5850008     	str	r0, [r5, #0x8]
  30f278: e1a00006     	mov	r0, r6
  30f27c: eb00046f     	bl	0x310440 <CustomFree(void*)> @ imm = #0x11bc
  30f280: e28dd00c     	add	sp, sp, #12
  30f284: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
