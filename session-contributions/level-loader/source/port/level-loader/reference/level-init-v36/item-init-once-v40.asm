
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003ece80 <ItemObject::InitOnce(int)>:
  3ece80: e59f30cc     	ldr	r3, [pc, #0xcc]         @ 0x3ecf54 <ItemObject::InitOnce(int)+0xd4>
  3ece84: e92d4070     	push	{r4, r5, r6, lr}
  3ece88: e08f3003     	add	r3, pc, r3
  3ece8c: e1a05001     	mov	r5, r1
  3ece90: e1a01003     	mov	r1, r3
  3ece94: e3a03feb     	mov	r3, #940
  3ece98: e18050b3     	strh	r5, [r0, r3]
  3ece9c: e1a04000     	mov	r4, r0
  3ecea0: e2812022     	add	r2, r1, #34
  3ecea4: e2800e29     	add	r0, r0, #656
  3ecea8: ebfc8ecc     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0xdc4d0
  3eceac: e59f30a4     	ldr	r3, [pc, #0xa4]         @ 0x3ecf58 <ItemObject::InitOnce(int)+0xd8>
  3eceb0: e3a02101     	mov	r2, #1073741824
  3eceb4: e3550000     	cmp	r5, #0
  3eceb8: e2822503     	add	r2, r2, #12582912
  3ecebc: e58423b0     	str	r2, [r4, #0x3b0]
  3ecec0: e08f3003     	add	r3, pc, r3
  3ecec4: ba000004     	blt	0x3ecedc <ItemObject::InitOnce(int)+0x5c> @ imm = #0x10
  3ecec8: e59f208c     	ldr	r2, [pc, #0x8c]         @ 0x3ecf5c <ItemObject::InitOnce(int)+0xdc>
  3ececc: e7932002     	ldr	r2, [r3, r2]
  3eced0: e5922000     	ldr	r2, [r2]
  3eced4: e1550002     	cmp	r5, r2
  3eced8: ba00000b     	blt	0x3ecf0c <ItemObject::InitOnce(int)+0x8c> @ imm = #0x2c
  3ecedc: e59f107c     	ldr	r1, [pc, #0x7c]         @ 0x3ecf60 <ItemObject::InitOnce(int)+0xe0>
  3ecee0: e2840faa     	add	r0, r4, #680
  3ecee4: e08f1001     	add	r1, pc, r1
  3ecee8: e2812012     	add	r2, r1, #18
  3eceec: ebfc8ebb     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0xdc514
  3ecef0: e1a00004     	mov	r0, r4
  3ecef4: ebfe7bd8     	bl	0x38be5c <GameObject::InitPost()> @ imm = #-0x610a0
  3ecef8: e59402d8     	ldr	r0, [r4, #0x2d8]
  3ecefc: e3500000     	cmp	r0, #0
  3ecf00: 0a000012     	beq	0x3ecf50 <ItemObject::InitOnce(int)+0xd0> @ imm = #0x48
  3ecf04: e8bd4070     	pop	{r4, r5, r6, lr}
  3ecf08: ea020ed1     	b	0x470a54 <VisualObject::ApplyMeshBox()> @ imm = #0x83b44
  3ecf0c: e59f2050     	ldr	r2, [pc, #0x50]         @ 0x3ecf64 <ItemObject::InitOnce(int)+0xe4>
  3ecf10: e7933002     	ldr	r3, [r3, r2]
  3ecf14: e3a02014     	mov	r2, #20
  3ecf18: e5933000     	ldr	r3, [r3]
  3ecf1c: e0253592     	mla	r5, r2, r5, r3
  3ecf20: e5955010     	ldr	r5, [r5, #0x10]
  3ecf24: e1a00005     	mov	r0, r5
  3ecf28: ebfc83c9     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0xdf0dc
  3ecf2c: e1a01005     	mov	r1, r5
  3ecf30: e0852000     	add	r2, r5, r0
  3ecf34: e2840faa     	add	r0, r4, #680
  3ecf38: ebfc8ea8     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0xdc560
  3ecf3c: e1a00004     	mov	r0, r4
  3ecf40: ebfe7bc5     	bl	0x38be5c <GameObject::InitPost()> @ imm = #-0x610ec
  3ecf44: e59402d8     	ldr	r0, [r4, #0x2d8]
  3ecf48: e3500000     	cmp	r0, #0
  3ecf4c: 1affffec     	bne	0x3ecf04 <ItemObject::InitOnce(int)+0x84> @ imm = #-0x50
  3ecf50: e8bd8070     	pop	{r4, r5, r6, pc}
  3ecf54: 18 94 4d 00  	.word	0x004d9418
  3ecf58: d0 7b 5a 00  	.word	0x005a7bd0
  3ecf5c: 10 45 00 00  	.word	0x00004510
  3ecf60: e4 93 4d 00  	.word	0x004d93e4
  3ecf64: 3c 09 00 00  	.word	0x0000093c

003ecf68 <std::basic_string<char, std::char_traits<char>, std::allocator<char>> std::operator+<char, std::char_traits<char>, std::allocator<char>>(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&)>:
  3ecf68: e92d4070     	push	{r4, r5, r6, lr}
  3ecf6c: e1a05002     	mov	r5, r2
  3ecf70: e591c010     	ldr	r12, [r1, #0x10]
  3ecf74: e5922010     	ldr	r2, [r2, #0x10]
  3ecf78: e5953014     	ldr	r3, [r5, #0x14]
  3ecf7c: e1a06001     	mov	r6, r1
  3ecf80: e5911014     	ldr	r1, [r1, #0x14]
  3ecf84: e0633002     	rsb	r3, r3, r2
  3ecf88: e1a04000     	mov	r4, r0
  3ecf8c: e061100c     	rsb	r1, r1, r12
  3ecf90: e0811003     	add	r1, r1, r3
  3ecf94: e5840010     	str	r0, [r4, #0x10]
  3ecf98: e5840014     	str	r0, [r4, #0x14]
  3ecf9c: e2811001     	add	r1, r1, #1
  3ecfa0: ebfc91b5     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0xdb92c
  3ecfa4: e5943010     	ldr	r3, [r4, #0x10]
  3ecfa8: e3a02000     	mov	r2, #0
  3ecfac: e1a00004     	mov	r0, r4
  3ecfb0: e5c32000     	strb	r2, [r3]
  3ecfb4: e5962010     	ldr	r2, [r6, #0x10]
  3ecfb8: e5961014     	ldr	r1, [r6, #0x14]
  3ecfbc: ebfc8e10     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0xdc7c0
  3ecfc0: e1a00004     	mov	r0, r4
  3ecfc4: e5952010     	ldr	r2, [r5, #0x10]
  3ecfc8: e5951014     	ldr	r1, [r5, #0x14]
  3ecfcc: ebfc8e0c     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0xdc7d0
  3ecfd0: e1a00004     	mov	r0, r4
  3ecfd4: e8bd8070     	pop	{r4, r5, r6, pc}

003ecfd8 <std::allocator<StatusMsg>::allocate(unsigned int, void const*) (.clone.4)>:
  3ecfd8: e52de004     	str	lr, [sp, #-0x4]!
  3ecfdc: e24dd00c     	sub	sp, sp, #12
  3ecfe0: e28d0008     	add	r0, sp, #8
  3ecfe4: e3a03070     	mov	r3, #112
  3ecfe8: e5203004     	str	r3, [r0, #-0x4]!
  3ecfec: eb0c6fb3     	bl	0x708ec0 <___ZNSt12__node_alloc11_M_allocateERj_veneer> @ imm = #0x31becc
  3ecff0: e28dd00c     	add	sp, sp, #12
  3ecff4: e8bd8000     	ldm	sp!, {pc}

003ecff8 <MenuMessageManager<StatusMsg, 4>::EnqueueMessage(StatusMsg const&, int, bool) (.clone.18)>:
  3ecff8: e92d40f0     	push	{r4, r5, r6, r7, lr}
  3ecffc: e59f412c     	ldr	r4, [pc, #0x12c]        @ 0x3ed130 <MenuMessageManager<StatusMsg, 4>::EnqueueMessage(StatusMsg const&, int, bool) (.clone.18)+0x138>
  3ed000: e59f512c     	ldr	r5, [pc, #0x12c]        @ 0x3ed134 <MenuMessageManager<StatusMsg, 4>::EnqueueMessage(StatusMsg const&, int, bool) (.clone.18)+0x13c>
  3ed004: e24dd034     	sub	sp, sp, #52
  3ed008: e08f4004     	add	r4, pc, r4
  3ed00c: e7945005     	ldr	r5, [r4, r5]
  3ed010: e1a01000     	mov	r1, r0
  3ed014: e2856004     	add	r6, r5, #4
  3ed018: e1a00006     	mov	r0, r6
  3ed01c: ebff449a     	bl	0x3be28c <std::deque<StatusMsg, std::allocator<StatusMsg>>::push_back(StatusMsg const&)> @ imm = #-0x2ed98
  3ed020: e896000f     	ldm	r6, {r0, r1, r2, r3}
  3ed024: e28dc00c     	add	r12, sp, #12
  3ed028: e88c000f     	stm	r12, {r0, r1, r2, r3}
  3ed02c: e2850014     	add	r0, r5, #20
  3ed030: e1a0100c     	mov	r1, r12
  3ed034: ebff3fd4     	bl	0x3bcf8c <std::priv::_Deque_iterator_base<StatusMsg>::_M_subtract(std::priv::_Deque_iterator_base<StatusMsg> const&) const> @ imm = #-0x300b0
  3ed038: e3500001     	cmp	r0, #1
  3ed03c: 0a000001     	beq	0x3ed048 <MenuMessageManager<StatusMsg, 4>::EnqueueMessage(StatusMsg const&, int, bool) (.clone.18)+0x50> @ imm = #0x4
  3ed040: e28dd034     	add	sp, sp, #52
  3ed044: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  3ed048: e59f30e8     	ldr	r3, [pc, #0xe8]         @ 0x3ed138 <MenuMessageManager<StatusMsg, 4>::EnqueueMessage(StatusMsg const&, int, bool) (.clone.18)+0x140>
  3ed04c: e7943003     	ldr	r3, [r4, r3]
  3ed050: e5937000     	ldr	r7, [r3]
  3ed054: eb00fe8c     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0x3fa30
  3ed058: eb00fecb     	bl	0x42cb8c <MenuManager::GetHUDRoot()> @ imm = #0x3fb2c
  3ed05c: e2506000     	subs	r6, r0, #0
  3ed060: 0afffff6     	beq	0x3ed040 <MenuMessageManager<StatusMsg, 4>::EnqueueMessage(StatusMsg const&, int, bool) (.clone.18)+0x48> @ imm = #-0x28
  3ed064: e59f50d0     	ldr	r5, [pc, #0xd0]         @ 0x3ed13c <MenuMessageManager<StatusMsg, 4>::EnqueueMessage(StatusMsg const&, int, bool) (.clone.18)+0x144>
  3ed068: e7943005     	ldr	r3, [r4, r5]
  3ed06c: e593202c     	ldr	r2, [r3, #0x2c]
  3ed070: e3520000     	cmp	r2, #0
  3ed074: 0a00000c     	beq	0x3ed0ac <MenuMessageManager<StatusMsg, 4>::EnqueueMessage(StatusMsg const&, int, bool) (.clone.18)+0xb4> @ imm = #0x30
  3ed078: e5930028     	ldr	r0, [r3, #0x28]
  3ed07c: e5d03004     	ldrb	r3, [r0, #0x4]
  3ed080: e3530000     	cmp	r3, #0
  3ed084: 1a00000f     	bne	0x3ed0c8 <MenuMessageManager<StatusMsg, 4>::EnqueueMessage(StatusMsg const&, int, bool) (.clone.18)+0xd0> @ imm = #0x3c
  3ed088: e5901000     	ldr	r1, [r0]
  3ed08c: e2411001     	sub	r1, r1, #1
  3ed090: e3510000     	cmp	r1, #0
  3ed094: e5801000     	str	r1, [r0]
  3ed098: 0a000022     	beq	0x3ed128 <MenuMessageManager<StatusMsg, 4>::EnqueueMessage(StatusMsg const&, int, bool) (.clone.18)+0x130> @ imm = #0x88
  3ed09c: e7943005     	ldr	r3, [r4, r5]
  3ed0a0: e3a02000     	mov	r2, #0
  3ed0a4: e583202c     	str	r2, [r3, #0x2c]
  3ed0a8: e5832028     	str	r2, [r3, #0x28]
  3ed0ac: e59f308c     	ldr	r3, [pc, #0x8c]         @ 0x3ed140 <MenuMessageManager<StatusMsg, 4>::EnqueueMessage(StatusMsg const&, int, bool) (.clone.18)+0x148>
  3ed0b0: e7940005     	ldr	r0, [r4, r5]
  3ed0b4: e1a02006     	mov	r2, r6
  3ed0b8: e7941003     	ldr	r1, [r4, r3]
  3ed0bc: e3a03000     	mov	r3, #0
  3ed0c0: e5911000     	ldr	r1, [r1]
  3ed0c4: eb00eaf5     	bl	0x427ca0 <DebugCachedCharacter::RefreshCache(char const*, MenuFX*, gameswf::character*)> @ imm = #0x3abd4
  3ed0c8: e7940005     	ldr	r0, [r4, r5]
  3ed0cc: eb00eb1f     	bl	0x427d50 <DebugCachedCharacter::GetChar()> @ imm = #0x3ac7c
  3ed0d0: e3a0c000     	mov	r12, #0
  3ed0d4: e5cdc01c     	strb	r12, [sp, #0x1c]
  3ed0d8: e3a02000     	mov	r2, #0
  3ed0dc: e3a03000     	mov	r3, #0
  3ed0e0: e3a0c002     	mov	r12, #2
  3ed0e4: e1cd22f8     	strd	r2, r3, [sp, #40]
  3ed0e8: e5cdc01d     	strb	r12, [sp, #0x1d]
  3ed0ec: e3a0c000     	mov	r12, #0
  3ed0f0: e58dc020     	str	r12, [sp, #0x20]
  3ed0f4: e59dc02c     	ldr	r12, [sp, #0x2c]
  3ed0f8: e28d401c     	add	r4, sp, #28
  3ed0fc: e1a01000     	mov	r1, r0
