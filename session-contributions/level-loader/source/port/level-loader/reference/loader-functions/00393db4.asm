
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00393db4 <GameObject::SetPosition(Point3D<float> const&, bool)>:
  393db4: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  393db8: e59052e0     	ldr	r5, [r0, #0x2e0]
  393dbc: e1a04000     	mov	r4, r0
  393dc0: e1a06001     	mov	r6, r1
  393dc4: e3550000     	cmp	r5, #0
  393dc8: e1a07002     	mov	r7, r2
  393dcc: 0a000016     	beq	0x393e2c <GameObject::SetPosition(Point3D<float> const&, bool)+0x78> @ imm = #0x58
  393dd0: e5901164     	ldr	r1, [r0, #0x164]
  393dd4: e5960004     	ldr	r0, [r6, #0x4]
  393dd8: ebfde973     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x85a34
  393ddc: e5941168     	ldr	r1, [r4, #0x168]
  393de0: e1a0a000     	mov	r10, r0
  393de4: e5960008     	ldr	r0, [r6, #0x8]
  393de8: ebfde96f     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x85a44
  393dec: e5941160     	ldr	r1, [r4, #0x160]
  393df0: e1a08000     	mov	r8, r0
  393df4: e5960000     	ldr	r0, [r6]
  393df8: ebfde96b     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x85a54
  393dfc: e1a01000     	mov	r1, r0
  393e00: e595000c     	ldr	r0, [r5, #0xc]
  393e04: ebfdeb66     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x85268
  393e08: e1a0100a     	mov	r1, r10
  393e0c: e585000c     	str	r0, [r5, #0xc]
  393e10: e5950010     	ldr	r0, [r5, #0x10]
  393e14: ebfdeb62     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x85278
  393e18: e1a01008     	mov	r1, r8
  393e1c: e5850010     	str	r0, [r5, #0x10]
  393e20: e5950014     	ldr	r0, [r5, #0x14]
  393e24: ebfdeb5e     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x85288
  393e28: e5850014     	str	r0, [r5, #0x14]
  393e2c: e5963000     	ldr	r3, [r6]
  393e30: e1a00004     	mov	r0, r4
  393e34: e5843160     	str	r3, [r4, #0x160]
  393e38: e5963004     	ldr	r3, [r6, #0x4]
  393e3c: e5843164     	str	r3, [r4, #0x164]
  393e40: e5963008     	ldr	r3, [r6, #0x8]
  393e44: e5843168     	str	r3, [r4, #0x168]
  393e48: ebffdb1e     	bl	0x38aac8 <GameObject::UpdateAbsoluteAABB()> @ imm = #-0x9388
  393e4c: e59402dc     	ldr	r0, [r4, #0x2dc]
  393e50: e3500000     	cmp	r0, #0
  393e54: 0a000002     	beq	0x393e64 <GameObject::SetPosition(Point3D<float> const&, bool)+0xb0> @ imm = #0x8
  393e58: e5941160     	ldr	r1, [r4, #0x160]
  393e5c: e5942164     	ldr	r2, [r4, #0x164]
  393e60: eb036b06     	bl	0x46ea80 <PhysicalObject::setPosition(float, float)> @ imm = #0xdac18
  393e64: e59402d8     	ldr	r0, [r4, #0x2d8]
  393e68: e3500000     	cmp	r0, #0
  393e6c: 0a000000     	beq	0x393e74 <GameObject::SetPosition(Point3D<float> const&, bool)+0xc0> @ imm = #0x0
  393e70: eb037390     	bl	0x470cb8 <VisualObject::SyncPosition()> @ imm = #0xdce40
  393e74: e3570000     	cmp	r7, #0
  393e78: 1a000000     	bne	0x393e80 <GameObject::SetPosition(Point3D<float> const&, bool)+0xcc> @ imm = #0x0
  393e7c: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  393e80: e1a00004     	mov	r0, r4
  393e84: e1a01006     	mov	r1, r6
  393e88: e8bd47f0     	pop	{r4, r5, r6, r7, r8, r9, r10, lr}
  393e8c: eafffddb     	b	0x393600 <GameObject::SetDestination(Point3D<float> const&)> @ imm = #-0x894
