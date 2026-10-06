
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f240c <Level::_LoadBatchInit()>:
  3f240c: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3f2410: e59f411c     	ldr	r4, [pc, #0x11c]        @ 0x3f2534 <Level::_LoadBatchInit()+0x128>
  3f2414: e59f611c     	ldr	r6, [pc, #0x11c]        @ 0x3f2538 <Level::_LoadBatchInit()+0x12c>
  3f2418: e59f311c     	ldr	r3, [pc, #0x11c]        @ 0x3f253c <Level::_LoadBatchInit()+0x130>
  3f241c: e08f4004     	add	r4, pc, r4
  3f2420: e7942006     	ldr	r2, [r4, r6]
  3f2424: e7943003     	ldr	r3, [r4, r3]
  3f2428: e24dd020     	sub	sp, sp, #32
  3f242c: e5922000     	ldr	r2, [r2]
  3f2430: e5933010     	ldr	r3, [r3, #0x10]
  3f2434: e3a01000     	mov	r1, #0
  3f2438: e58d201c     	str	r2, [sp, #0x1c]
  3f243c: e593301c     	ldr	r3, [r3, #0x1c]
  3f2440: e3a02000     	mov	r2, #0
  3f2444: e1a08000     	mov	r8, r0
  3f2448: e1a00003     	mov	r0, r3
  3f244c: e5933000     	ldr	r3, [r3]
  3f2450: e1a0e00f     	mov	lr, pc
  3f2454: e593f060     	ldr	pc, [r3, #0x60]
  3f2458: e59f30e0     	ldr	r3, [pc, #0xe0]         @ 0x3f2540 <Level::_LoadBatchInit()+0x134>
  3f245c: e28d5004     	add	r5, sp, #4
  3f2460: e7947003     	ldr	r7, [r4, r3]
  3f2464: e1a00007     	mov	r0, r7
  3f2468: ebfd1506     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xbabe8
  3f246c: e59f10d0     	ldr	r1, [pc, #0xd0]         @ 0x3f2544 <Level::_LoadBatchInit()+0x138>
  3f2470: e1a0200d     	mov	r2, sp
  3f2474: e1a00005     	mov	r0, r5
  3f2478: e08f1001     	add	r1, pc, r1
  3f247c: ebfc871a     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xde398
  3f2480: e1a00007     	mov	r0, r7
  3f2484: e1a01005     	mov	r1, r5
  3f2488: ebfd157e     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbaa08
  3f248c: e1a07000     	mov	r7, r0
  3f2490: e1a00005     	mov	r0, r5
  3f2494: ebfc8544     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xdeaf0
  3f2498: e3570000     	cmp	r7, #0
  3f249c: 0a000006     	beq	0x3f24bc <Level::_LoadBatchInit()+0xb0> @ imm = #0x18
  3f24a0: e7943006     	ldr	r3, [r4, r6]
  3f24a4: e59d201c     	ldr	r2, [sp, #0x1c]
  3f24a8: e5933000     	ldr	r3, [r3]
  3f24ac: e1520003     	cmp	r2, r3
  3f24b0: 1a00001e     	bne	0x3f2530 <Level::_LoadBatchInit()+0x124> @ imm = #0x78
  3f24b4: e28dd020     	add	sp, sp, #32
  3f24b8: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3f24bc: e5985158     	ldr	r5, [r8, #0x158]
  3f24c0: e3550000     	cmp	r5, #0
  3f24c4: 0a000004     	beq	0x3f24dc <Level::_LoadBatchInit()+0xd0> @ imm = #0x10
  3f24c8: e1a00005     	mov	r0, r5
  3f24cc: ebfffd5d     	bl	0x3f1a48 <batch::BatchNodeCompiler::~BatchNodeCompiler()> @ imm = #-0xa8c
  3f24d0: e1a00005     	mov	r0, r5
  3f24d4: ebfc77d9     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xe209c
  3f24d8: e5887158     	str	r7, [r8, #0x158]
  3f24dc: e3a01000     	mov	r1, #0
  3f24e0: e3a00038     	mov	r0, #56
  3f24e4: ebfc7821     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xe1f7c
  3f24e8: e59f1058     	ldr	r1, [pc, #0x58]         @ 0x3f2548 <Level::_LoadBatchInit()+0x13c>
  3f24ec: e3a03000     	mov	r3, #0
  3f24f0: e1a02000     	mov	r2, r0
  3f24f4: e7941001     	ldr	r1, [r4, r1]
  3f24f8: e5c03000     	strb	r3, [r0]
  3f24fc: e5803010     	str	r3, [r0, #0x10]
  3f2500: e2811008     	add	r1, r1, #8
  3f2504: e5801004     	str	r1, [r0, #0x4]
  3f2508: e5803014     	str	r3, [r0, #0x14]
  3f250c: e5803018     	str	r3, [r0, #0x18]
  3f2510: e5803020     	str	r3, [r0, #0x20]
  3f2514: e5e2301c     	strb	r3, [r2, #0x1c]!
  3f2518: e5802028     	str	r2, [r0, #0x28]
  3f251c: e5803034     	str	r3, [r0, #0x34]
  3f2520: e5802024     	str	r2, [r0, #0x24]
  3f2524: e580302c     	str	r3, [r0, #0x2c]
  3f2528: e5880158     	str	r0, [r8, #0x158]
  3f252c: eaffffdb     	b	0x3f24a0 <Level::_LoadBatchInit()+0x94> @ imm = #-0x94
  3f2530: ebfc6f76     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xe4228
  3f2534: 74 26 5a 00  	.word	0x005a2674
  3f2538: ac 40 00 00  	.word	0x000040ac
  3f253c: f4 37 00 00  	.word	0x000037f4
  3f2540: 84 08 00 00  	.word	0x00000884
  3f2544: 80 41 4d 00  	.word	0x004d4180
  3f2548: 7c 13 00 00  	.word	0x0000137c
