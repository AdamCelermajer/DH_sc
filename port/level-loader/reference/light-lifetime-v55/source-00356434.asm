
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00356434 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)>:
  356434: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  356438: e24dd05c     	sub	sp, sp, #92
  35643c: e5ddc08c     	ldrb	r12, [sp, #0x8c]
  356440: e58d2018     	str	r2, [sp, #0x18]
  356444: e58d301c     	str	r3, [sp, #0x1c]
  356448: e5dd2084     	ldrb	r2, [sp, #0x84]
  35644c: e5dd3088     	ldrb	r3, [sp, #0x88]
  356450: e2515000     	subs	r5, r1, #0
  356454: e58dc028     	str	r12, [sp, #0x28]
  356458: e58d0014     	str	r0, [sp, #0x14]
  35645c: e58d2020     	str	r2, [sp, #0x20]
  356460: e58d3024     	str	r3, [sp, #0x24]
  356464: 05905004     	ldreq	r5, [r0, #0x4]
  356468: e28d6040     	add	r6, sp, #64
  35646c: e3063164     	movw	r3, #0x6164
  356470: e3a04000     	mov	r4, #0
  356474: e3463d65     	movt	r3, #0x6d65
  356478: e59d0014     	ldr	r0, [sp, #0x14]
  35647c: e1a01005     	mov	r1, r5
  356480: e1a02006     	mov	r2, r6
  356484: e58d4040     	str	r4, [sp, #0x40]
  356488: e58d4044     	str	r4, [sp, #0x44]
  35648c: e58d4048     	str	r4, [sp, #0x48]
  356490: ebffea71     	bl	0x350e5c <SceneManager::SearchByType(glitch::scene::ISceneNode*, std::vector<glitch::scene::ISceneNode*, glitch::core::SAllocator<glitch::scene::ISceneNode*, (glitch::memory::E_MEMORY_HINT)0>>&, glitch::scene::E_SCENE_NODE_TYPE)> @ imm = #-0x563c
  356494: e3063164     	movw	r3, #0x6164
  356498: e59d0014     	ldr	r0, [sp, #0x14]
  35649c: e1a01005     	mov	r1, r5
  3564a0: e1a02006     	mov	r2, r6
  3564a4: e3473365     	movt	r3, #0x7365
  3564a8: ebffea6b     	bl	0x350e5c <SceneManager::SearchByType(glitch::scene::ISceneNode*, std::vector<glitch::scene::ISceneNode*, glitch::core::SAllocator<glitch::scene::ISceneNode*, (glitch::memory::E_MEMORY_HINT)0>>&, glitch::scene::E_SCENE_NODE_TYPE)> @ imm = #-0x5654
  3564ac: e3063164     	movw	r3, #0x6164
  3564b0: e59d0014     	ldr	r0, [sp, #0x14]
  3564b4: e1a02006     	mov	r2, r6
  3564b8: e3443d65     	movt	r3, #0x4d65
  3564bc: e1a01005     	mov	r1, r5
  3564c0: ebffea65     	bl	0x350e5c <SceneManager::SearchByType(glitch::scene::ISceneNode*, std::vector<glitch::scene::ISceneNode*, glitch::core::SAllocator<glitch::scene::ISceneNode*, (glitch::memory::E_MEMORY_HINT)0>>&, glitch::scene::E_SCENE_NODE_TYPE)> @ imm = #-0x566c
  3564c4: e59d3040     	ldr	r3, [sp, #0x40]
  3564c8: e59d2044     	ldr	r2, [sp, #0x44]
  3564cc: e1a00003     	mov	r0, r3
  3564d0: e0632002     	rsb	r2, r3, r2
  3564d4: e1b02142     	asrs	r2, r2, #2
  3564d8: e58d203c     	str	r2, [sp, #0x3c]
  3564dc: 0a000079     	beq	0x3566c8 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x294> @ imm = #0x1e4
  3564e0: e59f21f4     	ldr	r2, [pc, #0x1f4]        @ 0x3566dc <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x2a8>
  3564e4: e59f91f4     	ldr	r9, [pc, #0x1f4]        @ 0x3566e0 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x2ac>
  3564e8: e58d4030     	str	r4, [sp, #0x30]
  3564ec: e08f2002     	add	r2, pc, r2
  3564f0: e58d202c     	str	r2, [sp, #0x2c]
  3564f4: e59f21e8     	ldr	r2, [pc, #0x1e8]        @ 0x3566e4 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x2b0>
  3564f8: e08f9009     	add	r9, pc, r9
  3564fc: e08f2002     	add	r2, pc, r2
  356500: e58d2034     	str	r2, [sp, #0x34]
  356504: e59f21dc     	ldr	r2, [pc, #0x1dc]        @ 0x3566e8 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x2b4>
  356508: e08f2002     	add	r2, pc, r2
  35650c: e58d2038     	str	r2, [sp, #0x38]
  356510: e59d2030     	ldr	r2, [sp, #0x30]
  356514: e7935102     	ldr	r5, [r3, r2, lsl #2]
  356518: e5953000     	ldr	r3, [r5]
  35651c: e1a00005     	mov	r0, r5
  356520: e1a0e00f     	mov	lr, pc
  356524: e593f088     	ldr	pc, [r3, #0x88]
  356528: e2506000     	subs	r6, r0, #0
  35652c: da00005d     	ble	0x3566a8 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x274> @ imm = #0x174
  356530: e3a04000     	mov	r4, #0
  356534: e28d7054     	add	r7, sp, #84
  356538: e28d8050     	add	r8, sp, #80
  35653c: e28db04c     	add	r11, sp, #76
  356540: e1a0a006     	mov	r10, r6
  356544: ea00002f     	b	0x356608 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x1d4> @ imm = #0xbc
  356548: e59d3054     	ldr	r3, [sp, #0x54]
  35654c: e1a0100b     	mov	r1, r11
  356550: e3530000     	cmp	r3, #0
  356554: e58d304c     	str	r3, [sp, #0x4c]
  356558: 15932000     	ldrne	r2, [r3]
  35655c: 12822001     	addne	r2, r2, #1
  356560: 15832000     	strne	r2, [r3]
  356564: e59dc080     	ldr	r12, [sp, #0x80]
  356568: e59d0014     	ldr	r0, [sp, #0x14]
  35656c: e59d2018     	ldr	r2, [sp, #0x18]
  356570: e58dc000     	str	r12, [sp]
  356574: e59dc020     	ldr	r12, [sp, #0x20]
  356578: e59d301c     	ldr	r3, [sp, #0x1c]
  35657c: e58dc004     	str	r12, [sp, #0x4]
  356580: e59dc024     	ldr	r12, [sp, #0x24]
  356584: e58dc008     	str	r12, [sp, #0x8]
  356588: e59dc028     	ldr	r12, [sp, #0x28]
  35658c: e58dc00c     	str	r12, [sp, #0xc]
  356590: ebfffd96     	bl	0x355bf0 <SceneManager::ChangeCustomTechnique(boost::intrusive_ptr<glitch::video::CMaterial>, bool, bool, SceneManager::EForceBool, bool, bool, bool)> @ imm = #-0x9a8
  356594: e59d604c     	ldr	r6, [sp, #0x4c]
  356598: e3560000     	cmp	r6, #0
  35659c: 0a000008     	beq	0x3565c4 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x190> @ imm = #0x20
  3565a0: e5963000     	ldr	r3, [r6]
  3565a4: e2433001     	sub	r3, r3, #1
  3565a8: e3530000     	cmp	r3, #0
  3565ac: e5863000     	str	r3, [r6]
  3565b0: 1a000003     	bne	0x3565c4 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x190> @ imm = #0xc
  3565b4: e1a00006     	mov	r0, r6
  3565b8: eb09d66e     	bl	0x5cbf78 <glitch::video::CMaterial::~CMaterial()> @ imm = #0x2759b8
  3565bc: e1a00006     	mov	r0, r6
  3565c0: ebfee79e     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x46188
  3565c4: e1a00008     	mov	r0, r8
  3565c8: ebffef3a     	bl	0x3522b8 <boost::intrusive_ptr<glitch::video::CMaterialRenderer>::~intrusive_ptr()> @ imm = #-0x4318
  3565cc: e59d6054     	ldr	r6, [sp, #0x54]
  3565d0: e3560000     	cmp	r6, #0
  3565d4: 0a000008     	beq	0x3565fc <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x1c8> @ imm = #0x20
  3565d8: e5963000     	ldr	r3, [r6]
  3565dc: e2433001     	sub	r3, r3, #1
  3565e0: e3530000     	cmp	r3, #0
  3565e4: e5863000     	str	r3, [r6]
  3565e8: 1a000003     	bne	0x3565fc <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x1c8> @ imm = #0xc
  3565ec: e1a00006     	mov	r0, r6
  3565f0: eb09d660     	bl	0x5cbf78 <glitch::video::CMaterial::~CMaterial()> @ imm = #0x275980
  3565f4: e1a00006     	mov	r0, r6
  3565f8: ebfee790     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x461c0
  3565fc: e2844001     	add	r4, r4, #1
  356600: e15a0004     	cmp	r10, r4
  356604: 0a000027     	beq	0x3566a8 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x274> @ imm = #0x9c
  356608: e1a02004     	mov	r2, r4
  35660c: e5953000     	ldr	r3, [r5]
  356610: e1a00007     	mov	r0, r7
  356614: e1a01005     	mov	r1, r5
  356618: e1a0e00f     	mov	lr, pc
  35661c: e593f084     	ldr	pc, [r3, #0x84]
  356620: e59d3054     	ldr	r3, [sp, #0x54]
  356624: e1a01009     	mov	r1, r9
  356628: e5933004     	ldr	r3, [r3, #0x4]
  35662c: e3530000     	cmp	r3, #0
  356630: e58d3050     	str	r3, [sp, #0x50]
  356634: 15932000     	ldrne	r2, [r3]
  356638: 12822001     	addne	r2, r2, #1
  35663c: 15832000     	strne	r2, [r3]
  356640: 159d3050     	ldrne	r3, [sp, #0x50]
  356644: e3a0200a     	mov	r2, #10
  356648: e5936008     	ldr	r6, [r3, #0x8]
  35664c: e1a00006     	mov	r0, r6
  356650: ebfee189     	bl	0x30ec7c <.plt+0xf08>   @ imm = #-0x479dc
  356654: e3500000     	cmp	r0, #0
  356658: 0affffba     	beq	0x356548 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x114> @ imm = #-0x118
  35665c: e1a00006     	mov	r0, r6
  356660: e59d102c     	ldr	r1, [sp, #0x2c]
  356664: e3a0200c     	mov	r2, #12
  356668: ebfee183     	bl	0x30ec7c <.plt+0xf08>   @ imm = #-0x479f4
  35666c: e3500000     	cmp	r0, #0
  356670: 0affffb4     	beq	0x356548 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x114> @ imm = #-0x130
  356674: e1a00006     	mov	r0, r6
  356678: e59d1034     	ldr	r1, [sp, #0x34]
  35667c: e3a0200f     	mov	r2, #15
  356680: ebfee17d     	bl	0x30ec7c <.plt+0xf08>   @ imm = #-0x47a0c
  356684: e3500000     	cmp	r0, #0
  356688: 0affffae     	beq	0x356548 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x114> @ imm = #-0x148
  35668c: e1a00006     	mov	r0, r6
  356690: e59d1038     	ldr	r1, [sp, #0x38]
  356694: e3a02008     	mov	r2, #8
  356698: ebfee177     	bl	0x30ec7c <.plt+0xf08>   @ imm = #-0x47a24
  35669c: e3500000     	cmp	r0, #0
  3566a0: 1affffc7     	bne	0x3565c4 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x190> @ imm = #-0xe4
  3566a4: eaffffa7     	b	0x356548 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x114> @ imm = #-0x164
  3566a8: e59d2030     	ldr	r2, [sp, #0x30]
  3566ac: e59d303c     	ldr	r3, [sp, #0x3c]
  3566b0: e2822001     	add	r2, r2, #1
  3566b4: e1520003     	cmp	r2, r3
  3566b8: e58d2030     	str	r2, [sp, #0x30]
  3566bc: 159d3040     	ldrne	r3, [sp, #0x40]
  3566c0: 1affff92     	bne	0x356510 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0xdc> @ imm = #-0x1b8
  3566c4: e59d0040     	ldr	r0, [sp, #0x40]
  3566c8: e3500000     	cmp	r0, #0
  3566cc: 0a000000     	beq	0x3566d4 <SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, SceneManager::EForceBool, bool, bool, bool)+0x2a0> @ imm = #0x0
  3566d0: ebfee75e     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x46288
  3566d4: e28dd05c     	add	sp, sp, #92
  3566d8: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3566dc: 1c a4 56 00  	.word	0x0056a41c
  3566e0: 40 a3 56 00  	.word	0x0056a340
  3566e4: 4c a3 56 00  	.word	0x0056a34c
  3566e8: 10 a4 56 00  	.word	0x0056a410
