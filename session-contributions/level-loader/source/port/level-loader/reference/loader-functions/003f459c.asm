
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f459c <Level::_LoadLightSet()>:
  3f459c: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3f45a0: e59f7320     	ldr	r7, [pc, #0x320]        @ 0x3f48c8 <Level::_LoadLightSet()+0x32c>
  3f45a4: e59fb320     	ldr	r11, [pc, #0x320]       @ 0x3f48cc <Level::_LoadLightSet()+0x330>
  3f45a8: e24dd09c     	sub	sp, sp, #156
  3f45ac: e08f7007     	add	r7, pc, r7
  3f45b0: e797300b     	ldr	r3, [r7, r11]
  3f45b4: e1a04000     	mov	r4, r0
  3f45b8: e5933000     	ldr	r3, [r3]
  3f45bc: e58d3094     	str	r3, [sp, #0x94]
  3f45c0: ebfe3428     	bl	0x381668 <Device::IsFixedPipeline()> @ imm = #-0x72f60
  3f45c4: e3500000     	cmp	r0, #0
  3f45c8: 0a000045     	beq	0x3f46e4 <Level::_LoadLightSet()+0x148> @ imm = #0x114
  3f45cc: e5943038     	ldr	r3, [r4, #0x38]
  3f45d0: e3530000     	cmp	r3, #0
  3f45d4: 0a0000a4     	beq	0x3f486c <Level::_LoadLightSet()+0x2d0> @ imm = #0x290
  3f45d8: e59312e4     	ldr	r1, [r3, #0x2e4]
  3f45dc: e59322e0     	ldr	r2, [r3, #0x2e0]
  3f45e0: e1510002     	cmp	r1, r2
  3f45e4: 0a00005a     	beq	0x3f4754 <Level::_LoadLightSet()+0x1b8> @ imm = #0x168
  3f45e8: e59f22e0     	ldr	r2, [pc, #0x2e0]        @ 0x3f48d0 <Level::_LoadLightSet()+0x334>
  3f45ec: e59f12e0     	ldr	r1, [pc, #0x2e0]        @ 0x3f48d4 <Level::_LoadLightSet()+0x338>
  3f45f0: e59f82e0     	ldr	r8, [pc, #0x2e0]        @ 0x3f48d8 <Level::_LoadLightSet()+0x33c>
  3f45f4: e58d2010     	str	r2, [sp, #0x10]
  3f45f8: e59f22dc     	ldr	r2, [pc, #0x2dc]        @ 0x3f48dc <Level::_LoadLightSet()+0x340>
  3f45fc: e58d100c     	str	r1, [sp, #0xc]
  3f4600: e08f8008     	add	r8, pc, r8
  3f4604: e08f2002     	add	r2, pc, r2
  3f4608: e58d2014     	str	r2, [sp, #0x14]
  3f460c: e59f22cc     	ldr	r2, [pc, #0x2cc]        @ 0x3f48e0 <Level::_LoadLightSet()+0x344>
  3f4610: e28d607c     	add	r6, sp, #124
  3f4614: e28da030     	add	r10, sp, #48
  3f4618: e08f2002     	add	r2, pc, r2
  3f461c: e58d2018     	str	r2, [sp, #0x18]
  3f4620: e59f22bc     	ldr	r2, [pc, #0x2bc]        @ 0x3f48e4 <Level::_LoadLightSet()+0x348>
  3f4624: e28d5064     	add	r5, sp, #100
  3f4628: e28d902c     	add	r9, sp, #44
  3f462c: e08f2002     	add	r2, pc, r2
  3f4630: e58d201c     	str	r2, [sp, #0x1c]
  3f4634: ea000015     	b	0x3f4690 <Level::_LoadLightSet()+0xf4> @ imm = #0x54
  3f4638: e59312e4     	ldr	r1, [r3, #0x2e4]
  3f463c: e1a0200a     	mov	r2, r10
  3f4640: e1a00006     	mov	r0, r6
  3f4644: ebfc7ea8     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe0560
  3f4648: e1a01008     	mov	r1, r8
  3f464c: e1a02009     	mov	r2, r9
  3f4650: e1a00005     	mov	r0, r5
  3f4654: ebfc7ea4     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe0570
  3f4658: e1a01006     	mov	r1, r6
  3f465c: e1a02005     	mov	r2, r5
  3f4660: e1a00004     	mov	r0, r4
  3f4664: ebfffd35     	bl	0x3f3b40 <Level::LoadFile(std::string const&, std::string const&)> @ imm = #-0xb2c
  3f4668: e1a03000     	mov	r3, r0
  3f466c: e1a00005     	mov	r0, r5
  3f4670: e58d3008     	str	r3, [sp, #0x8]
  3f4674: ebfc7ccc     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe0cd0
  3f4678: e1a00006     	mov	r0, r6
  3f467c: ebfc7cca     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe0cd8
  3f4680: e59d3008     	ldr	r3, [sp, #0x8]
  3f4684: e3530000     	cmp	r3, #0
  3f4688: 1a000031     	bne	0x3f4754 <Level::_LoadLightSet()+0x1b8> @ imm = #0xc4
  3f468c: e5943038     	ldr	r3, [r4, #0x38]
  3f4690: e3530000     	cmp	r3, #0
  3f4694: 1affffe7     	bne	0x3f4638 <Level::_LoadLightSet()+0x9c> @ imm = #-0x64
  3f4698: e59d100c     	ldr	r1, [sp, #0xc]
  3f469c: e7972001     	ldr	r2, [r7, r1]
  3f46a0: e5922000     	ldr	r2, [r2]
  3f46a4: e3520002     	cmp	r2, #2
  3f46a8: 05833000     	streq	r3, [r3]
  3f46ac: 0affffe1     	beq	0x3f4638 <Level::_LoadLightSet()+0x9c> @ imm = #-0x7c
  3f46b0: e3520001     	cmp	r2, #1
  3f46b4: 1affffdf     	bne	0x3f4638 <Level::_LoadLightSet()+0x9c> @ imm = #-0x84
  3f46b8: e59d2010     	ldr	r2, [sp, #0x10]
  3f46bc: e59d301c     	ldr	r3, [sp, #0x1c]
  3f46c0: e3a0cf77     	mov	r12, #476
  3f46c4: e7970002     	ldr	r0, [r7, r2]
  3f46c8: e59d1014     	ldr	r1, [sp, #0x14]
  3f46cc: e59d2018     	ldr	r2, [sp, #0x18]
  3f46d0: e28000a8     	add	r0, r0, #168
  3f46d4: e58dc000     	str	r12, [sp]
  3f46d8: ebfc6649     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xe66dc
  3f46dc: e5943038     	ldr	r3, [r4, #0x38]
  3f46e0: eaffffd4     	b	0x3f4638 <Level::_LoadLightSet()+0x9c> @ imm = #-0xb0
  3f46e4: e5943038     	ldr	r3, [r4, #0x38]
  3f46e8: e3530000     	cmp	r3, #0
  3f46ec: 1a000014     	bne	0x3f4744 <Level::_LoadLightSet()+0x1a8> @ imm = #0x50
  3f46f0: e59f21dc     	ldr	r2, [pc, #0x1dc]        @ 0x3f48d4 <Level::_LoadLightSet()+0x338>
  3f46f4: e7972002     	ldr	r2, [r7, r2]
  3f46f8: e5922000     	ldr	r2, [r2]
  3f46fc: e3520002     	cmp	r2, #2
  3f4700: 05833000     	streq	r3, [r3]
  3f4704: 0a00000e     	beq	0x3f4744 <Level::_LoadLightSet()+0x1a8> @ imm = #0x38
  3f4708: e3520001     	cmp	r2, #1
  3f470c: 1a00000c     	bne	0x3f4744 <Level::_LoadLightSet()+0x1a8> @ imm = #0x30
  3f4710: e59f01b8     	ldr	r0, [pc, #0x1b8]        @ 0x3f48d0 <Level::_LoadLightSet()+0x334>
  3f4714: e59f11cc     	ldr	r1, [pc, #0x1cc]        @ 0x3f48e8 <Level::_LoadLightSet()+0x34c>
  3f4718: e59f21cc     	ldr	r2, [pc, #0x1cc]        @ 0x3f48ec <Level::_LoadLightSet()+0x350>
  3f471c: e7970000     	ldr	r0, [r7, r0]
  3f4720: e59f31c8     	ldr	r3, [pc, #0x1c8]        @ 0x3f48f0 <Level::_LoadLightSet()+0x354>
  3f4724: e3a0cf77     	mov	r12, #476
  3f4728: e08f1001     	add	r1, pc, r1
  3f472c: e08f3003     	add	r3, pc, r3
  3f4730: e28000a8     	add	r0, r0, #168
  3f4734: e08f2002     	add	r2, pc, r2
  3f4738: e58dc000     	str	r12, [sp]
  3f473c: ebfc6630     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xe6740
  3f4740: e5943038     	ldr	r3, [r4, #0x38]
  3f4744: e59312cc     	ldr	r1, [r3, #0x2cc]
  3f4748: e59322c8     	ldr	r2, [r3, #0x2c8]
  3f474c: e1510002     	cmp	r1, r2
  3f4750: 1a000006     	bne	0x3f4770 <Level::_LoadLightSet()+0x1d4> @ imm = #0x18
  3f4754: e797300b     	ldr	r3, [r7, r11]
  3f4758: e59d2094     	ldr	r2, [sp, #0x94]
  3f475c: e5933000     	ldr	r3, [r3]
  3f4760: e1520003     	cmp	r2, r3
  3f4764: 1a000056     	bne	0x3f48c4 <Level::_LoadLightSet()+0x328> @ imm = #0x158
  3f4768: e28dd09c     	add	sp, sp, #156
  3f476c: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3f4770: e59f2158     	ldr	r2, [pc, #0x158]        @ 0x3f48d0 <Level::_LoadLightSet()+0x334>
  3f4774: e59f1158     	ldr	r1, [pc, #0x158]        @ 0x3f48d4 <Level::_LoadLightSet()+0x338>
  3f4778: e59f8174     	ldr	r8, [pc, #0x174]        @ 0x3f48f4 <Level::_LoadLightSet()+0x358>
  3f477c: e58d2010     	str	r2, [sp, #0x10]
  3f4780: e59f2170     	ldr	r2, [pc, #0x170]        @ 0x3f48f8 <Level::_LoadLightSet()+0x35c>
  3f4784: e58d100c     	str	r1, [sp, #0xc]
  3f4788: e08f8008     	add	r8, pc, r8
  3f478c: e08f2002     	add	r2, pc, r2
  3f4790: e58d2014     	str	r2, [sp, #0x14]
  3f4794: e59f2160     	ldr	r2, [pc, #0x160]        @ 0x3f48fc <Level::_LoadLightSet()+0x360>
  3f4798: e28d604c     	add	r6, sp, #76
  3f479c: e28da028     	add	r10, sp, #40
  3f47a0: e08f2002     	add	r2, pc, r2
  3f47a4: e58d2018     	str	r2, [sp, #0x18]
  3f47a8: e59f2150     	ldr	r2, [pc, #0x150]        @ 0x3f4900 <Level::_LoadLightSet()+0x364>
  3f47ac: e28d5034     	add	r5, sp, #52
  3f47b0: e28d9024     	add	r9, sp, #36
  3f47b4: e08f2002     	add	r2, pc, r2
  3f47b8: e58d201c     	str	r2, [sp, #0x1c]
  3f47bc: ea000015     	b	0x3f4818 <Level::_LoadLightSet()+0x27c> @ imm = #0x54
  3f47c0: e59312cc     	ldr	r1, [r3, #0x2cc]
  3f47c4: e1a0200a     	mov	r2, r10
  3f47c8: e1a00006     	mov	r0, r6
  3f47cc: ebfc7e46     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe06e8
  3f47d0: e1a01008     	mov	r1, r8
  3f47d4: e1a02009     	mov	r2, r9
  3f47d8: e1a00005     	mov	r0, r5
  3f47dc: ebfc7e42     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe06f8
  3f47e0: e1a01006     	mov	r1, r6
  3f47e4: e1a02005     	mov	r2, r5
  3f47e8: e1a00004     	mov	r0, r4
  3f47ec: ebfffcd3     	bl	0x3f3b40 <Level::LoadFile(std::string const&, std::string const&)> @ imm = #-0xcb4
  3f47f0: e1a03000     	mov	r3, r0
  3f47f4: e1a00005     	mov	r0, r5
  3f47f8: e58d3008     	str	r3, [sp, #0x8]
  3f47fc: ebfc7c6a     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe0e58
  3f4800: e1a00006     	mov	r0, r6
  3f4804: ebfc7c68     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe0e60
  3f4808: e59d3008     	ldr	r3, [sp, #0x8]
  3f480c: e3530000     	cmp	r3, #0
  3f4810: 1affffcf     	bne	0x3f4754 <Level::_LoadLightSet()+0x1b8> @ imm = #-0xc4
  3f4814: e5943038     	ldr	r3, [r4, #0x38]
  3f4818: e3530000     	cmp	r3, #0
  3f481c: 1affffe7     	bne	0x3f47c0 <Level::_LoadLightSet()+0x224> @ imm = #-0x64
  3f4820: e59d100c     	ldr	r1, [sp, #0xc]
  3f4824: e7972001     	ldr	r2, [r7, r1]
  3f4828: e5922000     	ldr	r2, [r2]
  3f482c: e3520002     	cmp	r2, #2
  3f4830: 05833000     	streq	r3, [r3]
  3f4834: 0affffe1     	beq	0x3f47c0 <Level::_LoadLightSet()+0x224> @ imm = #-0x7c
  3f4838: e3520001     	cmp	r2, #1
  3f483c: 1affffdf     	bne	0x3f47c0 <Level::_LoadLightSet()+0x224> @ imm = #-0x84
  3f4840: e59d2010     	ldr	r2, [sp, #0x10]
  3f4844: e59d301c     	ldr	r3, [sp, #0x1c]
  3f4848: e3a0cf77     	mov	r12, #476
  3f484c: e7970002     	ldr	r0, [r7, r2]
  3f4850: e59d1014     	ldr	r1, [sp, #0x14]
  3f4854: e59d2018     	ldr	r2, [sp, #0x18]
  3f4858: e28000a8     	add	r0, r0, #168
  3f485c: e58dc000     	str	r12, [sp]
  3f4860: ebfc65e7     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xe6864
  3f4864: e5943038     	ldr	r3, [r4, #0x38]
  3f4868: eaffffd4     	b	0x3f47c0 <Level::_LoadLightSet()+0x224> @ imm = #-0xb0
  3f486c: e59f2060     	ldr	r2, [pc, #0x60]         @ 0x3f48d4 <Level::_LoadLightSet()+0x338>
  3f4870: e7972002     	ldr	r2, [r7, r2]
  3f4874: e5922000     	ldr	r2, [r2]
  3f4878: e3520002     	cmp	r2, #2
  3f487c: 05833000     	streq	r3, [r3]
  3f4880: 0affff54     	beq	0x3f45d8 <Level::_LoadLightSet()+0x3c> @ imm = #-0x2b0
  3f4884: e3520001     	cmp	r2, #1
  3f4888: 1affff52     	bne	0x3f45d8 <Level::_LoadLightSet()+0x3c> @ imm = #-0x2b8
  3f488c: e59f003c     	ldr	r0, [pc, #0x3c]         @ 0x3f48d0 <Level::_LoadLightSet()+0x334>
  3f4890: e59f106c     	ldr	r1, [pc, #0x6c]         @ 0x3f4904 <Level::_LoadLightSet()+0x368>
  3f4894: e59f206c     	ldr	r2, [pc, #0x6c]         @ 0x3f4908 <Level::_LoadLightSet()+0x36c>
  3f4898: e7970000     	ldr	r0, [r7, r0]
  3f489c: e59f3068     	ldr	r3, [pc, #0x68]         @ 0x3f490c <Level::_LoadLightSet()+0x370>
  3f48a0: e3a0cf77     	mov	r12, #476
  3f48a4: e08f1001     	add	r1, pc, r1
  3f48a8: e08f3003     	add	r3, pc, r3
  3f48ac: e28000a8     	add	r0, r0, #168
  3f48b0: e08f2002     	add	r2, pc, r2
  3f48b4: e58dc000     	str	r12, [sp]
  3f48b8: ebfc65d1     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xe68bc
  3f48bc: e5943038     	ldr	r3, [r4, #0x38]
  3f48c0: eaffff44     	b	0x3f45d8 <Level::_LoadLightSet()+0x3c> @ imm = #-0x2f0
  3f48c4: ebfc6691     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xe65bc
  3f48c8: e4 04 5a 00  	.word	0x005a04e4
  3f48cc: ac 40 00 00  	.word	0x000040ac
  3f48d0: c0 19 00 00  	.word	0x000019c0
  3f48d4: c0 39 00 00  	.word	0x000039c0
  3f48d8: 30 be 4c 00  	.word	0x004cbe30
  3f48dc: d4 9d 4c 00  	.word	0x004c9dd4
  3f48e0: d8 15 4d 00  	.word	0x004d15d8
  3f48e4: 2c d6 4c 00  	.word	0x004cd62c
  3f48e8: b0 9c 4c 00  	.word	0x004c9cb0
  3f48ec: bc 14 4d 00  	.word	0x004d14bc
  3f48f0: 2c d5 4c 00  	.word	0x004cd52c
  3f48f4: a8 bc 4c 00  	.word	0x004cbca8
  3f48f8: 4c 9c 4c 00  	.word	0x004c9c4c
  3f48fc: 50 14 4d 00  	.word	0x004d1450
  3f4900: a4 d4 4c 00  	.word	0x004cd4a4
  3f4904: 34 9b 4c 00  	.word	0x004c9b34
  3f4908: 40 13 4d 00  	.word	0x004d1340
  3f490c: b0 d3 4c 00  	.word	0x004cd3b0
