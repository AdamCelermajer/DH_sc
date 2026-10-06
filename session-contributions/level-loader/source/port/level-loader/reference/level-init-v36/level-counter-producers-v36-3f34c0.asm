
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f34c0 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)>:
  3f34c0: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3f34c4: e59fc360     	ldr	r12, [pc, #0x360]       @ 0x3f382c <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x36c>
  3f34c8: e24dde42     	sub	sp, sp, #1056
  3f34cc: e24dd00c     	sub	sp, sp, #12
  3f34d0: e58dc01c     	str	r12, [sp, #0x1c]
  3f34d4: e59f9354     	ldr	r9, [pc, #0x354]        @ 0x3f3830 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x370>
  3f34d8: e1a0c003     	mov	r12, r3
  3f34dc: e59d301c     	ldr	r3, [sp, #0x1c]
  3f34e0: e08f9009     	add	r9, pc, r9
  3f34e4: e1a04000     	mov	r4, r0
  3f34e8: e799e003     	ldr	lr, [r9, r3]
  3f34ec: e1a03002     	mov	r3, r2
  3f34f0: e1a08001     	mov	r8, r1
  3f34f4: e59e2000     	ldr	r2, [lr]
  3f34f8: e5ddb458     	ldrb	r11, [sp, #0x458]
  3f34fc: e58d3018     	str	r3, [sp, #0x18]
  3f3500: e58dc014     	str	r12, [sp, #0x14]
  3f3504: e58d2424     	str	r2, [sp, #0x424]
  3f3508: e5dda45c     	ldrb	r10, [sp, #0x45c]
  3f350c: ebfd12d2     	bl	0x33805c <EventManager::EventManager()> @ imm = #-0xbb4b8
  3f3510: e59f231c     	ldr	r2, [pc, #0x31c]        @ 0x3f3834 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x374>
  3f3514: e3a06000     	mov	r6, #0
  3f3518: e3e05000     	mvn	r5, #0
  3f351c: e7992002     	ldr	r2, [r9, r2]
  3f3520: e2847044     	add	r7, r4, #68
  3f3524: e1a01006     	mov	r1, r6
  3f3528: e2822008     	add	r2, r2, #8
  3f352c: e5842000     	str	r2, [r4]
  3f3530: e5846030     	str	r6, [r4, #0x30]
  3f3534: e5846038     	str	r6, [r4, #0x38]
  3f3538: e584503c     	str	r5, [r4, #0x3c]
  3f353c: e5845040     	str	r5, [r4, #0x40]
  3f3540: e1a00007     	mov	r0, r7
  3f3544: ebfe240e     	bl	0x37c584 <LuaScript::LuaScript(bool)> @ imm = #-0x76fc8
  3f3548: e59d2450     	ldr	r2, [sp, #0x450]
  3f354c: e1a01008     	mov	r1, r8
  3f3550: e28400f8     	add	r0, r4, #248
  3f3554: e58420dc     	str	r2, [r4, #0xdc]
  3f3558: e59d2454     	ldr	r2, [sp, #0x454]
  3f355c: e5c4b0f1     	strb	r11, [r4, #0xf1]
  3f3560: e5c4a0f2     	strb	r10, [r4, #0xf2]
  3f3564: e58420e0     	str	r2, [r4, #0xe0]
  3f3568: e3a02001     	mov	r2, #1
  3f356c: e58420e4     	str	r2, [r4, #0xe4]
  3f3570: e28d2028     	add	r2, sp, #40
  3f3574: e2422008     	sub	r2, r2, #8
  3f3578: e58460ec     	str	r6, [r4, #0xec]
  3f357c: e5c460f0     	strb	r6, [r4, #0xf0]
  3f3580: e5c460f3     	strb	r6, [r4, #0xf3]
  3f3584: e5c460f4     	strb	r6, [r4, #0xf4]
  3f3588: e5c460f5     	strb	r6, [r4, #0xf5]
  3f358c: ebfc82d6     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xdf4a8
  3f3590: e59d3018     	ldr	r3, [sp, #0x18]
  3f3594: e59f229c     	ldr	r2, [pc, #0x29c]        @ 0x3f3838 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x378>
  3f3598: e3a01000     	mov	r1, #0
  3f359c: e5843110     	str	r3, [r4, #0x110]
  3f35a0: e59dc014     	ldr	r12, [sp, #0x14]
  3f35a4: e08f2002     	add	r2, pc, r2
  3f35a8: e28400ac     	add	r0, r4, #172
  3f35ac: e584c114     	str	r12, [r4, #0x114]
  3f35b0: e59d3464     	ldr	r3, [sp, #0x464]
  3f35b4: e5846130     	str	r6, [r4, #0x130]
  3f35b8: e58411a4     	str	r1, [r4, #0x1a4]
  3f35bc: e5846134     	str	r6, [r4, #0x134]
  3f35c0: e5841160     	str	r1, [r4, #0x160]
  3f35c4: e5846138     	str	r6, [r4, #0x138]
  3f35c8: e5841164     	str	r1, [r4, #0x164]
  3f35cc: e5841168     	str	r1, [r4, #0x168]
  3f35d0: e584119c     	str	r1, [r4, #0x19c]
  3f35d4: e58411a0     	str	r1, [r4, #0x1a0]
  3f35d8: e5843118     	str	r3, [r4, #0x118]
  3f35dc: e584511c     	str	r5, [r4, #0x11c]
  3f35e0: e5845120     	str	r5, [r4, #0x120]
  3f35e4: e5845124     	str	r5, [r4, #0x124]
  3f35e8: e5846128     	str	r6, [r4, #0x128]
  3f35ec: e584612c     	str	r6, [r4, #0x12c]
  3f35f0: e584613c     	str	r6, [r4, #0x13c]
  3f35f4: e5846140     	str	r6, [r4, #0x140]
  3f35f8: e5c46144     	strb	r6, [r4, #0x144]
  3f35fc: e5c46145     	strb	r6, [r4, #0x145]
  3f3600: e5845148     	str	r5, [r4, #0x148]
  3f3604: e584614c     	str	r6, [r4, #0x14c]
  3f3608: e5846150     	str	r6, [r4, #0x150]
  3f360c: e5846154     	str	r6, [r4, #0x154]
  3f3610: e5846158     	str	r6, [r4, #0x158]
  3f3614: e584615c     	str	r6, [r4, #0x15c]
  3f3618: e5846194     	str	r6, [r4, #0x194]
  3f361c: e5c46198     	strb	r6, [r4, #0x198]
  3f3620: e5c461a8     	strb	r6, [r4, #0x1a8]
  3f3624: e5923000     	ldr	r3, [r2]
  3f3628: e59f120c     	ldr	r1, [pc, #0x20c]        @ 0x3f383c <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x37c>
  3f362c: e59fb20c     	ldr	r11, [pc, #0x20c]       @ 0x3f3840 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x380>
  3f3630: e2833001     	add	r3, r3, #1
  3f3634: e08f1001     	add	r1, pc, r1
  3f3638: e5823000     	str	r3, [r2]
  3f363c: e281200d     	add	r2, r1, #13
  3f3640: ebfc74e6     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0xe2c68
  3f3644: e59f11f8     	ldr	r1, [pc, #0x1f8]        @ 0x3f3844 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x384>
  3f3648: e1a00007     	mov	r0, r7
  3f364c: e08f1001     	add	r1, pc, r1
  3f3650: ebfe1fc7     	bl	0x37b574 <LuaScript::Load(char const*)> @ imm = #-0x780e4
  3f3654: e59f11ec     	ldr	r1, [pc, #0x1ec]        @ 0x3f3848 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x388>
  3f3658: e1a00007     	mov	r0, r7
  3f365c: e08f1001     	add	r1, pc, r1
  3f3660: ebfe1fc3     	bl	0x37b574 <LuaScript::Load(char const*)> @ imm = #-0x780f4
  3f3664: e59f21e0     	ldr	r2, [pc, #0x1e0]        @ 0x3f384c <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x38c>
  3f3668: e799300b     	ldr	r3, [r9, r11]
  3f366c: e584518c     	str	r5, [r4, #0x18c]
  3f3670: e7992002     	ldr	r2, [r9, r2]
  3f3674: e5c4616c     	strb	r6, [r4, #0x16c]
  3f3678: e5826000     	str	r6, [r2]
  3f367c: e5c460e8     	strb	r6, [r4, #0xe8]
  3f3680: e5933000     	ldr	r3, [r3]
  3f3684: e1530006     	cmp	r3, r6
  3f3688: 0a00001e     	beq	0x3f3708 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x248> @ imm = #0x78
  3f368c: e59f31bc     	ldr	r3, [pc, #0x1bc]        @ 0x3f3850 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x390>
  3f3690: e28d5028     	add	r5, sp, #40
  3f3694: e2455004     	sub	r5, r5, #4
  3f3698: e799a003     	ldr	r10, [r9, r3]
  3f369c: e1a07006     	mov	r7, r6
  3f36a0: ea000005     	b	0x3f36bc <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x1fc> @ imm = #0x14
  3f36a4: e799300b     	ldr	r3, [r9, r11]
  3f36a8: e2877001     	add	r7, r7, #1
  3f36ac: e2866048     	add	r6, r6, #72
  3f36b0: e5933000     	ldr	r3, [r3]
  3f36b4: e1530007     	cmp	r3, r7
  3f36b8: 9a000012     	bls	0x3f3708 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x248> @ imm = #0x48
  3f36bc: e59a8000     	ldr	r8, [r10]
  3f36c0: e1a00005     	mov	r0, r5
  3f36c4: e0888006     	add	r8, r8, r6
  3f36c8: e5981020     	ldr	r1, [r8, #0x20]
  3f36cc: ebfc6b93     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0xe51b4
  3f36d0: e1a00005     	mov	r0, r5
  3f36d4: e3a01000     	mov	r1, #0
  3f36d8: e3e02000     	mvn	r2, #0
  3f36dc: ebfd6b4c     	bl	0x34e414 <ToLowerCase(char*, int, int)> @ imm = #-0xa52d0
  3f36e0: e594010c     	ldr	r0, [r4, #0x10c]
  3f36e4: e1a01005     	mov	r1, r5
  3f36e8: ebfc6d39     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0xe4b1c
  3f36ec: e3500000     	cmp	r0, #0
  3f36f0: 0affffeb     	beq	0x3f36a4 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x1e4> @ imm = #-0x54
  3f36f4: e5d83014     	ldrb	r3, [r8, #0x14]
  3f36f8: e584703c     	str	r7, [r4, #0x3c]
  3f36fc: e5c430e8     	strb	r3, [r4, #0xe8]
  3f3700: e5983010     	ldr	r3, [r8, #0x10]
  3f3704: e5843040     	str	r3, [r4, #0x40]
  3f3708: eb102821     	bl	0x7fd794 <GetOnline()>  @ imm = #0x40a084
  3f370c: e5d03005     	ldrb	r3, [r0, #0x5]
  3f3710: e3530000     	cmp	r3, #0
  3f3714: 1a00002a     	bne	0x3f37c4 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x304> @ imm = #0xa8
  3f3718: e594303c     	ldr	r3, [r4, #0x3c]
  3f371c: e3730001     	cmn	r3, #1
  3f3720: 0a00001d     	beq	0x3f379c <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x2dc> @ imm = #0x74
  3f3724: e59dc460     	ldr	r12, [sp, #0x460]
  3f3728: e37c0001     	cmn	r12, #1
  3f372c: 0a00000b     	beq	0x3f3760 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x2a0> @ imm = #0x2c
  3f3730: e5943040     	ldr	r3, [r4, #0x40]
  3f3734: e3a02001     	mov	r2, #1
  3f3738: e5c420f5     	strb	r2, [r4, #0xf5]
  3f373c: e19cc003     	orrs	r12, r12, r3
  3f3740: 0a000004     	beq	0x3f3758 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x298> @ imm = #0x10
  3f3744: e59d2460     	ldr	r2, [sp, #0x460]
  3f3748: e1520003     	cmp	r2, r3
  3f374c: 0a000003     	beq	0x3f3760 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x2a0> @ imm = #0xc
  3f3750: e3530000     	cmp	r3, #0
  3f3754: 0a000001     	beq	0x3f3760 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x2a0> @ imm = #0x4
  3f3758: e3a03001     	mov	r3, #1
  3f375c: e5c430f3     	strb	r3, [r4, #0xf3]
  3f3760: e3a01000     	mov	r1, #0
  3f3764: e3a0003c     	mov	r0, #60
  3f3768: ebfc7380     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xe3200
  3f376c: e594c118     	ldr	r12, [r4, #0x118]
  3f3770: e594e03c     	ldr	lr, [r4, #0x3c]
  3f3774: e5942114     	ldr	r2, [r4, #0x114]
  3f3778: e5943040     	ldr	r3, [r4, #0x40]
  3f377c: e1a05000     	mov	r5, r0
  3f3780: e58dc004     	str	r12, [sp, #0x4]
  3f3784: e1a01004     	mov	r1, r4
  3f3788: e3a0c000     	mov	r12, #0
  3f378c: e58de000     	str	lr, [sp]
  3f3790: e58dc008     	str	r12, [sp, #0x8]
  3f3794: eb01bc66     	bl	0x462934 <LevelSavegame::LevelSavegame(Level*, unsigned int, int, int, int, bool)> @ imm = #0x6f198
  3f3798: e58450ec     	str	r5, [r4, #0xec]
  3f379c: e59dc01c     	ldr	r12, [sp, #0x1c]
  3f37a0: e59d2424     	ldr	r2, [sp, #0x424]
  3f37a4: e1a00004     	mov	r0, r4
  3f37a8: e799300c     	ldr	r3, [r9, r12]
  3f37ac: e5933000     	ldr	r3, [r3]
  3f37b0: e1520003     	cmp	r2, r3
  3f37b4: 1a00001b     	bne	0x3f3828 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x368> @ imm = #0x6c
  3f37b8: e28dd02c     	add	sp, sp, #44
  3f37bc: e28ddb01     	add	sp, sp, #1024
  3f37c0: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3f37c4: e59f5088     	ldr	r5, [pc, #0x88]         @ 0x3f3854 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x394>
  3f37c8: e7993005     	ldr	r3, [r9, r5]
  3f37cc: e5930040     	ldr	r0, [r3, #0x40]
  3f37d0: ebfdee27     	bl	0x36f074 <PlayerManager::IsLocalPlayerHosting()> @ imm = #-0x84764
  3f37d4: e3500000     	cmp	r0, #0
  3f37d8: 0a000005     	beq	0x3f37f4 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x334> @ imm = #0x14
  3f37dc: e7993005     	ldr	r3, [r9, r5]
  3f37e0: e5933040     	ldr	r3, [r3, #0x40]
  3f37e4: e5d33719     	ldrb	r3, [r3, #0x719]
  3f37e8: e3530000     	cmp	r3, #0
  3f37ec: 0affffc9     	beq	0x3f3718 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x258> @ imm = #-0xdc
  3f37f0: ea000004     	b	0x3f3808 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x348> @ imm = #0x10
  3f37f4: ebfcb5a7     	bl	0x320e98 <OnlineSingleton<OnlineGameState>::GetInstance()> @ imm = #-0xd2964
  3f37f8: e5903034     	ldr	r3, [r0, #0x34]
  3f37fc: e2433003     	sub	r3, r3, #3
  3f3800: e3530001     	cmp	r3, #1
  3f3804: 9a000002     	bls	0x3f3814 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x354> @ imm = #0x8
  3f3808: e3a03000     	mov	r3, #0
  3f380c: e5c430f1     	strb	r3, [r4, #0xf1]
  3f3810: eaffffc0     	b	0x3f3718 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x258> @ imm = #-0x100
  3f3814: eb1035dc     	bl	0x800f8c <CMatching::Get()> @ imm = #0x40d770
  3f3818: eb10af41     	bl	0x81f524 <CMatchingGLLive::IsHost()> @ imm = #0x42bd04
  3f381c: e3500000     	cmp	r0, #0
  3f3820: 0afffff8     	beq	0x3f3808 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x348> @ imm = #-0x20
  3f3824: eaffffec     	b	0x3f37dc <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x31c> @ imm = #-0x50
  3f3828: ebfc6ab8     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xe5520
  3f382c: ac 40 00 00  	.word	0x000040ac
  3f3830: b0 15 5a 00  	.word	0x005a15b0
  3f3834: 74 07 00 00  	.word	0x00000774
  3f3838: 3c fb 5a 00  	.word	0x005afb3c
  3f383c: b4 30 4d 00  	.word	0x004d30b4
  3f3840: c0 18 00 00  	.word	0x000018c0
  3f3844: ac 30 4d 00  	.word	0x004d30ac
  3f3848: b4 30 4d 00  	.word	0x004d30b4
  3f384c: 2c 28 00 00  	.word	0x0000282c
  3f3850: 74 08 00 00  	.word	0x00000874
  3f3854: f4 37 00 00  	.word	0x000037f4
