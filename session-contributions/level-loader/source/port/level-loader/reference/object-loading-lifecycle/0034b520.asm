
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034b520 <ObjectManager::GetNewObject(char const*, char const*, int, bool)>:
  34b520: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  34b524: e24dd064     	sub	sp, sp, #100
  34b528: e58d3018     	str	r3, [sp, #0x18]
  34b52c: e59f31c8     	ldr	r3, [pc, #0x1c8]        @ 0x34b6fc <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x1dc>
  34b530: e59f71c8     	ldr	r7, [pc, #0x1c8]        @ 0x34b700 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x1e0>
  34b534: e59fa1c8     	ldr	r10, [pc, #0x1c8]       @ 0x34b704 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x1e4>
  34b538: e08f3003     	add	r3, pc, r3
  34b53c: e58d3024     	str	r3, [sp, #0x24]
  34b540: e59f31c0     	ldr	r3, [pc, #0x1c0]        @ 0x34b708 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x1e8>
  34b544: e08f7007     	add	r7, pc, r7
  34b548: e5dde08c     	ldrb	lr, [sp, #0x8c]
  34b54c: e797c00a     	ldr	r12, [r7, r10]
  34b550: e08f3003     	add	r3, pc, r3
  34b554: e58d3028     	str	r3, [sp, #0x28]
  34b558: e59f31ac     	ldr	r3, [pc, #0x1ac]        @ 0x34b70c <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x1ec>
  34b55c: e59cc000     	ldr	r12, [r12]
  34b560: e58de01c     	str	lr, [sp, #0x1c]
  34b564: e59f51a4     	ldr	r5, [pc, #0x1a4]        @ 0x34b710 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x1f0>
  34b568: e59fe1a4     	ldr	lr, [pc, #0x1a4]        @ 0x34b714 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x1f4>
  34b56c: e08f3003     	add	r3, pc, r3
  34b570: e59f91a0     	ldr	r9, [pc, #0x1a0]        @ 0x34b718 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x1f8>
  34b574: e58de020     	str	lr, [sp, #0x20]
  34b578: e58dc05c     	str	r12, [sp, #0x5c]
  34b57c: e1a08000     	mov	r8, r0
  34b580: e58d1014     	str	r1, [sp, #0x14]
  34b584: e1a06002     	mov	r6, r2
  34b588: e08f5005     	add	r5, pc, r5
  34b58c: e58d302c     	str	r3, [sp, #0x2c]
  34b590: e3a04000     	mov	r4, #0
  34b594: ea000002     	b	0x34b5a4 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x84> @ imm = #0x8
  34b598: e2844008     	add	r4, r4, #8
  34b59c: e3540f42     	cmp	r4, #264
  34b5a0: 0a000030     	beq	0x34b668 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x148> @ imm = #0xc0
  34b5a4: e795b004     	ldr	r11, [r5, r4]
  34b5a8: e1a00006     	mov	r0, r6
  34b5ac: e1a0100b     	mov	r1, r11
  34b5b0: ebff0b59     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x3d29c
  34b5b4: e3500000     	cmp	r0, #0
  34b5b8: 1afffff6     	bne	0x34b598 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x78> @ imm = #-0x28
  34b5bc: e0853004     	add	r3, r5, r4
  34b5c0: e1a0e00f     	mov	lr, pc
  34b5c4: e593f004     	ldr	pc, [r3, #0x4]
  34b5c8: e3500000     	cmp	r0, #0
  34b5cc: 0a000012     	beq	0x34b61c <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0xfc> @ imm = #0x48
  34b5d0: e580b020     	str	r11, [r0, #0x20]
  34b5d4: e59dc088     	ldr	r12, [sp, #0x88]
  34b5d8: e1a02000     	mov	r2, r0
  34b5dc: e59d1014     	ldr	r1, [sp, #0x14]
  34b5e0: e58dc004     	str	r12, [sp, #0x4]
  34b5e4: e59dc01c     	ldr	r12, [sp, #0x1c]
  34b5e8: e59d3018     	ldr	r3, [sp, #0x18]
  34b5ec: e1a00008     	mov	r0, r8
  34b5f0: e58d6000     	str	r6, [sp]
  34b5f4: e58dc008     	str	r12, [sp, #0x8]
  34b5f8: ebffff1c     	bl	0x34b270 <ObjectManager::Add(ObjectBase*, char const*, char const*, int, bool)> @ imm = #-0x390
  34b5fc: e797300a     	ldr	r3, [r7, r10]
  34b600: e59d205c     	ldr	r2, [sp, #0x5c]
  34b604: e1a00008     	mov	r0, r8
  34b608: e5933000     	ldr	r3, [r3]
  34b60c: e1520003     	cmp	r2, r3
  34b610: 1a000038     	bne	0x34b6f8 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x1d8> @ imm = #0xe0
  34b614: e28dd064     	add	sp, sp, #100
  34b618: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  34b61c: e7973009     	ldr	r3, [r7, r9]
  34b620: e5933000     	ldr	r3, [r3]
  34b624: e3530002     	cmp	r3, #2
  34b628: 05800000     	streq	r0, [r0]
  34b62c: 0affffd9     	beq	0x34b598 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x78> @ imm = #-0x9c
  34b630: e3530001     	cmp	r3, #1
  34b634: 1affffd7     	bne	0x34b598 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x78> @ imm = #-0xa4
  34b638: e59d3020     	ldr	r3, [sp, #0x20]
  34b63c: e300c51e     	movw	r12, #0x51e
  34b640: e59d1024     	ldr	r1, [sp, #0x24]
  34b644: e7970003     	ldr	r0, [r7, r3]
  34b648: e59d2028     	ldr	r2, [sp, #0x28]
  34b64c: e59d302c     	ldr	r3, [sp, #0x2c]
  34b650: e28000a8     	add	r0, r0, #168
  34b654: e2844008     	add	r4, r4, #8
  34b658: e58dc000     	str	r12, [sp]
  34b65c: ebff0a68     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x3d660
  34b660: e3540f42     	cmp	r4, #264
  34b664: 1affffce     	bne	0x34b5a4 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x84> @ imm = #-0xc8
  34b668: e59f30ac     	ldr	r3, [pc, #0xac]         @ 0x34b71c <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x1fc>
  34b66c: e28d4044     	add	r4, sp, #68
  34b670: e7975003     	ldr	r5, [r7, r3]
  34b674: e1a00005     	mov	r0, r5
  34b678: ebffb082     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0x13df8
  34b67c: e59f109c     	ldr	r1, [pc, #0x9c]         @ 0x34b720 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x200>
  34b680: e28d2040     	add	r2, sp, #64
  34b684: e1a00004     	mov	r0, r4
  34b688: e08f1001     	add	r1, pc, r1
  34b68c: ebff2296     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x375a8
  34b690: e1a00005     	mov	r0, r5
  34b694: e1a01004     	mov	r1, r4
  34b698: ebffb0fa     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0x13c18
  34b69c: e59d0058     	ldr	r0, [sp, #0x58]
  34b6a0: e1500004     	cmp	r0, r4
  34b6a4: 0a000006     	beq	0x34b6c4 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x1a4> @ imm = #0x18
  34b6a8: e3500000     	cmp	r0, #0
  34b6ac: 0a000004     	beq	0x34b6c4 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x1a4> @ imm = #0x10
  34b6b0: e59d1044     	ldr	r1, [sp, #0x44]
  34b6b4: e0601001     	rsb	r1, r0, r1
  34b6b8: e3510080     	cmp	r1, #128
  34b6bc: 8a00000b     	bhi	0x34b6f0 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x1d0> @ imm = #0x2c
  34b6c0: eb0ef60e     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x3bd838
  34b6c4: e28d4034     	add	r4, sp, #52
  34b6c8: e1a00004     	mov	r0, r4
  34b6cc: ebffcf8e     	bl	0x33f50c <ObjectHandle::ObjectHandle()> @ imm = #-0xc1c8
  34b6d0: e59d0034     	ldr	r0, [sp, #0x34]
  34b6d4: e5941008     	ldr	r1, [r4, #0x8]
  34b6d8: e59d2038     	ldr	r2, [sp, #0x38]
  34b6dc: e1a03008     	mov	r3, r8
  34b6e0: e4830004     	str	r0, [r3], #4
  34b6e4: e5831004     	str	r1, [r3, #0x4]
  34b6e8: e5882004     	str	r2, [r8, #0x4]
  34b6ec: eaffffc2     	b	0x34b5fc <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0xdc> @ imm = #-0xf8
  34b6f0: ebff1352     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x3b2b8
  34b6f4: eafffff2     	b	0x34b6c4 <ObjectManager::GetNewObject(char const*, char const*, int, bool)+0x1a4> @ imm = #-0x38
  34b6f8: ebff0b04     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x3d3f0
  34b6fc: a0 2e 57 00  	.word	0x00572ea0
  34b700: 4c 95 64 00  	.word	0x0064954c
  34b704: ac 40 00 00  	.word	0x000040ac
  34b708: c0 4c 57 00  	.word	0x00574cc0
  34b70c: 2c 4d 57 00  	.word	0x00574d2c
  34b710: 70 12 61 00  	.word	0x00611270
  34b714: c0 19 00 00  	.word	0x000019c0
  34b718: c0 39 00 00  	.word	0x000039c0
  34b71c: 84 08 00 00  	.word	0x00000884
  34b720: 70 4d 57 00  	.word	0x00574d70
