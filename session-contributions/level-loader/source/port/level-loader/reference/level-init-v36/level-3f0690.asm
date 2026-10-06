
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f0690 <Level::Unload()>:
  3f0690: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3f0694: e5d010f0     	ldrb	r1, [r0, #0xf0]
  3f0698: e1a04000     	mov	r4, r0
  3f069c: ebffffbe     	bl	0x3f059c <Level::QuickSave(bool)> @ imm = #-0x108
  3f06a0: eb00f0f9     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #0x3c3e4
  3f06a4: e59f10ec     	ldr	r1, [pc, #0xec]         @ 0x3f0798 <Level::Unload()+0x108>
  3f06a8: e1a07000     	mov	r7, r0
  3f06ac: e59f50e8     	ldr	r5, [pc, #0xe8]         @ 0x3f079c <Level::Unload()+0x10c>
  3f06b0: e08f1001     	add	r1, pc, r1
  3f06b4: eb00f2cd     	bl	0x42d1f0 <MenuManager::GetMenuByName(char const*)> @ imm = #0x3cb34
  3f06b8: e1a06000     	mov	r6, r0
  3f06bc: e1a01006     	mov	r1, r6
  3f06c0: e1a00007     	mov	r0, r7
  3f06c4: eb010447     	bl	0x4317e8 <MenuManager::PushMenu(MenuBase*)> @ imm = #0x4111c
  3f06c8: e5943128     	ldr	r3, [r4, #0x128]
  3f06cc: e08f5005     	add	r5, pc, r5
  3f06d0: e3530000     	cmp	r3, #0
  3f06d4: 0a000005     	beq	0x3f06f0 <Level::Unload()+0x60> @ imm = #0x14
  3f06d8: e1a00003     	mov	r0, r3
  3f06dc: e5933000     	ldr	r3, [r3]
  3f06e0: e1a0e00f     	mov	lr, pc
  3f06e4: e593f004     	ldr	pc, [r3, #0x4]
  3f06e8: e3a03000     	mov	r3, #0
  3f06ec: e5843128     	str	r3, [r4, #0x128]
  3f06f0: e594312c     	ldr	r3, [r4, #0x12c]
  3f06f4: e3530000     	cmp	r3, #0
  3f06f8: 0a000005     	beq	0x3f0714 <Level::Unload()+0x84> @ imm = #0x14
  3f06fc: e1a00003     	mov	r0, r3
  3f0700: e5933000     	ldr	r3, [r3]
  3f0704: e1a0e00f     	mov	lr, pc
  3f0708: e593f004     	ldr	pc, [r3, #0x4]
  3f070c: e3a03000     	mov	r3, #0
  3f0710: e584312c     	str	r3, [r4, #0x12c]
  3f0714: e1a00004     	mov	r0, r4
  3f0718: e5d410f0     	ldrb	r1, [r4, #0xf0]
  3f071c: ebfffd47     	bl	0x3efc40 <Level::SG_SaveAllPlayer(bool)> @ imm = #-0xae4
  3f0720: e59f3078     	ldr	r3, [pc, #0x78]         @ 0x3f07a0 <Level::Unload()+0x110>
  3f0724: e3a01f7d     	mov	r1, #500
  3f0728: e7953003     	ldr	r3, [r5, r3]
  3f072c: e5930000     	ldr	r0, [r3]
  3f0730: ebfde496     	bl	0x369990 <VoxSoundManager::StopAllSounds(int)> @ imm = #-0x86da8
  3f0734: e1a00006     	mov	r0, r6
  3f0738: eb00bb2d     	bl	0x41f3f4 <MenuBase::IsVisible() const> @ imm = #0x2ecb4
  3f073c: e3500000     	cmp	r0, #0
  3f0740: 0a000002     	beq	0x3f0750 <Level::Unload()+0xc0> @ imm = #0x8
  3f0744: e1a00007     	mov	r0, r7
  3f0748: e1a01006     	mov	r1, r6
  3f074c: eb00f6ad     	bl	0x42e208 <MenuManager::PopMenu(MenuBase*)> @ imm = #0x3dab4
  3f0750: e59f304c     	ldr	r3, [pc, #0x4c]         @ 0x3f07a4 <Level::Unload()+0x114>
  3f0754: e3a06000     	mov	r6, #0
  3f0758: e5846130     	str	r6, [r4, #0x130]
  3f075c: e7954003     	ldr	r4, [r5, r3]
  3f0760: e5940038     	ldr	r0, [r4, #0x38]
  3f0764: ebfd4116     	bl	0x340bc4 <ObjectManager::NetworkUnInitLevel()> @ imm = #-0xafba8
  3f0768: e5943040     	ldr	r3, [r4, #0x40]
  3f076c: e1a00003     	mov	r0, r3
  3f0770: e5c366c9     	strb	r6, [r3, #0x6c9]
  3f0774: ebfe220e     	bl	0x378fb4 <PlayerManager::Update()> @ imm = #-0x777c8
  3f0778: eb086a53     	bl	0x60b0cc <glitch::os::Timer::getRealTime()> @ imm = #0x21a94c
  3f077c: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x3f07a8 <Level::Unload()+0x118>
  3f0780: e7952003     	ldr	r2, [r5, r3]
  3f0784: e59f3020     	ldr	r3, [pc, #0x20]         @ 0x3f07ac <Level::Unload()+0x11c>
  3f0788: e5820000     	str	r0, [r2]
  3f078c: e7953003     	ldr	r3, [r5, r3]
  3f0790: e5836000     	str	r6, [r3]
  3f0794: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3f0798: 20 19 4d 00  	.word	0x004d1920
  3f079c: c4 43 5a 00  	.word	0x005a43c4
  3f07a0: a4 0d 00 00  	.word	0x00000da4
  3f07a4: f4 37 00 00  	.word	0x000037f4
  3f07a8: 94 0c 00 00  	.word	0x00000c94
  3f07ac: 10 0b 00 00  	.word	0x00000b10
