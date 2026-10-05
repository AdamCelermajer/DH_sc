
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00488904 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)>:
  488904: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  488908: e59f0264     	ldr	r0, [pc, #0x264]        @ 0x488b74 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x270>
  48890c: e59f3264     	ldr	r3, [pc, #0x264]        @ 0x488b78 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x274>
  488910: e24dd074     	sub	sp, sp, #116
  488914: e08f0000     	add	r0, pc, r0
  488918: e58d0008     	str	r0, [sp, #0x8]
  48891c: e58d300c     	str	r3, [sp, #0xc]
  488920: e59dc008     	ldr	r12, [sp, #0x8]
  488924: e7903003     	ldr	r3, [r0, r3]
  488928: e59f024c     	ldr	r0, [pc, #0x24c]        @ 0x488b7c <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x278>
  48892c: e1a08002     	mov	r8, r2
  488930: e3a02000     	mov	r2, #0
  488934: e79c6000     	ldr	r6, [r12, r0]
  488938: e5930000     	ldr	r0, [r3]
  48893c: e28d4054     	add	r4, sp, #84
  488940: e5963010     	ldr	r3, [r6, #0x10]
  488944: e58d006c     	str	r0, [sp, #0x6c]
  488948: e593c034     	ldr	r12, [r3, #0x34]
  48894c: e1a03002     	mov	r3, r2
  488950: e1a0000c     	mov	r0, r12
  488954: e59cc000     	ldr	r12, [r12]
  488958: e1a0e00f     	mov	lr, pc
  48895c: e59cf088     	ldr	pc, [r12, #0x88]
  488960: e58d0014     	str	r0, [sp, #0x14]
  488964: e5903000     	ldr	r3, [r0]
  488968: e1a0e00f     	mov	lr, pc
  48896c: e593f008     	ldr	pc, [r3, #0x8]
  488970: e3a01000     	mov	r1, #0
  488974: e2800001     	add	r0, r0, #1
  488978: ebfa1efb     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x178414
  48897c: e59da014     	ldr	r10, [sp, #0x14]
  488980: e1a05000     	mov	r5, r0
  488984: e59a3000     	ldr	r3, [r10]
  488988: e1a0000a     	mov	r0, r10
  48898c: e5937018     	ldr	r7, [r3, #0x18]
  488990: e1a0e00f     	mov	lr, pc
  488994: e593f008     	ldr	pc, [r3, #0x8]
  488998: e1a02000     	mov	r2, r0
  48899c: e1a03001     	mov	r3, r1
  4889a0: e1a0000a     	mov	r0, r10
  4889a4: e1a01005     	mov	r1, r5
  4889a8: e12fff37     	blx	r7
  4889ac: e28d7070     	add	r7, sp, #112
  4889b0: e537305c     	ldr	r3, [r7, #-0x5c]!
  4889b4: e1a00003     	mov	r0, r3
  4889b8: e5933000     	ldr	r3, [r3]
  4889bc: e1a0e00f     	mov	lr, pc
  4889c0: e593f008     	ldr	pc, [r3, #0x8]
  4889c4: e3a03000     	mov	r3, #0
  4889c8: e7c53000     	strb	r3, [r5, r0]
  4889cc: e5963010     	ldr	r3, [r6, #0x10]
  4889d0: e1a01007     	mov	r1, r7
  4889d4: e5933034     	ldr	r3, [r3, #0x34]
  4889d8: e1a00003     	mov	r0, r3
  4889dc: e5933000     	ldr	r3, [r3]
  4889e0: e1a0e00f     	mov	lr, pc
  4889e4: e593f078     	ldr	pc, [r3, #0x78]
  4889e8: e1a00004     	mov	r0, r4
  4889ec: e1a01005     	mov	r1, r5
  4889f0: e28d2020     	add	r2, sp, #32
  4889f4: ebfa2dbc     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x174910
  4889f8: e3550000     	cmp	r5, #0
  4889fc: 0a000001     	beq	0x488a08 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x104> @ imm = #0x4
  488a00: e1a00005     	mov	r0, r5
  488a04: ebfa1e8d     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1785cc
  488a08: e59fa170     	ldr	r10, [pc, #0x170]       @ 0x488b80 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x27c>
  488a0c: e1a00004     	mov	r0, r4
  488a10: e08fa00a     	add	r10, pc, r10
  488a14: e1a0100a     	mov	r1, r10
  488a18: ebffef42     	bl	0x484728 <std::string::find(char const*, unsigned int) const (.clone.1)> @ imm = #-0x42f8
  488a1c: e3700001     	cmn	r0, #1
  488a20: e1a05000     	mov	r5, r0
  488a24: 0a00003c     	beq	0x488b1c <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x218> @ imm = #0xf0
  488a28: e28d603c     	add	r6, sp, #60
  488a2c: e28d901c     	add	r9, sp, #28
  488a30: e28d7024     	add	r7, sp, #36
  488a34: e28db018     	add	r11, sp, #24
  488a38: ea00001c     	b	0x488ab0 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x1ac> @ imm = #0x70
  488a3c: eb0a012f     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x2804bc
  488a40: e59d0064     	ldr	r0, [sp, #0x64]
  488a44: e59d3068     	ldr	r3, [sp, #0x68]
  488a48: e2852001     	add	r2, r5, #1
  488a4c: e1a01004     	mov	r1, r4
  488a50: e0633000     	rsb	r3, r3, r0
  488a54: e1a00007     	mov	r0, r7
  488a58: e58db000     	str	r11, [sp]
  488a5c: ebfe349d     	bl	0x415cd8 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&, unsigned int, unsigned int, std::allocator<char> const&)> @ imm = #-0x72d8c
  488a60: e1a00004     	mov	r0, r4
  488a64: e59d1038     	ldr	r1, [sp, #0x38]
  488a68: e59d2034     	ldr	r2, [sp, #0x34]
  488a6c: ebfa1fdb     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x178094
  488a70: e59d0038     	ldr	r0, [sp, #0x38]
  488a74: e1500007     	cmp	r0, r7
  488a78: 0a000006     	beq	0x488a98 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x194> @ imm = #0x18
  488a7c: e3500000     	cmp	r0, #0
  488a80: 0a000004     	beq	0x488a98 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x194> @ imm = #0x10
  488a84: e59d1024     	ldr	r1, [sp, #0x24]
  488a88: e0601001     	rsb	r1, r0, r1
  488a8c: e3510080     	cmp	r1, #128
  488a90: 8a00001a     	bhi	0x488b00 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x1fc> @ imm = #0x68
  488a94: eb0a0119     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x280464
  488a98: e1a00004     	mov	r0, r4
  488a9c: e1a0100a     	mov	r1, r10
  488aa0: ebffef20     	bl	0x484728 <std::string::find(char const*, unsigned int) const (.clone.1)> @ imm = #-0x4380
  488aa4: e3700001     	cmn	r0, #1
  488aa8: e1a05000     	mov	r5, r0
  488aac: 0a00001a     	beq	0x488b1c <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x218> @ imm = #0x68
  488ab0: e3a02000     	mov	r2, #0
  488ab4: e2453001     	sub	r3, r5, #1
  488ab8: e1a01004     	mov	r1, r4
  488abc: e1a00006     	mov	r0, r6
  488ac0: e58d9000     	str	r9, [sp]
  488ac4: ebfe3483     	bl	0x415cd8 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&, unsigned int, unsigned int, std::allocator<char> const&)> @ imm = #-0x72df4
  488ac8: e1a00008     	mov	r0, r8
  488acc: e1a01006     	mov	r1, r6
  488ad0: ebfa8c00     	bl	0x32bad8 <std::vector<std::string, std::allocator<std::string>>::push_back(std::string const&)> @ imm = #-0x15d000
  488ad4: e59d0050     	ldr	r0, [sp, #0x50]
  488ad8: e1500006     	cmp	r0, r6
  488adc: 0affffd7     	beq	0x488a40 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x13c> @ imm = #-0xa4
  488ae0: e3500000     	cmp	r0, #0
  488ae4: 0affffd5     	beq	0x488a40 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x13c> @ imm = #-0xac
  488ae8: e59d103c     	ldr	r1, [sp, #0x3c]
  488aec: e0601001     	rsb	r1, r0, r1
  488af0: e3510080     	cmp	r1, #128
  488af4: 9affffd0     	bls	0x488a3c <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x138> @ imm = #-0xc0
  488af8: ebfa1e50     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1786c0
  488afc: eaffffcf     	b	0x488a40 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x13c> @ imm = #-0xc4
  488b00: ebfa1e4e     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1786c8
  488b04: e1a00004     	mov	r0, r4
  488b08: e1a0100a     	mov	r1, r10
  488b0c: ebffef05     	bl	0x484728 <std::string::find(char const*, unsigned int) const (.clone.1)> @ imm = #-0x43ec
  488b10: e3700001     	cmn	r0, #1
  488b14: e1a05000     	mov	r5, r0
  488b18: 1affffe4     	bne	0x488ab0 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x1ac> @ imm = #-0x70
  488b1c: e59d0068     	ldr	r0, [sp, #0x68]
  488b20: e1500004     	cmp	r0, r4
  488b24: 0a000006     	beq	0x488b44 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x240> @ imm = #0x18
  488b28: e3500000     	cmp	r0, #0
  488b2c: 0a000004     	beq	0x488b44 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x240> @ imm = #0x10
  488b30: e59d1054     	ldr	r1, [sp, #0x54]
  488b34: e0601001     	rsb	r1, r0, r1
  488b38: e3510080     	cmp	r1, #128
  488b3c: 8a000009     	bhi	0x488b68 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x264> @ imm = #0x24
  488b40: eb0a00ee     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x2803b8
  488b44: e59d1008     	ldr	r1, [sp, #0x8]
  488b48: e59d000c     	ldr	r0, [sp, #0xc]
  488b4c: e59d206c     	ldr	r2, [sp, #0x6c]
  488b50: e7913000     	ldr	r3, [r1, r0]
  488b54: e5933000     	ldr	r3, [r3]
  488b58: e1520003     	cmp	r2, r3
  488b5c: 1a000003     	bne	0x488b70 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x26c> @ imm = #0xc
  488b60: e28dd074     	add	sp, sp, #116
  488b64: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  488b68: ebfa1e34     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x178730
  488b6c: eafffff4     	b	0x488b44 <rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)+0x240> @ imm = #-0x30
  488b70: ebfa15e6     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x17a868
  488b74: 7c c1 50 00  	.word	0x0050c17c
  488b78: ac 40 00 00  	.word	0x000040ac
  488b7c: f4 37 00 00  	.word	0x000037f4
  488b80: e0 2f 44 00  	.word	0x00442fe0
