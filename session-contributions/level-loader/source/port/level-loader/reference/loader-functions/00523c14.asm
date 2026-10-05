
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00523c14 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)>:
  523c14: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  523c18: e1a04000     	mov	r4, r0
  523c1c: e24dd02c     	sub	sp, sp, #44
  523c20: e1a07003     	mov	r7, r3
  523c24: e1a06001     	mov	r6, r1
  523c28: e1a08002     	mov	r8, r2
  523c2c: ebfffb04     	bl	0x522844 <PFWorld::_Init()> @ imm = #-0x13f0
  523c30: e5943004     	ldr	r3, [r4, #0x4]
  523c34: e59f0540     	ldr	r0, [pc, #0x540]        @ 0x52417c <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x568>
  523c38: e3530001     	cmp	r3, #1
  523c3c: e08f0000     	add	r0, pc, r0
  523c40: e58d0008     	str	r0, [sp, #0x8]
  523c44: 0a000008     	beq	0x523c6c <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x58> @ imm = #0x20
  523c48: e59f3530     	ldr	r3, [pc, #0x530]        @ 0x524180 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x56c>
  523c4c: e7903003     	ldr	r3, [r0, r3]
  523c50: e5933000     	ldr	r3, [r3]
  523c54: e3530002     	cmp	r3, #2
  523c58: 03a03000     	moveq	r3, #0
  523c5c: 05833000     	streq	r3, [r3]
  523c60: 0a000001     	beq	0x523c6c <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x58> @ imm = #0x4
  523c64: e3530001     	cmp	r3, #1
  523c68: 0a0000e8     	beq	0x524010 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x3fc> @ imm = #0x3a0
  523c6c: e3560000     	cmp	r6, #0
  523c70: 0a0000f4     	beq	0x524048 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x434> @ imm = #0x3d0
  523c74: e3a01000     	mov	r1, #0
  523c78: e3a00054     	mov	r0, #84
  523c7c: ebf7b23b     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x213714
  523c80: e594e044     	ldr	lr, [r4, #0x44]
  523c84: e594c048     	ldr	r12, [r4, #0x48]
  523c88: e1a01007     	mov	r1, r7
  523c8c: e1a03004     	mov	r3, r4
  523c90: e1a02008     	mov	r2, r8
  523c94: e1a05000     	mov	r5, r0
  523c98: e58de000     	str	lr, [sp]
  523c9c: e58dc004     	str	r12, [sp, #0x4]
  523ca0: ebfff626     	bl	0x521540 <PFRoom::PFRoom(char const*, unsigned int, PFWorld*, PFGOuterGraph*, PFGInnerGraph*)> @ imm = #-0x2768
  523ca4: e594700c     	ldr	r7, [r4, #0xc]
  523ca8: e5943010     	ldr	r3, [r4, #0x10]
  523cac: e1570003     	cmp	r7, r3
  523cb0: 0a000106     	beq	0x5240d0 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x4bc> @ imm = #0x418
  523cb4: e5875000     	str	r5, [r7]
  523cb8: e594300c     	ldr	r3, [r4, #0xc]
  523cbc: e2833004     	add	r3, r3, #4
  523cc0: e584300c     	str	r3, [r4, #0xc]
  523cc4: e59fc4b8     	ldr	r12, [pc, #0x4b8]       @ 0x524184 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x570>
  523cc8: e59d0008     	ldr	r0, [sp, #0x8]
  523ccc: e3063164     	movw	r3, #0x6164
  523cd0: e58dc014     	str	r12, [sp, #0x14]
  523cd4: e790200c     	ldr	r2, [r0, r12]
  523cd8: e3a0a000     	mov	r10, #0
  523cdc: e1a01006     	mov	r1, r6
  523ce0: e5920010     	ldr	r0, [r2, #0x10]
  523ce4: e3463d65     	movt	r3, #0x6d65
  523ce8: e28d2018     	add	r2, sp, #24
  523cec: e590001c     	ldr	r0, [r0, #0x1c]
  523cf0: e58da018     	str	r10, [sp, #0x18]
  523cf4: e58da01c     	str	r10, [sp, #0x1c]
  523cf8: e58da020     	str	r10, [sp, #0x20]
  523cfc: ebf8b456     	bl	0x350e5c <SceneManager::SearchByType(glitch::scene::ISceneNode*, std::vector<glitch::scene::ISceneNode*, glitch::core::SAllocator<glitch::scene::ISceneNode*, (glitch::memory::E_MEMORY_HINT)0>>&, glitch::scene::E_SCENE_NODE_TYPE)> @ imm = #-0x1d2ea8
  523d00: e59d6018     	ldr	r6, [sp, #0x18]
  523d04: e59d701c     	ldr	r7, [sp, #0x1c]
  523d08: e1560007     	cmp	r6, r7
  523d0c: 0a0000e4     	beq	0x5240a4 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x490> @ imm = #0x390
  523d10: e59f3470     	ldr	r3, [pc, #0x470]        @ 0x524188 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x574>
  523d14: e79f8003     	ldr	r8, [pc, r3]
  523d18: ea000007     	b	0x523d3c <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x128> @ imm = #0x1c
  523d1c: e1a00009     	mov	r0, r9
  523d20: e1a01008     	mov	r1, r8
  523d24: ebf7abaa     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x215158
  523d28: e3500000     	cmp	r0, #0
  523d2c: 1596a000     	ldrne	r10, [r6]
  523d30: e2866004     	add	r6, r6, #4
  523d34: e1560007     	cmp	r6, r7
  523d38: 0a00001a     	beq	0x523da8 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x194> @ imm = #0x68
  523d3c: e5963000     	ldr	r3, [r6]
  523d40: e1a00003     	mov	r0, r3
  523d44: e5933000     	ldr	r3, [r3]
  523d48: e1a0e00f     	mov	lr, pc
  523d4c: e593f024     	ldr	pc, [r3, #0x24]
  523d50: e5d03000     	ldrb	r3, [r0]
  523d54: e1a09000     	mov	r9, r0
  523d58: e3530000     	cmp	r3, #0
  523d5c: 1affffee     	bne	0x523d1c <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x108> @ imm = #-0x48
  523d60: e5960000     	ldr	r0, [r6]
  523d64: eb01cd49     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0x73524
  523d68: e3500000     	cmp	r0, #0
  523d6c: 0affffea     	beq	0x523d1c <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x108> @ imm = #-0x58
  523d70: e5960000     	ldr	r0, [r6]
  523d74: eb01cd45     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0x73514
  523d78: e5903000     	ldr	r3, [r0]
  523d7c: e1a0e00f     	mov	lr, pc
  523d80: e593f024     	ldr	pc, [r3, #0x24]
  523d84: e1a09000     	mov	r9, r0
  523d88: e1a00009     	mov	r0, r9
  523d8c: e1a01008     	mov	r1, r8
  523d90: ebf7ab8f     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x2151c4
  523d94: e3500000     	cmp	r0, #0
  523d98: 1596a000     	ldrne	r10, [r6]
  523d9c: e2866004     	add	r6, r6, #4
  523da0: e1560007     	cmp	r6, r7
  523da4: 1affffe4     	bne	0x523d3c <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x128> @ imm = #-0x70
  523da8: e59d7018     	ldr	r7, [sp, #0x18]
  523dac: e59db01c     	ldr	r11, [sp, #0x1c]
  523db0: e157000b     	cmp	r7, r11
  523db4: 0a0000ba     	beq	0x5240a4 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x490> @ imm = #0x2e8
  523db8: e59f33cc     	ldr	r3, [pc, #0x3cc]        @ 0x52418c <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x578>
  523dbc: e3a09000     	mov	r9, #0
  523dc0: e1a08004     	mov	r8, r4
  523dc4: e08f3003     	add	r3, pc, r3
  523dc8: e5931008     	ldr	r1, [r3, #0x8]
  523dcc: e5933004     	ldr	r3, [r3, #0x4]
  523dd0: e58d100c     	str	r1, [sp, #0xc]
  523dd4: e58d3010     	str	r3, [sp, #0x10]
  523dd8: ea00001e     	b	0x523e58 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x244> @ imm = #0x78
  523ddc: e59d1010     	ldr	r1, [sp, #0x10]
  523de0: e1a00004     	mov	r0, r4
  523de4: ebf7ab7a     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x215218
  523de8: e3500000     	cmp	r0, #0
  523dec: e1a02004     	mov	r2, r4
  523df0: e1a00005     	mov	r0, r5
  523df4: 0a00000b     	beq	0x523e28 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x214> @ imm = #0x2c
  523df8: e5971000     	ldr	r1, [r7]
  523dfc: ebfff82d     	bl	0x521eb8 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)> @ imm = #-0x1f4c
  523e00: e35a0000     	cmp	r10, #0
  523e04: e2899001     	add	r9, r9, #1
  523e08: 0a000029     	beq	0x523eb4 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x2a0> @ imm = #0xa4
  523e0c: e59dc008     	ldr	r12, [sp, #0x8]
  523e10: e59d2014     	ldr	r2, [sp, #0x14]
  523e14: e1a0100a     	mov	r1, r10
  523e18: e79c3002     	ldr	r3, [r12, r2]
  523e1c: e5933010     	ldr	r3, [r3, #0x10]
  523e20: e593001c     	ldr	r0, [r3, #0x1c]
  523e24: ebf8b99d     	bl	0x3524a0 <SceneManager::AddNodeToMap(glitch::scene::ISceneNode*)> @ imm = #-0x1d198c
  523e28: e59d100c     	ldr	r1, [sp, #0xc]
  523e2c: e1a00004     	mov	r0, r4
  523e30: ebf7ab67     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x215264
  523e34: e3500000     	cmp	r0, #0
  523e38: e2877004     	add	r7, r7, #4
  523e3c: e1a01006     	mov	r1, r6
  523e40: e1a02004     	mov	r2, r4
  523e44: e1a00008     	mov	r0, r8
  523e48: 0a000000     	beq	0x523e50 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x23c> @ imm = #0x0
  523e4c: ebfffee5     	bl	0x5239e8 <PFWorld::_AddExitPosition(glitch::scene::ISceneNode*, char const*)> @ imm = #-0x46c
  523e50: e157000b     	cmp	r7, r11
  523e54: 0a000020     	beq	0x523edc <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x2c8> @ imm = #0x80
  523e58: e5976000     	ldr	r6, [r7]
  523e5c: e5963000     	ldr	r3, [r6]
  523e60: e1a00006     	mov	r0, r6
  523e64: e1a0e00f     	mov	lr, pc
  523e68: e593f024     	ldr	pc, [r3, #0x24]
  523e6c: e5d03000     	ldrb	r3, [r0]
  523e70: e1a04000     	mov	r4, r0
  523e74: e3530000     	cmp	r3, #0
  523e78: 1affffd7     	bne	0x523ddc <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x1c8> @ imm = #-0xa4
  523e7c: e5970000     	ldr	r0, [r7]
  523e80: eb01cd02     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0x73408
  523e84: e3500000     	cmp	r0, #0
  523e88: 0affffd3     	beq	0x523ddc <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x1c8> @ imm = #-0xb4
  523e8c: e5970000     	ldr	r0, [r7]
  523e90: eb01ccfe     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0x733f8
  523e94: e5903000     	ldr	r3, [r0]
  523e98: e1a0e00f     	mov	lr, pc
  523e9c: e593f024     	ldr	pc, [r3, #0x24]
  523ea0: e1a04000     	mov	r4, r0
  523ea4: e1a00006     	mov	r0, r6
  523ea8: eb01ccf8     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0x733e0
  523eac: e1a06000     	mov	r6, r0
  523eb0: eaffffc9     	b	0x523ddc <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x1c8> @ imm = #-0xdc
  523eb4: e59d1008     	ldr	r1, [sp, #0x8]
  523eb8: e59d0014     	ldr	r0, [sp, #0x14]
  523ebc: e5953034     	ldr	r3, [r5, #0x34]
  523ec0: e7912000     	ldr	r2, [r1, r0]
  523ec4: e5133004     	ldr	r3, [r3, #-0x4]
  523ec8: e5922010     	ldr	r2, [r2, #0x10]
  523ecc: e5931040     	ldr	r1, [r3, #0x40]
  523ed0: e592001c     	ldr	r0, [r2, #0x1c]
  523ed4: ebf8b971     	bl	0x3524a0 <SceneManager::AddNodeToMap(glitch::scene::ISceneNode*)> @ imm = #-0x1d1a3c
  523ed8: eaffffd2     	b	0x523e28 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x214> @ imm = #-0xb8
  523edc: e3590000     	cmp	r9, #0
  523ee0: e1a04008     	mov	r4, r8
  523ee4: 0a00006e     	beq	0x5240a4 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x490> @ imm = #0x1b8
  523ee8: e598200c     	ldr	r2, [r8, #0xc]
  523eec: e5983008     	ldr	r3, [r8, #0x8]
  523ef0: e0633002     	rsb	r3, r3, r2
  523ef4: e1a03143     	asr	r3, r3, #2
  523ef8: e3530001     	cmp	r3, #1
  523efc: 0a000036     	beq	0x523fdc <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x3c8> @ imm = #0xd8
  523f00: e595603c     	ldr	r6, [r5, #0x3c]
  523f04: e5987014     	ldr	r7, [r8, #0x14]
  523f08: e1a00006     	mov	r0, r6
  523f0c: e1a01007     	mov	r1, r7
  523f10: ebf7a9fd     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x21580c
  523f14: e3500000     	cmp	r0, #0
  523f18: 01a06007     	moveq	r6, r7
  523f1c: e5886014     	str	r6, [r8, #0x14]
  523f20: e5956040     	ldr	r6, [r5, #0x40]
  523f24: e5987018     	ldr	r7, [r8, #0x18]
  523f28: e1a00006     	mov	r0, r6
  523f2c: e1a01007     	mov	r1, r7
  523f30: ebf7a9f5     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x21582c
  523f34: e3500000     	cmp	r0, #0
  523f38: 01a06007     	moveq	r6, r7
  523f3c: e5886018     	str	r6, [r8, #0x18]
  523f40: e5956044     	ldr	r6, [r5, #0x44]
  523f44: e598701c     	ldr	r7, [r8, #0x1c]
  523f48: e1a00006     	mov	r0, r6
  523f4c: e1a01007     	mov	r1, r7
  523f50: ebf7a9ed     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x21584c
  523f54: e3500000     	cmp	r0, #0
  523f58: 01a06007     	moveq	r6, r7
  523f5c: e588601c     	str	r6, [r8, #0x1c]
  523f60: e5956048     	ldr	r6, [r5, #0x48]
  523f64: e5987020     	ldr	r7, [r8, #0x20]
  523f68: e1a01006     	mov	r1, r6
  523f6c: e1a00007     	mov	r0, r7
  523f70: ebf7a9e5     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x21586c
  523f74: e3500000     	cmp	r0, #0
  523f78: 01a06007     	moveq	r6, r7
  523f7c: e5886020     	str	r6, [r8, #0x20]
  523f80: e595604c     	ldr	r6, [r5, #0x4c]
  523f84: e5987024     	ldr	r7, [r8, #0x24]
  523f88: e1a01006     	mov	r1, r6
  523f8c: e1a00007     	mov	r0, r7
  523f90: ebf7a9dd     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x21588c
  523f94: e3500000     	cmp	r0, #0
  523f98: 01a06007     	moveq	r6, r7
  523f9c: e5886024     	str	r6, [r8, #0x24]
  523fa0: e5987028     	ldr	r7, [r8, #0x28]
  523fa4: e5956050     	ldr	r6, [r5, #0x50]
  523fa8: e1a00007     	mov	r0, r7
  523fac: e1a01006     	mov	r1, r6
  523fb0: ebf7a9d5     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x2158ac
  523fb4: e3500000     	cmp	r0, #0
  523fb8: 01a06007     	moveq	r6, r7
  523fbc: e5886028     	str	r6, [r8, #0x28]
  523fc0: e59d0018     	ldr	r0, [sp, #0x18]
  523fc4: e3500000     	cmp	r0, #0
  523fc8: 0a000000     	beq	0x523fd0 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x3bc> @ imm = #0x0
  523fcc: ebf7b11f     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x213b84
  523fd0: e1a00005     	mov	r0, r5
  523fd4: e28dd02c     	add	sp, sp, #44
  523fd8: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  523fdc: e595303c     	ldr	r3, [r5, #0x3c]
  523fe0: e5883014     	str	r3, [r8, #0x14]
  523fe4: e5953040     	ldr	r3, [r5, #0x40]
  523fe8: e5883018     	str	r3, [r8, #0x18]
  523fec: e5953044     	ldr	r3, [r5, #0x44]
  523ff0: e588301c     	str	r3, [r8, #0x1c]
  523ff4: e5953048     	ldr	r3, [r5, #0x48]
  523ff8: e5883020     	str	r3, [r8, #0x20]
  523ffc: e595304c     	ldr	r3, [r5, #0x4c]
  524000: e5883024     	str	r3, [r8, #0x24]
  524004: e5953050     	ldr	r3, [r5, #0x50]
  524008: e5883028     	str	r3, [r8, #0x28]
  52400c: eaffffeb     	b	0x523fc0 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x3ac> @ imm = #-0x54
  524010: e59d1008     	ldr	r1, [sp, #0x8]
  524014: e59f0174     	ldr	r0, [pc, #0x174]        @ 0x524190 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x57c>
  524018: e59f2174     	ldr	r2, [pc, #0x174]        @ 0x524194 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x580>
  52401c: e59f3174     	ldr	r3, [pc, #0x174]        @ 0x524198 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x584>
  524020: e7910000     	ldr	r0, [r1, r0]
  524024: e59f1170     	ldr	r1, [pc, #0x170]        @ 0x52419c <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x588>
  524028: e3a0c05c     	mov	r12, #92
  52402c: e08f2002     	add	r2, pc, r2
  524030: e08f1001     	add	r1, pc, r1
  524034: e08f3003     	add	r3, pc, r3
  524038: e28000a8     	add	r0, r0, #168
  52403c: e58dc000     	str	r12, [sp]
  524040: ebf7a7ef     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x216044
  524044: eaffff08     	b	0x523c6c <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x58> @ imm = #-0x3e0
  524048: e59d2008     	ldr	r2, [sp, #0x8]
  52404c: e59f312c     	ldr	r3, [pc, #0x12c]        @ 0x524180 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x56c>
  524050: e7923003     	ldr	r3, [r2, r3]
  524054: e5933000     	ldr	r3, [r3]
  524058: e3530002     	cmp	r3, #2
  52405c: 05866000     	streq	r6, [r6]
  524060: 0affff03     	beq	0x523c74 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x60> @ imm = #-0x3f4
  524064: e3530001     	cmp	r3, #1
  524068: 1affff01     	bne	0x523c74 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x60> @ imm = #-0x3fc
  52406c: e59d3008     	ldr	r3, [sp, #0x8]
  524070: e59f0118     	ldr	r0, [pc, #0x118]        @ 0x524190 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x57c>
  524074: e59f1124     	ldr	r1, [pc, #0x124]        @ 0x5241a0 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x58c>
  524078: e59f2124     	ldr	r2, [pc, #0x124]        @ 0x5241a4 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x590>
  52407c: e7930000     	ldr	r0, [r3, r0]
  524080: e59f3120     	ldr	r3, [pc, #0x120]        @ 0x5241a8 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x594>
  524084: e3a0c062     	mov	r12, #98
  524088: e08f1001     	add	r1, pc, r1
  52408c: e08f2002     	add	r2, pc, r2
  524090: e08f3003     	add	r3, pc, r3
  524094: e28000a8     	add	r0, r0, #168
  524098: e58dc000     	str	r12, [sp]
  52409c: ebf7a7d8     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x2160a0
  5240a0: eafffef3     	b	0x523c74 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x60> @ imm = #-0x434
  5240a4: e594300c     	ldr	r3, [r4, #0xc]
  5240a8: e3550000     	cmp	r5, #0
  5240ac: e2433004     	sub	r3, r3, #4
  5240b0: e584300c     	str	r3, [r4, #0xc]
  5240b4: 0affffc1     	beq	0x523fc0 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x3ac> @ imm = #-0xfc
  5240b8: e1a00005     	mov	r0, r5
  5240bc: e5953000     	ldr	r3, [r5]
  5240c0: e1a0e00f     	mov	lr, pc
  5240c4: e593f004     	ldr	pc, [r3, #0x4]
  5240c8: e3a05000     	mov	r5, #0
  5240cc: eaffffbb     	b	0x523fc0 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x3ac> @ imm = #-0x114
  5240d0: e5943008     	ldr	r3, [r4, #0x8]
  5240d4: e0633007     	rsb	r3, r3, r7
  5240d8: e1a03143     	asr	r3, r3, #2
  5240dc: e3530001     	cmp	r3, #1
  5240e0: 20831003     	addhs	r1, r3, r3
  5240e4: 32831001     	addlo	r1, r3, #1
  5240e8: e3710107     	cmn	r1, #-1073741823
  5240ec: 9a000019     	bls	0x524158 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x544> @ imm = #0x64
  5240f0: e3e01103     	mvn	r1, #-1073741824
  5240f4: e28d2028     	add	r2, sp, #40
  5240f8: e5221004     	str	r1, [r2, #-0x4]!
  5240fc: e2840010     	add	r0, r4, #16
  524100: ebfffabf     	bl	0x522c04 <std::allocator<PFRoom*>::_M_allocate(unsigned int, unsigned int&)> @ imm = #-0x1504
  524104: e5941008     	ldr	r1, [r4, #0x8]
  524108: e1a08000     	mov	r8, r0
  52410c: e0577001     	subs	r7, r7, r1
  524110: 01a07000     	moveq	r7, r0
  524114: 1a000014     	bne	0x52416c <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x558> @ imm = #0x50
  524118: e4875004     	str	r5, [r7], #4
  52411c: e5940008     	ldr	r0, [r4, #0x8]
  524120: e5943010     	ldr	r3, [r4, #0x10]
  524124: e3500000     	cmp	r0, #0
  524128: 0a000004     	beq	0x524140 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x52c> @ imm = #0x10
  52412c: e0603003     	rsb	r3, r0, r3
  524130: e3c31003     	bic	r1, r3, #3
  524134: e3510080     	cmp	r1, #128
  524138: 8a000009     	bhi	0x524164 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x550> @ imm = #0x24
  52413c: eb07936f     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x1e4dbc
  524140: e59d3024     	ldr	r3, [sp, #0x24]
  524144: e5848008     	str	r8, [r4, #0x8]
  524148: e584700c     	str	r7, [r4, #0xc]
  52414c: e0888103     	add	r8, r8, r3, lsl #2
  524150: e5848010     	str	r8, [r4, #0x10]
  524154: eafffeda     	b	0x523cc4 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0xb0> @ imm = #-0x498
  524158: e1530001     	cmp	r3, r1
  52415c: 9affffe4     	bls	0x5240f4 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x4e0> @ imm = #-0x70
  524160: eaffffe2     	b	0x5240f0 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x4dc> @ imm = #-0x78
  524164: ebf7b0b5     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x213d2c
  524168: eafffff4     	b	0x524140 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x52c> @ imm = #-0x30
  52416c: e1a02007     	mov	r2, r7
  524170: ebf7a770     	bl	0x30df38 <.plt+0x1c4>   @ imm = #-0x216240
  524174: e0807007     	add	r7, r0, r7
  524178: eaffffe6     	b	0x524118 <PFWorld::LoadRoom(glitch::scene::ISceneNode*, unsigned int, char const*)+0x504> @ imm = #-0x68
  52417c: 54 0e 47 00  	.word	0x00470e54
  524180: c0 39 00 00  	.word	0x000039c0
  524184: f4 37 00 00  	.word	0x000037f4
  524188: 18 2e 43 00  	.word	0x00432e18
  52418c: 68 2d 43 00  	.word	0x00432d68
  524190: c0 19 00 00  	.word	0x000019c0
  524194: 6c 8a 3b 00  	.word	0x003b8a6c
  524198: dc 89 3b 00  	.word	0x003b89dc
  52419c: a8 a3 39 00  	.word	0x0039a3a8
  5241a0: 50 a3 39 00  	.word	0x0039a350
  5241a4: 5c 89 3b 00  	.word	0x003b895c
  5241a8: 80 89 3b 00  	.word	0x003b8980
