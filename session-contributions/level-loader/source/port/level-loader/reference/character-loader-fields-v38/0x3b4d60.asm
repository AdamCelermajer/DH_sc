
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003b4d60 <Character::InitPost()>:
  3b4d60: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3b4d64: e59f5828     	ldr	r5, [pc, #0x828]        @ 0x3b5594 <Character::InitPost()+0x834>
  3b4d68: e59f6828     	ldr	r6, [pc, #0x828]        @ 0x3b5598 <Character::InitPost()+0x838>
  3b4d6c: e3013394     	movw	r3, #0x1394
  3b4d70: e08f5005     	add	r5, pc, r5
  3b4d74: e7952006     	ldr	r2, [r5, r6]
  3b4d78: e7d07003     	ldrb	r7, [r0, r3]
  3b4d7c: e24dd0fc     	sub	sp, sp, #252
  3b4d80: e5922000     	ldr	r2, [r2]
  3b4d84: e3570000     	cmp	r7, #0
  3b4d88: e1a04000     	mov	r4, r0
  3b4d8c: e58d20f4     	str	r2, [sp, #0xf4]
  3b4d90: 0a000006     	beq	0x3b4db0 <Character::InitPost()+0x50> @ imm = #0x18
  3b4d94: e7953006     	ldr	r3, [r5, r6]
  3b4d98: e59d20f4     	ldr	r2, [sp, #0xf4]
  3b4d9c: e5933000     	ldr	r3, [r3]
  3b4da0: e1520003     	cmp	r2, r3
  3b4da4: 1a0001f9     	bne	0x3b5590 <Character::InitPost()+0x830> @ imm = #0x7e4
  3b4da8: e28dd0fc     	add	sp, sp, #252
  3b4dac: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3b4db0: e3a02001     	mov	r2, #1
  3b4db4: e7c02003     	strb	r2, [r0, r3]
  3b4db8: ebff5be9     	bl	0x38bd64 <GameObject::CheckSpawnProbability()> @ imm = #-0x2905c
  3b4dbc: e5943274     	ldr	r3, [r4, #0x274]
  3b4dc0: e1500003     	cmp	r0, r3
  3b4dc4: aafffff2     	bge	0x3b4d94 <Character::InitPost()+0x34> @ imm = #-0x38
  3b4dc8: e59fb7cc     	ldr	r11, [pc, #0x7cc]       @ 0x3b559c <Character::InitPost()+0x83c>
  3b4dcc: e28d80dc     	add	r8, sp, #220
  3b4dd0: e795a00b     	ldr	r10, [r5, r11]
  3b4dd4: e1a0000a     	mov	r0, r10
  3b4dd8: ebfe0aaa     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0x7d558
  3b4ddc: e59f17bc     	ldr	r1, [pc, #0x7bc]        @ 0x3b55a0 <Character::InitPost()+0x840>
  3b4de0: e28d2048     	add	r2, sp, #72
  3b4de4: e1a00008     	mov	r0, r8
  3b4de8: e08f1001     	add	r1, pc, r1
  3b4dec: ebfd7cbe     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xa0d08
  3b4df0: e1a01008     	mov	r1, r8
  3b4df4: e1a0000a     	mov	r0, r10
  3b4df8: ebfe0b22     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0x7d378
  3b4dfc: e1a00008     	mov	r0, r8
  3b4e00: ebfd8d13     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x9cbb4
  3b4e04: e30133fc     	movw	r3, #0x13fc
  3b4e08: e7942003     	ldr	r2, [r4, r3]
  3b4e0c: e30133f8     	movw	r3, #0x13f8
  3b4e10: e7943003     	ldr	r3, [r4, r3]
  3b4e14: e1530002     	cmp	r3, r2
  3b4e18: 0a000013     	beq	0x3b4e6c <Character::InitPost()+0x10c> @ imm = #0x4c
  3b4e1c: e59f1780     	ldr	r1, [pc, #0x780]        @ 0x3b55a4 <Character::InitPost()+0x844>
  3b4e20: e28d8024     	add	r8, sp, #36
  3b4e24: e5943064     	ldr	r3, [r4, #0x64]
  3b4e28: e7951001     	ldr	r1, [r5, r1]
  3b4e2c: e1a00008     	mov	r0, r8
  3b4e30: e5911038     	ldr	r1, [r1, #0x38]
  3b4e34: e58d7000     	str	r7, [sp]
  3b4e38: e58d7004     	str	r7, [sp, #0x4]
  3b4e3c: ebfe5797     	bl	0x34aca0 <ObjectManager::GetObjectByName(char const*, int, bool, char const*)> @ imm = #-0x6a1a4
  3b4e40: e1a00008     	mov	r0, r8
  3b4e44: e1a01007     	mov	r1, r7
  3b4e48: ebfe2bdc     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0x75090
  3b4e4c: e3500000     	cmp	r0, #0
  3b4e50: 0a000005     	beq	0x3b4e6c <Character::InitPost()+0x10c> @ imm = #0x14
  3b4e54: e1a00008     	mov	r0, r8
  3b4e58: ebfe2c21     	bl	0x33fee4 <ObjectHandle::operator GameObject*()> @ imm = #-0x74f7c
  3b4e5c: e2501000     	subs	r1, r0, #0
  3b4e60: 0a000001     	beq	0x3b4e6c <Character::InitPost()+0x10c> @ imm = #0x4
  3b4e64: e5940378     	ldr	r0, [r4, #0x378]
  3b4e68: eb0141b4     	bl	0x405540 <v2Controller::Cmd_MoveTo(GameObject*)> @ imm = #0x506d0
  3b4e6c: e1a00004     	mov	r0, r4
  3b4e70: e30173c8     	movw	r7, #0x13c8
  3b4e74: ebfffbaf     	bl	0x3b3d38 <Character::SafeGetCharPropsId()> @ imm = #-0x1144
  3b4e78: e19430f7     	ldrsh	r3, [r4, r7]
  3b4e7c: e3730001     	cmn	r3, #1
  3b4e80: 030f1fff     	movweq	r1, #0xffff
  3b4e84: 0a00000e     	beq	0x3b4ec4 <Character::InitPost()+0x164> @ imm = #0x38
  3b4e88: e795a00b     	ldr	r10, [r5, r11]
  3b4e8c: e28d80c4     	add	r8, sp, #196
  3b4e90: e1a0000a     	mov	r0, r10
  3b4e94: ebfe0a7b     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0x7d614
  3b4e98: e59f1708     	ldr	r1, [pc, #0x708]        @ 0x3b55a8 <Character::InitPost()+0x848>
  3b4e9c: e28d2044     	add	r2, sp, #68
  3b4ea0: e1a00008     	mov	r0, r8
  3b4ea4: e08f1001     	add	r1, pc, r1
  3b4ea8: ebfd7c8f     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xa0dc4
  3b4eac: e1a01008     	mov	r1, r8
  3b4eb0: e1a0000a     	mov	r0, r10
  3b4eb4: ebfe0af3     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0x7d434
  3b4eb8: e1a00008     	mov	r0, r8
  3b4ebc: ebfd8ce4     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x9cc70
  3b4ec0: e19410b7     	ldrh	r1, [r4, r7]
  3b4ec4: e2847e56     	add	r7, r4, #1376
  3b4ec8: e6bf1071     	sxth	r1, r1
  3b4ecc: e1a00007     	mov	r0, r7
  3b4ed0: eb00a8f3     	bl	0x3df2a4 <CharProperties::LoadBaseProperties(int)> @ imm = #0x2a3cc
  3b4ed4: e1a00007     	mov	r0, r7
  3b4ed8: e3a01001     	mov	r1, #1
  3b4edc: eb00ae4b     	bl	0x3e0810 <CharProperties::RecalcProperties(bool)> @ imm = #0x2b92c
  3b4ee0: e1a00004     	mov	r0, r4
  3b4ee4: ebffc17a     	bl	0x3a54d4 <Character::GetCharModelName() const> @ imm = #-0xfa18
  3b4ee8: e2508000     	subs	r8, r0, #0
  3b4eec: 0a000004     	beq	0x3b4f04 <Character::InitPost()+0x1a4> @ imm = #0x10
  3b4ef0: ebfd63d7     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0xa70a4
  3b4ef4: e1a01008     	mov	r1, r8
  3b4ef8: e0882000     	add	r2, r8, r0
  3b4efc: e2840e29     	add	r0, r4, #656
  3b4f00: ebfd6eb6     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0xa4528
  3b4f04: e795a00b     	ldr	r10, [r5, r11]
  3b4f08: e28d80ac     	add	r8, sp, #172
  3b4f0c: e1a0000a     	mov	r0, r10
  3b4f10: ebfe0a5c     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0x7d690
  3b4f14: e59f1690     	ldr	r1, [pc, #0x690]        @ 0x3b55ac <Character::InitPost()+0x84c>
  3b4f18: e28d2040     	add	r2, sp, #64
  3b4f1c: e1a00008     	mov	r0, r8
  3b4f20: e08f1001     	add	r1, pc, r1
  3b4f24: ebfd7c70     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xa0e40
  3b4f28: e1a01008     	mov	r1, r8
  3b4f2c: e1a0000a     	mov	r0, r10
  3b4f30: ebfe0ad4     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0x7d4b0
  3b4f34: e1a00008     	mov	r0, r8
  3b4f38: ebfd8cc5     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x9ccec
  3b4f3c: e594059c     	ldr	r0, [r4, #0x59c]
  3b4f40: ebfd6687     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0xa65e4
  3b4f44: e30714bc     	movw	r1, #0x74bc
  3b4f48: e3431c13     	movt	r1, #0x3c13
  3b4f4c: ebfd6786     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xa61e8
  3b4f50: e5840120     	str	r0, [r4, #0x120]
  3b4f54: e59405a0     	ldr	r0, [r4, #0x5a0]
  3b4f58: ebfd6681     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0xa65fc
  3b4f5c: e30714bc     	movw	r1, #0x74bc
  3b4f60: e3431c13     	movt	r1, #0x3c13
  3b4f64: ebfd6780     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xa6200
  3b4f68: e5840124     	str	r0, [r4, #0x124]
  3b4f6c: e59405a4     	ldr	r0, [r4, #0x5a4]
  3b4f70: ebfd667b     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0xa6614
  3b4f74: e30d170a     	movw	r1, #0xd70a
  3b4f78: e3431c23     	movt	r1, #0x3c23
  3b4f7c: ebfd677a     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xa6218
  3b4f80: e5840128     	str	r0, [r4, #0x128]
  3b4f84: e1a00004     	mov	r0, r4
  3b4f88: ebff5bb3     	bl	0x38be5c <GameObject::InitPost()> @ imm = #-0x29134
  3b4f8c: e1a00004     	mov	r0, r4
  3b4f90: ebff56f2     	bl	0x38ab60 <GameObject::MeetCondition() const> @ imm = #-0x2a438
  3b4f94: e2501000     	subs	r1, r0, #0
  3b4f98: 0a00013a     	beq	0x3b5488 <Character::InitPost()+0x728> @ imm = #0x4e8
  3b4f9c: e3a01002     	mov	r1, #2
  3b4fa0: e1a00004     	mov	r0, r4
  3b4fa4: eb001d49     	bl	0x3bc4d0 <Character::SG_Load(int)> @ imm = #0x7524
  3b4fa8: e59f3600     	ldr	r3, [pc, #0x600]        @ 0x3b55b0 <Character::InitPost()+0x850>
  3b4fac: e1a00004     	mov	r0, r4
  3b4fb0: e7953003     	ldr	r3, [r5, r3]
  3b4fb4: e5938000     	ldr	r8, [r3]
  3b4fb8: ebffb80b     	bl	0x3a2fec <Character::GetCharAIId() const> @ imm = #-0x11fd4
  3b4fbc: e3a03044     	mov	r3, #68
  3b4fc0: e0288093     	mla	r8, r3, r0, r8
  3b4fc4: e5943000     	ldr	r3, [r4]
  3b4fc8: e1a00004     	mov	r0, r4
  3b4fcc: e1a0e00f     	mov	lr, pc
  3b4fd0: e593f028     	ldr	pc, [r3, #0x28]
  3b4fd4: e3500000     	cmp	r0, #0
  3b4fd8: 1a000005     	bne	0x3b4ff4 <Character::InitPost()+0x294> @ imm = #0x14
  3b4fdc: e5d83010     	ldrb	r3, [r8, #0x10]
  3b4fe0: e3530000     	cmp	r3, #0
  3b4fe4: 0a000002     	beq	0x3b4ff4 <Character::InitPost()+0x294> @ imm = #0x8
  3b4fe8: e3a03001     	mov	r3, #1
  3b4fec: e5c433ec     	strb	r3, [r4, #0x3ec]
  3b4ff0: ea000007     	b	0x3b5014 <Character::InitPost()+0x2b4> @ imm = #0x1c
  3b4ff4: e5943000     	ldr	r3, [r4]
  3b4ff8: e1a00004     	mov	r0, r4
  3b4ffc: e1a0e00f     	mov	lr, pc
  3b5000: e593f028     	ldr	pc, [r3, #0x28]
  3b5004: e3500000     	cmp	r0, #0
  3b5008: 1a000125     	bne	0x3b54a4 <Character::InitPost()+0x744> @ imm = #0x494
  3b500c: e2840ff2     	add	r0, r4, #968
  3b5010: eb006876     	bl	0x3cf1f0 <CharAI::LoadScriptProcess()> @ imm = #0x1a1d8
  3b5014: e3013488     	movw	r3, #0x1488
  3b5018: e59f2594     	ldr	r2, [pc, #0x594]        @ 0x3b55b4 <Character::InitPost()+0x854>
  3b501c: e7941003     	ldr	r1, [r4, r3]
  3b5020: e5983030     	ldr	r3, [r8, #0x30]
  3b5024: e58d200c     	str	r2, [sp, #0xc]
  3b5028: e1a02004     	mov	r2, r4
  3b502c: e0811003     	add	r1, r1, r3
  3b5030: e59d300c     	ldr	r3, [sp, #0xc]
  3b5034: e59f957c     	ldr	r9, [pc, #0x57c]        @ 0x3b55b8 <Character::InitPost()+0x858>
  3b5038: e28da094     	add	r10, sp, #148
  3b503c: e7950003     	ldr	r0, [r5, r3]
  3b5040: eb0380fa     	bl	0x495430 <VisualFXManager::GrabAnimFX(int, GameObject*)> @ imm = #0xe03e8
  3b5044: e3013484     	movw	r3, #0x1484
  3b5048: e7840003     	str	r0, [r4, r3]
  3b504c: e1a00004     	mov	r0, r4
  3b5050: ebfffdb8     	bl	0x3b4738 <Character::RegisterCharacterFXTable()> @ imm = #-0x920
  3b5054: e795800b     	ldr	r8, [r5, r11]
  3b5058: e08f9009     	add	r9, pc, r9
  3b505c: e1a00008     	mov	r0, r8
  3b5060: ebfe0a08     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0x7d7e0
  3b5064: e28d203c     	add	r2, sp, #60
  3b5068: e1a0000a     	mov	r0, r10
  3b506c: e1a01009     	mov	r1, r9
  3b5070: ebfd7c1d     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xa0f8c
  3b5074: e1a0100a     	mov	r1, r10
  3b5078: e1a00008     	mov	r0, r8
  3b507c: ebfe0a81     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0x7d5fc
  3b5080: e1a0000a     	mov	r0, r10
  3b5084: ebfd8c72     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x9ce38
  3b5088: e2840e49     	add	r0, r4, #1168
  3b508c: e280000c     	add	r0, r0, #12
  3b5090: eb0053ad     	bl	0x3c9f4c <CharAnimator::SetAnimationSet()> @ imm = #0x14eb4
  3b5094: e28da07c     	add	r10, sp, #124
  3b5098: e1a00008     	mov	r0, r8
  3b509c: ebfe09f9     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0x7d81c
  3b50a0: e28d2038     	add	r2, sp, #56
  3b50a4: e1a0000a     	mov	r0, r10
  3b50a8: e1a01009     	mov	r1, r9
  3b50ac: ebfd7c0e     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xa0fc8
  3b50b0: e1a0100a     	mov	r1, r10
  3b50b4: e1a00008     	mov	r0, r8
  3b50b8: ebfe0a72     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0x7d638
  3b50bc: e1a0000a     	mov	r0, r10
  3b50c0: ebfd8c63     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x9ce74
  3b50c4: e1a00004     	mov	r0, r4
  3b50c8: ebfffa8c     	bl	0x3b3b00 <Character::_InitSounds()> @ imm = #-0x15d0
  3b50cc: e28da064     	add	r10, sp, #100
  3b50d0: e1a00008     	mov	r0, r8
  3b50d4: ebfe09eb     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0x7d854
  3b50d8: e28d2034     	add	r2, sp, #52
  3b50dc: e1a01009     	mov	r1, r9
  3b50e0: e1a0000a     	mov	r0, r10
  3b50e4: ebfd7c00     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xa1000
  3b50e8: e1a0100a     	mov	r1, r10
  3b50ec: e1a00008     	mov	r0, r8
  3b50f0: ebfe0a64     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0x7d670
  3b50f4: e1a0000a     	mov	r0, r10
  3b50f8: ebfd8c55     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x9ceac
  3b50fc: e5943000     	ldr	r3, [r4]
  3b5100: e1a00004     	mov	r0, r4
  3b5104: e1a0e00f     	mov	lr, pc
  3b5108: e593f028     	ldr	pc, [r3, #0x28]
  3b510c: e3500000     	cmp	r0, #0
  3b5110: 0a00009d     	beq	0x3b538c <Character::InitPost()+0x62c> @ imm = #0x274
  3b5114: e59432d8     	ldr	r3, [r4, #0x2d8]
  3b5118: e3530000     	cmp	r3, #0
  3b511c: 0a000002     	beq	0x3b512c <Character::InitPost()+0x3cc> @ imm = #0x8
  3b5120: e5930008     	ldr	r0, [r3, #0x8]
  3b5124: e3a01000     	mov	r1, #0
  3b5128: eb07881b     	bl	0x59719c <glitch::scene::ISceneNode::setAutomaticCulling(glitch::scene::E_CULLING_TYPE)> @ imm = #0x1e206c
  3b512c: e59f2470     	ldr	r2, [pc, #0x470]        @ 0x3b55a4 <Character::InitPost()+0x844>
  3b5130: e1a00004     	mov	r0, r4
  3b5134: e3a01004     	mov	r1, #4
  3b5138: e58d2010     	str	r2, [sp, #0x10]
  3b513c: eb001ce3     	bl	0x3bc4d0 <Character::SG_Load(int)> @ imm = #0x738c
  3b5140: e59d3010     	ldr	r3, [sp, #0x10]
  3b5144: e7950003     	ldr	r0, [r5, r3]
  3b5148: ebfda911     	bl	0x31f594 <Application::GetCurrentLevel() const> @ imm = #-0x95bbc
  3b514c: e3500000     	cmp	r0, #0
  3b5150: 0a000002     	beq	0x3b5160 <Character::InitPost()+0x400> @ imm = #0x8
  3b5154: e5901118     	ldr	r1, [r0, #0x118]
  3b5158: e1a00004     	mov	r0, r4
  3b515c: eb0019fb     	bl	0x3bb950 <Character::SG_SetGameDifficulty(int)> @ imm = #0x67ec
  3b5160: e59d2010     	ldr	r2, [sp, #0x10]
  3b5164: e1a01004     	mov	r1, r4
  3b5168: e7953002     	ldr	r3, [r5, r2]
  3b516c: e5930040     	ldr	r0, [r3, #0x40]
  3b5170: ebfee7a1     	bl	0x36effc <PlayerManager::IsLocalPlayer(Character const*)> @ imm = #-0x4617c
  3b5174: e3500000     	cmp	r0, #0
  3b5178: 1a0000d7     	bne	0x3b54dc <Character::InitPost()+0x77c> @ imm = #0x35c
  3b517c: e30133c8     	movw	r3, #0x13c8
  3b5180: e19410f3     	ldrsh	r1, [r4, r3]
  3b5184: e1a00007     	mov	r0, r7
  3b5188: eb00a845     	bl	0x3df2a4 <CharProperties::LoadBaseProperties(int)> @ imm = #0x2a114
  3b518c: e1a00007     	mov	r0, r7
  3b5190: eb00a8ba     	bl	0x3df480 <CharProperties::LoadGearsProperties()> @ imm = #0x2a2e8
  3b5194: e1a00007     	mov	r0, r7
  3b5198: e3a01001     	mov	r1, #1
  3b519c: eb00ad9b     	bl	0x3e0810 <CharProperties::RecalcProperties(bool)> @ imm = #0x2b66c
  3b51a0: e59d2010     	ldr	r2, [sp, #0x10]
  3b51a4: e1a01004     	mov	r1, r4
  3b51a8: e7953002     	ldr	r3, [r5, r2]
  3b51ac: e5930040     	ldr	r0, [r3, #0x40]
  3b51b0: ebfee791     	bl	0x36effc <PlayerManager::IsLocalPlayer(Character const*)> @ imm = #-0x461bc
  3b51b4: e3500000     	cmp	r0, #0
  3b51b8: 1a0000c4     	bne	0x3b54d0 <Character::InitPost()+0x770> @ imm = #0x310
  3b51bc: e30c39ff     	movw	r3, #0xc9ff
  3b51c0: e3433b9a     	movt	r3, #0x3b9a
  3b51c4: e58433a4     	str	r3, [r4, #0x3a4]
  3b51c8: e3a02000     	mov	r2, #0
  3b51cc: e1a00007     	mov	r0, r7
  3b51d0: e3a010c2     	mov	r1, #194
  3b51d4: eb00a941     	bl	0x3df6e0 <CharProperties::PROPS_GetInt(int, bool) const> @ imm = #0x2a504
  3b51d8: e59d200c     	ldr	r2, [sp, #0xc]
  3b51dc: e1c00fc0     	bic	r0, r0, r0, asr #31
  3b51e0: e5c403a8     	strb	r0, [r4, #0x3a8]
  3b51e4: e7953002     	ldr	r3, [r5, r2]
  3b51e8: e593201c     	ldr	r2, [r3, #0x1c]
  3b51ec: e5933020     	ldr	r3, [r3, #0x20]
  3b51f0: e0623003     	rsb	r3, r2, r3
  3b51f4: e1a031c3     	asr	r3, r3, #3
  3b51f8: e0832103     	add	r2, r3, r3, lsl #2
  3b51fc: e0822202     	add	r2, r2, r2, lsl #4
  3b5200: e0822402     	add	r2, r2, r2, lsl #8
  3b5204: e0822802     	add	r2, r2, r2, lsl #16
  3b5208: e0833082     	add	r3, r3, r2, lsl #1
  3b520c: e3530000     	cmp	r3, #0
  3b5210: 0a00005d     	beq	0x3b538c <Character::InitPost()+0x62c> @ imm = #0x174
  3b5214: e3a00024     	mov	r0, #36
  3b5218: e3a01000     	mov	r1, #0
  3b521c: ebfd6cd2     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0xa4cb8
  3b5220: e3013494     	movw	r3, #0x1494
  3b5224: e7840003     	str	r0, [r4, r3]
  3b5228: e59f338c     	ldr	r3, [pc, #0x38c]        @ 0x3b55bc <Character::InitPost()+0x85c>
  3b522c: e1a09000     	mov	r9, r0
  3b5230: e7953003     	ldr	r3, [r5, r3]
  3b5234: e593a000     	ldr	r10, [r3]
  3b5238: e35a0000     	cmp	r10, #0
  3b523c: 0a0000b2     	beq	0x3b550c <Character::InitPost()+0x7ac> @ imm = #0x2c8
  3b5240: e59f3378     	ldr	r3, [pc, #0x378]        @ 0x3b55c0 <Character::InitPost()+0x860>
  3b5244: e59f2378     	ldr	r2, [pc, #0x378]        @ 0x3b55c4 <Character::InitPost()+0x864>
  3b5248: e58d7018     	str	r7, [sp, #0x18]
  3b524c: e7953003     	ldr	r3, [r5, r3]
  3b5250: e08f2002     	add	r2, pc, r2
  3b5254: e3a08000     	mov	r8, #0
  3b5258: e5933000     	ldr	r3, [r3]
  3b525c: e58d0014     	str	r0, [sp, #0x14]
  3b5260: e1a09002     	mov	r9, r2
  3b5264: e1a07003     	mov	r7, r3
  3b5268: ea000002     	b	0x3b5278 <Character::InitPost()+0x518> @ imm = #0x8
  3b526c: e2888001     	add	r8, r8, #1
  3b5270: e158000a     	cmp	r8, r10
  3b5274: 0a0000a2     	beq	0x3b5504 <Character::InitPost()+0x7a4> @ imm = #0x288
  3b5278: e1a00009     	mov	r0, r9
  3b527c: e7971108     	ldr	r1, [r7, r8, lsl #2]
  3b5280: ebfd6425     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xa6f6c
  3b5284: e3500000     	cmp	r0, #0
  3b5288: 1afffff7     	bne	0x3b526c <Character::InitPost()+0x50c> @ imm = #-0x24
  3b528c: e59d9014     	ldr	r9, [sp, #0x14]
  3b5290: e59d7018     	ldr	r7, [sp, #0x18]
  3b5294: e1a01008     	mov	r1, r8
  3b5298: e59d300c     	ldr	r3, [sp, #0xc]
  3b529c: e58d7014     	str	r7, [sp, #0x14]
  3b52a0: e58db018     	str	r11, [sp, #0x18]
  3b52a4: e7952003     	ldr	r2, [r5, r3]
  3b52a8: e59f3318     	ldr	r3, [pc, #0x318]        @ 0x3b55c8 <Character::InitPost()+0x868>
  3b52ac: e58d601c     	str	r6, [sp, #0x1c]
  3b52b0: e3a08000     	mov	r8, #0
  3b52b4: e301a494     	movw	r10, #0x1494
  3b52b8: e1a07001     	mov	r7, r1
  3b52bc: e1a06002     	mov	r6, r2
  3b52c0: e1a0b003     	mov	r11, r3
  3b52c4: ea000000     	b	0x3b52cc <Character::InitPost()+0x56c> @ imm = #0x0
  3b52c8: e794900a     	ldr	r9, [r4, r10]
  3b52cc: e1a00006     	mov	r0, r6
  3b52d0: e0871008     	add	r1, r7, r8
  3b52d4: e3a02000     	mov	r2, #0
  3b52d8: eb038054     	bl	0x495430 <VisualFXManager::GrabAnimFX(int, GameObject*)> @ imm = #0xe0150
  3b52dc: e7890108     	str	r0, [r9, r8, lsl #2]
  3b52e0: e794300a     	ldr	r3, [r4, r10]
  3b52e4: e7933108     	ldr	r3, [r3, r8, lsl #2]
  3b52e8: e3530000     	cmp	r3, #0
  3b52ec: 0a000017     	beq	0x3b5350 <Character::InitPost()+0x5f0> @ imm = #0x5c
  3b52f0: e795200b     	ldr	r2, [r5, r11]
  3b52f4: e1a00003     	mov	r0, r3
  3b52f8: e3a01000     	mov	r1, #0
  3b52fc: e592e000     	ldr	lr, [r2]
  3b5300: e592c004     	ldr	r12, [r2, #0x4]
  3b5304: e5922008     	ldr	r2, [r2, #0x8]
  3b5308: e583e034     	str	lr, [r3, #0x34]
  3b530c: e583c038     	str	r12, [r3, #0x38]
  3b5310: e583203c     	str	r2, [r3, #0x3c]
  3b5314: eb0375e1     	bl	0x492aa0 <AnimatedFX::SyncIrrData(bool)> @ imm = #0xdd784
  3b5318: e794300a     	ldr	r3, [r4, r10]
  3b531c: e3a01000     	mov	r1, #0
  3b5320: e7930108     	ldr	r0, [r3, r8, lsl #2]
  3b5324: eb0376f1     	bl	0x492ef0 <AnimatedFX::SetVisible(bool)> @ imm = #0xddbc4
  3b5328: e794300a     	ldr	r3, [r4, r10]
  3b532c: e7930108     	ldr	r0, [r3, r8, lsl #2]
  3b5330: eb0374d1     	bl	0x49267c <AnimatedFX::GetAnimator()> @ imm = #0xdd344
  3b5334: e5903000     	ldr	r3, [r0]
  3b5338: e1a0e00f     	mov	lr, pc
  3b533c: e593f044     	ldr	pc, [r3, #0x44]
  3b5340: e3a01001     	mov	r1, #1
  3b5344: e5903000     	ldr	r3, [r0]
  3b5348: e1a0e00f     	mov	lr, pc
  3b534c: e593f040     	ldr	pc, [r3, #0x40]
  3b5350: e2888001     	add	r8, r8, #1
  3b5354: e3580009     	cmp	r8, #9
  3b5358: 1affffda     	bne	0x3b52c8 <Character::InitPost()+0x568> @ imm = #-0x98
  3b535c: e59d2010     	ldr	r2, [sp, #0x10]
  3b5360: e1a01004     	mov	r1, r4
  3b5364: e59d7014     	ldr	r7, [sp, #0x14]
  3b5368: e7953002     	ldr	r3, [r5, r2]
  3b536c: e59db018     	ldr	r11, [sp, #0x18]
  3b5370: e59d601c     	ldr	r6, [sp, #0x1c]
  3b5374: e5930040     	ldr	r0, [r3, #0x40]
  3b5378: ebfee71f     	bl	0x36effc <PlayerManager::IsLocalPlayer(Character const*)> @ imm = #-0x46384
  3b537c: e3500000     	cmp	r0, #0
  3b5380: 1a000063     	bne	0x3b5514 <Character::InitPost()+0x7b4> @ imm = #0x18c
  3b5384: e1a00004     	mov	r0, r4
  3b5388: ebffbb84     	bl	0x3a41a0 <Character::AddMultiplayerHighlight()> @ imm = #-0x111f0
  3b538c: e2848eff     	add	r8, r4, #4080
  3b5390: e2888004     	add	r8, r8, #4
  3b5394: e1a01008     	mov	r1, r8
  3b5398: e3a020d2     	mov	r2, #210
  3b539c: e1a00007     	mov	r0, r7
  3b53a0: eb00a683     	bl	0x3dedb4 <CharProperties::_GetProperty(Structs::CharacterProperties const&, int) const> @ imm = #0x29a0c
  3b53a4: ebfd656e     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0xa6a48
  3b53a8: e3a03d51     	mov	r3, #5184
  3b53ac: e7840003     	str	r0, [r4, r3]
  3b53b0: e3a020d3     	mov	r2, #211
  3b53b4: e1a01008     	mov	r1, r8
  3b53b8: e1a00007     	mov	r0, r7
  3b53bc: eb00a67c     	bl	0x3dedb4 <CharProperties::_GetProperty(Structs::CharacterProperties const&, int) const> @ imm = #0x299f0
  3b53c0: ebfd6567     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0xa6a64
  3b53c4: e3013444     	movw	r3, #0x1444
  3b53c8: e7840003     	str	r0, [r4, r3]
  3b53cc: e2841e16     	add	r1, r4, #352
  3b53d0: e1a00004     	mov	r0, r4
  3b53d4: ebffc146     	bl	0x3a58f4 <Character::SetInitialPosition(Point3D<float> const&)> @ imm = #-0xfae8
  3b53d8: e2841d51     	add	r1, r4, #5184
  3b53dc: e1a00004     	mov	r0, r4
  3b53e0: e2811010     	add	r1, r1, #16
  3b53e4: e3a02001     	mov	r2, #1
  3b53e8: ebff7a71     	bl	0x393db4 <GameObject::SetPosition(Point3D<float> const&, bool)> @ imm = #-0x2163c
  3b53ec: e594c16c     	ldr	r12, [r4, #0x16c]
  3b53f0: e59402d8     	ldr	r0, [r4, #0x2d8]
  3b53f4: e5941170     	ldr	r1, [r4, #0x170]
  3b53f8: e5942174     	ldr	r2, [r4, #0x174]
  3b53fc: e301345c     	movw	r3, #0x145c
  3b5400: e784c003     	str	r12, [r4, r3]
  3b5404: e3013460     	movw	r3, #0x1460
  3b5408: e7841003     	str	r1, [r4, r3]
  3b540c: e3500000     	cmp	r0, #0
  3b5410: e3013464     	movw	r3, #0x1464
  3b5414: e7842003     	str	r2, [r4, r3]
  3b5418: 0a000000     	beq	0x3b5420 <Character::InitPost()+0x6c0> @ imm = #0x0
  3b541c: eb02ed8c     	bl	0x470a54 <VisualObject::ApplyMeshBox()> @ imm = #0xbb630
  3b5420: e3a01000     	mov	r1, #0
  3b5424: e3a02001     	mov	r2, #1
  3b5428: e1a00004     	mov	r0, r4
  3b542c: ebffc15e     	bl	0x3a59ac <Character::Revive(GameObject*, bool)> @ imm = #-0xfa88
  3b5430: e1a00004     	mov	r0, r4
  3b5434: ebfff98d     	bl	0x3b3a70 <Character::_InitHpMp()> @ imm = #-0x19cc
  3b5438: e5d413ec     	ldrb	r1, [r4, #0x3ec]
  3b543c: e3510000     	cmp	r1, #0
  3b5440: 0a00001f     	beq	0x3b54c4 <Character::InitPost()+0x764> @ imm = #0x7c
  3b5444: e1a00004     	mov	r0, r4
  3b5448: eb0078e0     	bl	0x3d37d0 <CharAI::AddToGroup(Character*)> @ imm = #0x1e380
  3b544c: e795700b     	ldr	r7, [r5, r11]
  3b5450: e28d404c     	add	r4, sp, #76
  3b5454: e1a00007     	mov	r0, r7
  3b5458: ebfe090a     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0x7dbd8
  3b545c: e59f1168     	ldr	r1, [pc, #0x168]        @ 0x3b55cc <Character::InitPost()+0x86c>
  3b5460: e28d2030     	add	r2, sp, #48
  3b5464: e1a00004     	mov	r0, r4
  3b5468: e08f1001     	add	r1, pc, r1
  3b546c: ebfd7b1e     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xa1388
  3b5470: e1a00007     	mov	r0, r7
  3b5474: e1a01004     	mov	r1, r4
  3b5478: ebfe0982     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0x7d9f8
  3b547c: e1a00004     	mov	r0, r4
  3b5480: ebfd8b73     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x9d234
  3b5484: eafffe42     	b	0x3b4d94 <Character::InitPost()+0x34> @ imm = #-0x6f8
  3b5488: e1a00004     	mov	r0, r4
  3b548c: e5943000     	ldr	r3, [r4]
  3b5490: e1a0e00f     	mov	lr, pc
  3b5494: e593f040     	ldr	pc, [r3, #0x40]
  3b5498: e1a00004     	mov	r0, r4
  3b549c: ebfe2244     	bl	0x33ddb4 <ObjectBase::Delete()> @ imm = #-0x776f0
  3b54a0: eafffe3b     	b	0x3b4d94 <Character::InitPost()+0x34> @ imm = #-0x714
  3b54a4: e59f30f8     	ldr	r3, [pc, #0xf8]         @ 0x3b55a4 <Character::InitPost()+0x844>
  3b54a8: e1a01004     	mov	r1, r4
  3b54ac: e7953003     	ldr	r3, [r5, r3]
  3b54b0: e5930040     	ldr	r0, [r3, #0x40]
  3b54b4: ebfee6d0     	bl	0x36effc <PlayerManager::IsLocalPlayer(Character const*)> @ imm = #-0x464c0
  3b54b8: e3500000     	cmp	r0, #0
  3b54bc: 1afffed2     	bne	0x3b500c <Character::InitPost()+0x2ac> @ imm = #-0x4b8
  3b54c0: eafffec8     	b	0x3b4fe8 <Character::InitPost()+0x288> @ imm = #-0x4e0
  3b54c4: e2840ff2     	add	r0, r4, #968
  3b54c8: eb0064bc     	bl	0x3ce7c0 <CharAI::InitScriptProcess(bool)> @ imm = #0x192f0
  3b54cc: eaffffdc     	b	0x3b5444 <Character::InitPost()+0x6e4> @ imm = #-0x90
  3b54d0: e1a00004     	mov	r0, r4
  3b54d4: ebfff96d     	bl	0x3b3a90 <Character::_InitSkillsSlots()> @ imm = #-0x1a4c
  3b54d8: eaffff37     	b	0x3b51bc <Character::InitPost()+0x45c> @ imm = #-0x324
  3b54dc: e1a00004     	mov	r0, r4
  3b54e0: ebfff91d     	bl	0x3b395c <Character::_InitEquipment()> @ imm = #-0x1b8c
  3b54e4: e1a00007     	mov	r0, r7
  3b54e8: eb00a6af     	bl	0x3defac <CharProperties::ResetGearsProperties()> @ imm = #0x29abc
  3b54ec: e30134e8     	movw	r3, #0x14e8
  3b54f0: e7940003     	ldr	r0, [r4, r3]
  3b54f4: e3500000     	cmp	r0, #0
  3b54f8: 0affff1f     	beq	0x3b517c <Character::InitPost()+0x41c> @ imm = #-0x384
  3b54fc: eb02c939     	bl	0x4679e8 <PlayerSavegame::SG_TryQuestSync()> @ imm = #0xb24e4
  3b5500: eaffff1d     	b	0x3b517c <Character::InitPost()+0x41c> @ imm = #-0x38c
  3b5504: e59d9014     	ldr	r9, [sp, #0x14]
  3b5508: e59d7018     	ldr	r7, [sp, #0x18]
  3b550c: e3e01000     	mvn	r1, #0
  3b5510: eaffff60     	b	0x3b5298 <Character::InitPost()+0x538> @ imm = #-0x280
  3b5514: e59d300c     	ldr	r3, [sp, #0xc]
  3b5518: e3a02000     	mov	r2, #0
  3b551c: e301849c     	movw	r8, #0x149c
  3b5520: e7950003     	ldr	r0, [r5, r3]
  3b5524: e59f30a4     	ldr	r3, [pc, #0xa4]         @ 0x3b55d0 <Character::InitPost()+0x870>
  3b5528: e7953003     	ldr	r3, [r5, r3]
  3b552c: e5933000     	ldr	r3, [r3]
  3b5530: e5931088     	ldr	r1, [r3, #0x88]
  3b5534: eb037fbd     	bl	0x495430 <VisualFXManager::GrabAnimFX(int, GameObject*)> @ imm = #0xdfef4
  3b5538: e3500000     	cmp	r0, #0
  3b553c: e7840008     	str	r0, [r4, r8]
  3b5540: 0affff8f     	beq	0x3b5384 <Character::InitPost()+0x624> @ imm = #-0x1c4
  3b5544: e3a02000     	mov	r2, #0
  3b5548: e580203c     	str	r2, [r0, #0x3c]
  3b554c: e5802034     	str	r2, [r0, #0x34]
  3b5550: e5802038     	str	r2, [r0, #0x38]
  3b5554: e3a01000     	mov	r1, #0
  3b5558: eb037550     	bl	0x492aa0 <AnimatedFX::SyncIrrData(bool)> @ imm = #0xdd540
  3b555c: e3a01000     	mov	r1, #0
  3b5560: e7940008     	ldr	r0, [r4, r8]
  3b5564: eb037661     	bl	0x492ef0 <AnimatedFX::SetVisible(bool)> @ imm = #0xdd984
  3b5568: e7940008     	ldr	r0, [r4, r8]
  3b556c: eb037442     	bl	0x49267c <AnimatedFX::GetAnimator()> @ imm = #0xdd108
  3b5570: e5903000     	ldr	r3, [r0]
  3b5574: e1a0e00f     	mov	lr, pc
  3b5578: e593f044     	ldr	pc, [r3, #0x44]
  3b557c: e3a01001     	mov	r1, #1
  3b5580: e5903000     	ldr	r3, [r0]
  3b5584: e1a0e00f     	mov	lr, pc
  3b5588: e593f040     	ldr	pc, [r3, #0x40]
  3b558c: eaffff7c     	b	0x3b5384 <Character::InitPost()+0x624> @ imm = #-0x210
  3b5590: ebfd635e     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xa7288
  3b5594: 20 fd 5d 00  	.word	0x005dfd20
  3b5598: ac 40 00 00  	.word	0x000040ac
  3b559c: 84 08 00 00  	.word	0x00000884
  3b55a0: 28 f0 50 00  	.word	0x0050f028
  3b55a4: f4 37 00 00  	.word	0x000037f4
  3b55a8: 6c ef 50 00  	.word	0x0050ef6c
  3b55ac: f0 ee 50 00  	.word	0x0050eef0
  3b55b0: 58 07 00 00  	.word	0x00000758
  3b55b4: 08 1b 00 00  	.word	0x00001b08
  3b55b8: b8 ed 50 00  	.word	0x0050edb8
  3b55bc: c4 06 00 00  	.word	0x000006c4
  3b55c0: 94 12 00 00  	.word	0x00001294
  3b55c4: 60 ec 50 00  	.word	0x0050ec60
  3b55c8: 2c 3f 00 00  	.word	0x00003f2c
  3b55cc: a8 e9 50 00  	.word	0x0050e9a8
  3b55d0: c8 32 00 00  	.word	0x000032c8
