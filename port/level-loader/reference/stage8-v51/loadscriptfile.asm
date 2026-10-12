
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f38a0 <Level::_LoadScriptFile(std::string&)>:
  3f38a0: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  3f38a4: e59f4194     	ldr	r4, [pc, #0x194]        @ 0x3f3a40 <Level::_LoadScriptFile(std::string&)+0x1a0>
  3f38a8: e59f6194     	ldr	r6, [pc, #0x194]        @ 0x3f3a44 <Level::_LoadScriptFile(std::string&)+0x1a4>
  3f38ac: e5918010     	ldr	r8, [r1, #0x10]
  3f38b0: e08f4004     	add	r4, pc, r4
  3f38b4: e7943006     	ldr	r3, [r4, r6]
  3f38b8: e5917014     	ldr	r7, [r1, #0x14]
  3f38bc: e24dd05c     	sub	sp, sp, #92
  3f38c0: e5933000     	ldr	r3, [r3]
  3f38c4: e1580007     	cmp	r8, r7
  3f38c8: e1a05001     	mov	r5, r1
  3f38cc: e58d3054     	str	r3, [sp, #0x54]
  3f38d0: 0a000052     	beq	0x3f3a20 <Level::_LoadScriptFile(std::string&)+0x180> @ imm = #0x148
  3f38d4: e0678008     	rsb	r8, r7, r8
  3f38d8: e3580008     	cmp	r8, #8
  3f38dc: 9a00004f     	bls	0x3f3a20 <Level::_LoadScriptFile(std::string&)+0x180> @ imm = #0x13c
  3f38e0: e59fc160     	ldr	r12, [pc, #0x160]       @ 0x3f3a48 <Level::_LoadScriptFile(std::string&)+0x1a8>
  3f38e4: e0878008     	add	r8, r7, r8
  3f38e8: e28d001c     	add	r0, sp, #28
  3f38ec: e08fc00c     	add	r12, pc, r12
  3f38f0: e28ce009     	add	lr, r12, #9
  3f38f4: e58dc00c     	str	r12, [sp, #0xc]
  3f38f8: e28dc00c     	add	r12, sp, #12
  3f38fc: e58dc000     	str	r12, [sp]
  3f3900: e28d1018     	add	r1, sp, #24
  3f3904: e28dc020     	add	r12, sp, #32
  3f3908: e28d2014     	add	r2, sp, #20
  3f390c: e28d3010     	add	r3, sp, #16
  3f3910: e58de010     	str	lr, [sp, #0x10]
  3f3914: e58dc004     	str	r12, [sp, #0x4]
  3f3918: e58d8018     	str	r8, [sp, #0x18]
  3f391c: e58d7014     	str	r7, [sp, #0x14]
  3f3920: ebffee57     	bl	0x3ef284 <std::reverse_iterator<char const*> std::search<std::reverse_iterator<char const*>, std::reverse_iterator<char const*>, std::priv::_Eq_traits<std::char_traits<char>>>(std::reverse_iterator<char const*>, std::reverse_iterator<char const*>, std::reverse_iterator<char const*>, std::reverse_iterator<char const*>, std::priv::_Eq_traits<std::char_traits<char>>)> @ imm = #-0x46a4
  3f3924: e59da01c     	ldr	r10, [sp, #0x1c]
  3f3928: e157000a     	cmp	r7, r10
  3f392c: 01a0a008     	moveq	r10, r8
  3f3930: 124aa009     	subne	r10, r10, #9
  3f3934: e158000a     	cmp	r8, r10
  3f3938: 0a000038     	beq	0x3f3a20 <Level::_LoadScriptFile(std::string&)+0x180> @ imm = #0xe0
  3f393c: e5953014     	ldr	r3, [r5, #0x14]
  3f3940: e05aa003     	subs	r10, r10, r3
  3f3944: 4a000035     	bmi	0x3f3a20 <Level::_LoadScriptFile(std::string&)+0x180> @ imm = #0xd4
  3f3948: e5951010     	ldr	r1, [r5, #0x10]
  3f394c: e1510003     	cmp	r1, r3
  3f3950: 0a000006     	beq	0x3f3970 <Level::_LoadScriptFile(std::string&)+0xd0> @ imm = #0x18
  3f3954: e3a0002f     	mov	r0, #47
  3f3958: e1d320d0     	ldrsb	r2, [r3]
  3f395c: e352005c     	cmp	r2, #92
  3f3960: 05c30000     	strbeq	r0, [r3]
  3f3964: e2833001     	add	r3, r3, #1
  3f3968: e1530001     	cmp	r3, r1
  3f396c: 1afffff9     	bne	0x3f3958 <Level::_LoadScriptFile(std::string&)+0xb8> @ imm = #-0x1c
  3f3970: e28d803c     	add	r8, sp, #60
  3f3974: e1a01005     	mov	r1, r5
  3f3978: e1a00008     	mov	r0, r8
  3f397c: ebfcdfe5     	bl	0x32b918 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&)> @ imm = #-0xc806c
  3f3980: e59d3050     	ldr	r3, [sp, #0x50]
  3f3984: e59d204c     	ldr	r2, [sp, #0x4c]
  3f3988: e3a0c00e     	mov	r12, #14
  3f398c: e28d7024     	add	r7, sp, #36
  3f3990: e0632002     	rsb	r2, r3, r2
  3f3994: e59f30b0     	ldr	r3, [pc, #0xb0]         @ 0x3f3a4c <Level::_LoadScriptFile(std::string&)+0x1ac>
  3f3998: e06a2002     	rsb	r2, r10, r2
  3f399c: e1a0100a     	mov	r1, r10
  3f39a0: e08f3003     	add	r3, pc, r3
  3f39a4: e1a00008     	mov	r0, r8
  3f39a8: e58dc000     	str	r12, [sp]
  3f39ac: ebfff991     	bl	0x3f1ff8 <std::string::replace(unsigned int, unsigned int, char const*, unsigned int)> @ imm = #-0x19bc
  3f39b0: e1a01005     	mov	r1, r5
  3f39b4: e1a00007     	mov	r0, r7
  3f39b8: ebfcdfd6     	bl	0x32b918 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&)> @ imm = #-0xc80a8
  3f39bc: e59d3050     	ldr	r3, [sp, #0x50]
  3f39c0: e59d204c     	ldr	r2, [sp, #0x4c]
  3f39c4: e3a0c012     	mov	r12, #18
  3f39c8: e1a0100a     	mov	r1, r10
  3f39cc: e0632002     	rsb	r2, r3, r2
  3f39d0: e59f3078     	ldr	r3, [pc, #0x78]         @ 0x3f3a50 <Level::_LoadScriptFile(std::string&)+0x1b0>
  3f39d4: e06a2002     	rsb	r2, r10, r2
  3f39d8: e1a00007     	mov	r0, r7
  3f39dc: e08f3003     	add	r3, pc, r3
  3f39e0: e58dc000     	str	r12, [sp]
  3f39e4: ebfff983     	bl	0x3f1ff8 <std::string::replace(unsigned int, unsigned int, char const*, unsigned int)> @ imm = #-0x19f4
  3f39e8: e59f3064     	ldr	r3, [pc, #0x64]         @ 0x3f3a54 <Level::_LoadScriptFile(std::string&)+0x1b4>
  3f39ec: e59d1050     	ldr	r1, [sp, #0x50]
  3f39f0: e3a02000     	mov	r2, #0
  3f39f4: e7945003     	ldr	r5, [r4, r3]
  3f39f8: e1a00005     	mov	r0, r5
  3f39fc: eb019e18     	bl	0x45b264 <ScriptManager::LoadScriptFile(char const*, bool)> @ imm = #0x67860
  3f3a00: e1a00005     	mov	r0, r5
  3f3a04: e59d1038     	ldr	r1, [sp, #0x38]
  3f3a08: e3a02000     	mov	r2, #0
  3f3a0c: eb019ccb     	bl	0x45ad40 <ScriptManager::LoadScriptFileNames(char const*, bool)> @ imm = #0x6732c
  3f3a10: e1a00007     	mov	r0, r7
  3f3a14: ebfc7fe4     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe0070
  3f3a18: e1a00008     	mov	r0, r8
  3f3a1c: ebfc7fe2     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe0078
  3f3a20: e7943006     	ldr	r3, [r4, r6]
  3f3a24: e59d2054     	ldr	r2, [sp, #0x54]
  3f3a28: e5933000     	ldr	r3, [r3]
  3f3a2c: e1520003     	cmp	r2, r3
  3f3a30: 1a000001     	bne	0x3f3a3c <Level::_LoadScriptFile(std::string&)+0x19c> @ imm = #0x4
  3f3a34: e28dd05c     	add	sp, sp, #92
  3f3a38: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  3f3a3c: ebfc6a33     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xe5734
  3f3a40: e0 11 5a 00  	.word	0x005a11e0
  3f3a44: ac 40 00 00  	.word	0x000040ac
  3f3a48: 3c 2e 4d 00  	.word	0x004d2e3c
  3f3a4c: 98 2d 4d 00  	.word	0x004d2d98
  3f3a50: 6c 2d 4d 00  	.word	0x004d2d6c
  3f3a54: 20 1a 00 00  	.word	0x00001a20
