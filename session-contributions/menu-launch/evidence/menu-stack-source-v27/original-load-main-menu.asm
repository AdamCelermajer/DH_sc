
R:\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004324dc <MenuManager::LoadMainMenu()>:
  4324dc: e59f346c     	ldr	r3, [pc, #0x46c]        @ 0x432950 <MenuManager::LoadMainMenu()+0x474>
  4324e0: e59f246c     	ldr	r2, [pc, #0x46c]        @ 0x432954 <MenuManager::LoadMainMenu()+0x478>
  4324e4: e92d4070     	push	{r4, r5, r6, lr}
  4324e8: e08f3003     	add	r3, pc, r3
  4324ec: e7932002     	ldr	r2, [r3, r2]
  4324f0: e3001356     	movw	r1, #0x356
  4324f4: e1a04000     	mov	r4, r0
  4324f8: e5922000     	ldr	r2, [r2]
  4324fc: e1520001     	cmp	r2, r1
  432500: 0a0000e8     	beq	0x4328a8 <MenuManager::LoadMainMenu()+0x3cc> @ imm = #0x3a0
  432504: e3520d0f     	cmp	r2, #960
  432508: 0a000021     	beq	0x432594 <MenuManager::LoadMainMenu()+0xb8> @ imm = #0x84
  43250c: e3520e32     	cmp	r2, #800
  432510: 0a00000f     	beq	0x432554 <MenuManager::LoadMainMenu()+0x78> @ imm = #0x3c
  432514: e59f243c     	ldr	r2, [pc, #0x43c]        @ 0x432958 <MenuManager::LoadMainMenu()+0x47c>
  432518: e7935002     	ldr	r5, [r3, r2]
  43251c: e595004c     	ldr	r0, [r5, #0x4c]
  432520: eb00ebfb     	bl	0x46d514 <SavegameManager::getLanguage() const> @ imm = #0x3afec
  432524: e3500005     	cmp	r0, #5
  432528: 0a0000f6     	beq	0x432908 <MenuManager::LoadMainMenu()+0x42c> @ imm = #0x3d8
  43252c: e595004c     	ldr	r0, [r5, #0x4c]
  432530: eb00ebf7     	bl	0x46d514 <SavegameManager::getLanguage() const> @ imm = #0x3afdc
  432534: e3500004     	cmp	r0, #4
  432538: 0a0000fe     	beq	0x432938 <MenuManager::LoadMainMenu()+0x45c> @ imm = #0x3f8
  43253c: e59f1418     	ldr	r1, [pc, #0x418]        @ 0x43295c <MenuManager::LoadMainMenu()+0x480>
  432540: e59400f4     	ldr	r0, [r4, #0xf4]
  432544: e3a02002     	mov	r2, #2
  432548: e08f1001     	add	r1, pc, r1
  43254c: eb001605     	bl	0x437d68 <MultiMenuManager::LoadSWFFile(char const*, int)> @ imm = #0x5814
  432550: ea000014     	b	0x4325a8 <MenuManager::LoadMainMenu()+0xcc> @ imm = #0x50
  432554: e59f2404     	ldr	r2, [pc, #0x404]        @ 0x432960 <MenuManager::LoadMainMenu()+0x484>
  432558: e7932002     	ldr	r2, [r3, r2]
  43255c: e5d22000     	ldrb	r2, [r2]
  432560: e3520000     	cmp	r2, #0
  432564: 1a0000e1     	bne	0x4328f0 <MenuManager::LoadMainMenu()+0x414> @ imm = #0x384
  432568: e59f23f4     	ldr	r2, [pc, #0x3f4]        @ 0x432964 <MenuManager::LoadMainMenu()+0x488>
  43256c: e7933002     	ldr	r3, [r3, r2]
  432570: e5d33000     	ldrb	r3, [r3]
  432574: e3530000     	cmp	r3, #0
  432578: 0a0000e8     	beq	0x432920 <MenuManager::LoadMainMenu()+0x444> @ imm = #0x3a0
  43257c: e59f13e4     	ldr	r1, [pc, #0x3e4]        @ 0x432968 <MenuManager::LoadMainMenu()+0x48c>
  432580: e3a02002     	mov	r2, #2
  432584: e59000f4     	ldr	r0, [r0, #0xf4]
  432588: e08f1001     	add	r1, pc, r1
  43258c: eb0015f5     	bl	0x437d68 <MultiMenuManager::LoadSWFFile(char const*, int)> @ imm = #0x57d4
  432590: ea000004     	b	0x4325a8 <MenuManager::LoadMainMenu()+0xcc> @ imm = #0x10
  432594: e59f13d0     	ldr	r1, [pc, #0x3d0]        @ 0x43296c <MenuManager::LoadMainMenu()+0x490>
  432598: e59000f4     	ldr	r0, [r0, #0xf4]
  43259c: e3a02002     	mov	r2, #2
  4325a0: e08f1001     	add	r1, pc, r1
  4325a4: eb0015ef     	bl	0x437d68 <MultiMenuManager::LoadSWFFile(char const*, int)> @ imm = #0x57bc
  4325a8: e59430f4     	ldr	r3, [r4, #0xf4]
  4325ac: e3a01084     	mov	r1, #132
  4325b0: e593013c     	ldr	r0, [r3, #0x13c]
  4325b4: eb0dd5b7     	bl	0x7a7c98 <RenderFX::SetInputBehavior(int)> @ imm = #0x3756dc
  4325b8: e59430f4     	ldr	r3, [r4, #0xf4]
  4325bc: e3a01001     	mov	r1, #1
  4325c0: e3a02000     	mov	r2, #0
  4325c4: e593313c     	ldr	r3, [r3, #0x13c]
  4325c8: e1a00003     	mov	r0, r3
  4325cc: e5933000     	ldr	r3, [r3]
  4325d0: e1a0e00f     	mov	lr, pc
  4325d4: e593f010     	ldr	pc, [r3, #0x10]
  4325d8: ebffe020     	bl	0x42a660 <MenuDebug::GetInstance()> @ imm = #-0x7f80
  4325dc: e590304c     	ldr	r3, [r0, #0x4c]
  4325e0: e1a05000     	mov	r5, r0
  4325e4: e3530000     	cmp	r3, #0
  4325e8: 0a0000a1     	beq	0x432874 <MenuManager::LoadMainMenu()+0x398> @ imm = #0x284
  4325ec: e5900048     	ldr	r0, [r0, #0x48]
  4325f0: e5d03004     	ldrb	r3, [r0, #0x4]
  4325f4: e3530000     	cmp	r3, #0
  4325f8: 0a000095     	beq	0x432854 <MenuManager::LoadMainMenu()+0x378> @ imm = #0x254
  4325fc: ebffe017     	bl	0x42a660 <MenuDebug::GetInstance()> @ imm = #-0x7fa4
  432600: e590304c     	ldr	r3, [r0, #0x4c]
  432604: e1a05000     	mov	r5, r0
  432608: e3530000     	cmp	r3, #0
  43260c: 0a000003     	beq	0x432620 <MenuManager::LoadMainMenu()+0x144> @ imm = #0xc
  432610: e5900048     	ldr	r0, [r0, #0x48]
  432614: e5d02004     	ldrb	r2, [r0, #0x4]
  432618: e3520000     	cmp	r2, #0
  43261c: 0a000097     	beq	0x432880 <MenuManager::LoadMainMenu()+0x3a4> @ imm = #0x25c
  432620: e3a02000     	mov	r2, #0
  432624: e5c3209b     	strb	r2, [r3, #0x9b]
  432628: ebffe74b     	bl	0x42c35c <MenuMainMenu::GetInstance()> @ imm = #-0x62d4
  43262c: e590304c     	ldr	r3, [r0, #0x4c]
  432630: e1a05000     	mov	r5, r0
  432634: e3530000     	cmp	r3, #0
  432638: 0a00007e     	beq	0x432838 <MenuManager::LoadMainMenu()+0x35c> @ imm = #0x1f8
  43263c: e5900048     	ldr	r0, [r0, #0x48]
  432640: e5d03004     	ldrb	r3, [r0, #0x4]
  432644: e3530000     	cmp	r3, #0
  432648: 0a000072     	beq	0x432818 <MenuManager::LoadMainMenu()+0x33c> @ imm = #0x1c8
  43264c: ebffdd2f     	bl	0x429b10 <MenuCharacterSelect::GetInstance()> @ imm = #-0x8b44
  432650: e590304c     	ldr	r3, [r0, #0x4c]
  432654: e1a05000     	mov	r5, r0
  432658: e3530000     	cmp	r3, #0
  43265c: 0a00006a     	beq	0x43280c <MenuManager::LoadMainMenu()+0x330> @ imm = #0x1a8
  432660: e5900048     	ldr	r0, [r0, #0x48]
  432664: e5d03004     	ldrb	r3, [r0, #0x4]
  432668: e3530000     	cmp	r3, #0
  43266c: 0a00005e     	beq	0x4327ec <MenuManager::LoadMainMenu()+0x310> @ imm = #0x178
  432670: ebffe270     	bl	0x42b038 <MenuEnterName::GetInstance()> @ imm = #-0x7640
  432674: e590304c     	ldr	r3, [r0, #0x4c]
  432678: e1a05000     	mov	r5, r0
  43267c: e3530000     	cmp	r3, #0
  432680: 0a000052     	beq	0x4327d0 <MenuManager::LoadMainMenu()+0x2f4> @ imm = #0x148
  432684: e5900048     	ldr	r0, [r0, #0x48]
  432688: e5d03004     	ldrb	r3, [r0, #0x4]
  43268c: e3530000     	cmp	r3, #0
  432690: 0a000046     	beq	0x4327b0 <MenuManager::LoadMainMenu()+0x2d4> @ imm = #0x118
  432694: eb000c79     	bl	0x435880 <MenuMultiplayerLobbyMulti::GetInstance()> @ imm = #0x31e4
  432698: e590304c     	ldr	r3, [r0, #0x4c]
  43269c: e1a05000     	mov	r5, r0
  4326a0: e3530000     	cmp	r3, #0
  4326a4: 0a00003a     	beq	0x432794 <MenuManager::LoadMainMenu()+0x2b8> @ imm = #0xe8
  4326a8: e5900048     	ldr	r0, [r0, #0x48]
  4326ac: e5d03004     	ldrb	r3, [r0, #0x4]
  4326b0: e3530000     	cmp	r3, #0
  4326b4: 0a00002e     	beq	0x432774 <MenuManager::LoadMainMenu()+0x298> @ imm = #0xb8
  4326b8: ebffe48b     	bl	0x42b8ec <MenuLeaderboard::GetInstance()> @ imm = #-0x6dd4
  4326bc: e590304c     	ldr	r3, [r0, #0x4c]
  4326c0: e1a05000     	mov	r5, r0
  4326c4: e3530000     	cmp	r3, #0
  4326c8: 0a000022     	beq	0x432758 <MenuManager::LoadMainMenu()+0x27c> @ imm = #0x88
  4326cc: e5900048     	ldr	r0, [r0, #0x48]
  4326d0: e5d03004     	ldrb	r3, [r0, #0x4]
  4326d4: e3530000     	cmp	r3, #0
  4326d8: 0a000016     	beq	0x432738 <MenuManager::LoadMainMenu()+0x25c> @ imm = #0x58
  4326dc: e5945068     	ldr	r5, [r4, #0x68]
  4326e0: e5943064     	ldr	r3, [r4, #0x64]
  4326e4: e1a00004     	mov	r0, r4
  4326e8: e0635005     	rsb	r5, r3, r5
  4326ec: e1a05145     	asr	r5, r5, #2
  4326f0: ebfff230     	bl	0x42efb8 <MenuManager::PostLoad()> @ imm = #-0x3740
  4326f4: ea000009     	b	0x432720 <MenuManager::LoadMainMenu()+0x244> @ imm = #0x24
  4326f8: e7930105     	ldr	r0, [r3, r5, lsl #2]
  4326fc: e3a01000     	mov	r1, #0
  432700: ebffc5ac     	bl	0x423db8 <MenuBase::RegisterDragAndDrops(int (*)(gameswf::character const*))> @ imm = #-0xe950
  432704: e5943064     	ldr	r3, [r4, #0x64]
  432708: e7933105     	ldr	r3, [r3, r5, lsl #2]
  43270c: e2855001     	add	r5, r5, #1
  432710: e1a00003     	mov	r0, r3
  432714: e5933000     	ldr	r3, [r3]
  432718: e1a0e00f     	mov	lr, pc
  43271c: e593f010     	ldr	pc, [r3, #0x10]
  432720: e5943064     	ldr	r3, [r4, #0x64]
  432724: e5942068     	ldr	r2, [r4, #0x68]
  432728: e0632002     	rsb	r2, r3, r2
  43272c: e1550142     	cmp	r5, r2, asr #2
  432730: 3afffff0     	blo	0x4326f8 <MenuManager::LoadMainMenu()+0x21c> @ imm = #-0x40
  432734: e8bd8070     	pop	{r4, r5, r6, pc}
  432738: e5901000     	ldr	r1, [r0]
  43273c: e2411001     	sub	r1, r1, #1
  432740: e3510000     	cmp	r1, #0
  432744: e5801000     	str	r1, [r0]
  432748: 0a00005e     	beq	0x4328c8 <MenuManager::LoadMainMenu()+0x3ec> @ imm = #0x178
  43274c: e3a03000     	mov	r3, #0
  432750: e585304c     	str	r3, [r5, #0x4c]
  432754: e5853048     	str	r3, [r5, #0x48]
  432758: ebffe8cb     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #-0x5cd4
  43275c: e1a05000     	mov	r5, r0
  432760: ebffe461     	bl	0x42b8ec <MenuLeaderboard::GetInstance()> @ imm = #-0x6e7c
  432764: e1a01000     	mov	r1, r0
  432768: e1a00005     	mov	r0, r5
  43276c: ebfff1c8     	bl	0x42ee94 <MenuManager::RegisterMenu(MenuBase*)> @ imm = #-0x38e0
  432770: eaffffd9     	b	0x4326dc <MenuManager::LoadMainMenu()+0x200> @ imm = #-0x9c
  432774: e5901000     	ldr	r1, [r0]
  432778: e2411001     	sub	r1, r1, #1
  43277c: e3510000     	cmp	r1, #0
  432780: e5801000     	str	r1, [r0]
  432784: 0a000057     	beq	0x4328e8 <MenuManager::LoadMainMenu()+0x40c> @ imm = #0x15c
  432788: e3a03000     	mov	r3, #0
  43278c: e585304c     	str	r3, [r5, #0x4c]
  432790: e5853048     	str	r3, [r5, #0x48]
  432794: ebffe8bc     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #-0x5d10
  432798: e1a05000     	mov	r5, r0
  43279c: eb000c37     	bl	0x435880 <MenuMultiplayerLobbyMulti::GetInstance()> @ imm = #0x30dc
  4327a0: e1a01000     	mov	r1, r0
  4327a4: e1a00005     	mov	r0, r5
  4327a8: ebfff1b9     	bl	0x42ee94 <MenuManager::RegisterMenu(MenuBase*)> @ imm = #-0x391c
  4327ac: eaffffc1     	b	0x4326b8 <MenuManager::LoadMainMenu()+0x1dc> @ imm = #-0xfc
  4327b0: e5901000     	ldr	r1, [r0]
  4327b4: e2411001     	sub	r1, r1, #1
  4327b8: e3510000     	cmp	r1, #0
  4327bc: e5801000     	str	r1, [r0]
  4327c0: 0a000046     	beq	0x4328e0 <MenuManager::LoadMainMenu()+0x404> @ imm = #0x118
  4327c4: e3a03000     	mov	r3, #0
  4327c8: e585304c     	str	r3, [r5, #0x4c]
  4327cc: e5853048     	str	r3, [r5, #0x48]
  4327d0: ebffe8ad     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #-0x5d4c
  4327d4: e1a05000     	mov	r5, r0
  4327d8: ebffe216     	bl	0x42b038 <MenuEnterName::GetInstance()> @ imm = #-0x77a8
  4327dc: e1a01000     	mov	r1, r0
  4327e0: e1a00005     	mov	r0, r5
  4327e4: ebfff1aa     	bl	0x42ee94 <MenuManager::RegisterMenu(MenuBase*)> @ imm = #-0x3958
  4327e8: eaffffa9     	b	0x432694 <MenuManager::LoadMainMenu()+0x1b8> @ imm = #-0x15c
  4327ec: e5901000     	ldr	r1, [r0]
  4327f0: e2411001     	sub	r1, r1, #1
  4327f4: e3510000     	cmp	r1, #0
  4327f8: e5801000     	str	r1, [r0]
  4327fc: 0a000033     	beq	0x4328d0 <MenuManager::LoadMainMenu()+0x3f4> @ imm = #0xcc
  432800: e3a03000     	mov	r3, #0
  432804: e585304c     	str	r3, [r5, #0x4c]
  432808: e5853048     	str	r3, [r5, #0x48]
  43280c: ebffdcbf     	bl	0x429b10 <MenuCharacterSelect::GetInstance()> @ imm = #-0x8d04
  432810: ebffd92a     	bl	0x428cc0 <MenuCharacterSelect::Init()> @ imm = #-0x9b58
  432814: eaffff95     	b	0x432670 <MenuManager::LoadMainMenu()+0x194> @ imm = #-0x1ac
  432818: e5901000     	ldr	r1, [r0]
  43281c: e2411001     	sub	r1, r1, #1
  432820: e3510000     	cmp	r1, #0
  432824: e5801000     	str	r1, [r0]
  432828: 0a00002a     	beq	0x4328d8 <MenuManager::LoadMainMenu()+0x3fc> @ imm = #0xa8
  43282c: e3a03000     	mov	r3, #0
  432830: e585304c     	str	r3, [r5, #0x4c]
  432834: e5853048     	str	r3, [r5, #0x48]
  432838: ebffe893     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #-0x5db4
  43283c: e1a05000     	mov	r5, r0
  432840: ebffe6c5     	bl	0x42c35c <MenuMainMenu::GetInstance()> @ imm = #-0x64ec
  432844: e1a01000     	mov	r1, r0
  432848: e1a00005     	mov	r0, r5
  43284c: ebfff190     	bl	0x42ee94 <MenuManager::RegisterMenu(MenuBase*)> @ imm = #-0x39c0
  432850: eaffff7d     	b	0x43264c <MenuManager::LoadMainMenu()+0x170> @ imm = #-0x20c
  432854: e5901000     	ldr	r1, [r0]
  432858: e2411001     	sub	r1, r1, #1
  43285c: e3510000     	cmp	r1, #0
  432860: e5801000     	str	r1, [r0]
  432864: 0a000015     	beq	0x4328c0 <MenuManager::LoadMainMenu()+0x3e4> @ imm = #0x54
  432868: e3a03000     	mov	r3, #0
  43286c: e585304c     	str	r3, [r5, #0x4c]
  432870: e5853048     	str	r3, [r5, #0x48]
  432874: ebffdf79     	bl	0x42a660 <MenuDebug::GetInstance()> @ imm = #-0x821c
  432878: ebffddf0     	bl	0x42a040 <MenuDebug::Init()> @ imm = #-0x8840
  43287c: eaffff5e     	b	0x4325fc <MenuManager::LoadMainMenu()+0x120> @ imm = #-0x288
  432880: e5901000     	ldr	r1, [r0]
  432884: e2411001     	sub	r1, r1, #1
  432888: e3510000     	cmp	r1, #0
  43288c: e5801000     	str	r1, [r0]
  432890: 1a000000     	bne	0x432898 <MenuManager::LoadMainMenu()+0x3bc> @ imm = #0x0
  432894: eb0c80a7     	bl	0x752b38 <gameswf::free_internal(void*, unsigned int)> @ imm = #0x32029c
  432898: e3a03000     	mov	r3, #0
  43289c: e585304c     	str	r3, [r5, #0x4c]
  4328a0: e5853048     	str	r3, [r5, #0x48]
  4328a4: eaffff5d     	b	0x432620 <MenuManager::LoadMainMenu()+0x144> @ imm = #-0x28c
  4328a8: e59f10c0     	ldr	r1, [pc, #0xc0]         @ 0x432970 <MenuManager::LoadMainMenu()+0x494>
  4328ac: e3a02002     	mov	r2, #2
  4328b0: e59000f4     	ldr	r0, [r0, #0xf4]
  4328b4: e08f1001     	add	r1, pc, r1
  4328b8: eb00152a     	bl	0x437d68 <MultiMenuManager::LoadSWFFile(char const*, int)> @ imm = #0x54a8
  4328bc: eaffff39     	b	0x4325a8 <MenuManager::LoadMainMenu()+0xcc> @ imm = #-0x31c
  4328c0: eb0c809c     	bl	0x752b38 <gameswf::free_internal(void*, unsigned int)> @ imm = #0x320270
  4328c4: eaffffe7     	b	0x432868 <MenuManager::LoadMainMenu()+0x38c> @ imm = #-0x64
  4328c8: eb0c809a     	bl	0x752b38 <gameswf::free_internal(void*, unsigned int)> @ imm = #0x320268
  4328cc: eaffff9e     	b	0x43274c <MenuManager::LoadMainMenu()+0x270> @ imm = #-0x188
  4328d0: eb0c8098     	bl	0x752b38 <gameswf::free_internal(void*, unsigned int)> @ imm = #0x320260
  4328d4: eaffffc9     	b	0x432800 <MenuManager::LoadMainMenu()+0x324> @ imm = #-0xdc
  4328d8: eb0c8096     	bl	0x752b38 <gameswf::free_internal(void*, unsigned int)> @ imm = #0x320258
  4328dc: eaffffd2     	b	0x43282c <MenuManager::LoadMainMenu()+0x350> @ imm = #-0xb8
  4328e0: eb0c8094     	bl	0x752b38 <gameswf::free_internal(void*, unsigned int)> @ imm = #0x320250
  4328e4: eaffffb6     	b	0x4327c4 <MenuManager::LoadMainMenu()+0x2e8> @ imm = #-0x128
  4328e8: eb0c8092     	bl	0x752b38 <gameswf::free_internal(void*, unsigned int)> @ imm = #0x320248
  4328ec: eaffffa5     	b	0x432788 <MenuManager::LoadMainMenu()+0x2ac> @ imm = #-0x16c
  4328f0: e59f107c     	ldr	r1, [pc, #0x7c]         @ 0x432974 <MenuManager::LoadMainMenu()+0x498>
  4328f4: e3a02002     	mov	r2, #2
  4328f8: e59000f4     	ldr	r0, [r0, #0xf4]
  4328fc: e08f1001     	add	r1, pc, r1
  432900: eb001518     	bl	0x437d68 <MultiMenuManager::LoadSWFFile(char const*, int)> @ imm = #0x5460
  432904: eaffff27     	b	0x4325a8 <MenuManager::LoadMainMenu()+0xcc> @ imm = #-0x364
  432908: e59f1068     	ldr	r1, [pc, #0x68]         @ 0x432978 <MenuManager::LoadMainMenu()+0x49c>
  43290c: e59400f4     	ldr	r0, [r4, #0xf4]
  432910: e3a02002     	mov	r2, #2
  432914: e08f1001     	add	r1, pc, r1
  432918: eb001512     	bl	0x437d68 <MultiMenuManager::LoadSWFFile(char const*, int)> @ imm = #0x5448
  43291c: eaffff21     	b	0x4325a8 <MenuManager::LoadMainMenu()+0xcc> @ imm = #-0x37c
  432920: e59f1054     	ldr	r1, [pc, #0x54]         @ 0x43297c <MenuManager::LoadMainMenu()+0x4a0>
  432924: e3a02002     	mov	r2, #2
  432928: e59000f4     	ldr	r0, [r0, #0xf4]
  43292c: e08f1001     	add	r1, pc, r1
  432930: eb00150c     	bl	0x437d68 <MultiMenuManager::LoadSWFFile(char const*, int)> @ imm = #0x5430
  432934: eaffff1b     	b	0x4325a8 <MenuManager::LoadMainMenu()+0xcc> @ imm = #-0x394
  432938: e59f1040     	ldr	r1, [pc, #0x40]         @ 0x432980 <MenuManager::LoadMainMenu()+0x4a4>
  43293c: e59400f4     	ldr	r0, [r4, #0xf4]
  432940: e3a02002     	mov	r2, #2
  432944: e08f1001     	add	r1, pc, r1
  432948: eb001506     	bl	0x437d68 <MultiMenuManager::LoadSWFFile(char const*, int)> @ imm = #0x5418
  43294c: eaffff15     	b	0x4325a8 <MenuManager::LoadMainMenu()+0xcc> @ imm = #-0x3ac
  432950: a8 25 56 00  	.word	0x005625a8
  432954: c4 25 00 00  	.word	0x000025c4
  432958: f4 37 00 00  	.word	0x000037f4
  43295c: 38 7b 49 00  	.word	0x00497b38
  432960: 68 27 00 00  	.word	0x00002768
  432964: e0 16 00 00  	.word	0x000016e0
  432968: b8 7a 49 00  	.word	0x00497ab8
  43296c: e0 7a 49 00  	.word	0x00497ae0
  432970: ac 77 49 00  	.word	0x004977ac
  432974: 24 77 49 00  	.word	0x00497724
  432978: 84 77 49 00  	.word	0x00497784
  43297c: f4 76 49 00  	.word	0x004976f4
  432980: 74 77 49 00  	.word	0x00497774
