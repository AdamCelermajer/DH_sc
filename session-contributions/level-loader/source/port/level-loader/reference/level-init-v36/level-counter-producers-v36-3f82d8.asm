
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f82d8 <Level::Update(bool)>:
  3f82d8: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3f82dc: e59f48e0     	ldr	r4, [pc, #0x8e0]        @ 0x3f8bc4 <Level::Update(bool)+0x8ec>
  3f82e0: e59f68e0     	ldr	r6, [pc, #0x8e0]        @ 0x3f8bc8 <Level::Update(bool)+0x8f0>
  3f82e4: e1a05000     	mov	r5, r0
  3f82e8: e08f4004     	add	r4, pc, r4
  3f82ec: e7943006     	ldr	r3, [r4, r6]
  3f82f0: e59f08d4     	ldr	r0, [pc, #0x8d4]        @ 0x3f8bcc <Level::Update(bool)+0x8f4>
  3f82f4: e24ddf49     	sub	sp, sp, #292
  3f82f8: e5933000     	ldr	r3, [r3]
  3f82fc: e08f0000     	add	r0, pc, r0
  3f8300: e1a07001     	mov	r7, r1
  3f8304: e58d311c     	str	r3, [sp, #0x11c]
  3f8308: ebfc6ce9     	bl	0x3136b4 <PushProfilingContext(char const*)> @ imm = #-0xe4c5c
  3f830c: e5953130     	ldr	r3, [r5, #0x130]
  3f8310: e3530026     	cmp	r3, #38
  3f8314: 0a00000d     	beq	0x3f8350 <Level::Update(bool)+0x78> @ imm = #0x34
  3f8318: e3570000     	cmp	r7, #0
  3f831c: 1a00000b     	bne	0x3f8350 <Level::Update(bool)+0x78> @ imm = #0x2c
  3f8320: e1a00005     	mov	r0, r5
  3f8324: ebfff999     	bl	0x3f6990 <Level::_LoadProcess()> @ imm = #-0x199c
  3f8328: e59f08a0     	ldr	r0, [pc, #0x8a0]        @ 0x3f8bd0 <Level::Update(bool)+0x8f8>
  3f832c: e08f0000     	add	r0, pc, r0
  3f8330: ebfc6ce0     	bl	0x3136b8 <PopProfilingContext(char const*)> @ imm = #-0xe4c80
  3f8334: e7943006     	ldr	r3, [r4, r6]
  3f8338: e59d211c     	ldr	r2, [sp, #0x11c]
  3f833c: e5933000     	ldr	r3, [r3]
  3f8340: e1520003     	cmp	r2, r3
  3f8344: 1a00021d     	bne	0x3f8bc0 <Level::Update(bool)+0x8e8> @ imm = #0x874
  3f8348: e28ddf49     	add	sp, sp, #292
  3f834c: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3f8350: eb00c869     	bl	0x42a4fc <MenuDebugHUD::GetInstance()> @ imm = #0x321a4
  3f8354: e59030d8     	ldr	r3, [r0, #0xd8]
  3f8358: e1a07000     	mov	r7, r0
  3f835c: e3530000     	cmp	r3, #0
  3f8360: 1a000011     	bne	0x3f83ac <Level::Update(bool)+0xd4> @ imm = #0x44
  3f8364: e59fa868     	ldr	r10, [pc, #0x868]       @ 0x3f8bd4 <Level::Update(bool)+0x8fc>
  3f8368: e28d7f41     	add	r7, sp, #260
  3f836c: e794800a     	ldr	r8, [r4, r10]
  3f8370: e1a00008     	mov	r0, r8
  3f8374: ebfcfd43     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xc0af4
  3f8378: e59f1858     	ldr	r1, [pc, #0x858]        @ 0x3f8bd8 <Level::Update(bool)+0x900>
  3f837c: e28d2070     	add	r2, sp, #112
  3f8380: e1a00007     	mov	r0, r7
  3f8384: e08f1001     	add	r1, pc, r1
  3f8388: ebfc6f57     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe42a4
  3f838c: e1a00008     	mov	r0, r8
  3f8390: e1a01007     	mov	r1, r7
  3f8394: ebfcfdbb     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc0914
  3f8398: e3500000     	cmp	r0, #0
  3f839c: 0a00000c     	beq	0x3f83d4 <Level::Update(bool)+0xfc> @ imm = #0x30
  3f83a0: e1a00007     	mov	r0, r7
  3f83a4: ebfc6d80     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe4a00
  3f83a8: eaffffde     	b	0x3f8328 <Level::Update(bool)+0x50> @ imm = #-0x88
  3f83ac: e28780c8     	add	r8, r7, #200
  3f83b0: e1a00008     	mov	r0, r8
  3f83b4: e59710cc     	ldr	r1, [r7, #0xcc]
  3f83b8: ebffe5e0     	bl	0x3f1b40 <std::priv::_Rb_tree<std::string, std::less<std::string>, std::pair<std::string const, int>, std::priv::_Select1st<std::pair<std::string const, int>>, std::priv::_MapTraitsT<std::pair<std::string const, int>>, std::allocator<std::pair<std::string const, int>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x6880
  3f83bc: e3a03000     	mov	r3, #0
  3f83c0: e58730d8     	str	r3, [r7, #0xd8]
  3f83c4: e58780d4     	str	r8, [r7, #0xd4]
  3f83c8: e58780d0     	str	r8, [r7, #0xd0]
  3f83cc: e58730cc     	str	r3, [r7, #0xcc]
  3f83d0: eaffffe3     	b	0x3f8364 <Level::Update(bool)+0x8c> @ imm = #-0x74
  3f83d4: e1a00008     	mov	r0, r8
  3f83d8: ebfcfd2a     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xc0b58
  3f83dc: e59f17f8     	ldr	r1, [pc, #0x7f8]        @ 0x3f8bdc <Level::Update(bool)+0x904>
  3f83e0: e28d90ec     	add	r9, sp, #236
  3f83e4: e28d206c     	add	r2, sp, #108
  3f83e8: e08f1001     	add	r1, pc, r1
  3f83ec: e1a00009     	mov	r0, r9
  3f83f0: ebfc6f3d     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe430c
  3f83f4: e1a01009     	mov	r1, r9
  3f83f8: e1a00008     	mov	r0, r8
  3f83fc: ebfcfda1     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc097c
  3f8400: e1a0b000     	mov	r11, r0
  3f8404: e1a00009     	mov	r0, r9
  3f8408: ebfc6d67     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe4a64
  3f840c: e1a00007     	mov	r0, r7
  3f8410: ebfc6d65     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe4a6c
  3f8414: e35b0000     	cmp	r11, #0
  3f8418: 1affffc2     	bne	0x3f8328 <Level::Update(bool)+0x50> @ imm = #-0xf8
  3f841c: e5953148     	ldr	r3, [r5, #0x148]
  3f8420: e3730001     	cmn	r3, #1
  3f8424: 1a000104     	bne	0x3f883c <Level::Update(bool)+0x564> @ imm = #0x410
  3f8428: e59f87b0     	ldr	r8, [pc, #0x7b0]        @ 0x3f8be0 <Level::Update(bool)+0x908>
  3f842c: e7943008     	ldr	r3, [r4, r8]
  3f8430: e3a01000     	mov	r1, #0
  3f8434: e3a02001     	mov	r2, #1
  3f8438: e5930040     	ldr	r0, [r3, #0x40]
  3f843c: ebfdd80d     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x89fcc
  3f8440: e5900660     	ldr	r0, [r0, #0x660]
  3f8444: e3500000     	cmp	r0, #0
  3f8448: 0a000001     	beq	0x3f8454 <Level::Update(bool)+0x17c> @ imm = #0x4
  3f844c: e3a01000     	mov	r1, #0
  3f8450: ebff100a     	bl	0x3bc480 <Character::SG_Update(bool)> @ imm = #-0x3bfd8
  3f8454: e5d53144     	ldrb	r3, [r5, #0x144]
  3f8458: e3530000     	cmp	r3, #0
  3f845c: 0a00014e     	beq	0x3f899c <Level::Update(bool)+0x6c4> @ imm = #0x538
  3f8460: e7943008     	ldr	r3, [r4, r8]
  3f8464: e5933040     	ldr	r3, [r3, #0x40]
  3f8468: e5933714     	ldr	r3, [r3, #0x714]
  3f846c: e3530000     	cmp	r3, #0
  3f8470: 0a000145     	beq	0x3f898c <Level::Update(bool)+0x6b4> @ imm = #0x514
  3f8474: e5950194     	ldr	r0, [r5, #0x194]
  3f8478: eb020512     	bl	0x4798c8 <GameEventManager::Update()> @ imm = #0x81448
  3f847c: e7942008     	ldr	r2, [r4, r8]
  3f8480: e5953038     	ldr	r3, [r5, #0x38]
  3f8484: e5922010     	ldr	r2, [r2, #0x10]
  3f8488: e3530000     	cmp	r3, #0
  3f848c: e592701c     	ldr	r7, [r2, #0x1c]
  3f8490: 0a00014c     	beq	0x3f89c8 <Level::Update(bool)+0x6f0> @ imm = #0x530
  3f8494: e593c1cc     	ldr	r12, [r3, #0x1cc]
  3f8498: e59321d0     	ldr	r2, [r3, #0x1d0]
  3f849c: e59331d4     	ldr	r3, [r3, #0x1d4]
  3f84a0: e28d1030     	add	r1, sp, #48
  3f84a4: e1a00007     	mov	r0, r7
  3f84a8: e3a095fe     	mov	r9, #1065353216
  3f84ac: e58dc030     	str	r12, [sp, #0x30]
  3f84b0: e58d2034     	str	r2, [sp, #0x34]
  3f84b4: e58d3038     	str	r3, [sp, #0x38]
  3f84b8: e58d903c     	str	r9, [sp, #0x3c]
  3f84bc: eb064411     	bl	0x589508 <glitch::scene::CSceneManager::setAmbientLight(glitch::video::SColorf const&)> @ imm = #0x191044
  3f84c0: e7947008     	ldr	r7, [r4, r8]
  3f84c4: e5970044     	ldr	r0, [r7, #0x44]
  3f84c8: ebfd4e0e     	bl	0x34bd08 <PhysicalWorld::update()> @ imm = #-0xac7c8
  3f84cc: ebffc897     	bl	0x3ea730 <SpawnGroupManager::GetInstance()> @ imm = #-0xdda4
  3f84d0: e3a02000     	mov	r2, #0
  3f84d4: e3a03000     	mov	r3, #0
  3f84d8: ebffc356     	bl	0x3e9238 <SpawnGroupManager::update(double)> @ imm = #-0xf2a8
  3f84dc: ebff6805     	bl	0x3d24f8 <CharAI::HandleGroups()> @ imm = #-0x25fec
  3f84e0: e1a01009     	mov	r1, r9
  3f84e4: e5970038     	ldr	r0, [r7, #0x38]
  3f84e8: ebfd484c     	bl	0x34a620 <ObjectManager::Update(float)> @ imm = #-0xaded0
  3f84ec: ebff5931     	bl	0x3ce9b8 <CharAI::IncUpdateQueue()> @ imm = #-0x29b3c
  3f84f0: e1a00005     	mov	r0, r5
  3f84f4: eb0004dd     	bl	0x3f9870 <Level::UpdateCameraZoom()> @ imm = #0x1374
  3f84f8: e59f36e4     	ldr	r3, [pc, #0x6e4]        @ 0x3f8be4 <Level::Update(bool)+0x90c>
  3f84fc: e7940003     	ldr	r0, [r4, r3]
  3f8500: eb027823     	bl	0x496594 <VisualFXManager::Update()> @ imm = #0x9e08c
  3f8504: e595312c     	ldr	r3, [r5, #0x12c]
  3f8508: e3530000     	cmp	r3, #0
  3f850c: 0a000004     	beq	0x3f8524 <Level::Update(bool)+0x24c> @ imm = #0x10
  3f8510: e59f26d0     	ldr	r2, [pc, #0x6d0]        @ 0x3f8be8 <Level::Update(bool)+0x910>
  3f8514: e7942002     	ldr	r2, [r4, r2]
  3f8518: e5922000     	ldr	r2, [r2]
  3f851c: e1530002     	cmp	r3, r2
  3f8520: 0a0001a1     	beq	0x3f8bac <Level::Update(bool)+0x8d4> @ imm = #0x684
  3f8524: e5953128     	ldr	r3, [r5, #0x128]
  3f8528: e1a00003     	mov	r0, r3
  3f852c: e5933000     	ldr	r3, [r3]
  3f8530: e1a0e00f     	mov	lr, pc
  3f8534: e593f010     	ldr	pc, [r3, #0x10]
  3f8538: e5953128     	ldr	r3, [r5, #0x128]
  3f853c: e3a01000     	mov	r1, #0
  3f8540: e5933008     	ldr	r3, [r3, #0x8]
  3f8544: e1a00003     	mov	r0, r3
  3f8548: e5933000     	ldr	r3, [r3]
  3f854c: e1a0e00f     	mov	lr, pc
  3f8550: e593f0b8     	ldr	pc, [r3, #0xb8]
  3f8554: e5953128     	ldr	r3, [r5, #0x128]
  3f8558: e28d004c     	add	r0, sp, #76
  3f855c: e5931008     	ldr	r1, [r3, #0x8]
  3f8560: eb067b06     	bl	0x597180 <glitch::scene::ISceneNode::getAbsolutePosition() const> @ imm = #0x19ec18
  3f8564: e5953128     	ldr	r3, [r5, #0x128]
  3f8568: e5930008     	ldr	r0, [r3, #0x8]
  3f856c: eb067b47     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0x19ed1c
  3f8570: e1a01000     	mov	r1, r0
  3f8574: e28d0040     	add	r0, sp, #64
  3f8578: eb067b00     	bl	0x597180 <glitch::scene::ISceneNode::getAbsolutePosition() const> @ imm = #0x19ec00
  3f857c: e59d1048     	ldr	r1, [sp, #0x48]
  3f8580: e59d0054     	ldr	r0, [sp, #0x54]
  3f8584: ebfc5788     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0xea1e0
  3f8588: e59511a4     	ldr	r1, [r5, #0x1a4]
  3f858c: ebfc5786     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0xea1e8
  3f8590: e7943008     	ldr	r3, [r4, r8]
  3f8594: e1a0b000     	mov	r11, r0
  3f8598: e59d1040     	ldr	r1, [sp, #0x40]
  3f859c: e5933010     	ldr	r3, [r3, #0x10]
  3f85a0: e59d004c     	ldr	r0, [sp, #0x4c]
  3f85a4: e593701c     	ldr	r7, [r3, #0x1c]
  3f85a8: ebfc577f     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0xea204
  3f85ac: e595119c     	ldr	r1, [r5, #0x19c]
  3f85b0: ebfc577d     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0xea20c
  3f85b4: e5971458     	ldr	r1, [r7, #0x458]
  3f85b8: ebfc59eb     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xe9854
  3f85bc: e59d1044     	ldr	r1, [sp, #0x44]
  3f85c0: e1a09000     	mov	r9, r0
  3f85c4: e59d0050     	ldr	r0, [sp, #0x50]
  3f85c8: ebfc5777     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0xea224
  3f85cc: e59511a0     	ldr	r1, [r5, #0x1a0]
  3f85d0: ebfc5775     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0xea22c
  3f85d4: e597145c     	ldr	r1, [r7, #0x45c]
  3f85d8: ebfc59e3     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xe9874
  3f85dc: e5971460     	ldr	r1, [r7, #0x460]
  3f85e0: e1a03000     	mov	r3, r0
  3f85e4: e1a0000b     	mov	r0, r11
  3f85e8: e58d301c     	str	r3, [sp, #0x1c]
  3f85ec: ebfc59de     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xe9888
  3f85f0: e1a01009     	mov	r1, r9
  3f85f4: e1a07000     	mov	r7, r0
  3f85f8: e1a00009     	mov	r0, r9
  3f85fc: ebfc59da     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xe9898
  3f8600: e59d301c     	ldr	r3, [sp, #0x1c]
  3f8604: e1a09000     	mov	r9, r0
  3f8608: e1a01003     	mov	r1, r3
  3f860c: e1a00003     	mov	r0, r3
  3f8610: ebfc59d5     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xe98ac
  3f8614: e1a01000     	mov	r1, r0
  3f8618: e1a00009     	mov	r0, r9
  3f861c: ebfc5960     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0xe9a80
  3f8620: e1a01007     	mov	r1, r7
  3f8624: e1a09000     	mov	r9, r0
  3f8628: e1a00007     	mov	r0, r7
  3f862c: ebfc59ce     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xe98c8
  3f8630: e1a01000     	mov	r1, r0
  3f8634: e1a00009     	mov	r0, r9
  3f8638: ebfc5959     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0xe9a9c
  3f863c: ebfc5898     	bl	0x30e8a4 <.plt+0xb30>   @ imm = #-0xe9da0
  3f8640: ebfc56de     	bl	0x30e1c0 <.plt+0x44c>   @ imm = #-0xea488
  3f8644: ebfc5815     	bl	0x30e6a0 <.plt+0x92c>   @ imm = #-0xe9fac
  3f8648: e3a01000     	mov	r1, #0
  3f864c: e1a09000     	mov	r9, r0
  3f8650: e1a0000b     	mov	r0, r11
  3f8654: ebfc582c     	bl	0x30e70c <.plt+0x998>   @ imm = #-0xe9f50
  3f8658: e7943008     	ldr	r3, [r4, r8]
  3f865c: e5957038     	ldr	r7, [r5, #0x38]
  3f8660: e3500000     	cmp	r0, #0
  3f8664: e5933010     	ldr	r3, [r3, #0x10]
  3f8668: 12899102     	addne	r9, r9, #-2147483648
  3f866c: e3570000     	cmp	r7, #0
  3f8670: e593301c     	ldr	r3, [r3, #0x1c]
  3f8674: e58d3020     	str	r3, [sp, #0x20]
  3f8678: 0a0000e8     	beq	0x3f8a20 <Level::Update(bool)+0x748> @ imm = #0x3a0
  3f867c: e59701d8     	ldr	r0, [r7, #0x1d8]
  3f8680: ebfc58b7     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0xe9d24
  3f8684: e1a01009     	mov	r1, r9
  3f8688: ebfc5945     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0xe9aec
  3f868c: e1a0b000     	mov	r11, r0
  3f8690: e59701dc     	ldr	r0, [r7, #0x1dc]
  3f8694: ebfc58b2     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0xe9d38
  3f8698: e1a01009     	mov	r1, r9
  3f869c: ebfc5940     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0xe9b00
  3f86a0: e2873e1e     	add	r3, r7, #480
  3f86a4: e1a02000     	mov	r2, r0
  3f86a8: e1a0100b     	mov	r1, r11
  3f86ac: e59d0020     	ldr	r0, [sp, #0x20]
  3f86b0: ebfd6710     	bl	0x3522f8 <SceneManager::UpdateFog(float, float, Point3D<float> const&)> @ imm = #-0xa63c0
  3f86b4: e59f7530     	ldr	r7, [pc, #0x530]        @ 0x3f8bec <Level::Update(bool)+0x914>
  3f86b8: e794b00a     	ldr	r11, [r4, r10]
  3f86bc: e28d90d4     	add	r9, sp, #212
  3f86c0: e08f7007     	add	r7, pc, r7
  3f86c4: e1a0000b     	mov	r0, r11
  3f86c8: ebfcfc6e     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xc0e48
  3f86cc: e28d2068     	add	r2, sp, #104
  3f86d0: e1a00009     	mov	r0, r9
  3f86d4: e1a01007     	mov	r1, r7
  3f86d8: ebfc6e83     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe45f4
  3f86dc: e1a01009     	mov	r1, r9
  3f86e0: e1a0000b     	mov	r0, r11
  3f86e4: ebfcfce7     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc0c64
  3f86e8: e794b008     	ldr	r11, [r4, r8]
  3f86ec: e1a03000     	mov	r3, r0
  3f86f0: e1a00009     	mov	r0, r9
  3f86f4: e59b2010     	ldr	r2, [r11, #0x10]
  3f86f8: e592201c     	ldr	r2, [r2, #0x1c]
  3f86fc: e5d29430     	ldrb	r9, [r2, #0x430]
  3f8700: e58d301c     	str	r3, [sp, #0x1c]
  3f8704: ebfc6ca8     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe4d60
  3f8708: e59d301c     	ldr	r3, [sp, #0x1c]
  3f870c: e1530009     	cmp	r3, r9
  3f8710: 0a000112     	beq	0x3f8b60 <Level::Update(bool)+0x888> @ imm = #0x448
  3f8714: e794b00a     	ldr	r11, [r4, r10]
  3f8718: e59f74d0     	ldr	r7, [pc, #0x4d0]        @ 0x3f8bf0 <Level::Update(bool)+0x918>
  3f871c: e28d90a4     	add	r9, sp, #164
  3f8720: e1a0000b     	mov	r0, r11
  3f8724: e08f7007     	add	r7, pc, r7
  3f8728: ebfcfc56     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xc0ea8
  3f872c: e28d2060     	add	r2, sp, #96
  3f8730: e1a00009     	mov	r0, r9
  3f8734: e1a01007     	mov	r1, r7
  3f8738: ebfc6e6b     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe4654
  3f873c: e1a01009     	mov	r1, r9
  3f8740: e1a0000b     	mov	r0, r11
  3f8744: ebfcfccf     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc0cc4
  3f8748: e794b008     	ldr	r11, [r4, r8]
  3f874c: e1a03000     	mov	r3, r0
  3f8750: e1a00009     	mov	r0, r9
  3f8754: e59b2010     	ldr	r2, [r11, #0x10]
  3f8758: e592201c     	ldr	r2, [r2, #0x1c]
  3f875c: e5d29431     	ldrb	r9, [r2, #0x431]
  3f8760: e58d301c     	str	r3, [sp, #0x1c]
  3f8764: ebfc6c90     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe4dc0
  3f8768: e59d301c     	ldr	r3, [sp, #0x1c]
  3f876c: e1530009     	cmp	r3, r9
  3f8770: 0a0000e7     	beq	0x3f8b14 <Level::Update(bool)+0x83c> @ imm = #0x39c
  3f8774: e5d51144     	ldrb	r1, [r5, #0x144]
  3f8778: e3510000     	cmp	r1, #0
  3f877c: 1a000011     	bne	0x3f87c8 <Level::Update(bool)+0x4f0> @ imm = #0x44
  3f8780: e59f346c     	ldr	r3, [pc, #0x46c]        @ 0x3f8bf4 <Level::Update(bool)+0x91c>
  3f8784: e7943003     	ldr	r3, [r4, r3]
  3f8788: e5937000     	ldr	r7, [r3]
  3f878c: e5d73032     	ldrb	r3, [r7, #0x32]
  3f8790: e3530000     	cmp	r3, #0
  3f8794: 0a000084     	beq	0x3f89ac <Level::Update(bool)+0x6d4> @ imm = #0x210
  3f8798: e1a00007     	mov	r0, r7
  3f879c: ebfdce1c     	bl	0x36c014 <VoxSoundManager::SetInSafeZoneMusic(bool)> @ imm = #-0x8c790
  3f87a0: e3a0c000     	mov	r12, #0
  3f87a4: e5951124     	ldr	r1, [r5, #0x124]
  3f87a8: e1a0300c     	mov	r3, r12
  3f87ac: e1a0200c     	mov	r2, r12
  3f87b0: e1a00007     	mov	r0, r7
  3f87b4: e58dc000     	str	r12, [sp]
  3f87b8: e58dc004     	str	r12, [sp, #0x4]
  3f87bc: ebfdcc12     	bl	0x36b80c <VoxSoundManager::Play(int, bool, int, int, bool)> @ imm = #-0x8cfb8
  3f87c0: e3a03001     	mov	r3, #1
  3f87c4: e5c73031     	strb	r3, [r7, #0x31]
  3f87c8: e3a03001     	mov	r3, #1
  3f87cc: e5c53144     	strb	r3, [r5, #0x144]
  3f87d0: e1a00005     	mov	r0, r5
  3f87d4: ebffe0c1     	bl	0x3f0ae0 <Level::UpdateListener()> @ imm = #-0x7cfc
  3f87d8: e1a00005     	mov	r0, r5
  3f87dc: ebffe3b7     	bl	0x3f16c0 <Level::UpdateDynamicFog()> @ imm = #-0x7124
  3f87e0: eb00c745     	bl	0x42a4fc <MenuDebugHUD::GetInstance()> @ imm = #0x31d14
  3f87e4: e794a00a     	ldr	r10, [r4, r10]
  3f87e8: e58d0020     	str	r0, [sp, #0x20]
  3f87ec: e28d7074     	add	r7, sp, #116
  3f87f0: e1a0000a     	mov	r0, r10
  3f87f4: ebfcfc23     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xc0f74
  3f87f8: e59f13f8     	ldr	r1, [pc, #0x3f8]        @ 0x3f8bf8 <Level::Update(bool)+0x920>
  3f87fc: e28d2058     	add	r2, sp, #88
  3f8800: e1a00007     	mov	r0, r7
  3f8804: e08f1001     	add	r1, pc, r1
  3f8808: ebfc6e37     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe4724
  3f880c: e1a0000a     	mov	r0, r10
  3f8810: e1a01007     	mov	r1, r7
  3f8814: ebfcfc9b     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc0d94
  3f8818: e1a0a000     	mov	r10, r0
  3f881c: e1a00007     	mov	r0, r7
  3f8820: ebfc6c61     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe4e7c
  3f8824: e35a0000     	cmp	r10, #0
  3f8828: 1a00000f     	bne	0x3f886c <Level::Update(bool)+0x594> @ imm = #0x3c
  3f882c: e59f03c8     	ldr	r0, [pc, #0x3c8]        @ 0x3f8bfc <Level::Update(bool)+0x924>
  3f8830: e08f0000     	add	r0, pc, r0
  3f8834: ebfc6b9f     	bl	0x3136b8 <PopProfilingContext(char const*)> @ imm = #-0xe5184
  3f8838: eafffebd     	b	0x3f8334 <Level::Update(bool)+0x5c> @ imm = #-0x50c
  3f883c: e59f839c     	ldr	r8, [pc, #0x39c]        @ 0x3f8be0 <Level::Update(bool)+0x908>
  3f8840: e7940008     	ldr	r0, [r4, r8]
  3f8844: ebfc9b8e     	bl	0x31f684 <Application::IsCurrentlyInGameView() const> @ imm = #-0xd91c8
  3f8848: e3500000     	cmp	r0, #0
  3f884c: 0afffef6     	beq	0x3f842c <Level::Update(bool)+0x154> @ imm = #-0x428
  3f8850: e59f23a8     	ldr	r2, [pc, #0x3a8]        @ 0x3f8c00 <Level::Update(bool)+0x928>
  3f8854: e1a0300b     	mov	r3, r11
  3f8858: e5951148     	ldr	r1, [r5, #0x148]
  3f885c: e7940002     	ldr	r0, [r4, r2]
  3f8860: e3e02000     	mvn	r2, #0
  3f8864: eb019f55     	bl	0x4605c0 <ScriptManager::StartScript(int, int, bool)> @ imm = #0x67d54
  3f8868: eafffeef     	b	0x3f842c <Level::Update(bool)+0x154> @ imm = #-0x444
  3f886c: e59fa390     	ldr	r10, [pc, #0x390]       @ 0x3f8c04 <Level::Update(bool)+0x92c>
  3f8870: e59d0020     	ldr	r0, [sp, #0x20]
  3f8874: e59f738c     	ldr	r7, [pc, #0x38c]        @ 0x3f8c08 <Level::Update(bool)+0x930>
  3f8878: e08fa00a     	add	r10, pc, r10
  3f887c: e59a1000     	ldr	r1, [r10]
  3f8880: eb00c7ca     	bl	0x42a7b0 <MenuDebugHUD::DisplayHugeNumber(int)> @ imm = #0x31f28
  3f8884: e59a1000     	ldr	r1, [r10]
  3f8888: e59d0020     	ldr	r0, [sp, #0x20]
  3f888c: e08f7007     	add	r7, pc, r7
  3f8890: e351000f     	cmp	r1, #15
  3f8894: d3a01000     	movle	r1, #0
  3f8898: c3a01001     	movgt	r1, #1
  3f889c: eb00c807     	bl	0x42a8c0 <MenuDebugHUD::SetDisplaySealOfFreshness(bool)> @ imm = #0x3201c
  3f88a0: e7940008     	ldr	r0, [r4, r8]
  3f88a4: e597a000     	ldr	r10, [r7]
  3f88a8: ebfc9b6f     	bl	0x31f66c <Application::GetDt()> @ imm = #-0xd9244
  3f88ac: e060000a     	rsb	r0, r0, r10
  3f88b0: e3500000     	cmp	r0, #0
  3f88b4: e5870000     	str	r0, [r7]
  3f88b8: caffffdb     	bgt	0x3f882c <Level::Update(bool)+0x554> @ imm = #-0x94
  3f88bc: e59f3348     	ldr	r3, [pc, #0x348]        @ 0x3f8c0c <Level::Update(bool)+0x934>
  3f88c0: e30d2b17     	movw	r2, #0xdb17
  3f88c4: e3422b52     	movt	r2, #0x2b52
  3f88c8: e7943003     	ldr	r3, [r4, r3]
  3f88cc: e58d2024     	str	r2, [sp, #0x24]
  3f88d0: e59f2338     	ldr	r2, [pc, #0x338]        @ 0x3f8c10 <Level::Update(bool)+0x938>
  3f88d4: e593a000     	ldr	r10, [r3]
  3f88d8: e30fc26b     	movw	r12, #0xf26b
  3f88dc: e59fb330     	ldr	r11, [pc, #0x330]       @ 0x3f8c14 <Level::Update(bool)+0x93c>
  3f88e0: e59f9330     	ldr	r9, [pc, #0x330]        @ 0x3f8c18 <Level::Update(bool)+0x940>
  3f88e4: e340c0da     	movt	r12, #0xda
  3f88e8: e58dc028     	str	r12, [sp, #0x28]
  3f88ec: e3a07000     	mov	r7, #0
  3f88f0: e58d202c     	str	r2, [sp, #0x2c]
  3f88f4: e24aa001     	sub	r10, r10, #1
  3f88f8: e3570000     	cmp	r7, #0
  3f88fc: 0a00005d     	beq	0x3f8a78 <Level::Update(bool)+0x7a0> @ imm = #0x174
  3f8900: e1a00007     	mov	r0, r7
  3f8904: e595110c     	ldr	r1, [r5, #0x10c]
  3f8908: ebfc5683     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xea5f4
  3f890c: e3500000     	cmp	r0, #0
  3f8910: 0a000058     	beq	0x3f8a78 <Level::Update(bool)+0x7a0> @ imm = #0x160
  3f8914: e1a00007     	mov	r0, r7
  3f8918: ebffdc37     	bl	0x3ef9fc <DBG_DisplayLoadLevelStats(char const*)> @ imm = #-0x8f24
  3f891c: e59d0020     	ldr	r0, [sp, #0x20]
  3f8920: eb00c784     	bl	0x42a738 <MenuDebugHUD::HideHugeNumber()> @ imm = #0x31e10
  3f8924: e59d0020     	ldr	r0, [sp, #0x20]
  3f8928: e3a01000     	mov	r1, #0
  3f892c: eb00c7e3     	bl	0x42a8c0 <MenuDebugHUD::SetDisplaySealOfFreshness(bool)> @ imm = #0x31f8c
  3f8930: e59f32e4     	ldr	r3, [pc, #0x2e4]        @ 0x3f8c1c <Level::Update(bool)+0x944>
  3f8934: e7945008     	ldr	r5, [r4, r8]
  3f8938: e3a01000     	mov	r1, #0
  3f893c: e08f3003     	add	r3, pc, r3
  3f8940: e3012388     	movw	r2, #0x1388
  3f8944: e5832000     	str	r2, [r3]
  3f8948: e5950040     	ldr	r0, [r5, #0x40]
  3f894c: e1a02001     	mov	r2, r1
  3f8950: ebfdd77b     	bl	0x36e744 <PlayerManager::GetPlayer(int, bool)> @ imm = #-0x8a214
  3f8954: e5903664     	ldr	r3, [r0, #0x664]
  3f8958: e3a0c000     	mov	r12, #0
  3f895c: e3a0e001     	mov	lr, #1
  3f8960: e1a00005     	mov	r0, r5
  3f8964: e1a01007     	mov	r1, r7
  3f8968: e1a0200c     	mov	r2, r12
  3f896c: e1c33fc3     	bic	r3, r3, r3, asr #31
  3f8970: e88d5000     	stm	sp, {r12, lr}
  3f8974: e58dc008     	str	r12, [sp, #0x8]
  3f8978: e58dc00c     	str	r12, [sp, #0xc]
  3f897c: e58dc010     	str	r12, [sp, #0x10]
  3f8980: e58dc014     	str	r12, [sp, #0x14]
  3f8984: ebfccd0f     	bl	0x32bdc8 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)> @ imm = #-0xccbc4
  3f8988: eaffffa7     	b	0x3f882c <Level::Update(bool)+0x554> @ imm = #-0x164
  3f898c: e59f326c     	ldr	r3, [pc, #0x26c]        @ 0x3f8c00 <Level::Update(bool)+0x928>
  3f8990: e7940003     	ldr	r0, [r4, r3]
  3f8994: eb018e78     	bl	0x45c37c <ScriptManager::ExecuteAllScripts()> @ imm = #0x639e0
  3f8998: eafffeb5     	b	0x3f8474 <Level::Update(bool)+0x19c> @ imm = #-0x52c
  3f899c: e7943008     	ldr	r3, [r4, r8]
  3f89a0: e5930040     	ldr	r0, [r3, #0x40]
  3f89a4: ebfe0182     	bl	0x378fb4 <PlayerManager::Update()> @ imm = #-0x7f9f8
  3f89a8: eafffeac     	b	0x3f8460 <Level::Update(bool)+0x188> @ imm = #-0x550
  3f89ac: e595111c     	ldr	r1, [r5, #0x11c]
  3f89b0: e3a0ce7d     	mov	r12, #2000
  3f89b4: e1a00007     	mov	r0, r7
  3f89b8: e3a02001     	mov	r2, #1
  3f89bc: e58dc000     	str	r12, [sp]
  3f89c0: ebfdccec     	bl	0x36bd78 <VoxSoundManager::PlayMusic(int, bool, bool, int)> @ imm = #-0x8cc50
  3f89c4: eaffff75     	b	0x3f87a0 <Level::Update(bool)+0x4c8> @ imm = #-0x22c
  3f89c8: e59f2250     	ldr	r2, [pc, #0x250]        @ 0x3f8c20 <Level::Update(bool)+0x948>
  3f89cc: e7942002     	ldr	r2, [r4, r2]
  3f89d0: e5922000     	ldr	r2, [r2]
  3f89d4: e3520002     	cmp	r2, #2
  3f89d8: 05833000     	streq	r3, [r3]
  3f89dc: 0afffeac     	beq	0x3f8494 <Level::Update(bool)+0x1bc> @ imm = #-0x550
  3f89e0: e3520001     	cmp	r2, #1
  3f89e4: 1afffeaa     	bne	0x3f8494 <Level::Update(bool)+0x1bc> @ imm = #-0x558
  3f89e8: e59f0234     	ldr	r0, [pc, #0x234]        @ 0x3f8c24 <Level::Update(bool)+0x94c>
  3f89ec: e59f1234     	ldr	r1, [pc, #0x234]        @ 0x3f8c28 <Level::Update(bool)+0x950>
  3f89f0: e59f2234     	ldr	r2, [pc, #0x234]        @ 0x3f8c2c <Level::Update(bool)+0x954>
  3f89f4: e7940000     	ldr	r0, [r4, r0]
  3f89f8: e59f3230     	ldr	r3, [pc, #0x230]        @ 0x3f8c30 <Level::Update(bool)+0x958>
  3f89fc: e3a0cf77     	mov	r12, #476
  3f8a00: e08f1001     	add	r1, pc, r1
  3f8a04: e08f3003     	add	r3, pc, r3
  3f8a08: e28000a8     	add	r0, r0, #168
  3f8a0c: e08f2002     	add	r2, pc, r2
  3f8a10: e58dc000     	str	r12, [sp]
  3f8a14: ebfc557a     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xeaa18
  3f8a18: e5953038     	ldr	r3, [r5, #0x38]
  3f8a1c: eafffe9c     	b	0x3f8494 <Level::Update(bool)+0x1bc> @ imm = #-0x590
  3f8a20: e59f31f8     	ldr	r3, [pc, #0x1f8]        @ 0x3f8c20 <Level::Update(bool)+0x948>
  3f8a24: e7943003     	ldr	r3, [r4, r3]
  3f8a28: e5933000     	ldr	r3, [r3]
  3f8a2c: e3530002     	cmp	r3, #2
  3f8a30: 05877000     	streq	r7, [r7]
  3f8a34: 0affff10     	beq	0x3f867c <Level::Update(bool)+0x3a4> @ imm = #-0x3c0
  3f8a38: e3530001     	cmp	r3, #1
  3f8a3c: 1affff0e     	bne	0x3f867c <Level::Update(bool)+0x3a4> @ imm = #-0x3c8
  3f8a40: e59f01dc     	ldr	r0, [pc, #0x1dc]        @ 0x3f8c24 <Level::Update(bool)+0x94c>
  3f8a44: e59f11e8     	ldr	r1, [pc, #0x1e8]        @ 0x3f8c34 <Level::Update(bool)+0x95c>
  3f8a48: e59f21e8     	ldr	r2, [pc, #0x1e8]        @ 0x3f8c38 <Level::Update(bool)+0x960>
  3f8a4c: e7940000     	ldr	r0, [r4, r0]
  3f8a50: e59f31e4     	ldr	r3, [pc, #0x1e4]        @ 0x3f8c3c <Level::Update(bool)+0x964>
  3f8a54: e3a0cf77     	mov	r12, #476
  3f8a58: e08f1001     	add	r1, pc, r1
  3f8a5c: e28000a8     	add	r0, r0, #168
  3f8a60: e08f2002     	add	r2, pc, r2
  3f8a64: e08f3003     	add	r3, pc, r3
  3f8a68: e58dc000     	str	r12, [sp]
  3f8a6c: ebfc5564     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xeaa70
  3f8a70: e5957038     	ldr	r7, [r5, #0x38]
  3f8a74: eaffff00     	b	0x3f867c <Level::Update(bool)+0x3a4> @ imm = #-0x400
  3f8a78: e35a0000     	cmp	r10, #0
  3f8a7c: 01a0100a     	moveq	r1, r10
  3f8a80: 0a000018     	beq	0x3f8ae8 <Level::Update(bool)+0x810> @ imm = #0x60
  3f8a84: e59d302c     	ldr	r3, [sp, #0x2c]
  3f8a88: e30ec6ab     	movw	r12, #0xe6ab
  3f8a8c: e1a0100a     	mov	r1, r10
  3f8a90: e7942003     	ldr	r2, [r4, r3]
  3f8a94: e5920000     	ldr	r0, [r2]
  3f8a98: e000009c     	mul	r0, r12, r0
  3f8a9c: e59dc024     	ldr	r12, [sp, #0x24]
  3f8aa0: e2800a2b     	add	r0, r0, #176128
  3f8aa4: e2800fff     	add	r0, r0, #1020
  3f8aa8: e2800001     	add	r0, r0, #1
  3f8aac: e083c09c     	umull	r12, r3, r12, r0
  3f8ab0: e063c000     	rsb	r12, r3, r0
  3f8ab4: e08330ac     	add	r3, r3, r12, lsr #1
  3f8ab8: e59dc028     	ldr	r12, [sp, #0x28]
  3f8abc: e1a03ba3     	lsr	r3, r3, #23
  3f8ac0: e063039c     	mls	r3, r12, r3, r0
  3f8ac4: e5823000     	str	r3, [r2]
  3f8ac8: e1a00003     	mov	r0, r3
  3f8acc: ebfc5816     	bl	0x30eb2c <.plt+0xdb8>   @ imm = #-0xe9fa8
  3f8ad0: e3510000     	cmp	r1, #0
  3f8ad4: a3a02048     	movge	r2, #72
  3f8ad8: b2611000     	rsblt	r1, r1, #0
  3f8adc: b3a03048     	movlt	r3, #72
  3f8ae0: a0010192     	mulge	r1, r2, r1
  3f8ae4: b0010193     	mullt	r1, r3, r1
  3f8ae8: e794300b     	ldr	r3, [r4, r11]
  3f8aec: e7942009     	ldr	r2, [r4, r9]
  3f8af0: e5930000     	ldr	r0, [r3]
  3f8af4: e5922000     	ldr	r2, [r2]
  3f8af8: e2800001     	add	r0, r0, #1
  3f8afc: e5830000     	str	r0, [r3]
  3f8b00: e0822001     	add	r2, r2, r1
  3f8b04: e5d23004     	ldrb	r3, [r2, #0x4]
  3f8b08: e3530000     	cmp	r3, #0
  3f8b0c: 15927020     	ldrne	r7, [r2, #0x20]
  3f8b10: eaffff78     	b	0x3f88f8 <Level::Update(bool)+0x620> @ imm = #-0x220
  3f8b14: e59b3010     	ldr	r3, [r11, #0x10]
  3f8b18: e28d908c     	add	r9, sp, #140
  3f8b1c: e593301c     	ldr	r3, [r3, #0x1c]
  3f8b20: e58d301c     	str	r3, [sp, #0x1c]
  3f8b24: ebfcdf05     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc83ec
  3f8b28: e1a01007     	mov	r1, r7
  3f8b2c: e1a0b000     	mov	r11, r0
  3f8b30: e28d205c     	add	r2, sp, #92
  3f8b34: e1a00009     	mov	r0, r9
  3f8b38: ebfc6d6b     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe4a54
  3f8b3c: e1a0000b     	mov	r0, r11
  3f8b40: e1a01009     	mov	r1, r9
  3f8b44: ebfcfbcf     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc10c4
  3f8b48: e59d301c     	ldr	r3, [sp, #0x1c]
  3f8b4c: e2200001     	eor	r0, r0, #1
  3f8b50: e5c30431     	strb	r0, [r3, #0x431]
  3f8b54: e1a00009     	mov	r0, r9
  3f8b58: ebfc6b93     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe51b4
  3f8b5c: eaffff04     	b	0x3f8774 <Level::Update(bool)+0x49c> @ imm = #-0x3f0
  3f8b60: e59b3010     	ldr	r3, [r11, #0x10]
  3f8b64: e28d90bc     	add	r9, sp, #188
  3f8b68: e593301c     	ldr	r3, [r3, #0x1c]
  3f8b6c: e58d301c     	str	r3, [sp, #0x1c]
  3f8b70: ebfcdef2     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc8438
  3f8b74: e1a01007     	mov	r1, r7
  3f8b78: e1a0b000     	mov	r11, r0
  3f8b7c: e28d2064     	add	r2, sp, #100
  3f8b80: e1a00009     	mov	r0, r9
  3f8b84: ebfc6d58     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe4aa0
  3f8b88: e1a0000b     	mov	r0, r11
  3f8b8c: e1a01009     	mov	r1, r9
  3f8b90: ebfcfbbc     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc1110
  3f8b94: e59d301c     	ldr	r3, [sp, #0x1c]
  3f8b98: e2200001     	eor	r0, r0, #1
  3f8b9c: e5c30430     	strb	r0, [r3, #0x430]
  3f8ba0: e1a00009     	mov	r0, r9
  3f8ba4: ebfc6b80     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe5200
  3f8ba8: eafffed9     	b	0x3f8714 <Level::Update(bool)+0x43c> @ imm = #-0x49c
  3f8bac: e1a00003     	mov	r0, r3
  3f8bb0: e5933000     	ldr	r3, [r3]
  3f8bb4: e1a0e00f     	mov	lr, pc
  3f8bb8: e593f010     	ldr	pc, [r3, #0x10]
  3f8bbc: eafffe58     	b	0x3f8524 <Level::Update(bool)+0x24c> @ imm = #-0x6a0
  3f8bc0: ebfc55d2     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xea8b8
  3f8bc4: a8 c7 59 00  	.word	0x0059c7a8
  3f8bc8: ac 40 00 00  	.word	0x000040ac
  3f8bcc: 8c e8 4c 00  	.word	0x004ce88c
  3f8bd0: 5c e8 4c 00  	.word	0x004ce85c
  3f8bd4: 84 08 00 00  	.word	0x00000884
  3f8bd8: c4 6b 4c 00  	.word	0x004c6bc4
  3f8bdc: 60 75 4c 00  	.word	0x004c7560
  3f8be0: f4 37 00 00  	.word	0x000037f4
  3f8be4: 08 1b 00 00  	.word	0x00001b08
  3f8be8: b0 42 00 00  	.word	0x000042b0
  3f8bec: 30 73 4c 00  	.word	0x004c7330
  3f8bf0: ec 72 4c 00  	.word	0x004c72ec
  3f8bf4: a4 0d 00 00  	.word	0x00000da4
  3f8bf8: 94 e3 4c 00  	.word	0x004ce394
  3f8bfc: 58 e3 4c 00  	.word	0x004ce358
  3f8c00: 20 1a 00 00  	.word	0x00001a20
  3f8c04: 68 a8 5a 00  	.word	0x005aa868
  3f8c08: d0 0e 5a 00  	.word	0x005a0ed0
  3f8c0c: c0 18 00 00  	.word	0x000018c0
  3f8c10: 94 0c 00 00  	.word	0x00000c94
  3f8c14: 88 10 00 00  	.word	0x00001088
  3f8c18: 74 08 00 00  	.word	0x00000874
  3f8c1c: 20 0e 5a 00  	.word	0x005a0e20
  3f8c20: c0 39 00 00  	.word	0x000039c0
  3f8c24: c0 19 00 00  	.word	0x000019c0
  3f8c28: d8 59 4c 00  	.word	0x004c59d8
  3f8c2c: e4 d1 4c 00  	.word	0x004cd1e4
  3f8c30: 54 92 4c 00  	.word	0x004c9254
  3f8c34: 80 59 4c 00  	.word	0x004c5980
  3f8c38: 90 d1 4c 00  	.word	0x004cd190
  3f8c3c: f4 91 4c 00  	.word	0x004c91f4
