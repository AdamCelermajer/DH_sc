
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

005840f0 <glitch::scene::CLightSceneNode::CLightSceneNode(bool)>:
  5840f0: e92d4070     	push	{r4, r5, r6, lr}
  5840f4: e59f6128     	ldr	r6, [pc, #0x128]        @ 0x584224 <glitch::scene::CLightSceneNode::CLightSceneNode(bool)+0x134>
  5840f8: e59f2128     	ldr	r2, [pc, #0x128]        @ 0x584228 <glitch::scene::CLightSceneNode::CLightSceneNode(bool)+0x138>
  5840fc: e59f3128     	ldr	r3, [pc, #0x128]        @ 0x58422c <glitch::scene::CLightSceneNode::CLightSceneNode(bool)+0x13c>
  584100: e08f6006     	add	r6, pc, r6
  584104: e7961002     	ldr	r1, [r6, r2]
  584108: e7963003     	ldr	r3, [r6, r3]
  58410c: e3a0c001     	mov	r12, #1
  584110: e5912018     	ldr	r2, [r1, #0x18]
  584114: e2833008     	add	r3, r3, #8
  584118: e580c158     	str	r12, [r0, #0x158]
  58411c: e5802000     	str	r2, [r0]
  584120: e5803154     	str	r3, [r0, #0x154]
  584124: e512300c     	ldr	r3, [r2, #-0xc]
  584128: e591201c     	ldr	r2, [r1, #0x1c]
  58412c: e24dd030     	sub	sp, sp, #48
  584130: e28de008     	add	lr, sp, #8
  584134: e7802003     	str	r2, [r0, r3]
  584138: e3a0c000     	mov	r12, #0
  58413c: e3a055fe     	mov	r5, #1065353216
  584140: e2811004     	add	r1, r1, #4
  584144: e3e02000     	mvn	r2, #0
  584148: e28d3024     	add	r3, sp, #36
  58414c: e58de000     	str	lr, [sp]
  584150: e28de018     	add	lr, sp, #24
  584154: e1a04000     	mov	r4, r0
  584158: e58dc010     	str	r12, [sp, #0x10]
  58415c: e58de004     	str	lr, [sp, #0x4]
  584160: e58dc024     	str	r12, [sp, #0x24]
  584164: e58dc028     	str	r12, [sp, #0x28]
  584168: e58dc02c     	str	r12, [sp, #0x2c]
  58416c: e58dc008     	str	r12, [sp, #0x8]
  584170: e58dc00c     	str	r12, [sp, #0xc]
  584174: e58d5014     	str	r5, [sp, #0x14]
  584178: e58d5018     	str	r5, [sp, #0x18]
  58417c: e58d501c     	str	r5, [sp, #0x1c]
  584180: e58d5020     	str	r5, [sp, #0x20]
  584184: eb0053cd     	bl	0x5990c0 <glitch::scene::ISceneNode::ISceneNode(int, glitch::core::vector3d<float> const&, glitch::core::quaternion const&, glitch::core::vector3d<float> const&)> @ imm = #0x14f34
  584188: e59f30a0     	ldr	r3, [pc, #0xa0]         @ 0x584230 <glitch::scene::CLightSceneNode::CLightSceneNode(bool)+0x140>
  58418c: e2841e13     	add	r1, r4, #304
  584190: e2840f4d     	add	r0, r4, #308
  584194: e7963003     	ldr	r3, [r6, r3]
  584198: e2832e12     	add	r2, r3, #288
  58419c: e283301c     	add	r3, r3, #28
  5841a0: e5843000     	str	r3, [r4]
  5841a4: e5842154     	str	r2, [r4, #0x154]
  5841a8: eb006f42     	bl	0x59feb8 <glitch::video::CLight::allocate(glitch::video::STransformationSource**)> @ imm = #0x1bd08
  5841ac: e5941134     	ldr	r1, [r4, #0x134]
  5841b0: e3a024bf     	mov	r2, #-1090519040
  5841b4: e5943130     	ldr	r3, [r4, #0x130]
  5841b8: e1d115b8     	ldrh	r1, [r1, #88]
  5841bc: e2822502     	add	r2, r2, #8388608
  5841c0: e5842144     	str	r2, [r4, #0x144]
  5841c4: e584213c     	str	r2, [r4, #0x13c]
  5841c8: e5842140     	str	r2, [r4, #0x140]
  5841cc: e5845150     	str	r5, [r4, #0x150]
  5841d0: e5841138     	str	r1, [r4, #0x138]
  5841d4: e5845148     	str	r5, [r4, #0x148]
  5841d8: e584514c     	str	r5, [r4, #0x14c]
  5841dc: e5d31004     	ldrb	r1, [r3, #0x4]
  5841e0: e2842024     	add	r2, r4, #36
  5841e4: e3510000     	cmp	r1, #0
  5841e8: 1a000005     	bne	0x584204 <glitch::scene::CLightSceneNode::CLightSceneNode(bool)+0x114> @ imm = #0x14
  5841ec: e59f1040     	ldr	r1, [pc, #0x40]         @ 0x584234 <glitch::scene::CLightSceneNode::CLightSceneNode(bool)+0x144>
  5841f0: e5930000     	ldr	r0, [r3]
  5841f4: e7961001     	ldr	r1, [r6, r1]
  5841f8: e591c000     	ldr	r12, [r1]
  5841fc: e580c000     	str	r12, [r0]
  584200: e5810000     	str	r0, [r1]
  584204: e5832000     	str	r2, [r3]
  584208: e3a02001     	mov	r2, #1
  58420c: e1a00004     	mov	r0, r4
  584210: e5c32004     	strb	r2, [r3, #0x4]
  584214: ebfffe7f     	bl	0x583c18 <glitch::scene::CLightSceneNode::doLightRecalc()> @ imm = #-0x604
  584218: e1a00004     	mov	r0, r4
  58421c: e28dd030     	add	sp, sp, #48
  584220: e8bd8070     	pop	{r4, r5, r6, pc}
  584224: 90 09 41 00  	.word	0x00410990
  584228: e0 10 00 00  	.word	0x000010e0
  58422c: 44 2b 00 00  	.word	0x00002b44
  584230: 58 1a 00 00  	.word	0x00001a58
  584234: c0 3c 00 00  	.word	0x00003cc0
