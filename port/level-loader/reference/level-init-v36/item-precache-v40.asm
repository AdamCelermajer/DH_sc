
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003eb6e4 <ItemManager::PreCache()>:
  3eb6e4: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3eb6e8: e59f13ec     	ldr	r1, [pc, #0x3ec]        @ 0x3ebadc <ItemManager::PreCache()+0x3f8>
  3eb6ec: e59f23ec     	ldr	r2, [pc, #0x3ec]        @ 0x3ebae0 <ItemManager::PreCache()+0x3fc>
  3eb6f0: e24dd09c     	sub	sp, sp, #156
  3eb6f4: e08f1001     	add	r1, pc, r1
  3eb6f8: e7913002     	ldr	r3, [r1, r2]
  3eb6fc: e58d204c     	str	r2, [sp, #0x4c]
  3eb700: e59f23dc     	ldr	r2, [pc, #0x3dc]        @ 0x3ebae4 <ItemManager::PreCache()+0x400>
  3eb704: e5933000     	ldr	r3, [r3]
  3eb708: e1a06000     	mov	r6, r0
  3eb70c: e7912002     	ldr	r2, [r1, r2]
  3eb710: e28d9070     	add	r9, sp, #112
  3eb714: e286c004     	add	r12, r6, #4
  3eb718: e5922000     	ldr	r2, [r2]
  3eb71c: e1a00009     	mov	r0, r9
  3eb720: e58d101c     	str	r1, [sp, #0x1c]
  3eb724: e58d2040     	str	r2, [sp, #0x40]
  3eb728: e58dc044     	str	r12, [sp, #0x44]
  3eb72c: e58d3094     	str	r3, [sp, #0x94]
  3eb730: ebfd4f75     	bl	0x33f50c <ObjectHandle::ObjectHandle()> @ imm = #-0xac22c
  3eb734: e59d0044     	ldr	r0, [sp, #0x44]
  3eb738: e59d1040     	ldr	r1, [sp, #0x40]
  3eb73c: ebffff37     	bl	0x3eb420 <std::vector<ItemManager::CategoryInfo, std::allocator<ItemManager::CategoryInfo>>::reserve(unsigned int)> @ imm = #-0x324
  3eb740: e59d0040     	ldr	r0, [sp, #0x40]
  3eb744: e3500000     	cmp	r0, #0
  3eb748: 0a0000ce     	beq	0x3eba88 <ItemManager::PreCache()+0x3a4> @ imm = #0x338
  3eb74c: e59f3394     	ldr	r3, [pc, #0x394]        @ 0x3ebae8 <ItemManager::PreCache()+0x404>
  3eb750: e59f1394     	ldr	r1, [pc, #0x394]        @ 0x3ebaec <ItemManager::PreCache()+0x408>
  3eb754: e28db050     	add	r11, sp, #80
  3eb758: e08f3003     	add	r3, pc, r3
  3eb75c: e58d302c     	str	r3, [sp, #0x2c]
  3eb760: e59f3388     	ldr	r3, [pc, #0x388]        @ 0x3ebaf0 <ItemManager::PreCache()+0x40c>
  3eb764: e28b2004     	add	r2, r11, #4
  3eb768: e58d1024     	str	r1, [sp, #0x24]
  3eb76c: e08f3003     	add	r3, pc, r3
  3eb770: e58d3030     	str	r3, [sp, #0x30]
  3eb774: e28dc060     	add	r12, sp, #96
  3eb778: e3a03000     	mov	r3, #0
  3eb77c: e28d0080     	add	r0, sp, #128
  3eb780: e2821004     	add	r1, r2, #4
  3eb784: e58d2020     	str	r2, [sp, #0x20]
  3eb788: e58d3014     	str	r3, [sp, #0x14]
  3eb78c: e58dc034     	str	r12, [sp, #0x34]
  3eb790: e58d0018     	str	r0, [sp, #0x18]
  3eb794: e58d1028     	str	r1, [sp, #0x28]
  3eb798: e58d9010     	str	r9, [sp, #0x10]
  3eb79c: e3a03000     	mov	r3, #0
  3eb7a0: e59d1034     	ldr	r1, [sp, #0x34]
  3eb7a4: e59d0044     	ldr	r0, [sp, #0x44]
  3eb7a8: e58d306c     	str	r3, [sp, #0x6c]
  3eb7ac: e58d3060     	str	r3, [sp, #0x60]
  3eb7b0: e58d3064     	str	r3, [sp, #0x64]
  3eb7b4: e58d3068     	str	r3, [sp, #0x68]
  3eb7b8: ebffff5c     	bl	0x3eb530 <std::vector<ItemManager::CategoryInfo, std::allocator<ItemManager::CategoryInfo>>::push_back(ItemManager::CategoryInfo const&)> @ imm = #-0x290
  3eb7bc: e59d0034     	ldr	r0, [sp, #0x34]
  3eb7c0: ebfffeec     	bl	0x3eb378 <std::vector<ItemManager::ObjectInfo, std::allocator<ItemManager::ObjectInfo>>::~vector()> @ imm = #-0x450
  3eb7c4: e59d2014     	ldr	r2, [sp, #0x14]
  3eb7c8: e5963004     	ldr	r3, [r6, #0x4]
  3eb7cc: e3a01005     	mov	r1, #5
  3eb7d0: e1a07202     	lsl	r7, r2, #4
  3eb7d4: e58d107c     	str	r1, [sp, #0x7c]
  3eb7d8: e0834007     	add	r4, r3, r7
  3eb7dc: e7932202     	ldr	r2, [r3, r2, lsl #4]
  3eb7e0: e5940008     	ldr	r0, [r4, #0x8]
  3eb7e4: e0620000     	rsb	r0, r2, r0
  3eb7e8: e1a001c0     	asr	r0, r0, #3
  3eb7ec: e3500004     	cmp	r0, #4
  3eb7f0: 8a000011     	bhi	0x3eb83c <ItemManager::PreCache()+0x158> @ imm = #0x44
  3eb7f4: e5943004     	ldr	r3, [r4, #0x4]
  3eb7f8: e3520000     	cmp	r2, #0
  3eb7fc: e0620003     	rsb	r0, r2, r3
  3eb800: e1a081c0     	asr	r8, r0, #3
  3eb804: 0a0000ae     	beq	0x3ebac4 <ItemManager::PreCache()+0x3e0> @ imm = #0x2b8
  3eb808: e1a00004     	mov	r0, r4
  3eb80c: e28d107c     	add	r1, sp, #124
  3eb810: ebfffe6a     	bl	0x3eb1c0 <ItemManager::ObjectInfo* std::vector<ItemManager::ObjectInfo, std::allocator<ItemManager::ObjectInfo>>::_M_allocate_and_copy<ItemManager::ObjectInfo*>(unsigned int&, ItemManager::ObjectInfo*, ItemManager::ObjectInfo*)> @ imm = #-0x658
  3eb814: e1a05000     	mov	r5, r0
  3eb818: e1a00004     	mov	r0, r4
  3eb81c: ebfffe7d     	bl	0x3eb218 <std::vector<ItemManager::ObjectInfo, std::allocator<ItemManager::ObjectInfo>>::_M_clear()> @ imm = #-0x60c
  3eb820: e59d307c     	ldr	r3, [sp, #0x7c]
  3eb824: e0850188     	add	r0, r5, r8, lsl #3
  3eb828: e5840004     	str	r0, [r4, #0x4]
  3eb82c: e0853183     	add	r3, r5, r3, lsl #3
  3eb830: e5843008     	str	r3, [r4, #0x8]
  3eb834: e5845000     	str	r5, [r4]
  3eb838: e5963004     	ldr	r3, [r6, #0x4]
  3eb83c: e28d007c     	add	r0, sp, #124
  3eb840: e1a0a007     	mov	r10, r7
  3eb844: e58d003c     	str	r0, [sp, #0x3c]
  3eb848: e083700a     	add	r7, r3, r10
  3eb84c: e5978004     	ldr	r8, [r7, #0x4]
  3eb850: e5972008     	ldr	r2, [r7, #0x8]
  3eb854: e3a04000     	mov	r4, #0
  3eb858: e1a05004     	mov	r5, r4
  3eb85c: e1580002     	cmp	r8, r2
  3eb860: 0a00003d     	beq	0x3eb95c <ItemManager::PreCache()+0x278> @ imm = #0xf4
  3eb864: e5885000     	str	r5, [r8]
  3eb868: e5c85004     	strb	r5, [r8, #0x4]
  3eb86c: e5973004     	ldr	r3, [r7, #0x4]
  3eb870: e2833008     	add	r3, r3, #8
  3eb874: e5873004     	str	r3, [r7, #0x4]
  3eb878: e59d102c     	ldr	r1, [sp, #0x2c]
  3eb87c: e59d2014     	ldr	r2, [sp, #0x14]
  3eb880: e1a03004     	mov	r3, r4
  3eb884: e59d0018     	ldr	r0, [sp, #0x18]
  3eb888: ebfc8c95     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0xdcdac
  3eb88c: e59d101c     	ldr	r1, [sp, #0x1c]
  3eb890: e59d0024     	ldr	r0, [sp, #0x24]
  3eb894: e3a0c001     	mov	r12, #1
  3eb898: e59d2030     	ldr	r2, [sp, #0x30]
  3eb89c: e7913000     	ldr	r3, [r1, r0]
  3eb8a0: e1a0000b     	mov	r0, r11
  3eb8a4: e5931038     	ldr	r1, [r3, #0x38]
  3eb8a8: e59d3018     	ldr	r3, [sp, #0x18]
  3eb8ac: e88d1020     	stm	sp, {r5, r12}
  3eb8b0: ebfd7f9b     	bl	0x34b724 <ObjectManager::Spawn(char const*, char const*, bool, bool)> @ imm = #-0xa0194
  3eb8b4: e59b2000     	ldr	r2, [r11]
  3eb8b8: e59d1020     	ldr	r1, [sp, #0x20]
  3eb8bc: e1a03009     	mov	r3, r9
  3eb8c0: e4832004     	str	r2, [r3], #4
  3eb8c4: e5912000     	ldr	r2, [r1]
  3eb8c8: e59dc028     	ldr	r12, [sp, #0x28]
  3eb8cc: e59d0010     	ldr	r0, [sp, #0x10]
  3eb8d0: e5892004     	str	r2, [r9, #0x4]
  3eb8d4: e59c2000     	ldr	r2, [r12]
  3eb8d8: e1a01005     	mov	r1, r5
  3eb8dc: e1a09000     	mov	r9, r0
  3eb8e0: e5832004     	str	r2, [r3, #0x4]
  3eb8e4: ebfd5135     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xabb2c
  3eb8e8: e3500000     	cmp	r0, #0
  3eb8ec: 0a000011     	beq	0x3eb938 <ItemManager::PreCache()+0x254> @ imm = #0x44
  3eb8f0: e5963004     	ldr	r3, [r6, #0x4]
  3eb8f4: e59d0010     	ldr	r0, [sp, #0x10]
  3eb8f8: e1a01005     	mov	r1, r5
  3eb8fc: e793700a     	ldr	r7, [r3, r10]
  3eb900: ebfd512e     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xabb48
  3eb904: e3500000     	cmp	r0, #0
  3eb908: e1a03184     	lsl	r3, r4, #3
  3eb90c: 0a000002     	beq	0x3eb91c <ItemManager::PreCache()+0x238> @ imm = #0x8
  3eb910: e59020f4     	ldr	r2, [r0, #0xf4]
  3eb914: e3520003     	cmp	r2, #3
  3eb918: 0a000000     	beq	0x3eb920 <ItemManager::PreCache()+0x23c> @ imm = #0x0
  3eb91c: e3a00000     	mov	r0, #0
  3eb920: e7870003     	str	r0, [r7, r3]
  3eb924: e5962004     	ldr	r2, [r6, #0x4]
  3eb928: e59d1014     	ldr	r1, [sp, #0x14]
  3eb92c: e792200a     	ldr	r2, [r2, r10]
  3eb930: e7920003     	ldr	r0, [r2, r3]
  3eb934: eb000551     	bl	0x3ece80 <ItemObject::InitOnce(int)> @ imm = #0x1544
  3eb938: e2844001     	add	r4, r4, #1
  3eb93c: e3540005     	cmp	r4, #5
  3eb940: 0a000040     	beq	0x3eba48 <ItemManager::PreCache()+0x364> @ imm = #0x100
  3eb944: e5963004     	ldr	r3, [r6, #0x4]
  3eb948: e083700a     	add	r7, r3, r10
  3eb94c: e5978004     	ldr	r8, [r7, #0x4]
  3eb950: e5972008     	ldr	r2, [r7, #0x8]
  3eb954: e1580002     	cmp	r8, r2
  3eb958: 1affffc1     	bne	0x3eb864 <ItemManager::PreCache()+0x180> @ imm = #-0xfc
  3eb95c: e793200a     	ldr	r2, [r3, r10]
  3eb960: e0622008     	rsb	r2, r2, r8
  3eb964: e1a021c2     	asr	r2, r2, #3
  3eb968: e3520001     	cmp	r2, #1
  3eb96c: 20823002     	addhs	r3, r2, r2
  3eb970: 32823001     	addlo	r3, r2, #1
  3eb974: e373021e     	cmn	r3, #-536870911
  3eb978: 8a00004b     	bhi	0x3ebaac <ItemManager::PreCache()+0x3c8> @ imm = #0x12c
  3eb97c: e1520003     	cmp	r2, r3
  3eb980: 8a000049     	bhi	0x3ebaac <ItemManager::PreCache()+0x3c8> @ imm = #0x124
  3eb984: e1a01003     	mov	r1, r3
  3eb988: e2870008     	add	r0, r7, #8
  3eb98c: e59d203c     	ldr	r2, [sp, #0x3c]
  3eb990: e58d307c     	str	r3, [sp, #0x7c]
  3eb994: ebfffd97     	bl	0x3eaff8 <std::allocator<ItemManager::ObjectInfo>::_M_allocate(unsigned int, unsigned int&)> @ imm = #-0x9a4
  3eb998: e58d0038     	str	r0, [sp, #0x38]
  3eb99c: e597e000     	ldr	lr, [r7]
  3eb9a0: e06e8008     	rsb	r8, lr, r8
  3eb9a4: e1a081c8     	asr	r8, r8, #3
  3eb9a8: e3580000     	cmp	r8, #0
  3eb9ac: d1a08000     	movle	r8, r0
  3eb9b0: da00000f     	ble	0x3eb9f4 <ItemManager::PreCache()+0x310> @ imm = #0x3c
  3eb9b4: e1a01008     	mov	r1, r8
  3eb9b8: e58d8048     	str	r8, [sp, #0x48]
  3eb9bc: e59d8038     	ldr	r8, [sp, #0x38]
  3eb9c0: e3a00000     	mov	r0, #0
  3eb9c4: e1a0200e     	mov	r2, lr
  3eb9c8: e7b2c000     	ldr	r12, [r2, r0]!
  3eb9cc: e1a03008     	mov	r3, r8
  3eb9d0: e2511001     	subs	r1, r1, #1
  3eb9d4: e7a3c000     	str	r12, [r3, r0]!
  3eb9d8: e5d22004     	ldrb	r2, [r2, #0x4]
  3eb9dc: e2800008     	add	r0, r0, #8
  3eb9e0: e5c32004     	strb	r2, [r3, #0x4]
  3eb9e4: 1afffff6     	bne	0x3eb9c4 <ItemManager::PreCache()+0x2e0> @ imm = #-0x28
  3eb9e8: e59d8048     	ldr	r8, [sp, #0x48]
  3eb9ec: e59d1038     	ldr	r1, [sp, #0x38]
  3eb9f0: e0818188     	add	r8, r1, r8, lsl #3
  3eb9f4: e1a02008     	mov	r2, r8
  3eb9f8: e5c85004     	strb	r5, [r8, #0x4]
  3eb9fc: e4825008     	str	r5, [r2], #8
  3eba00: e5970000     	ldr	r0, [r7]
  3eba04: e5971008     	ldr	r1, [r7, #0x8]
  3eba08: e3500000     	cmp	r0, #0
  3eba0c: 0a000006     	beq	0x3eba2c <ItemManager::PreCache()+0x348> @ imm = #0x18
  3eba10: e0601001     	rsb	r1, r0, r1
  3eba14: e3c11007     	bic	r1, r1, #7
  3eba18: e3510080     	cmp	r1, #128
  3eba1c: 8a000024     	bhi	0x3ebab4 <ItemManager::PreCache()+0x3d0> @ imm = #0x90
  3eba20: e58d200c     	str	r2, [sp, #0xc]
  3eba24: eb0c7535     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x31d4d4
  3eba28: e59d200c     	ldr	r2, [sp, #0xc]
  3eba2c: e59d307c     	ldr	r3, [sp, #0x7c]
  3eba30: e59dc038     	ldr	r12, [sp, #0x38]
  3eba34: e5872004     	str	r2, [r7, #0x4]
  3eba38: e08c3183     	add	r3, r12, r3, lsl #3
  3eba3c: e587c000     	str	r12, [r7]
  3eba40: e5873008     	str	r3, [r7, #0x8]
  3eba44: eaffff8b     	b	0x3eb878 <ItemManager::PreCache()+0x194> @ imm = #-0x1d4
  3eba48: e1a0700a     	mov	r7, r10
  3eba4c: e3a04000     	mov	r4, #0
  3eba50: e5963004     	ldr	r3, [r6, #0x4]
  3eba54: e1a00006     	mov	r0, r6
  3eba58: e7933007     	ldr	r3, [r3, r7]
  3eba5c: e7931004     	ldr	r1, [r3, r4]
  3eba60: e2844008     	add	r4, r4, #8
  3eba64: ebfffc72     	bl	0x3eac34 <ItemManager::DeSpawn(ItemObject*)> @ imm = #-0xe38
  3eba68: e3540028     	cmp	r4, #40
  3eba6c: 1afffff7     	bne	0x3eba50 <ItemManager::PreCache()+0x36c> @ imm = #-0x24
  3eba70: e59d0014     	ldr	r0, [sp, #0x14]
  3eba74: e59d1040     	ldr	r1, [sp, #0x40]
  3eba78: e2800001     	add	r0, r0, #1
  3eba7c: e1500001     	cmp	r0, r1
  3eba80: e58d0014     	str	r0, [sp, #0x14]
  3eba84: 1affff44     	bne	0x3eb79c <ItemManager::PreCache()+0xb8> @ imm = #-0x2f0
  3eba88: e59d204c     	ldr	r2, [sp, #0x4c]
  3eba8c: e59dc01c     	ldr	r12, [sp, #0x1c]
  3eba90: e79c3002     	ldr	r3, [r12, r2]
  3eba94: e59d2094     	ldr	r2, [sp, #0x94]
  3eba98: e5933000     	ldr	r3, [r3]
  3eba9c: e1520003     	cmp	r2, r3
  3ebaa0: 1a00000c     	bne	0x3ebad8 <ItemManager::PreCache()+0x3f4> @ imm = #0x30
  3ebaa4: e28dd09c     	add	sp, sp, #156
  3ebaa8: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3ebaac: e3e0320e     	mvn	r3, #-536870912
  3ebab0: eaffffb3     	b	0x3eb984 <ItemManager::PreCache()+0x2a0> @ imm = #-0x134
  3ebab4: e58d200c     	str	r2, [sp, #0xc]
  3ebab8: ebfc9260     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xdb680
  3ebabc: e59d200c     	ldr	r2, [sp, #0xc]
  3ebac0: eaffffd9     	b	0x3eba2c <ItemManager::PreCache()+0x348> @ imm = #-0x9c
  3ebac4: e2840008     	add	r0, r4, #8
  3ebac8: e28d207c     	add	r2, sp, #124
  3ebacc: ebfffd49     	bl	0x3eaff8 <std::allocator<ItemManager::ObjectInfo>::_M_allocate(unsigned int, unsigned int&)> @ imm = #-0xadc
  3ebad0: e1a05000     	mov	r5, r0
  3ebad4: eaffff51     	b	0x3eb820 <ItemManager::PreCache()+0x13c> @ imm = #-0x2bc
  3ebad8: ebfc8a0c     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xdd7d0
  3ebadc: 9c 93 5a 00  	.word	0x005a939c
  3ebae0: ac 40 00 00  	.word	0x000040ac
  3ebae4: 10 45 00 00  	.word	0x00004510
  3ebae8: 90 aa 4d 00  	.word	0x004daa90
  3ebaec: f4 37 00 00  	.word	0x000037f4
  3ebaf0: 14 ee 4d 00  	.word	0x004dee14
