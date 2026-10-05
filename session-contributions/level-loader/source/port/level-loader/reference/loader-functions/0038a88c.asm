
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0038a88c <Module::LoadModule() const>:
  38a88c: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  38a890: e59f91fc     	ldr	r9, [pc, #0x1fc]        @ 0x38aa94 <Module::LoadModule() const+0x208>
  38a894: e59f31fc     	ldr	r3, [pc, #0x1fc]        @ 0x38aa98 <Module::LoadModule() const+0x20c>
  38a898: e59fb1fc     	ldr	r11, [pc, #0x1fc]       @ 0x38aa9c <Module::LoadModule() const+0x210>
  38a89c: e08f9009     	add	r9, pc, r9
  38a8a0: e7992003     	ldr	r2, [r9, r3]
  38a8a4: e799300b     	ldr	r3, [r9, r11]
  38a8a8: e24dd084     	sub	sp, sp, #132
  38a8ac: e5924000     	ldr	r4, [r2]
  38a8b0: e5933000     	ldr	r3, [r3]
  38a8b4: e1a05000     	mov	r5, r0
  38a8b8: e3540000     	cmp	r4, #0
  38a8bc: e58d307c     	str	r3, [sp, #0x7c]
  38a8c0: 0a00005d     	beq	0x38aa3c <Module::LoadModule() const+0x1b0> @ imm = #0x174
  38a8c4: e1a00004     	mov	r0, r4
  38a8c8: e595140c     	ldr	r1, [r5, #0x40c]
  38a8cc: eb019269     	bl	0x3ef278 <Level::SetObjectModuleId(int)> @ imm = #0x649a4
  38a8d0: e5953160     	ldr	r3, [r5, #0x160]
  38a8d4: e28d6064     	add	r6, sp, #100
  38a8d8: e1a00006     	mov	r0, r6
  38a8dc: e5843160     	str	r3, [r4, #0x160]
  38a8e0: e5953164     	ldr	r3, [r5, #0x164]
  38a8e4: e3a01010     	mov	r1, #16
  38a8e8: e28d704c     	add	r7, sp, #76
  38a8ec: e5843164     	str	r3, [r4, #0x164]
  38a8f0: e5953168     	ldr	r3, [r5, #0x168]
  38a8f4: e3a08000     	mov	r8, #0
  38a8f8: e5843168     	str	r3, [r4, #0x168]
  38a8fc: e58d6074     	str	r6, [sp, #0x74]
  38a900: e58d6078     	str	r6, [sp, #0x78]
  38a904: ebfe1b5c     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x79290
  38a908: e59d3074     	ldr	r3, [sp, #0x74]
  38a90c: e1a00007     	mov	r0, r7
  38a910: e3a01010     	mov	r1, #16
  38a914: e5c38000     	strb	r8, [r3]
  38a918: e58d705c     	str	r7, [sp, #0x5c]
  38a91c: e58d7060     	str	r7, [sp, #0x60]
  38a920: ebfe1b55     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x792ac
  38a924: e59d305c     	ldr	r3, [sp, #0x5c]
  38a928: e1a02007     	mov	r2, r7
  38a92c: e1a00005     	mov	r0, r5
  38a930: e5c38000     	strb	r8, [r3]
  38a934: e1a01006     	mov	r1, r6
  38a938: ebfffe93     	bl	0x38a38c <Module::_ChooseXmls(std::string&, std::string&) const> @ imm = #-0x5b4
  38a93c: e59d3074     	ldr	r3, [sp, #0x74]
  38a940: e59d2078     	ldr	r2, [sp, #0x78]
  38a944: e1520003     	cmp	r2, r3
  38a948: 0a000012     	beq	0x38a998 <Module::LoadModule() const+0x10c> @ imm = #0x48
  38a94c: e59f814c     	ldr	r8, [pc, #0x14c]        @ 0x38aaa0 <Module::LoadModule() const+0x214>
  38a950: e28d5034     	add	r5, sp, #52
  38a954: e28da018     	add	r10, sp, #24
  38a958: e08f8008     	add	r8, pc, r8
  38a95c: e1a01008     	mov	r1, r8
  38a960: e1a0200a     	mov	r2, r10
  38a964: e1a00005     	mov	r0, r5
  38a968: ebfe25df     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x76884
  38a96c: e1a01006     	mov	r1, r6
  38a970: e1a02005     	mov	r2, r5
  38a974: e1a00004     	mov	r0, r4
  38a978: eb01a470     	bl	0x3f3b40 <Level::LoadFile(std::string const&, std::string const&)> @ imm = #0x691c0
  38a97c: e1a03000     	mov	r3, r0
  38a980: e1a00005     	mov	r0, r5
  38a984: e58d300c     	str	r3, [sp, #0xc]
  38a988: ebfe2407     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x76fe4
  38a98c: e59d300c     	ldr	r3, [sp, #0xc]
  38a990: e3530000     	cmp	r3, #0
  38a994: 0afffff0     	beq	0x38a95c <Module::LoadModule() const+0xd0> @ imm = #-0x40
  38a998: e59d305c     	ldr	r3, [sp, #0x5c]
  38a99c: e59d2060     	ldr	r2, [sp, #0x60]
  38a9a0: e1520003     	cmp	r2, r3
  38a9a4: 0a000012     	beq	0x38a9f4 <Module::LoadModule() const+0x168> @ imm = #0x48
  38a9a8: e59f80f4     	ldr	r8, [pc, #0xf4]         @ 0x38aaa4 <Module::LoadModule() const+0x218>
  38a9ac: e28d501c     	add	r5, sp, #28
  38a9b0: e28da014     	add	r10, sp, #20
  38a9b4: e08f8008     	add	r8, pc, r8
  38a9b8: e1a01008     	mov	r1, r8
  38a9bc: e1a0200a     	mov	r2, r10
  38a9c0: e1a00005     	mov	r0, r5
  38a9c4: ebfe25c8     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x768e0
  38a9c8: e1a01007     	mov	r1, r7
  38a9cc: e1a02005     	mov	r2, r5
  38a9d0: e1a00004     	mov	r0, r4
  38a9d4: eb01a459     	bl	0x3f3b40 <Level::LoadFile(std::string const&, std::string const&)> @ imm = #0x69164
  38a9d8: e1a03000     	mov	r3, r0
  38a9dc: e1a00005     	mov	r0, r5
  38a9e0: e58d300c     	str	r3, [sp, #0xc]
  38a9e4: ebfe23f0     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x77040
  38a9e8: e59d300c     	ldr	r3, [sp, #0xc]
  38a9ec: e3530000     	cmp	r3, #0
  38a9f0: 0afffff0     	beq	0x38a9b8 <Module::LoadModule() const+0x12c> @ imm = #-0x40
  38a9f4: e3a03000     	mov	r3, #0
  38a9f8: e5843168     	str	r3, [r4, #0x168]
  38a9fc: e5843160     	str	r3, [r4, #0x160]
  38aa00: e5843164     	str	r3, [r4, #0x164]
  38aa04: e3e01000     	mvn	r1, #0
  38aa08: e1a00004     	mov	r0, r4
  38aa0c: eb019219     	bl	0x3ef278 <Level::SetObjectModuleId(int)> @ imm = #0x64864
  38aa10: e1a00007     	mov	r0, r7
  38aa14: ebfe23e4     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x77070
  38aa18: e1a00006     	mov	r0, r6
  38aa1c: ebfe23e2     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x77078
  38aa20: e799300b     	ldr	r3, [r9, r11]
  38aa24: e59d207c     	ldr	r2, [sp, #0x7c]
  38aa28: e5933000     	ldr	r3, [r3]
  38aa2c: e1520003     	cmp	r2, r3
  38aa30: 1a000016     	bne	0x38aa90 <Module::LoadModule() const+0x204> @ imm = #0x58
  38aa34: e28dd084     	add	sp, sp, #132
  38aa38: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  38aa3c: e59f3064     	ldr	r3, [pc, #0x64]         @ 0x38aaa8 <Module::LoadModule() const+0x21c>
  38aa40: e7993003     	ldr	r3, [r9, r3]
  38aa44: e5933000     	ldr	r3, [r3]
  38aa48: e3530002     	cmp	r3, #2
  38aa4c: 05844000     	streq	r4, [r4]
  38aa50: 0affff9b     	beq	0x38a8c4 <Module::LoadModule() const+0x38> @ imm = #-0x194
  38aa54: e3530001     	cmp	r3, #1
  38aa58: 1affff99     	bne	0x38a8c4 <Module::LoadModule() const+0x38> @ imm = #-0x19c
  38aa5c: e59f0048     	ldr	r0, [pc, #0x48]         @ 0x38aaac <Module::LoadModule() const+0x220>
  38aa60: e59f1048     	ldr	r1, [pc, #0x48]         @ 0x38aab0 <Module::LoadModule() const+0x224>
  38aa64: e59f2048     	ldr	r2, [pc, #0x48]         @ 0x38aab4 <Module::LoadModule() const+0x228>
  38aa68: e7990000     	ldr	r0, [r9, r0]
  38aa6c: e59f3044     	ldr	r3, [pc, #0x44]         @ 0x38aab8 <Module::LoadModule() const+0x22c>
  38aa70: e3a0c054     	mov	r12, #84
  38aa74: e08f1001     	add	r1, pc, r1
  38aa78: e08f2002     	add	r2, pc, r2
  38aa7c: e08f3003     	add	r3, pc, r3
  38aa80: e28000a8     	add	r0, r0, #168
  38aa84: e58dc000     	str	r12, [sp]
  38aa88: ebfe0d5d     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x7ca8c
  38aa8c: eaffff8c     	b	0x38a8c4 <Module::LoadModule() const+0x38> @ imm = #-0x1d0
  38aa90: ebfe0e1e     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x7c788
  38aa94: f4 a1 60 00  	.word	0x0060a1f4
  38aa98: 64 1d 00 00  	.word	0x00001d64
  38aa9c: ac 40 00 00  	.word	0x000040ac
  38aaa0: d8 5a 53 00  	.word	0x00535ad8
  38aaa4: 7c 5a 53 00  	.word	0x00535a7c
  38aaa8: c0 39 00 00  	.word	0x000039c0
  38aaac: c0 19 00 00  	.word	0x000019c0
  38aab0: 64 39 53 00  	.word	0x00533964
  38aab4: d8 78 53 00  	.word	0x005378d8
  38aab8: f4 77 53 00  	.word	0x005377f4
