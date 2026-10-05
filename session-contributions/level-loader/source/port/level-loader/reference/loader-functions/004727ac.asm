
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004727ac <VisualObject::SetScaling(Point3D<float> const&)>:
  4727ac: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  4727b0: e5903008     	ldr	r3, [r0, #0x8]
  4727b4: e24dd010     	sub	sp, sp, #16
  4727b8: e1a05000     	mov	r5, r0
  4727bc: e3530000     	cmp	r3, #0
  4727c0: e1a04001     	mov	r4, r1
  4727c4: 0a000018     	beq	0x47282c <VisualObject::SetScaling(Point3D<float> const&)+0x80> @ imm = #0x60
  4727c8: e1a00003     	mov	r0, r3
  4727cc: e5933000     	ldr	r3, [r3]
  4727d0: e1a0e00f     	mov	lr, pc
  4727d4: e593f090     	ldr	pc, [r3, #0x90]
  4727d8: e5947000     	ldr	r7, [r4]
  4727dc: e5901000     	ldr	r1, [r0]
  4727e0: e1a06000     	mov	r6, r0
  4727e4: e1a00007     	mov	r0, r7
  4727e8: ebfa6de7     	bl	0x30df8c <.plt+0x218>   @ imm = #-0x164864
  4727ec: e3500000     	cmp	r0, #0
  4727f0: e5948008     	ldr	r8, [r4, #0x8]
  4727f4: e5944004     	ldr	r4, [r4, #0x4]
  4727f8: 1a00000d     	bne	0x472834 <VisualObject::SetScaling(Point3D<float> const&)+0x88> @ imm = #0x34
  4727fc: e5950008     	ldr	r0, [r5, #0x8]
  472800: e28d1004     	add	r1, sp, #4
  472804: e5903000     	ldr	r3, [r0]
  472808: e5933094     	ldr	r3, [r3, #0x94]
  47280c: e58d7004     	str	r7, [sp, #0x4]
  472810: e58d4008     	str	r4, [sp, #0x8]
  472814: e58d800c     	str	r8, [sp, #0xc]
  472818: e12fff33     	blx	r3
  47281c: e1a00005     	mov	r0, r5
  472820: ebfffe3d     	bl	0x47211c <VisualObject::CalcMeshBox()> @ imm = #-0x70c
  472824: e1a00005     	mov	r0, r5
  472828: ebfff889     	bl	0x470a54 <VisualObject::ApplyMeshBox()> @ imm = #-0x1ddc
  47282c: e28dd010     	add	sp, sp, #16
  472830: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  472834: e1a00004     	mov	r0, r4
  472838: e5961004     	ldr	r1, [r6, #0x4]
  47283c: ebfa6dd2     	bl	0x30df8c <.plt+0x218>   @ imm = #-0x1648b8
  472840: e3500000     	cmp	r0, #0
  472844: 0affffec     	beq	0x4727fc <VisualObject::SetScaling(Point3D<float> const&)+0x50> @ imm = #-0x50
  472848: e5961008     	ldr	r1, [r6, #0x8]
  47284c: e1a00008     	mov	r0, r8
  472850: ebfa6dcd     	bl	0x30df8c <.plt+0x218>   @ imm = #-0x1648cc
  472854: e3500000     	cmp	r0, #0
  472858: 1afffff3     	bne	0x47282c <VisualObject::SetScaling(Point3D<float> const&)+0x80> @ imm = #-0x34
  47285c: eaffffe6     	b	0x4727fc <VisualObject::SetScaling(Point3D<float> const&)+0x50> @ imm = #-0x68
