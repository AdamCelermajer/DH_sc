
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003559e8 <SceneManager::ResetCustomShadowProjectionTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool)>:
  3559e8: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3559ec: e24dd034     	sub	sp, sp, #52
  3559f0: e2515000     	subs	r5, r1, #0
  3559f4: e58d2008     	str	r2, [sp, #0x8]
  3559f8: 05905004     	ldreq	r5, [r0, #0x4]
  3559fc: e28d6018     	add	r6, sp, #24
  355a00: e1a0b003     	mov	r11, r3
  355a04: e3063164     	movw	r3, #0x6164
  355a08: e3a04000     	mov	r4, #0
  355a0c: e3463d65     	movt	r3, #0x6d65
  355a10: e1a01005     	mov	r1, r5
  355a14: e1a02006     	mov	r2, r6
  355a18: e1a09000     	mov	r9, r0
  355a1c: e58d4018     	str	r4, [sp, #0x18]
  355a20: e58d401c     	str	r4, [sp, #0x1c]
  355a24: e58d4020     	str	r4, [sp, #0x20]
  355a28: ebffed0b     	bl	0x350e5c <SceneManager::SearchByType(glitch::scene::ISceneNode*, std::vector<glitch::scene::ISceneNode*, glitch::core::SAllocator<glitch::scene::ISceneNode*, (glitch::memory::E_MEMORY_HINT)0>>&, glitch::scene::E_SCENE_NODE_TYPE)> @ imm = #-0x4bd4
  355a2c: e3063164     	movw	r3, #0x6164
  355a30: e1a00009     	mov	r0, r9
  355a34: e1a01005     	mov	r1, r5
  355a38: e1a02006     	mov	r2, r6
  355a3c: e3473365     	movt	r3, #0x7365
  355a40: ebffed05     	bl	0x350e5c <SceneManager::SearchByType(glitch::scene::ISceneNode*, std::vector<glitch::scene::ISceneNode*, glitch::core::SAllocator<glitch::scene::ISceneNode*, (glitch::memory::E_MEMORY_HINT)0>>&, glitch::scene::E_SCENE_NODE_TYPE)> @ imm = #-0x4bec
  355a44: e3063164     	movw	r3, #0x6164
  355a48: e1a00009     	mov	r0, r9
  355a4c: e1a02006     	mov	r2, r6
  355a50: e3443d65     	movt	r3, #0x4d65
  355a54: e1a01005     	mov	r1, r5
  355a58: ebffecff     	bl	0x350e5c <SceneManager::SearchByType(glitch::scene::ISceneNode*, std::vector<glitch::scene::ISceneNode*, glitch::core::SAllocator<glitch::scene::ISceneNode*, (glitch::memory::E_MEMORY_HINT)0>>&, glitch::scene::E_SCENE_NODE_TYPE)> @ imm = #-0x4c04
  355a5c: e59d3018     	ldr	r3, [sp, #0x18]
  355a60: e59d201c     	ldr	r2, [sp, #0x1c]
  355a64: e1a00003     	mov	r0, r3
  355a68: e0632002     	rsb	r2, r3, r2
  355a6c: e1b02142     	asrs	r2, r2, #2
  355a70: e58d2014     	str	r2, [sp, #0x14]
  355a74: 0a000057     	beq	0x355bd8 <SceneManager::ResetCustomShadowProjectionTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool)+0x1f0> @ imm = #0x15c
  355a78: e59f816c     	ldr	r8, [pc, #0x16c]        @ 0x355bec <SceneManager::ResetCustomShadowProjectionTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool)+0x204>
  355a7c: e28d2024     	add	r2, sp, #36
  355a80: e58d4010     	str	r4, [sp, #0x10]
  355a84: e28d702c     	add	r7, sp, #44
  355a88: e08f8008     	add	r8, pc, r8
  355a8c: e28da028     	add	r10, sp, #40
  355a90: e58d200c     	str	r2, [sp, #0xc]
  355a94: e59d2010     	ldr	r2, [sp, #0x10]
  355a98: e7935102     	ldr	r5, [r3, r2, lsl #2]
  355a9c: e5953000     	ldr	r3, [r5]
  355aa0: e1a00005     	mov	r0, r5
  355aa4: e1a0e00f     	mov	lr, pc
  355aa8: e593f088     	ldr	pc, [r3, #0x88]
  355aac: e2506000     	subs	r6, r0, #0
  355ab0: da000040     	ble	0x355bb8 <SceneManager::ResetCustomShadowProjectionTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool)+0x1d0> @ imm = #0x100
  355ab4: e3a04000     	mov	r4, #0
  355ab8: e1a02004     	mov	r2, r4
  355abc: e5953000     	ldr	r3, [r5]
  355ac0: e1a00007     	mov	r0, r7
  355ac4: e1a01005     	mov	r1, r5
  355ac8: e1a0e00f     	mov	lr, pc
  355acc: e593f084     	ldr	pc, [r3, #0x84]
  355ad0: e59d302c     	ldr	r3, [sp, #0x2c]
  355ad4: e1a01008     	mov	r1, r8
  355ad8: e5933004     	ldr	r3, [r3, #0x4]
  355adc: e3530000     	cmp	r3, #0
  355ae0: e58d3028     	str	r3, [sp, #0x28]
  355ae4: 15932000     	ldrne	r2, [r3]
  355ae8: 12822001     	addne	r2, r2, #1
  355aec: 15832000     	strne	r2, [r3]
  355af0: 159d3028     	ldrne	r3, [sp, #0x28]
  355af4: e3a02013     	mov	r2, #19
  355af8: e5930008     	ldr	r0, [r3, #0x8]
  355afc: ebfee45e     	bl	0x30ec7c <.plt+0xf08>   @ imm = #-0x46e88
  355b00: e3500000     	cmp	r0, #0
  355b04: 0a000018     	beq	0x355b6c <SceneManager::ResetCustomShadowProjectionTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool)+0x184> @ imm = #0x60
  355b08: e59d302c     	ldr	r3, [sp, #0x2c]
  355b0c: e1a00009     	mov	r0, r9
  355b10: e3530000     	cmp	r3, #0
  355b14: e58d3024     	str	r3, [sp, #0x24]
  355b18: 15932000     	ldrne	r2, [r3]
  355b1c: 12822001     	addne	r2, r2, #1
  355b20: 15832000     	strne	r2, [r3]
  355b24: e59d100c     	ldr	r1, [sp, #0xc]
  355b28: e1a0300b     	mov	r3, r11
  355b2c: e59d2008     	ldr	r2, [sp, #0x8]
  355b30: ebffff56     	bl	0x355890 <SceneManager::ChangeCustomShadowProjectionTechnique(boost::intrusive_ptr<glitch::video::CMaterial>, bool, bool)> @ imm = #-0x2a8
  355b34: e59d3024     	ldr	r3, [sp, #0x24]
  355b38: e3530000     	cmp	r3, #0
  355b3c: 0a00000a     	beq	0x355b6c <SceneManager::ResetCustomShadowProjectionTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool)+0x184> @ imm = #0x28
  355b40: e5932000     	ldr	r2, [r3]
  355b44: e2422001     	sub	r2, r2, #1
  355b48: e3520000     	cmp	r2, #0
  355b4c: e5832000     	str	r2, [r3]
  355b50: 1a000005     	bne	0x355b6c <SceneManager::ResetCustomShadowProjectionTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool)+0x184> @ imm = #0x14
  355b54: e1a00003     	mov	r0, r3
  355b58: e58d3004     	str	r3, [sp, #0x4]
  355b5c: eb09d905     	bl	0x5cbf78 <glitch::video::CMaterial::~CMaterial()> @ imm = #0x276414
  355b60: e59d3004     	ldr	r3, [sp, #0x4]
  355b64: e1a00003     	mov	r0, r3
  355b68: ebfeea34     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x45730
  355b6c: e1a0000a     	mov	r0, r10
  355b70: ebfff1d0     	bl	0x3522b8 <boost::intrusive_ptr<glitch::video::CMaterialRenderer>::~intrusive_ptr()> @ imm = #-0x38c0
  355b74: e59d302c     	ldr	r3, [sp, #0x2c]
  355b78: e3530000     	cmp	r3, #0
  355b7c: 0a00000a     	beq	0x355bac <SceneManager::ResetCustomShadowProjectionTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool)+0x1c4> @ imm = #0x28
  355b80: e5932000     	ldr	r2, [r3]
  355b84: e2422001     	sub	r2, r2, #1
  355b88: e3520000     	cmp	r2, #0
  355b8c: e5832000     	str	r2, [r3]
  355b90: 1a000005     	bne	0x355bac <SceneManager::ResetCustomShadowProjectionTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool)+0x1c4> @ imm = #0x14
  355b94: e1a00003     	mov	r0, r3
  355b98: e58d3004     	str	r3, [sp, #0x4]
  355b9c: eb09d8f5     	bl	0x5cbf78 <glitch::video::CMaterial::~CMaterial()> @ imm = #0x2763d4
  355ba0: e59d3004     	ldr	r3, [sp, #0x4]
  355ba4: e1a00003     	mov	r0, r3
  355ba8: ebfeea24     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x45770
  355bac: e2844001     	add	r4, r4, #1
  355bb0: e1560004     	cmp	r6, r4
  355bb4: 1affffbf     	bne	0x355ab8 <SceneManager::ResetCustomShadowProjectionTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool)+0xd0> @ imm = #-0x104
  355bb8: e59d3010     	ldr	r3, [sp, #0x10]
  355bbc: e59d2014     	ldr	r2, [sp, #0x14]
  355bc0: e2833001     	add	r3, r3, #1
  355bc4: e1530002     	cmp	r3, r2
  355bc8: e58d3010     	str	r3, [sp, #0x10]
  355bcc: 159d3018     	ldrne	r3, [sp, #0x18]
  355bd0: 1affffaf     	bne	0x355a94 <SceneManager::ResetCustomShadowProjectionTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool)+0xac> @ imm = #-0x144
  355bd4: e59d0018     	ldr	r0, [sp, #0x18]
  355bd8: e3500000     	cmp	r0, #0
  355bdc: 0a000000     	beq	0x355be4 <SceneManager::ResetCustomShadowProjectionTechniqueToDefault(glitch::scene::ISceneNode*, bool, bool)+0x1fc> @ imm = #0x0
  355be0: ebfeea1a     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x45798
  355be4: e28dd034     	add	sp, sp, #52
  355be8: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  355bec: 60 ad 56 00  	.word	0x0056ad60
