
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00340e7c <ObjectBase* GetNewInstance<TriggerTrap>()>:
  340e7c: e92d4010     	push	{r4, lr}
  340e80: e3a01000     	mov	r1, #0
  340e84: e3000404     	movw	r0, #0x404
  340e88: ebff3db8     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x30920
  340e8c: e3a0100f     	mov	r1, #15
  340e90: e1a04000     	mov	r4, r0
  340e94: eb01778e     	bl	0x39ecd4 <TriggerTrap::TriggerTrap(ObjectBase::GO_IDS)> @ imm = #0x5de38
  340e98: e1a00004     	mov	r0, r4
  340e9c: e8bd8010     	pop	{r4, pc}


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039f168 <TriggerTrap::TransferVictims()>:
  39f168: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  39f16c: e5904390     	ldr	r4, [r0, #0x390]
  39f170: e24dd010     	sub	sp, sp, #16
  39f174: e1a06000     	mov	r6, r0
  39f178: e2807fe2     	add	r7, r0, #904
  39f17c: e2805e3e     	add	r5, r0, #992
  39f180: e2808ff2     	add	r8, r0, #968
  39f184: e28da004     	add	r10, sp, #4
  39f188: e28d900c     	add	r9, sp, #12
  39f18c: e1570004     	cmp	r7, r4
  39f190: 0a000021     	beq	0x39f21c <TriggerTrap::TransferVictims()+0xb4> @ imm = #0x84
  39f194: e59633e4     	ldr	r3, [r6, #0x3e4]
  39f198: e5941010     	ldr	r1, [r4, #0x10]
  39f19c: e3530000     	cmp	r3, #0
  39f1a0: e58d100c     	str	r1, [sp, #0xc]
  39f1a4: 0a00001e     	beq	0x39f224 <TriggerTrap::TransferVictims()+0xbc> @ imm = #0x78
  39f1a8: e1a00005     	mov	r0, r5
  39f1ac: ea000000     	b	0x39f1b4 <TriggerTrap::TransferVictims()+0x4c> @ imm = #0x0
  39f1b0: e1a03002     	mov	r3, r2
  39f1b4: e5932010     	ldr	r2, [r3, #0x10]
  39f1b8: e1510002     	cmp	r1, r2
  39f1bc: 8593200c     	ldrhi	r2, [r3, #0xc]
  39f1c0: 95932008     	ldrls	r2, [r3, #0x8]
  39f1c4: 81a03000     	movhi	r3, r0
  39f1c8: e1a00003     	mov	r0, r3
  39f1cc: e3520000     	cmp	r2, #0
  39f1d0: 1afffff6     	bne	0x39f1b0 <TriggerTrap::TransferVictims()+0x48> @ imm = #-0x28
  39f1d4: e1550003     	cmp	r5, r3
  39f1d8: 0a000014     	beq	0x39f230 <TriggerTrap::TransferVictims()+0xc8> @ imm = #0x50
  39f1dc: e5932010     	ldr	r2, [r3, #0x10]
  39f1e0: e1510002     	cmp	r1, r2
  39f1e4: 3a00000e     	blo	0x39f224 <TriggerTrap::TransferVictims()+0xbc> @ imm = #0x38
  39f1e8: e1550003     	cmp	r5, r3
  39f1ec: 0a00000f     	beq	0x39f230 <TriggerTrap::TransferVictims()+0xc8> @ imm = #0x3c
  39f1f0: e594200c     	ldr	r2, [r4, #0xc]
  39f1f4: e3520000     	cmp	r2, #0
  39f1f8: 0a00002d     	beq	0x39f2b4 <TriggerTrap::TransferVictims()+0x14c> @ imm = #0xb4
  39f1fc: e1a04002     	mov	r4, r2
  39f200: ea000000     	b	0x39f208 <TriggerTrap::TransferVictims()+0xa0> @ imm = #0x0
  39f204: e1a04003     	mov	r4, r3
  39f208: e5943008     	ldr	r3, [r4, #0x8]
  39f20c: e3530000     	cmp	r3, #0
  39f210: 1afffffb     	bne	0x39f204 <TriggerTrap::TransferVictims()+0x9c> @ imm = #-0x14
  39f214: e1570004     	cmp	r7, r4
  39f218: 1affffdd     	bne	0x39f194 <TriggerTrap::TransferVictims()+0x2c> @ imm = #-0x8c
  39f21c: e28dd010     	add	sp, sp, #16
  39f220: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  39f224: e1a03005     	mov	r3, r5
  39f228: e1550003     	cmp	r5, r3
  39f22c: 1affffef     	bne	0x39f1f0 <TriggerTrap::TransferVictims()+0x88> @ imm = #-0x44
  39f230: e59633cc     	ldr	r3, [r6, #0x3cc]
  39f234: e3530000     	cmp	r3, #0
  39f238: 0a00000f     	beq	0x39f27c <TriggerTrap::TransferVictims()+0x114> @ imm = #0x3c
  39f23c: e1a00008     	mov	r0, r8
  39f240: ea000000     	b	0x39f248 <TriggerTrap::TransferVictims()+0xe0> @ imm = #0x0
  39f244: e1a03002     	mov	r3, r2
  39f248: e5932010     	ldr	r2, [r3, #0x10]
  39f24c: e1510002     	cmp	r1, r2
  39f250: 8593200c     	ldrhi	r2, [r3, #0xc]
  39f254: 95932008     	ldrls	r2, [r3, #0x8]
  39f258: 81a03000     	movhi	r3, r0
  39f25c: e1a00003     	mov	r0, r3
  39f260: e3520000     	cmp	r2, #0
  39f264: 1afffff6     	bne	0x39f244 <TriggerTrap::TransferVictims()+0xdc> @ imm = #-0x28
  39f268: e1580003     	cmp	r8, r3
  39f26c: 0a000005     	beq	0x39f288 <TriggerTrap::TransferVictims()+0x120> @ imm = #0x14
  39f270: e5932010     	ldr	r2, [r3, #0x10]
  39f274: e1510002     	cmp	r1, r2
  39f278: 2a000000     	bhs	0x39f280 <TriggerTrap::TransferVictims()+0x118> @ imm = #0x0
  39f27c: e1a03008     	mov	r3, r8
  39f280: e1580003     	cmp	r8, r3
  39f284: 1affffd9     	bne	0x39f1f0 <TriggerTrap::TransferVictims()+0x88> @ imm = #-0x9c
  39f288: e1a00006     	mov	r0, r6
  39f28c: ebfffe02     	bl	0x39ea9c <TriggerTrap::CanTrigger(GameObject*)> @ imm = #-0x7f8
  39f290: e3500000     	cmp	r0, #0
  39f294: 0affffd5     	beq	0x39f1f0 <TriggerTrap::TransferVictims()+0x88> @ imm = #-0xac
  39f298: e1a02009     	mov	r2, r9
  39f29c: e1a0000a     	mov	r0, r10
  39f2a0: e1a01008     	mov	r1, r8
  39f2a4: ebffe458     	bl	0x39840c <std::priv::_Rb_tree<GameObject*, std::less<GameObject*>, GameObject*, std::priv::_Identity<GameObject*>, std::priv::_SetTraitsT<GameObject*>, std::allocator<GameObject*>>::insert_unique(GameObject* const&)> @ imm = #-0x6ea0
  39f2a8: e594200c     	ldr	r2, [r4, #0xc]
  39f2ac: e3520000     	cmp	r2, #0
  39f2b0: 1affffd1     	bne	0x39f1fc <TriggerTrap::TransferVictims()+0x94> @ imm = #-0xbc
  39f2b4: e5943004     	ldr	r3, [r4, #0x4]
  39f2b8: e593100c     	ldr	r1, [r3, #0xc]
  39f2bc: e1540001     	cmp	r4, r1
  39f2c0: 1a000005     	bne	0x39f2dc <TriggerTrap::TransferVictims()+0x174> @ imm = #0x14
  39f2c4: e1a04003     	mov	r4, r3
  39f2c8: e5933004     	ldr	r3, [r3, #0x4]
  39f2cc: e593200c     	ldr	r2, [r3, #0xc]
  39f2d0: e1520004     	cmp	r2, r4
  39f2d4: 0afffffa     	beq	0x39f2c4 <TriggerTrap::TransferVictims()+0x15c> @ imm = #-0x18
  39f2d8: e594200c     	ldr	r2, [r4, #0xc]
  39f2dc: e1520003     	cmp	r2, r3
  39f2e0: 11a04003     	movne	r4, r3
  39f2e4: eaffffa8     	b	0x39f18c <TriggerTrap::TransferVictims()+0x24> @ imm = #-0x160


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039e8ec <TriggerTrap::DeclareProperties()>:
  39e8ec: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  39e8f0: e59f4158     	ldr	r4, [pc, #0x158]        @ 0x39ea50 <TriggerTrap::DeclareProperties()+0x164>
  39e8f4: e59f9158     	ldr	r9, [pc, #0x158]        @ 0x39ea54 <TriggerTrap::DeclareProperties()+0x168>
  39e8f8: e24dd03c     	sub	sp, sp, #60
  39e8fc: e08f4004     	add	r4, pc, r4
  39e900: e7943009     	ldr	r3, [r4, r9]
  39e904: e28d601c     	add	r6, sp, #28
  39e908: e1a0a000     	mov	r10, r0
  39e90c: e5933000     	ldr	r3, [r3]
  39e910: e3a05000     	mov	r5, #0
  39e914: e28d7004     	add	r7, sp, #4
  39e918: e58d3034     	str	r3, [sp, #0x34]
  39e91c: ebffe535     	bl	0x397df8 <Zone::DeclareProperties()> @ imm = #-0x6b2c
  39e920: e1a00006     	mov	r0, r6
  39e924: e3a01010     	mov	r1, #16
  39e928: e58d602c     	str	r6, [sp, #0x2c]
  39e92c: e58d6030     	str	r6, [sp, #0x30]
  39e930: ebfdcb51     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x8d2bc
  39e934: e59d302c     	ldr	r3, [sp, #0x2c]
  39e938: e1a00007     	mov	r0, r7
  39e93c: e59f8114     	ldr	r8, [pc, #0x114]        @ 0x39ea58 <TriggerTrap::DeclareProperties()+0x16c>
  39e940: e5c35000     	strb	r5, [r3]
  39e944: e59d202c     	ldr	r2, [sp, #0x2c]
  39e948: e59d1030     	ldr	r1, [sp, #0x30]
  39e94c: e58d7014     	str	r7, [sp, #0x14]
  39e950: e58d7018     	str	r7, [sp, #0x18]
  39e954: ebfdcb63     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0x8d274
  39e958: e1a01005     	mov	r1, r5
  39e95c: e3a00038     	mov	r0, #56
  39e960: ebfdc702     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x8e3f8
  39e964: e59f30f0     	ldr	r3, [pc, #0xf0]         @ 0x39ea5c <TriggerTrap::DeclareProperties()+0x170>
  39e968: e08f8008     	add	r8, pc, r8
  39e96c: e1a05000     	mov	r5, r0
  39e970: e7943003     	ldr	r3, [r4, r3]
  39e974: e1a01008     	mov	r1, r8
  39e978: e1a0200d     	mov	r2, sp
  39e97c: e2833008     	add	r3, r3, #8
  39e980: e4803008     	str	r3, [r0], #8
  39e984: ebfdd5d8     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x8a8a0
  39e988: e59f30d0     	ldr	r3, [pc, #0xd0]         @ 0x39ea60 <TriggerTrap::DeclareProperties()+0x174>
  39e98c: e28abfea     	add	r11, r10, #936
  39e990: e28aa004     	add	r10, r10, #4
  39e994: e7943003     	ldr	r3, [r4, r3]
  39e998: e1a00005     	mov	r0, r5
  39e99c: e06ab00b     	rsb	r11, r10, r11
  39e9a0: e2833008     	add	r3, r3, #8
  39e9a4: e585b004     	str	r11, [r5, #0x4]
  39e9a8: e4803020     	str	r3, [r0], #32
  39e9ac: e5850030     	str	r0, [r5, #0x30]
  39e9b0: e5850034     	str	r0, [r5, #0x34]
  39e9b4: e59d1018     	ldr	r1, [sp, #0x18]
  39e9b8: e59d2014     	ldr	r2, [sp, #0x14]
  39e9bc: ebfdcb49     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0x8d2dc
  39e9c0: e1a0000a     	mov	r0, r10
  39e9c4: e1a01008     	mov	r1, r8
  39e9c8: e1a02005     	mov	r2, r5
  39e9cc: eb05d4c4     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x175310
  39e9d0: e59d0018     	ldr	r0, [sp, #0x18]
  39e9d4: e1500007     	cmp	r0, r7
  39e9d8: 0a000006     	beq	0x39e9f8 <TriggerTrap::DeclareProperties()+0x10c> @ imm = #0x18
  39e9dc: e3500000     	cmp	r0, #0
  39e9e0: 0a000004     	beq	0x39e9f8 <TriggerTrap::DeclareProperties()+0x10c> @ imm = #0x10
  39e9e4: e59d1004     	ldr	r1, [sp, #0x4]
  39e9e8: e0601001     	rsb	r1, r0, r1
  39e9ec: e3510080     	cmp	r1, #128
  39e9f0: 8a000013     	bhi	0x39ea44 <TriggerTrap::DeclareProperties()+0x158> @ imm = #0x4c
  39e9f4: eb0da941     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x36a504
  39e9f8: e59d0030     	ldr	r0, [sp, #0x30]
  39e9fc: e1500006     	cmp	r0, r6
  39ea00: 0a000006     	beq	0x39ea20 <TriggerTrap::DeclareProperties()+0x134> @ imm = #0x18
  39ea04: e3500000     	cmp	r0, #0
  39ea08: 0a000004     	beq	0x39ea20 <TriggerTrap::DeclareProperties()+0x134> @ imm = #0x10
  39ea0c: e59d101c     	ldr	r1, [sp, #0x1c]
  39ea10: e0601001     	rsb	r1, r0, r1
  39ea14: e3510080     	cmp	r1, #128
  39ea18: 8a000007     	bhi	0x39ea3c <TriggerTrap::DeclareProperties()+0x150> @ imm = #0x1c
  39ea1c: eb0da937     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x36a4dc
  39ea20: e7943009     	ldr	r3, [r4, r9]
  39ea24: e59d2034     	ldr	r2, [sp, #0x34]
  39ea28: e5933000     	ldr	r3, [r3]
  39ea2c: e1520003     	cmp	r2, r3
  39ea30: 1a000005     	bne	0x39ea4c <TriggerTrap::DeclareProperties()+0x160> @ imm = #0x14
  39ea34: e28dd03c     	add	sp, sp, #60
  39ea38: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  39ea3c: ebfdc67f     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x8e604
  39ea40: eafffff6     	b	0x39ea20 <TriggerTrap::DeclareProperties()+0x134> @ imm = #-0x28
  39ea44: ebfdc67d     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x8e60c
  39ea48: eaffffea     	b	0x39e9f8 <TriggerTrap::DeclareProperties()+0x10c> @ imm = #-0x58
  39ea4c: ebfdbe2f     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x90744
  39ea50: 94 61 5f 00  	.word	0x005f6194
  39ea54: ac 40 00 00  	.word	0x000040ac
  39ea58: f0 41 52 00  	.word	0x005241f0
  39ea5c: 30 23 00 00  	.word	0x00002330
  39ea60: 94 34 00 00  	.word	0x00003494


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039dee0 <TriggerTrap::InitPost()>:
  39dee0: e92d4070     	push	{r4, r5, r6, lr}
  39dee4: e24dd008     	sub	sp, sp, #8
  39dee8: e1a04000     	mov	r4, r0
  39deec: ebffb79c     	bl	0x38bd64 <GameObject::CheckSpawnProbability()> @ imm = #-0x12190
  39def0: e5943274     	ldr	r3, [r4, #0x274]
  39def4: e59f51a0     	ldr	r5, [pc, #0x1a0]        @ 0x39e09c <TriggerTrap::InitPost()+0x1bc>
  39def8: e1500003     	cmp	r0, r3
  39defc: e08f5005     	add	r5, pc, r5
  39df00: ba000001     	blt	0x39df0c <TriggerTrap::InitPost()+0x2c> @ imm = #0x4
  39df04: e28dd008     	add	sp, sp, #8
  39df08: e8bd8070     	pop	{r4, r5, r6, pc}
  39df0c: e5943000     	ldr	r3, [r4]
  39df10: e1a00004     	mov	r0, r4
  39df14: e1a0e00f     	mov	lr, pc
  39df18: e593f0d8     	ldr	pc, [r3, #0xd8]
  39df1c: e3700001     	cmn	r0, #1
  39df20: e58403c0     	str	r0, [r4, #0x3c0]
  39df24: 0a000015     	beq	0x39df80 <TriggerTrap::InitPost()+0xa0> @ imm = #0x54
  39df28: e5943000     	ldr	r3, [r4]
  39df2c: e1a00004     	mov	r0, r4
  39df30: e1a0e00f     	mov	lr, pc
  39df34: e593f0e0     	ldr	pc, [r3, #0xe0]
  39df38: e3700001     	cmn	r0, #1
  39df3c: 0a00000f     	beq	0x39df80 <TriggerTrap::InitPost()+0xa0> @ imm = #0x3c
  39df40: e59f2158     	ldr	r2, [pc, #0x158]        @ 0x39e0a0 <TriggerTrap::InitPost()+0x1c0>
  39df44: e5943000     	ldr	r3, [r4]
  39df48: e1a00004     	mov	r0, r4
  39df4c: e7952002     	ldr	r2, [r5, r2]
  39df50: e5926000     	ldr	r6, [r2]
  39df54: e1a0e00f     	mov	lr, pc
  39df58: e593f0e0     	ldr	pc, [r3, #0xe0]
  39df5c: e3a0300c     	mov	r3, #12
  39df60: e0266093     	mla	r6, r3, r0, r6
  39df64: e5966008     	ldr	r6, [r6, #0x8]
  39df68: e1a00006     	mov	r0, r6
  39df6c: ebfdbfb8     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x90120
  39df70: e1a01006     	mov	r1, r6
  39df74: e0862000     	add	r2, r6, r0
  39df78: e2840e29     	add	r0, r4, #656
  39df7c: ebfdca97     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x8d5a4
  39df80: e1a00004     	mov	r0, r4
  39df84: ebffe5e4     	bl	0x39771c <Zone::InitPost()> @ imm = #-0x6870
  39df88: e1a00004     	mov	r0, r4
  39df8c: ebffb2f3     	bl	0x38ab60 <GameObject::MeetCondition() const> @ imm = #-0x13434
  39df90: e2501000     	subs	r1, r0, #0
  39df94: 0a00003b     	beq	0x39e088 <TriggerTrap::InitPost()+0x1a8> @ imm = #0xec
  39df98: e59462d8     	ldr	r6, [r4, #0x2d8]
  39df9c: e3560000     	cmp	r6, #0
  39dfa0: 0a000021     	beq	0x39e02c <TriggerTrap::InitPost()+0x14c> @ imm = #0x84
  39dfa4: e59f30f8     	ldr	r3, [pc, #0xf8]         @ 0x39e0a4 <TriggerTrap::InitPost()+0x1c4>
  39dfa8: e5962038     	ldr	r2, [r6, #0x38]
  39dfac: e7951003     	ldr	r1, [r5, r3]
  39dfb0: e59f30f0     	ldr	r3, [pc, #0xf0]         @ 0x39e0a8 <TriggerTrap::InitPost()+0x1c8>
  39dfb4: e592c000     	ldr	r12, [r2]
  39dfb8: e1a00002     	mov	r0, r2
  39dfbc: e7953003     	ldr	r3, [r5, r3]
  39dfc0: e1a02004     	mov	r2, r4
  39dfc4: e58d4000     	str	r4, [sp]
  39dfc8: e1a0e00f     	mov	lr, pc
  39dfcc: e59cf02c     	ldr	pc, [r12, #0x2c]
  39dfd0: e596c038     	ldr	r12, [r6, #0x38]
  39dfd4: e59f10d0     	ldr	r1, [pc, #0xd0]         @ 0x39e0ac <TriggerTrap::InitPost()+0x1cc>
  39dfd8: e3a03000     	mov	r3, #0
  39dfdc: e1a0000c     	mov	r0, r12
  39dfe0: e1a02003     	mov	r2, r3
  39dfe4: e59cc000     	ldr	r12, [r12]
  39dfe8: e08f1001     	add	r1, pc, r1
  39dfec: e58d3000     	str	r3, [sp]
  39dff0: e1a0e00f     	mov	lr, pc
  39dff4: e59cf020     	ldr	pc, [r12, #0x20]
  39dff8: e250e000     	subs	lr, r0, #0
  39dffc: 1a00000a     	bne	0x39e02c <TriggerTrap::InitPost()+0x14c> @ imm = #0x28
  39e000: e3a02001     	mov	r2, #1
  39e004: e5c42401     	strb	r2, [r4, #0x401]
  39e008: e596c038     	ldr	r12, [r6, #0x38]
  39e00c: e59f109c     	ldr	r1, [pc, #0x9c]         @ 0x39e0b0 <TriggerTrap::InitPost()+0x1d0>
  39e010: e1a0300e     	mov	r3, lr
  39e014: e1a0000c     	mov	r0, r12
  39e018: e08f1001     	add	r1, pc, r1
  39e01c: e59cc000     	ldr	r12, [r12]
  39e020: e58de000     	str	lr, [sp]
  39e024: e1a0e00f     	mov	lr, pc
  39e028: e59cf020     	ldr	pc, [r12, #0x20]
  39e02c: e59f3080     	ldr	r3, [pc, #0x80]         @ 0x39e0b4 <TriggerTrap::InitPost()+0x1d4>
  39e030: e7953003     	ldr	r3, [r5, r3]
  39e034: e5935000     	ldr	r5, [r3]
  39e038: e3550000     	cmp	r5, #0
  39e03c: 0a000006     	beq	0x39e05c <TriggerTrap::InitPost()+0x17c> @ imm = #0x18
  39e040: e5943000     	ldr	r3, [r4]
  39e044: e1a00004     	mov	r0, r4
  39e048: e1a0e00f     	mov	lr, pc
  39e04c: e593f0ec     	ldr	pc, [r3, #0xec]
  39e050: e1a01000     	mov	r1, r0
  39e054: e1a00005     	mov	r0, r5
  39e058: ebff2e67     	bl	0x3699fc <VoxSoundManager::LoadSound(int)> @ imm = #-0x34664
  39e05c: e5943000     	ldr	r3, [r4]
  39e060: e1a00004     	mov	r0, r4
  39e064: e1a0e00f     	mov	lr, pc
  39e068: e593f0dc     	ldr	pc, [r3, #0xdc]
  39e06c: e59f2044     	ldr	r2, [pc, #0x44]         @ 0x39e0b8 <TriggerTrap::InitPost()+0x1d8>
  39e070: e1a01000     	mov	r1, r0
  39e074: e1a00004     	mov	r0, r4
  39e078: e08f2002     	add	r2, pc, r2
  39e07c: e28dd008     	add	sp, sp, #8
  39e080: e8bd4070     	pop	{r4, r5, r6, lr}
  39e084: eaffc3b5     	b	0x38ef60 <GameObject::LoadExternalScript(char const*, char const*)> @ imm = #-0xf12c
  39e088: e1a00004     	mov	r0, r4
  39e08c: e5943000     	ldr	r3, [r4]
  39e090: e1a0e00f     	mov	lr, pc
  39e094: e593f040     	ldr	pc, [r3, #0x40]
  39e098: eaffff99     	b	0x39df04 <TriggerTrap::InitPost()+0x24> @ imm = #-0x19c
  39e09c: 94 6b 5f 00  	.word	0x005f6b94
  39e0a0: a8 1c 00 00  	.word	0x00001ca8
  39e0a4: 60 48 00 00  	.word	0x00004860
  39e0a8: 34 2a 00 00  	.word	0x00002a34
  39e0ac: c0 4e 52 00  	.word	0x00524ec0
  39e0b0: 98 42 52 00  	.word	0x00524298
  39e0b4: a4 0d 00 00  	.word	0x00000da4
  39e0b8: 08 4b 52 00  	.word	0x00524b08


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039ecd4 <TriggerTrap::TriggerTrap(ObjectBase::GO_IDS)>:
  39ecd4: e92d4070     	push	{r4, r5, r6, lr}
  39ecd8: e3a02001     	mov	r2, #1
  39ecdc: e3a03000     	mov	r3, #0
  39ece0: e59f50ac     	ldr	r5, [pc, #0xac]         @ 0x39ed94 <TriggerTrap::TriggerTrap(ObjectBase::GO_IDS)+0xc0>
  39ece4: e1a04000     	mov	r4, r0
  39ece8: ebffe48e     	bl	0x397f28 <ZoneEx::ZoneEx(ObjectBase::GO_IDS, bool, bool)> @ imm = #-0x6dc8
  39ecec: e59f30a4     	ldr	r3, [pc, #0xa4]         @ 0x39ed98 <TriggerTrap::TriggerTrap(ObjectBase::GO_IDS)+0xc4>
  39ecf0: e08f5005     	add	r5, pc, r5
  39ecf4: e2842fea     	add	r2, r4, #936
  39ecf8: e7953003     	ldr	r3, [r5, r3]
  39ecfc: e1a00002     	mov	r0, r2
  39ed00: e58423b8     	str	r2, [r4, #0x3b8]
  39ed04: e283c008     	add	r12, r3, #8
  39ed08: e2831f45     	add	r1, r3, #276
  39ed0c: e2833f42     	add	r3, r3, #264
  39ed10: e5843004     	str	r3, [r4, #0x4]
  39ed14: e5841024     	str	r1, [r4, #0x24]
  39ed18: e58423bc     	str	r2, [r4, #0x3bc]
  39ed1c: e584c000     	str	r12, [r4]
  39ed20: e3a01010     	mov	r1, #16
  39ed24: ebfdca54     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x8d6b0
  39ed28: e59423b8     	ldr	r2, [r4, #0x3b8]
  39ed2c: e3a03000     	mov	r3, #0
  39ed30: e1a01004     	mov	r1, r4
  39ed34: e3e00000     	mvn	r0, #0
  39ed38: e5c23000     	strb	r3, [r2]
  39ed3c: e58403c0     	str	r0, [r4, #0x3c0]
  39ed40: e1a02004     	mov	r2, r4
  39ed44: e5c433c4     	strb	r3, [r4, #0x3c4]
  39ed48: e58433cc     	str	r3, [r4, #0x3cc]
  39ed4c: e5e133c8     	strb	r3, [r1, #0x3c8]!
  39ed50: e58413d4     	str	r1, [r4, #0x3d4]
  39ed54: e58413d0     	str	r1, [r4, #0x3d0]
  39ed58: e58433d8     	str	r3, [r4, #0x3d8]
  39ed5c: e3a01001     	mov	r1, #1
  39ed60: e58433e4     	str	r3, [r4, #0x3e4]
  39ed64: e5e233e0     	strb	r3, [r2, #0x3e0]!
  39ed68: e58423ec     	str	r2, [r4, #0x3ec]
  39ed6c: e58403fc     	str	r0, [r4, #0x3fc]
  39ed70: e5c43401     	strb	r3, [r4, #0x401]
  39ed74: e5c41085     	strb	r1, [r4, #0x85]
  39ed78: e58423e8     	str	r2, [r4, #0x3e8]
  39ed7c: e58433f0     	str	r3, [r4, #0x3f0]
  39ed80: e58433f8     	str	r3, [r4, #0x3f8]
  39ed84: e5c43400     	strb	r3, [r4, #0x400]
  39ed88: e5c41084     	strb	r1, [r4, #0x84]
  39ed8c: e1a00004     	mov	r0, r4
  39ed90: e8bd8070     	pop	{r4, r5, r6, pc}
  39ed94: a0 5d 5f 00  	.word	0x005f5da0
  39ed98: c8 1d 00 00  	.word	0x00001dc8


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039e670 <TriggerTrap::TriggerTrap(ObjectBase::GO_IDS)>:
  39e670: e92d4070     	push	{r4, r5, r6, lr}
  39e674: e3a02001     	mov	r2, #1
  39e678: e3a03000     	mov	r3, #0
  39e67c: e59f50ac     	ldr	r5, [pc, #0xac]         @ 0x39e730 <TriggerTrap::TriggerTrap(ObjectBase::GO_IDS)+0xc0>
  39e680: e1a04000     	mov	r4, r0
  39e684: ebffe627     	bl	0x397f28 <ZoneEx::ZoneEx(ObjectBase::GO_IDS, bool, bool)> @ imm = #-0x6764
  39e688: e59f30a4     	ldr	r3, [pc, #0xa4]         @ 0x39e734 <TriggerTrap::TriggerTrap(ObjectBase::GO_IDS)+0xc4>
  39e68c: e08f5005     	add	r5, pc, r5
  39e690: e2842fea     	add	r2, r4, #936
  39e694: e7953003     	ldr	r3, [r5, r3]
  39e698: e1a00002     	mov	r0, r2
  39e69c: e58423b8     	str	r2, [r4, #0x3b8]
  39e6a0: e283c008     	add	r12, r3, #8
  39e6a4: e2831f45     	add	r1, r3, #276
  39e6a8: e2833f42     	add	r3, r3, #264
  39e6ac: e5843004     	str	r3, [r4, #0x4]
  39e6b0: e5841024     	str	r1, [r4, #0x24]
  39e6b4: e58423bc     	str	r2, [r4, #0x3bc]
  39e6b8: e584c000     	str	r12, [r4]
  39e6bc: e3a01010     	mov	r1, #16
  39e6c0: ebfdcbed     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x8d04c
  39e6c4: e59423b8     	ldr	r2, [r4, #0x3b8]
  39e6c8: e3a03000     	mov	r3, #0
  39e6cc: e1a01004     	mov	r1, r4
  39e6d0: e3e00000     	mvn	r0, #0
  39e6d4: e5c23000     	strb	r3, [r2]
  39e6d8: e58403c0     	str	r0, [r4, #0x3c0]
  39e6dc: e1a02004     	mov	r2, r4
  39e6e0: e5c433c4     	strb	r3, [r4, #0x3c4]
  39e6e4: e58433cc     	str	r3, [r4, #0x3cc]
  39e6e8: e5e133c8     	strb	r3, [r1, #0x3c8]!
  39e6ec: e58413d4     	str	r1, [r4, #0x3d4]
  39e6f0: e58413d0     	str	r1, [r4, #0x3d0]
  39e6f4: e58433d8     	str	r3, [r4, #0x3d8]
  39e6f8: e3a01001     	mov	r1, #1
  39e6fc: e58433e4     	str	r3, [r4, #0x3e4]
  39e700: e5e233e0     	strb	r3, [r2, #0x3e0]!
  39e704: e58423ec     	str	r2, [r4, #0x3ec]
  39e708: e58403fc     	str	r0, [r4, #0x3fc]
  39e70c: e5c43401     	strb	r3, [r4, #0x401]
  39e710: e5c41085     	strb	r1, [r4, #0x85]
  39e714: e58423e8     	str	r2, [r4, #0x3e8]
  39e718: e58433f0     	str	r3, [r4, #0x3f0]
  39e71c: e58433f8     	str	r3, [r4, #0x3f8]
  39e720: e5c43400     	strb	r3, [r4, #0x400]
  39e724: e5c41084     	strb	r1, [r4, #0x84]
  39e728: e1a00004     	mov	r0, r4
  39e72c: e8bd8070     	pop	{r4, r5, r6, pc}
  39e730: 04 64 5f 00  	.word	0x005f6404
  39e734: c8 1d 00 00  	.word	0x00001dc8


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039e56c <TriggerTrap::~TriggerTrap()>:
  39e56c: e92d4010     	push	{r4, lr}
  39e570: e1a04000     	mov	r4, r0
  39e574: ebffffc1     	bl	0x39e480 <TriggerTrap::~TriggerTrap()> @ imm = #-0xfc
  39e578: e1a00004     	mov	r0, r4
  39e57c: ebfdc7af     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x8e144
  39e580: e1a00004     	mov	r0, r4
  39e584: e8bd8010     	pop	{r4, pc}


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039e480 <TriggerTrap::~TriggerTrap()>:
  39e480: e92d4070     	push	{r4, r5, r6, lr}
  39e484: e59f20d0     	ldr	r2, [pc, #0xd0]         @ 0x39e55c <TriggerTrap::~TriggerTrap()+0xdc>
  39e488: e59f30d0     	ldr	r3, [pc, #0xd0]         @ 0x39e560 <TriggerTrap::~TriggerTrap()+0xe0>
  39e48c: e59013f0     	ldr	r1, [r0, #0x3f0]
  39e490: e08f2002     	add	r2, pc, r2
  39e494: e7923003     	ldr	r3, [r2, r3]
  39e498: e3510000     	cmp	r1, #0
  39e49c: e1a04000     	mov	r4, r0
  39e4a0: e2832f45     	add	r2, r3, #276
  39e4a4: e2831008     	add	r1, r3, #8
  39e4a8: e2833f42     	add	r3, r3, #264
  39e4ac: e880000a     	stm	r0, {r1, r3}
  39e4b0: e5802024     	str	r2, [r0, #0x24]
  39e4b4: 0a000008     	beq	0x39e4dc <TriggerTrap::~TriggerTrap()+0x5c> @ imm = #0x20
  39e4b8: e2805e3e     	add	r5, r0, #992
  39e4bc: e1a00005     	mov	r0, r5
  39e4c0: e59413e4     	ldr	r1, [r4, #0x3e4]
  39e4c4: ebffe854     	bl	0x39861c <std::priv::_Rb_tree<GameObject*, std::less<GameObject*>, GameObject*, std::priv::_Identity<GameObject*>, std::priv::_SetTraitsT<GameObject*>, std::allocator<GameObject*>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x5eb0
  39e4c8: e3a03000     	mov	r3, #0
  39e4cc: e58453ec     	str	r5, [r4, #0x3ec]
  39e4d0: e58433f0     	str	r3, [r4, #0x3f0]
  39e4d4: e58453e8     	str	r5, [r4, #0x3e8]
  39e4d8: e58433e4     	str	r3, [r4, #0x3e4]
  39e4dc: e59433d8     	ldr	r3, [r4, #0x3d8]
  39e4e0: e3530000     	cmp	r3, #0
  39e4e4: 0a000008     	beq	0x39e50c <TriggerTrap::~TriggerTrap()+0x8c> @ imm = #0x20
  39e4e8: e2845ff2     	add	r5, r4, #968
  39e4ec: e1a00005     	mov	r0, r5
  39e4f0: e59413cc     	ldr	r1, [r4, #0x3cc]
  39e4f4: ebffe848     	bl	0x39861c <std::priv::_Rb_tree<GameObject*, std::less<GameObject*>, GameObject*, std::priv::_Identity<GameObject*>, std::priv::_SetTraitsT<GameObject*>, std::allocator<GameObject*>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x5ee0
  39e4f8: e3a03000     	mov	r3, #0
  39e4fc: e58453d4     	str	r5, [r4, #0x3d4]
  39e500: e58433d8     	str	r3, [r4, #0x3d8]
  39e504: e58453d0     	str	r5, [r4, #0x3d0]
  39e508: e58433cc     	str	r3, [r4, #0x3cc]
  39e50c: e2843fea     	add	r3, r4, #936
  39e510: e5930014     	ldr	r0, [r3, #0x14]
  39e514: e1500003     	cmp	r0, r3
  39e518: 0a000006     	beq	0x39e538 <TriggerTrap::~TriggerTrap()+0xb8> @ imm = #0x18
  39e51c: e3500000     	cmp	r0, #0
  39e520: 0a000004     	beq	0x39e538 <TriggerTrap::~TriggerTrap()+0xb8> @ imm = #0x10
  39e524: e59413a8     	ldr	r1, [r4, #0x3a8]
  39e528: e0601001     	rsb	r1, r0, r1
  39e52c: e3510080     	cmp	r1, #128
  39e530: 8a000004     	bhi	0x39e548 <TriggerTrap::~TriggerTrap()+0xc8> @ imm = #0x10
  39e534: eb0daa71     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x36a9c4
  39e538: e1a00004     	mov	r0, r4
  39e53c: ebffe86c     	bl	0x3986f4 <ZoneEx::~ZoneEx()> @ imm = #-0x5e50
  39e540: e1a00004     	mov	r0, r4
  39e544: e8bd8070     	pop	{r4, r5, r6, pc}
  39e548: ebfdc7bc     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x8e110
  39e54c: e1a00004     	mov	r0, r4
  39e550: ebffe867     	bl	0x3986f4 <ZoneEx::~ZoneEx()> @ imm = #-0x5e64
  39e554: e1a00004     	mov	r0, r4
  39e558: e8bd8070     	pop	{r4, r5, r6, pc}
  39e55c: 00 66 5f 00  	.word	0x005f6600
  39e560: c8 1d 00 00  	.word	0x00001dc8


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039e58c <TriggerTrap::~TriggerTrap()>:
  39e58c: e92d4070     	push	{r4, r5, r6, lr}
  39e590: e59f20d0     	ldr	r2, [pc, #0xd0]         @ 0x39e668 <TriggerTrap::~TriggerTrap()+0xdc>
  39e594: e59f30d0     	ldr	r3, [pc, #0xd0]         @ 0x39e66c <TriggerTrap::~TriggerTrap()+0xe0>
  39e598: e59013f0     	ldr	r1, [r0, #0x3f0]
  39e59c: e08f2002     	add	r2, pc, r2
  39e5a0: e7923003     	ldr	r3, [r2, r3]
  39e5a4: e3510000     	cmp	r1, #0
  39e5a8: e1a04000     	mov	r4, r0
  39e5ac: e2832f45     	add	r2, r3, #276
  39e5b0: e2831008     	add	r1, r3, #8
  39e5b4: e2833f42     	add	r3, r3, #264
  39e5b8: e880000a     	stm	r0, {r1, r3}
  39e5bc: e5802024     	str	r2, [r0, #0x24]
  39e5c0: 0a000008     	beq	0x39e5e8 <TriggerTrap::~TriggerTrap()+0x5c> @ imm = #0x20
  39e5c4: e2805e3e     	add	r5, r0, #992
  39e5c8: e1a00005     	mov	r0, r5
  39e5cc: e59413e4     	ldr	r1, [r4, #0x3e4]
  39e5d0: ebffe811     	bl	0x39861c <std::priv::_Rb_tree<GameObject*, std::less<GameObject*>, GameObject*, std::priv::_Identity<GameObject*>, std::priv::_SetTraitsT<GameObject*>, std::allocator<GameObject*>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x5fbc
  39e5d4: e3a03000     	mov	r3, #0
  39e5d8: e58453ec     	str	r5, [r4, #0x3ec]
  39e5dc: e58433f0     	str	r3, [r4, #0x3f0]
  39e5e0: e58453e8     	str	r5, [r4, #0x3e8]
  39e5e4: e58433e4     	str	r3, [r4, #0x3e4]
  39e5e8: e59433d8     	ldr	r3, [r4, #0x3d8]
  39e5ec: e3530000     	cmp	r3, #0
  39e5f0: 0a000008     	beq	0x39e618 <TriggerTrap::~TriggerTrap()+0x8c> @ imm = #0x20
  39e5f4: e2845ff2     	add	r5, r4, #968
  39e5f8: e1a00005     	mov	r0, r5
  39e5fc: e59413cc     	ldr	r1, [r4, #0x3cc]
  39e600: ebffe805     	bl	0x39861c <std::priv::_Rb_tree<GameObject*, std::less<GameObject*>, GameObject*, std::priv::_Identity<GameObject*>, std::priv::_SetTraitsT<GameObject*>, std::allocator<GameObject*>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x5fec
  39e604: e3a03000     	mov	r3, #0
  39e608: e58453d4     	str	r5, [r4, #0x3d4]
  39e60c: e58433d8     	str	r3, [r4, #0x3d8]
  39e610: e58453d0     	str	r5, [r4, #0x3d0]
  39e614: e58433cc     	str	r3, [r4, #0x3cc]
  39e618: e2843fea     	add	r3, r4, #936
  39e61c: e5930014     	ldr	r0, [r3, #0x14]
  39e620: e1500003     	cmp	r0, r3
  39e624: 0a000006     	beq	0x39e644 <TriggerTrap::~TriggerTrap()+0xb8> @ imm = #0x18
  39e628: e3500000     	cmp	r0, #0
  39e62c: 0a000004     	beq	0x39e644 <TriggerTrap::~TriggerTrap()+0xb8> @ imm = #0x10
  39e630: e59413a8     	ldr	r1, [r4, #0x3a8]
  39e634: e0601001     	rsb	r1, r0, r1
  39e638: e3510080     	cmp	r1, #128
  39e63c: 8a000004     	bhi	0x39e654 <TriggerTrap::~TriggerTrap()+0xc8> @ imm = #0x10
  39e640: eb0daa2e     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x36a8b8
  39e644: e1a00004     	mov	r0, r4
  39e648: ebffe829     	bl	0x3986f4 <ZoneEx::~ZoneEx()> @ imm = #-0x5f5c
  39e64c: e1a00004     	mov	r0, r4
  39e650: e8bd8070     	pop	{r4, r5, r6, pc}
  39e654: ebfdc779     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x8e21c
  39e658: e1a00004     	mov	r0, r4
  39e65c: ebffe824     	bl	0x3986f4 <ZoneEx::~ZoneEx()> @ imm = #-0x5f70
  39e660: e1a00004     	mov	r0, r4
  39e664: e8bd8070     	pop	{r4, r5, r6, pc}
  39e668: f4 64 5f 00  	.word	0x005f64f4
  39e66c: c8 1d 00 00  	.word	0x00001dc8


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039bfa4 <TriggerZone::DeclareProperties()>:
  39bfa4: e92d4070     	push	{r4, r5, r6, lr}
  39bfa8: e1a05000     	mov	r5, r0
  39bfac: ebfff2b5     	bl	0x398a88 <Trigger::DeclareProperties()> @ imm = #-0x352c
  39bfb0: e59f10cc     	ldr	r1, [pc, #0xcc]         @ 0x39c084 <TriggerZone::DeclareProperties()+0xe0>
  39bfb4: e2854004     	add	r4, r5, #4
  39bfb8: e2856e71     	add	r6, r5, #1808
  39bfbc: e1a00004     	mov	r0, r4
  39bfc0: e2862008     	add	r2, r6, #8
  39bfc4: e08f1001     	add	r1, pc, r1
  39bfc8: ebffffd0     	bl	0x39bf10 <void PropertyMap::AddProperty<int>(char const*, int&, int) (.clone.2)> @ imm = #-0xc0
  39bfcc: e59f10b4     	ldr	r1, [pc, #0xb4]         @ 0x39c088 <TriggerZone::DeclareProperties()+0xe4>
  39bfd0: e286200c     	add	r2, r6, #12
  39bfd4: e1a00004     	mov	r0, r4
  39bfd8: e08f1001     	add	r1, pc, r1
  39bfdc: ebffffcb     	bl	0x39bf10 <void PropertyMap::AddProperty<int>(char const*, int&, int) (.clone.2)> @ imm = #-0xd4
  39bfe0: e59f10a4     	ldr	r1, [pc, #0xa4]         @ 0x39c08c <TriggerZone::DeclareProperties()+0xe8>
  39bfe4: e2856e72     	add	r6, r5, #1824
  39bfe8: e1a00004     	mov	r0, r4
  39bfec: e1a02006     	mov	r2, r6
  39bff0: e08f1001     	add	r1, pc, r1
  39bff4: ebffffc5     	bl	0x39bf10 <void PropertyMap::AddProperty<int>(char const*, int&, int) (.clone.2)> @ imm = #-0xec
  39bff8: e59f1090     	ldr	r1, [pc, #0x90]         @ 0x39c090 <TriggerZone::DeclareProperties()+0xec>
  39bffc: e2862004     	add	r2, r6, #4
  39c000: e1a00004     	mov	r0, r4
  39c004: e08f1001     	add	r1, pc, r1
  39c008: ebfe8bdb     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x5d094
  39c00c: e59f1080     	ldr	r1, [pc, #0x80]         @ 0x39c094 <TriggerZone::DeclareProperties()+0xf0>
  39c010: e1a00004     	mov	r0, r4
  39c014: e2852d1d     	add	r2, r5, #1856
  39c018: e08f1001     	add	r1, pc, r1
  39c01c: ebfe8bd6     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x5d0a8
  39c020: e59f1070     	ldr	r1, [pc, #0x70]         @ 0x39c098 <TriggerZone::DeclareProperties()+0xf4>
  39c024: e2852e75     	add	r2, r5, #1872
  39c028: e1a00004     	mov	r0, r4
  39c02c: e282200c     	add	r2, r2, #12
  39c030: e08f1001     	add	r1, pc, r1
  39c034: ebfe8bd0     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x5d0c0
  39c038: e59f105c     	ldr	r1, [pc, #0x5c]         @ 0x39c09c <TriggerZone::DeclareProperties()+0xf8>
  39c03c: e2852e77     	add	r2, r5, #1904
  39c040: e1a00004     	mov	r0, r4
  39c044: e2822008     	add	r2, r2, #8
  39c048: e08f1001     	add	r1, pc, r1
  39c04c: ebfe8bca     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x5d0d8
  39c050: e59f1048     	ldr	r1, [pc, #0x48]         @ 0x39c0a0 <TriggerZone::DeclareProperties()+0xfc>
  39c054: e2852e79     	add	r2, r5, #1936
  39c058: e1a00004     	mov	r0, r4
  39c05c: e2822004     	add	r2, r2, #4
  39c060: e08f1001     	add	r1, pc, r1
  39c064: ebfe8bc4     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x5d0f0
  39c068: e59f1034     	ldr	r1, [pc, #0x34]         @ 0x39c0a4 <TriggerZone::DeclareProperties()+0x100>
  39c06c: e2852e7b     	add	r2, r5, #1968
  39c070: e1a00004     	mov	r0, r4
  39c074: e08f1001     	add	r1, pc, r1
  39c078: e282200c     	add	r2, r2, #12
  39c07c: e8bd4070     	pop	{r4, r5, r6, lr}
  39c080: eafe8bbd     	b	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x5d10c
  39c084: ec 6c 52 00  	.word	0x00526cec
  39c088: e0 6c 52 00  	.word	0x00526ce0
  39c08c: d8 6c 52 00  	.word	0x00526cd8
  39c090: d4 59 52 00  	.word	0x005259d4
  39c094: b8 6c 52 00  	.word	0x00526cb8
  39c098: b0 6c 52 00  	.word	0x00526cb0
  39c09c: b0 6c 52 00  	.word	0x00526cb0
  39c0a0: b8 6c 52 00  	.word	0x00526cb8
  39c0a4: bc 6c 52 00  	.word	0x00526cbc


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00397df8 <Zone::DeclareProperties()>:
  397df8: e92d40f0     	push	{r4, r5, r6, r7, lr}
  397dfc: e24dd00c     	sub	sp, sp, #12
  397e00: e1a07000     	mov	r7, r0
  397e04: ebffd437     	bl	0x38cee8 <GameObject::DeclareProperties()> @ imm = #-0xaf24
  397e08: e3a01000     	mov	r1, #0
  397e0c: e3a0002c     	mov	r0, #44
  397e10: ebfde1d6     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x878a8
  397e14: e59f5070     	ldr	r5, [pc, #0x70]         @ 0x397e8c <Zone::DeclareProperties()+0x94>
  397e18: e59f3070     	ldr	r3, [pc, #0x70]         @ 0x397e90 <Zone::DeclareProperties()+0x98>
  397e1c: e59f6070     	ldr	r6, [pc, #0x70]         @ 0x397e94 <Zone::DeclareProperties()+0x9c>
  397e20: e08f5005     	add	r5, pc, r5
  397e24: e7953003     	ldr	r3, [r5, r3]
  397e28: e08f6006     	add	r6, pc, r6
  397e2c: e1a04000     	mov	r4, r0
  397e30: e2833008     	add	r3, r3, #8
  397e34: e1a01006     	mov	r1, r6
  397e38: e28d2004     	add	r2, sp, #4
  397e3c: e4803008     	str	r3, [r0], #8
  397e40: ebfdf0a9     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x83d5c
  397e44: e59f204c     	ldr	r2, [pc, #0x4c]         @ 0x397e98 <Zone::DeclareProperties()+0xa0>
  397e48: e2871fdd     	add	r1, r7, #884
  397e4c: e2870004     	add	r0, r7, #4
  397e50: e7952002     	ldr	r2, [r5, r2]
  397e54: e3a03443     	mov	r3, #1124073472
  397e58: e2833712     	add	r3, r3, #4718592
  397e5c: e0601001     	rsb	r1, r0, r1
  397e60: e2822008     	add	r2, r2, #8
  397e64: e5841004     	str	r1, [r4, #0x4]
  397e68: e5842000     	str	r2, [r4]
  397e6c: e5843028     	str	r3, [r4, #0x28]
  397e70: e5843020     	str	r3, [r4, #0x20]
  397e74: e5843024     	str	r3, [r4, #0x24]
  397e78: e1a01006     	mov	r1, r6
  397e7c: e1a02004     	mov	r2, r4
  397e80: eb05ef97     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x17be5c
  397e84: e28dd00c     	add	sp, sp, #12
  397e88: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  397e8c: 70 cc 5f 00  	.word	0x005fcc70
  397e90: 30 23 00 00  	.word	0x00002330
  397e94: b8 a4 52 00  	.word	0x0052a4b8
  397e98: 44 0b 00 00  	.word	0x00000b44


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00397c2c <Zone::Zone(ObjectBase::GO_IDS, bool, bool)>:
  397c2c: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  397c30: e59f4060     	ldr	r4, [pc, #0x60]         @ 0x397c98 <Zone::Zone(ObjectBase::GO_IDS, bool, bool)+0x6c>
  397c34: e1a06000     	mov	r6, r0
  397c38: e1a05002     	mov	r5, r2
  397c3c: e1a07003     	mov	r7, r3
  397c40: ebffd1d4     	bl	0x38c398 <GameObject::GameObject(ObjectBase::GO_IDS)> @ imm = #-0xb8b0
  397c44: e59f3050     	ldr	r3, [pc, #0x50]         @ 0x397c9c <Zone::Zone(ObjectBase::GO_IDS, bool, bool)+0x70>
  397c48: e08f4004     	add	r4, pc, r4
  397c4c: e3a02000     	mov	r2, #0
  397c50: e7943003     	ldr	r3, [r4, r3]
  397c54: e586237c     	str	r2, [r6, #0x37c]
  397c58: e5c65380     	strb	r5, [r6, #0x380]
  397c5c: e28310f4     	add	r1, r3, #244
  397c60: e2830008     	add	r0, r3, #8
  397c64: e28330e8     	add	r3, r3, #232
  397c68: e5863004     	str	r3, [r6, #0x4]
  397c6c: e3a03000     	mov	r3, #0
  397c70: e5863384     	str	r3, [r6, #0x384]
  397c74: e3a03001     	mov	r3, #1
  397c78: e5860000     	str	r0, [r6]
  397c7c: e5861024     	str	r1, [r6, #0x24]
  397c80: e5c67381     	strb	r7, [r6, #0x381]
  397c84: e5c63084     	strb	r3, [r6, #0x84]
  397c88: e5862374     	str	r2, [r6, #0x374]
  397c8c: e5862378     	str	r2, [r6, #0x378]
  397c90: e1a00006     	mov	r0, r6
  397c94: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  397c98: 48 ce 5f 00  	.word	0x005fce48
  397c9c: 34 0d 00 00  	.word	0x00000d34


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00397ca0 <Zone::Zone(ObjectBase::GO_IDS, bool, bool)>:
  397ca0: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  397ca4: e59f4060     	ldr	r4, [pc, #0x60]         @ 0x397d0c <Zone::Zone(ObjectBase::GO_IDS, bool, bool)+0x6c>
  397ca8: e1a06000     	mov	r6, r0
  397cac: e1a05002     	mov	r5, r2
  397cb0: e1a07003     	mov	r7, r3
  397cb4: ebffd1b7     	bl	0x38c398 <GameObject::GameObject(ObjectBase::GO_IDS)> @ imm = #-0xb924
  397cb8: e59f3050     	ldr	r3, [pc, #0x50]         @ 0x397d10 <Zone::Zone(ObjectBase::GO_IDS, bool, bool)+0x70>
  397cbc: e08f4004     	add	r4, pc, r4
  397cc0: e3a02000     	mov	r2, #0
  397cc4: e7943003     	ldr	r3, [r4, r3]
  397cc8: e586237c     	str	r2, [r6, #0x37c]
  397ccc: e5c65380     	strb	r5, [r6, #0x380]
  397cd0: e28310f4     	add	r1, r3, #244
  397cd4: e2830008     	add	r0, r3, #8
  397cd8: e28330e8     	add	r3, r3, #232
  397cdc: e5863004     	str	r3, [r6, #0x4]
  397ce0: e3a03000     	mov	r3, #0
  397ce4: e5863384     	str	r3, [r6, #0x384]
  397ce8: e3a03001     	mov	r3, #1
  397cec: e5860000     	str	r0, [r6]
  397cf0: e5861024     	str	r1, [r6, #0x24]
  397cf4: e5c67381     	strb	r7, [r6, #0x381]
  397cf8: e5c63084     	strb	r3, [r6, #0x84]
  397cfc: e5862374     	str	r2, [r6, #0x374]
  397d00: e5862378     	str	r2, [r6, #0x378]
  397d04: e1a00006     	mov	r0, r6
  397d08: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  397d0c: d4 cd 5f 00  	.word	0x005fcdd4
  397d10: 34 0d 00 00  	.word	0x00000d34


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00397ec0 <ZoneEx::ZoneEx(ObjectBase::GO_IDS, bool, bool)>:
  397ec0: e92d4070     	push	{r4, r5, r6, lr}
  397ec4: e59f5054     	ldr	r5, [pc, #0x54]         @ 0x397f20 <ZoneEx::ZoneEx(ObjectBase::GO_IDS, bool, bool)+0x60>
  397ec8: e1a04000     	mov	r4, r0
  397ecc: ebffff73     	bl	0x397ca0 <Zone::Zone(ObjectBase::GO_IDS, bool, bool)> @ imm = #-0x234
  397ed0: e59f204c     	ldr	r2, [pc, #0x4c]         @ 0x397f24 <ZoneEx::ZoneEx(ObjectBase::GO_IDS, bool, bool)+0x64>
  397ed4: e08f5005     	add	r5, pc, r5
  397ed8: e3a03000     	mov	r3, #0
  397edc: e7952002     	ldr	r2, [r5, r2]
  397ee0: e1a01004     	mov	r1, r4
  397ee4: e584338c     	str	r3, [r4, #0x38c]
  397ee8: e28200f4     	add	r0, r2, #244
  397eec: e282c008     	add	r12, r2, #8
  397ef0: e28220e8     	add	r2, r2, #232
  397ef4: e5840024     	str	r0, [r4, #0x24]
  397ef8: e584c000     	str	r12, [r4]
  397efc: e5842004     	str	r2, [r4, #0x4]
  397f00: e5e13388     	strb	r3, [r1, #0x388]!
  397f04: e5841394     	str	r1, [r4, #0x394]
  397f08: e58433a4     	str	r3, [r4, #0x3a4]
  397f0c: e5841390     	str	r1, [r4, #0x390]
  397f10: e5843398     	str	r3, [r4, #0x398]
  397f14: e58433a0     	str	r3, [r4, #0x3a0]
  397f18: e1a00004     	mov	r0, r4
  397f1c: e8bd8070     	pop	{r4, r5, r6, pc}
  397f20: bc cb 5f 00  	.word	0x005fcbbc
  397f24: b4 30 00 00  	.word	0x000030b4


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00397f28 <ZoneEx::ZoneEx(ObjectBase::GO_IDS, bool, bool)>:
  397f28: e92d4070     	push	{r4, r5, r6, lr}
  397f2c: e59f5054     	ldr	r5, [pc, #0x54]         @ 0x397f88 <ZoneEx::ZoneEx(ObjectBase::GO_IDS, bool, bool)+0x60>
  397f30: e1a04000     	mov	r4, r0
  397f34: ebffff59     	bl	0x397ca0 <Zone::Zone(ObjectBase::GO_IDS, bool, bool)> @ imm = #-0x29c
  397f38: e59f204c     	ldr	r2, [pc, #0x4c]         @ 0x397f8c <ZoneEx::ZoneEx(ObjectBase::GO_IDS, bool, bool)+0x64>
  397f3c: e08f5005     	add	r5, pc, r5
  397f40: e3a03000     	mov	r3, #0
  397f44: e7952002     	ldr	r2, [r5, r2]
  397f48: e1a01004     	mov	r1, r4
  397f4c: e584338c     	str	r3, [r4, #0x38c]
  397f50: e28200f4     	add	r0, r2, #244
  397f54: e282c008     	add	r12, r2, #8
  397f58: e28220e8     	add	r2, r2, #232
  397f5c: e5840024     	str	r0, [r4, #0x24]
  397f60: e584c000     	str	r12, [r4]
  397f64: e5842004     	str	r2, [r4, #0x4]
  397f68: e5e13388     	strb	r3, [r1, #0x388]!
  397f6c: e5841394     	str	r1, [r4, #0x394]
  397f70: e58433a4     	str	r3, [r4, #0x3a4]
  397f74: e5841390     	str	r1, [r4, #0x390]
  397f78: e5843398     	str	r3, [r4, #0x398]
  397f7c: e58433a0     	str	r3, [r4, #0x3a0]
  397f80: e1a00004     	mov	r0, r4
  397f84: e8bd8070     	pop	{r4, r5, r6, pc}
  397f88: 54 cb 5f 00  	.word	0x005fcb54
  397f8c: b4 30 00 00  	.word	0x000030b4


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004d9c40 <Structs::TriggerTrap::~TriggerTrap()>:
  4d9c40: e92d4010     	push	{r4, lr}
  4d9c44: e1a04000     	mov	r4, r0
  4d9c48: ebffffec     	bl	0x4d9c00 <Structs::TriggerTrap::~TriggerTrap()> @ imm = #-0x50
  4d9c4c: e1a00004     	mov	r0, r4
  4d9c50: ebf8d9fa     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1c9818
  4d9c54: e1a00004     	mov	r0, r4
  4d9c58: e8bd8010     	pop	{r4, pc}


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004d9c00 <Structs::TriggerTrap::~TriggerTrap()>:
  4d9c00: e92d4010     	push	{r4, lr}
  4d9c04: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x4d9c38 <Structs::TriggerTrap::~TriggerTrap()+0x38>
  4d9c08: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x4d9c3c <Structs::TriggerTrap::~TriggerTrap()+0x3c>
  4d9c0c: e1a04000     	mov	r4, r0
  4d9c10: e08f3003     	add	r3, pc, r3
  4d9c14: e5900010     	ldr	r0, [r0, #0x10]
  4d9c18: e7932002     	ldr	r2, [r3, r2]
  4d9c1c: e3500000     	cmp	r0, #0
  4d9c20: e2822008     	add	r2, r2, #8
  4d9c24: e5842000     	str	r2, [r4]
  4d9c28: 0a000000     	beq	0x4d9c30 <Structs::TriggerTrap::~TriggerTrap()+0x30> @ imm = #0x0
  4d9c2c: ebf8da03     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1c97f4
  4d9c30: e1a00004     	mov	r0, r4
  4d9c34: e8bd8010     	pop	{r4, pc}
  4d9c38: 80 ae 4b 00  	.word	0x004bae80
  4d9c3c: 04 11 00 00  	.word	0x00001104


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004d9c5c <Structs::TriggerTrap::~TriggerTrap()>:
  4d9c5c: e92d4010     	push	{r4, lr}
  4d9c60: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x4d9c94 <Structs::TriggerTrap::~TriggerTrap()+0x38>
  4d9c64: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x4d9c98 <Structs::TriggerTrap::~TriggerTrap()+0x3c>
  4d9c68: e1a04000     	mov	r4, r0
  4d9c6c: e08f3003     	add	r3, pc, r3
  4d9c70: e5900010     	ldr	r0, [r0, #0x10]
  4d9c74: e7932002     	ldr	r2, [r3, r2]
  4d9c78: e3500000     	cmp	r0, #0
  4d9c7c: e2822008     	add	r2, r2, #8
  4d9c80: e5842000     	str	r2, [r4]
  4d9c84: 0a000000     	beq	0x4d9c8c <Structs::TriggerTrap::~TriggerTrap()+0x30> @ imm = #0x0
  4d9c88: ebf8d9ec     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1c9850
  4d9c8c: e1a00004     	mov	r0, r4
  4d9c90: e8bd8010     	pop	{r4, pc}
  4d9c94: 24 ae 4b 00  	.word	0x004bae24
  4d9c98: 04 11 00 00  	.word	0x00001104


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00396554 <RoomZone::DeclareProperties()>:
  396554: ea000627     	b	0x397df8 <Zone::DeclareProperties()> @ imm = #0x189c


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039e564 <non-virtual thunk to TriggerTrap::~TriggerTrap()>:
  39e564: e2400024     	sub	r0, r0, #36
  39e568: eaffffff     	b	0x39e56c <TriggerTrap::~TriggerTrap()> @ imm = #-0x4


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039e478 <non-virtual thunk to TriggerTrap::~TriggerTrap()>:
  39e478: e2400024     	sub	r0, r0, #36
  39e47c: eaffffff     	b	0x39e480 <TriggerTrap::~TriggerTrap()> @ imm = #-0x4


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039e8e4 <non-virtual thunk to TriggerTrap::DeclareProperties()>:
  39e8e4: e2400004     	sub	r0, r0, #4
  39e8e8: eaffffff     	b	0x39e8ec <TriggerTrap::DeclareProperties()> @ imm = #-0x4


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039bf9c <non-virtual thunk to TriggerZone::DeclareProperties()>:
  39bf9c: e2400004     	sub	r0, r0, #4
  39bfa0: eaffffff     	b	0x39bfa4 <TriggerZone::DeclareProperties()> @ imm = #-0x4


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00397df0 <non-virtual thunk to Zone::DeclareProperties()>:
  397df0: e2400004     	sub	r0, r0, #4
  397df4: eaffffff     	b	0x397df8 <Zone::DeclareProperties()> @ imm = #-0x4


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039654c <non-virtual thunk to RoomZone::DeclareProperties()>:
  39654c: e2400004     	sub	r0, r0, #4
  396550: eaffffff     	b	0x396554 <RoomZone::DeclareProperties()> @ imm = #-0x4
