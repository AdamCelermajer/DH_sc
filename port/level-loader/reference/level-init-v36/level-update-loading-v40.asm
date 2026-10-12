
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:
  3f82d0: eafffce7     	b	0x3f7674 <Level::_LoadProcess()+0xce4> @ imm = #-0xc64
  3f82d4: ebfc580d     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xe9fcc

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
