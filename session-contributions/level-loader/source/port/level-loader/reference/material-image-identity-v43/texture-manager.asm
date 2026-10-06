
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

005ed210 <glitch::video::CTextureManager::getTexture(char const*, char const*)>:
  5ed210: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  5ed214: e59f5158     	ldr	r5, [pc, #0x158]        @ 0x5ed374 <glitch::video::CTextureManager::getTexture(char const*, char const*)+0x164>
  5ed218: e59f8158     	ldr	r8, [pc, #0x158]        @ 0x5ed378 <glitch::video::CTextureManager::getTexture(char const*, char const*)+0x168>
  5ed21c: e1a04000     	mov	r4, r0
  5ed220: e08f5005     	add	r5, pc, r5
  5ed224: e7950008     	ldr	r0, [r5, r8]
  5ed228: e24dd034     	sub	sp, sp, #52
  5ed22c: e3a0c000     	mov	r12, #0
  5ed230: e5900000     	ldr	r0, [r0]
  5ed234: e3530000     	cmp	r3, #0
  5ed238: e584c000     	str	r12, [r4]
  5ed23c: e1a0a001     	mov	r10, r1
  5ed240: e58d002c     	str	r0, [sp, #0x2c]
  5ed244: e1a07002     	mov	r7, r2
  5ed248: 0a000027     	beq	0x5ed2ec <glitch::video::CTextureManager::getTexture(char const*, char const*)+0xdc> @ imm = #0x9c
  5ed24c: e28d6014     	add	r6, sp, #20
  5ed250: e1a01003     	mov	r1, r3
  5ed254: e1a00006     	mov	r0, r6
  5ed258: e28d2010     	add	r2, sp, #16
  5ed25c: ebf4e376     	bl	0x32603c <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::basic_string(char const*, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0> const&)> @ imm = #-0x2c7228
  5ed260: e59d2028     	ldr	r2, [sp, #0x28]
  5ed264: e28d000c     	add	r0, sp, #12
  5ed268: e1a0100a     	mov	r1, r10
  5ed26c: ebffef2e     	bl	0x5e8f2c <glitch::video::CTextureManager::findTexture(char const*) const> @ imm = #-0x4348
  5ed270: e59d300c     	ldr	r3, [sp, #0xc]
  5ed274: e3530000     	cmp	r3, #0
  5ed278: 15932004     	ldrne	r2, [r3, #0x4]
  5ed27c: 12822001     	addne	r2, r2, #1
  5ed280: 15832004     	strne	r2, [r3, #0x4]
  5ed284: e5940000     	ldr	r0, [r4]
  5ed288: e5843000     	str	r3, [r4]
  5ed28c: e3500000     	cmp	r0, #0
  5ed290: 0a000000     	beq	0x5ed298 <glitch::video::CTextureManager::getTexture(char const*, char const*)+0x88> @ imm = #0x0
  5ed294: ebf4c0ba     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x2cfd18
  5ed298: e59d000c     	ldr	r0, [sp, #0xc]
  5ed29c: e3500000     	cmp	r0, #0
  5ed2a0: 0a000000     	beq	0x5ed2a8 <glitch::video::CTextureManager::getTexture(char const*, char const*)+0x98> @ imm = #0x0
  5ed2a4: ebf4c0b6     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x2cfd28
  5ed2a8: e5949000     	ldr	r9, [r4]
  5ed2ac: e3590000     	cmp	r9, #0
  5ed2b0: 0a000011     	beq	0x5ed2fc <glitch::video::CTextureManager::getTexture(char const*, char const*)+0xec> @ imm = #0x44
  5ed2b4: e59d0028     	ldr	r0, [sp, #0x28]
  5ed2b8: e1500006     	cmp	r0, r6
  5ed2bc: 0a000002     	beq	0x5ed2cc <glitch::video::CTextureManager::getTexture(char const*, char const*)+0xbc> @ imm = #0x8
  5ed2c0: e3500000     	cmp	r0, #0
  5ed2c4: 0a000000     	beq	0x5ed2cc <glitch::video::CTextureManager::getTexture(char const*, char const*)+0xbc> @ imm = #0x0
  5ed2c8: ebf48c60     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x2dce80
  5ed2cc: e7953008     	ldr	r3, [r5, r8]
  5ed2d0: e59d202c     	ldr	r2, [sp, #0x2c]
  5ed2d4: e1a00004     	mov	r0, r4
  5ed2d8: e5933000     	ldr	r3, [r3]
  5ed2dc: e1520003     	cmp	r2, r3
  5ed2e0: 1a000022     	bne	0x5ed370 <glitch::video::CTextureManager::getTexture(char const*, char const*)+0x160> @ imm = #0x88
  5ed2e4: e28dd034     	add	sp, sp, #52
  5ed2e8: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  5ed2ec: e28d6014     	add	r6, sp, #20
  5ed2f0: e1a00006     	mov	r0, r6
  5ed2f4: ebfff183     	bl	0x5e9908 <glitch::video::CTextureManager::getHashName(char const*) const> @ imm = #-0x39f4
  5ed2f8: eaffffd8     	b	0x5ed260 <glitch::video::CTextureManager::getTexture(char const*, char const*)+0x50> @ imm = #-0xa0
  5ed2fc: e59a302c     	ldr	r3, [r10, #0x2c]
  5ed300: e1a01007     	mov	r1, r7
  5ed304: e1a00003     	mov	r0, r3
  5ed308: e5933000     	ldr	r3, [r3]
  5ed30c: e1a0e00f     	mov	lr, pc
  5ed310: e593f00c     	ldr	pc, [r3, #0xc]
  5ed314: e250b000     	subs	r11, r0, #0
  5ed318: 0a00000e     	beq	0x5ed358 <glitch::video::CTextureManager::getTexture(char const*, char const*)+0x148> @ imm = #0x38
  5ed31c: e28d7008     	add	r7, sp, #8
  5ed320: e1a0200b     	mov	r2, r11
  5ed324: e1a03006     	mov	r3, r6
  5ed328: e1a0100a     	mov	r1, r10
  5ed32c: e1a00007     	mov	r0, r7
  5ed330: e58d9000     	str	r9, [sp]
  5ed334: ebfffefe     	bl	0x5ecf34 <glitch::video::CTextureManager::getTextureInternal(glitch::io::IReadFile*, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, bool)> @ imm = #-0x408
  5ed338: e1a01007     	mov	r1, r7
  5ed33c: e1a00004     	mov	r0, r4
  5ed340: ebf65eac     	bl	0x384df8 <boost::intrusive_ptr<glitch::video::ITexture>::operator=(boost::intrusive_ptr<glitch::video::ITexture> const&)> @ imm = #-0x268550
  5ed344: e1a00007     	mov	r0, r7
  5ed348: ebf8a837     	bl	0x41742c <boost::intrusive_ptr<glitch::video::ITexture>::~intrusive_ptr()> @ imm = #-0x1d5f24
  5ed34c: e1a0000b     	mov	r0, r11
  5ed350: ebf4c08b     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x2cfdd4
  5ed354: eaffffd6     	b	0x5ed2b4 <glitch::video::CTextureManager::getTexture(char const*, char const*)+0xa4> @ imm = #-0xa8
  5ed358: e59f001c     	ldr	r0, [pc, #0x1c]         @ 0x5ed37c <glitch::video::CTextureManager::getTexture(char const*, char const*)+0x16c>
  5ed35c: e1a01007     	mov	r1, r7
  5ed360: e3a02003     	mov	r2, #3
  5ed364: e08f0000     	add	r0, pc, r0
  5ed368: eb00765e     	bl	0x60ace8 <glitch::os::Printer::log(char const*, char const*, glitch::ELOG_LEVEL)> @ imm = #0x1d978
  5ed36c: eaffffd0     	b	0x5ed2b4 <glitch::video::CTextureManager::getTexture(char const*, char const*)+0xa4> @ imm = #-0xc0
  5ed370: ebf483e6     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x2df068
  5ed374: 70 78 3a 00  	.word	0x003a7870
  5ed378: ac 40 00 00  	.word	0x000040ac
  5ed37c: 94 62 2f 00  	.word	0x002f6294
