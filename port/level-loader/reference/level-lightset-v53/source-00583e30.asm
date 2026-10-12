
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00583e30 <glitch::scene::CLightSceneNode::CLightSceneNode(glitch::scene::CLightSceneNode&)>:
  583e30: e92d40f0     	push	{r4, r5, r6, r7, lr}
  583e34: e59f6144     	ldr	r6, [pc, #0x144]        @ 0x583f80 <glitch::scene::CLightSceneNode::CLightSceneNode(glitch::scene::CLightSceneNode&)+0x150>
  583e38: e59f3144     	ldr	r3, [pc, #0x144]        @ 0x583f84 <glitch::scene::CLightSceneNode::CLightSceneNode(glitch::scene::CLightSceneNode&)+0x154>
  583e3c: e59f2144     	ldr	r2, [pc, #0x144]        @ 0x583f88 <glitch::scene::CLightSceneNode::CLightSceneNode(glitch::scene::CLightSceneNode&)+0x158>
  583e40: e08f6006     	add	r6, pc, r6
  583e44: e7963003     	ldr	r3, [r6, r3]
  583e48: e7962002     	ldr	r2, [r6, r2]
  583e4c: e3a0e001     	mov	lr, #1
  583e50: e593c018     	ldr	r12, [r3, #0x18]
  583e54: e2822008     	add	r2, r2, #8
  583e58: e580e158     	str	lr, [r0, #0x158]
  583e5c: e580c000     	str	r12, [r0]
  583e60: e5802154     	str	r2, [r0, #0x154]
  583e64: e51c200c     	ldr	r2, [r12, #-0xc]
  583e68: e593501c     	ldr	r5, [r3, #0x1c]
  583e6c: e24dd034     	sub	sp, sp, #52
  583e70: e28d7008     	add	r7, sp, #8
  583e74: e7805002     	str	r5, [r0, r2]
  583e78: e3a0c000     	mov	r12, #0
  583e7c: e1a05001     	mov	r5, r1
  583e80: e3a0e5fe     	mov	lr, #1065353216
  583e84: e2831004     	add	r1, r3, #4
  583e88: e3e02000     	mvn	r2, #0
  583e8c: e28d3024     	add	r3, sp, #36
  583e90: e58d7000     	str	r7, [sp]
  583e94: e28d7018     	add	r7, sp, #24
  583e98: e1a04000     	mov	r4, r0
  583e9c: e58dc010     	str	r12, [sp, #0x10]
  583ea0: e58de020     	str	lr, [sp, #0x20]
  583ea4: e58dc024     	str	r12, [sp, #0x24]
  583ea8: e58dc028     	str	r12, [sp, #0x28]
  583eac: e58dc02c     	str	r12, [sp, #0x2c]
  583eb0: e58dc008     	str	r12, [sp, #0x8]
  583eb4: e58dc00c     	str	r12, [sp, #0xc]
  583eb8: e58de014     	str	lr, [sp, #0x14]
  583ebc: e58de018     	str	lr, [sp, #0x18]
  583ec0: e58de01c     	str	lr, [sp, #0x1c]
  583ec4: e58d7004     	str	r7, [sp, #0x4]
  583ec8: eb00547c     	bl	0x5990c0 <glitch::scene::ISceneNode::ISceneNode(int, glitch::core::vector3d<float> const&, glitch::core::quaternion const&, glitch::core::vector3d<float> const&)> @ imm = #0x151f0
  583ecc: e59f30b8     	ldr	r3, [pc, #0xb8]         @ 0x583f8c <glitch::scene::CLightSceneNode::CLightSceneNode(glitch::scene::CLightSceneNode&)+0x15c>
  583ed0: e2842e13     	add	r2, r4, #304
  583ed4: e2840f4d     	add	r0, r4, #308
  583ed8: e7963003     	ldr	r3, [r6, r3]
  583edc: e2831e12     	add	r1, r3, #288
  583ee0: e283301c     	add	r3, r3, #28
  583ee4: e5843000     	str	r3, [r4]
  583ee8: e5841154     	str	r1, [r4, #0x154]
  583eec: e5951134     	ldr	r1, [r5, #0x134]
  583ef0: eb006f82     	bl	0x59fd00 <glitch::video::CLight::clone(glitch::video::STransformationSource**) const> @ imm = #0x1be08
  583ef4: e5941134     	ldr	r1, [r4, #0x134]
  583ef8: e5943130     	ldr	r3, [r4, #0x130]
  583efc: e2842024     	add	r2, r4, #36
  583f00: e1d115b8     	ldrh	r1, [r1, #88]
  583f04: e5841138     	str	r1, [r4, #0x138]
  583f08: e595113c     	ldr	r1, [r5, #0x13c]
  583f0c: e584113c     	str	r1, [r4, #0x13c]
  583f10: e5951140     	ldr	r1, [r5, #0x140]
  583f14: e5841140     	str	r1, [r4, #0x140]
  583f18: e5951144     	ldr	r1, [r5, #0x144]
  583f1c: e5841144     	str	r1, [r4, #0x144]
  583f20: e5951148     	ldr	r1, [r5, #0x148]
  583f24: e5841148     	str	r1, [r4, #0x148]
  583f28: e595114c     	ldr	r1, [r5, #0x14c]
  583f2c: e584114c     	str	r1, [r4, #0x14c]
  583f30: e5951150     	ldr	r1, [r5, #0x150]
  583f34: e5841150     	str	r1, [r4, #0x150]
  583f38: e5d31004     	ldrb	r1, [r3, #0x4]
  583f3c: e3510000     	cmp	r1, #0
  583f40: 1a000005     	bne	0x583f5c <glitch::scene::CLightSceneNode::CLightSceneNode(glitch::scene::CLightSceneNode&)+0x12c> @ imm = #0x14
  583f44: e59f1044     	ldr	r1, [pc, #0x44]         @ 0x583f90 <glitch::scene::CLightSceneNode::CLightSceneNode(glitch::scene::CLightSceneNode&)+0x160>
  583f48: e5930000     	ldr	r0, [r3]
  583f4c: e7961001     	ldr	r1, [r6, r1]
  583f50: e591c000     	ldr	r12, [r1]
  583f54: e580c000     	str	r12, [r0]
  583f58: e5810000     	str	r0, [r1]
  583f5c: e5832000     	str	r2, [r3]
  583f60: e3a02001     	mov	r2, #1
  583f64: e1a00004     	mov	r0, r4
  583f68: e5c32004     	strb	r2, [r3, #0x4]
  583f6c: e1a01005     	mov	r1, r5
  583f70: eb004f95     	bl	0x597dcc <glitch::scene::ISceneNode::cloneMembers(glitch::scene::ISceneNode*)> @ imm = #0x13e54
  583f74: e1a00004     	mov	r0, r4
  583f78: e28dd034     	add	sp, sp, #52
  583f7c: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  583f80: 50 0c 41 00  	.word	0x00410c50
  583f84: e0 10 00 00  	.word	0x000010e0
  583f88: 44 2b 00 00  	.word	0x00002b44
  583f8c: 58 1a 00 00  	.word	0x00001a58
  583f90: c0 3c 00 00  	.word	0x00003cc0
