
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f90b0 <Level::~Level()>:
  3f90b0: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3f90b4: e59f51e0     	ldr	r5, [pc, #0x1e0]        @ 0x3f929c <Level::~Level()+0x1ec>
  3f90b8: e59f31e0     	ldr	r3, [pc, #0x1e0]        @ 0x3f92a0 <Level::~Level()+0x1f0>
  3f90bc: e1a04000     	mov	r4, r0
  3f90c0: e08f5005     	add	r5, pc, r5
  3f90c4: e7953003     	ldr	r3, [r5, r3]
  3f90c8: e2833008     	add	r3, r3, #8
  3f90cc: e5803000     	str	r3, [r0]
  3f90d0: ebffd8c9     	bl	0x3ef3fc <Level::CleanUpAllSkills()> @ imm = #-0x9cdc
  3f90d4: e1a00004     	mov	r0, r4
  3f90d8: ebffd8e0     	bl	0x3ef460 <Level::UnlockAllObjects()> @ imm = #-0x9c80
  3f90dc: ebff50a8     	bl	0x3cd384 <CharAI::ClearAllAggro()> @ imm = #-0x2bd60
  3f90e0: ebff67b2     	bl	0x3d2fb0 <CharAI::ClearGroupInfo()> @ imm = #-0x26138
  3f90e4: e59f31b8     	ldr	r3, [pc, #0x1b8]        @ 0x3f92a4 <Level::~Level()+0x1f4>
  3f90e8: e7956003     	ldr	r6, [r5, r3]
  3f90ec: e5963010     	ldr	r3, [r6, #0x10]
  3f90f0: e3530000     	cmp	r3, #0
  3f90f4: 1a000061     	bne	0x3f9280 <Level::~Level()+0x1d0> @ imm = #0x184
  3f90f8: ebfeba27     	bl	0x3a799c <Character::ClearConcurrentAI()> @ imm = #-0x51764
  3f90fc: ebffffc5     	bl	0x3f9018 <MenuMessageManager<StatusMsg, 4>::FlushEnqueuedMessages(int) (.clone.25)> @ imm = #-0xec
  3f9100: ebffe45b     	bl	0x3f2274 <MenuMessageManager<TutorialMsg, 1>::FlushEnqueuedMessages(int) (.clone.26)> @ imm = #-0x6e94
  3f9104: ebffffd6     	bl	0x3f9064 <MenuMessageManager<DialogMsg, 1>::FlushEnqueuedMessages(int) (.clone.27)> @ imm = #-0xa8
  3f9108: ebffffd5     	bl	0x3f9064 <MenuMessageManager<DialogMsg, 1>::FlushEnqueuedMessages(int) (.clone.27)> @ imm = #-0xac
  3f910c: ebffe278     	bl	0x3f1af4 <MenuMessageManager<OnlineStatusMsg, 1>::FlushEnqueuedMessages(int) (.clone.16)> @ imm = #-0x7620
  3f9110: e59f3190     	ldr	r3, [pc, #0x190]        @ 0x3f92a8 <Level::~Level()+0x1f8>
  3f9114: e7950003     	ldr	r0, [r5, r3]
  3f9118: eb018463     	bl	0x45a2ac <ScriptManager::Flush()> @ imm = #0x6118c
  3f911c: eb00972b     	bl	0x41edd0 <InfoHUDManager::GetInstance()> @ imm = #0x25cac
  3f9120: eb009794     	bl	0x41ef78 <InfoHUDManager::Flush()> @ imm = #0x25e50
  3f9124: eb0087fc     	bl	0x41b11c <HUDControls::GetInstance()> @ imm = #0x21ff0
  3f9128: eb007c44     	bl	0x418240 <HUDControls::Flush()> @ imm = #0x1f110
  3f912c: e59f3178     	ldr	r3, [pc, #0x178]        @ 0x3f92ac <Level::~Level()+0x1fc>
  3f9130: e7950003     	ldr	r0, [r5, r3]
  3f9134: ebffc960     	bl	0x3eb6bc <ItemManager::Flush()> @ imm = #-0xda80
  3f9138: e59f3170     	ldr	r3, [pc, #0x170]        @ 0x3f92b0 <Level::~Level()+0x200>
  3f913c: e7950003     	ldr	r0, [r5, r3]
  3f9140: ebffb4c9     	bl	0x3e646c <ProjectileManager::Flush()> @ imm = #-0x12cdc
  3f9144: e59f3168     	ldr	r3, [pc, #0x168]        @ 0x3f92b4 <Level::~Level()+0x204>
  3f9148: e7950003     	ldr	r0, [r5, r3]
  3f914c: eb026fd3     	bl	0x4950a0 <VisualFXManager::FlushLibraries()> @ imm = #0x9bf4c
  3f9150: e5946194     	ldr	r6, [r4, #0x194]
  3f9154: e3560000     	cmp	r6, #0
  3f9158: 0a000005     	beq	0x3f9174 <Level::~Level()+0xc4> @ imm = #0x14
  3f915c: e1a00006     	mov	r0, r6
  3f9160: eb0202a1     	bl	0x479bec <GameEventManager::~GameEventManager()> @ imm = #0x80a84
  3f9164: e1a00006     	mov	r0, r6
  3f9168: ebfc5cb4     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xe8d30
  3f916c: e3a03000     	mov	r3, #0
  3f9170: e5843194     	str	r3, [r4, #0x194]
  3f9174: e5946158     	ldr	r6, [r4, #0x158]
  3f9178: e3560000     	cmp	r6, #0
  3f917c: 0a000005     	beq	0x3f9198 <Level::~Level()+0xe8> @ imm = #0x14
  3f9180: e1a00006     	mov	r0, r6
  3f9184: ebffe22f     	bl	0x3f1a48 <batch::BatchNodeCompiler::~BatchNodeCompiler()> @ imm = #-0x7744
  3f9188: e1a00006     	mov	r0, r6
  3f918c: ebfc5cab     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xe8d54
  3f9190: e3a03000     	mov	r3, #0
  3f9194: e5843158     	str	r3, [r4, #0x158]
  3f9198: e594615c     	ldr	r6, [r4, #0x15c]
  3f919c: e3560000     	cmp	r6, #0
  3f91a0: 0a000005     	beq	0x3f91bc <Level::~Level()+0x10c> @ imm = #0x14
  3f91a4: e1a00006     	mov	r0, r6
  3f91a8: ebffe226     	bl	0x3f1a48 <batch::BatchNodeCompiler::~BatchNodeCompiler()> @ imm = #-0x7768
  3f91ac: e1a00006     	mov	r0, r6
  3f91b0: ebfc5ca2     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xe8d78
  3f91b4: e3a03000     	mov	r3, #0
  3f91b8: e584315c     	str	r3, [r4, #0x15c]
  3f91bc: e59f70f4     	ldr	r7, [pc, #0xf4]         @ 0x3f92b8 <Level::~Level()+0x208>
  3f91c0: e7956007     	ldr	r6, [r5, r7]
  3f91c4: e5960038     	ldr	r0, [r6, #0x38]
  3f91c8: ebfd413a     	bl	0x3496b8 <ObjectManager::Flush()> @ imm = #-0xafb18
  3f91cc: e3a01000     	mov	r1, #0
  3f91d0: e5960050     	ldr	r0, [r6, #0x50]
  3f91d4: ebfe2375     	bl	0x381fb0 <ZoomHandler::setCamera(CameraLevel*)> @ imm = #-0x7722c
  3f91d8: e5963010     	ldr	r3, [r6, #0x10]
  3f91dc: e593301c     	ldr	r3, [r3, #0x1c]
  3f91e0: e1a00003     	mov	r0, r3
  3f91e4: e5933000     	ldr	r3, [r3]
  3f91e8: e1a0e00f     	mov	lr, pc
  3f91ec: e593f068     	ldr	pc, [r3, #0x68]
  3f91f0: e5963010     	ldr	r3, [r6, #0x10]
  3f91f4: e3a01000     	mov	r1, #0
  3f91f8: e593001c     	ldr	r0, [r3, #0x1c]
  3f91fc: eb063faf     	bl	0x5890c0 <glitch::scene::CSceneManager::setActiveCamera(glitch::scene::ICameraSceneNode*)> @ imm = #0x18febc
  3f9200: e5960044     	ldr	r0, [r6, #0x44]
  3f9204: ebfd4b00     	bl	0x34be0c <PhysicalWorld::clear()> @ imm = #-0xad400
  3f9208: e59f30ac     	ldr	r3, [pc, #0xac]         @ 0x3f92bc <Level::~Level()+0x20c>
  3f920c: e7950003     	ldr	r0, [r5, r3]
  3f9210: eb04a92e     	bl	0x5236d0 <PFWorld::Flush()> @ imm = #0x12a4b8
  3f9214: e59f30a4     	ldr	r3, [pc, #0xa4]         @ 0x3f92c0 <Level::~Level()+0x210>
  3f9218: e7950003     	ldr	r0, [r5, r3]
  3f921c: eb01f110     	bl	0x475664 <AnimSetManager::Flush()> @ imm = #0x7c440
  3f9220: e1a00004     	mov	r0, r4
  3f9224: ebfcfca0     	bl	0x3384ac <EventManager::Flush()> @ imm = #-0xc0d80
  3f9228: e59430ec     	ldr	r3, [r4, #0xec]
  3f922c: e3530000     	cmp	r3, #0
  3f9230: 0a000005     	beq	0x3f924c <Level::~Level()+0x19c> @ imm = #0x14
  3f9234: e1a00003     	mov	r0, r3
  3f9238: e5933000     	ldr	r3, [r3]
  3f923c: e1a0e00f     	mov	lr, pc
  3f9240: e593f004     	ldr	pc, [r3, #0x4]
  3f9244: e3a03000     	mov	r3, #0
  3f9248: e58430ec     	str	r3, [r4, #0xec]
  3f924c: e7955007     	ldr	r5, [r5, r7]
  3f9250: e1a00005     	mov	r0, r5
  3f9254: ebfc98c0     	bl	0x31f55c <Application::CleanGlitch()> @ imm = #-0xd9d00
  3f9258: e595003c     	ldr	r0, [r5, #0x3c]
  3f925c: ebfe0361     	bl	0x379fe8 <LuaManager::FlushBufferedFiles()> @ imm = #-0x7f27c
  3f9260: e28400f8     	add	r0, r4, #248
  3f9264: ebfc69d0     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe58c0
  3f9268: e2840044     	add	r0, r4, #68
  3f926c: ebfe0b64     	bl	0x37c004 <LuaScript::~LuaScript()> @ imm = #-0x7d270
  3f9270: e1a00004     	mov	r0, r4
  3f9274: ebfcfcc5     	bl	0x338590 <EventManager::~EventManager()> @ imm = #-0xc0cec
  3f9278: e1a00004     	mov	r0, r4
  3f927c: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3f9280: e5960004     	ldr	r0, [r6, #0x4]
  3f9284: ebffe193     	bl	0x3f18d8 <std::priv::_Rb_tree<int, std::less<int>, std::pair<int const, unsigned int>, std::priv::_Select1st<std::pair<int const, unsigned int>>, std::priv::_MapTraitsT<std::pair<int const, unsigned int>>, std::allocator<std::pair<int const, unsigned int>>>::_M_erase(std::priv::_Rb_tree_node_base*) (.clone.18)> @ imm = #-0x79b4
  3f9288: e3a03000     	mov	r3, #0
  3f928c: e5863010     	str	r3, [r6, #0x10]
  3f9290: e9860048     	stmib	r6, {r3, r6}
  3f9294: e586600c     	str	r6, [r6, #0xc]
  3f9298: eaffff96     	b	0x3f90f8 <Level::~Level()+0x48> @ imm = #-0x1a8
  3f929c: d0 b9 59 00  	.word	0x0059b9d0
  3f92a0: 74 07 00 00  	.word	0x00000774
  3f92a4: d8 43 00 00  	.word	0x000043d8
  3f92a8: 20 1a 00 00  	.word	0x00001a20
  3f92ac: 2c 0e 00 00  	.word	0x00000e2c
  3f92b0: 4c 08 00 00  	.word	0x0000084c
  3f92b4: 08 1b 00 00  	.word	0x00001b08
  3f92b8: f4 37 00 00  	.word	0x000037f4
  3f92bc: 04 12 00 00  	.word	0x00001204
  3f92c0: 38 48 00 00  	.word	0x00004838
