
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f92c4 <Level::~Level()>:
  3f92c4: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3f92c8: e59f51e0     	ldr	r5, [pc, #0x1e0]        @ 0x3f94b0 <Level::~Level()+0x1ec>
  3f92cc: e59f31e0     	ldr	r3, [pc, #0x1e0]        @ 0x3f94b4 <Level::~Level()+0x1f0>
  3f92d0: e1a04000     	mov	r4, r0
  3f92d4: e08f5005     	add	r5, pc, r5
  3f92d8: e7953003     	ldr	r3, [r5, r3]
  3f92dc: e2833008     	add	r3, r3, #8
  3f92e0: e5803000     	str	r3, [r0]
  3f92e4: ebffd844     	bl	0x3ef3fc <Level::CleanUpAllSkills()> @ imm = #-0x9ef0
  3f92e8: e1a00004     	mov	r0, r4
  3f92ec: ebffd85b     	bl	0x3ef460 <Level::UnlockAllObjects()> @ imm = #-0x9e94
  3f92f0: ebff5023     	bl	0x3cd384 <CharAI::ClearAllAggro()> @ imm = #-0x2bf74
  3f92f4: ebff672d     	bl	0x3d2fb0 <CharAI::ClearGroupInfo()> @ imm = #-0x2634c
  3f92f8: e59f31b8     	ldr	r3, [pc, #0x1b8]        @ 0x3f94b8 <Level::~Level()+0x1f4>
  3f92fc: e7956003     	ldr	r6, [r5, r3]
  3f9300: e5963010     	ldr	r3, [r6, #0x10]
  3f9304: e3530000     	cmp	r3, #0
  3f9308: 1a000061     	bne	0x3f9494 <Level::~Level()+0x1d0> @ imm = #0x184
  3f930c: ebfeb9a2     	bl	0x3a799c <Character::ClearConcurrentAI()> @ imm = #-0x51978
  3f9310: ebffff40     	bl	0x3f9018 <MenuMessageManager<StatusMsg, 4>::FlushEnqueuedMessages(int) (.clone.25)> @ imm = #-0x300
  3f9314: ebffe3d6     	bl	0x3f2274 <MenuMessageManager<TutorialMsg, 1>::FlushEnqueuedMessages(int) (.clone.26)> @ imm = #-0x70a8
  3f9318: ebffff51     	bl	0x3f9064 <MenuMessageManager<DialogMsg, 1>::FlushEnqueuedMessages(int) (.clone.27)> @ imm = #-0x2bc
  3f931c: ebffff50     	bl	0x3f9064 <MenuMessageManager<DialogMsg, 1>::FlushEnqueuedMessages(int) (.clone.27)> @ imm = #-0x2c0
  3f9320: ebffe1f3     	bl	0x3f1af4 <MenuMessageManager<OnlineStatusMsg, 1>::FlushEnqueuedMessages(int) (.clone.16)> @ imm = #-0x7834
  3f9324: e59f3190     	ldr	r3, [pc, #0x190]        @ 0x3f94bc <Level::~Level()+0x1f8>
  3f9328: e7950003     	ldr	r0, [r5, r3]
  3f932c: eb0183de     	bl	0x45a2ac <ScriptManager::Flush()> @ imm = #0x60f78
  3f9330: eb0096a6     	bl	0x41edd0 <InfoHUDManager::GetInstance()> @ imm = #0x25a98
  3f9334: eb00970f     	bl	0x41ef78 <InfoHUDManager::Flush()> @ imm = #0x25c3c
  3f9338: eb008777     	bl	0x41b11c <HUDControls::GetInstance()> @ imm = #0x21ddc
  3f933c: eb007bbf     	bl	0x418240 <HUDControls::Flush()> @ imm = #0x1eefc
  3f9340: e59f3178     	ldr	r3, [pc, #0x178]        @ 0x3f94c0 <Level::~Level()+0x1fc>
  3f9344: e7950003     	ldr	r0, [r5, r3]
  3f9348: ebffc8db     	bl	0x3eb6bc <ItemManager::Flush()> @ imm = #-0xdc94
  3f934c: e59f3170     	ldr	r3, [pc, #0x170]        @ 0x3f94c4 <Level::~Level()+0x200>
  3f9350: e7950003     	ldr	r0, [r5, r3]
  3f9354: ebffb444     	bl	0x3e646c <ProjectileManager::Flush()> @ imm = #-0x12ef0
  3f9358: e59f3168     	ldr	r3, [pc, #0x168]        @ 0x3f94c8 <Level::~Level()+0x204>
  3f935c: e7950003     	ldr	r0, [r5, r3]
  3f9360: eb026f4e     	bl	0x4950a0 <VisualFXManager::FlushLibraries()> @ imm = #0x9bd38
  3f9364: e5946194     	ldr	r6, [r4, #0x194]
  3f9368: e3560000     	cmp	r6, #0
  3f936c: 0a000005     	beq	0x3f9388 <Level::~Level()+0xc4> @ imm = #0x14
  3f9370: e1a00006     	mov	r0, r6
  3f9374: eb02021c     	bl	0x479bec <GameEventManager::~GameEventManager()> @ imm = #0x80870
  3f9378: e1a00006     	mov	r0, r6
  3f937c: ebfc5c2f     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xe8f44
  3f9380: e3a03000     	mov	r3, #0
  3f9384: e5843194     	str	r3, [r4, #0x194]
  3f9388: e5946158     	ldr	r6, [r4, #0x158]
  3f938c: e3560000     	cmp	r6, #0
  3f9390: 0a000005     	beq	0x3f93ac <Level::~Level()+0xe8> @ imm = #0x14
  3f9394: e1a00006     	mov	r0, r6
  3f9398: ebffe1aa     	bl	0x3f1a48 <batch::BatchNodeCompiler::~BatchNodeCompiler()> @ imm = #-0x7958
  3f939c: e1a00006     	mov	r0, r6
  3f93a0: ebfc5c26     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xe8f68
  3f93a4: e3a03000     	mov	r3, #0
  3f93a8: e5843158     	str	r3, [r4, #0x158]
  3f93ac: e594615c     	ldr	r6, [r4, #0x15c]
  3f93b0: e3560000     	cmp	r6, #0
  3f93b4: 0a000005     	beq	0x3f93d0 <Level::~Level()+0x10c> @ imm = #0x14
  3f93b8: e1a00006     	mov	r0, r6
  3f93bc: ebffe1a1     	bl	0x3f1a48 <batch::BatchNodeCompiler::~BatchNodeCompiler()> @ imm = #-0x797c
  3f93c0: e1a00006     	mov	r0, r6
  3f93c4: ebfc5c1d     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xe8f8c
  3f93c8: e3a03000     	mov	r3, #0
  3f93cc: e584315c     	str	r3, [r4, #0x15c]
  3f93d0: e59f70f4     	ldr	r7, [pc, #0xf4]         @ 0x3f94cc <Level::~Level()+0x208>
  3f93d4: e7956007     	ldr	r6, [r5, r7]
  3f93d8: e5960038     	ldr	r0, [r6, #0x38]
  3f93dc: ebfd40b5     	bl	0x3496b8 <ObjectManager::Flush()> @ imm = #-0xafd2c
  3f93e0: e3a01000     	mov	r1, #0
  3f93e4: e5960050     	ldr	r0, [r6, #0x50]
  3f93e8: ebfe22f0     	bl	0x381fb0 <ZoomHandler::setCamera(CameraLevel*)> @ imm = #-0x77440
  3f93ec: e5963010     	ldr	r3, [r6, #0x10]
  3f93f0: e593301c     	ldr	r3, [r3, #0x1c]
  3f93f4: e1a00003     	mov	r0, r3
  3f93f8: e5933000     	ldr	r3, [r3]
  3f93fc: e1a0e00f     	mov	lr, pc
  3f9400: e593f068     	ldr	pc, [r3, #0x68]
  3f9404: e5963010     	ldr	r3, [r6, #0x10]
  3f9408: e3a01000     	mov	r1, #0
  3f940c: e593001c     	ldr	r0, [r3, #0x1c]
  3f9410: eb063f2a     	bl	0x5890c0 <glitch::scene::CSceneManager::setActiveCamera(glitch::scene::ICameraSceneNode*)> @ imm = #0x18fca8
  3f9414: e5960044     	ldr	r0, [r6, #0x44]
  3f9418: ebfd4a7b     	bl	0x34be0c <PhysicalWorld::clear()> @ imm = #-0xad614
  3f941c: e59f30ac     	ldr	r3, [pc, #0xac]         @ 0x3f94d0 <Level::~Level()+0x20c>
  3f9420: e7950003     	ldr	r0, [r5, r3]
  3f9424: eb04a8a9     	bl	0x5236d0 <PFWorld::Flush()> @ imm = #0x12a2a4
  3f9428: e59f30a4     	ldr	r3, [pc, #0xa4]         @ 0x3f94d4 <Level::~Level()+0x210>
  3f942c: e7950003     	ldr	r0, [r5, r3]
  3f9430: eb01f08b     	bl	0x475664 <AnimSetManager::Flush()> @ imm = #0x7c22c
  3f9434: e1a00004     	mov	r0, r4
  3f9438: ebfcfc1b     	bl	0x3384ac <EventManager::Flush()> @ imm = #-0xc0f94
  3f943c: e59430ec     	ldr	r3, [r4, #0xec]
  3f9440: e3530000     	cmp	r3, #0
  3f9444: 0a000005     	beq	0x3f9460 <Level::~Level()+0x19c> @ imm = #0x14
  3f9448: e1a00003     	mov	r0, r3
  3f944c: e5933000     	ldr	r3, [r3]
  3f9450: e1a0e00f     	mov	lr, pc
  3f9454: e593f004     	ldr	pc, [r3, #0x4]
  3f9458: e3a03000     	mov	r3, #0
  3f945c: e58430ec     	str	r3, [r4, #0xec]
  3f9460: e7955007     	ldr	r5, [r5, r7]
  3f9464: e1a00005     	mov	r0, r5
  3f9468: ebfc983b     	bl	0x31f55c <Application::CleanGlitch()> @ imm = #-0xd9f14
  3f946c: e595003c     	ldr	r0, [r5, #0x3c]
  3f9470: ebfe02dc     	bl	0x379fe8 <LuaManager::FlushBufferedFiles()> @ imm = #-0x7f490
  3f9474: e28400f8     	add	r0, r4, #248
  3f9478: ebfc694b     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe5ad4
  3f947c: e2840044     	add	r0, r4, #68
  3f9480: ebfe0adf     	bl	0x37c004 <LuaScript::~LuaScript()> @ imm = #-0x7d484
  3f9484: e1a00004     	mov	r0, r4
  3f9488: ebfcfc40     	bl	0x338590 <EventManager::~EventManager()> @ imm = #-0xc0f00
  3f948c: e1a00004     	mov	r0, r4
  3f9490: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3f9494: e5960004     	ldr	r0, [r6, #0x4]
  3f9498: ebffe10e     	bl	0x3f18d8 <std::priv::_Rb_tree<int, std::less<int>, std::pair<int const, unsigned int>, std::priv::_Select1st<std::pair<int const, unsigned int>>, std::priv::_MapTraitsT<std::pair<int const, unsigned int>>, std::allocator<std::pair<int const, unsigned int>>>::_M_erase(std::priv::_Rb_tree_node_base*) (.clone.18)> @ imm = #-0x7bc8
  3f949c: e3a03000     	mov	r3, #0
  3f94a0: e5863010     	str	r3, [r6, #0x10]
  3f94a4: e9860048     	stmib	r6, {r3, r6}
  3f94a8: e586600c     	str	r6, [r6, #0xc]
  3f94ac: eaffff96     	b	0x3f930c <Level::~Level()+0x48> @ imm = #-0x1a8
  3f94b0: bc b7 59 00  	.word	0x0059b7bc
  3f94b4: 74 07 00 00  	.word	0x00000774
  3f94b8: d8 43 00 00  	.word	0x000043d8
  3f94bc: 20 1a 00 00  	.word	0x00001a20
  3f94c0: 2c 0e 00 00  	.word	0x00000e2c
  3f94c4: 4c 08 00 00  	.word	0x0000084c
  3f94c8: 08 1b 00 00  	.word	0x00001b08
  3f94cc: f4 37 00 00  	.word	0x000037f4
  3f94d0: 04 12 00 00  	.word	0x00001204
  3f94d4: 38 48 00 00  	.word	0x00004838
