
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00386630 <GSLevel::Update(StateMachine*, double)>:
  386630: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  386634: e59f41c0     	ldr	r4, [pc, #0x1c0]        @ 0x3867fc <GSLevel::Update(StateMachine*, double)+0x1cc>
  386638: e59f31c0     	ldr	r3, [pc, #0x1c0]        @ 0x386800 <GSLevel::Update(StateMachine*, double)+0x1d0>
  38663c: e59f51c0     	ldr	r5, [pc, #0x1c0]        @ 0x386804 <GSLevel::Update(StateMachine*, double)+0x1d4>
  386640: e08f4004     	add	r4, pc, r4
  386644: e7942003     	ldr	r2, [r4, r3]
  386648: e7943005     	ldr	r3, [r4, r5]
  38664c: e24dd044     	sub	sp, sp, #68
  386650: e5d220ec     	ldrb	r2, [r2, #0xec]
  386654: e5933000     	ldr	r3, [r3]
  386658: e1a06000     	mov	r6, r0
  38665c: e3520000     	cmp	r2, #0
  386660: e58d303c     	str	r3, [sp, #0x3c]
  386664: 1a000038     	bne	0x38674c <GSLevel::Update(StateMachine*, double)+0x11c> @ imm = #0xe0
  386668: e5903038     	ldr	r3, [r0, #0x38]
  38666c: e2433001     	sub	r3, r3, #1
  386670: e3530003     	cmp	r3, #3
  386674: 908ff103     	addls	pc, pc, r3, lsl #2
  386678: ea000024     	b	0x386710 <GSLevel::Update(StateMachine*, double)+0xe0> @ imm = #0x90
  38667c: ea000039     	b	0x386768 <GSLevel::Update(StateMachine*, double)+0x138> @ imm = #0xe4
  386680: ea00003c     	b	0x386778 <GSLevel::Update(StateMachine*, double)+0x148> @ imm = #0xf0
  386684: ea000041     	b	0x386790 <GSLevel::Update(StateMachine*, double)+0x160> @ imm = #0x104
  386688: eaffffff     	b	0x38668c <GSLevel::Update(StateMachine*, double)+0x5c> @ imm = #-0x4
  38668c: e59f3174     	ldr	r3, [pc, #0x174]        @ 0x386808 <GSLevel::Update(StateMachine*, double)+0x1d8>
  386690: e59f2174     	ldr	r2, [pc, #0x174]        @ 0x38680c <GSLevel::Update(StateMachine*, double)+0x1dc>
  386694: e59f9174     	ldr	r9, [pc, #0x174]        @ 0x386810 <GSLevel::Update(StateMachine*, double)+0x1e0>
  386698: e7948003     	ldr	r8, [r4, r3]
  38669c: e59f3170     	ldr	r3, [pc, #0x170]        @ 0x386814 <GSLevel::Update(StateMachine*, double)+0x1e4>
  3866a0: e794b002     	ldr	r11, [r4, r2]
  3866a4: e3a0a000     	mov	r10, #0
  3866a8: e7943003     	ldr	r3, [r4, r3]
  3866ac: e08f9009     	add	r9, pc, r9
  3866b0: e28d7024     	add	r7, sp, #36
  3866b4: e583a000     	str	r10, [r3]
  3866b8: e1a00008     	mov	r0, r8
  3866bc: e5cba000     	strb	r10, [r11]
  3866c0: e289900d     	add	r9, r9, #13
  3866c4: ebfec46f     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0x4ee44
  3866c8: e1a00007     	mov	r0, r7
  3866cc: e1a01009     	mov	r1, r9
  3866d0: e58d7034     	str	r7, [sp, #0x34]
  3866d4: e58d7038     	str	r7, [sp, #0x38]
  3866d8: ebffffc0     	bl	0x3865e0 <std::string::_M_range_initialize(char const*, char const*) (.clone.2)> @ imm = #-0x100
  3866dc: e1a01007     	mov	r1, r7
  3866e0: e1a00008     	mov	r0, r8
  3866e4: ebfec4e7     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0x4ec64
  3866e8: e1a03000     	mov	r3, r0
  3866ec: e1a00007     	mov	r0, r7
  3866f0: e58d3004     	str	r3, [sp, #0x4]
  3866f4: ebfe34ac     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x72d50
  3866f8: e59d3004     	ldr	r3, [sp, #0x4]
  3866fc: e153000a     	cmp	r3, r10
  386700: 1a00002b     	bne	0x3867b4 <GSLevel::Update(StateMachine*, double)+0x184> @ imm = #0xac
  386704: e5960034     	ldr	r0, [r6, #0x34]
  386708: e3a01000     	mov	r1, #0
  38670c: eb01c6f1     	bl	0x3f82d8 <Level::Update(bool)> @ imm = #0x71bc4
  386710: e5963034     	ldr	r3, [r6, #0x34]
  386714: e3530000     	cmp	r3, #0
  386718: 0a000008     	beq	0x386740 <GSLevel::Update(StateMachine*, double)+0x110> @ imm = #0x20
  38671c: e5d32145     	ldrb	r2, [r3, #0x145]
  386720: e3520000     	cmp	r2, #0
  386724: 1a000008     	bne	0x38674c <GSLevel::Update(StateMachine*, double)+0x11c> @ imm = #0x20
  386728: e5932130     	ldr	r2, [r3, #0x130]
  38672c: e3520001     	cmp	r2, #1
  386730: da000002     	ble	0x386740 <GSLevel::Update(StateMachine*, double)+0x110> @ imm = #0x8
  386734: e5933130     	ldr	r3, [r3, #0x130]
  386738: e353001a     	cmp	r3, #26
  38673c: da000002     	ble	0x38674c <GSLevel::Update(StateMachine*, double)+0x11c> @ imm = #0x8
  386740: eb0298d1     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0xa6344
  386744: e3a01001     	mov	r1, #1
  386748: eb02a0ad     	bl	0x42ea04 <MenuManager::Update(bool)> @ imm = #0xa82b4
  38674c: e7943005     	ldr	r3, [r4, r5]
  386750: e59d203c     	ldr	r2, [sp, #0x3c]
  386754: e5933000     	ldr	r3, [r3]
  386758: e1520003     	cmp	r2, r3
  38675c: 1a000025     	bne	0x3867f8 <GSLevel::Update(StateMachine*, double)+0x1c8> @ imm = #0x94
  386760: e28dd044     	add	sp, sp, #68
  386764: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  386768: e3a03002     	mov	r3, #2
  38676c: e5803038     	str	r3, [r0, #0x38]
  386770: e5903034     	ldr	r3, [r0, #0x34]
  386774: eaffffe6     	b	0x386714 <GSLevel::Update(StateMachine*, double)+0xe4> @ imm = #-0x68
  386778: e5900034     	ldr	r0, [r0, #0x34]
  38677c: eb01a2a5     	bl	0x3ef218 <Level::Load()> @ imm = #0x68a94
  386780: e3a03003     	mov	r3, #3
  386784: e5863038     	str	r3, [r6, #0x38]
  386788: e5963034     	ldr	r3, [r6, #0x34]
  38678c: eaffffe0     	b	0x386714 <GSLevel::Update(StateMachine*, double)+0xe4> @ imm = #-0x80
  386790: e5900034     	ldr	r0, [r0, #0x34]
  386794: e3a01000     	mov	r1, #0
  386798: eb01c6ce     	bl	0x3f82d8 <Level::Update(bool)> @ imm = #0x71b38
  38679c: e5963034     	ldr	r3, [r6, #0x34]
  3867a0: e5932130     	ldr	r2, [r3, #0x130]
  3867a4: e3520026     	cmp	r2, #38
  3867a8: 03a02004     	moveq	r2, #4
  3867ac: 05862038     	streq	r2, [r6, #0x38]
  3867b0: eaffffd7     	b	0x386714 <GSLevel::Update(StateMachine*, double)+0xe4> @ imm = #-0xa4
  3867b4: e3a03001     	mov	r3, #1
  3867b8: e28d700c     	add	r7, sp, #12
  3867bc: e5cb3000     	strb	r3, [r11]
  3867c0: e1a00008     	mov	r0, r8
  3867c4: ebfec42f     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0x4ef44
  3867c8: e1a01009     	mov	r1, r9
  3867cc: e1a00007     	mov	r0, r7
  3867d0: e58d701c     	str	r7, [sp, #0x1c]
  3867d4: e58d7020     	str	r7, [sp, #0x20]
  3867d8: ebffff80     	bl	0x3865e0 <std::string::_M_range_initialize(char const*, char const*) (.clone.2)> @ imm = #-0x200
  3867dc: e1a00008     	mov	r0, r8
  3867e0: e1a01007     	mov	r1, r7
  3867e4: e1a0200a     	mov	r2, r10
  3867e8: ebfec57b     	bl	0x337ddc <DebugSwitches::SetSwitch(std::string const&, bool)> @ imm = #-0x4ea14
  3867ec: e1a00007     	mov	r0, r7
  3867f0: ebfe346d     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x72e4c
  3867f4: eaffffc2     	b	0x386704 <GSLevel::Update(StateMachine*, double)+0xd4> @ imm = #-0xf8
  3867f8: ebfe1ec4     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x784f0
  3867fc: 50 e4 60 00  	.word	0x0060e450
  386800: f4 37 00 00  	.word	0x000037f4
  386804: ac 40 00 00  	.word	0x000040ac
  386808: 84 08 00 00  	.word	0x00000884
  38680c: d0 2f 00 00  	.word	0x00002fd0
  386810: 44 b9 53 00  	.word	0x0053b944
  386814: fc 2d 00 00  	.word	0x00002dfc
