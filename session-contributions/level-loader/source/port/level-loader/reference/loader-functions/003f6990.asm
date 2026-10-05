
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f6990 <Level::_LoadProcess()>:
  3f6990: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3f6994: e59f5f84     	ldr	r5, [pc, #0xf84]        @ 0x3f7920 <Level::_LoadProcess()+0xf90>
  3f6998: e59f3f84     	ldr	r3, [pc, #0xf84]        @ 0x3f7924 <Level::_LoadProcess()+0xf94>
  3f699c: e59f6f84     	ldr	r6, [pc, #0xf84]        @ 0x3f7928 <Level::_LoadProcess()+0xf98>
  3f69a0: e08f5005     	add	r5, pc, r5
  3f69a4: e7952003     	ldr	r2, [r5, r3]
  3f69a8: e7953006     	ldr	r3, [r5, r6]
  3f69ac: e24dde4b     	sub	sp, sp, #1200
  3f69b0: e5d22000     	ldrb	r2, [r2]
  3f69b4: e5933000     	ldr	r3, [r3]
  3f69b8: e24dd00c     	sub	sp, sp, #12
  3f69bc: e3520000     	cmp	r2, #0
  3f69c0: e1a04000     	mov	r4, r0
  3f69c4: e58d34b4     	str	r3, [sp, #0x4b4]
  3f69c8: 1a000047     	bne	0x3f6aec <Level::_LoadProcess()+0x15c> @ imm = #0x11c
  3f69cc: e59f3f58     	ldr	r3, [pc, #0xf58]        @ 0x3f792c <Level::_LoadProcess()+0xf9c>
  3f69d0: e28d7e49     	add	r7, sp, #1168
  3f69d4: e287700c     	add	r7, r7, #12
  3f69d8: e7958003     	ldr	r8, [r5, r3]
  3f69dc: e1a00008     	mov	r0, r8
  3f69e0: ebfd03a8     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xbf160
  3f69e4: e59f1f44     	ldr	r1, [pc, #0xf44]        @ 0x3f7930 <Level::_LoadProcess()+0xfa0>
  3f69e8: e28d2f4e     	add	r2, sp, #312
  3f69ec: e1a00007     	mov	r0, r7
  3f69f0: e08f1001     	add	r1, pc, r1
  3f69f4: ebfc75bc     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe2910
  3f69f8: e1a00008     	mov	r0, r8
  3f69fc: e1a01007     	mov	r1, r7
  3f6a00: ebfd0420     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbef80
  3f6a04: e1a08000     	mov	r8, r0
  3f6a08: e1a00007     	mov	r0, r7
  3f6a0c: ebfc73e6     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe3068
  3f6a10: e3580000     	cmp	r8, #0
  3f6a14: 1a00002d     	bne	0x3f6ad0 <Level::_LoadProcess()+0x140> @ imm = #0xb4
  3f6a18: e59f0f14     	ldr	r0, [pc, #0xf14]        @ 0x3f7934 <Level::_LoadProcess()+0xfa4>
  3f6a1c: e5941130     	ldr	r1, [r4, #0x130]
  3f6a20: e08f0000     	add	r0, pc, r0
  3f6a24: ebfcb5ba     	bl	0x324114 <_DEBUG_OUT(char const*, ...)> @ imm = #-0xd2918
  3f6a28: e5943130     	ldr	r3, [r4, #0x130]
  3f6a2c: e3530025     	cmp	r3, #37
  3f6a30: 908ff103     	addls	pc, pc, r3, lsl #2
  3f6a34: ea0002c0     	b	0x3f753c <Level::_LoadProcess()+0xbac> @ imm = #0xb00
  3f6a38: ea0002a2     	b	0x3f74c8 <Level::_LoadProcess()+0xb38> @ imm = #0xa88
  3f6a3c: ea0002be     	b	0x3f753c <Level::_LoadProcess()+0xbac> @ imm = #0xaf8
  3f6a40: ea000285     	b	0x3f745c <Level::_LoadProcess()+0xacc> @ imm = #0xa14
  3f6a44: ea000272     	b	0x3f7414 <Level::_LoadProcess()+0xa84> @ imm = #0x9c8
  3f6a48: ea00025c     	b	0x3f73c0 <Level::_LoadProcess()+0xa30> @ imm = #0x970
  3f6a4c: ea00023e     	b	0x3f734c <Level::_LoadProcess()+0x9bc> @ imm = #0x8f8
  3f6a50: ea00021e     	b	0x3f72d0 <Level::_LoadProcess()+0x940> @ imm = #0x878
  3f6a54: ea0001ea     	b	0x3f7204 <Level::_LoadProcess()+0x874> @ imm = #0x7a8
  3f6a58: ea0001c5     	b	0x3f7174 <Level::_LoadProcess()+0x7e4> @ imm = #0x714
  3f6a5c: ea0001b1     	b	0x3f7128 <Level::_LoadProcess()+0x798> @ imm = #0x6c4
  3f6a60: ea000199     	b	0x3f70cc <Level::_LoadProcess()+0x73c> @ imm = #0x664
  3f6a64: ea000184     	b	0x3f707c <Level::_LoadProcess()+0x6ec> @ imm = #0x610
  3f6a68: ea000405     	b	0x3f7a84 <Level::_LoadProcess()+0x10f4> @ imm = #0x1014
  3f6a6c: ea0003f0     	b	0x3f7a34 <Level::_LoadProcess()+0x10a4> @ imm = #0xfc0
  3f6a70: ea000382     	b	0x3f7880 <Level::_LoadProcess()+0xef0> @ imm = #0xe08
  3f6a74: ea00036b     	b	0x3f7828 <Level::_LoadProcess()+0xe98> @ imm = #0xdac
  3f6a78: ea000363     	b	0x3f780c <Level::_LoadProcess()+0xe7c> @ imm = #0xd8c
  3f6a7c: ea00034f     	b	0x3f77c0 <Level::_LoadProcess()+0xe30> @ imm = #0xd3c
  3f6a80: ea000310     	b	0x3f76c8 <Level::_LoadProcess()+0xd38> @ imm = #0xc40
  3f6a84: ea000308     	b	0x3f76ac <Level::_LoadProcess()+0xd1c> @ imm = #0xc20
  3f6a88: ea00043d     	b	0x3f7b84 <Level::_LoadProcess()+0x11f4> @ imm = #0x10f4
  3f6a8c: ea000429     	b	0x3f7b38 <Level::_LoadProcess()+0x11a8> @ imm = #0x10a4
  3f6a90: ea000415     	b	0x3f7aec <Level::_LoadProcess()+0x115c> @ imm = #0x1054
  3f6a94: ea000401     	b	0x3f7aa0 <Level::_LoadProcess()+0x1110> @ imm = #0x1004
  3f6a98: ea0004d5     	b	0x3f7df4 <Level::_LoadProcess()+0x1464> @ imm = #0x1354
  3f6a9c: ea0004c0     	b	0x3f7da4 <Level::_LoadProcess()+0x1414> @ imm = #0x1300
  3f6aa0: ea000142     	b	0x3f6fb0 <Level::_LoadProcess()+0x620> @ imm = #0x508
  3f6aa4: ea0000f9     	b	0x3f6e90 <Level::_LoadProcess()+0x500> @ imm = #0x3e4
  3f6aa8: ea0000f8     	b	0x3f6e90 <Level::_LoadProcess()+0x500> @ imm = #0x3e0
  3f6aac: ea000462     	b	0x3f7c3c <Level::_LoadProcess()+0x12ac> @ imm = #0x1188
  3f6ab0: ea00044c     	b	0x3f7be8 <Level::_LoadProcess()+0x1258> @ imm = #0x1130
  3f6ab4: ea000445     	b	0x3f7bd0 <Level::_LoadProcess()+0x1240> @ imm = #0x1114
  3f6ab8: ea000315     	b	0x3f7714 <Level::_LoadProcess()+0xd84> @ imm = #0xc54
  3f6abc: ea0002db     	b	0x3f7630 <Level::_LoadProcess()+0xca0> @ imm = #0xb6c
  3f6ac0: ea00001b     	b	0x3f6b34 <Level::_LoadProcess()+0x1a4> @ imm = #0x6c
  3f6ac4: ea00029c     	b	0x3f753c <Level::_LoadProcess()+0xbac> @ imm = #0xa70
  3f6ac8: ea0002a2     	b	0x3f7558 <Level::_LoadProcess()+0xbc8> @ imm = #0xa88
  3f6acc: ea000471     	b	0x3f7c98 <Level::_LoadProcess()+0x1308> @ imm = #0x11c4
  3f6ad0: e5940130     	ldr	r0, [r4, #0x130]
  3f6ad4: ebffe1ac     	bl	0x3ef18c <DBG_GetLoadingStepName(int)> @ imm = #-0x7950
  3f6ad8: e1a07000     	mov	r7, r0
  3f6adc: eb00cedf     	bl	0x42a660 <MenuDebug::GetInstance()> @ imm = #0x33b7c
  3f6ae0: e1a01007     	mov	r1, r7
  3f6ae4: eb00cd68     	bl	0x42a08c <MenuDebug::SetText(char const*)> @ imm = #0x335a0
  3f6ae8: eaffffca     	b	0x3f6a18 <Level::_LoadProcess()+0x88> @ imm = #-0xd8
  3f6aec: e59f7f24     	ldr	r7, [pc, #0xf24]        @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f6af0: e7957007     	ldr	r7, [r5, r7]
  3f6af4: e1a00007     	mov	r0, r7
  3f6af8: ebfca2a5     	bl	0x31f594 <Application::GetCurrentLevel() const> @ imm = #-0xd756c
  3f6afc: e3500000     	cmp	r0, #0
  3f6b00: 0affffb1     	beq	0x3f69cc <Level::_LoadProcess()+0x3c> @ imm = #-0x13c
  3f6b04: e5970040     	ldr	r0, [r7, #0x40]
  3f6b08: e3a01000     	mov	r1, #0
  3f6b0c: e3a02001     	mov	r2, #1
  3f6b10: ebfdde58     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x886a0
  3f6b14: e5900660     	ldr	r0, [r0, #0x660]
  3f6b18: e3500000     	cmp	r0, #0
  3f6b1c: 0affffaa     	beq	0x3f69cc <Level::_LoadProcess()+0x3c> @ imm = #-0x158
  3f6b20: ebff13d2     	bl	0x3bba70 <Character::SG_UnlockAllFastTravels()> @ imm = #-0x3b0b8
  3f6b24: eb00fe73     	bl	0x4364f8 <MenuWorldMap::GetInstance()> @ imm = #0x3f9cc
  3f6b28: e3a03001     	mov	r3, #1
  3f6b2c: e5c0312c     	strb	r3, [r0, #0x12c]
  3f6b30: eaffffa5     	b	0x3f69cc <Level::_LoadProcess()+0x3c> @ imm = #-0x16c
  3f6b34: ebfce701     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc63fc
  3f6b38: e59f1df8     	ldr	r1, [pc, #0xdf8]        @ 0x3f7938 <Level::_LoadProcess()+0xfa8>
  3f6b3c: e28d8f67     	add	r8, sp, #412
  3f6b40: e28d20bc     	add	r2, sp, #188
  3f6b44: e1a0a000     	mov	r10, r0
  3f6b48: e08f1001     	add	r1, pc, r1
  3f6b4c: e1a00008     	mov	r0, r8
  3f6b50: e59f7ec0     	ldr	r7, [pc, #0xec0]        @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f6b54: ebfc7564     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe2a70
  3f6b58: e1a01008     	mov	r1, r8
  3f6b5c: e1a0000a     	mov	r0, r10
  3f6b60: ebfd03c8     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbf0e0
  3f6b64: e1a00008     	mov	r0, r8
  3f6b68: ebfc738f     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe31c4
  3f6b6c: e7953007     	ldr	r3, [r5, r7]
  3f6b70: e3a01000     	mov	r1, #0
  3f6b74: e3a02001     	mov	r2, #1
  3f6b78: e5930040     	ldr	r0, [r3, #0x40]
  3f6b7c: e594b128     	ldr	r11, [r4, #0x128]
  3f6b80: ebfdde3c     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x88710
  3f6b84: e5908660     	ldr	r8, [r0, #0x660]
  3f6b88: e3580000     	cmp	r8, #0
  3f6b8c: 0a000007     	beq	0x3f6bb0 <Level::_LoadProcess()+0x220> @ imm = #0x1c
  3f6b90: e3e02000     	mvn	r2, #0
  3f6b94: e1a00008     	mov	r0, r8
  3f6b98: e594103c     	ldr	r1, [r4, #0x3c]
  3f6b9c: ebff132c     	bl	0x3bb854 <Character::SG_SetLevelId(int, int)> @ imm = #-0x3b350
  3f6ba0: e1a0000b     	mov	r0, r11
  3f6ba4: e1a01008     	mov	r1, r8
  3f6ba8: e3a02000     	mov	r2, #0
  3f6bac: eb006b84     	bl	0x4119c4 <CameraTarget::SetTarget(GameObject*, int)> @ imm = #0x1ae10
  3f6bb0: eb101af7     	bl	0x7fd794 <GetOnline()>  @ imm = #0x406bdc
  3f6bb4: e5d03005     	ldrb	r3, [r0, #0x5]
  3f6bb8: e3530000     	cmp	r3, #0
  3f6bbc: 1a0004e4     	bne	0x3f7f54 <Level::_LoadProcess()+0x15c4> @ imm = #0x1390
  3f6bc0: e1a0000b     	mov	r0, r11
  3f6bc4: eb006224     	bl	0x40f45c <CameraBase::SetActive()> @ imm = #0x18890
  3f6bc8: e795a007     	ldr	r10, [r5, r7]
  3f6bcc: e3a02000     	mov	r2, #0
  3f6bd0: e3a01000     	mov	r1, #0
  3f6bd4: e59a3010     	ldr	r3, [r10, #0x10]
  3f6bd8: e1a08002     	mov	r8, r2
  3f6bdc: e3a09001     	mov	r9, #1
  3f6be0: e593301c     	ldr	r3, [r3, #0x1c]
  3f6be4: e1a00003     	mov	r0, r3
  3f6be8: e5933000     	ldr	r3, [r3]
  3f6bec: e1a0e00f     	mov	lr, pc
  3f6bf0: e593f060     	ldr	pc, [r3, #0x60]
  3f6bf4: e1a0000b     	mov	r0, r11
  3f6bf8: e59b3000     	ldr	r3, [r11]
  3f6bfc: e1a0e00f     	mov	lr, pc
  3f6c00: e593f010     	ldr	pc, [r3, #0x10]
  3f6c04: ea000008     	b	0x3f6c2c <Level::_LoadProcess()+0x29c> @ imm = #0x20
  3f6c08: e1a01008     	mov	r1, r8
  3f6c0c: e59a0040     	ldr	r0, [r10, #0x40]
  3f6c10: e3a02001     	mov	r2, #1
  3f6c14: ebfdde17     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x887a4
  3f6c18: e5903660     	ldr	r3, [r0, #0x660]
  3f6c1c: e2888001     	add	r8, r8, #1
  3f6c20: e3530000     	cmp	r3, #0
  3f6c24: 15933378     	ldrne	r3, [r3, #0x378]
  3f6c28: 15c39008     	strbne	r9, [r3, #0x8]
  3f6c2c: e59a0040     	ldr	r0, [r10, #0x40]
  3f6c30: e3a01001     	mov	r1, #1
  3f6c34: ebfddfa5     	bl	0x36ead0 <PlayerManager::GetNumLocalPlayers(bool)> @ imm = #-0x8816c
  3f6c38: e1580000     	cmp	r8, r0
  3f6c3c: bafffff1     	blt	0x3f6c08 <Level::_LoadProcess()+0x278> @ imm = #-0x3c
  3f6c40: e5943128     	ldr	r3, [r4, #0x128]
  3f6c44: e28d0088     	add	r0, sp, #136
  3f6c48: e240000c     	sub	r0, r0, #12
  3f6c4c: e5931008     	ldr	r1, [r3, #0x8]
  3f6c50: eb06814a     	bl	0x597180 <glitch::scene::ISceneNode::getAbsolutePosition() const> @ imm = #0x1a0528
  3f6c54: e5943128     	ldr	r3, [r4, #0x128]
  3f6c58: e28d8f61     	add	r8, sp, #388
  3f6c5c: e5930008     	ldr	r0, [r3, #0x8]
  3f6c60: eb06818a     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0x1a0628
  3f6c64: e1a01000     	mov	r1, r0
  3f6c68: e28d0078     	add	r0, sp, #120
  3f6c6c: e2400008     	sub	r0, r0, #8
  3f6c70: eb068142     	bl	0x597180 <glitch::scene::ISceneNode::getAbsolutePosition() const> @ imm = #0x1a0508
  3f6c74: e59d1074     	ldr	r1, [sp, #0x74]
  3f6c78: e59d0080     	ldr	r0, [sp, #0x80]
  3f6c7c: ebfc5dca     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0xe88d8
  3f6c80: e59d1078     	ldr	r1, [sp, #0x78]
  3f6c84: e1a0b000     	mov	r11, r0
  3f6c88: e59d0084     	ldr	r0, [sp, #0x84]
  3f6c8c: ebfc5dc6     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0xe88e8
  3f6c90: e59d1070     	ldr	r1, [sp, #0x70]
  3f6c94: e1a09000     	mov	r9, r0
  3f6c98: e59d007c     	ldr	r0, [sp, #0x7c]
  3f6c9c: ebfc5dc2     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0xe88f8
  3f6ca0: e584b1a0     	str	r11, [r4, #0x1a0]
  3f6ca4: e584019c     	str	r0, [r4, #0x19c]
  3f6ca8: e58491a4     	str	r9, [r4, #0x1a4]
  3f6cac: e1a00004     	mov	r0, r4
  3f6cb0: e59a9058     	ldr	r9, [r10, #0x58]
  3f6cb4: ebffe1fb     	bl	0x3ef4a8 <Level::GetLevelConfig() const> @ imm = #-0x7814
  3f6cb8: e1a02000     	mov	r2, r0
  3f6cbc: e59231e4     	ldr	r3, [r2, #0x1e4]
  3f6cc0: e59221e8     	ldr	r2, [r2, #0x1e8]
  3f6cc4: e59001e0     	ldr	r0, [r0, #0x1e0]
  3f6cc8: e58d3010     	str	r3, [sp, #0x10]
  3f6ccc: e58d2014     	str	r2, [sp, #0x14]
  3f6cd0: eb131d72     	bl	0x8be2a0 <__fixunssfsi> @ imm = #0x4c75c8
  3f6cd4: e59d3010     	ldr	r3, [sp, #0x10]
  3f6cd8: e6efb070     	uxtb	r11, r0
  3f6cdc: e1a00003     	mov	r0, r3
  3f6ce0: eb131d6e     	bl	0x8be2a0 <__fixunssfsi> @ imm = #0x4c75b8
  3f6ce4: e59d2014     	ldr	r2, [sp, #0x14]
  3f6ce8: e6ef3070     	uxtb	r3, r0
  3f6cec: e58d3010     	str	r3, [sp, #0x10]
  3f6cf0: e1a00002     	mov	r0, r2
  3f6cf4: eb131d69     	bl	0x8be2a0 <__fixunssfsi> @ imm = #0x4c75a4
  3f6cf8: e3e02000     	mvn	r2, #0
  3f6cfc: e5c920f7     	strb	r2, [r9, #0xf7]
  3f6d00: e5c900f6     	strb	r0, [r9, #0xf6]
  3f6d04: e59d3010     	ldr	r3, [sp, #0x10]
  3f6d08: e1a00004     	mov	r0, r4
  3f6d0c: e5c9b0f4     	strb	r11, [r9, #0xf4]
  3f6d10: e5c930f5     	strb	r3, [r9, #0xf5]
  3f6d14: ebffe1e3     	bl	0x3ef4a8 <Level::GetLevelConfig() const> @ imm = #-0x7874
  3f6d18: e59001d8     	ldr	r0, [r0, #0x1d8]
  3f6d1c: ebfc5f10     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0xe83c0
  3f6d20: e1a0b000     	mov	r11, r0
  3f6d24: e1a00004     	mov	r0, r4
  3f6d28: ebffe1de     	bl	0x3ef4a8 <Level::GetLevelConfig() const> @ imm = #-0x7888
  3f6d2c: e59001dc     	ldr	r0, [r0, #0x1dc]
  3f6d30: ebfc5f0b     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0xe83d4
  3f6d34: e589b110     	str	r11, [r9, #0x110]
  3f6d38: e5890114     	str	r0, [r9, #0x114]
  3f6d3c: e1a00004     	mov	r0, r4
  3f6d40: ebffe1d8     	bl	0x3ef4a8 <Level::GetLevelConfig() const> @ imm = #-0x78a0
  3f6d44: e59011f8     	ldr	r1, [r0, #0x1f8]
  3f6d48: e59021fc     	ldr	r2, [r0, #0x1fc]
  3f6d4c: e5903200     	ldr	r3, [r0, #0x200]
  3f6d50: e589111c     	str	r1, [r9, #0x11c]
  3f6d54: e5892120     	str	r2, [r9, #0x120]
  3f6d58: e5893124     	str	r3, [r9, #0x124]
  3f6d5c: e59a3010     	ldr	r3, [r10, #0x10]
  3f6d60: e1a00004     	mov	r0, r4
  3f6d64: e593901c     	ldr	r9, [r3, #0x1c]
  3f6d68: ebffe1ce     	bl	0x3ef4a8 <Level::GetLevelConfig() const> @ imm = #-0x78c8
  3f6d6c: e59011f8     	ldr	r1, [r0, #0x1f8]
  3f6d70: e59021fc     	ldr	r2, [r0, #0x1fc]
  3f6d74: e5903200     	ldr	r3, [r0, #0x200]
  3f6d78: e5891458     	str	r1, [r9, #0x458]
  3f6d7c: e589245c     	str	r2, [r9, #0x45c]
  3f6d80: e5893460     	str	r3, [r9, #0x460]
  3f6d84: e59a3010     	ldr	r3, [r10, #0x10]
  3f6d88: e593b01c     	ldr	r11, [r3, #0x1c]
  3f6d8c: ebfce66b     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc6654
  3f6d90: e59f1ba4     	ldr	r1, [pc, #0xba4]        @ 0x3f793c <Level::_LoadProcess()+0xfac>
  3f6d94: e28d20b8     	add	r2, sp, #184
  3f6d98: e1a09000     	mov	r9, r0
  3f6d9c: e08f1001     	add	r1, pc, r1
  3f6da0: e1a00008     	mov	r0, r8
  3f6da4: ebfc74d0     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe2cc0
  3f6da8: e1a01008     	mov	r1, r8
  3f6dac: e1a00009     	mov	r0, r9
  3f6db0: ebfd0334     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbf330
  3f6db4: e2200001     	eor	r0, r0, #1
  3f6db8: e5cb0430     	strb	r0, [r11, #0x430]
  3f6dbc: e1a00008     	mov	r0, r8
  3f6dc0: ebfc72f9     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe341c
  3f6dc4: e59a0038     	ldr	r0, [r10, #0x38]
  3f6dc8: ebfd3ae1     	bl	0x345954 <ObjectManager::HandleNoRoomObjects()> @ imm = #-0xb147c
  3f6dcc: e1a00004     	mov	r0, r4
  3f6dd0: e3a01000     	mov	r1, #0
  3f6dd4: ebffed4a     	bl	0x3f2304 <Level::UpdateFog(glitch::scene::ISceneNode*)> @ imm = #-0x4ad8
  3f6dd8: e1a00004     	mov	r0, r4
  3f6ddc: e3a01001     	mov	r1, #1
  3f6de0: e3a02004     	mov	r2, #4
  3f6de4: ebffe125     	bl	0x3ef280 <Level::UpdateLightSet(bool, int)> @ imm = #-0x7b6c
  3f6de8: e1a00004     	mov	r0, r4
  3f6dec: e3a01000     	mov	r1, #0
  3f6df0: e3a02001     	mov	r2, #1
  3f6df4: ebffe10e     	bl	0x3ef234 <Level::UpdateMaterial(bool, bool)> @ imm = #-0x7bc8
  3f6df8: eb101a65     	bl	0x7fd794 <GetOnline()>  @ imm = #0x406994
  3f6dfc: e5d03005     	ldrb	r3, [r0, #0x5]
  3f6e00: e3530000     	cmp	r3, #0
  3f6e04: 1a000449     	bne	0x3f7f30 <Level::_LoadProcess()+0x15a0> @ imm = #0x1124
  3f6e08: e59400ec     	ldr	r0, [r4, #0xec]
  3f6e0c: e3500000     	cmp	r0, #0
  3f6e10: 0a00043a     	beq	0x3f7f00 <Level::_LoadProcess()+0x1570> @ imm = #0x10e8
  3f6e14: e5941114     	ldr	r1, [r4, #0x114]
  3f6e18: e5942040     	ldr	r2, [r4, #0x40]
  3f6e1c: e594303c     	ldr	r3, [r4, #0x3c]
  3f6e20: eb01b118     	bl	0x463288 <LevelSavegame::ValidateCheckpoint(int, int, int)> @ imm = #0x6c460
  3f6e24: e3500000     	cmp	r0, #0
  3f6e28: 1a000434     	bne	0x3f7f00 <Level::_LoadProcess()+0x1570> @ imm = #0x10d0
  3f6e2c: e1a00004     	mov	r0, r4
  3f6e30: ebffe1f7     	bl	0x3ef614 <Level::GetSpawnPoint()> @ imm = #-0x7824
  3f6e34: e7953007     	ldr	r3, [r5, r7]
  3f6e38: e1a08000     	mov	r8, r0
  3f6e3c: e3a01000     	mov	r1, #0
  3f6e40: e5930040     	ldr	r0, [r3, #0x40]
  3f6e44: e3a02001     	mov	r2, #1
  3f6e48: ebfddd8a     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x889d8
  3f6e4c: e3580000     	cmp	r8, #0
  3f6e50: e5903660     	ldr	r3, [r0, #0x660]
  3f6e54: 12888e16     	addne	r8, r8, #352
  3f6e58: 0a0004e2     	beq	0x3f81e8 <Level::_LoadProcess()+0x1858> @ imm = #0x1388
  3f6e5c: e5983000     	ldr	r3, [r8]
  3f6e60: e28d1068     	add	r1, sp, #104
  3f6e64: e2411004     	sub	r1, r1, #4
  3f6e68: e58d3064     	str	r3, [sp, #0x64]
  3f6e6c: e5983004     	ldr	r3, [r8, #0x4]
  3f6e70: e5d420f5     	ldrb	r2, [r4, #0xf5]
  3f6e74: e1a00004     	mov	r0, r4
  3f6e78: e58d3068     	str	r3, [sp, #0x68]
  3f6e7c: e5983008     	ldr	r3, [r8, #0x8]
  3f6e80: e58d306c     	str	r3, [sp, #0x6c]
  3f6e84: ebffe58a     	bl	0x3f04b4 <Level::CheckpointSave(Point3D<float> const&, bool)> @ imm = #-0x69d8
  3f6e88: e3a03000     	mov	r3, #0
  3f6e8c: e5c430f5     	strb	r3, [r4, #0xf5]
  3f6e90: e5943130     	ldr	r3, [r4, #0x130]
  3f6e94: e2833001     	add	r3, r3, #1
  3f6e98: e5843130     	str	r3, [r4, #0x130]
  3f6e9c: e5943130     	ldr	r3, [r4, #0x130]
  3f6ea0: e3530026     	cmp	r3, #38
  3f6ea4: 0a00002e     	beq	0x3f6f64 <Level::_LoadProcess()+0x5d4> @ imm = #0xb8
  3f6ea8: e5942138     	ldr	r2, [r4, #0x138]
  3f6eac: e5943134     	ldr	r3, [r4, #0x134]
  3f6eb0: e1520003     	cmp	r2, r3
  3f6eb4: b5943138     	ldrlt	r3, [r4, #0x138]
  3f6eb8: a5943134     	ldrge	r3, [r4, #0x134]
  3f6ebc: e5843134     	str	r3, [r4, #0x134]
  3f6ec0: e5943130     	ldr	r3, [r4, #0x130]
  3f6ec4: e3530024     	cmp	r3, #36
  3f6ec8: 0a000023     	beq	0x3f6f5c <Level::_LoadProcess()+0x5cc> @ imm = #0x8c
  3f6ecc: e5943130     	ldr	r3, [r4, #0x130]
  3f6ed0: e3a01064     	mov	r1, #100
  3f6ed4: e3012af3     	movw	r2, #0x1af3
  3f6ed8: e0010391     	mul	r1, r1, r3
  3f6edc: e3462bca     	movt	r2, #0x6bca
  3f6ee0: e0c30192     	smull	r0, r3, r2, r1
  3f6ee4: e1a01fc1     	asr	r1, r1, #31
  3f6ee8: e0613243     	rsb	r3, r1, r3, asr #4
  3f6eec: e3530063     	cmp	r3, #99
  3f6ef0: ca000019     	bgt	0x3f6f5c <Level::_LoadProcess()+0x5cc> @ imm = #0x64
  3f6ef4: e5843030     	str	r3, [r4, #0x30]
  3f6ef8: eb00d6e3     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0x35b8c
  3f6efc: e59f1a3c     	ldr	r1, [pc, #0xa3c]        @ 0x3f7940 <Level::_LoadProcess()+0xfb0>
  3f6f00: e08f1001     	add	r1, pc, r1
  3f6f04: eb00d8b9     	bl	0x42d1f0 <MenuManager::GetMenuByName(char const*)> @ imm = #0x362e4
  3f6f08: e2504000     	subs	r4, r0, #0
  3f6f0c: 0a00000a     	beq	0x3f6f3c <Level::_LoadProcess()+0x5ac> @ imm = #0x28
  3f6f10: e2840048     	add	r0, r4, #72
  3f6f14: e5947004     	ldr	r7, [r4, #0x4]
  3f6f18: ebfe3c89     	bl	0x386144 <gameswf::weak_ptr<gameswf::character>::check_proxy() const> @ imm = #-0x70ddc
  3f6f1c: e59f2a20     	ldr	r2, [pc, #0xa20]        @ 0x3f7944 <Level::_LoadProcess()+0xfb4>
  3f6f20: e3a0c000     	mov	r12, #0
  3f6f24: e594104c     	ldr	r1, [r4, #0x4c]
  3f6f28: e1a00007     	mov	r0, r7
  3f6f2c: e08f2002     	add	r2, pc, r2
  3f6f30: e1a0300c     	mov	r3, r12
  3f6f34: e58dc000     	str	r12, [sp]
  3f6f38: eb0ed3b3     	bl	0x7abe0c <RenderFX::InvokeASCallback(gameswf::character*, char const*, gameswf::as_value const*, int)> @ imm = #0x3b4ecc
  3f6f3c: e7953006     	ldr	r3, [r5, r6]
  3f6f40: e59d24b4     	ldr	r2, [sp, #0x4b4]
  3f6f44: e5933000     	ldr	r3, [r3]
  3f6f48: e1520003     	cmp	r2, r3
  3f6f4c: 1a0004e0     	bne	0x3f82d4 <Level::_LoadProcess()+0x1944> @ imm = #0x1380
  3f6f50: e28dd0bc     	add	sp, sp, #188
  3f6f54: e28ddb01     	add	sp, sp, #1024
  3f6f58: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3f6f5c: e3a03064     	mov	r3, #100
  3f6f60: eaffffe3     	b	0x3f6ef4 <Level::_LoadProcess()+0x564> @ imm = #-0x74
  3f6f64: e3a03064     	mov	r3, #100
  3f6f68: e5843030     	str	r3, [r4, #0x30]
  3f6f6c: e59f39d4     	ldr	r3, [pc, #0x9d4]        @ 0x3f7948 <Level::_LoadProcess()+0xfb8>
  3f6f70: e7953003     	ldr	r3, [r5, r3]
  3f6f74: e5d33000     	ldrb	r3, [r3]
  3f6f78: e3530000     	cmp	r3, #0
  3f6f7c: 1a000008     	bne	0x3f6fa4 <Level::_LoadProcess()+0x614> @ imm = #0x20
  3f6f80: eb00d6c1     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0x35b04
  3f6f84: e59f19c0     	ldr	r1, [pc, #0x9c0]        @ 0x3f794c <Level::_LoadProcess()+0xfbc>
  3f6f88: e1a04000     	mov	r4, r0
  3f6f8c: e08f1001     	add	r1, pc, r1
  3f6f90: eb00d896     	bl	0x42d1f0 <MenuManager::GetMenuByName(char const*)> @ imm = #0x36258
  3f6f94: e1a01000     	mov	r1, r0
  3f6f98: e1a00004     	mov	r0, r4
  3f6f9c: eb00ea11     	bl	0x4317e8 <MenuManager::PushMenu(MenuBase*)> @ imm = #0x3a844
  3f6fa0: eaffffd4     	b	0x3f6ef8 <Level::_LoadProcess()+0x568> @ imm = #-0xb0
  3f6fa4: e3a00001     	mov	r0, #1
  3f6fa8: eb1293c7     	bl	0x89becc <ALicenseCheck_ValidateLicense> @ imm = #0x4a4f1c
  3f6fac: eafffff3     	b	0x3f6f80 <Level::_LoadProcess()+0x5f0> @ imm = #-0x34
  3f6fb0: eb00d6b5     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0x35ad4
  3f6fb4: e3a01003     	mov	r1, #3
  3f6fb8: eb00ebb9     	bl	0x431ea4 <MenuManager::LoadMenu(int)> @ imm = #0x3aee4
  3f6fbc: e59f398c     	ldr	r3, [pc, #0x98c]        @ 0x3f7950 <Level::_LoadProcess()+0xfc0>
  3f6fc0: e3a01000     	mov	r1, #0
  3f6fc4: e1a02001     	mov	r2, r1
  3f6fc8: e7957003     	ldr	r7, [r5, r3]
  3f6fcc: e1a03001     	mov	r3, r1
  3f6fd0: e1a00007     	mov	r0, r7
  3f6fd4: eb00c31a     	bl	0x427c44 <DebugCachedCharacter::RefreshCache(gameswf::character*, MenuFX*, gameswf::character*)> @ imm = #0x30c68
  3f6fd8: e59f3974     	ldr	r3, [pc, #0x974]        @ 0x3f7954 <Level::_LoadProcess()+0xfc4>
  3f6fdc: e3a01000     	mov	r1, #0
  3f6fe0: e1a02001     	mov	r2, r1
  3f6fe4: e7950003     	ldr	r0, [r5, r3]
  3f6fe8: e1a03001     	mov	r3, r1
  3f6fec: eb00c314     	bl	0x427c44 <DebugCachedCharacter::RefreshCache(gameswf::character*, MenuFX*, gameswf::character*)> @ imm = #0x30c50
  3f6ff0: e3a01000     	mov	r1, #0
  3f6ff4: e1a02001     	mov	r2, r1
  3f6ff8: e1a03001     	mov	r3, r1
  3f6ffc: e1a00007     	mov	r0, r7
  3f7000: eb00c30f     	bl	0x427c44 <DebugCachedCharacter::RefreshCache(gameswf::character*, MenuFX*, gameswf::character*)> @ imm = #0x30c3c
  3f7004: e59f394c     	ldr	r3, [pc, #0x94c]        @ 0x3f7958 <Level::_LoadProcess()+0xfc8>
  3f7008: e3a01000     	mov	r1, #0
  3f700c: e1a02001     	mov	r2, r1
  3f7010: e7950003     	ldr	r0, [r5, r3]
  3f7014: e1a03001     	mov	r3, r1
  3f7018: eb00c309     	bl	0x427c44 <DebugCachedCharacter::RefreshCache(gameswf::character*, MenuFX*, gameswf::character*)> @ imm = #0x30c24
  3f701c: e59f3938     	ldr	r3, [pc, #0x938]        @ 0x3f795c <Level::_LoadProcess()+0xfcc>
  3f7020: e3a01000     	mov	r1, #0
  3f7024: e1a02001     	mov	r2, r1
  3f7028: e7950003     	ldr	r0, [r5, r3]
  3f702c: e1a03001     	mov	r3, r1
  3f7030: eb00c303     	bl	0x427c44 <DebugCachedCharacter::RefreshCache(gameswf::character*, MenuFX*, gameswf::character*)> @ imm = #0x30c0c
  3f7034: e59f39dc     	ldr	r3, [pc, #0x9dc]        @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f7038: e7953003     	ldr	r3, [r5, r3]
  3f703c: e5930040     	ldr	r0, [r3, #0x40]
  3f7040: ebfde09d     	bl	0x36f2bc <PlayerManager::PostInitCharacters()> @ imm = #-0x87d8c
  3f7044: eb00d690     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0x35a40
  3f7048: eb00d6cf     	bl	0x42cb8c <MenuManager::GetHUDRoot()> @ imm = #0x35b3c
  3f704c: e59f190c     	ldr	r1, [pc, #0x90c]        @ 0x3f7960 <Level::_LoadProcess()+0xfd0>
  3f7050: e59f290c     	ldr	r2, [pc, #0x90c]        @ 0x3f7964 <Level::_LoadProcess()+0xfd4>
  3f7054: e3a0c000     	mov	r12, #0
  3f7058: e1a0300c     	mov	r3, r12
  3f705c: e08f1001     	add	r1, pc, r1
  3f7060: e08f2002     	add	r2, pc, r2
  3f7064: e58dc000     	str	r12, [sp]
  3f7068: eb0ed9de     	bl	0x7ad7e8 <RenderFX::InvokeASCallback(char const*, char const*, gameswf::as_value const*, int)> @ imm = #0x3b6778
  3f706c: e5943130     	ldr	r3, [r4, #0x130]
  3f7070: e2833001     	add	r3, r3, #1
  3f7074: e5843130     	str	r3, [r4, #0x130]
  3f7078: eaffff87     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x1e4
  3f707c: ebfce5af     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc6944
  3f7080: e59f18e0     	ldr	r1, [pc, #0x8e0]        @ 0x3f7968 <Level::_LoadProcess()+0xfd8>
  3f7084: e28d7fc7     	add	r7, sp, #796
  3f7088: e28d20fc     	add	r2, sp, #252
  3f708c: e1a08000     	mov	r8, r0
  3f7090: e08f1001     	add	r1, pc, r1
  3f7094: e1a00007     	mov	r0, r7
  3f7098: ebfc7413     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe2fb4
  3f709c: e1a01007     	mov	r1, r7
  3f70a0: e1a00008     	mov	r0, r8
  3f70a4: ebfd0277     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbf624
  3f70a8: e1a00007     	mov	r0, r7
  3f70ac: ebfc723e     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe3708
  3f70b0: e59f3978     	ldr	r3, [pc, #0x978]        @ 0x3f7a30 <Level::_LoadProcess()+0x10a0>
  3f70b4: e7950003     	ldr	r0, [r5, r3]
  3f70b8: eb04ad83     	bl	0x5226cc <PFWorld::PostLoad()> @ imm = #0x12b60c
  3f70bc: e5943130     	ldr	r3, [r4, #0x130]
  3f70c0: e2833001     	add	r3, r3, #1
  3f70c4: e5843130     	str	r3, [r4, #0x130]
  3f70c8: eaffff73     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x234
  3f70cc: ebfce59b     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc6994
  3f70d0: e59f1894     	ldr	r1, [pc, #0x894]        @ 0x3f796c <Level::_LoadProcess()+0xfdc>
  3f70d4: e28d8fcd     	add	r8, sp, #820
  3f70d8: e28d2c01     	add	r2, sp, #256
  3f70dc: e1a0a000     	mov	r10, r0
  3f70e0: e08f1001     	add	r1, pc, r1
  3f70e4: e1a00008     	mov	r0, r8
  3f70e8: e59f7928     	ldr	r7, [pc, #0x928]        @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f70ec: ebfc73fe     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3008
  3f70f0: e1a01008     	mov	r1, r8
  3f70f4: e1a0000a     	mov	r0, r10
  3f70f8: ebfd0262     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbf678
  3f70fc: e1a00008     	mov	r0, r8
  3f7100: ebfc7229     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe375c
  3f7104: e7957007     	ldr	r7, [r5, r7]
  3f7108: e5973038     	ldr	r3, [r7, #0x38]
  3f710c: e593301c     	ldr	r3, [r3, #0x1c]
  3f7110: e5843138     	str	r3, [r4, #0x138]
  3f7114: e5970038     	ldr	r0, [r7, #0x38]
  3f7118: ebfd3903     	bl	0x34552c <ObjectManager::InitPost()> @ imm = #-0xb1bf4
  3f711c: e3500000     	cmp	r0, #0
  3f7120: 0afffffb     	beq	0x3f7114 <Level::_LoadProcess()+0x784> @ imm = #-0x14
  3f7124: eaffff59     	b	0x3f6e90 <Level::_LoadProcess()+0x500> @ imm = #-0x29c
  3f7128: ebfce584     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc69f0
  3f712c: e59f183c     	ldr	r1, [pc, #0x83c]        @ 0x3f7970 <Level::_LoadProcess()+0xfe0>
  3f7130: e28d7fd3     	add	r7, sp, #844
  3f7134: e28d2f41     	add	r2, sp, #260
  3f7138: e1a08000     	mov	r8, r0
  3f713c: e08f1001     	add	r1, pc, r1
  3f7140: e1a00007     	mov	r0, r7
  3f7144: ebfc73e8     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3060
  3f7148: e1a01007     	mov	r1, r7
  3f714c: e1a00008     	mov	r0, r8
  3f7150: ebfd024c     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbf6d0
  3f7154: e1a00007     	mov	r0, r7
  3f7158: ebfc7213     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe37b4
  3f715c: e1a00004     	mov	r0, r4
  3f7160: ebfff50d     	bl	0x3f459c <Level::_LoadLightSet()> @ imm = #-0x2bcc
  3f7164: e5943130     	ldr	r3, [r4, #0x130]
  3f7168: e2833001     	add	r3, r3, #1
  3f716c: e5843130     	str	r3, [r4, #0x130]
  3f7170: eaffff49     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x2dc
  3f7174: ebfce571     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc6a3c
  3f7178: e59f17f4     	ldr	r1, [pc, #0x7f4]        @ 0x3f7974 <Level::_LoadProcess()+0xfe4>
  3f717c: e28d7ff1     	add	r7, sp, #964
  3f7180: e28d2f46     	add	r2, sp, #280
  3f7184: e1a08000     	mov	r8, r0
  3f7188: e08f1001     	add	r1, pc, r1
  3f718c: e1a00007     	mov	r0, r7
  3f7190: ebfc73d5     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe30ac
  3f7194: e1a01007     	mov	r1, r7
  3f7198: e1a00008     	mov	r0, r8
  3f719c: ebfd0239     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbf71c
  3f71a0: e1a00007     	mov	r0, r7
  3f71a4: ebfc7200     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe3800
  3f71a8: e5943038     	ldr	r3, [r4, #0x38]
  3f71ac: e3530000     	cmp	r3, #0
  3f71b0: 0a0003ae     	beq	0x3f8070 <Level::_LoadProcess()+0x16e0> @ imm = #0xeb8
  3f71b4: e5931204     	ldr	r1, [r3, #0x204]
  3f71b8: e5932208     	ldr	r2, [r3, #0x208]
  3f71bc: e1510002     	cmp	r1, r2
  3f71c0: 0a000009     	beq	0x3f71ec <Level::_LoadProcess()+0x85c> @ imm = #0x24
  3f71c4: e5930210     	ldr	r0, [r3, #0x210]
  3f71c8: e2837f81     	add	r7, r3, #516
  3f71cc: e59f3844     	ldr	r3, [pc, #0x844]        @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f71d0: e7953003     	ldr	r3, [r5, r3]
  3f71d4: e5938038     	ldr	r8, [r3, #0x38]
  3f71d8: ebfc5cbb     	bl	0x30e4cc <.plt+0x758>   @ imm = #-0xe8d14
  3f71dc: e1a01007     	mov	r1, r7
  3f71e0: e1a02000     	mov	r2, r0
  3f71e4: e1a00008     	mov	r0, r8
  3f71e8: ebfd3fc4     	bl	0x347100 <ObjectManager::InitModulesFogColor(std::vector<Point3D<float>, std::allocator<Point3D<float>>> const&, int)> @ imm = #-0xb00f0
  3f71ec: e1a00004     	mov	r0, r4
  3f71f0: ebfff218     	bl	0x3f3a58 <Level::_LoadScripts()> @ imm = #-0x37a0
  3f71f4: e5943130     	ldr	r3, [r4, #0x130]
  3f71f8: e2833001     	add	r3, r3, #1
  3f71fc: e5843130     	str	r3, [r4, #0x130]
  3f7200: eaffff25     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x36c
  3f7204: e59f276c     	ldr	r2, [pc, #0x76c]        @ 0x3f7978 <Level::_LoadProcess()+0xfe8>
  3f7208: e59430dc     	ldr	r3, [r4, #0xdc]
  3f720c: e7957002     	ldr	r7, [r5, r2]
  3f7210: e59f2764     	ldr	r2, [pc, #0x764]        @ 0x3f797c <Level::_LoadProcess()+0xfec>
  3f7214: e5873000     	str	r3, [r7]
  3f7218: e7958002     	ldr	r8, [r5, r2]
  3f721c: e59430e0     	ldr	r3, [r4, #0xe0]
  3f7220: e5883000     	str	r3, [r8]
  3f7224: e5d430e8     	ldrb	r3, [r4, #0xe8]
  3f7228: e3530000     	cmp	r3, #0
  3f722c: 028480f8     	addeq	r8, r4, #248
  3f7230: 1a000351     	bne	0x3f7f7c <Level::_LoadProcess()+0x15ec> @ imm = #0xd44
  3f7234: ebfce541     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc6afc
  3f7238: e59f1740     	ldr	r1, [pc, #0x740]        @ 0x3f7980 <Level::_LoadProcess()+0xff0>
  3f723c: e28d7ffd     	add	r7, sp, #1012
  3f7240: e1a0a000     	mov	r10, r0
  3f7244: e28d2e12     	add	r2, sp, #288
  3f7248: e08f1001     	add	r1, pc, r1
  3f724c: e1a00007     	mov	r0, r7
  3f7250: ebfc73a5     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe316c
  3f7254: e1a01007     	mov	r1, r7
  3f7258: e1a0000a     	mov	r0, r10
  3f725c: e59fa720     	ldr	r10, [pc, #0x720]       @ 0x3f7984 <Level::_LoadProcess()+0xff4>
  3f7260: ebfd0208     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbf7e0
  3f7264: e1a00007     	mov	r0, r7
  3f7268: ebfc71cf     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe38c4
  3f726c: e3a03f7d     	mov	r3, #500
  3f7270: e5843138     	str	r3, [r4, #0x138]
  3f7274: e08fa00a     	add	r10, pc, r10
  3f7278: e28d7ff7     	add	r7, sp, #988
  3f727c: e28d9f47     	add	r9, sp, #284
  3f7280: e1a0100a     	mov	r1, r10
  3f7284: e1a02009     	mov	r2, r9
  3f7288: e1a00007     	mov	r0, r7
  3f728c: ebfc7396     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe31a8
  3f7290: e1a01008     	mov	r1, r8
  3f7294: e1a02007     	mov	r2, r7
  3f7298: e1a00004     	mov	r0, r4
  3f729c: ebfff227     	bl	0x3f3b40 <Level::LoadFile(std::string const&, std::string const&)> @ imm = #-0x3764
  3f72a0: e1a0b000     	mov	r11, r0
  3f72a4: e1a00007     	mov	r0, r7
  3f72a8: ebfc71bf     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe3904
  3f72ac: e35b0000     	cmp	r11, #0
  3f72b0: 0afffff2     	beq	0x3f7280 <Level::_LoadProcess()+0x8f0> @ imm = #-0x38
  3f72b4: e5943130     	ldr	r3, [r4, #0x130]
  3f72b8: e2833001     	add	r3, r3, #1
  3f72bc: e5843130     	str	r3, [r4, #0x130]
  3f72c0: e594313c     	ldr	r3, [r4, #0x13c]
  3f72c4: e2833001     	add	r3, r3, #1
  3f72c8: e584313c     	str	r3, [r4, #0x13c]
  3f72cc: eafffef2     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x438
  3f72d0: ebfce51a     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc6b98
  3f72d4: e59f16ac     	ldr	r1, [pc, #0x6ac]        @ 0x3f7988 <Level::_LoadProcess()+0xff8>
  3f72d8: e28d7fd9     	add	r7, sp, #868
  3f72dc: e28d2f42     	add	r2, sp, #264
  3f72e0: e1a08000     	mov	r8, r0
  3f72e4: e08f1001     	add	r1, pc, r1
  3f72e8: e1a00007     	mov	r0, r7
  3f72ec: ebfc737e     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3208
  3f72f0: e1a01007     	mov	r1, r7
  3f72f4: e1a00008     	mov	r0, r8
  3f72f8: ebfd01e2     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbf878
  3f72fc: e1a00007     	mov	r0, r7
  3f7300: ebfc71a9     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe395c
  3f7304: e59f370c     	ldr	r3, [pc, #0x70c]        @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f7308: e7953003     	ldr	r3, [r5, r3]
  3f730c: e5930040     	ldr	r0, [r3, #0x40]
  3f7310: e3a03001     	mov	r3, #1
  3f7314: e5c036c9     	strb	r3, [r0, #0x6c9]
  3f7318: ebfe0725     	bl	0x378fb4 <PlayerManager::Update()> @ imm = #-0x7e36c
  3f731c: e3a01000     	mov	r1, #0
  3f7320: e3a0000c     	mov	r0, #12
  3f7324: ebfc6491     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xe6dbc
  3f7328: e1a07000     	mov	r7, r0
  3f732c: eb020a75     	bl	0x479d08 <GameEventManager::GameEventManager()> @ imm = #0x829d4
  3f7330: e5847194     	str	r7, [r4, #0x194]
  3f7334: e1a00007     	mov	r0, r7
  3f7338: eb020ac7     	bl	0x479e5c <GameEventManager::Load()> @ imm = #0x82b1c
  3f733c: e5943130     	ldr	r3, [r4, #0x130]
  3f7340: e2833001     	add	r3, r3, #1
  3f7344: e5843130     	str	r3, [r4, #0x130]
  3f7348: eafffed3     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x4b4
  3f734c: ebfce4fb     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc6c14
  3f7350: e59f1634     	ldr	r1, [pc, #0x634]        @ 0x3f798c <Level::_LoadProcess()+0xffc>
  3f7354: e28d7e42     	add	r7, sp, #1056
  3f7358: e2877004     	add	r7, r7, #4
  3f735c: e28d2f49     	add	r2, sp, #292
  3f7360: e1a08000     	mov	r8, r0
  3f7364: e08f1001     	add	r1, pc, r1
  3f7368: e1a00007     	mov	r0, r7
  3f736c: ebfc735e     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3288
  3f7370: e1a01007     	mov	r1, r7
  3f7374: e1a00008     	mov	r0, r8
  3f7378: ebfd01c2     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbf8f8
  3f737c: e1a00007     	mov	r0, r7
  3f7380: ebfc7189     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe39dc
  3f7384: e59f368c     	ldr	r3, [pc, #0x68c]        @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f7388: e3a0c311     	mov	r12, #1140850688
  3f738c: e3a01331     	mov	r1, #-1006632960
  3f7390: e7950003     	ldr	r0, [r5, r3]
  3f7394: e28cc8fa     	add	r12, r12, #16384000
  3f7398: e28118fa     	add	r1, r1, #16384000
  3f739c: e1a0300c     	mov	r3, r12
  3f73a0: e5900044     	ldr	r0, [r0, #0x44]
  3f73a4: e1a02001     	mov	r2, r1
  3f73a8: e58dc000     	str	r12, [sp]
  3f73ac: ebfd5325     	bl	0x34c048 <PhysicalWorld::load(float, float, float, float)> @ imm = #-0xab36c
  3f73b0: e5943130     	ldr	r3, [r4, #0x130]
  3f73b4: e2833001     	add	r3, r3, #1
  3f73b8: e5843130     	str	r3, [r4, #0x130]
  3f73bc: eafffeb6     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x528
  3f73c0: ebfce4de     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc6c88
  3f73c4: e59f15c4     	ldr	r1, [pc, #0x5c4]        @ 0x3f7990 <Level::_LoadProcess()+0x1000>
  3f73c8: e28d7e43     	add	r7, sp, #1072
  3f73cc: e287700c     	add	r7, r7, #12
  3f73d0: e28d2f4a     	add	r2, sp, #296
  3f73d4: e1a08000     	mov	r8, r0
  3f73d8: e08f1001     	add	r1, pc, r1
  3f73dc: e1a00007     	mov	r0, r7
  3f73e0: ebfc7341     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe32fc
  3f73e4: e1a01007     	mov	r1, r7
  3f73e8: e1a00008     	mov	r0, r8
  3f73ec: ebfd01a5     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbf96c
  3f73f0: e1a00007     	mov	r0, r7
  3f73f4: ebfc716c     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe3a50
  3f73f8: e59f35e8     	ldr	r3, [pc, #0x5e8]        @ 0x3f79e8 <Level::_LoadProcess()+0x1058>
  3f73fc: e7950003     	ldr	r0, [r5, r3]
  3f7400: eb027df4     	bl	0x496bd8 <VisualFXManager::BuildLibraries()> @ imm = #0x9f7d0
  3f7404: e5943130     	ldr	r3, [r4, #0x130]
  3f7408: e2833001     	add	r3, r3, #1
  3f740c: e5843130     	str	r3, [r4, #0x130]
  3f7410: eafffea1     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x57c
  3f7414: ebfce4c9     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc6cdc
  3f7418: e59f1574     	ldr	r1, [pc, #0x574]        @ 0x3f7994 <Level::_LoadProcess()+0x1004>
  3f741c: e28d7e46     	add	r7, sp, #1120
  3f7420: e1a08000     	mov	r8, r0
  3f7424: e287700c     	add	r7, r7, #12
  3f7428: e28d2e13     	add	r2, sp, #304
  3f742c: e08f1001     	add	r1, pc, r1
  3f7430: e1a00007     	mov	r0, r7
  3f7434: ebfc732c     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3350
  3f7438: e1a01007     	mov	r1, r7
  3f743c: e1a00008     	mov	r0, r8
  3f7440: ebfd0190     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbf9c0
  3f7444: e1a00007     	mov	r0, r7
  3f7448: ebfc7157     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe3aa4
  3f744c: e5943130     	ldr	r3, [r4, #0x130]
  3f7450: e2833001     	add	r3, r3, #1
  3f7454: e5843130     	str	r3, [r4, #0x130]
  3f7458: eafffe8f     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x5c4
  3f745c: ebfce4b7     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc6d24
  3f7460: e59f1530     	ldr	r1, [pc, #0x530]        @ 0x3f7998 <Level::_LoadProcess()+0x1008>
  3f7464: e28d7d12     	add	r7, sp, #1152
  3f7468: e2877004     	add	r7, r7, #4
  3f746c: e28d2f4d     	add	r2, sp, #308
  3f7470: e1a08000     	mov	r8, r0
  3f7474: e08f1001     	add	r1, pc, r1
  3f7478: e1a00007     	mov	r0, r7
  3f747c: ebfc731a     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3398
  3f7480: e1a01007     	mov	r1, r7
  3f7484: e1a00008     	mov	r0, r8
  3f7488: ebfd017e     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbfa08
  3f748c: e1a00007     	mov	r0, r7
  3f7490: ebfc7145     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe3aec
  3f7494: eb00d57c     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0x355f0
  3f7498: e3a01002     	mov	r1, #2
  3f749c: eb00d806     	bl	0x42d4bc <MenuManager::UnloadMenu(int)> @ imm = #0x36018
  3f74a0: eb00d579     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0x355e4
  3f74a4: e3a01001     	mov	r1, #1
  3f74a8: eb00d803     	bl	0x42d4bc <MenuManager::UnloadMenu(int)> @ imm = #0x3600c
  3f74ac: eb00d576     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0x355d8
  3f74b0: e3a01003     	mov	r1, #3
  3f74b4: eb00d800     	bl	0x42d4bc <MenuManager::UnloadMenu(int)> @ imm = #0x36000
  3f74b8: e5943130     	ldr	r3, [r4, #0x130]
  3f74bc: e2833001     	add	r3, r3, #1
  3f74c0: e5843130     	str	r3, [r4, #0x130]
  3f74c4: eafffe74     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x630
  3f74c8: e59f3548     	ldr	r3, [pc, #0x548]        @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f74cc: e3a07000     	mov	r7, #0
  3f74d0: e7958003     	ldr	r8, [r5, r3]
  3f74d4: e59f34c0     	ldr	r3, [pc, #0x4c0]        @ 0x3f799c <Level::_LoadProcess()+0x100c>
  3f74d8: e1a00008     	mov	r0, r8
  3f74dc: e7952003     	ldr	r2, [r5, r3]
  3f74e0: e59f34b8     	ldr	r3, [pc, #0x4b8]        @ 0x3f79a0 <Level::_LoadProcess()+0x1010>
  3f74e4: e5827000     	str	r7, [r2]
  3f74e8: e7953003     	ldr	r3, [r5, r3]
  3f74ec: e5837000     	str	r7, [r3]
  3f74f0: ebfca019     	bl	0x31f55c <Application::CleanGlitch()> @ imm = #-0xd7f9c
  3f74f4: e59f04a8     	ldr	r0, [pc, #0x4a8]        @ 0x3f79a4 <Level::_LoadProcess()+0x1014>
  3f74f8: e08f0000     	add	r0, pc, r0
  3f74fc: ebfc63c6     	bl	0x31041c <ShowMemoryStats(char const*)> @ imm = #-0xe70e8
  3f7500: e59f34a0     	ldr	r3, [pc, #0x4a0]        @ 0x3f79a8 <Level::_LoadProcess()+0x1018>
  3f7504: e3a01f7d     	mov	r1, #500
  3f7508: e7953003     	ldr	r3, [r5, r3]
  3f750c: e5930000     	ldr	r0, [r3]
  3f7510: ebfdc91e     	bl	0x369990 <VoxSoundManager::StopAllSounds(int)> @ imm = #-0x8db88
  3f7514: e3a03001     	mov	r3, #1
  3f7518: e5c830b4     	strb	r3, [r8, #0xb4]
  3f751c: e5847134     	str	r7, [r4, #0x134]
  3f7520: e5847138     	str	r7, [r4, #0x138]
  3f7524: e584713c     	str	r7, [r4, #0x13c]
  3f7528: e5943130     	ldr	r3, [r4, #0x130]
  3f752c: e5c47144     	strb	r7, [r4, #0x144]
  3f7530: e2833001     	add	r3, r3, #1
  3f7534: e5843130     	str	r3, [r4, #0x130]
  3f7538: eafffe57     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x6a4
  3f753c: e5943130     	ldr	r3, [r4, #0x130]
  3f7540: e3530025     	cmp	r3, #37
  3f7544: d5943130     	ldrle	r3, [r4, #0x130]
  3f7548: c3a03026     	movgt	r3, #38
  3f754c: d2833001     	addle	r3, r3, #1
  3f7550: e5843130     	str	r3, [r4, #0x130]
  3f7554: eafffe50     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x6c0
  3f7558: ebfce478     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc6e20
  3f755c: e59f1448     	ldr	r1, [pc, #0x448]        @ 0x3f79ac <Level::_LoadProcess()+0x101c>
  3f7560: e28d7f5b     	add	r7, sp, #364
  3f7564: e28d20b8     	add	r2, sp, #184
  3f7568: e2422004     	sub	r2, r2, #4
  3f756c: e1a08000     	mov	r8, r0
  3f7570: e08f1001     	add	r1, pc, r1
  3f7574: e1a00007     	mov	r0, r7
  3f7578: ebfc72db     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3494
  3f757c: e1a01007     	mov	r1, r7
  3f7580: e1a00008     	mov	r0, r8
  3f7584: ebfd013f     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbfb04
  3f7588: e1a00007     	mov	r0, r7
  3f758c: ebfc7106     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe3be8
  3f7590: eb10187f     	bl	0x7fd794 <GetOnline()>  @ imm = #0x4061fc
  3f7594: e5d03005     	ldrb	r3, [r0, #0x5]
  3f7598: e3530000     	cmp	r3, #0
  3f759c: 0afffe3e     	beq	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x708
  3f75a0: e59f7470     	ldr	r7, [pc, #0x470]        @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f75a4: e3a01000     	mov	r1, #0
  3f75a8: e1a02001     	mov	r2, r1
  3f75ac: e7958007     	ldr	r8, [r5, r7]
  3f75b0: e5980040     	ldr	r0, [r8, #0x40]
  3f75b4: ebfddbaf     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x89144
  3f75b8: e5d03545     	ldrb	r3, [r0, #0x545]
  3f75bc: e3530000     	cmp	r3, #0
  3f75c0: 1a0002fb     	bne	0x3f81b4 <Level::_LoadProcess()+0x1824> @ imm = #0xbec
  3f75c4: e7953007     	ldr	r3, [r5, r7]
  3f75c8: e5930038     	ldr	r0, [r3, #0x38]
  3f75cc: e5d031ac     	ldrb	r3, [r0, #0x1ac]
  3f75d0: e3530000     	cmp	r3, #0
  3f75d4: 1a000000     	bne	0x3f75dc <Level::_LoadProcess()+0xc4c> @ imm = #0x0
  3f75d8: ebfd2580     	bl	0x340be0 <ObjectManager::NetworkInitLevel()> @ imm = #-0xb6a00
  3f75dc: e7957007     	ldr	r7, [r5, r7]
  3f75e0: e3a015fe     	mov	r1, #1065353216
  3f75e4: e5970038     	ldr	r0, [r7, #0x38]
  3f75e8: ebfd4c0c     	bl	0x34a620 <ObjectManager::Update(float)> @ imm = #-0xacfd0
  3f75ec: e3a01000     	mov	r1, #0
  3f75f0: e5970040     	ldr	r0, [r7, #0x40]
  3f75f4: e1a02001     	mov	r2, r1
  3f75f8: ebfddb9e     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x89188
  3f75fc: e1a07000     	mov	r7, r0
  3f7600: eb105f0d     	bl	0x80f23c <CNetPlayerInfo::IsHost()> @ imm = #0x417c34
  3f7604: e3500000     	cmp	r0, #0
  3f7608: 0afffe23     	beq	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x774
  3f760c: e5973660     	ldr	r3, [r7, #0x660]
  3f7610: e3530000     	cmp	r3, #0
  3f7614: 0afffe20     	beq	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x780
  3f7618: e30124e8     	movw	r2, #0x14e8
  3f761c: e7930002     	ldr	r0, [r3, r2]
  3f7620: e3500000     	cmp	r0, #0
  3f7624: 0afffe1c     	beq	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x790
  3f7628: eb01c0ee     	bl	0x4679e8 <PlayerSavegame::SG_TryQuestSync()> @ imm = #0x703b8
  3f762c: eafffe1a     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x798
  3f7630: ebfce442     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc6ef8
  3f7634: e59f1374     	ldr	r1, [pc, #0x374]        @ 0x3f79b0 <Level::_LoadProcess()+0x1020>
  3f7638: e28d7f6d     	add	r7, sp, #436
  3f763c: e28d20c0     	add	r2, sp, #192
  3f7640: e1a08000     	mov	r8, r0
  3f7644: e08f1001     	add	r1, pc, r1
  3f7648: e1a00007     	mov	r0, r7
  3f764c: ebfc72a6     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3568
  3f7650: e1a01007     	mov	r1, r7
  3f7654: e1a00008     	mov	r0, r8
  3f7658: ebfd010a     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbfbd8
  3f765c: e1a00007     	mov	r0, r7
  3f7660: ebfc70d1     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe3cbc
  3f7664: e5d430f1     	ldrb	r3, [r4, #0xf1]
  3f7668: e3530000     	cmp	r3, #0
  3f766c: 1a0001f3     	bne	0x3f7e40 <Level::_LoadProcess()+0x14b0> @ imm = #0x7cc
  3f7670: e59f73a0     	ldr	r7, [pc, #0x3a0]        @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f7674: e7953007     	ldr	r3, [r5, r7]
  3f7678: e3a02001     	mov	r2, #1
  3f767c: e3a01000     	mov	r1, #0
  3f7680: e5930040     	ldr	r0, [r3, #0x40]
  3f7684: ebfddb7b     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x89214
  3f7688: e3a01000     	mov	r1, #0
  3f768c: e5900660     	ldr	r0, [r0, #0x660]
  3f7690: ebff1040     	bl	0x3bb798 <Character::SG_SetUseSpawnPoint(bool)> @ imm = #-0x3bf00
  3f7694: e5943130     	ldr	r3, [r4, #0x130]
  3f7698: e3a02000     	mov	r2, #0
  3f769c: e5c420f3     	strb	r2, [r4, #0xf3]
  3f76a0: e2833001     	add	r3, r3, #1
  3f76a4: e5843130     	str	r3, [r4, #0x130]
  3f76a8: eafffdfb     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x814
  3f76ac: ebfce423     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc6f74
  3f76b0: e59f12fc     	ldr	r1, [pc, #0x2fc]        @ 0x3f79b4 <Level::_LoadProcess()+0x1024>
  3f76b4: e1a08000     	mov	r8, r0
  3f76b8: e28d7f9d     	add	r7, sp, #628
  3f76bc: e28d20e0     	add	r2, sp, #224
  3f76c0: e08f1001     	add	r1, pc, r1
  3f76c4: eaffff59     	b	0x3f7430 <Level::_LoadProcess()+0xaa0> @ imm = #-0x29c
  3f76c8: ebfce41c     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc6f90
  3f76cc: e59f12e4     	ldr	r1, [pc, #0x2e4]        @ 0x3f79b8 <Level::_LoadProcess()+0x1028>
  3f76d0: e28d7fa3     	add	r7, sp, #652
  3f76d4: e28d20e4     	add	r2, sp, #228
  3f76d8: e1a08000     	mov	r8, r0
  3f76dc: e08f1001     	add	r1, pc, r1
  3f76e0: e1a00007     	mov	r0, r7
  3f76e4: ebfc7280     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3600
  3f76e8: e1a01007     	mov	r1, r7
  3f76ec: e1a00008     	mov	r0, r8
  3f76f0: ebfd00e4     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbfc70
  3f76f4: e1a00007     	mov	r0, r7
  3f76f8: ebfc70ab     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe3d54
  3f76fc: e1a00004     	mov	r0, r4
  3f7700: ebffe224     	bl	0x3eff98 <Level::_LoadCharStates()> @ imm = #-0x7770
  3f7704: e5943130     	ldr	r3, [r4, #0x130]
  3f7708: e2833001     	add	r3, r3, #1
  3f770c: e5843130     	str	r3, [r4, #0x130]
  3f7710: eafffde1     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x87c
  3f7714: e59f32fc     	ldr	r3, [pc, #0x2fc]        @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f7718: e1a00004     	mov	r0, r4
  3f771c: e3a085fe     	mov	r8, #1065353216
  3f7720: e7957003     	ldr	r7, [r5, r3]
  3f7724: e5973010     	ldr	r3, [r7, #0x10]
  3f7728: e593b01c     	ldr	r11, [r3, #0x1c]
  3f772c: ebffdf5d     	bl	0x3ef4a8 <Level::GetLevelConfig() const> @ imm = #-0x828c
  3f7730: e59091cc     	ldr	r9, [r0, #0x1cc]
  3f7734: e1a00004     	mov	r0, r4
  3f7738: ebffdf5a     	bl	0x3ef4a8 <Level::GetLevelConfig() const> @ imm = #-0x8298
  3f773c: e590a1d0     	ldr	r10, [r0, #0x1d0]
  3f7740: e1a00004     	mov	r0, r4
  3f7744: ebffdf57     	bl	0x3ef4a8 <Level::GetLevelConfig() const> @ imm = #-0x82a4
  3f7748: e59031d4     	ldr	r3, [r0, #0x1d4]
  3f774c: e28d1058     	add	r1, sp, #88
  3f7750: e1a0000b     	mov	r0, r11
  3f7754: e2411004     	sub	r1, r1, #4
  3f7758: e58d305c     	str	r3, [sp, #0x5c]
  3f775c: e58d8060     	str	r8, [sp, #0x60]
  3f7760: e58d9054     	str	r9, [sp, #0x54]
  3f7764: e58da058     	str	r10, [sp, #0x58]
  3f7768: eb064766     	bl	0x589508 <glitch::scene::CSceneManager::setAmbientLight(glitch::video::SColorf const&)> @ imm = #0x191d98
  3f776c: e59f32bc     	ldr	r3, [pc, #0x2bc]        @ 0x3f7a30 <Level::_LoadProcess()+0x10a0>
  3f7770: e1a01008     	mov	r1, r8
  3f7774: e5970038     	ldr	r0, [r7, #0x38]
  3f7778: e7958003     	ldr	r8, [r5, r3]
  3f777c: e3a03001     	mov	r3, #1
  3f7780: e5c83094     	strb	r3, [r8, #0x94]
  3f7784: ebfd4ba5     	bl	0x34a620 <ObjectManager::Update(float)> @ imm = #-0xad16c
  3f7788: ebff5c8a     	bl	0x3ce9b8 <CharAI::IncUpdateQueue()> @ imm = #-0x28dd8
  3f778c: e5970044     	ldr	r0, [r7, #0x44]
  3f7790: ebfd515c     	bl	0x34bd08 <PhysicalWorld::update()> @ imm = #-0xaba90
  3f7794: e3a03000     	mov	r3, #0
  3f7798: e5c83094     	strb	r3, [r8, #0x94]
  3f779c: e5943128     	ldr	r3, [r4, #0x128]
  3f77a0: e1a00003     	mov	r0, r3
  3f77a4: e5933000     	ldr	r3, [r3]
  3f77a8: e1a0e00f     	mov	lr, pc
  3f77ac: e593f010     	ldr	pc, [r3, #0x10]
  3f77b0: e5943130     	ldr	r3, [r4, #0x130]
  3f77b4: e2833001     	add	r3, r3, #1
  3f77b8: e5843130     	str	r3, [r4, #0x130]
  3f77bc: eafffdb6     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x928
  3f77c0: ebfce3de     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc7088
  3f77c4: e59f11f0     	ldr	r1, [pc, #0x1f0]        @ 0x3f79bc <Level::_LoadProcess()+0x102c>
  3f77c8: e28d7fa9     	add	r7, sp, #676
  3f77cc: e28d20e8     	add	r2, sp, #232
  3f77d0: e1a08000     	mov	r8, r0
  3f77d4: e08f1001     	add	r1, pc, r1
  3f77d8: e1a00007     	mov	r0, r7
  3f77dc: ebfc7242     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe36f8
  3f77e0: e1a01007     	mov	r1, r7
  3f77e4: e1a00008     	mov	r0, r8
  3f77e8: ebfd00a6     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbfd68
  3f77ec: e1a00007     	mov	r0, r7
  3f77f0: ebfc706d     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe3e4c
  3f77f4: e1a00004     	mov	r0, r4
  3f77f8: ebffe129     	bl	0x3efca4 <Level::_LoadFinalInit()> @ imm = #-0x7b5c
  3f77fc: e5943130     	ldr	r3, [r4, #0x130]
  3f7800: e2833001     	add	r3, r3, #1
  3f7804: e5843130     	str	r3, [r4, #0x130]
  3f7808: eafffda3     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x974
  3f780c: ebfce3cb     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc70d4
  3f7810: e59f11a8     	ldr	r1, [pc, #0x1a8]        @ 0x3f79c0 <Level::_LoadProcess()+0x1030>
  3f7814: e1a08000     	mov	r8, r0
  3f7818: e28d7faf     	add	r7, sp, #700
  3f781c: e28d20ec     	add	r2, sp, #236
  3f7820: e08f1001     	add	r1, pc, r1
  3f7824: eaffff01     	b	0x3f7430 <Level::_LoadProcess()+0xaa0> @ imm = #-0x3fc
  3f7828: ebfce3c4     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc70f0
  3f782c: e59f1190     	ldr	r1, [pc, #0x190]        @ 0x3f79c4 <Level::_LoadProcess()+0x1034>
  3f7830: e28d7fb5     	add	r7, sp, #724
  3f7834: e28d20f0     	add	r2, sp, #240
  3f7838: e1a08000     	mov	r8, r0
  3f783c: e08f1001     	add	r1, pc, r1
  3f7840: e1a00007     	mov	r0, r7
  3f7844: ebfc7228     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3760
  3f7848: e1a01007     	mov	r1, r7
  3f784c: e1a00008     	mov	r0, r8
  3f7850: ebfd008c     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbfdd0
  3f7854: e1a00007     	mov	r0, r7
  3f7858: ebfc7053     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe3eb4
  3f785c: e59f3144     	ldr	r3, [pc, #0x144]        @ 0x3f79a8 <Level::_LoadProcess()+0x1018>
  3f7860: e594103c     	ldr	r1, [r4, #0x3c]
  3f7864: e7953003     	ldr	r3, [r5, r3]
  3f7868: e5930000     	ldr	r0, [r3]
  3f786c: ebfdc760     	bl	0x3695f4 <VoxSoundManager::SetLevelRouting(int)> @ imm = #-0x8e280
  3f7870: e5943130     	ldr	r3, [r4, #0x130]
  3f7874: e2833001     	add	r3, r3, #1
  3f7878: e5843130     	str	r3, [r4, #0x130]
  3f787c: eafffd86     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x9e8
  3f7880: ebfce3ae     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc7148
  3f7884: e59f113c     	ldr	r1, [pc, #0x13c]        @ 0x3f79c8 <Level::_LoadProcess()+0x1038>
  3f7888: e28d7fbb     	add	r7, sp, #748
  3f788c: e28d20f4     	add	r2, sp, #244
  3f7890: e1a08000     	mov	r8, r0
  3f7894: e08f1001     	add	r1, pc, r1
  3f7898: e1a00007     	mov	r0, r7
  3f789c: ebfc7212     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe37b8
  3f78a0: e1a01007     	mov	r1, r7
  3f78a4: e1a00008     	mov	r0, r8
  3f78a8: ebfd0076     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbfe28
  3f78ac: e1a00007     	mov	r0, r7
  3f78b0: ebfc703d     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe3f0c
  3f78b4: e1a00004     	mov	r0, r4
  3f78b8: ebffe1d6     	bl	0x3f0018 <Level::_LoadPlayer()> @ imm = #-0x78a8
  3f78bc: eb1017b4     	bl	0x7fd794 <GetOnline()>  @ imm = #0x405ed0
  3f78c0: e5d03005     	ldrb	r3, [r0, #0x5]
  3f78c4: e3530000     	cmp	r3, #0
  3f78c8: 0afffd70     	beq	0x3f6e90 <Level::_LoadProcess()+0x500> @ imm = #-0xa40
  3f78cc: e59f3144     	ldr	r3, [pc, #0x144]        @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f78d0: e3a01000     	mov	r1, #0
  3f78d4: e1a02001     	mov	r2, r1
  3f78d8: e7957003     	ldr	r7, [r5, r3]
  3f78dc: e5970040     	ldr	r0, [r7, #0x40]
  3f78e0: ebfddae4     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x89470
  3f78e4: e5903000     	ldr	r3, [r0]
  3f78e8: e1a0e00f     	mov	lr, pc
  3f78ec: e593f05c     	ldr	pc, [r3, #0x5c]
  3f78f0: e3500000     	cmp	r0, #0
  3f78f4: 1afffd65     	bne	0x3f6e90 <Level::_LoadProcess()+0x500> @ imm = #-0xa6c
  3f78f8: eb1025a3     	bl	0x800f8c <CMatching::Get()> @ imm = #0x40968c
  3f78fc: e5903000     	ldr	r3, [r0]
  3f7900: e1a0e00f     	mov	lr, pc
  3f7904: e593f03c     	ldr	pc, [r3, #0x3c]
  3f7908: eb101119     	bl	0x7fbd74 <GetConnectionMgr()> @ imm = #0x404464
  3f790c: eb101150     	bl	0x7fbe54 <CConnectionManager::DisconnectAll()> @ imm = #0x404540
  3f7910: e1a00007     	mov	r0, r7
  3f7914: e3a01003     	mov	r1, #3
  3f7918: ebfcd235     	bl	0x32c1f4 <Application::GoToMainMenu(GoToMainMenuEvent)> @ imm = #-0xcb72c
  3f791c: eafffd5b     	b	0x3f6e90 <Level::_LoadProcess()+0x500> @ imm = #-0xa94
  3f7920: f0 e0 59 00  	.word	0x0059e0f0
  3f7924: b0 33 00 00  	.word	0x000033b0
  3f7928: ac 40 00 00  	.word	0x000040ac
  3f792c: 84 08 00 00  	.word	0x00000884
  3f7930: b8 00 4d 00  	.word	0x004d00b8
  3f7934: a8 00 4d 00  	.word	0x004d00a8
  3f7938: d0 ff 4c 00  	.word	0x004cffd0
  3f793c: 54 8c 4c 00  	.word	0x004c8c54
  3f7940: d0 b0 4c 00  	.word	0x004cb0d0
  3f7944: b4 b0 4c 00  	.word	0x004cb0b4
  3f7948: 0c 21 00 00  	.word	0x0000210c
  3f794c: b4 7d 4c 00  	.word	0x004c7db4
  3f7950: 34 22 00 00  	.word	0x00002234
  3f7954: 08 45 00 00  	.word	0x00004508
  3f7958: 70 20 00 00  	.word	0x00002070
  3f795c: c4 35 00 00  	.word	0x000035c4
  3f7960: 4c a7 4c 00  	.word	0x004ca74c
  3f7964: 58 a7 4c 00  	.word	0x004ca758
  3f7968: 88 fa 4c 00  	.word	0x004cfa88
  3f796c: 38 fa 4c 00  	.word	0x004cfa38
  3f7970: dc f9 4c 00  	.word	0x004cf9dc
  3f7974: 90 f9 4c 00  	.word	0x004cf990
  3f7978: 94 0c 00 00  	.word	0x00000c94
  3f797c: 10 0b 00 00  	.word	0x00000b10
  3f7980: d0 f8 4c 00  	.word	0x004cf8d0
  3f7984: 3c f5 4c 00  	.word	0x004cf53c
  3f7988: 34 f8 4c 00  	.word	0x004cf834
  3f798c: b4 f7 4c 00  	.word	0x004cf7b4
  3f7990: 40 f7 4c 00  	.word	0x004cf740
  3f7994: ec f6 4c 00  	.word	0x004cf6ec
  3f7998: a4 f6 4c 00  	.word	0x004cf6a4
  3f799c: 7c 42 00 00  	.word	0x0000427c
  3f79a0: 84 16 00 00  	.word	0x00001684
  3f79a4: 08 f6 4c 00  	.word	0x004cf608
  3f79a8: a4 0d 00 00  	.word	0x00000da4
  3f79ac: a8 f5 4c 00  	.word	0x004cf5a8
  3f79b0: d4 f4 4c 00  	.word	0x004cf4d4
  3f79b4: 58 f4 4c 00  	.word	0x004cf458
  3f79b8: 3c f4 4c 00  	.word	0x004cf43c
  3f79bc: 44 f3 4c 00  	.word	0x004cf344
  3f79c0: f8 f2 4c 00  	.word	0x004cf2f8
  3f79c4: dc f2 4c 00  	.word	0x004cf2dc
  3f79c8: 84 f2 4c 00  	.word	0x004cf284
  3f79cc: d0 f0 4c 00  	.word	0x004cf0d0
  3f79d0: 20 1a 00 00  	.word	0x00001a20
  3f79d4: 64 f0 4c 00  	.word	0x004cf064
  3f79d8: 18 f0 4c 00  	.word	0x004cf018
  3f79dc: cc ef 4c 00  	.word	0x004cefcc
  3f79e0: 80 ef 4c 00  	.word	0x004cef80
  3f79e4: 18 ef 4c 00  	.word	0x004cef18
  3f79e8: 08 1b 00 00  	.word	0x00001b08
  3f79ec: c8 ee 4c 00  	.word	0x004ceec8
  3f79f0: 2c 0e 00 00  	.word	0x00000e2c
  3f79f4: 4c 08 00 00  	.word	0x0000084c
  3f79f8: 68 ee 4c 00  	.word	0x004cee68
  3f79fc: f4 a2 4c 00  	.word	0x004ca2f4
  3f7a00: a8 9a 4c 00  	.word	0x004c9aa8
  3f7a04: 64 ee 4c 00  	.word	0x004cee64
  3f7a08: 84 ed 4c 00  	.word	0x004ced84
  3f7a0c: 60 ed 4c 00  	.word	0x004ced60
  3f7a10: 10 ed 4c 00  	.word	0x004ced10
  3f7a14: d4 ea 4c 00  	.word	0x004cead4
  3f7a18: f4 37 00 00  	.word	0x000037f4
  3f7a1c: 98 ea 4c 00  	.word	0x004cea98
  3f7a20: f0 e6 4c 00  	.word	0x004ce6f0
  3f7a24: 24 83 4c 00  	.word	0x004c8324
  3f7a28: 00 ea 4c 00  	.word	0x004cea00
  3f7a2c: 2c 3f 00 00  	.word	0x00003f2c
  3f7a30: 04 12 00 00  	.word	0x00001204
  3f7a34: ebfce341     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc72fc
  3f7a38: e51f1074     	ldr	r1, [pc, #-0x74]        @ 0x3f79cc <Level::_LoadProcess()+0x103c>
  3f7a3c: e28d7fc1     	add	r7, sp, #772
  3f7a40: e28d20f8     	add	r2, sp, #248
  3f7a44: e1a08000     	mov	r8, r0
  3f7a48: e08f1001     	add	r1, pc, r1
  3f7a4c: e1a00007     	mov	r0, r7
  3f7a50: ebfc71a5     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe396c
  3f7a54: e1a01007     	mov	r1, r7
  3f7a58: e1a00008     	mov	r0, r8
  3f7a5c: ebfd0009     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbffdc
  3f7a60: e1a00007     	mov	r0, r7
  3f7a64: ebfc6fd0     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe40c0
  3f7a68: e51f30a0     	ldr	r3, [pc, #-0xa0]        @ 0x3f79d0 <Level::_LoadProcess()+0x1040>
  3f7a6c: e7950003     	ldr	r0, [r5, r3]
  3f7a70: eb017818     	bl	0x455ad8 <ScriptManager::InitCommands()> @ imm = #0x5e060
  3f7a74: e5943130     	ldr	r3, [r4, #0x130]
  3f7a78: e2833001     	add	r3, r3, #1
  3f7a7c: e5843130     	str	r3, [r4, #0x130]
  3f7a80: eafffd05     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0xbec
  3f7a84: eb00d400     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0x35000
  3f7a88: e3a01001     	mov	r1, #1
  3f7a8c: eb00e904     	bl	0x431ea4 <MenuManager::LoadMenu(int)> @ imm = #0x3a410
  3f7a90: e5943130     	ldr	r3, [r4, #0x130]
  3f7a94: e2833001     	add	r3, r3, #1
  3f7a98: e5843130     	str	r3, [r4, #0x130]
  3f7a9c: eafffcfe     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0xc08
  3f7aa0: ebfce326     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc7368
  3f7aa4: e51f10d8     	ldr	r1, [pc, #-0xd8]        @ 0x3f79d4 <Level::_LoadProcess()+0x1044>
  3f7aa8: e28d7f85     	add	r7, sp, #532
  3f7aac: e28d20d0     	add	r2, sp, #208
  3f7ab0: e1a08000     	mov	r8, r0
  3f7ab4: e08f1001     	add	r1, pc, r1
  3f7ab8: e1a00007     	mov	r0, r7
  3f7abc: ebfc718a     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe39d8
  3f7ac0: e1a01007     	mov	r1, r7
  3f7ac4: e1a00008     	mov	r0, r8
  3f7ac8: ebfcffee     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc0048
  3f7acc: e1a00007     	mov	r0, r7
  3f7ad0: ebfc6fb5     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe412c
  3f7ad4: e1a00004     	mov	r0, r4
  3f7ad8: ebfff8f0     	bl	0x3f5ea0 <Level::_LoadBatchMap()> @ imm = #-0x1c40
  3f7adc: e5943130     	ldr	r3, [r4, #0x130]
  3f7ae0: e2833001     	add	r3, r3, #1
  3f7ae4: e5843130     	str	r3, [r4, #0x130]
  3f7ae8: eafffceb     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0xc54
  3f7aec: ebfce313     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc73b4
  3f7af0: e51f1120     	ldr	r1, [pc, #-0x120]       @ 0x3f79d8 <Level::_LoadProcess()+0x1048>
  3f7af4: e28d7f8b     	add	r7, sp, #556
  3f7af8: e28d20d4     	add	r2, sp, #212
  3f7afc: e1a08000     	mov	r8, r0
  3f7b00: e08f1001     	add	r1, pc, r1
  3f7b04: e1a00007     	mov	r0, r7
  3f7b08: ebfc7177     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3a24
  3f7b0c: e1a01007     	mov	r1, r7
  3f7b10: e1a00008     	mov	r0, r8
  3f7b14: ebfcffdb     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc0094
  3f7b18: e1a00007     	mov	r0, r7
  3f7b1c: ebfc6fa2     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe4178
  3f7b20: e1a00004     	mov	r0, r4
  3f7b24: ebffec83     	bl	0x3f2d38 <Level::_LoadBatchList()> @ imm = #-0x4df4
  3f7b28: e5943130     	ldr	r3, [r4, #0x130]
  3f7b2c: e2833001     	add	r3, r3, #1
  3f7b30: e5843130     	str	r3, [r4, #0x130]
  3f7b34: eafffcd8     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0xca0
  3f7b38: ebfce300     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc7400
  3f7b3c: e51f1168     	ldr	r1, [pc, #-0x168]       @ 0x3f79dc <Level::_LoadProcess()+0x104c>
  3f7b40: e28d7f91     	add	r7, sp, #580
  3f7b44: e28d20d8     	add	r2, sp, #216
  3f7b48: e1a08000     	mov	r8, r0
  3f7b4c: e08f1001     	add	r1, pc, r1
  3f7b50: e1a00007     	mov	r0, r7
  3f7b54: ebfc7164     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3a70
  3f7b58: e1a01007     	mov	r1, r7
  3f7b5c: e1a00008     	mov	r0, r8
  3f7b60: ebfcffc8     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc00e0
  3f7b64: e1a00007     	mov	r0, r7
  3f7b68: ebfc6f8f     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe41c4
  3f7b6c: e1a00004     	mov	r0, r4
  3f7b70: ebffea25     	bl	0x3f240c <Level::_LoadBatchInit()> @ imm = #-0x576c
  3f7b74: e5943130     	ldr	r3, [r4, #0x130]
  3f7b78: e2833001     	add	r3, r3, #1
  3f7b7c: e5843130     	str	r3, [r4, #0x130]
  3f7b80: eafffcc5     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0xcec
  3f7b84: ebfce2ed     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc744c
  3f7b88: e51f11b0     	ldr	r1, [pc, #-0x1b0]       @ 0x3f79e0 <Level::_LoadProcess()+0x1050>
  3f7b8c: e28d7f97     	add	r7, sp, #604
  3f7b90: e28d20dc     	add	r2, sp, #220
  3f7b94: e1a08000     	mov	r8, r0
  3f7b98: e08f1001     	add	r1, pc, r1
  3f7b9c: e1a00007     	mov	r0, r7
  3f7ba0: ebfc7151     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3abc
  3f7ba4: e1a01007     	mov	r1, r7
  3f7ba8: e1a00008     	mov	r0, r8
  3f7bac: ebfcffb5     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc012c
  3f7bb0: e1a00007     	mov	r0, r7
  3f7bb4: ebfc6f7c     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe4210
  3f7bb8: e1a00004     	mov	r0, r4
  3f7bbc: ebffe511     	bl	0x3f1008 <Level::_LoadCamera()> @ imm = #-0x6bbc
  3f7bc0: e5943130     	ldr	r3, [r4, #0x130]
  3f7bc4: e2833001     	add	r3, r3, #1
  3f7bc8: e5843130     	str	r3, [r4, #0x130]
  3f7bcc: eafffcb2     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0xd38
  3f7bd0: e5940194     	ldr	r0, [r4, #0x194]
  3f7bd4: eb0206e8     	bl	0x47977c <GameEventManager::Compile()> @ imm = #0x81ba0
  3f7bd8: e5943130     	ldr	r3, [r4, #0x130]
  3f7bdc: e2833001     	add	r3, r3, #1
  3f7be0: e5843130     	str	r3, [r4, #0x130]
  3f7be4: eafffcac     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0xd50
  3f7be8: ebfce2d4     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc74b0
  3f7bec: e51f1210     	ldr	r1, [pc, #-0x210]       @ 0x3f79e4 <Level::_LoadProcess()+0x1054>
  3f7bf0: e28d7e45     	add	r7, sp, #1104
  3f7bf4: e2877004     	add	r7, r7, #4
  3f7bf8: e28d2f4b     	add	r2, sp, #300
  3f7bfc: e1a08000     	mov	r8, r0
  3f7c00: e08f1001     	add	r1, pc, r1
  3f7c04: e1a00007     	mov	r0, r7
  3f7c08: ebfc7137     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3b24
  3f7c0c: e1a01007     	mov	r1, r7
  3f7c10: e1a00008     	mov	r0, r8
  3f7c14: ebfcff9b     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc0194
  3f7c18: e1a00007     	mov	r0, r7
  3f7c1c: ebfc6f62     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe4278
  3f7c20: e51f3240     	ldr	r3, [pc, #-0x240]       @ 0x3f79e8 <Level::_LoadProcess()+0x1058>
  3f7c24: e7950003     	ldr	r0, [r5, r3]
  3f7c28: eb027796     	bl	0x495a88 <VisualFXManager::PreCacheLibraries()> @ imm = #0x9de58
  3f7c2c: e5943130     	ldr	r3, [r4, #0x130]
  3f7c30: e2833001     	add	r3, r3, #1
  3f7c34: e5843130     	str	r3, [r4, #0x130]
  3f7c38: eafffc97     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0xda4
  3f7c3c: ebfce2bf     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc7504
  3f7c40: e51f125c     	ldr	r1, [pc, #-0x25c]       @ 0x3f79ec <Level::_LoadProcess()+0x105c>
  3f7c44: e28d7f73     	add	r7, sp, #460
  3f7c48: e28d20c4     	add	r2, sp, #196
  3f7c4c: e1a08000     	mov	r8, r0
  3f7c50: e08f1001     	add	r1, pc, r1
  3f7c54: e1a00007     	mov	r0, r7
  3f7c58: ebfc7123     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3b74
  3f7c5c: e1a01007     	mov	r1, r7
  3f7c60: e1a00008     	mov	r0, r8
  3f7c64: ebfcff87     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc01e4
  3f7c68: e1a00007     	mov	r0, r7
  3f7c6c: ebfc6f4e     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe42c8
  3f7c70: e51f3288     	ldr	r3, [pc, #-0x288]       @ 0x3f79f0 <Level::_LoadProcess()+0x1060>
  3f7c74: e7950003     	ldr	r0, [r5, r3]
  3f7c78: ebffce99     	bl	0x3eb6e4 <ItemManager::PreCache()> @ imm = #-0xc59c
  3f7c7c: e51f3290     	ldr	r3, [pc, #-0x290]       @ 0x3f79f4 <Level::_LoadProcess()+0x1064>
  3f7c80: e7950003     	ldr	r0, [r5, r3]
  3f7c84: ebffbd50     	bl	0x3e71cc <ProjectileManager::PreCache()> @ imm = #-0x10ac0
  3f7c88: e5943130     	ldr	r3, [r4, #0x130]
  3f7c8c: e2833001     	add	r3, r3, #1
  3f7c90: e5843130     	str	r3, [r4, #0x130]
  3f7c94: eafffc80     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0xe00
  3f7c98: ebfce2a8     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc7560
  3f7c9c: e51f12ac     	ldr	r1, [pc, #-0x2ac]       @ 0x3f79f8 <Level::_LoadProcess()+0x1068>
  3f7ca0: e28d7f55     	add	r7, sp, #340
  3f7ca4: e28d80b8     	add	r8, sp, #184
  3f7ca8: e2482008     	sub	r2, r8, #8
  3f7cac: e1a0a000     	mov	r10, r0
  3f7cb0: e08f1001     	add	r1, pc, r1
  3f7cb4: e1a00007     	mov	r0, r7
  3f7cb8: ebfc710b     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3bd4
  3f7cbc: e1a01007     	mov	r1, r7
  3f7cc0: e1a0000a     	mov	r0, r10
  3f7cc4: ebfcff6f     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc0244
  3f7cc8: e1a00007     	mov	r0, r7
  3f7ccc: ebfc6f36     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe4328
  3f7cd0: eb00d36d     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0x34db4
  3f7cd4: e51f12e0     	ldr	r1, [pc, #-0x2e0]       @ 0x3f79fc <Level::_LoadProcess()+0x106c>
  3f7cd8: e1a07000     	mov	r7, r0
  3f7cdc: e08f1001     	add	r1, pc, r1
  3f7ce0: eb00d542     	bl	0x42d1f0 <MenuManager::GetMenuByName(char const*)> @ imm = #0x35508
  3f7ce4: e1a0a000     	mov	r10, r0
  3f7ce8: eb009dc1     	bl	0x41f3f4 <MenuBase::IsVisible() const> @ imm = #0x27704
  3f7cec: e3500000     	cmp	r0, #0
  3f7cf0: 1a00008a     	bne	0x3f7f20 <Level::_LoadProcess()+0x1590> @ imm = #0x228
  3f7cf4: e51f12fc     	ldr	r1, [pc, #-0x2fc]       @ 0x3f7a00 <Level::_LoadProcess()+0x1070>
  3f7cf8: e1a00007     	mov	r0, r7
  3f7cfc: e28d7f4f     	add	r7, sp, #316
  3f7d00: e08f1001     	add	r1, pc, r1
  3f7d04: eb00d539     	bl	0x42d1f0 <MenuManager::GetMenuByName(char const*)> @ imm = #0x354e4
  3f7d08: e51f030c     	ldr	r0, [pc, #-0x30c]       @ 0x3f7a04 <Level::_LoadProcess()+0x1074>
  3f7d0c: e08f0000     	add	r0, pc, r0
  3f7d10: ebfc61c1     	bl	0x31041c <ShowMemoryStats(char const*)> @ imm = #-0xe78fc
  3f7d14: ebfce289     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc75dc
  3f7d18: e51f1318     	ldr	r1, [pc, #-0x318]       @ 0x3f7a08 <Level::_LoadProcess()+0x1078>
  3f7d1c: e248200c     	sub	r2, r8, #12
  3f7d20: e1a0a000     	mov	r10, r0
  3f7d24: e08f1001     	add	r1, pc, r1
  3f7d28: e1a00007     	mov	r0, r7
  3f7d2c: ebfc70ee     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3c48
  3f7d30: e1a01007     	mov	r1, r7
  3f7d34: e1a0000a     	mov	r0, r10
  3f7d38: ebfcff52     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc02b8
  3f7d3c: e1a08000     	mov	r8, r0
  3f7d40: e1a00007     	mov	r0, r7
  3f7d44: ebfc6f18     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe43a0
  3f7d48: e3580000     	cmp	r8, #0
  3f7d4c: 1a00006f     	bne	0x3f7f10 <Level::_LoadProcess()+0x1580> @ imm = #0x1bc
  3f7d50: e51f7340     	ldr	r7, [pc, #-0x340]       @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f7d54: e3a08000     	mov	r8, #0
  3f7d58: e1a0a008     	mov	r10, r8
  3f7d5c: e7957007     	ldr	r7, [r5, r7]
  3f7d60: e5970040     	ldr	r0, [r7, #0x40]
  3f7d64: e3a01000     	mov	r1, #0
  3f7d68: ebfddb58     	bl	0x36ead0 <PlayerManager::GetNumLocalPlayers(bool)> @ imm = #-0x892a0
  3f7d6c: e1580000     	cmp	r8, r0
  3f7d70: aa000055     	bge	0x3f7ecc <Level::_LoadProcess()+0x153c> @ imm = #0x154
  3f7d74: e5970040     	ldr	r0, [r7, #0x40]
  3f7d78: e1a01008     	mov	r1, r8
  3f7d7c: e3a02000     	mov	r2, #0
  3f7d80: ebfdd9bc     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x89910
  3f7d84: e5903660     	ldr	r3, [r0, #0x660]
  3f7d88: e3530000     	cmp	r3, #0
  3f7d8c: 0a000002     	beq	0x3f7d9c <Level::_LoadProcess()+0x140c> @ imm = #0x8
  3f7d90: e5933378     	ldr	r3, [r3, #0x378]
  3f7d94: e3530000     	cmp	r3, #0
  3f7d98: 15c3a008     	strbne	r10, [r3, #0x8]
  3f7d9c: e2888001     	add	r8, r8, #1
  3f7da0: eaffffee     	b	0x3f7d60 <Level::_LoadProcess()+0x13d0> @ imm = #-0x48
  3f7da4: ebfce265     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc766c
  3f7da8: e51f13a4     	ldr	r1, [pc, #-0x3a4]       @ 0x3f7a0c <Level::_LoadProcess()+0x107c>
  3f7dac: e28d7f79     	add	r7, sp, #484
  3f7db0: e28d20c8     	add	r2, sp, #200
  3f7db4: e1a08000     	mov	r8, r0
  3f7db8: e08f1001     	add	r1, pc, r1
  3f7dbc: e1a00007     	mov	r0, r7
  3f7dc0: ebfc70c9     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3cdc
  3f7dc4: e1a01007     	mov	r1, r7
  3f7dc8: e1a00008     	mov	r0, r8
  3f7dcc: ebfcff2d     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc034c
  3f7dd0: e1a00007     	mov	r0, r7
  3f7dd4: ebfc6ef4     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe4430
  3f7dd8: e51f33c8     	ldr	r3, [pc, #-0x3c8]       @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f7ddc: e7950003     	ldr	r0, [r5, r3]
  3f7de0: ebfc9ddd     	bl	0x31f55c <Application::CleanGlitch()> @ imm = #-0xd888c
  3f7de4: e5943130     	ldr	r3, [r4, #0x130]
  3f7de8: e2833001     	add	r3, r3, #1
  3f7dec: e5843130     	str	r3, [r4, #0x130]
  3f7df0: eafffc29     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0xf5c
  3f7df4: ebfce251     	bl	0x330740 <DebugSwitches::GetInstance()> @ imm = #-0xc76bc
  3f7df8: e51f13f0     	ldr	r1, [pc, #-0x3f0]       @ 0x3f7a10 <Level::_LoadProcess()+0x1080>
  3f7dfc: e28d7f7f     	add	r7, sp, #508
  3f7e00: e28d20cc     	add	r2, sp, #204
  3f7e04: e1a08000     	mov	r8, r0
  3f7e08: e08f1001     	add	r1, pc, r1
  3f7e0c: e1a00007     	mov	r0, r7
  3f7e10: ebfc70b5     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3d2c
  3f7e14: e1a01007     	mov	r1, r7
  3f7e18: e1a00008     	mov	r0, r8
  3f7e1c: ebfcff19     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xc039c
  3f7e20: e1a00007     	mov	r0, r7
  3f7e24: ebfc6ee0     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe4480
  3f7e28: e1a00004     	mov	r0, r4
  3f7e2c: ebffeb2c     	bl	0x3f2ae4 <Level::_LoadBatching()> @ imm = #-0x5350
  3f7e30: e5943130     	ldr	r3, [r4, #0x130]
  3f7e34: e2833001     	add	r3, r3, #1
  3f7e38: e5843130     	str	r3, [r4, #0x130]
  3f7e3c: eafffc16     	b	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0xfa8
  3f7e40: e59400ec     	ldr	r0, [r4, #0xec]
  3f7e44: eb01a5c8     	bl	0x46156c <LevelSavegame::Load()> @ imm = #0x69720
  3f7e48: eb101651     	bl	0x7fd794 <GetOnline()>  @ imm = #0x405944
  3f7e4c: e5d03005     	ldrb	r3, [r0, #0x5]
  3f7e50: e3530000     	cmp	r3, #0
  3f7e54: 0afffe05     	beq	0x3f7670 <Level::_LoadProcess()+0xce0> @ imm = #-0x7ec
  3f7e58: e51f7448     	ldr	r7, [pc, #-0x448]       @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f7e5c: e795a007     	ldr	r10, [r5, r7]
  3f7e60: e59a0040     	ldr	r0, [r10, #0x40]
  3f7e64: ebfddc82     	bl	0x36f074 <PlayerManager::IsLocalPlayerHosting()> @ imm = #-0x88df8
  3f7e68: e3500000     	cmp	r0, #0
  3f7e6c: 1a0000e2     	bne	0x3f81fc <Level::_LoadProcess()+0x186c> @ imm = #0x388
  3f7e70: e59a3040     	ldr	r3, [r10, #0x40]
  3f7e74: e5d336d0     	ldrb	r3, [r3, #0x6d0]
  3f7e78: e3530000     	cmp	r3, #0
  3f7e7c: 11a08000     	movne	r8, r0
  3f7e80: 1a00000b     	bne	0x3f7eb4 <Level::_LoadProcess()+0x1524> @ imm = #0x2c
  3f7e84: eafffdfa     	b	0x3f7674 <Level::_LoadProcess()+0xce4> @ imm = #-0x818
  3f7e88: e1a01008     	mov	r1, r8
  3f7e8c: e3a02001     	mov	r2, #1
  3f7e90: e59a0040     	ldr	r0, [r10, #0x40]
  3f7e94: ebfdd977     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x89a24
  3f7e98: e59a1040     	ldr	r1, [r10, #0x40]
  3f7e9c: e5900660     	ldr	r0, [r0, #0x660]
  3f7ea0: e3a02001     	mov	r2, #1
  3f7ea4: e2811e6d     	add	r1, r1, #1744
  3f7ea8: e2811004     	add	r1, r1, #4
  3f7eac: ebfe6fc0     	bl	0x393db4 <GameObject::SetPosition(Point3D<float> const&, bool)> @ imm = #-0x64100
  3f7eb0: e2888001     	add	r8, r8, #1
  3f7eb4: e59a0040     	ldr	r0, [r10, #0x40]
  3f7eb8: e3a01001     	mov	r1, #1
  3f7ebc: ebfddb03     	bl	0x36ead0 <PlayerManager::GetNumLocalPlayers(bool)> @ imm = #-0x893f4
  3f7ec0: e1580000     	cmp	r8, r0
  3f7ec4: baffffef     	blt	0x3f7e88 <Level::_LoadProcess()+0x14f8> @ imm = #-0x44
  3f7ec8: eafffde9     	b	0x3f7674 <Level::_LoadProcess()+0xce4> @ imm = #-0x85c
  3f7ecc: eb101630     	bl	0x7fd794 <GetOnline()>  @ imm = #0x4058c0
  3f7ed0: e5d03005     	ldrb	r3, [r0, #0x5]
  3f7ed4: e3530000     	cmp	r3, #0
  3f7ed8: 0a000004     	beq	0x3f7ef0 <Level::_LoadProcess()+0x1560> @ imm = #0x10
  3f7edc: e5970038     	ldr	r0, [r7, #0x38]
  3f7ee0: e5d031ac     	ldrb	r3, [r0, #0x1ac]
  3f7ee4: e3530000     	cmp	r3, #0
  3f7ee8: 1a000000     	bne	0x3f7ef0 <Level::_LoadProcess()+0x1560> @ imm = #0x0
  3f7eec: ebfd233b     	bl	0x340be0 <ObjectManager::NetworkInitLevel()> @ imm = #-0xb7314
  3f7ef0: e1a00004     	mov	r0, r4
  3f7ef4: e3a01000     	mov	r1, #0
  3f7ef8: ebffe266     	bl	0x3f0898 <Level::PlaceFaeryAndFollowers(Character*)> @ imm = #-0x7668
  3f7efc: eafffbe3     	b	0x3f6e90 <Level::_LoadProcess()+0x500> @ imm = #-0x1074
  3f7f00: e5d430f5     	ldrb	r3, [r4, #0xf5]
  3f7f04: e3530000     	cmp	r3, #0
  3f7f08: 0afffbe0     	beq	0x3f6e90 <Level::_LoadProcess()+0x500> @ imm = #-0x1080
  3f7f0c: eafffbc6     	b	0x3f6e2c <Level::_LoadProcess()+0x49c> @ imm = #-0x10e8
  3f7f10: eb00c9d2     	bl	0x42a660 <MenuDebug::GetInstance()> @ imm = #0x32748
  3f7f14: e3a01000     	mov	r1, #0
  3f7f18: eb00c85b     	bl	0x42a08c <MenuDebug::SetText(char const*)> @ imm = #0x3216c
  3f7f1c: eaffff8b     	b	0x3f7d50 <Level::_LoadProcess()+0x13c0> @ imm = #-0x1d4
  3f7f20: e1a0100a     	mov	r1, r10
  3f7f24: e1a00007     	mov	r0, r7
  3f7f28: eb00d8b6     	bl	0x42e208 <MenuManager::PopMenu(MenuBase*)> @ imm = #0x362d8
  3f7f2c: eaffff70     	b	0x3f7cf4 <Level::_LoadProcess()+0x1364> @ imm = #-0x240
  3f7f30: e59a0040     	ldr	r0, [r10, #0x40]
  3f7f34: ebfddc4e     	bl	0x36f074 <PlayerManager::IsLocalPlayerHosting()> @ imm = #-0x88ec8
  3f7f38: e3500000     	cmp	r0, #0
  3f7f3c: 0afffbba     	beq	0x3f6e2c <Level::_LoadProcess()+0x49c> @ imm = #-0x1118
  3f7f40: e59a3040     	ldr	r3, [r10, #0x40]
  3f7f44: e5d33719     	ldrb	r3, [r3, #0x719]
  3f7f48: e3530000     	cmp	r3, #0
  3f7f4c: 1afffbb6     	bne	0x3f6e2c <Level::_LoadProcess()+0x49c> @ imm = #-0x1128
  3f7f50: eafffbac     	b	0x3f6e08 <Level::_LoadProcess()+0x478> @ imm = #-0x1150
  3f7f54: eb10240c     	bl	0x800f8c <CMatching::Get()> @ imm = #0x409030
  3f7f58: e5903000     	ldr	r3, [r0]
  3f7f5c: e1a0e00f     	mov	lr, pc
  3f7f60: e593f0a4     	ldr	pc, [r3, #0xa4]
  3f7f64: e3a01003     	mov	r1, #3
  3f7f68: e5903000     	ldr	r3, [r0]
  3f7f6c: e594203c     	ldr	r2, [r4, #0x3c]
  3f7f70: e1a0e00f     	mov	lr, pc
  3f7f74: e593f008     	ldr	pc, [r3, #0x8]
  3f7f78: eafffb10     	b	0x3f6bc0 <Level::_LoadProcess()+0x230> @ imm = #-0x13c0
  3f7f7c: eb101604     	bl	0x7fd794 <GetOnline()>  @ imm = #0x405810
  3f7f80: e5d03005     	ldrb	r3, [r0, #0x5]
  3f7f84: e3530000     	cmp	r3, #0
  3f7f88: 05978000     	ldreq	r8, [r7]
  3f7f8c: 15988000     	ldrne	r8, [r8]
  3f7f90: e28d7028     	add	r7, sp, #40
  3f7f94: e2477008     	sub	r7, r7, #8
  3f7f98: e1a00007     	mov	r0, r7
  3f7f9c: ebfc7b66     	bl	0x316d3c <StreamBuffer::StreamBuffer()> @ imm = #-0xe1268
  3f7fa0: e1a00004     	mov	r0, r4
  3f7fa4: e1a01007     	mov	r1, r7
  3f7fa8: e1a02008     	mov	r2, r8
  3f7fac: ebffe211     	bl	0x3f07f8 <Level::GenerateRandomLevel(StreamBuffer&, unsigned int)> @ imm = #-0x77bc
  3f7fb0: e3500000     	cmp	r0, #0
  3f7fb4: 1a000079     	bne	0x3f81a0 <Level::_LoadProcess()+0x1810> @ imm = #0x1e4
  3f7fb8: e594310c     	ldr	r3, [r4, #0x10c]
  3f7fbc: e3a02078     	mov	r2, #120
  3f7fc0: e5c32000     	strb	r2, [r3]
  3f7fc4: e5941108     	ldr	r1, [r4, #0x108]
  3f7fc8: e594010c     	ldr	r0, [r4, #0x10c]
  3f7fcc: e1510000     	cmp	r1, r0
  3f7fd0: 0a000070     	beq	0x3f8198 <Level::_LoadProcess()+0x1808> @ imm = #0x1c0
  3f7fd4: e28d20a8     	add	r2, sp, #168
  3f7fd8: e1a03002     	mov	r3, r2
  3f7fdc: e3a0c02e     	mov	r12, #46
  3f7fe0: e2422004     	sub	r2, r2, #4
  3f7fe4: e5cdc0a4     	strb	r12, [sp, #0xa4]
  3f7fe8: ebfd5b05     	bl	0x34ec04 <char const* std::priv::__find_if<char const*, std::priv::_Eq_char_bound<std::char_traits<char>>>(char const*, char const*, std::priv::_Eq_char_bound<std::char_traits<char>>, std::random_access_iterator_tag const&)> @ imm = #-0xa93ec
  3f7fec: e5942108     	ldr	r2, [r4, #0x108]
  3f7ff0: e1500002     	cmp	r0, r2
  3f7ff4: 0a000067     	beq	0x3f8198 <Level::_LoadProcess()+0x1808> @ imm = #0x19c
  3f7ff8: e594110c     	ldr	r1, [r4, #0x10c]
  3f7ffc: e0613000     	rsb	r3, r1, r0
  3f8000: e3730001     	cmn	r3, #1
  3f8004: 0a000063     	beq	0x3f8198 <Level::_LoadProcess()+0x1808> @ imm = #0x18c
  3f8008: e28dab01     	add	r10, sp, #1024
  3f800c: e28aa00c     	add	r10, r10, #12
  3f8010: e0612002     	rsb	r2, r1, r2
  3f8014: e1530002     	cmp	r3, r2
  3f8018: 90812003     	addls	r2, r1, r3
  3f801c: 80812002     	addhi	r2, r1, r2
  3f8020: e28480f8     	add	r8, r4, #248
  3f8024: e1a0000a     	mov	r0, r10
  3f8028: e58da41c     	str	r10, [sp, #0x41c]
  3f802c: e58da420     	str	r10, [sp, #0x420]
  3f8030: ebfc65ac     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0xe6950
  3f8034: e158000a     	cmp	r8, r10
  3f8038: 0a000003     	beq	0x3f804c <Level::_LoadProcess()+0x16bc> @ imm = #0xc
  3f803c: e1a00008     	mov	r0, r8
  3f8040: e59d1420     	ldr	r1, [sp, #0x420]
  3f8044: e59d241c     	ldr	r2, [sp, #0x41c]
  3f8048: ebfc6264     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0xe7670
  3f804c: e1a0000a     	mov	r0, r10
  3f8050: ebfc6e55     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe46ac
  3f8054: e51f1648     	ldr	r1, [pc, #-0x648]       @ 0x3f7a14 <Level::_LoadProcess()+0x1084>
  3f8058: e1a00008     	mov	r0, r8
  3f805c: e08f1001     	add	r1, pc, r1
  3f8060: ebffe6c6     	bl	0x3f1b80 <std::string::append(char const*)> @ imm = #-0x64e8
  3f8064: e1a00007     	mov	r0, r7
  3f8068: ebfc7a55     	bl	0x3169c4 <StreamBuffer::~StreamBuffer()> @ imm = #-0xe16ac
  3f806c: eafffc70     	b	0x3f7234 <Level::_LoadProcess()+0x8a4> @ imm = #-0xe40
  3f8070: e51f7660     	ldr	r7, [pc, #-0x660]       @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f8074: e7953007     	ldr	r3, [r5, r7]
  3f8078: e59310cc     	ldr	r1, [r3, #0xcc]
  3f807c: e59320d0     	ldr	r2, [r3, #0xd0]
  3f8080: e1510002     	cmp	r1, r2
  3f8084: 0a000029     	beq	0x3f8130 <Level::_LoadProcess()+0x17a0> @ imm = #0xa4
  3f8088: e59310e8     	ldr	r1, [r3, #0xe8]
  3f808c: e28d2f45     	add	r2, sp, #276
  3f8090: e28d0feb     	add	r0, sp, #940
  3f8094: e58d001c     	str	r0, [sp, #0x1c]
  3f8098: ebfc7013     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3fb4
  3f809c: e51f1688     	ldr	r1, [pc, #-0x688]       @ 0x3f7a1c <Level::_LoadProcess()+0x108c>
  3f80a0: e51fb688     	ldr	r11, [pc, #-0x688]      @ 0x3f7a20 <Level::_LoadProcess()+0x1090>
  3f80a4: e59d001c     	ldr	r0, [sp, #0x1c]
  3f80a8: e08f1001     	add	r1, pc, r1
  3f80ac: ebffe6b3     	bl	0x3f1b80 <std::string::append(char const*)> @ imm = #-0x6534
  3f80b0: e28d2f43     	add	r2, sp, #268
  3f80b4: e28dafe5     	add	r10, sp, #916
  3f80b8: e28d9e11     	add	r9, sp, #272
  3f80bc: e28d8fdf     	add	r8, sp, #892
  3f80c0: e08fb00b     	add	r11, pc, r11
  3f80c4: e58d2018     	str	r2, [sp, #0x18]
  3f80c8: e59d13c0     	ldr	r1, [sp, #0x3c0]
  3f80cc: e1a02009     	mov	r2, r9
  3f80d0: e1a0000a     	mov	r0, r10
  3f80d4: ebfc7004     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3ff0
  3f80d8: e1a0100b     	mov	r1, r11
  3f80dc: e59d2018     	ldr	r2, [sp, #0x18]
  3f80e0: e1a00008     	mov	r0, r8
  3f80e4: ebfc7000     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe4000
  3f80e8: e1a0100a     	mov	r1, r10
  3f80ec: e1a02008     	mov	r2, r8
  3f80f0: e1a00004     	mov	r0, r4
  3f80f4: ebffee91     	bl	0x3f3b40 <Level::LoadFile(std::string const&, std::string const&)> @ imm = #-0x45bc
  3f80f8: e1a03000     	mov	r3, r0
  3f80fc: e1a00008     	mov	r0, r8
  3f8100: e58d3010     	str	r3, [sp, #0x10]
  3f8104: ebfc6e28     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe4760
  3f8108: e1a0000a     	mov	r0, r10
  3f810c: ebfc6e26     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe4768
  3f8110: e59d3010     	ldr	r3, [sp, #0x10]
  3f8114: e3530000     	cmp	r3, #0
  3f8118: 0affffea     	beq	0x3f80c8 <Level::_LoadProcess()+0x1738> @ imm = #-0x58
  3f811c: e59d001c     	ldr	r0, [sp, #0x1c]
  3f8120: ebfc6e21     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe477c
  3f8124: e5943038     	ldr	r3, [r4, #0x38]
  3f8128: e3530000     	cmp	r3, #0
  3f812c: 1afffc20     	bne	0x3f71b4 <Level::_LoadProcess()+0x824> @ imm = #-0xf80
  3f8130: e7951007     	ldr	r1, [r5, r7]
  3f8134: e51f2718     	ldr	r2, [pc, #-0x718]       @ 0x3f7a24 <Level::_LoadProcess()+0x1094>
  3f8138: e51f3718     	ldr	r3, [pc, #-0x718]       @ 0x3f7a28 <Level::_LoadProcess()+0x1098>
  3f813c: e28d7098     	add	r7, sp, #152
  3f8140: e2477004     	sub	r7, r7, #4
  3f8144: e5911038     	ldr	r1, [r1, #0x38]
  3f8148: e3a0c001     	mov	r12, #1
  3f814c: e08f2002     	add	r2, pc, r2
  3f8150: e08f3003     	add	r3, pc, r3
  3f8154: e3a08000     	mov	r8, #0
  3f8158: e1a00007     	mov	r0, r7
  3f815c: e88d1100     	stm	sp, {r8, r12}
  3f8160: ebfd4d6f     	bl	0x34b724 <ObjectManager::Spawn(char const*, char const*, bool, bool)> @ imm = #-0xaca44
  3f8164: e1a01008     	mov	r1, r8
  3f8168: e1a00007     	mov	r0, r7
  3f816c: ebfd1f13     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xb83b4
  3f8170: e2501000     	subs	r1, r0, #0
  3f8174: 0a000002     	beq	0x3f8184 <Level::_LoadProcess()+0x17f4> @ imm = #0x8
  3f8178: e59130f4     	ldr	r3, [r1, #0xf4]
  3f817c: e3530004     	cmp	r3, #4
  3f8180: 0a000000     	beq	0x3f8188 <Level::_LoadProcess()+0x17f8> @ imm = #0x0
  3f8184: e3a01000     	mov	r1, #0
  3f8188: e1a00004     	mov	r0, r4
  3f818c: ebffe4de     	bl	0x3f150c <Level::SetLevelConfig(LevelConfig*)> @ imm = #-0x6c88
  3f8190: e5943038     	ldr	r3, [r4, #0x38]
  3f8194: eafffc06     	b	0x3f71b4 <Level::_LoadProcess()+0x824> @ imm = #-0xfe8
  3f8198: e28480f8     	add	r8, r4, #248
  3f819c: eaffffb0     	b	0x3f8064 <Level::_LoadProcess()+0x16d4> @ imm = #-0x140
  3f81a0: e1a00004     	mov	r0, r4
  3f81a4: e1a01007     	mov	r1, r7
  3f81a8: ebffe03f     	bl	0x3f02ac <Level::AssignSteamToLoadDataFile(StreamBuffer&)> @ imm = #-0x7f04
  3f81ac: e28480f8     	add	r8, r4, #248
  3f81b0: eaffffab     	b	0x3f8064 <Level::_LoadProcess()+0x16d4> @ imm = #-0x154
  3f81b4: e5980040     	ldr	r0, [r8, #0x40]
  3f81b8: ebfddbad     	bl	0x36f074 <PlayerManager::IsLocalPlayerHosting()> @ imm = #-0x8914c
  3f81bc: e3500000     	cmp	r0, #0
  3f81c0: 0afffb35     	beq	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x132c
  3f81c4: e5980040     	ldr	r0, [r8, #0x40]
  3f81c8: ebfdd9f1     	bl	0x36e994 <PlayerManager::AllLoadingDone()> @ imm = #-0x8983c
  3f81cc: e3500000     	cmp	r0, #0
  3f81d0: 0afffb31     	beq	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x133c
  3f81d4: e5980040     	ldr	r0, [r8, #0x40]
  3f81d8: ebfdd9b7     	bl	0x36e8bc <PlayerManager::AllClientsReadyToRoll()> @ imm = #-0x89924
  3f81dc: e3500000     	cmp	r0, #0
  3f81e0: 1afffb2d     	bne	0x3f6e9c <Level::_LoadProcess()+0x50c> @ imm = #-0x134c
  3f81e4: eafffcf6     	b	0x3f75c4 <Level::_LoadProcess()+0xc34> @ imm = #-0xc28
  3f81e8: e3530000     	cmp	r3, #0
  3f81ec: 12838e16     	addne	r8, r3, #352
  3f81f0: 051f37cc     	ldreq	r3, [pc, #-0x7cc]       @ 0x3f7a2c <Level::_LoadProcess()+0x109c>
  3f81f4: 07958003     	ldreq	r8, [r5, r3]
  3f81f8: eafffb17     	b	0x3f6e5c <Level::_LoadProcess()+0x4cc> @ imm = #-0x13a4
  3f81fc: e59a0040     	ldr	r0, [r10, #0x40]
  3f8200: e3a01000     	mov	r1, #0
  3f8204: e3a02001     	mov	r2, #1
  3f8208: ebfdd89a     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x89d98
  3f820c: e5908660     	ldr	r8, [r0, #0x660]
  3f8210: e3580000     	cmp	r8, #0
  3f8214: 0afffd16     	beq	0x3f7674 <Level::_LoadProcess()+0xce4> @ imm = #-0xba8
  3f8218: e5983160     	ldr	r3, [r8, #0x160]
  3f821c: e51f27f4     	ldr	r2, [pc, #-0x7f4]       @ 0x3f7a30 <Level::_LoadProcess()+0x10a0>
  3f8220: e3a0c000     	mov	r12, #0
  3f8224: e58d3088     	str	r3, [sp, #0x88]
  3f8228: e5983164     	ldr	r3, [r8, #0x164]
  3f822c: e7950002     	ldr	r0, [r5, r2]
  3f8230: e28da088     	add	r10, sp, #136
  3f8234: e58d308c     	str	r3, [sp, #0x8c]
  3f8238: e598e168     	ldr	lr, [r8, #0x168]
  3f823c: e28d20a8     	add	r2, sp, #168
  3f8240: e2422008     	sub	r2, r2, #8
  3f8244: e58de090     	str	lr, [sp, #0x90]
  3f8248: e1a0300c     	mov	r3, r12
  3f824c: e3a0e000     	mov	lr, #0
  3f8250: e1a0100a     	mov	r1, r10
  3f8254: e58de0a0     	str	lr, [sp, #0xa0]
  3f8258: e58dc000     	str	r12, [sp]
  3f825c: e58dc004     	str	r12, [sp, #0x4]
  3f8260: e58dc008     	str	r12, [sp, #0x8]
  3f8264: eb04b4a7     	bl	0x525508 <PFWorld::GetFloorHeightAt(Point3D<float> const&, float*, Point3D<float>*, PFRoom**, PFFloor**, bool)> @ imm = #0x12d29c
  3f8268: e3500000     	cmp	r0, #0
  3f826c: 0a000005     	beq	0x3f8288 <Level::_LoadProcess()+0x18f8> @ imm = #0x14
  3f8270: e59d30a0     	ldr	r3, [sp, #0xa0]
  3f8274: e1a0100a     	mov	r1, r10
  3f8278: e1a00008     	mov	r0, r8
  3f827c: e3a02001     	mov	r2, #1
  3f8280: e58d3090     	str	r3, [sp, #0x90]
  3f8284: ebfe6eca     	bl	0x393db4 <GameObject::SetPosition(Point3D<float> const&, bool)> @ imm = #-0x644d8
  3f8288: e2889e16     	add	r9, r8, #352
  3f828c: e795a007     	ldr	r10, [r5, r7]
  3f8290: e3a08000     	mov	r8, #0
  3f8294: ea000008     	b	0x3f82bc <Level::_LoadProcess()+0x192c> @ imm = #0x20
  3f8298: e1a01008     	mov	r1, r8
  3f829c: e3a02001     	mov	r2, #1
  3f82a0: e59a0040     	ldr	r0, [r10, #0x40]
  3f82a4: ebfdd873     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x89e34
  3f82a8: e1a01009     	mov	r1, r9
  3f82ac: e5900660     	ldr	r0, [r0, #0x660]
  3f82b0: e3a02001     	mov	r2, #1
  3f82b4: ebfe6ebe     	bl	0x393db4 <GameObject::SetPosition(Point3D<float> const&, bool)> @ imm = #-0x64508
  3f82b8: e2888001     	add	r8, r8, #1
  3f82bc: e59a0040     	ldr	r0, [r10, #0x40]
  3f82c0: e3a01001     	mov	r1, #1
  3f82c4: ebfdda01     	bl	0x36ead0 <PlayerManager::GetNumLocalPlayers(bool)> @ imm = #-0x897fc
  3f82c8: e1580000     	cmp	r8, r0
  3f82cc: bafffff1     	blt	0x3f8298 <Level::_LoadProcess()+0x1908> @ imm = #-0x3c
  3f82d0: eafffce7     	b	0x3f7674 <Level::_LoadProcess()+0xce4> @ imm = #-0xc64
  3f82d4: ebfc580d     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xe9fcc
