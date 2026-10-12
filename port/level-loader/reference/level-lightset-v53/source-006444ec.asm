
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

006444ec <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)>:
  6444ec: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  6444f0: e59f62a0     	ldr	r6, [pc, #0x2a0]        @ 0x644798 <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x2ac>
  6444f4: e59f32a0     	ldr	r3, [pc, #0x2a0]        @ 0x64479c <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x2b0>
  6444f8: e59fc2a0     	ldr	r12, [pc, #0x2a0]       @ 0x6447a0 <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x2b4>
  6444fc: e08f6006     	add	r6, pc, r6
  644500: e7963003     	ldr	r3, [r6, r3]
  644504: e796c00c     	ldr	r12, [r6, r12]
  644508: e3a0e001     	mov	lr, #1
  64450c: e5935024     	ldr	r5, [r3, #0x24]
  644510: e28cc008     	add	r12, r12, #8
  644514: e580c164     	str	r12, [r0, #0x164]
  644518: e5805000     	str	r5, [r0]
  64451c: e580e168     	str	lr, [r0, #0x168]
  644520: e515c00c     	ldr	r12, [r5, #-0xc]
  644524: e5938028     	ldr	r8, [r3, #0x28]
  644528: e1a07001     	mov	r7, r1
  64452c: e1a05002     	mov	r5, r2
  644530: e2831004     	add	r1, r3, #4
  644534: e1a0200e     	mov	r2, lr
  644538: e780800c     	str	r8, [r0, r12]
  64453c: e1a04000     	mov	r4, r0
  644540: ebfcff3c     	bl	0x584238 <glitch::scene::CLightSceneNode::CLightSceneNode(bool)> @ imm = #-0xc0310
  644544: e5973000     	ldr	r3, [r7]
  644548: e5843158     	str	r3, [r4, #0x158]
  64454c: e5972004     	ldr	r2, [r7, #0x4]
  644550: e3530000     	cmp	r3, #0
  644554: e584215c     	str	r2, [r4, #0x15c]
  644558: 0a000003     	beq	0x64456c <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x80> @ imm = #0xc
  64455c: e5932004     	ldr	r2, [r3, #0x4]
  644560: e3520000     	cmp	r2, #0
  644564: 12822001     	addne	r2, r2, #1
  644568: 15832004     	strne	r2, [r3, #0x4]
  64456c: e59f2230     	ldr	r2, [pc, #0x230]        @ 0x6447a4 <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x2b8>
  644570: e59f3230     	ldr	r3, [pc, #0x230]        @ 0x6447a8 <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x2bc>
  644574: e5845160     	str	r5, [r4, #0x160]
  644578: e7962002     	ldr	r2, [r6, r2]
  64457c: e7963003     	ldr	r3, [r6, r3]
  644580: e3a01443     	mov	r1, #1124073472
  644584: e2822004     	add	r2, r2, #4
  644588: e2830e12     	add	r0, r3, #288
  64458c: e283301c     	add	r3, r3, #28
  644590: e5843000     	str	r3, [r4]
  644594: e5842154     	str	r2, [r4, #0x154]
  644598: e5840164     	str	r0, [r4, #0x164]
  64459c: e5950010     	ldr	r0, [r5, #0x10]
  6445a0: e281187f     	add	r1, r1, #8323072
  6445a4: ebf329ba     	bl	0x30ec94 <.plt+0xf20>   @ imm = #-0x335918
  6445a8: e1a06000     	mov	r6, r0
  6445ac: e5d5000c     	ldrb	r0, [r5, #0xc]
  6445b0: ebf328eb     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x335c54
  6445b4: e1a01006     	mov	r1, r6
  6445b8: ebf329eb     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x335854
  6445bc: e1a0a000     	mov	r10, r0
  6445c0: e5d5000d     	ldrb	r0, [r5, #0xd]
  6445c4: ebf328e6     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x335c68
  6445c8: e1a01006     	mov	r1, r6
  6445cc: ebf329e6     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x335868
  6445d0: e1a07000     	mov	r7, r0
  6445d4: e5d5000e     	ldrb	r0, [r5, #0xe]
  6445d8: ebf328e1     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x335c7c
  6445dc: e1a01006     	mov	r1, r6
  6445e0: ebf329e1     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x33587c
  6445e4: e1a08000     	mov	r8, r0
  6445e8: e5d5000f     	ldrb	r0, [r5, #0xf]
  6445ec: ebf328dc     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x335c90
  6445f0: e1a01006     	mov	r1, r6
  6445f4: ebf329dc     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x335890
  6445f8: e5943134     	ldr	r3, [r4, #0x134]
  6445fc: e583a024     	str	r10, [r3, #0x24]
  644600: e5830030     	str	r0, [r3, #0x30]
  644604: e583802c     	str	r8, [r3, #0x2c]
  644608: e5837028     	str	r7, [r3, #0x28]
  64460c: e5953008     	ldr	r3, [r5, #0x8]
  644610: e3530003     	cmp	r3, #3
  644614: 908ff103     	addls	pc, pc, r3, lsl #2
  644618: ea000016     	b	0x644678 <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x18c> @ imm = #0x58
  64461c: ea000002     	b	0x64462c <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x140> @ imm = #0x8
  644620: ea000044     	b	0x644738 <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x24c> @ imm = #0x110
  644624: ea000023     	b	0x6446b8 <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x1cc> @ imm = #0x8c
  644628: ea000016     	b	0x644688 <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x19c> @ imm = #0x58
  64462c: e5942134     	ldr	r2, [r4, #0x134]
  644630: e3a01003     	mov	r1, #3
  644634: e3a03000     	mov	r3, #0
  644638: e1c215b8     	strh	r1, [r2, #88]
  64463c: e5942134     	ldr	r2, [r4, #0x134]
  644640: e582a004     	str	r10, [r2, #0x4]
  644644: e5820010     	str	r0, [r2, #0x10]
  644648: e582800c     	str	r8, [r2, #0xc]
  64464c: e5827008     	str	r7, [r2, #0x8]
  644650: e5942134     	ldr	r2, [r4, #0x134]
  644654: e5823014     	str	r3, [r2, #0x14]
  644658: e5823020     	str	r3, [r2, #0x20]
  64465c: e582301c     	str	r3, [r2, #0x1c]
  644660: e5823018     	str	r3, [r2, #0x18]
  644664: e5942134     	ldr	r2, [r4, #0x134]
  644668: e5823024     	str	r3, [r2, #0x24]
  64466c: e5823030     	str	r3, [r2, #0x30]
  644670: e582302c     	str	r3, [r2, #0x2c]
  644674: e5823028     	str	r3, [r2, #0x28]
  644678: e1a00004     	mov	r0, r4
  64467c: ebfcfd65     	bl	0x583c18 <glitch::scene::CLightSceneNode::doLightRecalc()> @ imm = #-0xc0a6c
  644680: e1a00004     	mov	r0, r4
  644684: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  644688: e5943134     	ldr	r3, [r4, #0x134]
  64468c: e3a01002     	mov	r1, #2
  644690: e1c315b8     	strh	r1, [r3, #88]
  644694: e5943134     	ldr	r3, [r4, #0x134]
  644698: e5830020     	str	r0, [r3, #0x20]
  64469c: e583a014     	str	r10, [r3, #0x14]
  6446a0: e1a00004     	mov	r0, r4
  6446a4: e583801c     	str	r8, [r3, #0x1c]
  6446a8: e5837018     	str	r7, [r3, #0x18]
  6446ac: ebfcfd59     	bl	0x583c18 <glitch::scene::CLightSceneNode::doLightRecalc()> @ imm = #-0xc0a9c
  6446b0: e1a00004     	mov	r0, r4
  6446b4: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  6446b8: e5943134     	ldr	r3, [r4, #0x134]
  6446bc: e3a02001     	mov	r2, #1
  6446c0: e1c325b8     	strh	r2, [r3, #88]
  6446c4: e5943134     	ldr	r3, [r4, #0x134]
  6446c8: e583a014     	str	r10, [r3, #0x14]
  6446cc: e5830020     	str	r0, [r3, #0x20]
  6446d0: e583801c     	str	r8, [r3, #0x1c]
  6446d4: e5837018     	str	r7, [r3, #0x18]
  6446d8: e5952014     	ldr	r2, [r5, #0x14]
  6446dc: e5943134     	ldr	r3, [r4, #0x134]
  6446e0: e1a00004     	mov	r0, r4
  6446e4: e5922000     	ldr	r2, [r2]
  6446e8: e5832034     	str	r2, [r3, #0x34]
  6446ec: e5952014     	ldr	r2, [r5, #0x14]
  6446f0: e5943134     	ldr	r3, [r4, #0x134]
  6446f4: e5922004     	ldr	r2, [r2, #0x4]
  6446f8: e5832038     	str	r2, [r3, #0x38]
  6446fc: e5952014     	ldr	r2, [r5, #0x14]
  644700: e5943134     	ldr	r3, [r4, #0x134]
  644704: e5922008     	ldr	r2, [r2, #0x8]
  644708: e583203c     	str	r2, [r3, #0x3c]
  64470c: e5952014     	ldr	r2, [r5, #0x14]
  644710: e5943134     	ldr	r3, [r4, #0x134]
  644714: e592200c     	ldr	r2, [r2, #0xc]
  644718: e5832048     	str	r2, [r3, #0x48]
  64471c: e5952014     	ldr	r2, [r5, #0x14]
  644720: e5943134     	ldr	r3, [r4, #0x134]
  644724: e5922010     	ldr	r2, [r2, #0x10]
  644728: e583204c     	str	r2, [r3, #0x4c]
  64472c: ebfcfd39     	bl	0x583c18 <glitch::scene::CLightSceneNode::doLightRecalc()> @ imm = #-0xc0b1c
  644730: e1a00004     	mov	r0, r4
  644734: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  644738: e5943134     	ldr	r3, [r4, #0x134]
  64473c: e3a01000     	mov	r1, #0
  644740: e1c315b8     	strh	r1, [r3, #88]
  644744: e5943134     	ldr	r3, [r4, #0x134]
  644748: e583a014     	str	r10, [r3, #0x14]
  64474c: e5830020     	str	r0, [r3, #0x20]
  644750: e583801c     	str	r8, [r3, #0x1c]
  644754: e5837018     	str	r7, [r3, #0x18]
  644758: e5952014     	ldr	r2, [r5, #0x14]
  64475c: e5943134     	ldr	r3, [r4, #0x134]
  644760: e1a00004     	mov	r0, r4
  644764: e5922000     	ldr	r2, [r2]
  644768: e5832034     	str	r2, [r3, #0x34]
  64476c: e5952014     	ldr	r2, [r5, #0x14]
  644770: e5943134     	ldr	r3, [r4, #0x134]
  644774: e5922004     	ldr	r2, [r2, #0x4]
  644778: e5832038     	str	r2, [r3, #0x38]
  64477c: e5952014     	ldr	r2, [r5, #0x14]
  644780: e5943134     	ldr	r3, [r4, #0x134]
  644784: e5922008     	ldr	r2, [r2, #0x8]
  644788: e583203c     	str	r2, [r3, #0x3c]
  64478c: ebfcfd21     	bl	0x583c18 <glitch::scene::CLightSceneNode::doLightRecalc()> @ imm = #-0xc0b7c
  644790: e1a00004     	mov	r0, r4
  644794: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  644798: 94 05 35 00  	.word	0x00350594
  64479c: dc 3a 00 00  	.word	0x00003adc
  6447a0: 44 2b 00 00  	.word	0x00002b44
  6447a4: b4 17 00 00  	.word	0x000017b4
  6447a8: 48 10 00 00  	.word	0x00001048
