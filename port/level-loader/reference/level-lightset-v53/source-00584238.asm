
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00584238 <glitch::scene::CLightSceneNode::CLightSceneNode(bool)>:
  584238: e92d40f0     	push	{r4, r5, r6, r7, lr}
  58423c: e24dd034     	sub	sp, sp, #52
  584240: e28de008     	add	lr, sp, #8
  584244: e3a0c000     	mov	r12, #0
  584248: e3a055fe     	mov	r5, #1065353216
  58424c: e1a06001     	mov	r6, r1
  584250: e3e02000     	mvn	r2, #0
  584254: e2811004     	add	r1, r1, #4
  584258: e28d3024     	add	r3, sp, #36
  58425c: e58de000     	str	lr, [sp]
  584260: e28de018     	add	lr, sp, #24
  584264: e1a04000     	mov	r4, r0
  584268: e58dc010     	str	r12, [sp, #0x10]
  58426c: e58de004     	str	lr, [sp, #0x4]
  584270: e58dc024     	str	r12, [sp, #0x24]
  584274: e58dc028     	str	r12, [sp, #0x28]
  584278: e58dc02c     	str	r12, [sp, #0x2c]
  58427c: e58dc008     	str	r12, [sp, #0x8]
  584280: e58dc00c     	str	r12, [sp, #0xc]
  584284: e58d5014     	str	r5, [sp, #0x14]
  584288: e58d5018     	str	r5, [sp, #0x18]
  58428c: e58d501c     	str	r5, [sp, #0x1c]
  584290: e58d5020     	str	r5, [sp, #0x20]
  584294: eb005389     	bl	0x5990c0 <glitch::scene::ISceneNode::ISceneNode(int, glitch::core::vector3d<float> const&, glitch::core::quaternion const&, glitch::core::vector3d<float> const&)> @ imm = #0x14e24
  584298: e5963000     	ldr	r3, [r6]
  58429c: e2841e13     	add	r1, r4, #304
  5842a0: e2840f4d     	add	r0, r4, #308
  5842a4: e5843000     	str	r3, [r4]
  5842a8: e513301c     	ldr	r3, [r3, #-0x1c]
  5842ac: e5962010     	ldr	r2, [r6, #0x10]
  5842b0: e59f7090     	ldr	r7, [pc, #0x90]         @ 0x584348 <glitch::scene::CLightSceneNode::CLightSceneNode(bool)+0x110>
  5842b4: e7842003     	str	r2, [r4, r3]
  5842b8: e5943000     	ldr	r3, [r4]
  5842bc: e5962014     	ldr	r2, [r6, #0x14]
  5842c0: e08f7007     	add	r7, pc, r7
  5842c4: e513300c     	ldr	r3, [r3, #-0xc]
  5842c8: e7842003     	str	r2, [r4, r3]
  5842cc: eb006ef9     	bl	0x59feb8 <glitch::video::CLight::allocate(glitch::video::STransformationSource**)> @ imm = #0x1bbe4
  5842d0: e5941134     	ldr	r1, [r4, #0x134]
  5842d4: e3a024bf     	mov	r2, #-1090519040
  5842d8: e5943130     	ldr	r3, [r4, #0x130]
  5842dc: e1d115b8     	ldrh	r1, [r1, #88]
  5842e0: e2822502     	add	r2, r2, #8388608
  5842e4: e5842144     	str	r2, [r4, #0x144]
  5842e8: e584213c     	str	r2, [r4, #0x13c]
  5842ec: e5842140     	str	r2, [r4, #0x140]
  5842f0: e5845150     	str	r5, [r4, #0x150]
  5842f4: e5841138     	str	r1, [r4, #0x138]
  5842f8: e5845148     	str	r5, [r4, #0x148]
  5842fc: e584514c     	str	r5, [r4, #0x14c]
  584300: e5d31004     	ldrb	r1, [r3, #0x4]
  584304: e2842024     	add	r2, r4, #36
  584308: e3510000     	cmp	r1, #0
  58430c: 1a000005     	bne	0x584328 <glitch::scene::CLightSceneNode::CLightSceneNode(bool)+0xf0> @ imm = #0x14
  584310: e59f1034     	ldr	r1, [pc, #0x34]         @ 0x58434c <glitch::scene::CLightSceneNode::CLightSceneNode(bool)+0x114>
  584314: e5930000     	ldr	r0, [r3]
  584318: e7971001     	ldr	r1, [r7, r1]
  58431c: e591c000     	ldr	r12, [r1]
  584320: e580c000     	str	r12, [r0]
  584324: e5810000     	str	r0, [r1]
  584328: e5832000     	str	r2, [r3]
  58432c: e3a02001     	mov	r2, #1
  584330: e1a00004     	mov	r0, r4
  584334: e5c32004     	strb	r2, [r3, #0x4]
  584338: ebfffe36     	bl	0x583c18 <glitch::scene::CLightSceneNode::doLightRecalc()> @ imm = #-0x728
  58433c: e1a00004     	mov	r0, r4
  584340: e28dd034     	add	sp, sp, #52
  584344: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  584348: d0 07 41 00  	.word	0x004107d0
  58434c: c0 3c 00 00  	.word	0x00003cc0
