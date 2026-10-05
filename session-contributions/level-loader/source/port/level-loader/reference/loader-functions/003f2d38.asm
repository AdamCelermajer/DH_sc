
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f2d38 <Level::_LoadBatchList()>:
  3f2d38: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3f2d3c: e59f6398     	ldr	r6, [pc, #0x398]        @ 0x3f30dc <Level::_LoadBatchList()+0x3a4>
  3f2d40: e59f9398     	ldr	r9, [pc, #0x398]        @ 0x3f30e0 <Level::_LoadBatchList()+0x3a8>
  3f2d44: e59f8398     	ldr	r8, [pc, #0x398]        @ 0x3f30e4 <Level::_LoadBatchList()+0x3ac>
  3f2d48: e08f6006     	add	r6, pc, r6
  3f2d4c: e7963009     	ldr	r3, [r6, r9]
  3f2d50: e7965008     	ldr	r5, [r6, r8]
  3f2d54: e24dd0bc     	sub	sp, sp, #188
  3f2d58: e5933000     	ldr	r3, [r3]
  3f2d5c: e58d000c     	str	r0, [sp, #0xc]
  3f2d60: e1a00005     	mov	r0, r5
  3f2d64: e58d30b4     	str	r3, [sp, #0xb4]
  3f2d68: ebfd12c6     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xbb4e8
  3f2d6c: e59f1374     	ldr	r1, [pc, #0x374]        @ 0x3f30e8 <Level::_LoadBatchList()+0x3b0>
  3f2d70: e28d409c     	add	r4, sp, #156
  3f2d74: e28d2038     	add	r2, sp, #56
  3f2d78: e08f1001     	add	r1, pc, r1
  3f2d7c: e1a00004     	mov	r0, r4
  3f2d80: ebfc84d9     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xdec9c
  3f2d84: e1a00005     	mov	r0, r5
  3f2d88: e1a01004     	mov	r1, r4
  3f2d8c: ebfd133d     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbb30c
  3f2d90: e1a05000     	mov	r5, r0
  3f2d94: e1a00004     	mov	r0, r4
  3f2d98: ebfc8303     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xdf3f4
  3f2d9c: e3550000     	cmp	r5, #0
  3f2da0: 1a00004a     	bne	0x3f2ed0 <Level::_LoadBatchList()+0x198> @ imm = #0x128
  3f2da4: e59f2340     	ldr	r2, [pc, #0x340]        @ 0x3f30ec <Level::_LoadBatchList()+0x3b4>
  3f2da8: e59f3340     	ldr	r3, [pc, #0x340]        @ 0x3f30f0 <Level::_LoadBatchList()+0x3b8>
  3f2dac: e59fb340     	ldr	r11, [pc, #0x340]       @ 0x3f30f4 <Level::_LoadBatchList()+0x3bc>
  3f2db0: e08f2002     	add	r2, pc, r2
  3f2db4: e58d2010     	str	r2, [sp, #0x10]
  3f2db8: e59f2338     	ldr	r2, [pc, #0x338]        @ 0x3f30f8 <Level::_LoadBatchList()+0x3c0>
  3f2dbc: e7963003     	ldr	r3, [r6, r3]
  3f2dc0: e08fb00b     	add	r11, pc, r11
  3f2dc4: e08f2002     	add	r2, pc, r2
  3f2dc8: e58d2004     	str	r2, [sp, #0x4]
  3f2dcc: e59f2328     	ldr	r2, [pc, #0x328]        @ 0x3f30fc <Level::_LoadBatchList()+0x3c4>
  3f2dd0: e5933038     	ldr	r3, [r3, #0x38]
  3f2dd4: e1a0a008     	mov	r10, r8
  3f2dd8: e08f2002     	add	r2, pc, r2
  3f2ddc: e58d2008     	str	r2, [sp, #0x8]
  3f2de0: e5934014     	ldr	r4, [r3, #0x14]
  3f2de4: e283700c     	add	r7, r3, #12
  3f2de8: e59f3310     	ldr	r3, [pc, #0x310]        @ 0x3f3100 <Level::_LoadBatchList()+0x3c8>
  3f2dec: e58d3014     	str	r3, [sp, #0x14]
  3f2df0: e1570004     	cmp	r7, r4
  3f2df4: 0a000035     	beq	0x3f2ed0 <Level::_LoadBatchList()+0x198> @ imm = #0xd4
  3f2df8: e28d5018     	add	r5, sp, #24
  3f2dfc: e594102c     	ldr	r1, [r4, #0x2c]
  3f2e00: e1a00005     	mov	r0, r5
  3f2e04: ebfd31c6     	bl	0x33f524 <ObjectHandle::ObjectHandle(ObjectBase*)> @ imm = #-0xb38e8
  3f2e08: e1a00005     	mov	r0, r5
  3f2e0c: ebfd3434     	bl	0x33fee4 <ObjectHandle::operator GameObject*()> @ imm = #-0xb2f30
  3f2e10: e2505000     	subs	r5, r0, #0
  3f2e14: 0a000022     	beq	0x3f2ea4 <Level::_LoadBatchList()+0x16c> @ imm = #0x88
  3f2e18: e5d53083     	ldrb	r3, [r5, #0x83]
  3f2e1c: e3530000     	cmp	r3, #0
  3f2e20: 1a00001f     	bne	0x3f2ea4 <Level::_LoadBatchList()+0x16c> @ imm = #0x7c
  3f2e24: e595805c     	ldr	r8, [r5, #0x5c]
  3f2e28: e1a0100b     	mov	r1, r11
  3f2e2c: e1a00008     	mov	r0, r8
  3f2e30: ebfc6d39     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xe4b1c
  3f2e34: e3500000     	cmp	r0, #0
  3f2e38: 1a00002b     	bne	0x3f2eec <Level::_LoadBatchList()+0x1b4> @ imm = #0xac
  3f2e3c: e796300a     	ldr	r3, [r6, r10]
  3f2e40: e28d8054     	add	r8, sp, #84
  3f2e44: e1a00003     	mov	r0, r3
  3f2e48: e58d3000     	str	r3, [sp]
  3f2e4c: ebfd128d     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xbb5cc
  3f2e50: e59d1010     	ldr	r1, [sp, #0x10]
  3f2e54: e28d202c     	add	r2, sp, #44
  3f2e58: e1a00008     	mov	r0, r8
  3f2e5c: ebfc84a2     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xded78
  3f2e60: e59d3000     	ldr	r3, [sp]
  3f2e64: e1a01008     	mov	r1, r8
  3f2e68: e1a00003     	mov	r0, r3
  3f2e6c: ebfd1305     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbb3ec
  3f2e70: e1a00008     	mov	r0, r8
  3f2e74: ebfc82cc     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xdf4d0
  3f2e78: e59d300c     	ldr	r3, [sp, #0xc]
  3f2e7c: e5930158     	ldr	r0, [r3, #0x158]
  3f2e80: e58d5024     	str	r5, [sp, #0x24]
  3f2e84: e5901014     	ldr	r1, [r0, #0x14]
  3f2e88: e5903018     	ldr	r3, [r0, #0x18]
  3f2e8c: e1510003     	cmp	r1, r3
  3f2e90: 0a00008c     	beq	0x3f30c8 <Level::_LoadBatchList()+0x390> @ imm = #0x230
  3f2e94: e5815000     	str	r5, [r1]
  3f2e98: e5903014     	ldr	r3, [r0, #0x14]
  3f2e9c: e2833004     	add	r3, r3, #4
  3f2ea0: e5803014     	str	r3, [r0, #0x14]
  3f2ea4: e594300c     	ldr	r3, [r4, #0xc]
  3f2ea8: e3530000     	cmp	r3, #0
  3f2eac: 1a000001     	bne	0x3f2eb8 <Level::_LoadBatchList()+0x180> @ imm = #0x4
  3f2eb0: ea000028     	b	0x3f2f58 <Level::_LoadBatchList()+0x220> @ imm = #0xa0
  3f2eb4: e1a03002     	mov	r3, r2
  3f2eb8: e5932008     	ldr	r2, [r3, #0x8]
  3f2ebc: e3520000     	cmp	r2, #0
  3f2ec0: 1afffffb     	bne	0x3f2eb4 <Level::_LoadBatchList()+0x17c> @ imm = #-0x14
  3f2ec4: e1a04003     	mov	r4, r3
  3f2ec8: e1570004     	cmp	r7, r4
  3f2ecc: 1affffc9     	bne	0x3f2df8 <Level::_LoadBatchList()+0xc0> @ imm = #-0xdc
  3f2ed0: e7963009     	ldr	r3, [r6, r9]
  3f2ed4: e59d20b4     	ldr	r2, [sp, #0xb4]
  3f2ed8: e5933000     	ldr	r3, [r3]
  3f2edc: e1520003     	cmp	r2, r3
  3f2ee0: 1a00007c     	bne	0x3f30d8 <Level::_LoadBatchList()+0x3a0> @ imm = #0x1f0
  3f2ee4: e28dd0bc     	add	sp, sp, #188
  3f2ee8: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3f2eec: e1a00008     	mov	r0, r8
  3f2ef0: e59d1004     	ldr	r1, [sp, #0x4]
  3f2ef4: ebfc6d08     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xe4be0
  3f2ef8: e3500000     	cmp	r0, #0
  3f2efc: 0affffce     	beq	0x3f2e3c <Level::_LoadBatchList()+0x104> @ imm = #-0xc8
  3f2f00: e1a00008     	mov	r0, r8
  3f2f04: e59d1008     	ldr	r1, [sp, #0x8]
  3f2f08: ebfc6d03     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xe4bf4
  3f2f0c: e3500000     	cmp	r0, #0
  3f2f10: 1a00001d     	bne	0x3f2f8c <Level::_LoadBatchList()+0x254> @ imm = #0x74
  3f2f14: e796800a     	ldr	r8, [r6, r10]
  3f2f18: e28d503c     	add	r5, sp, #60
  3f2f1c: e1a00008     	mov	r0, r8
  3f2f20: ebfd1258     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xbb6a0
  3f2f24: e59d3014     	ldr	r3, [sp, #0x14]
  3f2f28: e28d2028     	add	r2, sp, #40
  3f2f2c: e1a00005     	mov	r0, r5
  3f2f30: e08f1003     	add	r1, pc, r3
  3f2f34: ebfc846c     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xdee50
  3f2f38: e1a01005     	mov	r1, r5
  3f2f3c: e1a00008     	mov	r0, r8
  3f2f40: ebfd12d0     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbb4c0
  3f2f44: e1a00005     	mov	r0, r5
  3f2f48: ebfc8297     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xdf5a4
  3f2f4c: e594300c     	ldr	r3, [r4, #0xc]
  3f2f50: e3530000     	cmp	r3, #0
  3f2f54: 1affffd7     	bne	0x3f2eb8 <Level::_LoadBatchList()+0x180> @ imm = #-0xa4
  3f2f58: e5942004     	ldr	r2, [r4, #0x4]
  3f2f5c: e592100c     	ldr	r1, [r2, #0xc]
  3f2f60: e1510004     	cmp	r1, r4
  3f2f64: 1a000005     	bne	0x3f2f80 <Level::_LoadBatchList()+0x248> @ imm = #0x14
  3f2f68: e1a04002     	mov	r4, r2
  3f2f6c: e5922004     	ldr	r2, [r2, #0x4]
  3f2f70: e592300c     	ldr	r3, [r2, #0xc]
  3f2f74: e1540003     	cmp	r4, r3
  3f2f78: 0afffffa     	beq	0x3f2f68 <Level::_LoadBatchList()+0x230> @ imm = #-0x18
  3f2f7c: e594300c     	ldr	r3, [r4, #0xc]
  3f2f80: e1520003     	cmp	r2, r3
  3f2f84: 11a04002     	movne	r4, r2
  3f2f88: eaffff98     	b	0x3f2df0 <Level::_LoadBatchList()+0xb8> @ imm = #-0x1a0
  3f2f8c: e5950044     	ldr	r0, [r5, #0x44]
  3f2f90: e59d1008     	ldr	r1, [sp, #0x8]
  3f2f94: ebfc6f0e     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0xe43c8
  3f2f98: e3500000     	cmp	r0, #0
  3f2f9c: 1affffdc     	bne	0x3f2f14 <Level::_LoadBatchList()+0x1dc> @ imm = #-0x90
  3f2fa0: e59f115c     	ldr	r1, [pc, #0x15c]        @ 0x3f3104 <Level::_LoadBatchList()+0x3cc>
  3f2fa4: e1a00008     	mov	r0, r8
  3f2fa8: e08f1001     	add	r1, pc, r1
  3f2fac: ebfc6cda     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xe4c98
  3f2fb0: e3500000     	cmp	r0, #0
  3f2fb4: 01a08005     	moveq	r8, r5
  3f2fb8: 1a000013     	bne	0x3f300c <Level::_LoadBatchList()+0x2d4> @ imm = #0x4c
  3f2fbc: e1a00005     	mov	r0, r5
  3f2fc0: ebfe5ee6     	bl	0x38ab60 <GameObject::MeetCondition() const> @ imm = #-0x68468
  3f2fc4: e3500000     	cmp	r0, #0
  3f2fc8: 0a000035     	beq	0x3f30a4 <Level::_LoadBatchList()+0x36c> @ imm = #0xd4
  3f2fcc: e3580000     	cmp	r8, #0
  3f2fd0: 0a000003     	beq	0x3f2fe4 <Level::_LoadBatchList()+0x2ac> @ imm = #0xc
  3f2fd4: e1a00008     	mov	r0, r8
  3f2fd8: ebfec02d     	bl	0x3a3094 <Character::IsFaerie() const> @ imm = #-0x4ff4c
  3f2fdc: e3500000     	cmp	r0, #0
  3f2fe0: 1a00002f     	bne	0x3f30a4 <Level::_LoadBatchList()+0x36c> @ imm = #0xbc
  3f2fe4: e796300a     	ldr	r3, [r6, r10]
  3f2fe8: e28d8084     	add	r8, sp, #132
  3f2fec: e1a00003     	mov	r0, r3
  3f2ff0: e58d3000     	str	r3, [sp]
  3f2ff4: ebfd1223     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xbb774
  3f2ff8: e59f1108     	ldr	r1, [pc, #0x108]        @ 0x3f3108 <Level::_LoadBatchList()+0x3d0>
  3f2ffc: e28d2034     	add	r2, sp, #52
  3f3000: e1a00008     	mov	r0, r8
  3f3004: e08f1001     	add	r1, pc, r1
  3f3008: eaffff93     	b	0x3f2e5c <Level::_LoadBatchList()+0x124> @ imm = #-0x1b4
  3f300c: e59f10f8     	ldr	r1, [pc, #0xf8]         @ 0x3f310c <Level::_LoadBatchList()+0x3d4>
  3f3010: e1a00008     	mov	r0, r8
  3f3014: e08f1001     	add	r1, pc, r1
  3f3018: ebfc6cbf     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xe4d04
  3f301c: e3500000     	cmp	r0, #0
  3f3020: 1a000001     	bne	0x3f302c <Level::_LoadBatchList()+0x2f4> @ imm = #0x4
  3f3024: e3a08000     	mov	r8, #0
  3f3028: eaffffe3     	b	0x3f2fbc <Level::_LoadBatchList()+0x284> @ imm = #-0x74
  3f302c: e59f10dc     	ldr	r1, [pc, #0xdc]         @ 0x3f3110 <Level::_LoadBatchList()+0x3d8>
  3f3030: e1a00008     	mov	r0, r8
  3f3034: e08f1001     	add	r1, pc, r1
  3f3038: ebfc6cb7     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xe4d24
  3f303c: e3500000     	cmp	r0, #0
  3f3040: 0afffff7     	beq	0x3f3024 <Level::_LoadBatchList()+0x2ec> @ imm = #-0x24
  3f3044: e59f10c8     	ldr	r1, [pc, #0xc8]         @ 0x3f3114 <Level::_LoadBatchList()+0x3dc>
  3f3048: e1a00008     	mov	r0, r8
  3f304c: e08f1001     	add	r1, pc, r1
  3f3050: ebfc6cb1     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xe4d3c
  3f3054: e3500000     	cmp	r0, #0
  3f3058: 0afffff1     	beq	0x3f3024 <Level::_LoadBatchList()+0x2ec> @ imm = #-0x3c
  3f305c: e59f10b4     	ldr	r1, [pc, #0xb4]         @ 0x3f3118 <Level::_LoadBatchList()+0x3e0>
  3f3060: e1a00008     	mov	r0, r8
  3f3064: e08f1001     	add	r1, pc, r1
  3f3068: ebfc6cab     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xe4d54
  3f306c: e3500000     	cmp	r0, #0
  3f3070: 0affffeb     	beq	0x3f3024 <Level::_LoadBatchList()+0x2ec> @ imm = #-0x54
  3f3074: e59f10a0     	ldr	r1, [pc, #0xa0]         @ 0x3f311c <Level::_LoadBatchList()+0x3e4>
  3f3078: e1a00008     	mov	r0, r8
  3f307c: e08f1001     	add	r1, pc, r1
  3f3080: ebfc6ca5     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xe4d6c
  3f3084: e3500000     	cmp	r0, #0
  3f3088: 0affffe5     	beq	0x3f3024 <Level::_LoadBatchList()+0x2ec> @ imm = #-0x6c
  3f308c: e59f108c     	ldr	r1, [pc, #0x8c]         @ 0x3f3120 <Level::_LoadBatchList()+0x3e8>
  3f3090: e1a00008     	mov	r0, r8
  3f3094: e08f1001     	add	r1, pc, r1
  3f3098: ebfc6c9f     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xe4d84
  3f309c: e3500000     	cmp	r0, #0
  3f30a0: 0affffdf     	beq	0x3f3024 <Level::_LoadBatchList()+0x2ec> @ imm = #-0x84
  3f30a4: e796800a     	ldr	r8, [r6, r10]
  3f30a8: e28d506c     	add	r5, sp, #108
  3f30ac: e1a00008     	mov	r0, r8
  3f30b0: ebfd11f4     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xbb830
  3f30b4: e59f1068     	ldr	r1, [pc, #0x68]         @ 0x3f3124 <Level::_LoadBatchList()+0x3ec>
  3f30b8: e28d2030     	add	r2, sp, #48
  3f30bc: e1a00005     	mov	r0, r5
  3f30c0: e08f1001     	add	r1, pc, r1
  3f30c4: eaffff9a     	b	0x3f2f34 <Level::_LoadBatchList()+0x1fc> @ imm = #-0x198
  3f30c8: e2800010     	add	r0, r0, #16
  3f30cc: e28d2024     	add	r2, sp, #36
  3f30d0: ebfffa2a     	bl	0x3f1980 <std::vector<GameObject*, std::allocator<GameObject*>>::_M_insert_overflow(GameObject**, GameObject* const&, std::__true_type const&, unsigned int, bool) (.clone.17)> @ imm = #-0x1758
  3f30d4: eaffff72     	b	0x3f2ea4 <Level::_LoadBatchList()+0x16c> @ imm = #-0x238
  3f30d8: ebfc6c8c     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xe4dd0
  3f30dc: 48 1d 5a 00  	.word	0x005a1d48
  3f30e0: ac 40 00 00  	.word	0x000040ac
  3f30e4: 84 08 00 00  	.word	0x00000884
  3f30e8: 80 38 4d 00  	.word	0x004d3880
  3f30ec: 18 39 4d 00  	.word	0x004d3918
  3f30f0: f4 37 00 00  	.word	0x000037f4
  3f30f4: 70 d6 4c 00  	.word	0x004cd670
  3f30f8: c4 d6 4c 00  	.word	0x004cd6c4
  3f30fc: f0 d5 4c 00  	.word	0x004cd5f0
  3f3100: 98 37 4d 00  	.word	0x004d3798
  3f3104: 10 d5 4c 00  	.word	0x004cd510
  3f3108: c4 36 4d 00  	.word	0x004d36c4
  3f310c: ac d5 4c 00  	.word	0x004cd5ac
  3f3110: 64 d5 4c 00  	.word	0x004cd564
  3f3114: 14 d5 4c 00  	.word	0x004cd514
  3f3118: dc d4 4c 00  	.word	0x004cd4dc
  3f311c: b4 d4 4c 00  	.word	0x004cd4b4
  3f3120: ec d3 4c 00  	.word	0x004cd3ec
  3f3124: 08 36 4d 00  	.word	0x004d3608
