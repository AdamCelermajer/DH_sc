
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00342600 <ObjectBase* GetNewInstance<AnimatedDecor>()>:
  342600: e92d4070     	push	{r4, r5, r6, lr}
  342604: e3a01000     	mov	r1, #0
  342608: e3a00fe5     	mov	r0, #916
  34260c: ebff37d7     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x320a4
  342610: e59f6070     	ldr	r6, [pc, #0x70]         @ 0x342688 <ObjectBase* GetNewInstance<AnimatedDecor>()+0x88>
  342614: e3a01014     	mov	r1, #20
  342618: e1a04000     	mov	r4, r0
  34261c: eb01275d     	bl	0x38c398 <GameObject::GameObject(ObjectBase::GO_IDS)> @ imm = #0x49d74
  342620: e59f3064     	ldr	r3, [pc, #0x64]         @ 0x34268c <ObjectBase* GetNewInstance<AnimatedDecor>()+0x8c>
  342624: e08f6006     	add	r6, pc, r6
  342628: e2842fdf     	add	r2, r4, #892
  34262c: e7963003     	ldr	r3, [r6, r3]
  342630: e3a05001     	mov	r5, #1
  342634: e1a00002     	mov	r0, r2
  342638: e283c008     	add	r12, r3, #8
  34263c: e28310e4     	add	r1, r3, #228
  342640: e28330d8     	add	r3, r3, #216
  342644: e5843004     	str	r3, [r4, #0x4]
  342648: e5841024     	str	r1, [r4, #0x24]
  34264c: e584238c     	str	r2, [r4, #0x38c]
  342650: e5842390     	str	r2, [r4, #0x390]
  342654: e584c000     	str	r12, [r4]
  342658: e5c45375     	strb	r5, [r4, #0x375]
  34265c: e5c45376     	strb	r5, [r4, #0x376]
  342660: e5c45084     	strb	r5, [r4, #0x84]
  342664: e3a01010     	mov	r1, #16
  342668: ebff3c03     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x30ff4
  34266c: e594238c     	ldr	r2, [r4, #0x38c]
  342670: e3a03000     	mov	r3, #0
  342674: e1a00004     	mov	r0, r4
  342678: e5c23000     	strb	r3, [r2]
  34267c: e5c45084     	strb	r5, [r4, #0x84]
  342680: e5c43375     	strb	r3, [r4, #0x375]
  342684: e8bd8070     	pop	{r4, r5, r6, pc}
  342688: 6c 24 65 00  	.word	0x0065246c
  34268c: 38 1d 00 00  	.word	0x00001d38


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00389128 <AnimatedDecor::InitPost()>:
  389128: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  38912c: e3a03001     	mov	r3, #1
  389130: e5c0310c     	strb	r3, [r0, #0x10c]
  389134: e24dd008     	sub	sp, sp, #8
  389138: e1a04000     	mov	r4, r0
  38913c: ebfffe55     	bl	0x388a98 <Decor::InitPost()> @ imm = #-0x6ac
  389140: e1a00004     	mov	r0, r4
  389144: eb000685     	bl	0x38ab60 <GameObject::MeetCondition() const> @ imm = #0x1a14
  389148: e59f51d8     	ldr	r5, [pc, #0x1d8]        @ 0x389328 <AnimatedDecor::InitPost()+0x200>
  38914c: e2501000     	subs	r1, r0, #0
  389150: e08f5005     	add	r5, pc, r5
  389154: 0a000058     	beq	0x3892bc <AnimatedDecor::InitPost()+0x194> @ imm = #0x160
  389158: e59482d8     	ldr	r8, [r4, #0x2d8]
  38915c: e3580000     	cmp	r8, #0
  389160: 0a000035     	beq	0x38923c <AnimatedDecor::InitPost()+0x114> @ imm = #0xd4
  389164: e5947390     	ldr	r7, [r4, #0x390]
  389168: e594338c     	ldr	r3, [r4, #0x38c]
  38916c: e1530007     	cmp	r3, r7
  389170: 0a000064     	beq	0x389308 <AnimatedDecor::InitPost()+0x1e0> @ imm = #0x190
  389174: e59f11b0     	ldr	r1, [pc, #0x1b0]        @ 0x38932c <AnimatedDecor::InitPost()+0x204>
  389178: e1a00007     	mov	r0, r7
  38917c: e08f1001     	add	r1, pc, r1
  389180: ebfe1558     	bl	0x30e6e8 <.plt+0x974>   @ imm = #-0x7aaa0
  389184: e2506000     	subs	r6, r0, #0
  389188: 0a00002d     	beq	0x389244 <AnimatedDecor::InitPost()+0x11c> @ imm = #0xb4
  38918c: e5983038     	ldr	r3, [r8, #0x38]
  389190: e1a01007     	mov	r1, r7
  389194: e3a02000     	mov	r2, #0
  389198: e1a00003     	mov	r0, r3
  38919c: e5933000     	ldr	r3, [r3]
  3891a0: e1a0e00f     	mov	lr, pc
  3891a4: e593f014     	ldr	pc, [r3, #0x14]
  3891a8: e3500000     	cmp	r0, #0
  3891ac: 1a000047     	bne	0x3892d0 <AnimatedDecor::InitPost()+0x1a8> @ imm = #0x11c
  3891b0: e59432d8     	ldr	r3, [r4, #0x2d8]
  3891b4: e3a0e000     	mov	lr, #0
  3891b8: e1a0100e     	mov	r1, lr
  3891bc: e593c038     	ldr	r12, [r3, #0x38]
  3891c0: e3a02001     	mov	r2, #1
  3891c4: e1a0300e     	mov	r3, lr
  3891c8: e1a0000c     	mov	r0, r12
  3891cc: e59cc000     	ldr	r12, [r12]
  3891d0: e58de000     	str	lr, [sp]
  3891d4: e1a0e00f     	mov	lr, pc
  3891d8: e59cf01c     	ldr	pc, [r12, #0x1c]
  3891dc: e59402d8     	ldr	r0, [r4, #0x2d8]
  3891e0: eb039e1b     	bl	0x470a54 <VisualObject::ApplyMeshBox()> @ imm = #0xe786c
  3891e4: e59432d8     	ldr	r3, [r4, #0x2d8]
  3891e8: e5d33028     	ldrb	r3, [r3, #0x28]
  3891ec: e3530000     	cmp	r3, #0
  3891f0: 0a00000d     	beq	0x38922c <AnimatedDecor::InitPost()+0x104> @ imm = #0x34
  3891f4: e59f3134     	ldr	r3, [pc, #0x134]        @ 0x389330 <AnimatedDecor::InitPost()+0x208>
  3891f8: e3a01000     	mov	r1, #0
  3891fc: e3a00028     	mov	r0, #40
  389200: e7953003     	ldr	r3, [r5, r3]
  389204: e5936044     	ldr	r6, [r3, #0x44]
  389208: ebfe1cd8     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x78ca0
  38920c: e1a01006     	mov	r1, r6
  389210: e1a05000     	mov	r5, r0
  389214: e1a02004     	mov	r2, r4
  389218: ebfffe03     	bl	0x388a2c <PODecor::PODecor(PhysicalWorld*, GameObject*, bool) (.clone.2)> @ imm = #-0x7f4
  38921c: e1a00004     	mov	r0, r4
  389220: e1a01005     	mov	r1, r5
  389224: e3a02000     	mov	r2, #0
  389228: eb002e72     	bl	0x394bf8 <GameObject::SetPhysicalObject(PhysicalObject*, bool)> @ imm = #0xb9c8
  38922c: e1a00004     	mov	r0, r4
  389230: e5943000     	ldr	r3, [r4]
  389234: e1a0e00f     	mov	lr, pc
  389238: e593f02c     	ldr	pc, [r3, #0x2c]
  38923c: e28dd008     	add	sp, sp, #8
  389240: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  389244: e5983038     	ldr	r3, [r8, #0x38]
  389248: e1a01006     	mov	r1, r6
  38924c: e1a00003     	mov	r0, r3
  389250: e5933000     	ldr	r3, [r3]
  389254: e1a0e00f     	mov	lr, pc
  389258: e593f010     	ldr	pc, [r3, #0x10]
  38925c: e2400001     	sub	r0, r0, #1
  389260: ebfffe7c     	bl	0x388c58 <Random::GetRandom(int, bool) (.clone.3)> @ imm = #-0x610
  389264: e59432d8     	ldr	r3, [r4, #0x2d8]
  389268: e1a01000     	mov	r1, r0
  38926c: e1a02006     	mov	r2, r6
  389270: e593c038     	ldr	r12, [r3, #0x38]
  389274: e1a03006     	mov	r3, r6
  389278: e1a0000c     	mov	r0, r12
  38927c: e59cc000     	ldr	r12, [r12]
  389280: e58d6000     	str	r6, [sp]
  389284: e1a0e00f     	mov	lr, pc
  389288: e59cf01c     	ldr	pc, [r12, #0x1c]
  38928c: e59422d8     	ldr	r2, [r4, #0x2d8]
  389290: e1a03006     	mov	r3, r6
  389294: e592c038     	ldr	r12, [r2, #0x38]
  389298: e59f2094     	ldr	r2, [pc, #0x94]         @ 0x389334 <AnimatedDecor::InitPost()+0x20c>
  38929c: e1a0000c     	mov	r0, r12
  3892a0: e7951002     	ldr	r1, [r5, r2]
  3892a4: e59cc000     	ldr	r12, [r12]
  3892a8: e1a02004     	mov	r2, r4
  3892ac: e58d4000     	str	r4, [sp]
  3892b0: e1a0e00f     	mov	lr, pc
  3892b4: e59cf02c     	ldr	pc, [r12, #0x2c]
  3892b8: eaffffc7     	b	0x3891dc <AnimatedDecor::InitPost()+0xb4> @ imm = #-0xe4
  3892bc: e1a00004     	mov	r0, r4
  3892c0: e5943000     	ldr	r3, [r4]
  3892c4: e1a0e00f     	mov	lr, pc
  3892c8: e593f040     	ldr	pc, [r3, #0x40]
  3892cc: eaffffda     	b	0x38923c <AnimatedDecor::InitPost()+0x114> @ imm = #-0x98
  3892d0: e59432d8     	ldr	r3, [r4, #0x2d8]
  3892d4: e3a02000     	mov	r2, #0
  3892d8: e5941390     	ldr	r1, [r4, #0x390]
  3892dc: e593c038     	ldr	r12, [r3, #0x38]
  3892e0: e1a03002     	mov	r3, r2
  3892e4: e1a0000c     	mov	r0, r12
  3892e8: e59cc000     	ldr	r12, [r12]
  3892ec: e58d2000     	str	r2, [sp]
  3892f0: e3a02001     	mov	r2, #1
  3892f4: e1a0e00f     	mov	lr, pc
  3892f8: e59cf020     	ldr	pc, [r12, #0x20]
  3892fc: e3500000     	cmp	r0, #0
  389300: 1affffb5     	bne	0x3891dc <AnimatedDecor::InitPost()+0xb4> @ imm = #-0x12c
  389304: eaffffa9     	b	0x3891b0 <AnimatedDecor::InitPost()+0x88> @ imm = #-0x15c
  389308: e59f1028     	ldr	r1, [pc, #0x28]         @ 0x389338 <AnimatedDecor::InitPost()+0x210>
  38930c: e2840fdf     	add	r0, r4, #892
  389310: e08f1001     	add	r1, pc, r1
  389314: e2812004     	add	r2, r1, #4
  389318: ebfe1db0     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x78940
  38931c: e59482d8     	ldr	r8, [r4, #0x2d8]
  389320: e5947390     	ldr	r7, [r4, #0x390]
  389324: eaffff92     	b	0x389174 <AnimatedDecor::InitPost()+0x4c> @ imm = #-0x1b8
  389328: 40 b9 60 00  	.word	0x0060b940
  38932c: 3c 91 53 00  	.word	0x0053913c
  389330: f4 37 00 00  	.word	0x000037f4
  389334: d0 38 00 00  	.word	0x000038d0
  389338: a0 8f 53 00  	.word	0x00538fa0


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00389dd4 <AnimatedDecor::DeclareProperties()>:
  389dd4: e92d4010     	push	{r4, lr}
  389dd8: e1a04000     	mov	r4, r0
  389ddc: ebfffefb     	bl	0x3899d0 <Decor::DeclareProperties()> @ imm = #-0x414
  389de0: e59f1010     	ldr	r1, [pc, #0x10]         @ 0x389df8 <AnimatedDecor::DeclareProperties()+0x24>
  389de4: e2842fdf     	add	r2, r4, #892
  389de8: e2840004     	add	r0, r4, #4
  389dec: e08f1001     	add	r1, pc, r1
  389df0: e8bd4010     	pop	{r4, lr}
  389df4: eafed460     	b	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x4ae80
  389df8: 14 85 53 00  	.word	0x00538514


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00388cec <AnimatedDecor::__CallbackRandomAll(glitch::scene::ITimelineController*, void*)>:
  388cec: e92d4030     	push	{r4, r5, lr}
  388cf0: e59132d8     	ldr	r3, [r1, #0x2d8]
  388cf4: e24dd00c     	sub	sp, sp, #12
  388cf8: e1a05001     	mov	r5, r1
  388cfc: e5933038     	ldr	r3, [r3, #0x38]
  388d00: e3a01000     	mov	r1, #0
  388d04: e59f40a8     	ldr	r4, [pc, #0xa8]         @ 0x388db4 <AnimatedDecor::__CallbackRandomAll(glitch::scene::ITimelineController*, void*)+0xc8>
  388d08: e1a00003     	mov	r0, r3
  388d0c: e5933000     	ldr	r3, [r3]
  388d10: e1a0e00f     	mov	lr, pc
  388d14: e593f010     	ldr	pc, [r3, #0x10]
  388d18: e2400001     	sub	r0, r0, #1
  388d1c: ebffffcd     	bl	0x388c58 <Random::GetRandom(int, bool) (.clone.3)> @ imm = #-0xcc
  388d20: e59532d8     	ldr	r3, [r5, #0x2d8]
  388d24: e3a0e000     	mov	lr, #0
  388d28: e1a01000     	mov	r1, r0
  388d2c: e593c038     	ldr	r12, [r3, #0x38]
  388d30: e1a0200e     	mov	r2, lr
  388d34: e1a0300e     	mov	r3, lr
  388d38: e1a0000c     	mov	r0, r12
  388d3c: e59cc000     	ldr	r12, [r12]
  388d40: e58de000     	str	lr, [sp]
  388d44: e1a0e00f     	mov	lr, pc
  388d48: e59cf01c     	ldr	pc, [r12, #0x1c]
  388d4c: e3500000     	cmp	r0, #0
  388d50: e08f4004     	add	r4, pc, r4
  388d54: 1a000007     	bne	0x388d78 <AnimatedDecor::__CallbackRandomAll(glitch::scene::ITimelineController*, void*)+0x8c> @ imm = #0x1c
  388d58: e59f3058     	ldr	r3, [pc, #0x58]         @ 0x388db8 <AnimatedDecor::__CallbackRandomAll(glitch::scene::ITimelineController*, void*)+0xcc>
  388d5c: e7943003     	ldr	r3, [r4, r3]
  388d60: e5933000     	ldr	r3, [r3]
  388d64: e3530002     	cmp	r3, #2
  388d68: 05800000     	streq	r0, [r0]
  388d6c: 0a000001     	beq	0x388d78 <AnimatedDecor::__CallbackRandomAll(glitch::scene::ITimelineController*, void*)+0x8c> @ imm = #0x4
  388d70: e3530001     	cmp	r3, #1
  388d74: 0a000001     	beq	0x388d80 <AnimatedDecor::__CallbackRandomAll(glitch::scene::ITimelineController*, void*)+0x94> @ imm = #0x4
  388d78: e28dd00c     	add	sp, sp, #12
  388d7c: e8bd8030     	pop	{r4, r5, pc}
  388d80: e59f0034     	ldr	r0, [pc, #0x34]         @ 0x388dbc <AnimatedDecor::__CallbackRandomAll(glitch::scene::ITimelineController*, void*)+0xd0>
  388d84: e59f1034     	ldr	r1, [pc, #0x34]         @ 0x388dc0 <AnimatedDecor::__CallbackRandomAll(glitch::scene::ITimelineController*, void*)+0xd4>
  388d88: e59f2034     	ldr	r2, [pc, #0x34]         @ 0x388dc4 <AnimatedDecor::__CallbackRandomAll(glitch::scene::ITimelineController*, void*)+0xd8>
  388d8c: e7940000     	ldr	r0, [r4, r0]
  388d90: e59f3030     	ldr	r3, [pc, #0x30]         @ 0x388dc8 <AnimatedDecor::__CallbackRandomAll(glitch::scene::ITimelineController*, void*)+0xdc>
  388d94: e300c159     	movw	r12, #0x159
  388d98: e08f1001     	add	r1, pc, r1
  388d9c: e08f2002     	add	r2, pc, r2
  388da0: e08f3003     	add	r3, pc, r3
  388da4: e28000a8     	add	r0, r0, #168
  388da8: e58dc000     	str	r12, [sp]
  388dac: ebfe1494     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x7adb0
  388db0: eafffff0     	b	0x388d78 <AnimatedDecor::__CallbackRandomAll(glitch::scene::ITimelineController*, void*)+0x8c> @ imm = #-0x40
  388db4: 40 bd 60 00  	.word	0x0060bd40
  388db8: c0 39 00 00  	.word	0x000039c0
  388dbc: c0 19 00 00  	.word	0x000019c0
  388dc0: 40 56 53 00  	.word	0x00535640
  388dc4: c4 94 53 00  	.word	0x005394c4
  388dc8: d0 94 53 00  	.word	0x005394d0


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00389098 <AnimatedDecor::~AnimatedDecor()>:
  389098: e92d4070     	push	{r4, r5, r6, lr}
  38909c: e59f5054     	ldr	r5, [pc, #0x54]         @ 0x3890f8 <AnimatedDecor::~AnimatedDecor()+0x60>
  3890a0: e59f3054     	ldr	r3, [pc, #0x54]         @ 0x3890fc <AnimatedDecor::~AnimatedDecor()+0x64>
  3890a4: e1a04000     	mov	r4, r0
  3890a8: e08f5005     	add	r5, pc, r5
  3890ac: e7953003     	ldr	r3, [r5, r3]
  3890b0: e2800fdf     	add	r0, r0, #892
  3890b4: e28320e4     	add	r2, r3, #228
  3890b8: e2831008     	add	r1, r3, #8
  3890bc: e28330d8     	add	r3, r3, #216
  3890c0: e884000a     	stm	r4, {r1, r3}
  3890c4: e5842024     	str	r2, [r4, #0x24]
  3890c8: ebfe2a37     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x75724
  3890cc: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x389100 <AnimatedDecor::~AnimatedDecor()+0x68>
  3890d0: e1a00004     	mov	r0, r4
  3890d4: e7953003     	ldr	r3, [r5, r3]
  3890d8: e28320e4     	add	r2, r3, #228
  3890dc: e2831008     	add	r1, r3, #8
  3890e0: e28330d8     	add	r3, r3, #216
  3890e4: e884000a     	stm	r4, {r1, r3}
  3890e8: e5842024     	str	r2, [r4, #0x24]
  3890ec: eb0010a1     	bl	0x38d378 <GameObject::~GameObject()> @ imm = #0x4284
  3890f0: e1a00004     	mov	r0, r4
  3890f4: e8bd8070     	pop	{r4, r5, r6, pc}
  3890f8: e8 b9 60 00  	.word	0x0060b9e8
  3890fc: 38 1d 00 00  	.word	0x00001d38
  389100: 0c 2b 00 00  	.word	0x00002b0c
