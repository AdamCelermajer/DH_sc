
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00472a0c <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)>:
  472a0c: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  472a10: e59f6234     	ldr	r6, [pc, #0x234]        @ 0x472c4c <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)+0x240>
  472a14: e59f5234     	ldr	r5, [pc, #0x234]        @ 0x472c50 <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)+0x244>
  472a18: e3a0c4bf     	mov	r12, #-1090519040
  472a1c: e08f6006     	add	r6, pc, r6
  472a20: e7965005     	ldr	r5, [r6, r5]
  472a24: e3a0e000     	mov	lr, #0
  472a28: e28cc502     	add	r12, r12, #8388608
  472a2c: e1a07001     	mov	r7, r1
  472a30: e2851008     	add	r1, r5, #8
  472a34: e3a05000     	mov	r5, #0
  472a38: e1a08003     	mov	r8, r3
  472a3c: e24dd00c     	sub	sp, sp, #12
  472a40: e5801000     	str	r1, [r0]
  472a44: e580e024     	str	lr, [r0, #0x24]
  472a48: e580c074     	str	r12, [r0, #0x74]
  472a4c: e580e010     	str	lr, [r0, #0x10]
  472a50: e580e014     	str	lr, [r0, #0x14]
  472a54: e580e018     	str	lr, [r0, #0x18]
  472a58: e580e01c     	str	lr, [r0, #0x1c]
  472a5c: e580e020     	str	lr, [r0, #0x20]
  472a60: e580c058     	str	r12, [r0, #0x58]
  472a64: e580c05c     	str	r12, [r0, #0x5c]
  472a68: e580c060     	str	r12, [r0, #0x60]
  472a6c: e580c064     	str	r12, [r0, #0x64]
  472a70: e580c068     	str	r12, [r0, #0x68]
  472a74: e5807004     	str	r7, [r0, #0x4]
  472a78: e5805008     	str	r5, [r0, #0x8]
  472a7c: e580500c     	str	r5, [r0, #0xc]
  472a80: e5c05028     	strb	r5, [r0, #0x28]
  472a84: e580502c     	str	r5, [r0, #0x2c]
  472a88: e5805030     	str	r5, [r0, #0x30]
  472a8c: e5805034     	str	r5, [r0, #0x34]
  472a90: e5805038     	str	r5, [r0, #0x38]
  472a94: e5c0503c     	strb	r5, [r0, #0x3c]
  472a98: e5805040     	str	r5, [r0, #0x40]
  472a9c: e5805044     	str	r5, [r0, #0x44]
  472aa0: e5805048     	str	r5, [r0, #0x48]
  472aa4: e580504c     	str	r5, [r0, #0x4c]
  472aa8: e5805050     	str	r5, [r0, #0x50]
  472aac: e5805054     	str	r5, [r0, #0x54]
  472ab0: e5c0506c     	strb	r5, [r0, #0x6c]
  472ab4: e5c0507c     	strb	r5, [r0, #0x7c]
  472ab8: e5c0507d     	strb	r5, [r0, #0x7d]
  472abc: e5c0507e     	strb	r5, [r0, #0x7e]
  472ac0: e5c0507f     	strb	r5, [r0, #0x7f]
  472ac4: e5805080     	str	r5, [r0, #0x80]
  472ac8: e5805084     	str	r5, [r0, #0x84]
  472acc: e5805088     	str	r5, [r0, #0x88]
  472ad0: e580508c     	str	r5, [r0, #0x8c]
  472ad4: e5805090     	str	r5, [r0, #0x90]
  472ad8: e5805094     	str	r5, [r0, #0x94]
  472adc: e580509c     	str	r5, [r0, #0x9c]
  472ae0: e58050a0     	str	r5, [r0, #0xa0]
  472ae4: e58050a4     	str	r5, [r0, #0xa4]
  472ae8: e5c050a9     	strb	r5, [r0, #0xa9]
  472aec: e1a0a002     	mov	r10, r2
  472af0: e1a04000     	mov	r4, r0
  472af4: eb025e9a     	bl	0x50a564 <AssetManager::GetAssetManager()> @ imm = #0x97a68
  472af8: e598c010     	ldr	r12, [r8, #0x10]
  472afc: e5982014     	ldr	r2, [r8, #0x14]
  472b00: e59a1014     	ldr	r1, [r10, #0x14]
  472b04: e1a03005     	mov	r3, r5
  472b08: e15c0002     	cmp	r12, r2
  472b0c: 01a02005     	moveq	r2, r5
  472b10: e3e0c102     	mvn	r12, #-2147483648
  472b14: e58dc000     	str	r12, [sp]
  472b18: eb025e79     	bl	0x50a504 <AssetManager::loadSceneNode(char const*, char const*, bool, int)> @ imm = #0x979e4
  472b1c: e1500005     	cmp	r0, r5
  472b20: e5840008     	str	r0, [r4, #0x8]
  472b24: 0a000045     	beq	0x472c40 <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)+0x234> @ imm = #0x114
  472b28: e1a01007     	mov	r1, r7
  472b2c: e1a00004     	mov	r0, r4
  472b30: ebffff89     	bl	0x47295c <VisualObject::SetParent(GameObject*)> @ imm = #-0x1dc
  472b34: e5940008     	ldr	r0, [r4, #0x8]
  472b38: ebfba745     	bl	0x35c854 <RootSceneNode::RefreshBoundingBox()> @ imm = #-0x1162ec
  472b3c: e1a00004     	mov	r0, r4
  472b40: ebfffb6a     	bl	0x4718f0 <VisualObject::_FindModularSkinnedMeshNode()> @ imm = #-0x1258
  472b44: e59f3108     	ldr	r3, [pc, #0x108]        @ 0x472c54 <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)+0x248>
  472b48: e5941008     	ldr	r1, [r4, #0x8]
  472b4c: e7965003     	ldr	r5, [r6, r3]
  472b50: e5953010     	ldr	r3, [r5, #0x10]
  472b54: e593301c     	ldr	r3, [r3, #0x1c]
  472b58: e5933004     	ldr	r3, [r3, #0x4]
  472b5c: e1a00003     	mov	r0, r3
  472b60: e5933000     	ldr	r3, [r3]
  472b64: e1a0e00f     	mov	lr, pc
  472b68: e593f05c     	ldr	pc, [r3, #0x5c]
  472b6c: e5953010     	ldr	r3, [r5, #0x10]
  472b70: e593001c     	ldr	r0, [r3, #0x1c]
  472b74: ebfb78d9     	bl	0x350ee0 <SceneManager::ForceRegister()> @ imm = #-0x121c9c
  472b78: e5953010     	ldr	r3, [r5, #0x10]
  472b7c: e59f20d4     	ldr	r2, [pc, #0xd4]         @ 0x472c58 <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)+0x24c>
  472b80: e5941008     	ldr	r1, [r4, #0x8]
  472b84: e593001c     	ldr	r0, [r3, #0x1c]
  472b88: e08f2002     	add	r2, pc, r2
  472b8c: e3a03001     	mov	r3, #1
  472b90: ebfb9d53     	bl	0x35a0e4 <SceneManager::SearchByName(glitch::scene::ISceneNode*, char const*, bool)> @ imm = #-0x118ab4
  472b94: e2502000     	subs	r2, r0, #0
  472b98: 0a00001a     	beq	0x472c08 <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)+0x1fc> @ imm = #0x68
  472b9c: e3a03001     	mov	r3, #1
  472ba0: e5c43028     	strb	r3, [r4, #0x28]
  472ba4: e5953010     	ldr	r3, [r5, #0x10]
  472ba8: e3061164     	movw	r1, #0x6164
  472bac: e3461d65     	movt	r1, #0x6d65
  472bb0: e593301c     	ldr	r3, [r3, #0x1c]
  472bb4: e1a00003     	mov	r0, r3
  472bb8: e5933000     	ldr	r3, [r3]
  472bbc: e1a0e00f     	mov	lr, pc
  472bc0: e593f01c     	ldr	pc, [r3, #0x1c]
  472bc4: e3500000     	cmp	r0, #0
  472bc8: e584000c     	str	r0, [r4, #0xc]
  472bcc: 0a000006     	beq	0x472bec <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)+0x1e0> @ imm = #0x18
  472bd0: e5903000     	ldr	r3, [r0]
  472bd4: e513300c     	ldr	r3, [r3, #-0xc]
  472bd8: e0800003     	add	r0, r0, r3
  472bdc: e5903004     	ldr	r3, [r0, #0x4]
  472be0: e2833001     	add	r3, r3, #1
  472be4: e5803004     	str	r3, [r0, #0x4]
  472be8: e594000c     	ldr	r0, [r4, #0xc]
  472bec: e3a01000     	mov	r1, #0
  472bf0: e5c01138     	strb	r1, [r0, #0x138]
  472bf4: e594300c     	ldr	r3, [r4, #0xc]
  472bf8: e1a00003     	mov	r0, r3
  472bfc: e5933000     	ldr	r3, [r3]
  472c00: e1a0e00f     	mov	lr, pc
  472c04: e593f048     	ldr	pc, [r3, #0x48]
  472c08: e1a00004     	mov	r0, r4
  472c0c: ebfffd42     	bl	0x47211c <VisualObject::CalcMeshBox()> @ imm = #-0xaf8
  472c10: e1a00004     	mov	r0, r4
  472c14: ebfff78e     	bl	0x470a54 <VisualObject::ApplyMeshBox()> @ imm = #-0x21c8
  472c18: e3a01000     	mov	r1, #0
  472c1c: e3a00008     	mov	r0, #8
  472c20: ebfa7652     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x1626b8
  472c24: e5941008     	ldr	r1, [r4, #0x8]
  472c28: e1a05000     	mov	r5, r0
  472c2c: e3a02000     	mov	r2, #0
  472c30: eb00083e     	bl	0x474d30 <AnimController::AnimController(RootSceneNode*, bool)> @ imm = #0x20f8
  472c34: e1a00004     	mov	r0, r4
  472c38: e1a01005     	mov	r1, r5
  472c3c: ebfff790     	bl	0x470a84 <VisualObject::SetAnimController(AnimController*)> @ imm = #-0x21c0
  472c40: e1a00004     	mov	r0, r4
  472c44: e28dd00c     	add	sp, sp, #12
  472c48: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  472c4c: 74 20 52 00  	.word	0x00522074
  472c50: 94 1e 00 00  	.word	0x00001e94
  472c54: f4 37 00 00  	.word	0x000037f4
  472c58: 20 ab 45 00  	.word	0x0045ab20
