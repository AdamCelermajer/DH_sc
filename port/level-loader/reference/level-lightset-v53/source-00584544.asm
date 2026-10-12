
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00584544 <glitch::scene::CLightSceneNode::CLightSceneNode(int, glitch::core::vector3d<float> const&, glitch::video::SColorf const&, float)>:
  584544: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  584548: e24dd028     	sub	sp, sp, #40
  58454c: e28de00c     	add	lr, sp, #12
  584550: e3a065fe     	mov	r6, #1065353216
  584554: e3a0c000     	mov	r12, #0
  584558: e1a07001     	mov	r7, r1
  58455c: e58de000     	str	lr, [sp]
  584560: e2811004     	add	r1, r1, #4
  584564: e28de01c     	add	lr, sp, #28
  584568: e1a04000     	mov	r4, r0
  58456c: e58dc014     	str	r12, [sp, #0x14]
  584570: e58de004     	str	lr, [sp, #0x4]
  584574: e58dc00c     	str	r12, [sp, #0xc]
  584578: e58dc010     	str	r12, [sp, #0x10]
  58457c: e58d6018     	str	r6, [sp, #0x18]
  584580: e58d601c     	str	r6, [sp, #0x1c]
  584584: e58d6020     	str	r6, [sp, #0x20]
  584588: e58d6024     	str	r6, [sp, #0x24]
  58458c: e59d5040     	ldr	r5, [sp, #0x40]
  584590: eb0052ca     	bl	0x5990c0 <glitch::scene::ISceneNode::ISceneNode(int, glitch::core::vector3d<float> const&, glitch::core::quaternion const&, glitch::core::vector3d<float> const&)> @ imm = #0x14b28
  584594: e5973000     	ldr	r3, [r7]
  584598: e2841e13     	add	r1, r4, #304
  58459c: e2840f4d     	add	r0, r4, #308
  5845a0: e5843000     	str	r3, [r4]
  5845a4: e513301c     	ldr	r3, [r3, #-0x1c]
  5845a8: e5972010     	ldr	r2, [r7, #0x10]
  5845ac: e59f813c     	ldr	r8, [pc, #0x13c]        @ 0x5846f0 <glitch::scene::CLightSceneNode::CLightSceneNode(int, glitch::core::vector3d<float> const&, glitch::video::SColorf const&, float)+0x1ac>
  5845b0: e7842003     	str	r2, [r4, r3]
  5845b4: e5943000     	ldr	r3, [r4]
  5845b8: e5972014     	ldr	r2, [r7, #0x14]
  5845bc: e08f8008     	add	r8, pc, r8
  5845c0: e513300c     	ldr	r3, [r3, #-0xc]
  5845c4: e7842003     	str	r2, [r4, r3]
  5845c8: eb006e3a     	bl	0x59feb8 <glitch::video::CLight::allocate(glitch::video::STransformationSource**)> @ imm = #0x1b8e8
  5845cc: e5941134     	ldr	r1, [r4, #0x134]
  5845d0: e3a024bf     	mov	r2, #-1090519040
  5845d4: e5943130     	ldr	r3, [r4, #0x130]
  5845d8: e1d115b8     	ldrh	r1, [r1, #88]
  5845dc: e2822502     	add	r2, r2, #8388608
  5845e0: e5842144     	str	r2, [r4, #0x144]
  5845e4: e584213c     	str	r2, [r4, #0x13c]
  5845e8: e5842140     	str	r2, [r4, #0x140]
  5845ec: e5846150     	str	r6, [r4, #0x150]
  5845f0: e5841138     	str	r1, [r4, #0x138]
  5845f4: e5846148     	str	r6, [r4, #0x148]
  5845f8: e584614c     	str	r6, [r4, #0x14c]
  5845fc: e5d31004     	ldrb	r1, [r3, #0x4]
  584600: e2842024     	add	r2, r4, #36
  584604: e3510000     	cmp	r1, #0
  584608: 1a000005     	bne	0x584624 <glitch::scene::CLightSceneNode::CLightSceneNode(int, glitch::core::vector3d<float> const&, glitch::video::SColorf const&, float)+0xe0> @ imm = #0x14
  58460c: e59f10e0     	ldr	r1, [pc, #0xe0]         @ 0x5846f4 <glitch::scene::CLightSceneNode::CLightSceneNode(int, glitch::core::vector3d<float> const&, glitch::video::SColorf const&, float)+0x1b0>
  584610: e5930000     	ldr	r0, [r3]
  584614: e7981001     	ldr	r1, [r8, r1]
  584618: e591c000     	ldr	r12, [r1]
  58461c: e580c000     	str	r12, [r0]
  584620: e5810000     	str	r0, [r1]
  584624: e5832000     	str	r2, [r3]
  584628: e3a02001     	mov	r2, #1
  58462c: e5c32004     	strb	r2, [r3, #0x4]
  584630: e5943134     	ldr	r3, [r4, #0x134]
  584634: e59d2044     	ldr	r2, [sp, #0x44]
  584638: e5832040     	str	r2, [r3, #0x40]
  58463c: e594c134     	ldr	r12, [r4, #0x134]
  584640: e895000f     	ldm	r5, {r0, r1, r2, r3}
  584644: e28cc014     	add	r12, r12, #20
  584648: e88c000f     	stm	r12, {r0, r1, r2, r3}
  58464c: e3031333     	movw	r1, #0x3333
  584650: e5950000     	ldr	r0, [r5]
  584654: e3431f33     	movt	r1, #0x3f33
  584658: ebf629c3     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x2758f4
  58465c: e309199a     	movw	r1, #0x999a
  584660: e3431e99     	movt	r1, #0x3e99
  584664: ebf6294e     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x275ac8
  584668: e3031333     	movw	r1, #0x3333
  58466c: e1a08000     	mov	r8, r0
  584670: e3431f33     	movt	r1, #0x3f33
  584674: e5950004     	ldr	r0, [r5, #0x4]
  584678: ebf629bb     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x275914
  58467c: e309199a     	movw	r1, #0x999a
  584680: e3431e99     	movt	r1, #0x3e99
  584684: ebf62946     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x275ae8
  584688: e3031333     	movw	r1, #0x3333
  58468c: e1a06000     	mov	r6, r0
  584690: e3431f33     	movt	r1, #0x3f33
  584694: e5950008     	ldr	r0, [r5, #0x8]
  584698: ebf629b3     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x275934
  58469c: e309199a     	movw	r1, #0x999a
  5846a0: e3431e99     	movt	r1, #0x3e99
  5846a4: ebf6293e     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x275b08
  5846a8: e3031333     	movw	r1, #0x3333
  5846ac: e1a07000     	mov	r7, r0
  5846b0: e3431f33     	movt	r1, #0x3f33
  5846b4: e595000c     	ldr	r0, [r5, #0xc]
  5846b8: ebf629ab     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x275954
  5846bc: e309199a     	movw	r1, #0x999a
  5846c0: e3431e99     	movt	r1, #0x3e99
  5846c4: ebf62936     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x275b28
  5846c8: e5943134     	ldr	r3, [r4, #0x134]
  5846cc: e5830030     	str	r0, [r3, #0x30]
  5846d0: e5838024     	str	r8, [r3, #0x24]
  5846d4: e1a00004     	mov	r0, r4
  5846d8: e583702c     	str	r7, [r3, #0x2c]
  5846dc: e5836028     	str	r6, [r3, #0x28]
  5846e0: ebfffd4c     	bl	0x583c18 <glitch::scene::CLightSceneNode::doLightRecalc()> @ imm = #-0xad0
  5846e4: e1a00004     	mov	r0, r4
  5846e8: e28dd028     	add	sp, sp, #40
  5846ec: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  5846f0: d4 04 41 00  	.word	0x004104d4
  5846f4: c0 3c 00 00  	.word	0x00003cc0
