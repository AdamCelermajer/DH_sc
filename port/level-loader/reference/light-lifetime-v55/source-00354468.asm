
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00354468 <SceneManager::clear()>:
  354468: e92d40f0     	push	{r4, r5, r6, r7, lr}
  35446c: e5902450     	ldr	r2, [r0, #0x450]
  354470: e590344c     	ldr	r3, [r0, #0x44c]
  354474: e59f5104     	ldr	r5, [pc, #0x104]        @ 0x354580 <SceneManager::clear()+0x118>
  354478: e24ddf6b     	sub	sp, sp, #428
  35447c: e0637002     	rsb	r7, r3, r2
  354480: e1b07147     	asrs	r7, r7, #2
  354484: e1a04000     	mov	r4, r0
  354488: e08f5005     	add	r5, pc, r5
  35448c: 0a00000c     	beq	0x3544c4 <SceneManager::clear()+0x5c> @ imm = #0x30
  354490: e3a06000     	mov	r6, #0
  354494: ea000000     	b	0x35449c <SceneManager::clear()+0x34> @ imm = #0x0
  354498: e594344c     	ldr	r3, [r4, #0x44c]
  35449c: e7933106     	ldr	r3, [r3, r6, lsl #2]
  3544a0: e2866001     	add	r6, r6, #1
  3544a4: e5932000     	ldr	r2, [r3]
  3544a8: e512000c     	ldr	r0, [r2, #-0xc]
  3544ac: e0830000     	add	r0, r3, r0
  3544b0: ebff2433     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x36f34
  3544b4: e1560007     	cmp	r6, r7
  3544b8: 1afffff6     	bne	0x354498 <SceneManager::clear()+0x30> @ imm = #-0x28
  3544bc: e594344c     	ldr	r3, [r4, #0x44c]
  3544c0: e5942450     	ldr	r2, [r4, #0x450]
  3544c4: e1530002     	cmp	r3, r2
  3544c8: 15843450     	strne	r3, [r4, #0x450]
  3544cc: e28d6004     	add	r6, sp, #4
  3544d0: e1a00004     	mov	r0, r4
  3544d4: eb08d4eb     	bl	0x589888 <glitch::scene::CSceneManager::clear()> @ imm = #0x2353ac
  3544d8: e1a00006     	mov	r0, r6
  3544dc: eb02e4bc     	bl	0x40d7d4 <LightSetManager::LightSetManager()> @ imm = #0xb92f0
  3544e0: e1a01006     	mov	r1, r6
  3544e4: e2840fa5     	add	r0, r4, #660
  3544e8: ebffff39     	bl	0x3541d4 <LightSetManager::operator=(LightSetManager const&)> @ imm = #-0x31c
  3544ec: e1a00006     	mov	r0, r6
  3544f0: eb02e1f6     	bl	0x40ccd0 <LightSetManager::~LightSetManager()> @ imm = #0xb87d8
  3544f4: e59f3088     	ldr	r3, [pc, #0x88]         @ 0x354584 <SceneManager::clear()+0x11c>
  3544f8: e5946014     	ldr	r6, [r4, #0x14]
  3544fc: e7953003     	ldr	r3, [r5, r3]
  354500: e5930000     	ldr	r0, [r3]
  354504: eb0c15cc     	bl	0x659c3c <glitch::collada::CResFileManager::unloadAll()> @ imm = #0x305730
  354508: e59650dc     	ldr	r5, [r6, #0xdc]
  35450c: e1a00005     	mov	r0, r5
  354510: eb0a165f     	bl	0x5d9e94 <glitch::video::CMaterialRendererManager::clearUnusedInstances()> @ imm = #0x28597c
  354514: e3a01000     	mov	r1, #0
  354518: e1a00005     	mov	r0, r5
  35451c: eb0a16d7     	bl	0x5da080 <glitch::core::detail::SIDedCollection<boost::intrusive_ptr<glitch::video::CMaterialRenderer>, unsigned short, false, glitch::video::detail::materialrenderermanager::SProperties, glitch::core::detail::sidedcollection::SValueTraits>::removeAll(bool)> @ imm = #0x285b5c
  354520: e59650e0     	ldr	r5, [r6, #0xe0]
  354524: e1a00005     	mov	r0, r5
  354528: eb0a4f57     	bl	0x5e828c <glitch::video::CTextureManager::clearPlaceHolders()> @ imm = #0x293d5c
  35452c: e3a01000     	mov	r1, #0
  354530: e1a00005     	mov	r0, r5
  354534: eb0a568f     	bl	0x5e9f78 <glitch::core::detail::SIDedCollection<boost::intrusive_ptr<glitch::video::ITexture>, unsigned short, false, glitch::video::detail::texturemanager::STextureProperties, glitch::core::detail::sidedcollection::SValueTraits>::removeAll(bool)> @ imm = #0x295a3c
  354538: e59630d8     	ldr	r3, [r6, #0xd8]
  35453c: e1a00003     	mov	r0, r3
  354540: e5933000     	ldr	r3, [r3]
  354544: e1a0e00f     	mov	lr, pc
  354548: e593f00c     	ldr	pc, [r3, #0xc]
  35454c: e5943438     	ldr	r3, [r4, #0x438]
  354550: e3530000     	cmp	r3, #0
  354554: 0a000005     	beq	0x354570 <SceneManager::clear()+0x108> @ imm = #0x14
  354558: e5932000     	ldr	r2, [r3]
  35455c: e512000c     	ldr	r0, [r2, #-0xc]
  354560: e0830000     	add	r0, r3, r0
  354564: ebff2406     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x36fe8
  354568: e3a03000     	mov	r3, #0
  35456c: e5843438     	str	r3, [r4, #0x438]
  354570: e3a03000     	mov	r3, #0
  354574: e584328c     	str	r3, [r4, #0x28c]
  354578: e28ddf6b     	add	sp, sp, #428
  35457c: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  354580: 08 06 64 00  	.word	0x00640608
  354584: 48 44 00 00  	.word	0x00004448
