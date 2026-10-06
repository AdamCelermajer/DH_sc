
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f3128 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)>:
  3f3128: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3f312c: e59fc360     	ldr	r12, [pc, #0x360]       @ 0x3f3494 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x36c>
  3f3130: e24dde42     	sub	sp, sp, #1056
  3f3134: e24dd00c     	sub	sp, sp, #12
  3f3138: e58dc01c     	str	r12, [sp, #0x1c]
  3f313c: e59f9354     	ldr	r9, [pc, #0x354]        @ 0x3f3498 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x370>
  3f3140: e1a0c003     	mov	r12, r3
  3f3144: e59d301c     	ldr	r3, [sp, #0x1c]
  3f3148: e08f9009     	add	r9, pc, r9
  3f314c: e1a04000     	mov	r4, r0
  3f3150: e799e003     	ldr	lr, [r9, r3]
  3f3154: e1a03002     	mov	r3, r2
  3f3158: e1a08001     	mov	r8, r1
  3f315c: e59e2000     	ldr	r2, [lr]
  3f3160: e5ddb458     	ldrb	r11, [sp, #0x458]
  3f3164: e58d3018     	str	r3, [sp, #0x18]
  3f3168: e58dc014     	str	r12, [sp, #0x14]
  3f316c: e58d2424     	str	r2, [sp, #0x424]
  3f3170: e5dda45c     	ldrb	r10, [sp, #0x45c]
  3f3174: ebfd13b8     	bl	0x33805c <EventManager::EventManager()> @ imm = #-0xbb120
  3f3178: e59f231c     	ldr	r2, [pc, #0x31c]        @ 0x3f349c <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x374>
  3f317c: e3a06000     	mov	r6, #0
  3f3180: e3e05000     	mvn	r5, #0
  3f3184: e7992002     	ldr	r2, [r9, r2]
  3f3188: e2847044     	add	r7, r4, #68
  3f318c: e1a01006     	mov	r1, r6
  3f3190: e2822008     	add	r2, r2, #8
  3f3194: e5842000     	str	r2, [r4]
  3f3198: e5846030     	str	r6, [r4, #0x30]
  3f319c: e5846038     	str	r6, [r4, #0x38]
  3f31a0: e584503c     	str	r5, [r4, #0x3c]
  3f31a4: e5845040     	str	r5, [r4, #0x40]
  3f31a8: e1a00007     	mov	r0, r7
  3f31ac: ebfe24f4     	bl	0x37c584 <LuaScript::LuaScript(bool)> @ imm = #-0x76c30
  3f31b0: e59d2450     	ldr	r2, [sp, #0x450]
  3f31b4: e1a01008     	mov	r1, r8
  3f31b8: e28400f8     	add	r0, r4, #248
  3f31bc: e58420dc     	str	r2, [r4, #0xdc]
  3f31c0: e59d2454     	ldr	r2, [sp, #0x454]
  3f31c4: e5c4b0f1     	strb	r11, [r4, #0xf1]
  3f31c8: e5c4a0f2     	strb	r10, [r4, #0xf2]
  3f31cc: e58420e0     	str	r2, [r4, #0xe0]
  3f31d0: e3a02001     	mov	r2, #1
  3f31d4: e58420e4     	str	r2, [r4, #0xe4]
  3f31d8: e28d2028     	add	r2, sp, #40
  3f31dc: e2422008     	sub	r2, r2, #8
  3f31e0: e58460ec     	str	r6, [r4, #0xec]
  3f31e4: e5c460f0     	strb	r6, [r4, #0xf0]
  3f31e8: e5c460f3     	strb	r6, [r4, #0xf3]
  3f31ec: e5c460f4     	strb	r6, [r4, #0xf4]
  3f31f0: e5c460f5     	strb	r6, [r4, #0xf5]
  3f31f4: ebfc83bc     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xdf110
  3f31f8: e59d3018     	ldr	r3, [sp, #0x18]
  3f31fc: e59f229c     	ldr	r2, [pc, #0x29c]        @ 0x3f34a0 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x378>
  3f3200: e3a01000     	mov	r1, #0
  3f3204: e5843110     	str	r3, [r4, #0x110]
  3f3208: e59dc014     	ldr	r12, [sp, #0x14]
  3f320c: e08f2002     	add	r2, pc, r2
  3f3210: e28400ac     	add	r0, r4, #172
  3f3214: e584c114     	str	r12, [r4, #0x114]
  3f3218: e59d3464     	ldr	r3, [sp, #0x464]
  3f321c: e5846130     	str	r6, [r4, #0x130]
  3f3220: e58411a4     	str	r1, [r4, #0x1a4]
  3f3224: e5846134     	str	r6, [r4, #0x134]
  3f3228: e5841160     	str	r1, [r4, #0x160]
  3f322c: e5846138     	str	r6, [r4, #0x138]
  3f3230: e5841164     	str	r1, [r4, #0x164]
  3f3234: e5841168     	str	r1, [r4, #0x168]
  3f3238: e584119c     	str	r1, [r4, #0x19c]
  3f323c: e58411a0     	str	r1, [r4, #0x1a0]
  3f3240: e5843118     	str	r3, [r4, #0x118]
  3f3244: e584511c     	str	r5, [r4, #0x11c]
  3f3248: e5845120     	str	r5, [r4, #0x120]
  3f324c: e5845124     	str	r5, [r4, #0x124]
  3f3250: e5846128     	str	r6, [r4, #0x128]
  3f3254: e584612c     	str	r6, [r4, #0x12c]
  3f3258: e584613c     	str	r6, [r4, #0x13c]
  3f325c: e5846140     	str	r6, [r4, #0x140]
  3f3260: e5c46144     	strb	r6, [r4, #0x144]
  3f3264: e5c46145     	strb	r6, [r4, #0x145]
  3f3268: e5845148     	str	r5, [r4, #0x148]
  3f326c: e584614c     	str	r6, [r4, #0x14c]
  3f3270: e5846150     	str	r6, [r4, #0x150]
  3f3274: e5846154     	str	r6, [r4, #0x154]
  3f3278: e5846158     	str	r6, [r4, #0x158]
  3f327c: e584615c     	str	r6, [r4, #0x15c]
  3f3280: e5846194     	str	r6, [r4, #0x194]
  3f3284: e5c46198     	strb	r6, [r4, #0x198]
  3f3288: e5c461a8     	strb	r6, [r4, #0x1a8]
  3f328c: e5923000     	ldr	r3, [r2]
  3f3290: e59f120c     	ldr	r1, [pc, #0x20c]        @ 0x3f34a4 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x37c>
  3f3294: e59fb20c     	ldr	r11, [pc, #0x20c]       @ 0x3f34a8 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x380>
  3f3298: e2833001     	add	r3, r3, #1
  3f329c: e08f1001     	add	r1, pc, r1
  3f32a0: e5823000     	str	r3, [r2]
  3f32a4: e281200d     	add	r2, r1, #13
  3f32a8: ebfc75cc     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0xe28d0
  3f32ac: e59f11f8     	ldr	r1, [pc, #0x1f8]        @ 0x3f34ac <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x384>
  3f32b0: e1a00007     	mov	r0, r7
  3f32b4: e08f1001     	add	r1, pc, r1
  3f32b8: ebfe20ad     	bl	0x37b574 <LuaScript::Load(char const*)> @ imm = #-0x77d4c
  3f32bc: e59f11ec     	ldr	r1, [pc, #0x1ec]        @ 0x3f34b0 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x388>
  3f32c0: e1a00007     	mov	r0, r7
  3f32c4: e08f1001     	add	r1, pc, r1
  3f32c8: ebfe20a9     	bl	0x37b574 <LuaScript::Load(char const*)> @ imm = #-0x77d5c
  3f32cc: e59f21e0     	ldr	r2, [pc, #0x1e0]        @ 0x3f34b4 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x38c>
  3f32d0: e799300b     	ldr	r3, [r9, r11]
  3f32d4: e584518c     	str	r5, [r4, #0x18c]
  3f32d8: e7992002     	ldr	r2, [r9, r2]
  3f32dc: e5c4616c     	strb	r6, [r4, #0x16c]
  3f32e0: e5826000     	str	r6, [r2]
  3f32e4: e5c460e8     	strb	r6, [r4, #0xe8]
  3f32e8: e5933000     	ldr	r3, [r3]
  3f32ec: e1530006     	cmp	r3, r6
  3f32f0: 0a00001e     	beq	0x3f3370 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x248> @ imm = #0x78
  3f32f4: e59f31bc     	ldr	r3, [pc, #0x1bc]        @ 0x3f34b8 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x390>
  3f32f8: e28d5028     	add	r5, sp, #40
  3f32fc: e2455004     	sub	r5, r5, #4
  3f3300: e799a003     	ldr	r10, [r9, r3]
  3f3304: e1a07006     	mov	r7, r6
  3f3308: ea000005     	b	0x3f3324 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x1fc> @ imm = #0x14
  3f330c: e799300b     	ldr	r3, [r9, r11]
  3f3310: e2877001     	add	r7, r7, #1
  3f3314: e2866048     	add	r6, r6, #72
  3f3318: e5933000     	ldr	r3, [r3]
  3f331c: e1530007     	cmp	r3, r7
  3f3320: 9a000012     	bls	0x3f3370 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x248> @ imm = #0x48
  3f3324: e59a8000     	ldr	r8, [r10]
  3f3328: e1a00005     	mov	r0, r5
  3f332c: e0888006     	add	r8, r8, r6
  3f3330: e5981020     	ldr	r1, [r8, #0x20]
  3f3334: ebfc6c79     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0xe4e1c
  3f3338: e1a00005     	mov	r0, r5
  3f333c: e3a01000     	mov	r1, #0
  3f3340: e3e02000     	mvn	r2, #0
  3f3344: ebfd6c32     	bl	0x34e414 <ToLowerCase(char*, int, int)> @ imm = #-0xa4f38
  3f3348: e594010c     	ldr	r0, [r4, #0x10c]
  3f334c: e1a01005     	mov	r1, r5
  3f3350: ebfc6e1f     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0xe4784
  3f3354: e3500000     	cmp	r0, #0
  3f3358: 0affffeb     	beq	0x3f330c <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x1e4> @ imm = #-0x54
  3f335c: e5d83014     	ldrb	r3, [r8, #0x14]
  3f3360: e584703c     	str	r7, [r4, #0x3c]
  3f3364: e5c430e8     	strb	r3, [r4, #0xe8]
  3f3368: e5983010     	ldr	r3, [r8, #0x10]
  3f336c: e5843040     	str	r3, [r4, #0x40]
  3f3370: eb102907     	bl	0x7fd794 <GetOnline()>  @ imm = #0x40a41c
  3f3374: e5d03005     	ldrb	r3, [r0, #0x5]
  3f3378: e3530000     	cmp	r3, #0
  3f337c: 1a00002a     	bne	0x3f342c <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x304> @ imm = #0xa8
  3f3380: e594303c     	ldr	r3, [r4, #0x3c]
  3f3384: e3730001     	cmn	r3, #1
  3f3388: 0a00001d     	beq	0x3f3404 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x2dc> @ imm = #0x74
  3f338c: e59dc460     	ldr	r12, [sp, #0x460]
  3f3390: e37c0001     	cmn	r12, #1
  3f3394: 0a00000b     	beq	0x3f33c8 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x2a0> @ imm = #0x2c
  3f3398: e5943040     	ldr	r3, [r4, #0x40]
  3f339c: e3a02001     	mov	r2, #1
  3f33a0: e5c420f5     	strb	r2, [r4, #0xf5]
  3f33a4: e19cc003     	orrs	r12, r12, r3
  3f33a8: 0a000004     	beq	0x3f33c0 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x298> @ imm = #0x10
  3f33ac: e59d2460     	ldr	r2, [sp, #0x460]
  3f33b0: e1520003     	cmp	r2, r3
  3f33b4: 0a000003     	beq	0x3f33c8 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x2a0> @ imm = #0xc
  3f33b8: e3530000     	cmp	r3, #0
  3f33bc: 0a000001     	beq	0x3f33c8 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x2a0> @ imm = #0x4
  3f33c0: e3a03001     	mov	r3, #1
  3f33c4: e5c430f3     	strb	r3, [r4, #0xf3]
  3f33c8: e3a01000     	mov	r1, #0
  3f33cc: e3a0003c     	mov	r0, #60
  3f33d0: ebfc7466     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xe2e68
  3f33d4: e594c118     	ldr	r12, [r4, #0x118]
  3f33d8: e594e03c     	ldr	lr, [r4, #0x3c]
  3f33dc: e5942114     	ldr	r2, [r4, #0x114]
  3f33e0: e5943040     	ldr	r3, [r4, #0x40]
  3f33e4: e1a05000     	mov	r5, r0
  3f33e8: e58dc004     	str	r12, [sp, #0x4]
  3f33ec: e1a01004     	mov	r1, r4
  3f33f0: e3a0c000     	mov	r12, #0
  3f33f4: e58de000     	str	lr, [sp]
  3f33f8: e58dc008     	str	r12, [sp, #0x8]
  3f33fc: eb01bd4c     	bl	0x462934 <LevelSavegame::LevelSavegame(Level*, unsigned int, int, int, int, bool)> @ imm = #0x6f530
  3f3400: e58450ec     	str	r5, [r4, #0xec]
  3f3404: e59dc01c     	ldr	r12, [sp, #0x1c]
  3f3408: e59d2424     	ldr	r2, [sp, #0x424]
  3f340c: e1a00004     	mov	r0, r4
  3f3410: e799300c     	ldr	r3, [r9, r12]
  3f3414: e5933000     	ldr	r3, [r3]
  3f3418: e1520003     	cmp	r2, r3
  3f341c: 1a00001b     	bne	0x3f3490 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x368> @ imm = #0x6c
  3f3420: e28dd02c     	add	sp, sp, #44
  3f3424: e28ddb01     	add	sp, sp, #1024
  3f3428: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3f342c: e59f5088     	ldr	r5, [pc, #0x88]         @ 0x3f34bc <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x394>
  3f3430: e7993005     	ldr	r3, [r9, r5]
  3f3434: e5930040     	ldr	r0, [r3, #0x40]
  3f3438: ebfdef0d     	bl	0x36f074 <PlayerManager::IsLocalPlayerHosting()> @ imm = #-0x843cc
  3f343c: e3500000     	cmp	r0, #0
  3f3440: 0a000005     	beq	0x3f345c <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x334> @ imm = #0x14
  3f3444: e7993005     	ldr	r3, [r9, r5]
  3f3448: e5933040     	ldr	r3, [r3, #0x40]
  3f344c: e5d33719     	ldrb	r3, [r3, #0x719]
  3f3450: e3530000     	cmp	r3, #0
  3f3454: 0affffc9     	beq	0x3f3380 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x258> @ imm = #-0xdc
  3f3458: ea000004     	b	0x3f3470 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x348> @ imm = #0x10
  3f345c: ebfcb68d     	bl	0x320e98 <OnlineSingleton<OnlineGameState>::GetInstance()> @ imm = #-0xd25cc
  3f3460: e5903034     	ldr	r3, [r0, #0x34]
  3f3464: e2433003     	sub	r3, r3, #3
  3f3468: e3530001     	cmp	r3, #1
  3f346c: 9a000002     	bls	0x3f347c <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x354> @ imm = #0x8
  3f3470: e3a03000     	mov	r3, #0
  3f3474: e5c430f1     	strb	r3, [r4, #0xf1]
  3f3478: eaffffc0     	b	0x3f3380 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x258> @ imm = #-0x100
  3f347c: eb1036c2     	bl	0x800f8c <CMatching::Get()> @ imm = #0x40db08
  3f3480: eb10b027     	bl	0x81f524 <CMatchingGLLive::IsHost()> @ imm = #0x42c09c
  3f3484: e3500000     	cmp	r0, #0
  3f3488: 0afffff8     	beq	0x3f3470 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x348> @ imm = #-0x20
  3f348c: eaffffec     	b	0x3f3444 <Level::Level(char const*, int, unsigned int, unsigned int, unsigned int, bool, bool, int, int)+0x31c> @ imm = #-0x50
  3f3490: ebfc6b9e     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xe5188
  3f3494: ac 40 00 00  	.word	0x000040ac
  3f3498: 48 19 5a 00  	.word	0x005a1948
  3f349c: 74 07 00 00  	.word	0x00000774
  3f34a0: d4 fe 5a 00  	.word	0x005afed4
  3f34a4: 4c 34 4d 00  	.word	0x004d344c
  3f34a8: c0 18 00 00  	.word	0x000018c0
  3f34ac: 44 34 4d 00  	.word	0x004d3444
  3f34b0: 4c 34 4d 00  	.word	0x004d344c
  3f34b4: 2c 28 00 00  	.word	0x0000282c
  3f34b8: 74 08 00 00  	.word	0x00000874
  3f34bc: f4 37 00 00  	.word	0x000037f4
