
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

005164a8 <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)>:
  5164a8: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  5164ac: e59f5178     	ldr	r5, [pc, #0x178]        @ 0x51662c <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)+0x184>
  5164b0: e59f9178     	ldr	r9, [pc, #0x178]        @ 0x516630 <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)+0x188>
  5164b4: e1a08002     	mov	r8, r2
  5164b8: e08f5005     	add	r5, pc, r5
  5164bc: e7952009     	ldr	r2, [r5, r9]
  5164c0: e1a0b003     	mov	r11, r3
  5164c4: e24dd02c     	sub	sp, sp, #44
  5164c8: e5923000     	ldr	r3, [r2]
  5164cc: e1a06000     	mov	r6, r0
  5164d0: e1a0a001     	mov	r10, r1
  5164d4: e58d3024     	str	r3, [sp, #0x24]
  5164d8: ebfff77a     	bl	0x5142c8 <TiXmlNode::Clear()> @ imm = #-0x2218
  5164dc: e3e03000     	mvn	r3, #0
  5164e0: e3580000     	cmp	r8, #0
  5164e4: e5863004     	str	r3, [r6, #0x4]
  5164e8: e5863008     	str	r3, [r6, #0x8]
  5164ec: da000045     	ble	0x516608 <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)+0x160> @ imm = #0x114
  5164f0: e28d400c     	add	r4, sp, #12
  5164f4: e1a00004     	mov	r0, r4
  5164f8: e3a01010     	mov	r1, #16
  5164fc: e58d401c     	str	r4, [sp, #0x1c]
  516500: e58d4020     	str	r4, [sp, #0x20]
  516504: ebf7ec5c     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x204e90
  516508: e59d301c     	ldr	r3, [sp, #0x1c]
  51650c: e3a02000     	mov	r2, #0
  516510: e1a01008     	mov	r1, r8
  516514: e5c32000     	strb	r2, [r3]
  516518: e1a00004     	mov	r0, r4
  51651c: ebf85013     	bl	0x32a570 <std::string::reserve(unsigned int)> @ imm = #-0x1ebfb4
  516520: e1a0700a     	mov	r7, r10
  516524: e08a8008     	add	r8, r10, r8
  516528: e1a0100a     	mov	r1, r10
  51652c: e1570008     	cmp	r7, r8
  516530: 2a000016     	bhs	0x516590 <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)+0xe8> @ imm = #0x58
  516534: e1d730d0     	ldrsb	r3, [r7]
  516538: e353000a     	cmp	r3, #10
  51653c: 0a00002b     	beq	0x5165f0 <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)+0x148> @ imm = #0xac
  516540: e353000d     	cmp	r3, #13
  516544: 12877001     	addne	r7, r7, #1
  516548: 1afffff7     	bne	0x51652c <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)+0x84> @ imm = #-0x24
  51654c: e0612007     	rsb	r2, r1, r7
  516550: e3520000     	cmp	r2, #0
  516554: da000002     	ble	0x516564 <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)+0xbc> @ imm = #0x8
  516558: e0812002     	add	r2, r1, r2
  51655c: e1a00004     	mov	r0, r4
  516560: ebf7e8a7     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0x205d64
  516564: e3a0100a     	mov	r1, #10
  516568: e1a00004     	mov	r0, r4
  51656c: ebf84f3a     	bl	0x32a25c <std::string::push_back(char)> @ imm = #-0x1ec318
  516570: e1d730d1     	ldrsb	r3, [r7, #1]
  516574: e2871001     	add	r1, r7, #1
  516578: e353000a     	cmp	r3, #10
  51657c: 02877002     	addeq	r7, r7, #2
  516580: 11a07001     	movne	r7, r1
  516584: 01a01007     	moveq	r1, r7
  516588: e1570008     	cmp	r7, r8
  51658c: 3affffe8     	blo	0x516534 <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)+0x8c> @ imm = #-0x60
  516590: e1570001     	cmp	r7, r1
  516594: 0a000002     	beq	0x5165a4 <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)+0xfc> @ imm = #0x8
  516598: e1a02007     	mov	r2, r7
  51659c: e1a00004     	mov	r0, r4
  5165a0: ebf7e897     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0x205da4
  5165a4: e1a0300b     	mov	r3, r11
  5165a8: e596c000     	ldr	r12, [r6]
  5165ac: e1a00006     	mov	r0, r6
  5165b0: e59d1020     	ldr	r1, [sp, #0x20]
  5165b4: e3a02000     	mov	r2, #0
  5165b8: e1a0e00f     	mov	lr, pc
  5165bc: e59cf00c     	ldr	pc, [r12, #0xc]
  5165c0: e5d63040     	ldrb	r3, [r6, #0x40]
  5165c4: e1a00004     	mov	r0, r4
  5165c8: e2234001     	eor	r4, r3, #1
  5165cc: ebf7f4f6     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x202c28
  5165d0: e7953009     	ldr	r3, [r5, r9]
  5165d4: e59d2024     	ldr	r2, [sp, #0x24]
  5165d8: e1a00004     	mov	r0, r4
  5165dc: e5933000     	ldr	r3, [r3]
  5165e0: e1520003     	cmp	r2, r3
  5165e4: 1a00000f     	bne	0x516628 <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)+0x180> @ imm = #0x3c
  5165e8: e28dd02c     	add	sp, sp, #44
  5165ec: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  5165f0: e2872001     	add	r2, r7, #1
  5165f4: e1a07002     	mov	r7, r2
  5165f8: e1a00004     	mov	r0, r4
  5165fc: ebf7e880     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0x205e00
  516600: e1a01007     	mov	r1, r7
  516604: eaffffc8     	b	0x51652c <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)+0x84> @ imm = #-0xe0
  516608: e3a04000     	mov	r4, #0
  51660c: e1a00006     	mov	r0, r6
  516610: e3a0100d     	mov	r1, #13
  516614: e1a02004     	mov	r2, r4
  516618: e1a03004     	mov	r3, r4
  51661c: e58d4000     	str	r4, [sp]
  516620: eb000bb0     	bl	0x5194e8 <TiXmlDocument::SetError(int, char const*, TiXmlParsingData*, TiXmlEncoding)> @ imm = #0x2ec0
  516624: eaffffe9     	b	0x5165d0 <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)+0x128> @ imm = #-0x5c
  516628: ebf7df38     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x208320
  51662c: d8 e5 47 00  	.word	0x0047e5d8
  516630: ac 40 00 00  	.word	0x000040ac
