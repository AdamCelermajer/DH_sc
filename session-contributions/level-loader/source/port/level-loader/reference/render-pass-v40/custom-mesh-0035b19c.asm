
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0035b19c <BaseMeshSceneNode<glitch::collada::CMeshSceneNode>::BaseMeshSceneNode(boost::intrusive_ptr<glitch::collada::IMesh> const&)>:
  35b19c: e92d4030     	push	{r4, r5, lr}
  35b1a0: e3e04000     	mvn	r4, #0
  35b1a4: e24dd03c     	sub	sp, sp, #60
  35b1a8: e58d4000     	str	r4, [sp]
  35b1ac: e28d402c     	add	r4, sp, #44
  35b1b0: e58d4004     	str	r4, [sp, #0x4]
  35b1b4: e28d4010     	add	r4, sp, #16
  35b1b8: e3a0c000     	mov	r12, #0
  35b1bc: e3a0e5fe     	mov	lr, #1065353216
  35b1c0: e1a05001     	mov	r5, r1
  35b1c4: e58d4008     	str	r4, [sp, #0x8]
  35b1c8: e2811004     	add	r1, r1, #4
  35b1cc: e28d4020     	add	r4, sp, #32
  35b1d0: e3a03000     	mov	r3, #0
  35b1d4: e58d400c     	str	r4, [sp, #0xc]
  35b1d8: e58dc018     	str	r12, [sp, #0x18]
  35b1dc: e1a04000     	mov	r4, r0
  35b1e0: e58de028     	str	lr, [sp, #0x28]
  35b1e4: e58dc02c     	str	r12, [sp, #0x2c]
  35b1e8: e58dc030     	str	r12, [sp, #0x30]
  35b1ec: e58dc034     	str	r12, [sp, #0x34]
  35b1f0: e58dc010     	str	r12, [sp, #0x10]
  35b1f4: e58dc014     	str	r12, [sp, #0x14]
  35b1f8: e58de01c     	str	lr, [sp, #0x1c]
  35b1fc: e58de020     	str	lr, [sp, #0x20]
  35b200: e58de024     	str	lr, [sp, #0x24]
  35b204: eb0bad1b     	bl	0x646678 <glitch::collada::CMeshSceneNode::CMeshSceneNode(boost::intrusive_ptr<glitch::collada::IMesh> const&, glitch::collada::SNode*, int, glitch::core::vector3d<float> const&, glitch::core::quaternion const&, glitch::core::vector3d<float> const&)> @ imm = #0x2eb46c
  35b208: e5953000     	ldr	r3, [r5]
  35b20c: e1a00004     	mov	r0, r4
  35b210: e5843000     	str	r3, [r4]
  35b214: e513301c     	ldr	r3, [r3, #-0x1c]
  35b218: e5952028     	ldr	r2, [r5, #0x28]
  35b21c: e7842003     	str	r2, [r4, r3]
  35b220: e5943000     	ldr	r3, [r4]
  35b224: e595202c     	ldr	r2, [r5, #0x2c]
  35b228: e513300c     	ldr	r3, [r3, #-0xc]
  35b22c: e7842003     	str	r2, [r4, r3]
  35b230: e3a03001     	mov	r3, #1
  35b234: e5c43138     	strb	r3, [r4, #0x138]
  35b238: e28dd03c     	add	sp, sp, #60
  35b23c: e8bd8030     	pop	{r4, r5, pc}
