
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00359338 <SceneManager::drawAll(glitch::scene::ISceneNode*)>:
  359338: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  35933c: e59f414c     	ldr	r4, [pc, #0x14c]        @ 0x359490 <SceneManager::drawAll(glitch::scene::ISceneNode*)+0x158>
  359340: e59f514c     	ldr	r5, [pc, #0x14c]        @ 0x359494 <SceneManager::drawAll(glitch::scene::ISceneNode*)+0x15c>
  359344: e24dd08c     	sub	sp, sp, #140
  359348: e08f4004     	add	r4, pc, r4
  35934c: e7943005     	ldr	r3, [r4, r5]
  359350: e1a06000     	mov	r6, r0
  359354: e1a07001     	mov	r7, r1
  359358: e5933000     	ldr	r3, [r3]
  35935c: e2800fa5     	add	r0, r0, #660
  359360: e58d3084     	str	r3, [sp, #0x84]
  359364: eb02d08d     	bl	0x40d5a0 <LightSetManager::Update()> @ imm = #0xb4234
  359368: e1a00006     	mov	r0, r6
  35936c: e1a01007     	mov	r1, r7
  359370: ebffffce     	bl	0x3592b0 <SceneManager::_drawAll(glitch::scene::ISceneNode*)> @ imm = #-0xc8
  359374: e5d63290     	ldrb	r3, [r6, #0x290]
  359378: e3530000     	cmp	r3, #0
  35937c: 1a000006     	bne	0x35939c <SceneManager::drawAll(glitch::scene::ISceneNode*)+0x64> @ imm = #0x18
  359380: e7943005     	ldr	r3, [r4, r5]
  359384: e59d2084     	ldr	r2, [sp, #0x84]
  359388: e5933000     	ldr	r3, [r3]
  35938c: e1520003     	cmp	r2, r3
  359390: 1a00003d     	bne	0x35948c <SceneManager::drawAll(glitch::scene::ISceneNode*)+0x154> @ imm = #0xf4
  359394: e28dd08c     	add	sp, sp, #140
  359398: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  35939c: e59f30f4     	ldr	r3, [pc, #0xf4]         @ 0x359498 <SceneManager::drawAll(glitch::scene::ISceneNode*)+0x160>
  3593a0: e3a08000     	mov	r8, #0
  3593a4: e5c68290     	strb	r8, [r6, #0x290]
  3593a8: e794a003     	ldr	r10, [r4, r3]
  3593ac: e28d706c     	add	r7, sp, #108
  3593b0: e1a0000a     	mov	r0, r10
  3593b4: ebff7933     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0x21b34
  3593b8: e59f10dc     	ldr	r1, [pc, #0xdc]         @ 0x35949c <SceneManager::drawAll(glitch::scene::ISceneNode*)+0x164>
  3593bc: e28d2004     	add	r2, sp, #4
  3593c0: e1a00007     	mov	r0, r7
  3593c4: e08f1001     	add	r1, pc, r1
  3593c8: ebfeeb47     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x452e4
  3593cc: e1a0000a     	mov	r0, r10
  3593d0: e1a01007     	mov	r1, r7
  3593d4: ebff79ab     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0x21954
  3593d8: e1a0a000     	mov	r10, r0
  3593dc: e1a00007     	mov	r0, r7
  3593e0: ebfefb9b     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x41194
  3593e4: e15a0008     	cmp	r10, r8
  3593e8: 0affffe4     	beq	0x359380 <SceneManager::drawAll(glitch::scene::ISceneNode*)+0x48> @ imm = #-0x70
  3593ec: e1a00008     	mov	r0, r8
  3593f0: ebfed462     	bl	0x30e580 <.plt+0x80c>   @ imm = #-0x4ae78
  3593f4: e59f10a4     	ldr	r1, [pc, #0xa4]         @ 0x3594a0 <SceneManager::drawAll(glitch::scene::ISceneNode*)+0x168>
  3593f8: e28d7008     	add	r7, sp, #8
  3593fc: e1a02000     	mov	r2, r0
  359400: e08f1001     	add	r1, pc, r1
  359404: e1a00007     	mov	r0, r7
  359408: ebfed5b5     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x4a92c
  35940c: e5963014     	ldr	r3, [r6, #0x14]
  359410: e1a0000d     	mov	r0, sp
  359414: e1a0a00d     	mov	r10, sp
  359418: e1a01003     	mov	r1, r3
  35941c: e5933000     	ldr	r3, [r3]
  359420: e1a0e00f     	mov	lr, pc
  359424: e593f098     	ldr	pc, [r3, #0x98]
  359428: e1a00007     	mov	r0, r7
  35942c: e1a01008     	mov	r1, r8
  359430: eb085def     	bl	0x570bf4 <glitch::io::createWriteFile(char const*, bool)> @ imm = #0x2177bc
  359434: e1a01008     	mov	r1, r8
  359438: e1a07000     	mov	r7, r0
  35943c: e3a00008     	mov	r0, #8
  359440: eb076b59     	bl	0x5341ac <operator new(unsigned int, glitch::memory::E_MEMORY_HINT)> @ imm = #0x1dad64
  359444: e1a06000     	mov	r6, r0
  359448: eb0ab7af     	bl	0x60730c <glitch::video::CImageWriterTGA::CImageWriterTGA()> @ imm = #0x2adebc
  35944c: e1a0200d     	mov	r2, sp
  359450: e1a03008     	mov	r3, r8
  359454: e1a01007     	mov	r1, r7
  359458: e596c000     	ldr	r12, [r6]
  35945c: e1a00006     	mov	r0, r6
  359460: e1a0e00f     	mov	lr, pc
  359464: e59cf010     	ldr	pc, [r12, #0x10]
  359468: e1a00006     	mov	r0, r6
  35946c: ebff1044     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x3bef0
  359470: e1a00007     	mov	r0, r7
  359474: ebff1042     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x3bef8
  359478: e59d0000     	ldr	r0, [sp]
  35947c: e1500008     	cmp	r0, r8
  359480: 0affffbe     	beq	0x359380 <SceneManager::drawAll(glitch::scene::ISceneNode*)+0x48> @ imm = #-0x108
  359484: ebff103e     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x3bf08
  359488: eaffffbc     	b	0x359380 <SceneManager::drawAll(glitch::scene::ISceneNode*)+0x48> @ imm = #-0x110
  35948c: ebfed39f     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x4b184
  359490: 48 b7 63 00  	.word	0x0063b748
  359494: ac 40 00 00  	.word	0x000040ac
  359498: 84 08 00 00  	.word	0x00000884
  35949c: 0c 67 56 00  	.word	0x0056670c
  3594a0: c0 77 56 00  	.word	0x005677c0
