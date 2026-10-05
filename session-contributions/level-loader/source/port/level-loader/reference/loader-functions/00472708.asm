
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00472708 <VisualObject::SetScaling(float)>:
  472708: e92d4030     	push	{r4, r5, lr}
  47270c: e5903008     	ldr	r3, [r0, #0x8]
  472710: e24dd014     	sub	sp, sp, #20
  472714: e1a04000     	mov	r4, r0
  472718: e3530000     	cmp	r3, #0
  47271c: 0a000020     	beq	0x4727a4 <VisualObject::SetScaling(float)+0x9c> @ imm = #0x80
  472720: e58d100c     	str	r1, [sp, #0xc]
  472724: e58d1004     	str	r1, [sp, #0x4]
  472728: e58d1008     	str	r1, [sp, #0x8]
  47272c: e1a00003     	mov	r0, r3
  472730: e5933000     	ldr	r3, [r3]
  472734: e1a0e00f     	mov	lr, pc
  472738: e593f090     	ldr	pc, [r3, #0x90]
  47273c: e59d1004     	ldr	r1, [sp, #0x4]
  472740: e1a05000     	mov	r5, r0
  472744: e5900000     	ldr	r0, [r0]
  472748: ebfa6e0f     	bl	0x30df8c <.plt+0x218>   @ imm = #-0x1647c4
  47274c: e3500000     	cmp	r0, #0
  472750: 0a000009     	beq	0x47277c <VisualObject::SetScaling(float)+0x74> @ imm = #0x24
  472754: e5950004     	ldr	r0, [r5, #0x4]
  472758: e59d1008     	ldr	r1, [sp, #0x8]
  47275c: ebfa6e0a     	bl	0x30df8c <.plt+0x218>   @ imm = #-0x1647d8
  472760: e3500000     	cmp	r0, #0
  472764: 0a000004     	beq	0x47277c <VisualObject::SetScaling(float)+0x74> @ imm = #0x10
  472768: e5950008     	ldr	r0, [r5, #0x8]
  47276c: e59d100c     	ldr	r1, [sp, #0xc]
  472770: ebfa6e05     	bl	0x30df8c <.plt+0x218>   @ imm = #-0x1647ec
  472774: e3500000     	cmp	r0, #0
  472778: 1a000009     	bne	0x4727a4 <VisualObject::SetScaling(float)+0x9c> @ imm = #0x24
  47277c: e5943008     	ldr	r3, [r4, #0x8]
  472780: e28d1004     	add	r1, sp, #4
  472784: e1a00003     	mov	r0, r3
  472788: e5933000     	ldr	r3, [r3]
  47278c: e1a0e00f     	mov	lr, pc
  472790: e593f094     	ldr	pc, [r3, #0x94]
  472794: e1a00004     	mov	r0, r4
  472798: ebfffe5f     	bl	0x47211c <VisualObject::CalcMeshBox()> @ imm = #-0x684
  47279c: e1a00004     	mov	r0, r4
  4727a0: ebfff8ab     	bl	0x470a54 <VisualObject::ApplyMeshBox()> @ imm = #-0x1d54
  4727a4: e28dd014     	add	sp, sp, #20
  4727a8: e8bd8030     	pop	{r4, r5, pc}
