
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00583fbc <glitch::scene::CLightSceneNode::CLightSceneNode(glitch::scene::CLightSceneNode&)>:
  583fbc: e92d40f0     	push	{r4, r5, r6, r7, lr}
  583fc0: e24dd034     	sub	sp, sp, #52
  583fc4: e28d4008     	add	r4, sp, #8
  583fc8: e3a0c000     	mov	r12, #0
  583fcc: e1a05002     	mov	r5, r2
  583fd0: e3a0e5fe     	mov	lr, #1065353216
  583fd4: e1a06001     	mov	r6, r1
  583fd8: e3e02000     	mvn	r2, #0
  583fdc: e2811004     	add	r1, r1, #4
  583fe0: e28d3024     	add	r3, sp, #36
  583fe4: e58d4000     	str	r4, [sp]
  583fe8: e28d4018     	add	r4, sp, #24
  583fec: e58dc010     	str	r12, [sp, #0x10]
  583ff0: e58de020     	str	lr, [sp, #0x20]
  583ff4: e58d4004     	str	r4, [sp, #0x4]
  583ff8: e58dc024     	str	r12, [sp, #0x24]
  583ffc: e1a04000     	mov	r4, r0
  584000: e58dc028     	str	r12, [sp, #0x28]
  584004: e58dc02c     	str	r12, [sp, #0x2c]
  584008: e58dc008     	str	r12, [sp, #0x8]
  58400c: e58dc00c     	str	r12, [sp, #0xc]
  584010: e58de014     	str	lr, [sp, #0x14]
  584014: e58de018     	str	lr, [sp, #0x18]
  584018: e58de01c     	str	lr, [sp, #0x1c]
  58401c: eb005427     	bl	0x5990c0 <glitch::scene::ISceneNode::ISceneNode(int, glitch::core::vector3d<float> const&, glitch::core::quaternion const&, glitch::core::vector3d<float> const&)> @ imm = #0x1509c
  584020: e5963000     	ldr	r3, [r6]
  584024: e2842e13     	add	r2, r4, #304
  584028: e2840f4d     	add	r0, r4, #308
  58402c: e5843000     	str	r3, [r4]
  584030: e513301c     	ldr	r3, [r3, #-0x1c]
  584034: e5961010     	ldr	r1, [r6, #0x10]
  584038: e59f70a8     	ldr	r7, [pc, #0xa8]         @ 0x5840e8 <glitch::scene::CLightSceneNode::CLightSceneNode(glitch::scene::CLightSceneNode&)+0x12c>
  58403c: e7841003     	str	r1, [r4, r3]
  584040: e5943000     	ldr	r3, [r4]
  584044: e5961014     	ldr	r1, [r6, #0x14]
  584048: e08f7007     	add	r7, pc, r7
  58404c: e513300c     	ldr	r3, [r3, #-0xc]
  584050: e7841003     	str	r1, [r4, r3]
  584054: e5951134     	ldr	r1, [r5, #0x134]
  584058: eb006f28     	bl	0x59fd00 <glitch::video::CLight::clone(glitch::video::STransformationSource**) const> @ imm = #0x1bca0
  58405c: e5941134     	ldr	r1, [r4, #0x134]
  584060: e5943130     	ldr	r3, [r4, #0x130]
  584064: e2842024     	add	r2, r4, #36
  584068: e1d115b8     	ldrh	r1, [r1, #88]
  58406c: e5841138     	str	r1, [r4, #0x138]
  584070: e595113c     	ldr	r1, [r5, #0x13c]
  584074: e584113c     	str	r1, [r4, #0x13c]
  584078: e5951140     	ldr	r1, [r5, #0x140]
  58407c: e5841140     	str	r1, [r4, #0x140]
  584080: e5951144     	ldr	r1, [r5, #0x144]
  584084: e5841144     	str	r1, [r4, #0x144]
  584088: e5951148     	ldr	r1, [r5, #0x148]
  58408c: e5841148     	str	r1, [r4, #0x148]
  584090: e595114c     	ldr	r1, [r5, #0x14c]
  584094: e584114c     	str	r1, [r4, #0x14c]
  584098: e5951150     	ldr	r1, [r5, #0x150]
  58409c: e5841150     	str	r1, [r4, #0x150]
  5840a0: e5d31004     	ldrb	r1, [r3, #0x4]
  5840a4: e3510000     	cmp	r1, #0
  5840a8: 1a000005     	bne	0x5840c4 <glitch::scene::CLightSceneNode::CLightSceneNode(glitch::scene::CLightSceneNode&)+0x108> @ imm = #0x14
  5840ac: e59f1038     	ldr	r1, [pc, #0x38]         @ 0x5840ec <glitch::scene::CLightSceneNode::CLightSceneNode(glitch::scene::CLightSceneNode&)+0x130>
  5840b0: e5930000     	ldr	r0, [r3]
  5840b4: e7971001     	ldr	r1, [r7, r1]
  5840b8: e591c000     	ldr	r12, [r1]
  5840bc: e580c000     	str	r12, [r0]
  5840c0: e5810000     	str	r0, [r1]
  5840c4: e5832000     	str	r2, [r3]
  5840c8: e3a02001     	mov	r2, #1
  5840cc: e1a00004     	mov	r0, r4
  5840d0: e5c32004     	strb	r2, [r3, #0x4]
  5840d4: e1a01005     	mov	r1, r5
  5840d8: eb004f3b     	bl	0x597dcc <glitch::scene::ISceneNode::cloneMembers(glitch::scene::ISceneNode*)> @ imm = #0x13cec
  5840dc: e1a00004     	mov	r0, r4
  5840e0: e28dd034     	add	sp, sp, #52
  5840e4: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  5840e8: 48 0a 41 00  	.word	0x00410a48
  5840ec: c0 3c 00 00  	.word	0x00003cc0
