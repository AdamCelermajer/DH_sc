
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0031f594 <Application::GetCurrentLevel() const>:
  31f594: e59f3010     	ldr	r3, [pc, #0x10]         @ 0x31f5ac <Application::GetCurrentLevel() const+0x18>
  31f598: e59f2010     	ldr	r2, [pc, #0x10]         @ 0x31f5b0 <Application::GetCurrentLevel() const+0x1c>
  31f59c: e08f3003     	add	r3, pc, r3
  31f5a0: e7932002     	ldr	r2, [r3, r2]
  31f5a4: e5920000     	ldr	r0, [r2]
  31f5a8: e12fff1e     	bx	lr
  31f5ac: f4 54 67 00  	.word	0x006754f4
  31f5b0: 64 1d 00 00  	.word	0x00001d64


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00386040 <GSLevel::Resume(StateMachine const*)>:
  3860b0: e8bd8010     	pop	{r4, pc}
  3860b4: 44 ea 60 00  	.word	0x0060ea44
  3860b8: f4 37 00 00  	.word	0x000037f4

003860bc <GSLevel::Dtor(StateMachine const*)>:
  3860bc: e92d4070     	push	{r4, r5, r6, lr}
  3860c0: e1a05000     	mov	r5, r0
  3860c4: e5900034     	ldr	r0, [r0, #0x34]
  3860c8: e59f4068     	ldr	r4, [pc, #0x68]         @ 0x386138 <GSLevel::Dtor(StateMachine const*)+0x7c>
  3860cc: e3500000     	cmp	r0, #0
  3860d0: e08f4004     	add	r4, pc, r4
  3860d4: 0a000012     	beq	0x386124 <GSLevel::Dtor(StateMachine const*)+0x68> @ imm = #0x48
  3860d8: eb01a96c     	bl	0x3f0690 <Level::Unload()> @ imm = #0x6a5b0
  3860dc: eb029a6a     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0xa69a8
  3860e0: e59f1054     	ldr	r1, [pc, #0x54]         @ 0x38613c <GSLevel::Dtor(StateMachine const*)+0x80>
  3860e4: e08f1001     	add	r1, pc, r1
  3860e8: eb029c40     	bl	0x42d1f0 <MenuManager::GetMenuByName(char const*)> @ imm = #0xa7100
  3860ec: e2503000     	subs	r3, r0, #0
  3860f0: 0a000002     	beq	0x386100 <GSLevel::Dtor(StateMachine const*)+0x44> @ imm = #0x8
  3860f4: e5933000     	ldr	r3, [r3]
  3860f8: e1a0e00f     	mov	lr, pc
  3860fc: e593f010     	ldr	pc, [r3, #0x10]
  386100: e5953034     	ldr	r3, [r5, #0x34]
  386104: e3530000     	cmp	r3, #0
  386108: 0a000005     	beq	0x386124 <GSLevel::Dtor(StateMachine const*)+0x68> @ imm = #0x14
  38610c: e1a00003     	mov	r0, r3
  386110: e5933000     	ldr	r3, [r3]
  386114: e1a0e00f     	mov	lr, pc
  386118: e593f004     	ldr	pc, [r3, #0x4]
  38611c: e3a03000     	mov	r3, #0
  386120: e5853034     	str	r3, [r5, #0x34]
  386124: e59f3014     	ldr	r3, [pc, #0x14]         @ 0x386140 <GSLevel::Dtor(StateMachine const*)+0x84>
  386128: e3a02000     	mov	r2, #0
  38612c: e7943003     	ldr	r3, [r4, r3]
  386130: e5832000     	str	r2, [r3]
  386134: e8bd8070     	pop	{r4, r5, r6, pc}
  386138: c0 e9 60 00  	.word	0x0060e9c0
  38613c: c4 b6 53 00  	.word	0x0053b6c4
  386140: 64 1d 00 00  	.word	0x00001d64


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
