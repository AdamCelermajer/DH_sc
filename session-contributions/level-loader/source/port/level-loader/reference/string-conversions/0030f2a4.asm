
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0030f2a4 <StrToObj(char const*, glitch::core::dimension2d<float>&)>:
  30f2a4: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  30f2a8: e24dd00c     	sub	sp, sp, #12
  30f2ac: e3a0302c     	mov	r3, #44
  30f2b0: e1a07000     	mov	r7, r0
  30f2b4: e28d4008     	add	r4, sp, #8
  30f2b8: e3a00c01     	mov	r0, #256
  30f2bc: e16430b4     	strh	r3, [r4, #-4]!
  30f2c0: e1a05001     	mov	r5, r1
  30f2c4: eb000462     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #0x1188
  30f2c8: e1a01007     	mov	r1, r7
  30f2cc: e1a06000     	mov	r6, r0
  30f2d0: ebfffc92     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0xdb8
  30f2d4: e1a00006     	mov	r0, r6
  30f2d8: e1a01004     	mov	r1, r4
  30f2dc: ebfffb51     	bl	0x30e028 <.plt+0x2b4>   @ imm = #-0x12bc
  30f2e0: e3500000     	cmp	r0, #0
  30f2e4: 0a000003     	beq	0x30f2f8 <StrToObj(char const*, glitch::core::dimension2d<float>&)+0x54> @ imm = #0xc
  30f2e8: e3a01000     	mov	r1, #0
  30f2ec: ebfffcee     	bl	0x30e6ac <.plt+0x938>   @ imm = #-0xc48
  30f2f0: ebfffcea     	bl	0x30e6a0 <.plt+0x92c>   @ imm = #-0xc58
  30f2f4: e1a0a000     	mov	r10, r0
  30f2f8: e1a01004     	mov	r1, r4
  30f2fc: e3a00000     	mov	r0, #0
  30f300: ebfffb48     	bl	0x30e028 <.plt+0x2b4>   @ imm = #-0x12e0
  30f304: e3500000     	cmp	r0, #0
  30f308: 0a000003     	beq	0x30f31c <StrToObj(char const*, glitch::core::dimension2d<float>&)+0x78> @ imm = #0xc
  30f30c: e3a01000     	mov	r1, #0
  30f310: ebfffce5     	bl	0x30e6ac <.plt+0x938>   @ imm = #-0xc6c
  30f314: ebfffce1     	bl	0x30e6a0 <.plt+0x92c>   @ imm = #-0xc7c
  30f318: e1a08000     	mov	r8, r0
  30f31c: e1a00006     	mov	r0, r6
  30f320: eb000446     	bl	0x310440 <CustomFree(void*)> @ imm = #0x1118
  30f324: e5858004     	str	r8, [r5, #0x4]
  30f328: e585a000     	str	r10, [r5]
  30f32c: e28dd00c     	add	sp, sp, #12
  30f330: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
