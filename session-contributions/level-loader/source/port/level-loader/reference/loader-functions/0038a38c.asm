
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0038a38c <Module::_ChooseXmls(std::string&, std::string&) const>:
  38a38c: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  38a390: e59fb4d8     	ldr	r11, [pc, #0x4d8]       @ 0x38a870 <Module::_ChooseXmls(std::string&, std::string&) const+0x4e4>
  38a394: e59f34d8     	ldr	r3, [pc, #0x4d8]        @ 0x38a874 <Module::_ChooseXmls(std::string&, std::string&) const+0x4e8>
  38a398: e24ddf8d     	sub	sp, sp, #564
  38a39c: e08fb00b     	add	r11, pc, r11
  38a3a0: e58d3014     	str	r3, [sp, #0x14]
  38a3a4: e79b3003     	ldr	r3, [r11, r3]
  38a3a8: e2806fde     	add	r6, r0, #888
  38a3ac: e1560001     	cmp	r6, r1
  38a3b0: e5933000     	ldr	r3, [r3]
  38a3b4: e1a0a000     	mov	r10, r0
  38a3b8: e58d100c     	str	r1, [sp, #0xc]
  38a3bc: e58d2010     	str	r2, [sp, #0x10]
  38a3c0: e58d322c     	str	r3, [sp, #0x22c]
  38a3c4: 0a000003     	beq	0x38a3d8 <Module::_ChooseXmls(std::string&, std::string&) const+0x4c> @ imm = #0xc
  38a3c8: e1a00001     	mov	r0, r1
  38a3cc: e59a2388     	ldr	r2, [r10, #0x388]
  38a3d0: e59a138c     	ldr	r1, [r10, #0x38c]
  38a3d4: ebfe1981     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x799fc
  38a3d8: e59d0010     	ldr	r0, [sp, #0x10]
  38a3dc: e28a9e39     	add	r9, r10, #912
  38a3e0: e1590000     	cmp	r9, r0
  38a3e4: 0a000002     	beq	0x38a3f4 <Module::_ChooseXmls(std::string&, std::string&) const+0x68> @ imm = #0x8
  38a3e8: e59a13a4     	ldr	r1, [r10, #0x3a4]
  38a3ec: e59a23a0     	ldr	r2, [r10, #0x3a0]
  38a3f0: ebfe197a     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x79a18
  38a3f4: e59a23bc     	ldr	r2, [r10, #0x3bc]
  38a3f8: e59a33b8     	ldr	r3, [r10, #0x3b8]
  38a3fc: e1520003     	cmp	r2, r3
  38a400: 0a00009d     	beq	0x38a67c <Module::_ChooseXmls(std::string&, std::string&) const+0x2f0> @ imm = #0x274
  38a404: e59a23d4     	ldr	r2, [r10, #0x3d4]
  38a408: e59a33d0     	ldr	r3, [r10, #0x3d0]
  38a40c: e1520003     	cmp	r2, r3
  38a410: 0a000099     	beq	0x38a67c <Module::_ChooseXmls(std::string&, std::string&) const+0x2f0> @ imm = #0x264
  38a414: e28d4f85     	add	r4, sp, #532
  38a418: e3a07000     	mov	r7, #0
  38a41c: e1a00004     	mov	r0, r4
  38a420: e3a01010     	mov	r1, #16
  38a424: e58d7038     	str	r7, [sp, #0x38]
  38a428: e58d703c     	str	r7, [sp, #0x3c]
  38a42c: e58d7040     	str	r7, [sp, #0x40]
  38a430: e58d702c     	str	r7, [sp, #0x2c]
  38a434: e58d7030     	str	r7, [sp, #0x30]
  38a438: e58d7034     	str	r7, [sp, #0x34]
  38a43c: e58d7020     	str	r7, [sp, #0x20]
  38a440: e58d7024     	str	r7, [sp, #0x24]
  38a444: e58d7028     	str	r7, [sp, #0x28]
  38a448: e58d4224     	str	r4, [sp, #0x224]
  38a44c: e58d4228     	str	r4, [sp, #0x228]
  38a450: ebfe1c89     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x78ddc
  38a454: e59d3224     	ldr	r3, [sp, #0x224]
  38a458: e28d5f5f     	add	r5, sp, #380
  38a45c: e1a00005     	mov	r0, r5
  38a460: e5c37000     	strb	r7, [r3]
  38a464: e28a1fea     	add	r1, r10, #936
  38a468: ebfffd7f     	bl	0x389a6c <std::basic_stringstream<char, std::char_traits<char>, std::allocator<char>>::basic_stringstream(std::string const&, int) (.clone.4)> @ imm = #-0xa04
  38a46c: e28d8038     	add	r8, sp, #56
  38a470: e1a00005     	mov	r0, r5
  38a474: e1a01004     	mov	r1, r4
  38a478: ebffff76     	bl	0x38a258 <std::basic_istream<char, std::char_traits<char>>& std::getline<char, std::char_traits<char>, std::allocator<char>>(std::basic_istream<char, std::char_traits<char>>&, std::basic_string<char, std::char_traits<char>, std::allocator<char>>&, char) (.clone.8)> @ imm = #-0x228
  38a47c: e5903000     	ldr	r3, [r0]
  38a480: e513300c     	ldr	r3, [r3, #-0xc]
  38a484: e0800003     	add	r0, r0, r3
  38a488: e5903008     	ldr	r3, [r0, #0x8]
  38a48c: e3130005     	tst	r3, #5
  38a490: 0a000081     	beq	0x38a69c <Module::_ChooseXmls(std::string&, std::string&) const+0x310> @ imm = #0x204
  38a494: e1a01006     	mov	r1, r6
  38a498: e1a00008     	mov	r0, r8
  38a49c: e28d60e4     	add	r6, sp, #228
  38a4a0: ebfe858c     	bl	0x32bad8 <std::vector<std::string, std::allocator<std::string>>::push_back(std::string const&)> @ imm = #-0x5e9d0
  38a4a4: e1a00006     	mov	r0, r6
  38a4a8: e28a1d0f     	add	r1, r10, #960
  38a4ac: ebfffd6e     	bl	0x389a6c <std::basic_stringstream<char, std::char_traits<char>, std::allocator<char>>::basic_stringstream(std::string const&, int) (.clone.4)> @ imm = #-0xa48
  38a4b0: e28d702c     	add	r7, sp, #44
  38a4b4: e1a00006     	mov	r0, r6
  38a4b8: e1a01004     	mov	r1, r4
  38a4bc: ebffff65     	bl	0x38a258 <std::basic_istream<char, std::char_traits<char>>& std::getline<char, std::char_traits<char>, std::allocator<char>>(std::basic_istream<char, std::char_traits<char>>&, std::basic_string<char, std::char_traits<char>, std::allocator<char>>&, char) (.clone.8)> @ imm = #-0x26c
  38a4c0: e5903000     	ldr	r3, [r0]
  38a4c4: e513300c     	ldr	r3, [r3, #-0xc]
  38a4c8: e0800003     	add	r0, r0, r3
  38a4cc: e5903008     	ldr	r3, [r0, #0x8]
  38a4d0: e3130005     	tst	r3, #5
  38a4d4: 0a000074     	beq	0x38a6ac <Module::_ChooseXmls(std::string&, std::string&) const+0x320> @ imm = #0x1d0
  38a4d8: e1a01009     	mov	r1, r9
  38a4dc: e1a00007     	mov	r0, r7
  38a4e0: ebfe857c     	bl	0x32bad8 <std::vector<std::string, std::allocator<std::string>>::push_back(std::string const&)> @ imm = #-0x5ea10
  38a4e4: e59a23ec     	ldr	r2, [r10, #0x3ec]
  38a4e8: e59a33e8     	ldr	r3, [r10, #0x3e8]
  38a4ec: e1520003     	cmp	r2, r3
  38a4f0: 0a000097     	beq	0x38a754 <Module::_ChooseXmls(std::string&, std::string&) const+0x3c8> @ imm = #0x25c
  38a4f4: e28d904c     	add	r9, sp, #76
  38a4f8: e28a1ff6     	add	r1, r10, #984
  38a4fc: e1a00009     	mov	r0, r9
  38a500: ebfffd59     	bl	0x389a6c <std::basic_stringstream<char, std::char_traits<char>, std::allocator<char>>::basic_stringstream(std::string const&, int) (.clone.4)> @ imm = #-0xa9c
  38a504: e28d1020     	add	r1, sp, #32
  38a508: e28d2044     	add	r2, sp, #68
  38a50c: e3a0a000     	mov	r10, #0
  38a510: e58d1018     	str	r1, [sp, #0x18]
  38a514: e58d201c     	str	r2, [sp, #0x1c]
  38a518: e1a00009     	mov	r0, r9
  38a51c: e1a01004     	mov	r1, r4
  38a520: ebffff4c     	bl	0x38a258 <std::basic_istream<char, std::char_traits<char>>& std::getline<char, std::char_traits<char>, std::allocator<char>>(std::basic_istream<char, std::char_traits<char>>&, std::basic_string<char, std::char_traits<char>, std::allocator<char>>&, char) (.clone.8)> @ imm = #-0x2d0
  38a524: e5903000     	ldr	r3, [r0]
  38a528: e513300c     	ldr	r3, [r3, #-0xc]
  38a52c: e0800003     	add	r0, r0, r3
  38a530: e5903008     	ldr	r3, [r0, #0x8]
  38a534: e3130005     	tst	r3, #5
  38a538: 0a00005f     	beq	0x38a6bc <Module::_ChooseXmls(std::string&, std::string&) const+0x330> @ imm = #0x17c
  38a53c: e59d1024     	ldr	r1, [sp, #0x24]
  38a540: e59d3028     	ldr	r3, [sp, #0x28]
  38a544: e26aa064     	rsb	r10, r10, #100
  38a548: e58da048     	str	r10, [sp, #0x48]
  38a54c: e1510003     	cmp	r1, r3
  38a550: 0a0000c1     	beq	0x38a85c <Module::_ChooseXmls(std::string&, std::string&) const+0x4d0> @ imm = #0x304
  38a554: e581a000     	str	r10, [r1]
  38a558: e59d3024     	ldr	r3, [sp, #0x24]
  38a55c: e2833004     	add	r3, r3, #4
  38a560: e58d3024     	str	r3, [sp, #0x24]
  38a564: e1a00009     	mov	r0, r9
  38a568: ebfffb75     	bl	0x389344 <std::basic_stringstream<char, std::char_traits<char>, std::allocator<char>>::~basic_stringstream()> @ imm = #-0x122c
  38a56c: e59d1024     	ldr	r1, [sp, #0x24]
  38a570: e59d203c     	ldr	r2, [sp, #0x3c]
  38a574: e59d3038     	ldr	r3, [sp, #0x38]
  38a578: e0633002     	rsb	r3, r3, r2
  38a57c: e1a031c3     	asr	r3, r3, #3
  38a580: e0839103     	add	r9, r3, r3, lsl #2
  38a584: e0899209     	add	r9, r9, r9, lsl #4
  38a588: e0899409     	add	r9, r9, r9, lsl #8
  38a58c: e0899809     	add	r9, r9, r9, lsl #16
  38a590: e0839089     	add	r9, r3, r9, lsl #1
  38a594: e59d3020     	ldr	r3, [sp, #0x20]
  38a598: e0631001     	rsb	r1, r3, r1
  38a59c: e1590141     	cmp	r9, r1, asr #2
  38a5a0: 0a000092     	beq	0x38a7f0 <Module::_ChooseXmls(std::string&, std::string&) const+0x464> @ imm = #0x248
  38a5a4: e59f32cc     	ldr	r3, [pc, #0x2cc]        @ 0x38a878 <Module::_ChooseXmls(std::string&, std::string&) const+0x4ec>
  38a5a8: e79b3003     	ldr	r3, [r11, r3]
  38a5ac: e5933000     	ldr	r3, [r3]
  38a5b0: e3530002     	cmp	r3, #2
  38a5b4: 03a03000     	moveq	r3, #0
  38a5b8: 05833000     	streq	r3, [r3]
  38a5bc: 0a000001     	beq	0x38a5c8 <Module::_ChooseXmls(std::string&, std::string&) const+0x23c> @ imm = #0x4
  38a5c0: e3530001     	cmp	r3, #1
  38a5c4: 0a000097     	beq	0x38a828 <Module::_ChooseXmls(std::string&, std::string&) const+0x49c> @ imm = #0x25c
  38a5c8: e3a00064     	mov	r0, #100
  38a5cc: ebfff9a1     	bl	0x388c58 <Random::GetRandom(int, bool) (.clone.3)> @ imm = #-0x197c
  38a5d0: e59da038     	ldr	r10, [sp, #0x38]
  38a5d4: e59d303c     	ldr	r3, [sp, #0x3c]
  38a5d8: e06a3003     	rsb	r3, r10, r3
  38a5dc: e1a031c3     	asr	r3, r3, #3
  38a5e0: e083e103     	add	lr, r3, r3, lsl #2
  38a5e4: e08ee20e     	add	lr, lr, lr, lsl #4
  38a5e8: e08ee40e     	add	lr, lr, lr, lsl #8
  38a5ec: e08ee80e     	add	lr, lr, lr, lsl #16
  38a5f0: e093e08e     	adds	lr, r3, lr, lsl #1
  38a5f4: 0a00000d     	beq	0x38a630 <Module::_ChooseXmls(std::string&, std::string&) const+0x2a4> @ imm = #0x34
  38a5f8: e59dc020     	ldr	r12, [sp, #0x20]
  38a5fc: e59c2000     	ldr	r2, [r12]
  38a600: e1500002     	cmp	r0, r2
  38a604: b3a09000     	movlt	r9, #0
  38a608: ba00003f     	blt	0x38a70c <Module::_ChooseXmls(std::string&, std::string&) const+0x380> @ imm = #0xfc
  38a60c: e3a03000     	mov	r3, #0
  38a610: ea000003     	b	0x38a624 <Module::_ChooseXmls(std::string&, std::string&) const+0x298> @ imm = #0xc
  38a614: e79c1103     	ldr	r1, [r12, r3, lsl #2]
  38a618: e0822001     	add	r2, r2, r1
  38a61c: e1520000     	cmp	r2, r0
  38a620: ca000037     	bgt	0x38a704 <Module::_ChooseXmls(std::string&, std::string&) const+0x378> @ imm = #0xdc
  38a624: e2833001     	add	r3, r3, #1
  38a628: e153000e     	cmp	r3, lr
  38a62c: 1afffff8     	bne	0x38a614 <Module::_ChooseXmls(std::string&, std::string&) const+0x288> @ imm = #-0x20
  38a630: e1a00006     	mov	r0, r6
  38a634: ebfffb42     	bl	0x389344 <std::basic_stringstream<char, std::char_traits<char>, std::allocator<char>>::~basic_stringstream()> @ imm = #-0x12f8
  38a638: e1a00005     	mov	r0, r5
  38a63c: ebfffb40     	bl	0x389344 <std::basic_stringstream<char, std::char_traits<char>, std::allocator<char>>::~basic_stringstream()> @ imm = #-0x1300
  38a640: e1a00004     	mov	r0, r4
  38a644: ebfe24d8     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x76ca0
  38a648: e59d0020     	ldr	r0, [sp, #0x20]
  38a64c: e3500000     	cmp	r0, #0
  38a650: 0a000005     	beq	0x38a66c <Module::_ChooseXmls(std::string&, std::string&) const+0x2e0> @ imm = #0x14
  38a654: e59d1028     	ldr	r1, [sp, #0x28]
  38a658: e0601001     	rsb	r1, r0, r1
  38a65c: e3c11003     	bic	r1, r1, #3
  38a660: e3510080     	cmp	r1, #128
  38a664: 8a00006d     	bhi	0x38a820 <Module::_ChooseXmls(std::string&, std::string&) const+0x494> @ imm = #0x1b4
  38a668: eb0dfa24     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x37e890
  38a66c: e1a00007     	mov	r0, r7
  38a670: ebfe262e     	bl	0x313f30 <std::vector<std::string, std::allocator<std::string>>::~vector()> @ imm = #-0x76748
  38a674: e1a00008     	mov	r0, r8
  38a678: ebfe262c     	bl	0x313f30 <std::vector<std::string, std::allocator<std::string>>::~vector()> @ imm = #-0x76750
  38a67c: e59d0014     	ldr	r0, [sp, #0x14]
  38a680: e59d222c     	ldr	r2, [sp, #0x22c]
  38a684: e79b3000     	ldr	r3, [r11, r0]
  38a688: e5933000     	ldr	r3, [r3]
  38a68c: e1520003     	cmp	r2, r3
  38a690: 1a000075     	bne	0x38a86c <Module::_ChooseXmls(std::string&, std::string&) const+0x4e0> @ imm = #0x1d4
  38a694: e28ddf8d     	add	sp, sp, #564
  38a698: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  38a69c: e1a00008     	mov	r0, r8
  38a6a0: e1a01004     	mov	r1, r4
  38a6a4: ebfe850b     	bl	0x32bad8 <std::vector<std::string, std::allocator<std::string>>::push_back(std::string const&)> @ imm = #-0x5ebd4
  38a6a8: eaffff70     	b	0x38a470 <Module::_ChooseXmls(std::string&, std::string&) const+0xe4> @ imm = #-0x240
  38a6ac: e1a00007     	mov	r0, r7
  38a6b0: e1a01004     	mov	r1, r4
  38a6b4: ebfe8507     	bl	0x32bad8 <std::vector<std::string, std::allocator<std::string>>::push_back(std::string const&)> @ imm = #-0x5ebe4
  38a6b8: eaffff7d     	b	0x38a4b4 <Module::_ChooseXmls(std::string&, std::string&) const+0x128> @ imm = #-0x20c
  38a6bc: e59d0228     	ldr	r0, [sp, #0x228]
  38a6c0: ebfe0e73     	bl	0x30e094 <.plt+0x320>   @ imm = #-0x7c634
  38a6c4: e59d1024     	ldr	r1, [sp, #0x24]
  38a6c8: e59d3028     	ldr	r3, [sp, #0x28]
  38a6cc: e58d0044     	str	r0, [sp, #0x44]
  38a6d0: e1510003     	cmp	r1, r3
  38a6d4: 0a000006     	beq	0x38a6f4 <Module::_ChooseXmls(std::string&, std::string&) const+0x368> @ imm = #0x18
  38a6d8: e5810000     	str	r0, [r1]
  38a6dc: e59d3024     	ldr	r3, [sp, #0x24]
  38a6e0: e2833004     	add	r3, r3, #4
  38a6e4: e58d3024     	str	r3, [sp, #0x24]
  38a6e8: e59d3044     	ldr	r3, [sp, #0x44]
  38a6ec: e08aa003     	add	r10, r10, r3
  38a6f0: eaffff88     	b	0x38a518 <Module::_ChooseXmls(std::string&, std::string&) const+0x18c> @ imm = #-0x1e0
  38a6f4: e59d0018     	ldr	r0, [sp, #0x18]
  38a6f8: e59d201c     	ldr	r2, [sp, #0x1c]
  38a6fc: ebfffbc9     	bl	0x389628 <std::vector<int, std::allocator<int>>::_M_insert_overflow(int*, int const&, std::__true_type const&, unsigned int, bool) (.clone.9)> @ imm = #-0x10dc
  38a700: eafffff8     	b	0x38a6e8 <Module::_ChooseXmls(std::string&, std::string&) const+0x35c> @ imm = #-0x20
  38a704: e3a09018     	mov	r9, #24
  38a708: e0090399     	mul	r9, r9, r3
  38a70c: e59d100c     	ldr	r1, [sp, #0xc]
  38a710: e08aa009     	add	r10, r10, r9
  38a714: e151000a     	cmp	r1, r10
  38a718: 0a000003     	beq	0x38a72c <Module::_ChooseXmls(std::string&, std::string&) const+0x3a0> @ imm = #0xc
  38a71c: e1a00001     	mov	r0, r1
  38a720: e59a2010     	ldr	r2, [r10, #0x10]
  38a724: e59a1014     	ldr	r1, [r10, #0x14]
  38a728: ebfe18ac     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x79d50
  38a72c: e59d302c     	ldr	r3, [sp, #0x2c]
  38a730: e59d2010     	ldr	r2, [sp, #0x10]
  38a734: e0839009     	add	r9, r3, r9
  38a738: e1520009     	cmp	r2, r9
  38a73c: 0affffbb     	beq	0x38a630 <Module::_ChooseXmls(std::string&, std::string&) const+0x2a4> @ imm = #-0x114
  38a740: e1a00002     	mov	r0, r2
  38a744: e5991014     	ldr	r1, [r9, #0x14]
  38a748: e5992010     	ldr	r2, [r9, #0x10]
  38a74c: ebfe18a3     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x79d74
  38a750: eaffffb6     	b	0x38a630 <Module::_ChooseXmls(std::string&, std::string&) const+0x2a4> @ imm = #-0x128
  38a754: e59d203c     	ldr	r2, [sp, #0x3c]
  38a758: e59d3038     	ldr	r3, [sp, #0x38]
  38a75c: e3a00064     	mov	r0, #100
  38a760: e0633002     	rsb	r3, r3, r2
  38a764: e1a031c3     	asr	r3, r3, #3
  38a768: e0839103     	add	r9, r3, r3, lsl #2
  38a76c: e0899209     	add	r9, r9, r9, lsl #4
  38a770: e0899409     	add	r9, r9, r9, lsl #8
  38a774: e0899809     	add	r9, r9, r9, lsl #16
  38a778: e0839089     	add	r9, r3, r9, lsl #1
  38a77c: e1a01009     	mov	r1, r9
  38a780: ebfe0ec7     	bl	0x30e2a4 <.plt+0x530>   @ imm = #-0x7c4e4
  38a784: e3590000     	cmp	r9, #0
  38a788: e58d0044     	str	r0, [sp, #0x44]
  38a78c: d59d1024     	ldrle	r1, [sp, #0x24]
  38a790: daffff7f     	ble	0x38a594 <Module::_ChooseXmls(std::string&, std::string&) const+0x208> @ imm = #-0x204
  38a794: e28d3020     	add	r3, sp, #32
  38a798: e28d0044     	add	r0, sp, #68
  38a79c: e59d1024     	ldr	r1, [sp, #0x24]
  38a7a0: e3a0a000     	mov	r10, #0
  38a7a4: e58d3018     	str	r3, [sp, #0x18]
  38a7a8: e58d001c     	str	r0, [sp, #0x1c]
  38a7ac: ea000007     	b	0x38a7d0 <Module::_ChooseXmls(std::string&, std::string&) const+0x444> @ imm = #0x1c
  38a7b0: e59d3044     	ldr	r3, [sp, #0x44]
  38a7b4: e5813000     	str	r3, [r1]
  38a7b8: e59d1024     	ldr	r1, [sp, #0x24]
  38a7bc: e2811004     	add	r1, r1, #4
  38a7c0: e58d1024     	str	r1, [sp, #0x24]
  38a7c4: e28aa001     	add	r10, r10, #1
  38a7c8: e15a0009     	cmp	r10, r9
  38a7cc: 0affff67     	beq	0x38a570 <Module::_ChooseXmls(std::string&, std::string&) const+0x1e4> @ imm = #-0x264
  38a7d0: e59d3028     	ldr	r3, [sp, #0x28]
  38a7d4: e1530001     	cmp	r3, r1
  38a7d8: 1afffff4     	bne	0x38a7b0 <Module::_ChooseXmls(std::string&, std::string&) const+0x424> @ imm = #-0x30
  38a7dc: e59d0018     	ldr	r0, [sp, #0x18]
  38a7e0: e59d201c     	ldr	r2, [sp, #0x1c]
  38a7e4: ebfffb8f     	bl	0x389628 <std::vector<int, std::allocator<int>>::_M_insert_overflow(int*, int const&, std::__true_type const&, unsigned int, bool) (.clone.9)> @ imm = #-0x11c4
  38a7e8: e59d1024     	ldr	r1, [sp, #0x24]
  38a7ec: eafffff4     	b	0x38a7c4 <Module::_ChooseXmls(std::string&, std::string&) const+0x438> @ imm = #-0x30
  38a7f0: e59d2030     	ldr	r2, [sp, #0x30]
  38a7f4: e59d302c     	ldr	r3, [sp, #0x2c]
  38a7f8: e0633002     	rsb	r3, r3, r2
  38a7fc: e1a031c3     	asr	r3, r3, #3
  38a800: e0832103     	add	r2, r3, r3, lsl #2
  38a804: e0822202     	add	r2, r2, r2, lsl #4
  38a808: e0822402     	add	r2, r2, r2, lsl #8
  38a80c: e0822802     	add	r2, r2, r2, lsl #16
  38a810: e0833082     	add	r3, r3, r2, lsl #1
  38a814: e1590003     	cmp	r9, r3
  38a818: 1affff61     	bne	0x38a5a4 <Module::_ChooseXmls(std::string&, std::string&) const+0x218> @ imm = #-0x27c
  38a81c: eaffff69     	b	0x38a5c8 <Module::_ChooseXmls(std::string&, std::string&) const+0x23c> @ imm = #-0x25c
  38a820: ebfe1706     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x7a3e8
  38a824: eaffff90     	b	0x38a66c <Module::_ChooseXmls(std::string&, std::string&) const+0x2e0> @ imm = #-0x1c0
  38a828: e59f004c     	ldr	r0, [pc, #0x4c]         @ 0x38a87c <Module::_ChooseXmls(std::string&, std::string&) const+0x4f0>
  38a82c: e59f104c     	ldr	r1, [pc, #0x4c]         @ 0x38a880 <Module::_ChooseXmls(std::string&, std::string&) const+0x4f4>
  38a830: e59f204c     	ldr	r2, [pc, #0x4c]         @ 0x38a884 <Module::_ChooseXmls(std::string&, std::string&) const+0x4f8>
  38a834: e79b0000     	ldr	r0, [r11, r0]
  38a838: e59f3048     	ldr	r3, [pc, #0x48]         @ 0x38a888 <Module::_ChooseXmls(std::string&, std::string&) const+0x4fc>
  38a83c: e3a0c0bd     	mov	r12, #189
  38a840: e08f1001     	add	r1, pc, r1
  38a844: e08f2002     	add	r2, pc, r2
  38a848: e08f3003     	add	r3, pc, r3
  38a84c: e28000a8     	add	r0, r0, #168
  38a850: e58dc000     	str	r12, [sp]
  38a854: ebfe0dea     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x7c858
  38a858: eaffff5a     	b	0x38a5c8 <Module::_ChooseXmls(std::string&, std::string&) const+0x23c> @ imm = #-0x298
  38a85c: e28d0020     	add	r0, sp, #32
  38a860: e28d2048     	add	r2, sp, #72
  38a864: ebfffb6f     	bl	0x389628 <std::vector<int, std::allocator<int>>::_M_insert_overflow(int*, int const&, std::__true_type const&, unsigned int, bool) (.clone.9)> @ imm = #-0x1244
  38a868: eaffff3d     	b	0x38a564 <Module::_ChooseXmls(std::string&, std::string&) const+0x1d8> @ imm = #-0x30c
  38a86c: ebfe0ea7     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x7c564
  38a870: f4 a6 60 00  	.word	0x0060a6f4
  38a874: ac 40 00 00  	.word	0x000040ac
  38a878: c0 39 00 00  	.word	0x000039c0
  38a87c: c0 19 00 00  	.word	0x000019c0
  38a880: 98 3b 53 00  	.word	0x00533b98
  38a884: 24 3d 53 00  	.word	0x00533d24
  38a888: 28 7a 53 00  	.word	0x00537a28
