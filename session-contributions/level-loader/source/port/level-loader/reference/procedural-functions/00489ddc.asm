
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00489ddc <rnd::Exit::GetBlockUnitPosition(Point3D<float>&, rnd::Block*)>:
  489ddc: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  489de0: e1a04002     	mov	r4, r2
  489de4: e1a05000     	mov	r5, r0
  489de8: e5920058     	ldr	r0, [r2, #0x58]
  489dec: e1a07001     	mov	r7, r1
  489df0: ebfa12db     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x17b494
  489df4: e5941050     	ldr	r1, [r4, #0x50]
  489df8: ebfa13db     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x17b094
  489dfc: e3a0143f     	mov	r1, #1056964608
  489e00: ebfa13d9     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x17b09c
  489e04: e1a08000     	mov	r8, r0
  489e08: e5940054     	ldr	r0, [r4, #0x54]
  489e0c: e594604c     	ldr	r6, [r4, #0x4c]
  489e10: e2600000     	rsb	r0, r0, #0
  489e14: ebfa12d2     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x17b4b8
  489e18: e1a01006     	mov	r1, r6
  489e1c: ebfa13d2     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x17b0b8
  489e20: e3a0143f     	mov	r1, #1056964608
  489e24: ebfa13d0     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x17b0c0
  489e28: e1a01000     	mov	r1, r0
  489e2c: e5970000     	ldr	r0, [r7]
  489e30: ebfa115d     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x17ba8c
  489e34: e1a01006     	mov	r1, r6
  489e38: ebfa1395     	bl	0x30ec94 <.plt+0xf20>   @ imm = #-0x17b1ac
  489e3c: ebfa11b4     	bl	0x30e514 <.plt+0x7a0>   @ imm = #-0x17b930
  489e40: e3a015fe     	mov	r1, #1065353216
  489e44: ebfa1158     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x17baa0
  489e48: ebfa119f     	bl	0x30e4cc <.plt+0x758>   @ imm = #-0x17b984
  489e4c: e5850008     	str	r0, [r5, #0x8]
  489e50: e5971004     	ldr	r1, [r7, #0x4]
  489e54: e1a06000     	mov	r6, r0
  489e58: e1a00008     	mov	r0, r8
  489e5c: ebfa1152     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x17bab8
  489e60: e5941050     	ldr	r1, [r4, #0x50]
  489e64: ebfa138a     	bl	0x30ec94 <.plt+0xf20>   @ imm = #-0x17b1d8
  489e68: e1c66fc6     	bic	r6, r6, r6, asr #31
  489e6c: ebfa11a8     	bl	0x30e514 <.plt+0x7a0>   @ imm = #-0x17b960
  489e70: e3a015fe     	mov	r1, #1065353216
  489e74: e5856008     	str	r6, [r5, #0x8]
  489e78: ebfa114b     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x17bad4
  489e7c: ebfa1192     	bl	0x30e4cc <.plt+0x758>   @ imm = #-0x17b9b8
  489e80: e1c00fc0     	bic	r0, r0, r0, asr #31
  489e84: e585000c     	str	r0, [r5, #0xc]
  489e88: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
