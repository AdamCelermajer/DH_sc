
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0045b264 <ScriptManager::LoadScriptFile(char const*, bool)>:
  45b264: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  45b268: e59f3398     	ldr	r3, [pc, #0x398]        @ 0x45b608 <ScriptManager::LoadScriptFile(char const*, bool)+0x3a4>
  45b26c: e2526000     	subs	r6, r2, #0
  45b270: e24dd07c     	sub	sp, sp, #124
  45b274: e1a04000     	mov	r4, r0
  45b278: e08f3003     	add	r3, pc, r3
  45b27c: 0a00000b     	beq	0x45b2b0 <ScriptManager::LoadScriptFile(char const*, bool)+0x4c> @ imm = #0x2c
  45b280: e590001c     	ldr	r0, [r0, #0x1c]
  45b284: e5942018     	ldr	r2, [r4, #0x18]
  45b288: e594c008     	ldr	r12, [r4, #0x8]
  45b28c: e0622000     	rsb	r2, r2, r0
  45b290: e1a02142     	asr	r2, r2, #2
  45b294: e0820102     	add	r0, r2, r2, lsl #2
  45b298: e0800200     	add	r0, r0, r0, lsl #4
  45b29c: e0800400     	add	r0, r0, r0, lsl #8
  45b2a0: e0800800     	add	r0, r0, r0, lsl #16
  45b2a4: e0822080     	add	r2, r2, r0, lsl #1
  45b2a8: e15c0002     	cmp	r12, r2
  45b2ac: ba0000c2     	blt	0x45b5bc <ScriptManager::LoadScriptFile(char const*, bool)+0x358> @ imm = #0x308
  45b2b0: e59f0354     	ldr	r0, [pc, #0x354]        @ 0x45b60c <ScriptManager::LoadScriptFile(char const*, bool)+0x3a8>
  45b2b4: e3a02000     	mov	r2, #0
  45b2b8: e7935000     	ldr	r5, [r3, r0]
  45b2bc: e1a03002     	mov	r3, r2
  45b2c0: e5950010     	ldr	r0, [r5, #0x10]
  45b2c4: e590c034     	ldr	r12, [r0, #0x34]
  45b2c8: e1a0000c     	mov	r0, r12
  45b2cc: e59cc000     	ldr	r12, [r12]
  45b2d0: e1a0e00f     	mov	lr, pc
  45b2d4: e59cf088     	ldr	pc, [r12, #0x88]
  45b2d8: e3500000     	cmp	r0, #0
  45b2dc: e58d0074     	str	r0, [sp, #0x74]
  45b2e0: 0a0000b5     	beq	0x45b5bc <ScriptManager::LoadScriptFile(char const*, bool)+0x358> @ imm = #0x2d4
  45b2e4: e28d7018     	add	r7, sp, #24
  45b2e8: e1a01000     	mov	r1, r0
  45b2ec: e1a00007     	mov	r0, r7
  45b2f0: ebfaeff8     	bl	0x3172d8 <StreamBuffer::StreamBuffer(IStreamBase*)> @ imm = #-0x144020
  45b2f4: e5953010     	ldr	r3, [r5, #0x10]
  45b2f8: e28d1074     	add	r1, sp, #116
  45b2fc: e28d5070     	add	r5, sp, #112
  45b300: e5933034     	ldr	r3, [r3, #0x34]
  45b304: e1a00003     	mov	r0, r3
  45b308: e5933000     	ldr	r3, [r3]
  45b30c: e1a0e00f     	mov	lr, pc
  45b310: e593f078     	ldr	pc, [r3, #0x78]
  45b314: e1a00007     	mov	r0, r7
  45b318: e1a01005     	mov	r1, r5
  45b31c: ebfff75b     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0x2294
  45b320: e3a03001     	mov	r3, #1
  45b324: e3530000     	cmp	r3, #0
  45b328: e58d3064     	str	r3, [sp, #0x64]
  45b32c: 0a0000a4     	beq	0x45b5c4 <ScriptManager::LoadScriptFile(char const*, bool)+0x360> @ imm = #0x290
  45b330: e594201c     	ldr	r2, [r4, #0x1c]
  45b334: e5943018     	ldr	r3, [r4, #0x18]
  45b338: e3560000     	cmp	r6, #0
  45b33c: e3a05000     	mov	r5, #0
  45b340: e0633002     	rsb	r3, r3, r2
  45b344: e1a03143     	asr	r3, r3, #2
  45b348: e2840018     	add	r0, r4, #24
  45b34c: e0832103     	add	r2, r3, r3, lsl #2
  45b350: e0822202     	add	r2, r2, r2, lsl #4
  45b354: e0822402     	add	r2, r2, r2, lsl #8
  45b358: e0822802     	add	r2, r2, r2, lsl #16
  45b35c: e0832082     	add	r2, r3, r2, lsl #1
  45b360: e58d200c     	str	r2, [sp, #0xc]
  45b364: 15942008     	ldrne	r2, [r4, #0x8]
  45b368: 159d3070     	ldrne	r3, [sp, #0x70]
  45b36c: 059d3070     	ldreq	r3, [sp, #0x70]
  45b370: 10822003     	addne	r2, r2, r3
  45b374: 15842008     	strne	r2, [r4, #0x8]
  45b378: e59d100c     	ldr	r1, [sp, #0xc]
  45b37c: e28d2058     	add	r2, sp, #88
  45b380: e58d5058     	str	r5, [sp, #0x58]
  45b384: e0813003     	add	r3, r1, r3
  45b388: e1a01003     	mov	r1, r3
  45b38c: e58d3014     	str	r3, [sp, #0x14]
  45b390: e5cd505c     	strb	r5, [sp, #0x5c]
  45b394: e58d5060     	str	r5, [sp, #0x60]
  45b398: ebffff9a     	bl	0x45b208 <std::vector<ScriptManager::ScriptCmds, std::allocator<ScriptManager::ScriptCmds>>::resize(unsigned int, ScriptManager::ScriptCmds const&)> @ imm = #-0x198
  45b39c: e28d204c     	add	r2, sp, #76
  45b3a0: e284000c     	add	r0, r4, #12
  45b3a4: e59d1014     	ldr	r1, [sp, #0x14]
  45b3a8: e58d5054     	str	r5, [sp, #0x54]
  45b3ac: e58d504c     	str	r5, [sp, #0x4c]
  45b3b0: e58d5050     	str	r5, [sp, #0x50]
  45b3b4: ebfffdf4     	bl	0x45ab8c <std::vector<ScriptManager::ScriptContext, std::allocator<ScriptManager::ScriptContext>>::resize(unsigned int, ScriptManager::ScriptContext const&)> @ imm = #-0x830
  45b3b8: e59d2014     	ldr	r2, [sp, #0x14]
  45b3bc: e59d300c     	ldr	r3, [sp, #0xc]
  45b3c0: e1520003     	cmp	r2, r3
  45b3c4: da00007a     	ble	0x45b5b4 <ScriptManager::LoadScriptFile(char const*, bool)+0x350> @ imm = #0x1e8
  45b3c8: e59f8240     	ldr	r8, [pc, #0x240]        @ 0x45b610 <ScriptManager::LoadScriptFile(char const*, bool)+0x3ac>
  45b3cc: e28db068     	add	r11, sp, #104
  45b3d0: e3a0500c     	mov	r5, #12
  45b3d4: e3a06001     	mov	r6, #1
  45b3d8: e0050395     	mul	r5, r5, r3
  45b3dc: e28d106c     	add	r1, sp, #108
  45b3e0: e08b2006     	add	r2, r11, r6
  45b3e4: e28b3002     	add	r3, r11, #2
  45b3e8: e08f8008     	add	r8, pc, r8
  45b3ec: e58d1010     	str	r1, [sp, #0x10]
  45b3f0: e98d000c     	stmib	sp, {r2, r3}
  45b3f4: e1a00007     	mov	r0, r7
  45b3f8: e59d1010     	ldr	r1, [sp, #0x10]
  45b3fc: ebfff723     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0x2374
  45b400: e3560000     	cmp	r6, #0
  45b404: e58d6064     	str	r6, [sp, #0x64]
  45b408: 1a000010     	bne	0x45b450 <ScriptManager::LoadScriptFile(char const*, bool)+0x1ec> @ imm = #0x40
  45b40c: e59d1010     	ldr	r1, [sp, #0x10]
  45b410: e2813001     	add	r3, r1, #1
  45b414: e2812002     	add	r2, r1, #2
  45b418: e5d20001     	ldrb	r0, [r2, #0x1]
  45b41c: e5531001     	ldrb	r1, [r3, #-0x1]
  45b420: e1530002     	cmp	r3, r2
  45b424: e0201001     	eor	r1, r0, r1
  45b428: e5431001     	strb	r1, [r3, #-0x1]
  45b42c: e5d20001     	ldrb	r0, [r2, #0x1]
  45b430: e0211000     	eor	r1, r1, r0
  45b434: e5c21001     	strb	r1, [r2, #0x1]
  45b438: e5530001     	ldrb	r0, [r3, #-0x1]
  45b43c: e2422001     	sub	r2, r2, #1
  45b440: e0211000     	eor	r1, r1, r0
  45b444: e5431001     	strb	r1, [r3, #-0x1]
  45b448: e2833001     	add	r3, r3, #1
  45b44c: 3afffff1     	blo	0x45b418 <ScriptManager::LoadScriptFile(char const*, bool)+0x1b4> @ imm = #-0x3c
  45b450: e5943018     	ldr	r3, [r4, #0x18]
  45b454: e59d206c     	ldr	r2, [sp, #0x6c]
  45b458: e3a01000     	mov	r1, #0
  45b45c: e7832005     	str	r2, [r3, r5]
  45b460: e59d006c     	ldr	r0, [sp, #0x6c]
  45b464: e594a018     	ldr	r10, [r4, #0x18]
  45b468: e1a00100     	lsl	r0, r0, #2
  45b46c: ebfad43e     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x14af08
  45b470: e08aa005     	add	r10, r10, r5
  45b474: e58a0008     	str	r0, [r10, #0x8]
  45b478: e59d306c     	ldr	r3, [sp, #0x6c]
  45b47c: e3530000     	cmp	r3, #0
  45b480: da000040     	ble	0x45b588 <ScriptManager::LoadScriptFile(char const*, bool)+0x324> @ imm = #0x100
  45b484: e3a0a000     	mov	r10, #0
  45b488: e1a00007     	mov	r0, r7
  45b48c: e1a0100b     	mov	r1, r11
  45b490: ebfff72a     	bl	0x459140 <void StreamReader::peekAs<int>(IStreamBase*, int*)> @ imm = #-0x2358
  45b494: e3560000     	cmp	r6, #0
  45b498: e58d6064     	str	r6, [sp, #0x64]
  45b49c: 1a00000f     	bne	0x45b4e0 <ScriptManager::LoadScriptFile(char const*, bool)+0x27c> @ imm = #0x3c
  45b4a0: e59d3004     	ldr	r3, [sp, #0x4]
  45b4a4: e59d2008     	ldr	r2, [sp, #0x8]
  45b4a8: e5d20001     	ldrb	r0, [r2, #0x1]
  45b4ac: e5531001     	ldrb	r1, [r3, #-0x1]
  45b4b0: e1530002     	cmp	r3, r2
  45b4b4: e0201001     	eor	r1, r0, r1
  45b4b8: e5431001     	strb	r1, [r3, #-0x1]
  45b4bc: e5d20001     	ldrb	r0, [r2, #0x1]
  45b4c0: e0211000     	eor	r1, r1, r0
  45b4c4: e5c21001     	strb	r1, [r2, #0x1]
  45b4c8: e5530001     	ldrb	r0, [r3, #-0x1]
  45b4cc: e2422001     	sub	r2, r2, #1
  45b4d0: e0211000     	eor	r1, r1, r0
  45b4d4: e5431001     	strb	r1, [r3, #-0x1]
  45b4d8: e2833001     	add	r3, r3, #1
  45b4dc: 3afffff1     	blo	0x45b4a8 <ScriptManager::LoadScriptFile(char const*, bool)+0x244> @ imm = #-0x3c
  45b4e0: e5943018     	ldr	r3, [r4, #0x18]
  45b4e4: e0833005     	add	r3, r3, r5
  45b4e8: e5939008     	ldr	r9, [r3, #0x8]
  45b4ec: e59d3068     	ldr	r3, [sp, #0x68]
  45b4f0: e1a0e00f     	mov	lr, pc
  45b4f4: e798f103     	ldr	pc, [r8, r3, lsl #2]
  45b4f8: e789010a     	str	r0, [r9, r10, lsl #2]
  45b4fc: e59d3068     	ldr	r3, [sp, #0x68]
  45b500: e0883103     	add	r3, r8, r3, lsl #2
  45b504: e1a0e00f     	mov	lr, pc
  45b508: e593f140     	ldr	pc, [r3, #0x140]
  45b50c: e1a01007     	mov	r1, r7
  45b510: e5903000     	ldr	r3, [r0]
  45b514: e1a09000     	mov	r9, r0
  45b518: e1a0e00f     	mov	lr, pc
  45b51c: e593f00c     	ldr	pc, [r3, #0xc]
  45b520: e5943018     	ldr	r3, [r4, #0x18]
  45b524: e59d2068     	ldr	r2, [sp, #0x68]
  45b528: e0833005     	add	r3, r3, r5
  45b52c: e5933008     	ldr	r3, [r3, #0x8]
  45b530: e793310a     	ldr	r3, [r3, r10, lsl #2]
  45b534: e5832008     	str	r2, [r3, #0x8]
  45b538: e583900c     	str	r9, [r3, #0xc]
  45b53c: e5943018     	ldr	r3, [r4, #0x18]
  45b540: e0833005     	add	r3, r3, r5
  45b544: e5933008     	ldr	r3, [r3, #0x8]
  45b548: e793310a     	ldr	r3, [r3, r10, lsl #2]
  45b54c: e1a00003     	mov	r0, r3
  45b550: e5933000     	ldr	r3, [r3]
  45b554: e1a0e00f     	mov	lr, pc
  45b558: e593f000     	ldr	pc, [r3]
  45b55c: e5943018     	ldr	r3, [r4, #0x18]
  45b560: e0833005     	add	r3, r3, r5
  45b564: e5932008     	ldr	r2, [r3, #0x8]
  45b568: e792210a     	ldr	r2, [r2, r10, lsl #2]
  45b56c: e28aa001     	add	r10, r10, #1
  45b570: e5d22004     	ldrb	r2, [r2, #0x4]
  45b574: e3520000     	cmp	r2, #0
  45b578: 15c36004     	strbne	r6, [r3, #0x4]
  45b57c: e59d306c     	ldr	r3, [sp, #0x6c]
  45b580: e153000a     	cmp	r3, r10
  45b584: caffffbf     	bgt	0x45b488 <ScriptManager::LoadScriptFile(char const*, bool)+0x224> @ imm = #-0x104
  45b588: e59d100c     	ldr	r1, [sp, #0xc]
  45b58c: e1a00004     	mov	r0, r4
  45b590: e285500c     	add	r5, r5, #12
  45b594: e2812001     	add	r2, r1, #1
  45b598: e58d200c     	str	r2, [sp, #0xc]
  45b59c: e3a02000     	mov	r2, #0
  45b5a0: ebffe984     	bl	0x455bb8 <ScriptManager::StopScript(int, bool)> @ imm = #-0x59f0
  45b5a4: e59d300c     	ldr	r3, [sp, #0xc]
  45b5a8: e59d1014     	ldr	r1, [sp, #0x14]
  45b5ac: e1530001     	cmp	r3, r1
  45b5b0: 1affff8f     	bne	0x45b3f4 <ScriptManager::LoadScriptFile(char const*, bool)+0x190> @ imm = #-0x1c4
  45b5b4: e1a00007     	mov	r0, r7
  45b5b8: ebfaed01     	bl	0x3169c4 <StreamBuffer::~StreamBuffer()> @ imm = #-0x144bfc
  45b5bc: e28dd07c     	add	sp, sp, #124
  45b5c0: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  45b5c4: e2853002     	add	r3, r5, #2
  45b5c8: e2855001     	add	r5, r5, #1
  45b5cc: e5d31001     	ldrb	r1, [r3, #0x1]
  45b5d0: e5552001     	ldrb	r2, [r5, #-0x1]
  45b5d4: e1550003     	cmp	r5, r3
  45b5d8: e0212002     	eor	r2, r1, r2
  45b5dc: e5452001     	strb	r2, [r5, #-0x1]
  45b5e0: e5d31001     	ldrb	r1, [r3, #0x1]
  45b5e4: e0222001     	eor	r2, r2, r1
  45b5e8: e5c32001     	strb	r2, [r3, #0x1]
  45b5ec: e5551001     	ldrb	r1, [r5, #-0x1]
  45b5f0: e2433001     	sub	r3, r3, #1
  45b5f4: e0222001     	eor	r2, r2, r1
  45b5f8: e5452001     	strb	r2, [r5, #-0x1]
  45b5fc: e2855001     	add	r5, r5, #1
  45b600: 3afffff1     	blo	0x45b5cc <ScriptManager::LoadScriptFile(char const*, bool)+0x368> @ imm = #-0x3c
  45b604: eaffff49     	b	0x45b330 <ScriptManager::LoadScriptFile(char const*, bool)+0xcc> @ imm = #-0x2dc
  45b608: 18 98 53 00  	.word	0x00539818
  45b60c: f4 37 00 00  	.word	0x000037f4
  45b610: 10 d8 50 00  	.word	0x0050d810

0045b614 <std::allocator<TutorialMsg>::allocate(unsigned int, void const*) (.clone.10)>:
  45b614: e52de004     	str	lr, [sp, #-0x4]!
  45b618: e24dd00c     	sub	sp, sp, #12
  45b61c: e28d0008     	add	r0, sp, #8
  45b620: e3a03080     	mov	r3, #128
  45b624: e5203004     	str	r3, [r0, #-0x4]!
  45b628: eb0ab624     	bl	0x708ec0 <___ZNSt12__node_alloc11_M_allocateERj_veneer> @ imm = #0x2ad890
  45b62c: e28dd00c     	add	sp, sp, #12
