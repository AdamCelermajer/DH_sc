
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00355484 <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)>:
  355484: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  355488: e24dd04c     	sub	sp, sp, #76
  35548c: e58d2010     	str	r2, [sp, #0x10]
  355490: e5dd2070     	ldrb	r2, [sp, #0x70]
  355494: e2515000     	subs	r5, r1, #0
  355498: e58d000c     	str	r0, [sp, #0xc]
  35549c: e58d3014     	str	r3, [sp, #0x14]
  3554a0: e58d2018     	str	r2, [sp, #0x18]
  3554a4: 05905004     	ldreq	r5, [r0, #0x4]
  3554a8: e28d6030     	add	r6, sp, #48
  3554ac: e3063164     	movw	r3, #0x6164
  3554b0: e3a04000     	mov	r4, #0
  3554b4: e3463d65     	movt	r3, #0x6d65
  3554b8: e59d000c     	ldr	r0, [sp, #0xc]
  3554bc: e1a01005     	mov	r1, r5
  3554c0: e1a02006     	mov	r2, r6
  3554c4: e58d4030     	str	r4, [sp, #0x30]
  3554c8: e58d4034     	str	r4, [sp, #0x34]
  3554cc: e58d4038     	str	r4, [sp, #0x38]
  3554d0: ebffee61     	bl	0x350e5c <SceneManager::SearchByType(glitch::scene::ISceneNode*, std::vector<glitch::scene::ISceneNode*, glitch::core::SAllocator<glitch::scene::ISceneNode*, (glitch::memory::E_MEMORY_HINT)0>>&, glitch::scene::E_SCENE_NODE_TYPE)> @ imm = #-0x467c
  3554d4: e3063164     	movw	r3, #0x6164
  3554d8: e59d000c     	ldr	r0, [sp, #0xc]
  3554dc: e1a01005     	mov	r1, r5
  3554e0: e1a02006     	mov	r2, r6
  3554e4: e3473365     	movt	r3, #0x7365
  3554e8: ebffee5b     	bl	0x350e5c <SceneManager::SearchByType(glitch::scene::ISceneNode*, std::vector<glitch::scene::ISceneNode*, glitch::core::SAllocator<glitch::scene::ISceneNode*, (glitch::memory::E_MEMORY_HINT)0>>&, glitch::scene::E_SCENE_NODE_TYPE)> @ imm = #-0x4694
  3554ec: e3063164     	movw	r3, #0x6164
  3554f0: e59d000c     	ldr	r0, [sp, #0xc]
  3554f4: e1a02006     	mov	r2, r6
  3554f8: e3443d65     	movt	r3, #0x4d65
  3554fc: e1a01005     	mov	r1, r5
  355500: ebffee55     	bl	0x350e5c <SceneManager::SearchByType(glitch::scene::ISceneNode*, std::vector<glitch::scene::ISceneNode*, glitch::core::SAllocator<glitch::scene::ISceneNode*, (glitch::memory::E_MEMORY_HINT)0>>&, glitch::scene::E_SCENE_NODE_TYPE)> @ imm = #-0x46ac
  355504: e59d3030     	ldr	r3, [sp, #0x30]
  355508: e59d2034     	ldr	r2, [sp, #0x34]
  35550c: e1a00003     	mov	r0, r3
  355510: e0632002     	rsb	r2, r3, r2
  355514: e1b02142     	asrs	r2, r2, #2
  355518: e58d202c     	str	r2, [sp, #0x2c]
  35551c: 0a00006f     	beq	0x3556e0 <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0x25c> @ imm = #0x1bc
  355520: e59f21cc     	ldr	r2, [pc, #0x1cc]        @ 0x3556f4 <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0x270>
  355524: e59f91cc     	ldr	r9, [pc, #0x1cc]        @ 0x3556f8 <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0x274>
  355528: e58d4020     	str	r4, [sp, #0x20]
  35552c: e08f2002     	add	r2, pc, r2
  355530: e58d201c     	str	r2, [sp, #0x1c]
  355534: e59f21c0     	ldr	r2, [pc, #0x1c0]        @ 0x3556fc <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0x278>
  355538: e08f9009     	add	r9, pc, r9
  35553c: e08f2002     	add	r2, pc, r2
  355540: e58d2024     	str	r2, [sp, #0x24]
  355544: e59f21b4     	ldr	r2, [pc, #0x1b4]        @ 0x355700 <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0x27c>
  355548: e08f2002     	add	r2, pc, r2
  35554c: e58d2028     	str	r2, [sp, #0x28]
  355550: e59dc020     	ldr	r12, [sp, #0x20]
  355554: e793510c     	ldr	r5, [r3, r12, lsl #2]
  355558: e5953000     	ldr	r3, [r5]
  35555c: e1a00005     	mov	r0, r5
  355560: e1a0e00f     	mov	lr, pc
  355564: e593f088     	ldr	pc, [r3, #0x88]
  355568: e2506000     	subs	r6, r0, #0
  35556c: da000053     	ble	0x3556c0 <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0x23c> @ imm = #0x14c
  355570: e3a04000     	mov	r4, #0
  355574: e28d7044     	add	r7, sp, #68
  355578: e28d8040     	add	r8, sp, #64
  35557c: e28db03c     	add	r11, sp, #60
  355580: e1a0a006     	mov	r10, r6
  355584: e1a02004     	mov	r2, r4
  355588: e5953000     	ldr	r3, [r5]
  35558c: e1a00007     	mov	r0, r7
  355590: e1a01005     	mov	r1, r5
  355594: e1a0e00f     	mov	lr, pc
  355598: e593f084     	ldr	pc, [r3, #0x84]
  35559c: e59d3044     	ldr	r3, [sp, #0x44]
  3555a0: e1a01009     	mov	r1, r9
  3555a4: e5933004     	ldr	r3, [r3, #0x4]
  3555a8: e3530000     	cmp	r3, #0
  3555ac: e58d3040     	str	r3, [sp, #0x40]
  3555b0: 15932000     	ldrne	r2, [r3]
  3555b4: 12822001     	addne	r2, r2, #1
  3555b8: 15832000     	strne	r2, [r3]
  3555bc: 159d3040     	ldrne	r3, [sp, #0x40]
  3555c0: e3a0200a     	mov	r2, #10
  3555c4: e5936008     	ldr	r6, [r3, #0x8]
  3555c8: e1a00006     	mov	r0, r6
  3555cc: ebfee5aa     	bl	0x30ec7c <.plt+0xf08>   @ imm = #-0x46958
  3555d0: e3500000     	cmp	r0, #0
  3555d4: 0a000028     	beq	0x35567c <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0x1f8> @ imm = #0xa0
  3555d8: e1a00006     	mov	r0, r6
  3555dc: e59d101c     	ldr	r1, [sp, #0x1c]
  3555e0: e3a0200c     	mov	r2, #12
  3555e4: ebfee5a4     	bl	0x30ec7c <.plt+0xf08>   @ imm = #-0x46970
  3555e8: e3500000     	cmp	r0, #0
  3555ec: 0a000022     	beq	0x35567c <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0x1f8> @ imm = #0x88
  3555f0: e1a00006     	mov	r0, r6
  3555f4: e59d1024     	ldr	r1, [sp, #0x24]
  3555f8: e3a0200f     	mov	r2, #15
  3555fc: ebfee59e     	bl	0x30ec7c <.plt+0xf08>   @ imm = #-0x46988
  355600: e3500000     	cmp	r0, #0
  355604: 0a00001c     	beq	0x35567c <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0x1f8> @ imm = #0x70
  355608: e1a00006     	mov	r0, r6
  35560c: e59d1028     	ldr	r1, [sp, #0x28]
  355610: e3a02008     	mov	r2, #8
  355614: ebfee598     	bl	0x30ec7c <.plt+0xf08>   @ imm = #-0x469a0
  355618: e3500000     	cmp	r0, #0
  35561c: 0a000016     	beq	0x35567c <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0x1f8> @ imm = #0x58
  355620: e59d3044     	ldr	r3, [sp, #0x44]
  355624: e1a0100b     	mov	r1, r11
  355628: e3530000     	cmp	r3, #0
  35562c: e58d303c     	str	r3, [sp, #0x3c]
  355630: 15932000     	ldrne	r2, [r3]
  355634: 12822001     	addne	r2, r2, #1
  355638: 15832000     	strne	r2, [r3]
  35563c: e28d000c     	add	r0, sp, #12
  355640: e890100d     	ldm	r0, {r0, r2, r3, r12}
  355644: e58dc000     	str	r12, [sp]
  355648: ebfffee5     	bl	0x3551e4 <SceneManager::ChangeCommonTechnique(boost::intrusive_ptr<glitch::video::CMaterial>, bool, bool, bool)> @ imm = #-0x46c
  35564c: e59d603c     	ldr	r6, [sp, #0x3c]
  355650: e3560000     	cmp	r6, #0
  355654: 0a000008     	beq	0x35567c <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0x1f8> @ imm = #0x20
  355658: e5963000     	ldr	r3, [r6]
  35565c: e2433001     	sub	r3, r3, #1
  355660: e3530000     	cmp	r3, #0
  355664: e5863000     	str	r3, [r6]
  355668: 1a000003     	bne	0x35567c <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0x1f8> @ imm = #0xc
  35566c: e1a00006     	mov	r0, r6
  355670: eb09da40     	bl	0x5cbf78 <glitch::video::CMaterial::~CMaterial()> @ imm = #0x276900
  355674: e1a00006     	mov	r0, r6
  355678: ebfeeb70     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x45240
  35567c: e1a00008     	mov	r0, r8
  355680: ebfff30c     	bl	0x3522b8 <boost::intrusive_ptr<glitch::video::CMaterialRenderer>::~intrusive_ptr()> @ imm = #-0x33d0
  355684: e59d6044     	ldr	r6, [sp, #0x44]
  355688: e3560000     	cmp	r6, #0
  35568c: 0a000008     	beq	0x3556b4 <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0x230> @ imm = #0x20
  355690: e5963000     	ldr	r3, [r6]
  355694: e2433001     	sub	r3, r3, #1
  355698: e3530000     	cmp	r3, #0
  35569c: e5863000     	str	r3, [r6]
  3556a0: 1a000003     	bne	0x3556b4 <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0x230> @ imm = #0xc
  3556a4: e1a00006     	mov	r0, r6
  3556a8: eb09da32     	bl	0x5cbf78 <glitch::video::CMaterial::~CMaterial()> @ imm = #0x2768c8
  3556ac: e1a00006     	mov	r0, r6
  3556b0: ebfeeb62     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x45278
  3556b4: e2844001     	add	r4, r4, #1
  3556b8: e15a0004     	cmp	r10, r4
  3556bc: 1affffb0     	bne	0x355584 <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0x100> @ imm = #-0x140
  3556c0: e59d2020     	ldr	r2, [sp, #0x20]
  3556c4: e59d302c     	ldr	r3, [sp, #0x2c]
  3556c8: e2822001     	add	r2, r2, #1
  3556cc: e1520003     	cmp	r2, r3
  3556d0: e58d2020     	str	r2, [sp, #0x20]
  3556d4: 159d3030     	ldrne	r3, [sp, #0x30]
  3556d8: 1affff9c     	bne	0x355550 <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0xcc> @ imm = #-0x190
  3556dc: e59d0030     	ldr	r0, [sp, #0x30]
  3556e0: e3500000     	cmp	r0, #0
  3556e4: 0a000000     	beq	0x3556ec <SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool, bool)+0x268> @ imm = #0x0
  3556e8: ebfeeb58     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x452a0
  3556ec: e28dd04c     	add	sp, sp, #76
  3556f0: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3556f4: dc b3 56 00  	.word	0x0056b3dc
  3556f8: 00 b3 56 00  	.word	0x0056b300
  3556fc: 0c b3 56 00  	.word	0x0056b30c
  355700: d0 b3 56 00  	.word	0x0056b3d0
