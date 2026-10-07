
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00386190 <GSLevel::Ctor(StateMachine const*)>:
  386190: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  386194: e59f5128     	ldr	r5, [pc, #0x128]        @ 0x3862c4 <GSLevel::Ctor(StateMachine const*)+0x134>
  386198: e59f3128     	ldr	r3, [pc, #0x128]        @ 0x3862c8 <GSLevel::Ctor(StateMachine const*)+0x138>
  38619c: e1a04000     	mov	r4, r0
  3861a0: e08f5005     	add	r5, pc, r5
  3861a4: e24dd01c     	sub	sp, sp, #28
  3861a8: e7950003     	ldr	r0, [r5, r3]
  3861ac: eb03bd2c     	bl	0x475664 <AnimSetManager::Flush()> @ imm = #0xef4b0
  3861b0: e3a01000     	mov	r1, #0
  3861b4: e3a00f6b     	mov	r0, #428
  3861b8: e5949018     	ldr	r9, [r4, #0x18]
  3861bc: ebfe28eb     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x75c54
  3861c0: e5d4e02c     	ldrb	lr, [r4, #0x2c]
  3861c4: e5d4c02d     	ldrb	r12, [r4, #0x2d]
  3861c8: e594b024     	ldr	r11, [r4, #0x24]
  3861cc: e5947028     	ldr	r7, [r4, #0x28]
  3861d0: e5948030     	ldr	r8, [r4, #0x30]
  3861d4: e594a040     	ldr	r10, [r4, #0x40]
  3861d8: e594201c     	ldr	r2, [r4, #0x1c]
  3861dc: e5943020     	ldr	r3, [r4, #0x20]
  3861e0: e1a01009     	mov	r1, r9
  3861e4: e58de008     	str	lr, [sp, #0x8]
  3861e8: e58dc00c     	str	r12, [sp, #0xc]
  3861ec: e1a06000     	mov	r6, r0
  3861f0: e58db000     	str	r11, [sp]
  3861f4: e58d7004     	str	r7, [sp, #0x4]
  3861f8: e58d8010     	str	r8, [sp, #0x10]
  3861fc: e58da014     	str	r10, [sp, #0x14]
  386200: eb01b3c8     	bl	0x3f3128 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)> @ imm = #0x6cf20
  386204: e59f20c0     	ldr	r2, [pc, #0xc0]         @ 0x3862cc <GSLevel::Ctor(StateMachine const*)+0x13c>
  386208: e3a03001     	mov	r3, #1
  38620c: e5843038     	str	r3, [r4, #0x38]
  386210: e7952002     	ldr	r2, [r5, r2]
  386214: e5846034     	str	r6, [r4, #0x34]
  386218: e5826000     	str	r6, [r2]
  38621c: e5c4303c     	strb	r3, [r4, #0x3c]
  386220: eb029a19     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0xa6864
  386224: e59f10a4     	ldr	r1, [pc, #0xa4]         @ 0x3862d0 <GSLevel::Ctor(StateMachine const*)+0x140>
  386228: e08f1001     	add	r1, pc, r1
  38622c: eb029bef     	bl	0x42d1f0 <MenuManager::GetMenuByName(char const*)> @ imm = #0xa6fbc
  386230: e1a04000     	mov	r4, r0
  386234: eb11dd56     	bl	0x7fd794 <GetOnline()>  @ imm = #0x477558
  386238: e5d03005     	ldrb	r3, [r0, #0x5]
  38623c: e3530000     	cmp	r3, #0
  386240: 1a000011     	bne	0x38628c <GSLevel::Ctor(StateMachine const*)+0xfc> @ imm = #0x44
  386244: e3540000     	cmp	r4, #0
  386248: 0a00000d     	beq	0x386284 <GSLevel::Ctor(StateMachine const*)+0xf4> @ imm = #0x34
  38624c: eb029a0e     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0xa6838
  386250: e1a01004     	mov	r1, r4
  386254: eb02ad63     	bl	0x4317e8 <MenuManager::PushMenu(MenuBase*)> @ imm = #0xab58c
  386258: e2840048     	add	r0, r4, #72
  38625c: e5945004     	ldr	r5, [r4, #0x4]
  386260: ebffffb7     	bl	0x386144 <gameswf::weak_ptr<gameswf::character>::check_proxy() const> @ imm = #-0x124
  386264: e59f2068     	ldr	r2, [pc, #0x68]         @ 0x3862d4 <GSLevel::Ctor(StateMachine const*)+0x144>
  386268: e3a0c000     	mov	r12, #0
  38626c: e594104c     	ldr	r1, [r4, #0x4c]
  386270: e1a00005     	mov	r0, r5
  386274: e08f2002     	add	r2, pc, r2
  386278: e1a0300c     	mov	r3, r12
  38627c: e58dc000     	str	r12, [sp]
  386280: eb1096e1     	bl	0x7abe0c <RenderFX::InvokeASCallback(gameswf::character*, char const*, gameswf::as_value const*, int)> @ imm = #0x425b84
  386284: e28dd01c     	add	sp, sp, #28
  386288: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  38628c: ebfe6b01     	bl	0x320e98 <OnlineSingleton<OnlineGameState>::GetInstance()> @ imm = #-0x653fc
  386290: e5903034     	ldr	r3, [r0, #0x34]
  386294: e3530003     	cmp	r3, #3
  386298: 1affffe9     	bne	0x386244 <GSLevel::Ctor(StateMachine const*)+0xb4> @ imm = #-0x5c
  38629c: eb0299fa     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0xa67e8
  3862a0: e59030f4     	ldr	r3, [r0, #0xf4]
  3862a4: e1a01004     	mov	r1, r4
  3862a8: e1a00003     	mov	r0, r3
  3862ac: e5933000     	ldr	r3, [r3]
  3862b0: e1a0e00f     	mov	lr, pc
  3862b4: e593f040     	ldr	pc, [r3, #0x40]
  3862b8: e3500000     	cmp	r0, #0
  3862bc: 1afffff0     	bne	0x386284 <GSLevel::Ctor(StateMachine const*)+0xf4> @ imm = #-0x40
  3862c0: eaffffdf     	b	0x386244 <GSLevel::Ctor(StateMachine const*)+0xb4> @ imm = #-0x84
  3862c4: f0 e8 60 00  	.word	0x0060e8f0
  3862c8: 38 48 00 00  	.word	0x00004838
  3862cc: 64 1d 00 00  	.word	0x00001d64
  3862d0: a8 bd 53 00  	.word	0x0053bda8
  3862d4: 6c bd 53 00  	.word	0x0053bd6c
