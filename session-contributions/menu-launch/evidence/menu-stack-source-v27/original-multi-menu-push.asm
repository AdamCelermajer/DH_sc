
R:\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00438278 <MultiMenuManager::PushMenu(MenuBase*)>:
  438278: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  43827c: e1a08001     	mov	r8, r1
  438280: e24dd00c     	sub	sp, sp, #12
  438284: e1a06000     	mov	r6, r0
  438288: e2811008     	add	r1, r1, #8
  43828c: e5980004     	ldr	r0, [r8, #0x4]
  438290: eb0dc00b     	bl	0x7a82c4 <MenuFX::GetState(char const*)> @ imm = #0x37002c
  438294: e59f78b4     	ldr	r7, [pc, #0x8b4]        @ 0x438b50 <MultiMenuManager::PushMenu(MenuBase*)+0x8d8>
  438298: e2504000     	subs	r4, r0, #0
  43829c: e08f7007     	add	r7, pc, r7
  4382a0: 0a000088     	beq	0x4384c8 <MultiMenuManager::PushMenu(MenuBase*)+0x250> @ imm = #0x220
  4382a4: e59f08a8     	ldr	r0, [pc, #0x8a8]        @ 0x438b54 <MultiMenuManager::PushMenu(MenuBase*)+0x8dc>
  4382a8: e2845008     	add	r5, r4, #8
  4382ac: e1a01005     	mov	r1, r5
  4382b0: e08f0000     	add	r0, pc, r0
  4382b4: ebfbaf96     	bl	0x324114 <_DEBUG_OUT(char const*, ...)> @ imm = #-0x1141a8
  4382b8: e59f1898     	ldr	r1, [pc, #0x898]        @ 0x438b58 <MultiMenuManager::PushMenu(MenuBase*)+0x8e0>
  4382bc: e1a00005     	mov	r0, r5
  4382c0: e08f1001     	add	r1, pc, r1
  4382c4: ebfb5814     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x129fb0
  4382c8: e3500000     	cmp	r0, #0
  4382cc: 0a000090     	beq	0x438514 <MultiMenuManager::PushMenu(MenuBase*)+0x29c> @ imm = #0x240
  4382d0: e59f1884     	ldr	r1, [pc, #0x884]        @ 0x438b5c <MultiMenuManager::PushMenu(MenuBase*)+0x8e4>
  4382d4: e1a00005     	mov	r0, r5
  4382d8: e08f1001     	add	r1, pc, r1
  4382dc: ebfb580e     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x129fc8
  4382e0: e3500000     	cmp	r0, #0
  4382e4: 0a0000b1     	beq	0x4385b0 <MultiMenuManager::PushMenu(MenuBase*)+0x338> @ imm = #0x2c4
  4382e8: e59f1870     	ldr	r1, [pc, #0x870]        @ 0x438b60 <MultiMenuManager::PushMenu(MenuBase*)+0x8e8>
  4382ec: e1a00005     	mov	r0, r5
  4382f0: e08f1001     	add	r1, pc, r1
  4382f4: ebfb5808     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x129fe0
  4382f8: e250a000     	subs	r10, r0, #0
  4382fc: 0a0000a0     	beq	0x438584 <MultiMenuManager::PushMenu(MenuBase*)+0x30c> @ imm = #0x280
  438300: e59f185c     	ldr	r1, [pc, #0x85c]        @ 0x438b64 <MultiMenuManager::PushMenu(MenuBase*)+0x8ec>
  438304: e1a00005     	mov	r0, r5
  438308: e08f1001     	add	r1, pc, r1
  43830c: ebfb5802     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x129ff8
  438310: e3500000     	cmp	r0, #0
  438314: 1a00006d     	bne	0x4384d0 <MultiMenuManager::PushMenu(MenuBase*)+0x258> @ imm = #0x1b4
  438318: e59f3848     	ldr	r3, [pc, #0x848]        @ 0x438b68 <MultiMenuManager::PushMenu(MenuBase*)+0x8f0>
  43831c: e3a02001     	mov	r2, #1
  438320: e7973003     	ldr	r3, [r7, r3]
  438324: e5c32000     	strb	r2, [r3]
  438328: e59f383c     	ldr	r3, [pc, #0x83c]        @ 0x438b6c <MultiMenuManager::PushMenu(MenuBase*)+0x8f4>
  43832c: e3a0a000     	mov	r10, #0
  438330: e7973003     	ldr	r3, [r7, r3]
  438334: e5c3a000     	strb	r10, [r3]
  438338: e5969128     	ldr	r9, [r6, #0x128]
  43833c: e159000a     	cmp	r9, r10
  438340: da000083     	ble	0x438554 <MultiMenuManager::PushMenu(MenuBase*)+0x2dc> @ imm = #0x20c
  438344: e5963124     	ldr	r3, [r6, #0x124]
  438348: e2492001     	sub	r2, r9, #1
  43834c: e7937102     	ldr	r7, [r3, r2, lsl #2]
  438350: e5973118     	ldr	r3, [r7, #0x118]
  438354: e3530000     	cmp	r3, #0
  438358: da00007d     	ble	0x438554 <MultiMenuManager::PushMenu(MenuBase*)+0x2dc> @ imm = #0x1f4
  43835c: e1a00007     	mov	r0, r7
  438360: eb0dbef2     	bl	0x7a7f30 <MenuFX::GetCurrentState()> @ imm = #0x36fbc8
  438364: e1a0b000     	mov	r11, r0
  438368: e49b3008     	ldr	r3, [r11], #8
  43836c: e1a09000     	mov	r9, r0
  438370: e1a0e00f     	mov	lr, pc
  438374: e593f018     	ldr	pc, [r3, #0x18]
  438378: e59f27f0     	ldr	r2, [pc, #0x7f0]        @ 0x438b70 <MultiMenuManager::PushMenu(MenuBase*)+0x8f8>
  43837c: e1a0100b     	mov	r1, r11
  438380: e1a0300a     	mov	r3, r10
  438384: e08f2002     	add	r2, pc, r2
  438388: e1a00007     	mov	r0, r7
  43838c: e58da000     	str	r10, [sp]
  438390: eb0dd514     	bl	0x7ad7e8 <RenderFX::InvokeASCallback(char const*, char const*, gameswf::as_value const*, int)> @ imm = #0x375450
  438394: e597b0f8     	ldr	r11, [r7, #0xf8]
  438398: e21bb040     	ands	r11, r11, #64
  43839c: 0a0000eb     	beq	0x438750 <MultiMenuManager::PushMenu(MenuBase*)+0x4d8> @ imm = #0x3ac
  4383a0: e3a01000     	mov	r1, #0
  4383a4: e1a00007     	mov	r0, r7
  4383a8: eb0dbe31     	bl	0x7a7c74 <RenderFX::GetController(int)> @ imm = #0x36f8c4
  4383ac: e5901010     	ldr	r1, [r0, #0x10]
  4383b0: e2890050     	add	r0, r9, #80
  4383b4: ebffbdfb     	bl	0x427ba8 <gameswf::weak_ptr<gameswf::character>::operator=(gameswf::character*)> @ imm = #-0x10814
  4383b8: e59730f8     	ldr	r3, [r7, #0xf8]
  4383bc: e3130008     	tst	r3, #8
  4383c0: 02847048     	addeq	r7, r4, #72
  4383c4: 1a0000c2     	bne	0x4386d4 <MultiMenuManager::PushMenu(MenuBase*)+0x45c> @ imm = #0x308
  4383c8: e5969128     	ldr	r9, [r6, #0x128]
  4383cc: e596312c     	ldr	r3, [r6, #0x12c]
  4383d0: e5988004     	ldr	r8, [r8, #0x4]
  4383d4: e289a001     	add	r10, r9, #1
  4383d8: e15a0003     	cmp	r10, r3
  4383dc: d1a02009     	movle	r2, r9
  4383e0: ca000062     	bgt	0x438570 <MultiMenuManager::PushMenu(MenuBase*)+0x2f8> @ imm = #0x188
  4383e4: e5963124     	ldr	r3, [r6, #0x124]
  4383e8: e7838102     	str	r8, [r3, r2, lsl #2]
  4383ec: e5963124     	ldr	r3, [r6, #0x124]
  4383f0: e586a128     	str	r10, [r6, #0x128]
  4383f4: e7936109     	ldr	r6, [r3, r9, lsl #2]
  4383f8: e5968118     	ldr	r8, [r6, #0x118]
  4383fc: e298a001     	adds	r10, r8, #1
  438400: 0a000002     	beq	0x438410 <MultiMenuManager::PushMenu(MenuBase*)+0x198> @ imm = #0x8
  438404: e596311c     	ldr	r3, [r6, #0x11c]
  438408: e15a0003     	cmp	r10, r3
  43840c: ca0000c0     	bgt	0x438714 <MultiMenuManager::PushMenu(MenuBase*)+0x49c> @ imm = #0x300
  438410: e5963114     	ldr	r3, [r6, #0x114]
  438414: e3a02000     	mov	r2, #0
  438418: e7832108     	str	r2, [r3, r8, lsl #2]
  43841c: e5963114     	ldr	r3, [r6, #0x114]
  438420: e586a118     	str	r10, [r6, #0x118]
  438424: e7834108     	str	r4, [r3, r8, lsl #2]
  438428: e594304c     	ldr	r3, [r4, #0x4c]
  43842c: e1530002     	cmp	r3, r2
  438430: 0a000003     	beq	0x438444 <MultiMenuManager::PushMenu(MenuBase*)+0x1cc> @ imm = #0xc
  438434: e5940048     	ldr	r0, [r4, #0x48]
  438438: e5d02004     	ldrb	r2, [r0, #0x4]
  43843c: e3520000     	cmp	r2, #0
  438440: 0a000087     	beq	0x438664 <MultiMenuManager::PushMenu(MenuBase*)+0x3ec> @ imm = #0x21c
  438444: e3a02001     	mov	r2, #1
  438448: e5c3209b     	strb	r2, [r3, #0x9b]
  43844c: e59630f8     	ldr	r3, [r6, #0xf8]
  438450: e3130008     	tst	r3, #8
  438454: 1a00006f     	bne	0x438618 <MultiMenuManager::PushMenu(MenuBase*)+0x3a0> @ imm = #0x1bc
  438458: e1a00007     	mov	r0, r7
  43845c: ebffff70     	bl	0x438224 <gameswf::weak_ptr<gameswf::character>::get_ptr() const> @ imm = #-0x240
  438460: e1a01000     	mov	r1, r0
  438464: e1a00006     	mov	r0, r6
  438468: eb0dbe9e     	bl	0x7a7ee8 <RenderFX::SetContext(gameswf::character*)> @ imm = #0x36fa78
  43846c: e59f2700     	ldr	r2, [pc, #0x700]        @ 0x438b74 <MultiMenuManager::PushMenu(MenuBase*)+0x8fc>
  438470: e3a0c000     	mov	r12, #0
  438474: e1a01005     	mov	r1, r5
  438478: e1a0300c     	mov	r3, r12
  43847c: e08f2002     	add	r2, pc, r2
  438480: e1a00006     	mov	r0, r6
  438484: e58dc000     	str	r12, [sp]
  438488: eb0dd4d6     	bl	0x7ad7e8 <RenderFX::InvokeASCallback(char const*, char const*, gameswf::as_value const*, int)> @ imm = #0x375358
  43848c: e59630f8     	ldr	r3, [r6, #0xf8]
  438490: e2135040     	ands	r5, r3, #64
  438494: 0a000055     	beq	0x4385f0 <MultiMenuManager::PushMenu(MenuBase*)+0x378> @ imm = #0x154
  438498: e3130001     	tst	r3, #1
  43849c: 1a00004e     	bne	0x4385dc <MultiMenuManager::PushMenu(MenuBase*)+0x364> @ imm = #0x138
  4384a0: e1a00004     	mov	r0, r4
  4384a4: e5943000     	ldr	r3, [r4]
  4384a8: e1a0e00f     	mov	lr, pc
  4384ac: e593f00c     	ldr	pc, [r3, #0xc]
  4384b0: e5943000     	ldr	r3, [r4]
  4384b4: e1a00004     	mov	r0, r4
  4384b8: e1a0e00f     	mov	lr, pc
  4384bc: e593f014     	ldr	pc, [r3, #0x14]
  4384c0: e3a03001     	mov	r3, #1
  4384c4: e5843058     	str	r3, [r4, #0x58]
  4384c8: e28dd00c     	add	sp, sp, #12
  4384cc: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  4384d0: e59f16a0     	ldr	r1, [pc, #0x6a0]        @ 0x438b78 <MultiMenuManager::PushMenu(MenuBase*)+0x900>
  4384d4: e1a00005     	mov	r0, r5
  4384d8: e08f1001     	add	r1, pc, r1
  4384dc: ebfb578e     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a1c8
  4384e0: e3500000     	cmp	r0, #0
  4384e4: 0a000068     	beq	0x43868c <MultiMenuManager::PushMenu(MenuBase*)+0x414> @ imm = #0x1a0
  4384e8: e59f168c     	ldr	r1, [pc, #0x68c]        @ 0x438b7c <MultiMenuManager::PushMenu(MenuBase*)+0x904>
  4384ec: e1a00005     	mov	r0, r5
  4384f0: e08f1001     	add	r1, pc, r1
  4384f4: ebfb5788     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a1e0
  4384f8: e3500000     	cmp	r0, #0
  4384fc: 1a000088     	bne	0x438724 <MultiMenuManager::PushMenu(MenuBase*)+0x4ac> @ imm = #0x220
  438500: e59f3678     	ldr	r3, [pc, #0x678]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  438504: e3a02002     	mov	r2, #2
  438508: e7973003     	ldr	r3, [r7, r3]
  43850c: e5832000     	str	r2, [r3]
  438510: eaffff84     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x1f0
  438514: e59f3668     	ldr	r3, [pc, #0x668]        @ 0x438b84 <MultiMenuManager::PushMenu(MenuBase*)+0x90c>
  438518: e7973003     	ldr	r3, [r7, r3]
  43851c: e5d33000     	ldrb	r3, [r3]
  438520: e3530000     	cmp	r3, #0
  438524: 1a000078     	bne	0x43870c <MultiMenuManager::PushMenu(MenuBase*)+0x494> @ imm = #0x1e0
  438528: e59f0658     	ldr	r0, [pc, #0x658]        @ 0x438b88 <MultiMenuManager::PushMenu(MenuBase*)+0x910>
  43852c: e08f0000     	add	r0, pc, r0
  438530: ebfbaef7     	bl	0x324114 <_DEBUG_OUT(char const*, ...)> @ imm = #-0x114424
  438534: e59f2650     	ldr	r2, [pc, #0x650]        @ 0x438b8c <MultiMenuManager::PushMenu(MenuBase*)+0x914>
  438538: e3a03001     	mov	r3, #1
  43853c: e7971002     	ldr	r1, [r7, r2]
  438540: e59f2648     	ldr	r2, [pc, #0x648]        @ 0x438b90 <MultiMenuManager::PushMenu(MenuBase*)+0x918>
  438544: e5c13000     	strb	r3, [r1]
  438548: e7972002     	ldr	r2, [r7, r2]
  43854c: e5c23000     	strb	r3, [r2]
  438550: eaffff5e     	b	0x4382d0 <MultiMenuManager::PushMenu(MenuBase*)+0x58> @ imm = #-0x288
  438554: e596312c     	ldr	r3, [r6, #0x12c]
  438558: e289a001     	add	r10, r9, #1
  43855c: e2847048     	add	r7, r4, #72
  438560: e15a0003     	cmp	r10, r3
  438564: e5988004     	ldr	r8, [r8, #0x4]
  438568: d1a02009     	movle	r2, r9
  43856c: daffff9c     	ble	0x4383e4 <MultiMenuManager::PushMenu(MenuBase*)+0x16c> @ imm = #-0x190
  438570: e2860f49     	add	r0, r6, #292
  438574: e08a10ca     	add	r1, r10, r10, asr #1
  438578: ebfffd3b     	bl	0x437a6c <gameswf::array<MenuFX*>::reserve(int)> @ imm = #-0xb14
  43857c: e5962128     	ldr	r2, [r6, #0x128]
  438580: eaffff97     	b	0x4383e4 <MultiMenuManager::PushMenu(MenuBase*)+0x16c> @ imm = #-0x1a4
  438584: e59f0608     	ldr	r0, [pc, #0x608]        @ 0x438b94 <MultiMenuManager::PushMenu(MenuBase*)+0x91c>
  438588: e08f0000     	add	r0, pc, r0
  43858c: ebfbaee0     	bl	0x324114 <_DEBUG_OUT(char const*, ...)> @ imm = #-0x114480
  438590: e59f35f4     	ldr	r3, [pc, #0x5f4]        @ 0x438b8c <MultiMenuManager::PushMenu(MenuBase*)+0x914>
  438594: e3a01001     	mov	r1, #1
  438598: e7972003     	ldr	r2, [r7, r3]
  43859c: e59f35ec     	ldr	r3, [pc, #0x5ec]        @ 0x438b90 <MultiMenuManager::PushMenu(MenuBase*)+0x918>
  4385a0: e5c21000     	strb	r1, [r2]
  4385a4: e7973003     	ldr	r3, [r7, r3]
  4385a8: e5c3a000     	strb	r10, [r3]
  4385ac: eaffff53     	b	0x438300 <MultiMenuManager::PushMenu(MenuBase*)+0x88> @ imm = #-0x2b4
  4385b0: e59f05e0     	ldr	r0, [pc, #0x5e0]        @ 0x438b98 <MultiMenuManager::PushMenu(MenuBase*)+0x920>
  4385b4: e08f0000     	add	r0, pc, r0
  4385b8: ebfbaed5     	bl	0x324114 <_DEBUG_OUT(char const*, ...)> @ imm = #-0x1144ac
  4385bc: e59f25c8     	ldr	r2, [pc, #0x5c8]        @ 0x438b8c <MultiMenuManager::PushMenu(MenuBase*)+0x914>
  4385c0: e3a03001     	mov	r3, #1
  4385c4: e7971002     	ldr	r1, [r7, r2]
  4385c8: e59f25c0     	ldr	r2, [pc, #0x5c0]        @ 0x438b90 <MultiMenuManager::PushMenu(MenuBase*)+0x918>
  4385cc: e5c13000     	strb	r3, [r1]
  4385d0: e7972002     	ldr	r2, [r7, r2]
  4385d4: e5c23000     	strb	r3, [r2]
  4385d8: eaffff42     	b	0x4382e8 <MultiMenuManager::PushMenu(MenuBase*)+0x70> @ imm = #-0x2f8
  4385dc: e1a00006     	mov	r0, r6
  4385e0: e5963000     	ldr	r3, [r6]
  4385e4: e1a0e00f     	mov	lr, pc
  4385e8: e593f024     	ldr	pc, [r3, #0x24]
  4385ec: eaffffab     	b	0x4384a0 <MultiMenuManager::PushMenu(MenuBase*)+0x228> @ imm = #-0x154
  4385f0: e1a00007     	mov	r0, r7
  4385f4: ebffff0a     	bl	0x438224 <gameswf::weak_ptr<gameswf::character>::get_ptr() const> @ imm = #-0x3d8
  4385f8: e59f259c     	ldr	r2, [pc, #0x59c]        @ 0x438b9c <MultiMenuManager::PushMenu(MenuBase*)+0x924>
  4385fc: e1a01000     	mov	r1, r0
  438600: e1a03005     	mov	r3, r5
  438604: e08f2002     	add	r2, pc, r2
  438608: e1a00006     	mov	r0, r6
  43860c: eb0dccfc     	bl	0x7aba04 <RenderFX::PlayAnim(gameswf::character*, char const*, int)> @ imm = #0x3733f0
  438610: e59630f8     	ldr	r3, [r6, #0xf8]
  438614: eaffff9f     	b	0x438498 <MultiMenuManager::PushMenu(MenuBase*)+0x220> @ imm = #-0x184
  438618: e594304c     	ldr	r3, [r4, #0x4c]
  43861c: e3530000     	cmp	r3, #0
  438620: 0a000003     	beq	0x438634 <MultiMenuManager::PushMenu(MenuBase*)+0x3bc> @ imm = #0xc
  438624: e5940048     	ldr	r0, [r4, #0x48]
  438628: e5d02004     	ldrb	r2, [r0, #0x4]
  43862c: e3520000     	cmp	r2, #0
  438630: 0a00001d     	beq	0x4386ac <MultiMenuManager::PushMenu(MenuBase*)+0x434> @ imm = #0x74
  438634: e1a00003     	mov	r0, r3
  438638: e3a01002     	mov	r1, #2
  43863c: e5933000     	ldr	r3, [r3]
  438640: e1a0e00f     	mov	lr, pc
  438644: e593f008     	ldr	pc, [r3, #0x8]
  438648: e3500000     	cmp	r0, #0
  43864c: 0affff81     	beq	0x438458 <MultiMenuManager::PushMenu(MenuBase*)+0x1e0> @ imm = #-0x1fc
  438650: e1a00007     	mov	r0, r7
  438654: ebfffef2     	bl	0x438224 <gameswf::weak_ptr<gameswf::character>::get_ptr() const> @ imm = #-0x438
  438658: e3a03001     	mov	r3, #1
  43865c: e5c030ea     	strb	r3, [r0, #0xea]
  438660: eaffff7c     	b	0x438458 <MultiMenuManager::PushMenu(MenuBase*)+0x1e0> @ imm = #-0x210
  438664: e5901000     	ldr	r1, [r0]
  438668: e2411001     	sub	r1, r1, #1
  43866c: e3510000     	cmp	r1, #0
  438670: e5801000     	str	r1, [r0]
  438674: 1a000000     	bne	0x43867c <MultiMenuManager::PushMenu(MenuBase*)+0x404> @ imm = #0x0
  438678: eb0c692e     	bl	0x752b38 <gameswf::free_internal(void*, unsigned int)> @ imm = #0x31a4b8
  43867c: e3a03000     	mov	r3, #0
  438680: e5843048     	str	r3, [r4, #0x48]
  438684: e584304c     	str	r3, [r4, #0x4c]
  438688: eaffff6d     	b	0x438444 <MultiMenuManager::PushMenu(MenuBase*)+0x1cc> @ imm = #-0x24c
  43868c: e59f350c     	ldr	r3, [pc, #0x50c]        @ 0x438ba0 <MultiMenuManager::PushMenu(MenuBase*)+0x928>
  438690: e7972003     	ldr	r2, [r7, r3]
  438694: e59f34e4     	ldr	r3, [pc, #0x4e4]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  438698: e5c20000     	strb	r0, [r2]
  43869c: e7973003     	ldr	r3, [r7, r3]
  4386a0: e3a02001     	mov	r2, #1
  4386a4: e5832000     	str	r2, [r3]
  4386a8: eaffff1e     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x388
  4386ac: e5901000     	ldr	r1, [r0]
  4386b0: e2411001     	sub	r1, r1, #1
  4386b4: e3510000     	cmp	r1, #0
  4386b8: e5801000     	str	r1, [r0]
  4386bc: 1a000000     	bne	0x4386c4 <MultiMenuManager::PushMenu(MenuBase*)+0x44c> @ imm = #0x0
  4386c0: eb0c691c     	bl	0x752b38 <gameswf::free_internal(void*, unsigned int)> @ imm = #0x31a470
  4386c4: e3a03000     	mov	r3, #0
  4386c8: e5843048     	str	r3, [r4, #0x48]
  4386cc: e584304c     	str	r3, [r4, #0x4c]
  4386d0: eaffffd7     	b	0x438634 <MultiMenuManager::PushMenu(MenuBase*)+0x3bc> @ imm = #-0xa4
  4386d4: e2847048     	add	r7, r4, #72
  4386d8: e1a00007     	mov	r0, r7
  4386dc: ebfffebb     	bl	0x4381d0 <gameswf::weak_ptr<gameswf::character>::operator->() const> @ imm = #-0x514
  4386e0: e3a01002     	mov	r1, #2
  4386e4: e5903000     	ldr	r3, [r0]
  4386e8: e1a0e00f     	mov	lr, pc
  4386ec: e593f008     	ldr	pc, [r3, #0x8]
  4386f0: e3500000     	cmp	r0, #0
  4386f4: 0affff33     	beq	0x4383c8 <MultiMenuManager::PushMenu(MenuBase*)+0x150> @ imm = #-0x334
  4386f8: e2890048     	add	r0, r9, #72
  4386fc: ebfffec8     	bl	0x438224 <gameswf::weak_ptr<gameswf::character>::get_ptr() const> @ imm = #-0x4e0
  438700: e3a03000     	mov	r3, #0
  438704: e5c030ea     	strb	r3, [r0, #0xea]
  438708: eaffff2e     	b	0x4383c8 <MultiMenuManager::PushMenu(MenuBase*)+0x150> @ imm = #-0x348
  43870c: eb118dee     	bl	0x89becc <ALicenseCheck_ValidateLicense> @ imm = #0x4637b8
  438710: eaffff84     	b	0x438528 <MultiMenuManager::PushMenu(MenuBase*)+0x2b0> @ imm = #-0x1f0
  438714: e2860f45     	add	r0, r6, #276
  438718: e08a10ca     	add	r1, r10, r10, asr #1
  43871c: ebfffd3e     	bl	0x437c1c <gameswf::array<MenuFX::State*>::reserve(int)> @ imm = #-0xb08
  438720: eaffff3a     	b	0x438410 <MultiMenuManager::PushMenu(MenuBase*)+0x198> @ imm = #-0x318
  438724: e59f1478     	ldr	r1, [pc, #0x478]        @ 0x438ba4 <MultiMenuManager::PushMenu(MenuBase*)+0x92c>
  438728: e1a00005     	mov	r0, r5
  43872c: e08f1001     	add	r1, pc, r1
  438730: ebfb56f9     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a41c
  438734: e3500000     	cmp	r0, #0
  438738: 1a00001d     	bne	0x4387b4 <MultiMenuManager::PushMenu(MenuBase*)+0x53c> @ imm = #0x74
  43873c: e59f343c     	ldr	r3, [pc, #0x43c]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  438740: e3a02003     	mov	r2, #3
  438744: e7973003     	ldr	r3, [r7, r3]
  438748: e5832000     	str	r2, [r3]
  43874c: eafffef5     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x42c
  438750: e289a048     	add	r10, r9, #72
  438754: e1a0000a     	mov	r0, r10
  438758: ebfffeb1     	bl	0x438224 <gameswf::weak_ptr<gameswf::character>::get_ptr() const> @ imm = #-0x53c
  43875c: e59f2444     	ldr	r2, [pc, #0x444]        @ 0x438ba8 <MultiMenuManager::PushMenu(MenuBase*)+0x930>
  438760: e1a0300b     	mov	r3, r11
  438764: e1a01000     	mov	r1, r0
  438768: e08f2002     	add	r2, pc, r2
  43876c: e1a00007     	mov	r0, r7
  438770: eb0dcca3     	bl	0x7aba04 <RenderFX::PlayAnim(gameswf::character*, char const*, int)> @ imm = #0x37328c
  438774: e250b000     	subs	r11, r0, #0
  438778: 13a03004     	movne	r3, #4
  43877c: 15893058     	strne	r3, [r9, #0x58]
  438780: 1affff06     	bne	0x4383a0 <MultiMenuManager::PushMenu(MenuBase*)+0x128> @ imm = #-0x3e8
  438784: e1a0000a     	mov	r0, r10
  438788: ebfffea5     	bl	0x438224 <gameswf::weak_ptr<gameswf::character>::get_ptr() const> @ imm = #-0x56c
  43878c: e59f2418     	ldr	r2, [pc, #0x418]        @ 0x438bac <MultiMenuManager::PushMenu(MenuBase*)+0x934>
  438790: e1a0300b     	mov	r3, r11
  438794: e1a01000     	mov	r1, r0
  438798: e08f2002     	add	r2, pc, r2
  43879c: e1a00007     	mov	r0, r7
  4387a0: eb0dcc97     	bl	0x7aba04 <RenderFX::PlayAnim(gameswf::character*, char const*, int)> @ imm = #0x37325c
  4387a4: e3500000     	cmp	r0, #0
  4387a8: 13a03002     	movne	r3, #2
  4387ac: 15893058     	strne	r3, [r9, #0x58]
  4387b0: eafffefa     	b	0x4383a0 <MultiMenuManager::PushMenu(MenuBase*)+0x128> @ imm = #-0x418
  4387b4: e59f13f4     	ldr	r1, [pc, #0x3f4]        @ 0x438bb0 <MultiMenuManager::PushMenu(MenuBase*)+0x938>
  4387b8: e1a00005     	mov	r0, r5
  4387bc: e08f1001     	add	r1, pc, r1
  4387c0: ebfb56d5     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a4ac
  4387c4: e3500000     	cmp	r0, #0
  4387c8: 1a000004     	bne	0x4387e0 <MultiMenuManager::PushMenu(MenuBase*)+0x568> @ imm = #0x10
  4387cc: e59f33ac     	ldr	r3, [pc, #0x3ac]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  4387d0: e3a02004     	mov	r2, #4
  4387d4: e7973003     	ldr	r3, [r7, r3]
  4387d8: e5832000     	str	r2, [r3]
  4387dc: eafffed1     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x4bc
  4387e0: e59f13cc     	ldr	r1, [pc, #0x3cc]        @ 0x438bb4 <MultiMenuManager::PushMenu(MenuBase*)+0x93c>
  4387e4: e1a00005     	mov	r0, r5
  4387e8: e08f1001     	add	r1, pc, r1
  4387ec: ebfb56ca     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a4d8
  4387f0: e3500000     	cmp	r0, #0
  4387f4: 1a000009     	bne	0x438820 <MultiMenuManager::PushMenu(MenuBase*)+0x5a8> @ imm = #0x24
  4387f8: e59f33a0     	ldr	r3, [pc, #0x3a0]        @ 0x438ba0 <MultiMenuManager::PushMenu(MenuBase*)+0x928>
  4387fc: e7973003     	ldr	r3, [r7, r3]
  438800: e5d33000     	ldrb	r3, [r3]
  438804: e3530000     	cmp	r3, #0
  438808: e59f3370     	ldr	r3, [pc, #0x370]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  43880c: 13a02005     	movne	r2, #5
  438810: 03a02006     	moveq	r2, #6
  438814: e7973003     	ldr	r3, [r7, r3]
  438818: e5832000     	str	r2, [r3]
  43881c: eafffec1     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x4fc
  438820: e59f1390     	ldr	r1, [pc, #0x390]        @ 0x438bb8 <MultiMenuManager::PushMenu(MenuBase*)+0x940>
  438824: e1a00005     	mov	r0, r5
  438828: e08f1001     	add	r1, pc, r1
  43882c: ebfb56ba     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a518
  438830: e3500000     	cmp	r0, #0
  438834: 1a000004     	bne	0x43884c <MultiMenuManager::PushMenu(MenuBase*)+0x5d4> @ imm = #0x10
  438838: e59f3340     	ldr	r3, [pc, #0x340]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  43883c: e3a02007     	mov	r2, #7
  438840: e7973003     	ldr	r3, [r7, r3]
  438844: e5832000     	str	r2, [r3]
  438848: eafffeb6     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x528
  43884c: e59f1368     	ldr	r1, [pc, #0x368]        @ 0x438bbc <MultiMenuManager::PushMenu(MenuBase*)+0x944>
  438850: e1a00005     	mov	r0, r5
  438854: e08f1001     	add	r1, pc, r1
  438858: ebfb56af     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a544
  43885c: e3500000     	cmp	r0, #0
  438860: 1a000004     	bne	0x438878 <MultiMenuManager::PushMenu(MenuBase*)+0x600> @ imm = #0x10
  438864: e59f3314     	ldr	r3, [pc, #0x314]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  438868: e3a02008     	mov	r2, #8
  43886c: e7973003     	ldr	r3, [r7, r3]
  438870: e5832000     	str	r2, [r3]
  438874: eafffeab     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x554
  438878: e59f1340     	ldr	r1, [pc, #0x340]        @ 0x438bc0 <MultiMenuManager::PushMenu(MenuBase*)+0x948>
  43887c: e1a00005     	mov	r0, r5
  438880: e08f1001     	add	r1, pc, r1
  438884: ebfb56a4     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a570
  438888: e3500000     	cmp	r0, #0
  43888c: 1a000004     	bne	0x4388a4 <MultiMenuManager::PushMenu(MenuBase*)+0x62c> @ imm = #0x10
  438890: e59f32e8     	ldr	r3, [pc, #0x2e8]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  438894: e3a02009     	mov	r2, #9
  438898: e7973003     	ldr	r3, [r7, r3]
  43889c: e5832000     	str	r2, [r3]
  4388a0: eafffea0     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x580
  4388a4: e59f1318     	ldr	r1, [pc, #0x318]        @ 0x438bc4 <MultiMenuManager::PushMenu(MenuBase*)+0x94c>
  4388a8: e1a00005     	mov	r0, r5
  4388ac: e08f1001     	add	r1, pc, r1
  4388b0: ebfb5699     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a59c
  4388b4: e3500000     	cmp	r0, #0
  4388b8: 1a000008     	bne	0x4388e0 <MultiMenuManager::PushMenu(MenuBase*)+0x668> @ imm = #0x20
  4388bc: e59f32dc     	ldr	r3, [pc, #0x2dc]        @ 0x438ba0 <MultiMenuManager::PushMenu(MenuBase*)+0x928>
  4388c0: e3a01001     	mov	r1, #1
  4388c4: e7972003     	ldr	r2, [r7, r3]
  4388c8: e59f32b0     	ldr	r3, [pc, #0x2b0]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  4388cc: e5c21000     	strb	r1, [r2]
  4388d0: e7973003     	ldr	r3, [r7, r3]
  4388d4: e3a0200a     	mov	r2, #10
  4388d8: e5832000     	str	r2, [r3]
  4388dc: eafffe91     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x5bc
  4388e0: e59f12e0     	ldr	r1, [pc, #0x2e0]        @ 0x438bc8 <MultiMenuManager::PushMenu(MenuBase*)+0x950>
  4388e4: e1a00005     	mov	r0, r5
  4388e8: e08f1001     	add	r1, pc, r1
  4388ec: ebfb568a     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a5d8
  4388f0: e3500000     	cmp	r0, #0
  4388f4: 1a000004     	bne	0x43890c <MultiMenuManager::PushMenu(MenuBase*)+0x694> @ imm = #0x10
  4388f8: e59f3280     	ldr	r3, [pc, #0x280]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  4388fc: e3a0200b     	mov	r2, #11
  438900: e7973003     	ldr	r3, [r7, r3]
  438904: e5832000     	str	r2, [r3]
  438908: eafffe86     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x5e8
  43890c: e59f12b8     	ldr	r1, [pc, #0x2b8]        @ 0x438bcc <MultiMenuManager::PushMenu(MenuBase*)+0x954>
  438910: e1a00005     	mov	r0, r5
  438914: e08f1001     	add	r1, pc, r1
  438918: ebfb567f     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a604
  43891c: e3500000     	cmp	r0, #0
  438920: 0a000016     	beq	0x438980 <MultiMenuManager::PushMenu(MenuBase*)+0x708> @ imm = #0x58
  438924: e59f12a4     	ldr	r1, [pc, #0x2a4]        @ 0x438bd0 <MultiMenuManager::PushMenu(MenuBase*)+0x958>
  438928: e1a00005     	mov	r0, r5
  43892c: e08f1001     	add	r1, pc, r1
  438930: ebfb5679     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a61c
  438934: e3500000     	cmp	r0, #0
  438938: 0a00002c     	beq	0x4389f0 <MultiMenuManager::PushMenu(MenuBase*)+0x778> @ imm = #0xb0
  43893c: e59f1290     	ldr	r1, [pc, #0x290]        @ 0x438bd4 <MultiMenuManager::PushMenu(MenuBase*)+0x95c>
  438940: e1a00005     	mov	r0, r5
  438944: e08f1001     	add	r1, pc, r1
  438948: ebfb5673     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a634
  43894c: e3500000     	cmp	r0, #0
  438950: 0a00001d     	beq	0x4389cc <MultiMenuManager::PushMenu(MenuBase*)+0x754> @ imm = #0x74
  438954: e59f127c     	ldr	r1, [pc, #0x27c]        @ 0x438bd8 <MultiMenuManager::PushMenu(MenuBase*)+0x960>
  438958: e1a00005     	mov	r0, r5
  43895c: e08f1001     	add	r1, pc, r1
  438960: ebfb566d     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a64c
  438964: e3500000     	cmp	r0, #0
  438968: 1a00000c     	bne	0x4389a0 <MultiMenuManager::PushMenu(MenuBase*)+0x728> @ imm = #0x30
  43896c: e59f320c     	ldr	r3, [pc, #0x20c]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  438970: e3a02011     	mov	r2, #17
  438974: e7973003     	ldr	r3, [r7, r3]
  438978: e5832000     	str	r2, [r3]
  43897c: eafffe69     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x65c
  438980: e59f31f8     	ldr	r3, [pc, #0x1f8]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  438984: e59f0250     	ldr	r0, [pc, #0x250]        @ 0x438bdc <MultiMenuManager::PushMenu(MenuBase*)+0x964>
  438988: e3a02013     	mov	r2, #19
  43898c: e7973003     	ldr	r3, [r7, r3]
  438990: e08f0000     	add	r0, pc, r0
  438994: e5832000     	str	r2, [r3]
  438998: ebfbaddd     	bl	0x324114 <_DEBUG_OUT(char const*, ...)> @ imm = #-0x11488c
  43899c: eafffe61     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x67c
  4389a0: e59f1238     	ldr	r1, [pc, #0x238]        @ 0x438be0 <MultiMenuManager::PushMenu(MenuBase*)+0x968>
  4389a4: e1a00005     	mov	r0, r5
  4389a8: e08f1001     	add	r1, pc, r1
  4389ac: ebfb565a     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a698
  4389b0: e3500000     	cmp	r0, #0
  4389b4: 1a000015     	bne	0x438a10 <MultiMenuManager::PushMenu(MenuBase*)+0x798> @ imm = #0x54
  4389b8: e59f31c0     	ldr	r3, [pc, #0x1c0]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  4389bc: e3a02010     	mov	r2, #16
  4389c0: e7973003     	ldr	r3, [r7, r3]
  4389c4: e5832000     	str	r2, [r3]
  4389c8: eafffe56     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x6a8
  4389cc: e59f21ac     	ldr	r2, [pc, #0x1ac]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  4389d0: e59f020c     	ldr	r0, [pc, #0x20c]        @ 0x438be4 <MultiMenuManager::PushMenu(MenuBase*)+0x96c>
  4389d4: e3a03012     	mov	r3, #18
  4389d8: e7972002     	ldr	r2, [r7, r2]
  4389dc: e08f0000     	add	r0, pc, r0
  4389e0: e1a01003     	mov	r1, r3
  4389e4: e5823000     	str	r3, [r2]
  4389e8: ebfbadc9     	bl	0x324114 <_DEBUG_OUT(char const*, ...)> @ imm = #-0x1148dc
  4389ec: eafffe4d     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x6cc
  4389f0: e59f3188     	ldr	r3, [pc, #0x188]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  4389f4: e59f01ec     	ldr	r0, [pc, #0x1ec]        @ 0x438be8 <MultiMenuManager::PushMenu(MenuBase*)+0x970>
  4389f8: e3a02013     	mov	r2, #19
  4389fc: e7973003     	ldr	r3, [r7, r3]
  438a00: e08f0000     	add	r0, pc, r0
  438a04: e5832000     	str	r2, [r3]
  438a08: ebfbadc1     	bl	0x324114 <_DEBUG_OUT(char const*, ...)> @ imm = #-0x1148fc
  438a0c: eafffe45     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x6ec
  438a10: e59f11d4     	ldr	r1, [pc, #0x1d4]        @ 0x438bec <MultiMenuManager::PushMenu(MenuBase*)+0x974>
  438a14: e1a00005     	mov	r0, r5
  438a18: e08f1001     	add	r1, pc, r1
  438a1c: ebfb563e     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a708
  438a20: e3500000     	cmp	r0, #0
  438a24: 1a000004     	bne	0x438a3c <MultiMenuManager::PushMenu(MenuBase*)+0x7c4> @ imm = #0x10
  438a28: e59f3150     	ldr	r3, [pc, #0x150]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  438a2c: e3a0200f     	mov	r2, #15
  438a30: e7973003     	ldr	r3, [r7, r3]
  438a34: e5832000     	str	r2, [r3]
  438a38: eafffe3a     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x718
  438a3c: e59f11ac     	ldr	r1, [pc, #0x1ac]        @ 0x438bf0 <MultiMenuManager::PushMenu(MenuBase*)+0x978>
  438a40: e1a00005     	mov	r0, r5
  438a44: e08f1001     	add	r1, pc, r1
  438a48: ebfb5633     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a734
  438a4c: e3500000     	cmp	r0, #0
  438a50: 1a000004     	bne	0x438a68 <MultiMenuManager::PushMenu(MenuBase*)+0x7f0> @ imm = #0x10
  438a54: e59f3124     	ldr	r3, [pc, #0x124]        @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  438a58: e3a0200e     	mov	r2, #14
  438a5c: e7973003     	ldr	r3, [r7, r3]
  438a60: e5832000     	str	r2, [r3]
  438a64: eafffe2f     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x744
  438a68: e59f1184     	ldr	r1, [pc, #0x184]        @ 0x438bf4 <MultiMenuManager::PushMenu(MenuBase*)+0x97c>
  438a6c: e1a00005     	mov	r0, r5
  438a70: e08f1001     	add	r1, pc, r1
  438a74: ebfb5628     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a760
  438a78: e3500000     	cmp	r0, #0
  438a7c: 0a000029     	beq	0x438b28 <MultiMenuManager::PushMenu(MenuBase*)+0x8b0> @ imm = #0xa4
  438a80: e59f1170     	ldr	r1, [pc, #0x170]        @ 0x438bf8 <MultiMenuManager::PushMenu(MenuBase*)+0x980>
  438a84: e1a00005     	mov	r0, r5
  438a88: e08f1001     	add	r1, pc, r1
  438a8c: ebfb5622     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a778
  438a90: e3500000     	cmp	r0, #0
  438a94: 0a000023     	beq	0x438b28 <MultiMenuManager::PushMenu(MenuBase*)+0x8b0> @ imm = #0x8c
  438a98: e59f115c     	ldr	r1, [pc, #0x15c]        @ 0x438bfc <MultiMenuManager::PushMenu(MenuBase*)+0x984>
  438a9c: e1a00005     	mov	r0, r5
  438aa0: e08f1001     	add	r1, pc, r1
  438aa4: ebfb561c     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a790
  438aa8: e3500000     	cmp	r0, #0
  438aac: 0a00001d     	beq	0x438b28 <MultiMenuManager::PushMenu(MenuBase*)+0x8b0> @ imm = #0x74
  438ab0: e59f1148     	ldr	r1, [pc, #0x148]        @ 0x438c00 <MultiMenuManager::PushMenu(MenuBase*)+0x988>
  438ab4: e1a00005     	mov	r0, r5
  438ab8: e08f1001     	add	r1, pc, r1
  438abc: ebfb5616     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a7a8
  438ac0: e3500000     	cmp	r0, #0
  438ac4: 0a000017     	beq	0x438b28 <MultiMenuManager::PushMenu(MenuBase*)+0x8b0> @ imm = #0x5c
  438ac8: e59f1134     	ldr	r1, [pc, #0x134]        @ 0x438c04 <MultiMenuManager::PushMenu(MenuBase*)+0x98c>
  438acc: e1a00005     	mov	r0, r5
  438ad0: e08f1001     	add	r1, pc, r1
  438ad4: ebfb5610     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a7c0
  438ad8: e3500000     	cmp	r0, #0
  438adc: 0a000011     	beq	0x438b28 <MultiMenuManager::PushMenu(MenuBase*)+0x8b0> @ imm = #0x44
  438ae0: e59f1120     	ldr	r1, [pc, #0x120]        @ 0x438c08 <MultiMenuManager::PushMenu(MenuBase*)+0x990>
  438ae4: e1a00005     	mov	r0, r5
  438ae8: e08f1001     	add	r1, pc, r1
  438aec: ebfb560a     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a7d8
  438af0: e3500000     	cmp	r0, #0
  438af4: 0a00000b     	beq	0x438b28 <MultiMenuManager::PushMenu(MenuBase*)+0x8b0> @ imm = #0x2c
  438af8: e59f110c     	ldr	r1, [pc, #0x10c]        @ 0x438c0c <MultiMenuManager::PushMenu(MenuBase*)+0x994>
  438afc: e1a00005     	mov	r0, r5
  438b00: e08f1001     	add	r1, pc, r1
  438b04: ebfb5604     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a7f0
  438b08: e3500000     	cmp	r0, #0
  438b0c: 0a000005     	beq	0x438b28 <MultiMenuManager::PushMenu(MenuBase*)+0x8b0> @ imm = #0x14
  438b10: e59f10f8     	ldr	r1, [pc, #0xf8]         @ 0x438c10 <MultiMenuManager::PushMenu(MenuBase*)+0x998>
  438b14: e1a00005     	mov	r0, r5
  438b18: e08f1001     	add	r1, pc, r1
  438b1c: ebfb55fe     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x12a808
  438b20: e3500000     	cmp	r0, #0
  438b24: 1a000004     	bne	0x438b3c <MultiMenuManager::PushMenu(MenuBase*)+0x8c4> @ imm = #0x10
  438b28: e59f3050     	ldr	r3, [pc, #0x50]         @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  438b2c: e3a0200c     	mov	r2, #12
  438b30: e7973003     	ldr	r3, [r7, r3]
  438b34: e5832000     	str	r2, [r3]
  438b38: eafffdfa     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x818
  438b3c: e59f303c     	ldr	r3, [pc, #0x3c]         @ 0x438b80 <MultiMenuManager::PushMenu(MenuBase*)+0x908>
  438b40: e3a0200d     	mov	r2, #13
  438b44: e7973003     	ldr	r3, [r7, r3]
  438b48: e5832000     	str	r2, [r3]
  438b4c: eafffdf5     	b	0x438328 <MultiMenuManager::PushMenu(MenuBase*)+0xb0> @ imm = #-0x82c
  438b50: f4 c7 55 00  	.word	0x0055c7f4
  438b54: f0 34 49 00  	.word	0x004934f0
  438b58: 00 35 49 00  	.word	0x00493500
  438b5c: 38 35 49 00  	.word	0x00493538
  438b60: e0 6d 48 00  	.word	0x00486de0
  438b64: e0 0e 49 00  	.word	0x00490ee0
  438b68: a4 1e 00 00  	.word	0x00001ea4
  438b6c: 2c 1e 00 00  	.word	0x00001e2c
  438b70: 14 37 49 00  	.word	0x00493714
  438b74: 3c 36 49 00  	.word	0x0049363c
  438b78: f8 6b 48 00  	.word	0x00486bf8
  438b7c: c0 33 49 00  	.word	0x004933c0
  438b80: 50 38 00 00  	.word	0x00003850
  438b84: 0c 21 00 00  	.word	0x0000210c
  438b88: a4 32 49 00  	.word	0x004932a4
  438b8c: d4 29 00 00  	.word	0x000029d4
  438b90: dc 2b 00 00  	.word	0x00002bdc
  438b94: f0 32 49 00  	.word	0x004932f0
  438b98: 7c 32 49 00  	.word	0x0049327c
  438b9c: bc 34 49 00  	.word	0x004934bc
  438ba0: 50 45 00 00  	.word	0x00004550
  438ba4: 04 0b 49 00  	.word	0x00490b04
  438ba8: 38 33 49 00  	.word	0x00493338
  438bac: 18 33 49 00  	.word	0x00493318
  438bb0: 04 31 49 00  	.word	0x00493104
  438bb4: e8 30 49 00  	.word	0x004930e8
  438bb8: 20 13 49 00  	.word	0x00491320
  438bbc: 7c 97 48 00  	.word	0x0048977c
  438bc0: c0 64 48 00  	.word	0x004864c0
  438bc4: 6c 64 48 00  	.word	0x0048646c
  438bc8: 48 96 48 00  	.word	0x00489648
  438bcc: 4c 12 49 00  	.word	0x0049124c
  438bd0: 14 30 49 00  	.word	0x00493014
  438bd4: 94 08 49 00  	.word	0x00490894
  438bd8: e4 0f 49 00  	.word	0x00490fe4
  438bdc: 58 2f 49 00  	.word	0x00492f58
  438be0: c8 11 49 00  	.word	0x004911c8
  438be4: cc 2f 49 00  	.word	0x00492fcc
  438be8: 50 2f 49 00  	.word	0x00492f50
  438bec: 98 93 48 00  	.word	0x00489398
  438bf0: b4 2f 49 00  	.word	0x00492fb4
  438bf4: 90 62 48 00  	.word	0x00486290
  438bf8: 80 2f 49 00  	.word	0x00492f80
  438bfc: 80 2f 49 00  	.word	0x00492f80
  438c00: 88 2f 49 00  	.word	0x00492f88
  438c04: 88 2f 49 00  	.word	0x00492f88
  438c08: 88 2f 49 00  	.word	0x00492f88
  438c0c: 80 2f 49 00  	.word	0x00492f80
  438c10: e0 2b 49 00  	.word	0x00492be0
