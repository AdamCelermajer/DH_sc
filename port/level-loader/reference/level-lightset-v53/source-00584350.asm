
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00584350 <glitch::scene::CLightSceneNode::CLightSceneNode(int, glitch::core::vector3d<float> const&, glitch::video::SColorf const&, float)>:
  584350: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  584354: e59f71d4     	ldr	r7, [pc, #0x1d4]        @ 0x584530 <glitch::scene::CLightSceneNode::CLightSceneNode(int, glitch::core::vector3d<float> const&, glitch::video::SColorf const&, float)+0x1e0>
  584358: e59fe1d4     	ldr	lr, [pc, #0x1d4]        @ 0x584534 <glitch::scene::CLightSceneNode::CLightSceneNode(int, glitch::core::vector3d<float> const&, glitch::video::SColorf const&, float)+0x1e4>
  58435c: e59fc1d4     	ldr	r12, [pc, #0x1d4]       @ 0x584538 <glitch::scene::CLightSceneNode::CLightSceneNode(int, glitch::core::vector3d<float> const&, glitch::video::SColorf const&, float)+0x1e8>
  584360: e08f7007     	add	r7, pc, r7
  584364: e797e00e     	ldr	lr, [r7, lr]
  584368: e797c00c     	ldr	r12, [r7, r12]
  58436c: e3a06001     	mov	r6, #1
  584370: e59e5018     	ldr	r5, [lr, #0x18]
  584374: e28cc008     	add	r12, r12, #8
  584378: e5806158     	str	r6, [r0, #0x158]
  58437c: e580c154     	str	r12, [r0, #0x154]
  584380: e5805000     	str	r5, [r0]
  584384: e515a00c     	ldr	r10, [r5, #-0xc]
  584388: e59e901c     	ldr	r9, [lr, #0x1c]
  58438c: e24dd028     	sub	sp, sp, #40
  584390: e1a05001     	mov	r5, r1
  584394: e1a08002     	mov	r8, r2
  584398: e28e1004     	add	r1, lr, #4
  58439c: e28de00c     	add	lr, sp, #12
  5843a0: e780900a     	str	r9, [r0, r10]
  5843a4: e3a065fe     	mov	r6, #1065353216
  5843a8: e3a0c000     	mov	r12, #0
  5843ac: e1a02005     	mov	r2, r5
  5843b0: e58de000     	str	lr, [sp]
  5843b4: e1a05003     	mov	r5, r3
  5843b8: e28de01c     	add	lr, sp, #28
  5843bc: e1a03008     	mov	r3, r8
  5843c0: e1a04000     	mov	r4, r0
  5843c4: e58dc014     	str	r12, [sp, #0x14]
  5843c8: e58de004     	str	lr, [sp, #0x4]
  5843cc: e58dc00c     	str	r12, [sp, #0xc]
  5843d0: e58dc010     	str	r12, [sp, #0x10]
  5843d4: e58d6018     	str	r6, [sp, #0x18]
  5843d8: e58d601c     	str	r6, [sp, #0x1c]
  5843dc: e58d6020     	str	r6, [sp, #0x20]
  5843e0: e58d6024     	str	r6, [sp, #0x24]
  5843e4: eb005335     	bl	0x5990c0 <glitch::scene::ISceneNode::ISceneNode(int, glitch::core::vector3d<float> const&, glitch::core::quaternion const&, glitch::core::vector3d<float> const&)> @ imm = #0x14cd4
  5843e8: e59f314c     	ldr	r3, [pc, #0x14c]        @ 0x58453c <glitch::scene::CLightSceneNode::CLightSceneNode(int, glitch::core::vector3d<float> const&, glitch::video::SColorf const&, float)+0x1ec>
  5843ec: e2841e13     	add	r1, r4, #304
  5843f0: e2840f4d     	add	r0, r4, #308
  5843f4: e7973003     	ldr	r3, [r7, r3]
  5843f8: e2832e12     	add	r2, r3, #288
  5843fc: e283301c     	add	r3, r3, #28
  584400: e5843000     	str	r3, [r4]
  584404: e5842154     	str	r2, [r4, #0x154]
  584408: eb006eaa     	bl	0x59feb8 <glitch::video::CLight::allocate(glitch::video::STransformationSource**)> @ imm = #0x1baa8
  58440c: e5941134     	ldr	r1, [r4, #0x134]
  584410: e3a024bf     	mov	r2, #-1090519040
  584414: e5943130     	ldr	r3, [r4, #0x130]
  584418: e1d115b8     	ldrh	r1, [r1, #88]
  58441c: e2822502     	add	r2, r2, #8388608
  584420: e5842144     	str	r2, [r4, #0x144]
  584424: e584213c     	str	r2, [r4, #0x13c]
  584428: e5842140     	str	r2, [r4, #0x140]
  58442c: e5846150     	str	r6, [r4, #0x150]
  584430: e5841138     	str	r1, [r4, #0x138]
  584434: e5846148     	str	r6, [r4, #0x148]
  584438: e584614c     	str	r6, [r4, #0x14c]
  58443c: e5d31004     	ldrb	r1, [r3, #0x4]
  584440: e2842024     	add	r2, r4, #36
  584444: e3510000     	cmp	r1, #0
  584448: 1a000005     	bne	0x584464 <glitch::scene::CLightSceneNode::CLightSceneNode(int, glitch::core::vector3d<float> const&, glitch::video::SColorf const&, float)+0x114> @ imm = #0x14
  58444c: e59f10ec     	ldr	r1, [pc, #0xec]         @ 0x584540 <glitch::scene::CLightSceneNode::CLightSceneNode(int, glitch::core::vector3d<float> const&, glitch::video::SColorf const&, float)+0x1f0>
  584450: e5930000     	ldr	r0, [r3]
  584454: e7971001     	ldr	r1, [r7, r1]
  584458: e591c000     	ldr	r12, [r1]
  58445c: e580c000     	str	r12, [r0]
  584460: e5810000     	str	r0, [r1]
  584464: e5832000     	str	r2, [r3]
  584468: e3a02001     	mov	r2, #1
  58446c: e5c32004     	strb	r2, [r3, #0x4]
  584470: e5943134     	ldr	r3, [r4, #0x134]
  584474: e59d2048     	ldr	r2, [sp, #0x48]
  584478: e5832040     	str	r2, [r3, #0x40]
  58447c: e594c134     	ldr	r12, [r4, #0x134]
  584480: e895000f     	ldm	r5, {r0, r1, r2, r3}
  584484: e28cc014     	add	r12, r12, #20
  584488: e88c000f     	stm	r12, {r0, r1, r2, r3}
  58448c: e3031333     	movw	r1, #0x3333
  584490: e5950000     	ldr	r0, [r5]
  584494: e3431f33     	movt	r1, #0x3f33
  584498: ebf62a33     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x275734
  58449c: e309199a     	movw	r1, #0x999a
  5844a0: e3431e99     	movt	r1, #0x3e99
  5844a4: ebf629be     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x275908
  5844a8: e3031333     	movw	r1, #0x3333
  5844ac: e1a08000     	mov	r8, r0
  5844b0: e3431f33     	movt	r1, #0x3f33
  5844b4: e5950004     	ldr	r0, [r5, #0x4]
  5844b8: ebf62a2b     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x275754
  5844bc: e309199a     	movw	r1, #0x999a
  5844c0: e3431e99     	movt	r1, #0x3e99
  5844c4: ebf629b6     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x275928
  5844c8: e3031333     	movw	r1, #0x3333
  5844cc: e1a06000     	mov	r6, r0
  5844d0: e3431f33     	movt	r1, #0x3f33
  5844d4: e5950008     	ldr	r0, [r5, #0x8]
  5844d8: ebf62a23     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x275774
  5844dc: e309199a     	movw	r1, #0x999a
  5844e0: e3431e99     	movt	r1, #0x3e99
  5844e4: ebf629ae     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x275948
  5844e8: e3031333     	movw	r1, #0x3333
  5844ec: e1a07000     	mov	r7, r0
  5844f0: e3431f33     	movt	r1, #0x3f33
  5844f4: e595000c     	ldr	r0, [r5, #0xc]
  5844f8: ebf62a1b     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x275794
  5844fc: e309199a     	movw	r1, #0x999a
  584500: e3431e99     	movt	r1, #0x3e99
  584504: ebf629a6     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x275968
  584508: e5943134     	ldr	r3, [r4, #0x134]
  58450c: e5830030     	str	r0, [r3, #0x30]
  584510: e5838024     	str	r8, [r3, #0x24]
  584514: e1a00004     	mov	r0, r4
  584518: e583702c     	str	r7, [r3, #0x2c]
  58451c: e5836028     	str	r6, [r3, #0x28]
  584520: ebfffdbc     	bl	0x583c18 <glitch::scene::CLightSceneNode::doLightRecalc()> @ imm = #-0x910
  584524: e1a00004     	mov	r0, r4
  584528: e28dd028     	add	sp, sp, #40
  58452c: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  584530: 30 07 41 00  	.word	0x00410730
  584534: e0 10 00 00  	.word	0x000010e0
  584538: 44 2b 00 00  	.word	0x00002b44
  58453c: 58 1a 00 00  	.word	0x00001a58
  584540: c0 3c 00 00  	.word	0x00003cc0
