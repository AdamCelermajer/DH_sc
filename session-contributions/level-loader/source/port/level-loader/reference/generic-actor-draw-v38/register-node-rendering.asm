
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0058f348 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)>:
  58f348: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  58f34c: e24dd0d4     	sub	sp, sp, #212
  58f350: e59dc0f0     	ldr	r12, [sp, #0xf0]
  58f354: e1a04000     	mov	r4, r0
  58f358: e1a05003     	mov	r5, r3
  58f35c: e59d80f4     	ldr	r8, [sp, #0xf4]
  58f360: e59d60f8     	ldr	r6, [sp, #0xf8]
  58f364: e35c0008     	cmp	r12, #8
  58f368: 908ff10c     	addls	pc, pc, r12, lsl #2
  58f36c: ea000032     	b	0x58f43c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0xf4> @ imm = #0xc8
  58f370: ea00003c     	b	0x58f468 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x120> @ imm = #0xf0
  58f374: ea000057     	b	0x58f4d8 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x190> @ imm = #0x15c
  58f378: ea00006a     	b	0x58f528 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1e0> @ imm = #0x1a8
  58f37c: ea000076     	b	0x58f55c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x214> @ imm = #0x1d8
  58f380: ea0000a2     	b	0x58f610 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x2c8> @ imm = #0x288
  58f384: ea0000e4     	b	0x58f71c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x3d4> @ imm = #0x390
  58f388: ea0000c9     	b	0x58f6b4 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x36c> @ imm = #0x324
  58f38c: ea0000d5     	b	0x58f6e8 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x3a0> @ imm = #0x354
  58f390: eaffffff     	b	0x58f394 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x4c> @ imm = #-0x4
  58f394: e5d0328a     	ldrb	r3, [r0, #0x28a]
  58f398: e3530000     	cmp	r3, #0
  58f39c: 0a0000eb     	beq	0x58f750 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x408> @ imm = #0x3ac
  58f3a0: e5927000     	ldr	r7, [r2]
  58f3a4: e2804078     	add	r4, r0, #120
  58f3a8: e3570000     	cmp	r7, #0
  58f3ac: 058d1050     	streq	r1, [sp, #0x50]
  58f3b0: 058d5054     	streq	r5, [sp, #0x54]
  58f3b4: 058d7058     	streq	r7, [sp, #0x58]
  58f3b8: 0a000008     	beq	0x58f3e0 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x98> @ imm = #0x20
  58f3bc: e5973000     	ldr	r3, [r7]
  58f3c0: e2833001     	add	r3, r3, #1
  58f3c4: e5873000     	str	r3, [r7]
  58f3c8: e58d1050     	str	r1, [sp, #0x50]
  58f3cc: e58d5054     	str	r5, [sp, #0x54]
  58f3d0: e58d7058     	str	r7, [sp, #0x58]
  58f3d4: e5973000     	ldr	r3, [r7]
  58f3d8: e2833001     	add	r3, r3, #1
  58f3dc: e5873000     	str	r3, [r7]
  58f3e0: e3760106     	cmn	r6, #-2147483647
  58f3e4: 158d605c     	strne	r6, [sp, #0x5c]
  58f3e8: 0a00010c     	beq	0x58f820 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x4d8> @ imm = #0x430
  58f3ec: e1a00004     	mov	r0, r4
  58f3f0: e28d1050     	add	r1, sp, #80
  58f3f4: ebf70b17     	bl	0x352058 <std::vector<glitch::scene::CSceneManager::SDefaultNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SDefaultNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::push_back(glitch::scene::CSceneManager::SDefaultNodeEntry const&)> @ imm = #-0x23d3a4
  58f3f8: e59d4058     	ldr	r4, [sp, #0x58]
  58f3fc: e3540000     	cmp	r4, #0
  58f400: 0a000008     	beq	0x58f428 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0xe0> @ imm = #0x20
  58f404: e5943000     	ldr	r3, [r4]
  58f408: e2433001     	sub	r3, r3, #1
  58f40c: e3530000     	cmp	r3, #0
  58f410: e5843000     	str	r3, [r4]
  58f414: 1a000003     	bne	0x58f428 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0xe0> @ imm = #0xc
  58f418: e1a00004     	mov	r0, r4
  58f41c: eb00f2d5     	bl	0x5cbf78 <glitch::video::CMaterial::~CMaterial()> @ imm = #0x3cb54
  58f420: e1a00004     	mov	r0, r4
  58f424: ebf5fba1     	bl	0x30e2b0 <.plt+0x53c>   @ imm = #-0x28117c
  58f428: e3570000     	cmp	r7, #0
  58f42c: 0a000036     	beq	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #0xd8
  58f430: e1a00007     	mov	r0, r7
  58f434: ebffea6b     	bl	0x589de8 <glitch::intrusive_ptr_release(glitch::ISharedMemoryBlockHeader<glitch::video::CMaterial> const*)> @ imm = #-0x5654
  58f438: ea000033     	b	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #0xcc
  58f43c: e59f3540     	ldr	r3, [pc, #0x540]        @ 0x58f984 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x63c>
  58f440: e3a00000     	mov	r0, #0
  58f444: e08f3003     	add	r3, pc, r3
  58f448: e5932000     	ldr	r2, [r3]
  58f44c: e5931004     	ldr	r1, [r3, #0x4]
  58f450: e2822001     	add	r2, r2, #1
  58f454: e2811001     	add	r1, r1, #1
  58f458: e5831004     	str	r1, [r3, #0x4]
  58f45c: e5832000     	str	r2, [r3]
  58f460: e28dd0d4     	add	sp, sp, #212
  58f464: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  58f468: e590c03c     	ldr	r12, [r0, #0x3c]
  58f46c: e5903040     	ldr	r3, [r0, #0x40]
  58f470: e06c6003     	rsb	r6, r12, r3
  58f474: e1b061c6     	asrs	r6, r6, #3
  58f478: 0a00000a     	beq	0x58f4a8 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x160> @ imm = #0x28
  58f47c: e59c2000     	ldr	r2, [r12]
  58f480: e1520001     	cmp	r2, r1
  58f484: 13a02000     	movne	r2, #0
  58f488: 1a000003     	bne	0x58f49c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x154> @ imm = #0xc
  58f48c: eaffffea     	b	0x58f43c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0xf4> @ imm = #-0x58
  58f490: e79c0182     	ldr	r0, [r12, r2, lsl #3]
  58f494: e1500001     	cmp	r0, r1
  58f498: 0affffe7     	beq	0x58f43c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0xf4> @ imm = #-0x64
  58f49c: e2822001     	add	r2, r2, #1
  58f4a0: e1520006     	cmp	r2, r6
  58f4a4: 1afffff9     	bne	0x58f490 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x148> @ imm = #-0x1c
  58f4a8: e5942044     	ldr	r2, [r4, #0x44]
  58f4ac: e58d50a8     	str	r5, [sp, #0xa8]
  58f4b0: e58d10a4     	str	r1, [sp, #0xa4]
  58f4b4: e1530002     	cmp	r3, r2
  58f4b8: 0a0000df     	beq	0x58f83c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x4f4> @ imm = #0x37c
  58f4bc: e5831000     	str	r1, [r3]
  58f4c0: e59d20a8     	ldr	r2, [sp, #0xa8]
  58f4c4: e5832004     	str	r2, [r3, #0x4]
  58f4c8: e5943040     	ldr	r3, [r4, #0x40]
  58f4cc: e2833008     	add	r3, r3, #8
  58f4d0: e5843040     	str	r3, [r4, #0x40]
  58f4d4: ea00000c     	b	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #0x30
  58f4d8: e28d6070     	add	r6, sp, #112
  58f4dc: e1a00006     	mov	r0, r6
  58f4e0: e28420e8     	add	r2, r4, #232
  58f4e4: ebf70602     	bl	0x350cf4 <glitch::scene::CSceneManager::SDistanceNodeEntry::SDistanceNodeEntry(glitch::scene::ISceneNode*, glitch::core::vector3d<float> const&, void*)> @ imm = #-0x23e7f8
  58f4e8: e594c04c     	ldr	r12, [r4, #0x4c]
  58f4ec: e5943050     	ldr	r3, [r4, #0x50]
  58f4f0: e15c0003     	cmp	r12, r3
  58f4f4: 0a000119     	beq	0x58f960 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x618> @ imm = #0x464
  58f4f8: e896000f     	ldm	r6, {r0, r1, r2, r3}
  58f4fc: e88c000f     	stm	r12, {r0, r1, r2, r3}
  58f500: e594304c     	ldr	r3, [r4, #0x4c]
  58f504: e2833010     	add	r3, r3, #16
  58f508: e584304c     	str	r3, [r4, #0x4c]
  58f50c: e59f3474     	ldr	r3, [pc, #0x474]        @ 0x58f988 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x640>
  58f510: e3a00001     	mov	r0, #1
  58f514: e08f3003     	add	r3, pc, r3
  58f518: e5932000     	ldr	r2, [r3]
  58f51c: e0822000     	add	r2, r2, r0
  58f520: e5832000     	str	r2, [r3]
  58f524: eaffffcd     	b	0x58f460 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x118> @ imm = #-0xcc
  58f528: e5903070     	ldr	r3, [r0, #0x70]
  58f52c: e5902074     	ldr	r2, [r0, #0x74]
  58f530: e58d50a0     	str	r5, [sp, #0xa0]
  58f534: e58d109c     	str	r1, [sp, #0x9c]
  58f538: e1530002     	cmp	r3, r2
  58f53c: 0a0000f6     	beq	0x58f91c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x5d4> @ imm = #0x3d8
  58f540: e5831000     	str	r1, [r3]
  58f544: e59d20a0     	ldr	r2, [sp, #0xa0]
  58f548: e5832004     	str	r2, [r3, #0x4]
  58f54c: e5903070     	ldr	r3, [r0, #0x70]
  58f550: e2833008     	add	r3, r3, #8
  58f554: e5803070     	str	r3, [r0, #0x70]
  58f558: eaffffeb     	b	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #-0x54
  58f55c: e5927000     	ldr	r7, [r2]
  58f560: e3570000     	cmp	r7, #0
  58f564: 0a0000c4     	beq	0x58f87c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x534> @ imm = #0x310
  58f568: e1a00007     	mov	r0, r7
  58f56c: e58d1014     	str	r1, [sp, #0x14]
  58f570: e58d2010     	str	r2, [sp, #0x10]
  58f574: eb00d9ee     	bl	0x5c5d34 <glitch::video::CMaterial::getTechnique() const> @ imm = #0x367b8
  58f578: e5973004     	ldr	r3, [r7, #0x4]
  58f57c: e3a0c00c     	mov	r12, #12
  58f580: e59d1014     	ldr	r1, [sp, #0x14]
  58f584: e5933018     	ldr	r3, [r3, #0x18]
  58f588: e59d2010     	ldr	r2, [sp, #0x10]
  58f58c: e023309c     	mla	r3, r12, r0, r3
  58f590: e5933008     	ldr	r3, [r3, #0x8]
  58f594: e5933004     	ldr	r3, [r3, #0x4]
  58f598: e3130801     	tst	r3, #65536
  58f59c: 1a000085     	bne	0x58f7b8 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x470> @ imm = #0x214
  58f5a0: e5927000     	ldr	r7, [r2]
  58f5a4: e2844078     	add	r4, r4, #120
  58f5a8: e3570000     	cmp	r7, #0
  58f5ac: 0a0000b3     	beq	0x58f880 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x538> @ imm = #0x2cc
  58f5b0: e5973000     	ldr	r3, [r7]
  58f5b4: e2833001     	add	r3, r3, #1
  58f5b8: e5873000     	str	r3, [r7]
  58f5bc: e58d1040     	str	r1, [sp, #0x40]
  58f5c0: e58d5044     	str	r5, [sp, #0x44]
  58f5c4: e58d7048     	str	r7, [sp, #0x48]
  58f5c8: e5973000     	ldr	r3, [r7]
  58f5cc: e2833001     	add	r3, r3, #1
  58f5d0: e5873000     	str	r3, [r7]
  58f5d4: e3760106     	cmn	r6, #-2147483647
  58f5d8: 158d604c     	strne	r6, [sp, #0x4c]
  58f5dc: 0a0000ac     	beq	0x58f894 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x54c> @ imm = #0x2b0
  58f5e0: e1a00004     	mov	r0, r4
  58f5e4: e28d1040     	add	r1, sp, #64
  58f5e8: ebf70a9a     	bl	0x352058 <std::vector<glitch::scene::CSceneManager::SDefaultNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SDefaultNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::push_back(glitch::scene::CSceneManager::SDefaultNodeEntry const&)> @ imm = #-0x23d598
  58f5ec: e59d0048     	ldr	r0, [sp, #0x48]
  58f5f0: e3500000     	cmp	r0, #0
  58f5f4: 0a000000     	beq	0x58f5fc <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x2b4> @ imm = #0x0
  58f5f8: ebffe9fa     	bl	0x589de8 <glitch::intrusive_ptr_release(glitch::ISharedMemoryBlockHeader<glitch::video::CMaterial> const*)> @ imm = #-0x5818
  58f5fc: e3570000     	cmp	r7, #0
  58f600: 0affffc1     	beq	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #-0xfc
  58f604: e1a00007     	mov	r0, r7
  58f608: ebffe9f6     	bl	0x589de8 <glitch::intrusive_ptr_release(glitch::ISharedMemoryBlockHeader<glitch::video::CMaterial> const*)> @ imm = #-0x5828
  58f60c: eaffffbe     	b	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #-0x108
  58f610: e5927000     	ldr	r7, [r2]
  58f614: e3570000     	cmp	r7, #0
  58f618: 058d1060     	streq	r1, [sp, #0x60]
  58f61c: 058d3064     	streq	r3, [sp, #0x64]
  58f620: 058d7068     	streq	r7, [sp, #0x68]
  58f624: 0a000008     	beq	0x58f64c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x304> @ imm = #0x20
  58f628: e5973000     	ldr	r3, [r7]
  58f62c: e2833001     	add	r3, r3, #1
  58f630: e5873000     	str	r3, [r7]
  58f634: e58d1060     	str	r1, [sp, #0x60]
  58f638: e58d5064     	str	r5, [sp, #0x64]
  58f63c: e58d7068     	str	r7, [sp, #0x68]
  58f640: e5973000     	ldr	r3, [r7]
  58f644: e2833001     	add	r3, r3, #1
  58f648: e5873000     	str	r3, [r7]
  58f64c: e3760106     	cmn	r6, #-2147483647
  58f650: 158d606c     	strne	r6, [sp, #0x6c]
  58f654: 0a000081     	beq	0x58f860 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x518> @ imm = #0x204
  58f658: e594107c     	ldr	r1, [r4, #0x7c]
  58f65c: e5943080     	ldr	r3, [r4, #0x80]
  58f660: e1510003     	cmp	r1, r3
  58f664: 0a0000b5     	beq	0x58f940 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x5f8> @ imm = #0x2d4
  58f668: e59d3060     	ldr	r3, [sp, #0x60]
  58f66c: e5813000     	str	r3, [r1]
  58f670: e59d3064     	ldr	r3, [sp, #0x64]
  58f674: e5813004     	str	r3, [r1, #0x4]
  58f678: e59d3068     	ldr	r3, [sp, #0x68]
  58f67c: e5813008     	str	r3, [r1, #0x8]
  58f680: e3530000     	cmp	r3, #0
  58f684: 15932000     	ldrne	r2, [r3]
  58f688: 12822001     	addne	r2, r2, #1
  58f68c: 15832000     	strne	r2, [r3]
  58f690: e59d306c     	ldr	r3, [sp, #0x6c]
  58f694: e581300c     	str	r3, [r1, #0xc]
  58f698: e594307c     	ldr	r3, [r4, #0x7c]
  58f69c: e2833010     	add	r3, r3, #16
  58f6a0: e584307c     	str	r3, [r4, #0x7c]
  58f6a4: e59d0068     	ldr	r0, [sp, #0x68]
  58f6a8: e3500000     	cmp	r0, #0
  58f6ac: 1affffd1     	bne	0x58f5f8 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x2b0> @ imm = #-0xbc
  58f6b0: eaffffd1     	b	0x58f5fc <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x2b4> @ imm = #-0xbc
  58f6b4: e5903064     	ldr	r3, [r0, #0x64]
  58f6b8: e5902068     	ldr	r2, [r0, #0x68]
  58f6bc: e58d5090     	str	r5, [sp, #0x90]
  58f6c0: e58d108c     	str	r1, [sp, #0x8c]
  58f6c4: e1530002     	cmp	r3, r2
  58f6c8: 0a000081     	beq	0x58f8d4 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x58c> @ imm = #0x204
  58f6cc: e5831000     	str	r1, [r3]
  58f6d0: e59d2090     	ldr	r2, [sp, #0x90]
  58f6d4: e5832004     	str	r2, [r3, #0x4]
  58f6d8: e5903064     	ldr	r3, [r0, #0x64]
  58f6dc: e2833008     	add	r3, r3, #8
  58f6e0: e5803064     	str	r3, [r0, #0x64]
  58f6e4: eaffff88     	b	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #-0x1e0
  58f6e8: e5903034     	ldr	r3, [r0, #0x34]
  58f6ec: e5902038     	ldr	r2, [r0, #0x38]
  58f6f0: e58d5088     	str	r5, [sp, #0x88]
  58f6f4: e58d1084     	str	r1, [sp, #0x84]
  58f6f8: e1530002     	cmp	r3, r2
  58f6fc: 0a00006b     	beq	0x58f8b0 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x568> @ imm = #0x1ac
  58f700: e5831000     	str	r1, [r3]
  58f704: e59d2088     	ldr	r2, [sp, #0x88]
  58f708: e5832004     	str	r2, [r3, #0x4]
  58f70c: e5903034     	ldr	r3, [r0, #0x34]
  58f710: e2833008     	add	r3, r3, #8
  58f714: e5803034     	str	r3, [r0, #0x34]
  58f718: eaffff7b     	b	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #-0x214
  58f71c: e5903058     	ldr	r3, [r0, #0x58]
  58f720: e590205c     	ldr	r2, [r0, #0x5c]
  58f724: e58d5098     	str	r5, [sp, #0x98]
  58f728: e58d1094     	str	r1, [sp, #0x94]
  58f72c: e1530002     	cmp	r3, r2
  58f730: 0a000070     	beq	0x58f8f8 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x5b0> @ imm = #0x1c0
  58f734: e5831000     	str	r1, [r3]
  58f738: e59d2098     	ldr	r2, [sp, #0x98]
  58f73c: e5832004     	str	r2, [r3, #0x4]
  58f740: e5903058     	ldr	r3, [r0, #0x58]
  58f744: e2833008     	add	r3, r3, #8
  58f748: e5803058     	str	r3, [r0, #0x58]
  58f74c: eaffff6e     	b	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #-0x248
  58f750: e5923000     	ldr	r3, [r2]
  58f754: e2807084     	add	r7, r0, #132
  58f758: e28020e8     	add	r2, r0, #232
  58f75c: e3530000     	cmp	r3, #0
  58f760: e58d30b0     	str	r3, [sp, #0xb0]
  58f764: 15930000     	ldrne	r0, [r3]
  58f768: e28d402c     	add	r4, sp, #44
  58f76c: 12800001     	addne	r0, r0, #1
  58f770: 15830000     	strne	r0, [r3]
  58f774: e28d30b0     	add	r3, sp, #176
  58f778: e1a00004     	mov	r0, r4
  58f77c: e88d0120     	stm	sp, {r5, r8}
  58f780: e58d6008     	str	r6, [sp, #0x8]
  58f784: ebf715c0     	bl	0x354e8c <glitch::scene::CSceneManager::STransparentNodeEntry::STransparentNodeEntry(glitch::scene::ISceneNode*, glitch::core::vector3d<float> const&, boost::intrusive_ptr<glitch::video::CMaterial>, void*, glitch::core::vector3d<float> const*, int)> @ imm = #-0x23a900
  58f788: e1a00007     	mov	r0, r7
  58f78c: e1a01004     	mov	r1, r4
  58f790: ebf71cf4     	bl	0x356b68 <std::vector<glitch::scene::CSceneManager::STransparentNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::STransparentNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::push_back(glitch::scene::CSceneManager::STransparentNodeEntry const&)> @ imm = #-0x238c30
  58f794: e59d0034     	ldr	r0, [sp, #0x34]
  58f798: e3500000     	cmp	r0, #0
  58f79c: 0a000000     	beq	0x58f7a4 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x45c> @ imm = #0x0
  58f7a0: ebffe990     	bl	0x589de8 <glitch::intrusive_ptr_release(glitch::ISharedMemoryBlockHeader<glitch::video::CMaterial> const*)> @ imm = #-0x59c0
  58f7a4: e59d00b0     	ldr	r0, [sp, #0xb0]
  58f7a8: e3500000     	cmp	r0, #0
  58f7ac: 0affff56     	beq	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #-0x2a8
  58f7b0: ebffe98c     	bl	0x589de8 <glitch::intrusive_ptr_release(glitch::ISharedMemoryBlockHeader<glitch::video::CMaterial> const*)> @ imm = #-0x59d0
  58f7b4: eaffff54     	b	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #-0x2b0
  58f7b8: e5d4328a     	ldrb	r3, [r4, #0x28a]
  58f7bc: e3530000     	cmp	r3, #0
  58f7c0: 1affff76     	bne	0x58f5a0 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x258> @ imm = #-0x228
  58f7c4: e5923000     	ldr	r3, [r2]
  58f7c8: e28d7018     	add	r7, sp, #24
  58f7cc: e28420e8     	add	r2, r4, #232
  58f7d0: e3530000     	cmp	r3, #0
  58f7d4: e58d30ac     	str	r3, [sp, #0xac]
  58f7d8: 15930000     	ldrne	r0, [r3]
  58f7dc: e284a084     	add	r10, r4, #132
  58f7e0: e28d40ac     	add	r4, sp, #172
  58f7e4: 12800001     	addne	r0, r0, #1
  58f7e8: 15830000     	strne	r0, [r3]
  58f7ec: e1a03004     	mov	r3, r4
  58f7f0: e1a00007     	mov	r0, r7
  58f7f4: e88d0120     	stm	sp, {r5, r8}
  58f7f8: e58d6008     	str	r6, [sp, #0x8]
  58f7fc: ebf715a2     	bl	0x354e8c <glitch::scene::CSceneManager::STransparentNodeEntry::STransparentNodeEntry(glitch::scene::ISceneNode*, glitch::core::vector3d<float> const&, boost::intrusive_ptr<glitch::video::CMaterial>, void*, glitch::core::vector3d<float> const*, int)> @ imm = #-0x23a978
  58f800: e1a0000a     	mov	r0, r10
  58f804: e1a01007     	mov	r1, r7
  58f808: ebf71cd6     	bl	0x356b68 <std::vector<glitch::scene::CSceneManager::STransparentNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::STransparentNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::push_back(glitch::scene::CSceneManager::STransparentNodeEntry const&)> @ imm = #-0x238ca8
  58f80c: e1a00007     	mov	r0, r7
  58f810: ebfff7be     	bl	0x58d710 <glitch::scene::CSceneManager::STransparentNodeEntry::~STransparentNodeEntry()> @ imm = #-0x2108
  58f814: e1a00004     	mov	r0, r4
  58f818: ebf604f2     	bl	0x310be8 <boost::intrusive_ptr<glitch::video::CMaterial>::~intrusive_ptr()> @ imm = #-0x27ec38
  58f81c: eaffff3a     	b	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #-0x318
  58f820: e59d3050     	ldr	r3, [sp, #0x50]
  58f824: e1a00003     	mov	r0, r3
  58f828: e5933000     	ldr	r3, [r3]
  58f82c: e1a0e00f     	mov	lr, pc
  58f830: e593f0d8     	ldr	pc, [r3, #0xd8]
  58f834: e58d005c     	str	r0, [sp, #0x5c]
  58f838: eafffeeb     	b	0x58f3ec <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0xa4> @ imm = #-0x454
  58f83c: e3a0c001     	mov	r12, #1
  58f840: e1a01003     	mov	r1, r3
  58f844: e284003c     	add	r0, r4, #60
  58f848: e28d20a4     	add	r2, sp, #164
  58f84c: e28d30c8     	add	r3, sp, #200
  58f850: e58dc004     	str	r12, [sp, #0x4]
  58f854: e58dc000     	str	r12, [sp]
  58f858: ebf708a7     	bl	0x351afc <std::vector<glitch::scene::CSceneManager::SUnsortedNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SUnsortedNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::_M_insert_overflow_aux(glitch::scene::CSceneManager::SUnsortedNodeEntry*, glitch::scene::CSceneManager::SUnsortedNodeEntry const&, std::__false_type const&, unsigned int, bool)> @ imm = #-0x23dd64
  58f85c: eaffff2a     	b	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #-0x358
  58f860: e59d3060     	ldr	r3, [sp, #0x60]
  58f864: e1a00003     	mov	r0, r3
  58f868: e5933000     	ldr	r3, [r3]
  58f86c: e1a0e00f     	mov	lr, pc
  58f870: e593f0d8     	ldr	pc, [r3, #0xd8]
  58f874: e58d006c     	str	r0, [sp, #0x6c]
  58f878: eaffff76     	b	0x58f658 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x310> @ imm = #-0x228
  58f87c: e2804078     	add	r4, r0, #120
  58f880: e3a03000     	mov	r3, #0
  58f884: e58d1040     	str	r1, [sp, #0x40]
  58f888: e58d5044     	str	r5, [sp, #0x44]
  58f88c: e58d3048     	str	r3, [sp, #0x48]
  58f890: eaffff4f     	b	0x58f5d4 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x28c> @ imm = #-0x2c4
  58f894: e59d3040     	ldr	r3, [sp, #0x40]
  58f898: e1a00003     	mov	r0, r3
  58f89c: e5933000     	ldr	r3, [r3]
  58f8a0: e1a0e00f     	mov	lr, pc
  58f8a4: e593f0d8     	ldr	pc, [r3, #0xd8]
  58f8a8: e58d004c     	str	r0, [sp, #0x4c]
  58f8ac: eaffff4b     	b	0x58f5e0 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x298> @ imm = #-0x2d4
  58f8b0: e3a0c001     	mov	r12, #1
  58f8b4: e1a01003     	mov	r1, r3
  58f8b8: e2800030     	add	r0, r0, #48
  58f8bc: e28d2084     	add	r2, sp, #132
  58f8c0: e28d30b4     	add	r3, sp, #180
  58f8c4: e58dc004     	str	r12, [sp, #0x4]
  58f8c8: e58dc000     	str	r12, [sp]
  58f8cc: ebf7088a     	bl	0x351afc <std::vector<glitch::scene::CSceneManager::SUnsortedNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SUnsortedNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::_M_insert_overflow_aux(glitch::scene::CSceneManager::SUnsortedNodeEntry*, glitch::scene::CSceneManager::SUnsortedNodeEntry const&, std::__false_type const&, unsigned int, bool)> @ imm = #-0x23ddd8
  58f8d0: eaffff0d     	b	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #-0x3cc
  58f8d4: e3a0c001     	mov	r12, #1
  58f8d8: e1a01003     	mov	r1, r3
  58f8dc: e2800060     	add	r0, r0, #96
  58f8e0: e28d208c     	add	r2, sp, #140
  58f8e4: e28d30b8     	add	r3, sp, #184
  58f8e8: e58dc004     	str	r12, [sp, #0x4]
  58f8ec: e58dc000     	str	r12, [sp]
  58f8f0: ebf708e9     	bl	0x351c9c <std::vector<glitch::scene::CSceneManager::SRenderDataSortNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SRenderDataSortNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::_M_insert_overflow_aux(glitch::scene::CSceneManager::SRenderDataSortNodeEntry*, glitch::scene::CSceneManager::SRenderDataSortNodeEntry const&, std::__false_type const&, unsigned int, bool)> @ imm = #-0x23dc5c
  58f8f4: eaffff04     	b	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #-0x3f0
  58f8f8: e3a0c001     	mov	r12, #1
  58f8fc: e1a01003     	mov	r1, r3
  58f900: e2800054     	add	r0, r0, #84
  58f904: e28d2094     	add	r2, sp, #148
  58f908: e28d30bc     	add	r3, sp, #188
  58f90c: e58dc004     	str	r12, [sp, #0x4]
  58f910: e58dc000     	str	r12, [sp]
  58f914: ebf708e0     	bl	0x351c9c <std::vector<glitch::scene::CSceneManager::SRenderDataSortNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SRenderDataSortNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::_M_insert_overflow_aux(glitch::scene::CSceneManager::SRenderDataSortNodeEntry*, glitch::scene::CSceneManager::SRenderDataSortNodeEntry const&, std::__false_type const&, unsigned int, bool)> @ imm = #-0x23dc80
  58f918: eafffefb     	b	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #-0x414
  58f91c: e3a0c001     	mov	r12, #1
  58f920: e1a01003     	mov	r1, r3
  58f924: e280006c     	add	r0, r0, #108
  58f928: e28d209c     	add	r2, sp, #156
  58f92c: e28d30c4     	add	r3, sp, #196
  58f930: e58dc004     	str	r12, [sp, #0x4]
  58f934: e58dc000     	str	r12, [sp]
  58f938: ebf7086f     	bl	0x351afc <std::vector<glitch::scene::CSceneManager::SUnsortedNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SUnsortedNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::_M_insert_overflow_aux(glitch::scene::CSceneManager::SUnsortedNodeEntry*, glitch::scene::CSceneManager::SUnsortedNodeEntry const&, std::__false_type const&, unsigned int, bool)> @ imm = #-0x23de44
  58f93c: eafffef2     	b	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #-0x438
  58f940: e3a0c001     	mov	r12, #1
  58f944: e2840078     	add	r0, r4, #120
  58f948: e28d2060     	add	r2, sp, #96
  58f94c: e28d30c0     	add	r3, sp, #192
  58f950: e58dc004     	str	r12, [sp, #0x4]
  58f954: e58dc000     	str	r12, [sp]
  58f958: ebf70949     	bl	0x351e84 <std::vector<glitch::scene::CSceneManager::SDefaultNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SDefaultNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::_M_insert_overflow_aux(glitch::scene::CSceneManager::SDefaultNodeEntry*, glitch::scene::CSceneManager::SDefaultNodeEntry const&, std::__false_type const&, unsigned int, bool)> @ imm = #-0x23dadc
  58f95c: eaffff50     	b	0x58f6a4 <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x35c> @ imm = #-0x2c0
  58f960: e1a0100c     	mov	r1, r12
  58f964: e2840048     	add	r0, r4, #72
  58f968: e3a0c001     	mov	r12, #1
  58f96c: e1a02006     	mov	r2, r6
  58f970: e28d30cc     	add	r3, sp, #204
  58f974: e58dc004     	str	r12, [sp, #0x4]
  58f978: e58dc000     	str	r12, [sp]
  58f97c: ebf707e7     	bl	0x351920 <std::vector<glitch::scene::CSceneManager::SDistanceNodeEntry, glitch::core::SAllocator<glitch::scene::CSceneManager::SDistanceNodeEntry, (glitch::memory::E_MEMORY_HINT)0>>::_M_insert_overflow_aux(glitch::scene::CSceneManager::SDistanceNodeEntry*, glitch::scene::CSceneManager::SDistanceNodeEntry const&, std::__false_type const&, unsigned int, bool)> @ imm = #-0x23e064
  58f980: eafffee1     	b	0x58f50c <glitch::scene::CSceneManager::registerNodeForRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<glitch::video::CMaterial> const&, void*, glitch::scene::E_SCENE_NODE_RENDER_PASS, glitch::core::vector3d<float> const*, int)+0x1c4> @ imm = #-0x47c
  58f984: 2c 74 46 00  	.word	0x0046742c
  58f988: 5c 73 46 00  	.word	0x0046735c
