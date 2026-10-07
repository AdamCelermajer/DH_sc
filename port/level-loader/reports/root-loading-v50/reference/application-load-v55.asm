
.local-inputs/libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0032bdc8 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)>:
  32bdc8: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  32bdcc: e59f43d8     	ldr	r4, [pc, #0x3d8]        @ 0x32c1ac <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x3e4>
  32bdd0: e59f63d8     	ldr	r6, [pc, #0x3d8]        @ 0x32c1b0 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x3e8>
  32bdd4: e24ddf7d     	sub	sp, sp, #500
  32bdd8: e08f4004     	add	r4, pc, r4
  32bddc: e7948006     	ldr	r8, [r4, r6]
  32bde0: e59f63cc     	ldr	r6, [pc, #0x3cc]        @ 0x32c1b4 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x3ec>
  32bde4: e59f53cc     	ldr	r5, [pc, #0x3cc]        @ 0x32c1b8 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x3f0>
  32bde8: e58d2028     	str	r2, [sp, #0x28]
  32bdec: e7946006     	ldr	r6, [r4, r6]
  32bdf0: e794c005     	ldr	r12, [r4, r5]
  32bdf4: e5883000     	str	r3, [r8]
  32bdf8: e58d601c     	str	r6, [sp, #0x1c]
  32bdfc: e59f63b8     	ldr	r6, [pc, #0x3b8]        @ 0x32c1bc <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x3f4>
  32be00: e59ce000     	ldr	lr, [r12]
  32be04: e59fc3b4     	ldr	r12, [pc, #0x3b4]       @ 0x32c1c0 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x3f8>
  32be08: e7946006     	ldr	r6, [r4, r6]
  32be0c: e5dd7224     	ldrb	r7, [sp, #0x224]
  32be10: e794c00c     	ldr	r12, [r4, r12]
  32be14: e58d6024     	str	r6, [sp, #0x24]
  32be18: e59f63a4     	ldr	r6, [pc, #0x3a4]        @ 0x32c1c4 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x3fc>
  32be1c: e58c2000     	str	r2, [r12]
  32be20: e59d201c     	ldr	r2, [sp, #0x1c]
  32be24: e7946006     	ldr	r6, [r4, r6]
  32be28: e58de1ec     	str	lr, [sp, #0x1ec]
  32be2c: e1a08001     	mov	r8, r1
  32be30: e58d6034     	str	r6, [sp, #0x34]
  32be34: e59f638c     	ldr	r6, [pc, #0x38c]        @ 0x32c1c8 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x400>
  32be38: e794b006     	ldr	r11, [r4, r6]
  32be3c: e59f6388     	ldr	r6, [pc, #0x388]        @ 0x32c1cc <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x404>
  32be40: e5cb7000     	strb	r7, [r11]
  32be44: e7949006     	ldr	r9, [r4, r6]
  32be48: e59f6380     	ldr	r6, [pc, #0x380]        @ 0x32c1d0 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x408>
  32be4c: e3a0b001     	mov	r11, #1
  32be50: e794a006     	ldr	r10, [r4, r6]
  32be54: e5dd6218     	ldrb	r6, [sp, #0x218]
  32be58: e58d602c     	str	r6, [sp, #0x2c]
  32be5c: e5dd621c     	ldrb	r6, [sp, #0x21c]
  32be60: e59dc02c     	ldr	r12, [sp, #0x2c]
  32be64: e58d6030     	str	r6, [sp, #0x30]
  32be68: e5c2c000     	strb	r12, [r2]
  32be6c: e1a06003     	mov	r6, r3
  32be70: e59dc024     	ldr	r12, [sp, #0x24]
  32be74: e59d3030     	ldr	r3, [sp, #0x30]
  32be78: e59d2220     	ldr	r2, [sp, #0x220]
  32be7c: e5cc3000     	strb	r3, [r12]
  32be80: e59d3034     	ldr	r3, [sp, #0x34]
  32be84: e59dc228     	ldr	r12, [sp, #0x228]
  32be88: e5832000     	str	r2, [r3]
  32be8c: e589c000     	str	r12, [r9]
  32be90: e59d222c     	ldr	r2, [sp, #0x22c]
  32be94: e1a09000     	mov	r9, r0
  32be98: e58a2000     	str	r2, [r10]
  32be9c: ebffff99     	bl	0x32bd08 <GCSingleton<GameCenter>::GetInstance()> @ imm = #-0x19c
  32bea0: e3a01000     	mov	r1, #0
  32bea4: e5c0b014     	strb	r11, [r0, #0x14]
  32bea8: e5c910ab     	strb	r1, [r9, #0xab]
  32beac: e1a00009     	mov	r0, r9
  32beb0: e58d1018     	str	r1, [sp, #0x18]
  32beb4: ebffcdb6     	bl	0x31f594 <Application::GetCurrentLevel() const> @ imm = #-0xc928
  32beb8: e59d1018     	ldr	r1, [sp, #0x18]
  32bebc: e1a0a000     	mov	r10, r0
  32bec0: e1a00009     	mov	r0, r9
  32bec4: ebffcde7     	bl	0x31f668 <Application::ShowStatubBar(bool)> @ imm = #-0xc864
  32bec8: e35a0000     	cmp	r10, #0
  32becc: 0a000038     	beq	0x32bfb4 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x1ec> @ imm = #0xe0
  32bed0: e59a3130     	ldr	r3, [r10, #0x130]
  32bed4: e3530026     	cmp	r3, #38
  32bed8: 0a000006     	beq	0x32bef8 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x130> @ imm = #0x18
  32bedc: e7943005     	ldr	r3, [r4, r5]
  32bee0: e59d21ec     	ldr	r2, [sp, #0x1ec]
  32bee4: e5933000     	ldr	r3, [r3]
  32bee8: e1520003     	cmp	r2, r3
  32beec: 1a0000ad     	bne	0x32c1a8 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x3e0> @ imm = #0x2b4
  32bef0: e28ddf7d     	add	sp, sp, #500
  32bef4: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  32bef8: e5da3198     	ldrb	r3, [r10, #0x198]
  32befc: e3530000     	cmp	r3, #0
  32bf00: 0a00008a     	beq	0x32c130 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x368> @ imm = #0x228
  32bf04: e59fc2c8     	ldr	r12, [pc, #0x2c8]       @ 0x32c1d4 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x40c>
  32bf08: e58dc01c     	str	r12, [sp, #0x1c]
  32bf0c: e3a0b001     	mov	r11, #1
  32bf10: e1a00009     	mov	r0, r9
  32bf14: e5cab145     	strb	r11, [r10, #0x145]
  32bf18: ebffd1a1     	bl	0x3205a4 <Application::SendGLHiScore()> @ imm = #-0xb97c
  32bf1c: e59a2040     	ldr	r2, [r10, #0x40]
  32bf20: e5cab0f0     	strb	r11, [r10, #0xf0]
  32bf24: e1a0000a     	mov	r0, r10
  32bf28: e1a0100b     	mov	r1, r11
  32bf2c: e58d2024     	str	r2, [sp, #0x24]
  32bf30: eb03121e     	bl	0x3f07b0 <Level::SG_SaveLocalPlayer(bool)> @ imm = #0xc4878
  32bf34: e1a0000a     	mov	r0, r10
  32bf38: e1a0100b     	mov	r1, r11
  32bf3c: eb031196     	bl	0x3f059c <Level::QuickSave(bool)> @ imm = #0xc4658
  32bf40: e1a0000a     	mov	r0, r10
  32bf44: eb030cb6     	bl	0x3ef224 <Level::ResetIsLoaded()> @ imm = #0xc32d8
  32bf48: e3570000     	cmp	r7, #0
  32bf4c: 0a000021     	beq	0x32bfd8 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x210> @ imm = #0x84
  32bf50: e1a02006     	mov	r2, r6
  32bf54: e59d622c     	ldr	r6, [sp, #0x22c]
  32bf58: e59dc02c     	ldr	r12, [sp, #0x2c]
  32bf5c: e1a00008     	mov	r0, r8
  32bf60: e58d6000     	str	r6, [sp]
  32bf64: e59d6030     	ldr	r6, [sp, #0x30]
  32bf68: e58dc004     	str	r12, [sp, #0x4]
  32bf6c: e59dc024     	ldr	r12, [sp, #0x24]
  32bf70: e58d6008     	str	r6, [sp, #0x8]
  32bf74: e59d6220     	ldr	r6, [sp, #0x220]
  32bf78: e59d1028     	ldr	r1, [sp, #0x28]
  32bf7c: e59d3228     	ldr	r3, [sp, #0x228]
  32bf80: e58dc00c     	str	r12, [sp, #0xc]
  32bf84: e58d6010     	str	r6, [sp, #0x10]
  32bf88: eb016a22     	bl	0x386818 <GSLevel::LoadLevel(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)> @ imm = #0x5a888
  32bf8c: e59f3244     	ldr	r3, [pc, #0x244]        @ 0x32c1d8 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x410>
  32bf90: e7940003     	ldr	r0, [r4, r3]
  32bf94: eb01344e     	bl	0x3790d4 <PlayerStatManager::Reset()> @ imm = #0x4d138
  32bf98: e59dc01c     	ldr	r12, [sp, #0x1c]
  32bf9c: e794300c     	ldr	r3, [r4, r12]
  32bfa0: e5930040     	ldr	r0, [r3, #0x40]
  32bfa4: e3a03000     	mov	r3, #0
  32bfa8: e5c036c9     	strb	r3, [r0, #0x6c9]
  32bfac: eb013400     	bl	0x378fb4 <PlayerManager::Update()> @ imm = #0x4d000
  32bfb0: eaffffc9     	b	0x32bedc <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x114> @ imm = #-0xdc
  32bfb4: e59f3218     	ldr	r3, [pc, #0x218]        @ 0x32c1d4 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x40c>
  32bfb8: e3e0c000     	mvn	r12, #0
  32bfbc: e3570000     	cmp	r7, #0
  32bfc0: e58d301c     	str	r3, [sp, #0x1c]
  32bfc4: e7943003     	ldr	r3, [r4, r3]
  32bfc8: e58dc024     	str	r12, [sp, #0x24]
  32bfcc: e5933040     	ldr	r3, [r3, #0x40]
  32bfd0: e5c3b71b     	strb	r11, [r3, #0x71b]
  32bfd4: 1affffdd     	bne	0x32bf50 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x188> @ imm = #-0x8c
  32bfd8: e59fa1fc     	ldr	r10, [pc, #0x1fc]       @ 0x32c1dc <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x414>
  32bfdc: e28d203c     	add	r2, sp, #60
  32bfe0: e58d2020     	str	r2, [sp, #0x20]
  32bfe4: e1a03007     	mov	r3, r7
  32bfe8: e1a00002     	mov	r0, r2
  32bfec: e1a01006     	mov	r1, r6
  32bff0: e3a02001     	mov	r2, #1
  32bff4: eb04e56c     	bl	0x4655ac <PlayerSavegame::PlayerSavegame(unsigned int, int, bool)> @ imm = #0x1395b0
  32bff8: e794300a     	ldr	r3, [r4, r10]
  32bffc: e28dce1f     	add	r12, sp, #496
  32c000: e5933000     	ldr	r3, [r3]
  32c004: e08c3103     	add	r3, r12, r3, lsl #2
  32c008: e5133158     	ldr	r3, [r3, #-0x158]
  32c00c: e3530000     	cmp	r3, #0
  32c010: e58d3228     	str	r3, [sp, #0x228]
  32c014: 1a000050     	bne	0x32c15c <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x394> @ imm = #0x140
  32c018: eb0b7c2b     	bl	0x60b0cc <glitch::os::Timer::getRealTime()> @ imm = #0x2df0ac
  32c01c: e58d0228     	str	r0, [sp, #0x228]
  32c020: e794300a     	ldr	r3, [r4, r10]
  32c024: e59dc228     	ldr	r12, [sp, #0x228]
  32c028: e28d2e1f     	add	r2, sp, #496
  32c02c: e5933000     	ldr	r3, [r3]
  32c030: e0823103     	add	r3, r2, r3, lsl #2
  32c034: e503c158     	str	r12, [r3, #-0x158]
  32c038: eb1345d5     	bl	0x7fd794 <GetOnline()>  @ imm = #0x4d1754
  32c03c: e5d03005     	ldrb	r3, [r0, #0x5]
  32c040: e3530000     	cmp	r3, #0
  32c044: 0a000034     	beq	0x32c11c <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x354> @ imm = #0xd0
  32c048: e59f3190     	ldr	r3, [pc, #0x190]        @ 0x32c1e0 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x418>
  32c04c: e7943003     	ldr	r3, [r4, r3]
  32c050: e593b000     	ldr	r11, [r3]
  32c054: e35b0000     	cmp	r11, #0
  32c058: da000026     	ble	0x32c0f8 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x330> @ imm = #0x98
  32c05c: e59f3180     	ldr	r3, [pc, #0x180]        @ 0x32c1e4 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x41c>
  32c060: e3a07000     	mov	r7, #0
  32c064: e3e09000     	mvn	r9, #0
  32c068: e7943003     	ldr	r3, [r4, r3]
  32c06c: e593a000     	ldr	r10, [r3]
  32c070: e59a0020     	ldr	r0, [r10, #0x20]
  32c074: e1a01008     	mov	r1, r8
  32c078: ebff899a     	bl	0x30e6e8 <.plt+0x974>   @ imm = #-0x1d998
  32c07c: e3500000     	cmp	r0, #0
  32c080: 01a09007     	moveq	r9, r7
  32c084: e2877001     	add	r7, r7, #1
  32c088: e157000b     	cmp	r7, r11
  32c08c: e28aa048     	add	r10, r10, #72
  32c090: 1afffff6     	bne	0x32c070 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x2a8> @ imm = #-0x28
  32c094: e3790001     	cmn	r9, #1
  32c098: 0a000016     	beq	0x32c0f8 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x330> @ imm = #0x58
  32c09c: eb137c46     	bl	0x80b1bc <CMessaging::Get()> @ imm = #0x4df118
  32c0a0: e1a07000     	mov	r7, r0
  32c0a4: e59f013c     	ldr	r0, [pc, #0x13c]        @ 0x32c1e8 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x420>
  32c0a8: e3a01001     	mov	r1, #1
  32c0ac: e08f0000     	add	r0, pc, r0
  32c0b0: eb137863     	bl	0x80a244 <CMessage::CreateMessage(char const*, bool)> @ imm = #0x4de18c
  32c0b4: e3e03000     	mvn	r3, #0
  32c0b8: e5809050     	str	r9, [r0, #0x50]
  32c0bc: e5803068     	str	r3, [r0, #0x68]
  32c0c0: e59d2028     	ldr	r2, [sp, #0x28]
  32c0c4: e1a01000     	mov	r1, r0
  32c0c8: e5802054     	str	r2, [r0, #0x54]
  32c0cc: e59d302c     	ldr	r3, [sp, #0x2c]
  32c0d0: e5c03058     	strb	r3, [r0, #0x58]
  32c0d4: e59dc030     	ldr	r12, [sp, #0x30]
  32c0d8: e5c0c059     	strb	r12, [r0, #0x59]
  32c0dc: e59d2220     	ldr	r2, [sp, #0x220]
  32c0e0: e580205c     	str	r2, [r0, #0x5c]
  32c0e4: e59d3228     	ldr	r3, [sp, #0x228]
  32c0e8: e5803060     	str	r3, [r0, #0x60]
  32c0ec: e5803064     	str	r3, [r0, #0x64]
  32c0f0: e1a00007     	mov	r0, r7
  32c0f4: eb13886a     	bl	0x80e2a4 <CMessaging::SendMsg(CMessage*)> @ imm = #0x4e21a8
  32c0f8: eb1353a3     	bl	0x800f8c <CMatching::Get()> @ imm = #0x4d4e8c
  32c0fc: e5903000     	ldr	r3, [r0]
  32c100: e1a0e00f     	mov	lr, pc
  32c104: e593f0a4     	ldr	pc, [r3, #0xa4]
  32c108: e3a01007     	mov	r1, #7
  32c10c: e5903000     	ldr	r3, [r0]
  32c110: e3a02001     	mov	r2, #1
  32c114: e1a0e00f     	mov	lr, pc
  32c118: e593f008     	ldr	pc, [r3, #0x8]
  32c11c: e59d0020     	ldr	r0, [sp, #0x20]
  32c120: eb04dd99     	bl	0x46378c <PlayerSavegame::~PlayerSavegame()> @ imm = #0x137664
  32c124: e59dc228     	ldr	r12, [sp, #0x228]
  32c128: e58dc22c     	str	r12, [sp, #0x22c]
  32c12c: eaffff87     	b	0x32bf50 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x188> @ imm = #-0x1e4
  32c130: e59f209c     	ldr	r2, [pc, #0x9c]         @ 0x32c1d4 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x40c>
  32c134: e1a0100b     	mov	r1, r11
  32c138: e7943002     	ldr	r3, [r4, r2]
  32c13c: e58d201c     	str	r2, [sp, #0x1c]
  32c140: e5933054     	ldr	r3, [r3, #0x54]
  32c144: e59330f4     	ldr	r3, [r3, #0xf4]
  32c148: e1a00003     	mov	r0, r3
  32c14c: e5933000     	ldr	r3, [r3]
  32c150: e1a0e00f     	mov	lr, pc
  32c154: e593f038     	ldr	pc, [r3, #0x38]
  32c158: eaffff6b     	b	0x32bf0c <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x144> @ imm = #-0x254
  32c15c: e59f3088     	ldr	r3, [pc, #0x88]         @ 0x32c1ec <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x424>
  32c160: e28d9f75     	add	r9, sp, #468
  32c164: e7947003     	ldr	r7, [r4, r3]
  32c168: e1a00007     	mov	r0, r7
  32c16c: eb002dc5     	bl	0x337888 <DebugSwitches::load()> @ imm = #0xb714
  32c170: e59f1078     	ldr	r1, [pc, #0x78]         @ 0x32c1f0 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x428>
  32c174: e28d2038     	add	r2, sp, #56
  32c178: e1a00009     	mov	r0, r9
  32c17c: e08f1001     	add	r1, pc, r1
  32c180: ebff9fd9     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x1809c
  32c184: e1a00007     	mov	r0, r7
  32c188: e1a01009     	mov	r1, r9
  32c18c: eb002e3d     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #0xb8f4
  32c190: e1a07000     	mov	r7, r0
  32c194: e1a00009     	mov	r0, r9
  32c198: ebff9e03     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x187f4
  32c19c: e3570000     	cmp	r7, #0
  32c1a0: 0affff9e     	beq	0x32c020 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x258> @ imm = #-0x188
  32c1a4: eaffff9b     	b	0x32c018 <Application::LoadLevel(char const*, int, unsigned int, bool, bool, int, bool, unsigned int, unsigned int)+0x250> @ imm = #-0x194
  32c1a8: ebff8858     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x1dea0
  32c1ac: b8 8c 66 00  	.word	0x00668cb8
  32c1b0: e0 46 00 00  	.word	0x000046e0
  32c1b4: 58 4c 00 00  	.word	0x00004c58
  32c1b8: ac 40 00 00  	.word	0x000040ac
  32c1bc: 18 26 00 00  	.word	0x00002618
  32c1c0: f8 2f 00 00  	.word	0x00002ff8
  32c1c4: 2c 3d 00 00  	.word	0x00003d2c
  32c1c8: 10 1d 00 00  	.word	0x00001d10
  32c1cc: 10 3f 00 00  	.word	0x00003f10
  32c1d0: 74 42 00 00  	.word	0x00004274
  32c1d4: f4 37 00 00  	.word	0x000037f4
  32c1d8: 14 27 00 00  	.word	0x00002714
  32c1dc: 9c 1a 00 00  	.word	0x00001a9c
  32c1e0: c0 18 00 00  	.word	0x000018c0
  32c1e4: 74 08 00 00  	.word	0x00000874
  32c1e8: 74 2e 59 00  	.word	0x00592e74
  32c1ec: 84 08 00 00  	.word	0x00000884
