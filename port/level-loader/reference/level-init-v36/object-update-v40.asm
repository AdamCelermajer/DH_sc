
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034a620 <ObjectManager::Update(float)>:
  34a620: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  34a624: e59f75c4     	ldr	r7, [pc, #0x5c4]        @ 0x34abf0 <ObjectManager::Update(float)+0x5d0>
  34a628: e59fa5c4     	ldr	r10, [pc, #0x5c4]       @ 0x34abf4 <ObjectManager::Update(float)+0x5d4>
  34a62c: e1a05000     	mov	r5, r0
  34a630: e08f7007     	add	r7, pc, r7
  34a634: e797300a     	ldr	r3, [r7, r10]
  34a638: e59f05b8     	ldr	r0, [pc, #0x5b8]        @ 0x34abf8 <ObjectManager::Update(float)+0x5d8>
  34a63c: e24dd0b4     	sub	sp, sp, #180
  34a640: e5933000     	ldr	r3, [r3]
  34a644: e08f0000     	add	r0, pc, r0
  34a648: e1a04001     	mov	r4, r1
  34a64c: e58d30ac     	str	r3, [sp, #0xac]
  34a650: ebff2417     	bl	0x3136b4 <PushProfilingContext(char const*)> @ imm = #-0x36fa4
  34a654: eb12cc4e     	bl	0x7fd794 <GetOnline()>  @ imm = #0x4b3138
  34a658: e5d03005     	ldrb	r3, [r0, #0x5]
  34a65c: e3530000     	cmp	r3, #0
  34a660: 13a03001     	movne	r3, #1
  34a664: 15c530fd     	strbne	r3, [r5, #0xfd]
  34a668: 15c530fc     	strbne	r3, [r5, #0xfc]
  34a66c: e59f3588     	ldr	r3, [pc, #0x588]        @ 0x34abfc <ObjectManager::Update(float)+0x5dc>
  34a670: e7970003     	ldr	r0, [r7, r3]
  34a674: ebff53c6     	bl	0x31f594 <Application::GetCurrentLevel() const> @ imm = #-0x2b0e8
  34a678: e3500000     	cmp	r0, #0
  34a67c: 0a000002     	beq	0x34a68c <ObjectManager::Update(float)+0x6c> @ imm = #0x8
  34a680: e5d03144     	ldrb	r3, [r0, #0x144]
  34a684: e3530000     	cmp	r3, #0
  34a688: 1a0000ff     	bne	0x34aa8c <ObjectManager::Update(float)+0x46c> @ imm = #0x3fc
  34a68c: e1a00005     	mov	r0, r5
  34a690: ebffdbe9     	bl	0x34163c <ObjectManager::UpdateRooms()> @ imm = #-0x905c
  34a694: e1a01004     	mov	r1, r4
  34a698: e1a00005     	mov	r0, r5
  34a69c: ebffd6f4     	bl	0x340274 <ObjectManager::DoRemoteUpdateUpdate(float)> @ imm = #-0xa430
  34a6a0: e1a04005     	mov	r4, r5
  34a6a4: e1a00005     	mov	r0, r5
  34a6a8: ebffee87     	bl	0x3460cc <ObjectManager::ProcessNextGameObjectToStartUpdate()> @ imm = #-0x45e4
  34a6ac: e5b4c034     	ldr	r12, [r4, #0x34]!
  34a6b0: e15c0004     	cmp	r12, r4
  34a6b4: 0285802c     	addeq	r8, r5, #44
  34a6b8: 0a000010     	beq	0x34a700 <ObjectManager::Update(float)+0xe0> @ imm = #0x40
  34a6bc: e1a0300c     	mov	r3, r12
  34a6c0: e5933000     	ldr	r3, [r3]
  34a6c4: e1540003     	cmp	r4, r3
  34a6c8: 1afffffc     	bne	0x34a6c0 <ObjectManager::Update(float)+0xa0> @ imm = #-0x10
  34a6cc: e285802c     	add	r8, r5, #44
  34a6d0: e1a00008     	mov	r0, r8
  34a6d4: e58dc068     	str	r12, [sp, #0x68]
  34a6d8: e28d1064     	add	r1, sp, #100
  34a6dc: e28dc070     	add	r12, sp, #112
  34a6e0: e28d2068     	add	r2, sp, #104
  34a6e4: e28d306c     	add	r3, sp, #108
  34a6e8: e58dc000     	str	r12, [sp]
  34a6ec: e58d8064     	str	r8, [sp, #0x64]
  34a6f0: e58d406c     	str	r4, [sp, #0x6c]
  34a6f4: ebffeb4b     	bl	0x345428 <void std::list<ObjectBase*, std::allocator<ObjectBase*>>::_M_splice_insert_dispatch<std::priv::_List_iterator<ObjectBase*, std::_Nonconst_traits<ObjectBase*>>>(std::priv::_List_iterator<ObjectBase*, std::_Nonconst_traits<ObjectBase*>>, std::priv::_List_iterator<ObjectBase*, std::_Nonconst_traits<ObjectBase*>>, std::priv::_List_iterator<ObjectBase*, std::_Nonconst_traits<ObjectBase*>>, std::__false_type const&)> @ imm = #-0x52d4
  34a6f8: e1a00004     	mov	r0, r4
  34a6fc: ebffeada     	bl	0x34526c <std::priv::_List_base<ObjectBase*, std::allocator<ObjectBase*>>::clear()> @ imm = #-0x5498
  34a700: e1a06005     	mov	r6, r5
  34a704: e5b6403c     	ldr	r4, [r6, #0x3c]!
  34a708: e1540006     	cmp	r4, r6
  34a70c: 0a000013     	beq	0x34a760 <ObjectManager::Update(float)+0x140> @ imm = #0x4c
  34a710: e1a03004     	mov	r3, r4
  34a714: e5933000     	ldr	r3, [r3]
  34a718: e1560003     	cmp	r6, r3
  34a71c: 1afffffc     	bne	0x34a714 <ObjectManager::Update(float)+0xf4> @ imm = #-0x10
  34a720: e28d2054     	add	r2, sp, #84
  34a724: e1540006     	cmp	r4, r6
  34a728: e28d9048     	add	r9, sp, #72
  34a72c: e58d200c     	str	r2, [sp, #0xc]
  34a730: 0a00000a     	beq	0x34a760 <ObjectManager::Update(float)+0x140> @ imm = #0x28
  34a734: e5941008     	ldr	r1, [r4, #0x8]
  34a738: e5d13029     	ldrb	r3, [r1, #0x29]
  34a73c: e3530000     	cmp	r3, #0
  34a740: 0a0000f3     	beq	0x34ab14 <ObjectManager::Update(float)+0x4f4> @ imm = #0x3cc
  34a744: e5d13082     	ldrb	r3, [r1, #0x82]
  34a748: e3530000     	cmp	r3, #0
  34a74c: 1a0000f3     	bne	0x34ab20 <ObjectManager::Update(float)+0x500> @ imm = #0x3cc
  34a750: e594b000     	ldr	r11, [r4]
  34a754: e1a0400b     	mov	r4, r11
  34a758: e1540006     	cmp	r4, r6
  34a75c: 1afffff4     	bne	0x34a734 <ObjectManager::Update(float)+0x114> @ imm = #-0x30
  34a760: e1a09005     	mov	r9, r5
  34a764: e5b96044     	ldr	r6, [r9, #0x44]!
  34a768: e1590006     	cmp	r9, r6
  34a76c: 0a00000f     	beq	0x34a7b0 <ObjectManager::Update(float)+0x190> @ imm = #0x3c
  34a770: e5964008     	ldr	r4, [r6, #0x8]
  34a774: e5d430ac     	ldrb	r3, [r4, #0xac]
  34a778: e3530000     	cmp	r3, #0
  34a77c: 1a00008f     	bne	0x34a9c0 <ObjectManager::Update(float)+0x3a0> @ imm = #0x23c
  34a780: e59430a8     	ldr	r3, [r4, #0xa8]
  34a784: e3530000     	cmp	r3, #0
  34a788: 0a00008c     	beq	0x34a9c0 <ObjectManager::Update(float)+0x3a0> @ imm = #0x230
  34a78c: e3a01001     	mov	r1, #1
  34a790: e1a00004     	mov	r0, r4
  34a794: ebffcfce     	bl	0x33e6d4 <ObjectBase::TestEnableCondition(bool)> @ imm = #-0xc0c8
  34a798: e1a00004     	mov	r0, r4
  34a79c: e3a01001     	mov	r1, #1
  34a7a0: ebffcf9d     	bl	0x33e61c <ObjectBase::TestDisableCondition(bool)> @ imm = #-0xc18c
  34a7a4: e5966000     	ldr	r6, [r6]
  34a7a8: e1590006     	cmp	r9, r6
  34a7ac: 1affffef     	bne	0x34a770 <ObjectManager::Update(float)+0x150> @ imm = #-0x44
  34a7b0: e3a03000     	mov	r3, #0
  34a7b4: e5853058     	str	r3, [r5, #0x58]
  34a7b8: e585305c     	str	r3, [r5, #0x5c]
  34a7bc: e595402c     	ldr	r4, [r5, #0x2c]
  34a7c0: e28d303c     	add	r3, sp, #60
  34a7c4: e58d300c     	str	r3, [sp, #0xc]
  34a7c8: e59f3430     	ldr	r3, [pc, #0x430]        @ 0x34ac00 <ObjectManager::Update(float)+0x5e0>
  34a7cc: e28dc024     	add	r12, sp, #36
  34a7d0: e58dc010     	str	r12, [sp, #0x10]
  34a7d4: e28d2030     	add	r2, sp, #48
  34a7d8: e28dc060     	add	r12, sp, #96
  34a7dc: e1580004     	cmp	r8, r4
  34a7e0: e2856070     	add	r6, r5, #112
  34a7e4: e58d2014     	str	r2, [sp, #0x14]
  34a7e8: e58d3018     	str	r3, [sp, #0x18]
  34a7ec: e58dc01c     	str	r12, [sp, #0x1c]
  34a7f0: 0a000039     	beq	0x34a8dc <ObjectManager::Update(float)+0x2bc> @ imm = #0xe4
  34a7f4: e5949008     	ldr	r9, [r4, #0x8]
  34a7f8: e3590000     	cmp	r9, #0
  34a7fc: 0a000032     	beq	0x34a8cc <ObjectManager::Update(float)+0x2ac> @ imm = #0xc8
  34a800: e5993000     	ldr	r3, [r9]
  34a804: e1a00009     	mov	r0, r9
  34a808: e1a0e00f     	mov	lr, pc
  34a80c: e593f024     	ldr	pc, [r3, #0x24]
  34a810: e3500000     	cmp	r0, #0
  34a814: 1a000092     	bne	0x34aa64 <ObjectManager::Update(float)+0x444> @ imm = #0x248
  34a818: e5d93085     	ldrb	r3, [r9, #0x85]
  34a81c: e3530000     	cmp	r3, #0
  34a820: 0a00007a     	beq	0x34aa10 <ObjectManager::Update(float)+0x3f0> @ imm = #0x1e8
  34a824: e5d9308a     	ldrb	r3, [r9, #0x8a]
  34a828: e3530000     	cmp	r3, #0
  34a82c: 0a000077     	beq	0x34aa10 <ObjectManager::Update(float)+0x3f0> @ imm = #0x1dc
  34a830: e5d9b081     	ldrb	r11, [r9, #0x81]
  34a834: e35b0000     	cmp	r11, #0
  34a838: 1a000068     	bne	0x34a9e0 <ObjectManager::Update(float)+0x3c0> @ imm = #0x1a0
  34a83c: e5993000     	ldr	r3, [r9]
  34a840: e1a00009     	mov	r0, r9
  34a844: e5c9b088     	strb	r11, [r9, #0x88]
  34a848: e1a0e00f     	mov	lr, pc
  34a84c: e593f02c     	ldr	pc, [r3, #0x2c]
  34a850: e59d000c     	ldr	r0, [sp, #0xc]
  34a854: e1a01009     	mov	r1, r9
  34a858: ebffcd33     	bl	0x33dd2c <ObjectBase::GetHandle()> @ imm = #-0xcb34
  34a85c: e59d000c     	ldr	r0, [sp, #0xc]
  34a860: e1a0100b     	mov	r1, r11
  34a864: ebffd555     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xaaac
  34a868: e3500000     	cmp	r0, #0
  34a86c: 0a000016     	beq	0x34a8cc <ObjectManager::Update(float)+0x2ac> @ imm = #0x58
  34a870: e5d93088     	ldrb	r3, [r9, #0x88]
  34a874: e5d92089     	ldrb	r2, [r9, #0x89]
  34a878: e1520003     	cmp	r2, r3
  34a87c: 0a000012     	beq	0x34a8cc <ObjectManager::Update(float)+0x2ac> @ imm = #0x48
  34a880: e3530000     	cmp	r3, #0
  34a884: e5c93089     	strb	r3, [r9, #0x89]
  34a888: 1a000091     	bne	0x34aad4 <ObjectManager::Update(float)+0x4b4> @ imm = #0x244
  34a88c: e1a01009     	mov	r1, r9
  34a890: e59d0010     	ldr	r0, [sp, #0x10]
  34a894: ebffcd24     	bl	0x33dd2c <ObjectBase::GetHandle()> @ imm = #-0xcb70
  34a898: e59d0010     	ldr	r0, [sp, #0x10]
  34a89c: ebffd5ac     	bl	0x33ff54 <ObjectHandle::operator Character*()> @ imm = #-0xa950
  34a8a0: e1a0b000     	mov	r11, r0
  34a8a4: e5950070     	ldr	r0, [r5, #0x70]
  34a8a8: e1560000     	cmp	r6, r0
  34a8ac: 0a000006     	beq	0x34a8cc <ObjectManager::Update(float)+0x2ac> @ imm = #0x18
  34a8b0: e5903008     	ldr	r3, [r0, #0x8]
  34a8b4: e5909000     	ldr	r9, [r0]
  34a8b8: e15b0003     	cmp	r11, r3
  34a8bc: 0a00007d     	beq	0x34aab8 <ObjectManager::Update(float)+0x498> @ imm = #0x1f4
  34a8c0: e1a00009     	mov	r0, r9
  34a8c4: e1560000     	cmp	r6, r0
  34a8c8: 1afffff8     	bne	0x34a8b0 <ObjectManager::Update(float)+0x290> @ imm = #-0x20
  34a8cc: e5949000     	ldr	r9, [r4]
  34a8d0: e1a04009     	mov	r4, r9
  34a8d4: e1580004     	cmp	r8, r4
  34a8d8: 1affffc5     	bne	0x34a7f4 <ObjectManager::Update(float)+0x1d4> @ imm = #-0xec
  34a8dc: e59f5320     	ldr	r5, [pc, #0x320]        @ 0x34ac04 <ObjectManager::Update(float)+0x5e4>
  34a8e0: e28d4094     	add	r4, sp, #148
  34a8e4: e7976005     	ldr	r6, [r7, r5]
  34a8e8: e1a00006     	mov	r0, r6
  34a8ec: ebffb3e5     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0x1306c
  34a8f0: e59f1310     	ldr	r1, [pc, #0x310]        @ 0x34ac08 <ObjectManager::Update(float)+0x5e8>
  34a8f4: e28d2078     	add	r2, sp, #120
  34a8f8: e1a00004     	mov	r0, r4
  34a8fc: e08f1001     	add	r1, pc, r1
  34a900: ebff25f9     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x3681c
  34a904: e1a00006     	mov	r0, r6
  34a908: e1a01004     	mov	r1, r4
  34a90c: e3a02000     	mov	r2, #0
  34a910: ebffb531     	bl	0x337ddc <DebugSwitches::SetSwitch(std::string const&, bool)> @ imm = #-0x12b3c
  34a914: e59d00a8     	ldr	r0, [sp, #0xa8]
  34a918: e1500004     	cmp	r0, r4
  34a91c: 0a000006     	beq	0x34a93c <ObjectManager::Update(float)+0x31c> @ imm = #0x18
  34a920: e3500000     	cmp	r0, #0
  34a924: 0a000004     	beq	0x34a93c <ObjectManager::Update(float)+0x31c> @ imm = #0x10
  34a928: e59d1094     	ldr	r1, [sp, #0x94]
  34a92c: e0601001     	rsb	r1, r0, r1
  34a930: e3510080     	cmp	r1, #128
  34a934: 8a0000a8     	bhi	0x34abdc <ObjectManager::Update(float)+0x5bc> @ imm = #0x2a0
  34a938: eb0ef970     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x3be5c0
  34a93c: e7975005     	ldr	r5, [r7, r5]
  34a940: e28d407c     	add	r4, sp, #124
  34a944: e1a00005     	mov	r0, r5
  34a948: ebffb3ce     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0x130c8
  34a94c: e59f12b8     	ldr	r1, [pc, #0x2b8]        @ 0x34ac0c <ObjectManager::Update(float)+0x5ec>
  34a950: e28d2074     	add	r2, sp, #116
  34a954: e1a00004     	mov	r0, r4
  34a958: e08f1001     	add	r1, pc, r1
  34a95c: ebff25e2     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x36878
  34a960: e1a00005     	mov	r0, r5
  34a964: e1a01004     	mov	r1, r4
  34a968: e3a02000     	mov	r2, #0
  34a96c: ebffb51a     	bl	0x337ddc <DebugSwitches::SetSwitch(std::string const&, bool)> @ imm = #-0x12b98
  34a970: e59d0090     	ldr	r0, [sp, #0x90]
  34a974: e1500004     	cmp	r0, r4
  34a978: 0a000006     	beq	0x34a998 <ObjectManager::Update(float)+0x378> @ imm = #0x18
  34a97c: e3500000     	cmp	r0, #0
  34a980: 0a000004     	beq	0x34a998 <ObjectManager::Update(float)+0x378> @ imm = #0x10
  34a984: e59d107c     	ldr	r1, [sp, #0x7c]
  34a988: e0601001     	rsb	r1, r0, r1
  34a98c: e3510080     	cmp	r1, #128
  34a990: 8a000093     	bhi	0x34abe4 <ObjectManager::Update(float)+0x5c4> @ imm = #0x24c
  34a994: eb0ef959     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x3be564
  34a998: e59f0270     	ldr	r0, [pc, #0x270]        @ 0x34ac10 <ObjectManager::Update(float)+0x5f0>
  34a99c: e08f0000     	add	r0, pc, r0
  34a9a0: ebff2344     	bl	0x3136b8 <PopProfilingContext(char const*)> @ imm = #-0x372f0
  34a9a4: e797300a     	ldr	r3, [r7, r10]
  34a9a8: e59d20ac     	ldr	r2, [sp, #0xac]
  34a9ac: e5933000     	ldr	r3, [r3]
  34a9b0: e1520003     	cmp	r2, r3
  34a9b4: 1a00008c     	bne	0x34abec <ObjectManager::Update(float)+0x5cc> @ imm = #0x230
  34a9b8: e28dd0b4     	add	sp, sp, #180
  34a9bc: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  34a9c0: e5d430d0     	ldrb	r3, [r4, #0xd0]
  34a9c4: e3530000     	cmp	r3, #0
  34a9c8: 1affff75     	bne	0x34a7a4 <ObjectManager::Update(float)+0x184> @ imm = #-0x22c
  34a9cc: e59430cc     	ldr	r3, [r4, #0xcc]
  34a9d0: e3530000     	cmp	r3, #0
  34a9d4: 1affff6c     	bne	0x34a78c <ObjectManager::Update(float)+0x16c> @ imm = #-0x250
  34a9d8: e5966000     	ldr	r6, [r6]
  34a9dc: eaffff71     	b	0x34a7a8 <ObjectManager::Update(float)+0x188> @ imm = #-0x23c
  34a9e0: e1a01009     	mov	r1, r9
  34a9e4: e1a00005     	mov	r0, r5
  34a9e8: ebffe242     	bl	0x3432f8 <ObjectManager::MarkForDeletion(ObjectBase*)> @ imm = #-0x76f8
  34a9ec: e5949000     	ldr	r9, [r4]
  34a9f0: e5943004     	ldr	r3, [r4, #0x4]
  34a9f4: e1a00004     	mov	r0, r4
  34a9f8: e3a0100c     	mov	r1, #12
  34a9fc: e5839000     	str	r9, [r3]
  34aa00: e5893004     	str	r3, [r9, #0x4]
  34aa04: eb0ef93d     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x3be4f4
  34aa08: e1a04009     	mov	r4, r9
  34aa0c: eaffffb0     	b	0x34a8d4 <ObjectManager::Update(float)+0x2b4> @ imm = #-0x140
  34aa10: eb12cb5f     	bl	0x7fd794 <GetOnline()>  @ imm = #0x4b2d7c
  34aa14: e5d03005     	ldrb	r3, [r0, #0x5]
  34aa18: e3530000     	cmp	r3, #0
  34aa1c: 1a000013     	bne	0x34aa70 <ObjectManager::Update(float)+0x450> @ imm = #0x4c
  34aa20: e3a02000     	mov	r2, #0
  34aa24: e5c92086     	strb	r2, [r9, #0x86]
  34aa28: e5993000     	ldr	r3, [r9]
  34aa2c: e1a00009     	mov	r0, r9
  34aa30: e1a0e00f     	mov	lr, pc
  34aa34: e593f024     	ldr	pc, [r3, #0x24]
  34aa38: e3500000     	cmp	r0, #0
  34aa3c: 0affffa2     	beq	0x34a8cc <ObjectManager::Update(float)+0x2ac> @ imm = #-0x178
  34aa40: e59dc018     	ldr	r12, [sp, #0x18]
  34aa44: e1a00009     	mov	r0, r9
  34aa48: e59d101c     	ldr	r1, [sp, #0x1c]
  34aa4c: e797300c     	ldr	r3, [r7, r12]
  34aa50: e3a02000     	mov	r2, #0
  34aa54: e58d3060     	str	r3, [sp, #0x60]
  34aa58: eb017431     	bl	0x3a7b24 <Character::UnLoadScriptProcess(std::priv::_Rb_tree_iterator<std::pair<int const, Character*>, std::priv::_MapTraitsT<std::pair<int const, Character*>>>, bool)> @ imm = #0x5d0c4
  34aa5c: e5949000     	ldr	r9, [r4]
  34aa60: eaffff9a     	b	0x34a8d0 <ObjectManager::Update(float)+0x2b0> @ imm = #-0x198
  34aa64: e1a00009     	mov	r0, r9
  34aa68: eb016635     	bl	0x3a4344 <Character::UpdateAIPointers()> @ imm = #0x598d4
  34aa6c: eaffff69     	b	0x34a818 <ObjectManager::Update(float)+0x1f8> @ imm = #-0x25c
  34aa70: e5993000     	ldr	r3, [r9]
  34aa74: e1a00009     	mov	r0, r9
  34aa78: e1a0e00f     	mov	lr, pc
  34aa7c: e593f054     	ldr	pc, [r3, #0x54]
  34aa80: e3500000     	cmp	r0, #0
  34aa84: 0affffe5     	beq	0x34aa20 <ObjectManager::Update(float)+0x400> @ imm = #-0x6c
  34aa88: eaffff68     	b	0x34a830 <ObjectManager::Update(float)+0x210> @ imm = #-0x260
  34aa8c: e5d03198     	ldrb	r3, [r0, #0x198]
  34aa90: e3530000     	cmp	r3, #0
  34aa94: 1afffefc     	bne	0x34a68c <ObjectManager::Update(float)+0x6c> @ imm = #-0x410
  34aa98: eb12cb3d     	bl	0x7fd794 <GetOnline()>  @ imm = #0x4b2cf4
  34aa9c: e5d03005     	ldrb	r3, [r0, #0x5]
  34aaa0: e3530000     	cmp	r3, #0
  34aaa4: 1afffef8     	bne	0x34a68c <ObjectManager::Update(float)+0x6c> @ imm = #-0x420
  34aaa8: e59f0164     	ldr	r0, [pc, #0x164]        @ 0x34ac14 <ObjectManager::Update(float)+0x5f4>
  34aaac: e08f0000     	add	r0, pc, r0
  34aab0: ebff2300     	bl	0x3136b8 <PopProfilingContext(char const*)> @ imm = #-0x37400
  34aab4: eaffffba     	b	0x34a9a4 <ObjectManager::Update(float)+0x384> @ imm = #-0x118
  34aab8: e5903004     	ldr	r3, [r0, #0x4]
  34aabc: e3a0100c     	mov	r1, #12
  34aac0: e5839000     	str	r9, [r3]
  34aac4: e5893004     	str	r3, [r9, #0x4]
  34aac8: eb0ef90c     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x3be430
  34aacc: e1a00009     	mov	r0, r9
  34aad0: eaffff7b     	b	0x34a8c4 <ObjectManager::Update(float)+0x2a4> @ imm = #-0x214
  34aad4: e1a01009     	mov	r1, r9
  34aad8: e59d0014     	ldr	r0, [sp, #0x14]
  34aadc: ebffcc92     	bl	0x33dd2c <ObjectBase::GetHandle()> @ imm = #-0xcdb8
  34aae0: e59d0014     	ldr	r0, [sp, #0x14]
  34aae4: ebffd51a     	bl	0x33ff54 <ObjectHandle::operator Character*()> @ imm = #-0xab98
  34aae8: e1a09000     	mov	r9, r0
  34aaec: e1a00006     	mov	r0, r6
  34aaf0: ebffdee6     	bl	0x342690 <std::allocator<std::priv::_List_node<Character*>>::allocate(unsigned int, void const*) (.clone.13)> @ imm = #-0x8468
  34aaf4: e5809008     	str	r9, [r0, #0x8]
  34aaf8: e5953074     	ldr	r3, [r5, #0x74]
  34aafc: e5806000     	str	r6, [r0]
  34ab00: e5803004     	str	r3, [r0, #0x4]
  34ab04: e5830000     	str	r0, [r3]
  34ab08: e5850074     	str	r0, [r5, #0x74]
  34ab0c: e5949000     	ldr	r9, [r4]
  34ab10: eaffff6e     	b	0x34a8d0 <ObjectManager::Update(float)+0x2b0> @ imm = #-0x248
  34ab14: e5d13082     	ldrb	r3, [r1, #0x82]
  34ab18: e3530000     	cmp	r3, #0
  34ab1c: 0a000003     	beq	0x34ab30 <ObjectManager::Update(float)+0x510> @ imm = #0xc
  34ab20: e2433001     	sub	r3, r3, #1
  34ab24: e5c13082     	strb	r3, [r1, #0x82]
  34ab28: e594b000     	ldr	r11, [r4]
  34ab2c: eaffff08     	b	0x34a754 <ObjectManager::Update(float)+0x134> @ imm = #-0x3e0
  34ab30: e1a00005     	mov	r0, r5
  34ab34: ebfff4ac     	bl	0x347dec <ObjectManager::IsOnlineDeferred(ObjectBase*)> @ imm = #-0x2d50
  34ab38: e3500000     	cmp	r0, #0
  34ab3c: 1a000015     	bne	0x34ab98 <ObjectManager::Update(float)+0x578> @ imm = #0x54
  34ab40: e5943008     	ldr	r3, [r4, #0x8]
  34ab44: e1a00003     	mov	r0, r3
  34ab48: e5933000     	ldr	r3, [r3]
  34ab4c: e1a0e00f     	mov	lr, pc
  34ab50: e593f024     	ldr	pc, [r3, #0x24]
  34ab54: e3500000     	cmp	r0, #0
  34ab58: 1a000010     	bne	0x34aba0 <ObjectManager::Update(float)+0x580> @ imm = #0x40
  34ab5c: e5941008     	ldr	r1, [r4, #0x8]
  34ab60: e1a00009     	mov	r0, r9
  34ab64: ebffd26e     	bl	0x33f524 <ObjectHandle::ObjectHandle(ObjectBase*)> @ imm = #-0xb648
  34ab68: e899000e     	ldm	r9, {r1, r2, r3}
  34ab6c: e1a00005     	mov	r0, r5
  34ab70: ebfff8cb     	bl	0x348ea4 <ObjectManager::Remove(ObjectHandle)> @ imm = #-0x1cd4
  34ab74: e594b000     	ldr	r11, [r4]
  34ab78: e5943004     	ldr	r3, [r4, #0x4]
  34ab7c: e1a00004     	mov	r0, r4
  34ab80: e3a0100c     	mov	r1, #12
  34ab84: e583b000     	str	r11, [r3]
  34ab88: e58b3004     	str	r3, [r11, #0x4]
  34ab8c: eb0ef8db     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x3be36c
  34ab90: e1a0400b     	mov	r4, r11
  34ab94: eafffeef     	b	0x34a758 <ObjectManager::Update(float)+0x138> @ imm = #-0x444
  34ab98: e5941008     	ldr	r1, [r4, #0x8]
  34ab9c: eafffee8     	b	0x34a744 <ObjectManager::Update(float)+0x124> @ imm = #-0x460
  34aba0: e5943008     	ldr	r3, [r4, #0x8]
  34aba4: e1a00003     	mov	r0, r3
  34aba8: e5933000     	ldr	r3, [r3]
  34abac: e1a0e00f     	mov	lr, pc
  34abb0: e593f028     	ldr	pc, [r3, #0x28]
  34abb4: e3500000     	cmp	r0, #0
  34abb8: 0affffe7     	beq	0x34ab5c <ObjectManager::Update(float)+0x53c> @ imm = #-0x64
  34abbc: e5941008     	ldr	r1, [r4, #0x8]
  34abc0: e59d000c     	ldr	r0, [sp, #0xc]
  34abc4: ebffd256     	bl	0x33f524 <ObjectHandle::ObjectHandle(ObjectBase*)> @ imm = #-0xb6a8
  34abc8: e59dc00c     	ldr	r12, [sp, #0xc]
  34abcc: e1a00005     	mov	r0, r5
  34abd0: e89c000e     	ldm	r12, {r1, r2, r3}
  34abd4: ebfff999     	bl	0x349240 <ObjectManager::FakeRemove(ObjectHandle)> @ imm = #-0x199c
  34abd8: eaffffe5     	b	0x34ab74 <ObjectManager::Update(float)+0x554> @ imm = #-0x6c
  34abdc: ebff1617     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x3a7a4
  34abe0: eaffff55     	b	0x34a93c <ObjectManager::Update(float)+0x31c> @ imm = #-0x2ac
  34abe4: ebff1615     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x3a7ac
  34abe8: eaffff6a     	b	0x34a998 <ObjectManager::Update(float)+0x378> @ imm = #-0x258
  34abec: ebff0dc7     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x3c8e4
  34abf0: 60 a4 64 00  	.word	0x0064a460
  34abf4: ac 40 00 00  	.word	0x000040ac
  34abf8: 3c 5d 57 00  	.word	0x00575d3c
  34abfc: f4 37 00 00  	.word	0x000037f4
  34ac00: 34 11 00 00  	.word	0x00001134
  34ac04: 84 08 00 00  	.word	0x00000884
  34ac08: 9c 5a 57 00  	.word	0x00575a9c
  34ac0c: 60 5a 57 00  	.word	0x00575a60
  34ac10: e4 59 57 00  	.word	0x005759e4
  34ac14: d4 58 57 00  	.word	0x005758d4
