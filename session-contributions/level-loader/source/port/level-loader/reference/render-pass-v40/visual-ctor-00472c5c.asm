
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00472c5c <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)>:
  472c5c: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  472c60: e59f6234     	ldr	r6, [pc, #0x234]        @ 0x472e9c <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)+0x240>
  472c64: e59f5234     	ldr	r5, [pc, #0x234]        @ 0x472ea0 <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)+0x244>
  472c68: e3a0c4bf     	mov	r12, #-1090519040
  472c6c: e08f6006     	add	r6, pc, r6
  472c70: e7965005     	ldr	r5, [r6, r5]
  472c74: e3a0e000     	mov	lr, #0
  472c78: e28cc502     	add	r12, r12, #8388608
  472c7c: e1a07001     	mov	r7, r1
  472c80: e2851008     	add	r1, r5, #8
  472c84: e3a05000     	mov	r5, #0
  472c88: e1a08003     	mov	r8, r3
  472c8c: e24dd00c     	sub	sp, sp, #12
  472c90: e5801000     	str	r1, [r0]
  472c94: e580e024     	str	lr, [r0, #0x24]
  472c98: e580c074     	str	r12, [r0, #0x74]
  472c9c: e580e010     	str	lr, [r0, #0x10]
  472ca0: e580e014     	str	lr, [r0, #0x14]
  472ca4: e580e018     	str	lr, [r0, #0x18]
  472ca8: e580e01c     	str	lr, [r0, #0x1c]
  472cac: e580e020     	str	lr, [r0, #0x20]
  472cb0: e580c058     	str	r12, [r0, #0x58]
  472cb4: e580c05c     	str	r12, [r0, #0x5c]
  472cb8: e580c060     	str	r12, [r0, #0x60]
  472cbc: e580c064     	str	r12, [r0, #0x64]
  472cc0: e580c068     	str	r12, [r0, #0x68]
  472cc4: e5807004     	str	r7, [r0, #0x4]
  472cc8: e5805008     	str	r5, [r0, #0x8]
  472ccc: e580500c     	str	r5, [r0, #0xc]
  472cd0: e5c05028     	strb	r5, [r0, #0x28]
  472cd4: e580502c     	str	r5, [r0, #0x2c]
  472cd8: e5805030     	str	r5, [r0, #0x30]
  472cdc: e5805034     	str	r5, [r0, #0x34]
  472ce0: e5805038     	str	r5, [r0, #0x38]
  472ce4: e5c0503c     	strb	r5, [r0, #0x3c]
  472ce8: e5805040     	str	r5, [r0, #0x40]
  472cec: e5805044     	str	r5, [r0, #0x44]
  472cf0: e5805048     	str	r5, [r0, #0x48]
  472cf4: e580504c     	str	r5, [r0, #0x4c]
  472cf8: e5805050     	str	r5, [r0, #0x50]
  472cfc: e5805054     	str	r5, [r0, #0x54]
  472d00: e5c0506c     	strb	r5, [r0, #0x6c]
  472d04: e5c0507c     	strb	r5, [r0, #0x7c]
  472d08: e5c0507d     	strb	r5, [r0, #0x7d]
  472d0c: e5c0507e     	strb	r5, [r0, #0x7e]
  472d10: e5c0507f     	strb	r5, [r0, #0x7f]
  472d14: e5805080     	str	r5, [r0, #0x80]
  472d18: e5805084     	str	r5, [r0, #0x84]
  472d1c: e5805088     	str	r5, [r0, #0x88]
  472d20: e580508c     	str	r5, [r0, #0x8c]
  472d24: e5805090     	str	r5, [r0, #0x90]
  472d28: e5805094     	str	r5, [r0, #0x94]
  472d2c: e580509c     	str	r5, [r0, #0x9c]
  472d30: e58050a0     	str	r5, [r0, #0xa0]
  472d34: e58050a4     	str	r5, [r0, #0xa4]
  472d38: e5c050a9     	strb	r5, [r0, #0xa9]
  472d3c: e1a0a002     	mov	r10, r2
  472d40: e1a04000     	mov	r4, r0
  472d44: eb025e06     	bl	0x50a564 <AssetManager::GetAssetManager()> @ imm = #0x97818
  472d48: e598c010     	ldr	r12, [r8, #0x10]
  472d4c: e5982014     	ldr	r2, [r8, #0x14]
  472d50: e59a1014     	ldr	r1, [r10, #0x14]
  472d54: e1a03005     	mov	r3, r5
  472d58: e15c0002     	cmp	r12, r2
  472d5c: 01a02005     	moveq	r2, r5
  472d60: e3e0c102     	mvn	r12, #-2147483648
  472d64: e58dc000     	str	r12, [sp]
  472d68: eb025de5     	bl	0x50a504 <AssetManager::loadSceneNode(char const*, char const*, bool, int)> @ imm = #0x97794
  472d6c: e1500005     	cmp	r0, r5
  472d70: e5840008     	str	r0, [r4, #0x8]
  472d74: 0a000045     	beq	0x472e90 <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)+0x234> @ imm = #0x114
  472d78: e1a01007     	mov	r1, r7
  472d7c: e1a00004     	mov	r0, r4
  472d80: ebfffef5     	bl	0x47295c <VisualObject::SetParent(GameObject*)> @ imm = #-0x42c
  472d84: e5940008     	ldr	r0, [r4, #0x8]
  472d88: ebfba6b1     	bl	0x35c854 <RootSceneNode::RefreshBoundingBox()> @ imm = #-0x11653c
  472d8c: e1a00004     	mov	r0, r4
  472d90: ebfffad6     	bl	0x4718f0 <VisualObject::_FindModularSkinnedMeshNode()> @ imm = #-0x14a8
  472d94: e59f3108     	ldr	r3, [pc, #0x108]        @ 0x472ea4 <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)+0x248>
  472d98: e5941008     	ldr	r1, [r4, #0x8]
  472d9c: e7965003     	ldr	r5, [r6, r3]
  472da0: e5953010     	ldr	r3, [r5, #0x10]
  472da4: e593301c     	ldr	r3, [r3, #0x1c]
  472da8: e5933004     	ldr	r3, [r3, #0x4]
  472dac: e1a00003     	mov	r0, r3
  472db0: e5933000     	ldr	r3, [r3]
  472db4: e1a0e00f     	mov	lr, pc
  472db8: e593f05c     	ldr	pc, [r3, #0x5c]
  472dbc: e5953010     	ldr	r3, [r5, #0x10]
  472dc0: e593001c     	ldr	r0, [r3, #0x1c]
  472dc4: ebfb7845     	bl	0x350ee0 <SceneManager::ForceRegister()> @ imm = #-0x121eec
  472dc8: e5953010     	ldr	r3, [r5, #0x10]
  472dcc: e59f20d4     	ldr	r2, [pc, #0xd4]         @ 0x472ea8 <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)+0x24c>
  472dd0: e5941008     	ldr	r1, [r4, #0x8]
  472dd4: e593001c     	ldr	r0, [r3, #0x1c]
  472dd8: e08f2002     	add	r2, pc, r2
  472ddc: e3a03001     	mov	r3, #1
  472de0: ebfb9cbf     	bl	0x35a0e4 <SceneManager::SearchByName(glitch::scene::ISceneNode*, char const*, bool)> @ imm = #-0x118d04
  472de4: e2502000     	subs	r2, r0, #0
  472de8: 0a00001a     	beq	0x472e58 <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)+0x1fc> @ imm = #0x68
  472dec: e3a03001     	mov	r3, #1
  472df0: e5c43028     	strb	r3, [r4, #0x28]
  472df4: e5953010     	ldr	r3, [r5, #0x10]
  472df8: e3061164     	movw	r1, #0x6164
  472dfc: e3461d65     	movt	r1, #0x6d65
  472e00: e593301c     	ldr	r3, [r3, #0x1c]
  472e04: e1a00003     	mov	r0, r3
  472e08: e5933000     	ldr	r3, [r3]
  472e0c: e1a0e00f     	mov	lr, pc
  472e10: e593f01c     	ldr	pc, [r3, #0x1c]
  472e14: e3500000     	cmp	r0, #0
  472e18: e584000c     	str	r0, [r4, #0xc]
  472e1c: 0a000006     	beq	0x472e3c <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)+0x1e0> @ imm = #0x18
  472e20: e5903000     	ldr	r3, [r0]
  472e24: e513300c     	ldr	r3, [r3, #-0xc]
  472e28: e0800003     	add	r0, r0, r3
  472e2c: e5903004     	ldr	r3, [r0, #0x4]
  472e30: e2833001     	add	r3, r3, #1
  472e34: e5803004     	str	r3, [r0, #0x4]
  472e38: e594000c     	ldr	r0, [r4, #0xc]
  472e3c: e3a01000     	mov	r1, #0
  472e40: e5c01138     	strb	r1, [r0, #0x138]
  472e44: e594300c     	ldr	r3, [r4, #0xc]
  472e48: e1a00003     	mov	r0, r3
  472e4c: e5933000     	ldr	r3, [r3]
  472e50: e1a0e00f     	mov	lr, pc
  472e54: e593f048     	ldr	pc, [r3, #0x48]
  472e58: e1a00004     	mov	r0, r4
  472e5c: ebfffcae     	bl	0x47211c <VisualObject::CalcMeshBox()> @ imm = #-0xd48
  472e60: e1a00004     	mov	r0, r4
  472e64: ebfff6fa     	bl	0x470a54 <VisualObject::ApplyMeshBox()> @ imm = #-0x2418
  472e68: e3a01000     	mov	r1, #0
  472e6c: e3a00008     	mov	r0, #8
  472e70: ebfa75be     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x162908
  472e74: e5941008     	ldr	r1, [r4, #0x8]
  472e78: e1a05000     	mov	r5, r0
  472e7c: e3a02000     	mov	r2, #0
  472e80: eb0007aa     	bl	0x474d30 <AnimController::AnimController(RootSceneNode*, bool)> @ imm = #0x1ea8
  472e84: e1a00004     	mov	r0, r4
  472e88: e1a01005     	mov	r1, r5
  472e8c: ebfff6fc     	bl	0x470a84 <VisualObject::SetAnimController(AnimController*)> @ imm = #-0x2410
  472e90: e1a00004     	mov	r0, r4
  472e94: e28dd00c     	add	sp, sp, #12
  472e98: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  472e9c: 24 1e 52 00  	.word	0x00521e24
  472ea0: 94 1e 00 00  	.word	0x00001e94
  472ea4: f4 37 00 00  	.word	0x000037f4
  472ea8: d0 a8 45 00  	.word	0x0045a8d0
