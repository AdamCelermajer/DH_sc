
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003541d4 <LightSetManager::operator=(LightSetManager const&)>:
  3541d4: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3541d8: e1a07001     	mov	r7, r1
  3541dc: e4973004     	ldr	r3, [r7], #4
  3541e0: e59f6278     	ldr	r6, [pc, #0x278]        @ 0x354460 <LightSetManager::operator=(LightSetManager const&)+0x28c>
  3541e4: e1a0a000     	mov	r10, r0
  3541e8: e24dd014     	sub	sp, sp, #20
  3541ec: e1a05001     	mov	r5, r1
  3541f0: e1a04000     	mov	r4, r0
  3541f4: e48a3004     	str	r3, [r10], #4
  3541f8: e3a08000     	mov	r8, #0
  3541fc: e08f6006     	add	r6, pc, r6
  354200: e08a0008     	add	r0, r10, r8
  354204: e1500007     	cmp	r0, r7
  354208: e2888018     	add	r8, r8, #24
  35420c: 0a000002     	beq	0x35421c <LightSetManager::operator=(LightSetManager const&)+0x48> @ imm = #0x8
  354210: e5971014     	ldr	r1, [r7, #0x14]
  354214: e5972010     	ldr	r2, [r7, #0x10]
  354218: ebfef1f0     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x43840
  35421c: e3580060     	cmp	r8, #96
  354220: e2877018     	add	r7, r7, #24
  354224: 1afffff5     	bne	0x354200 <LightSetManager::operator=(LightSetManager const&)+0x2c> @ imm = #-0x2c
  354228: e284c064     	add	r12, r4, #100
  35422c: e285e064     	add	lr, r5, #100
  354230: e8be000f     	ldm	lr!, {r0, r1, r2, r3}
  354234: e8ac000f     	stm	r12!, {r0, r1, r2, r3}
  354238: e59f1224     	ldr	r1, [pc, #0x224]        @ 0x354464 <LightSetManager::operator=(LightSetManager const&)+0x290>
  35423c: e59e3000     	ldr	r3, [lr]
  354240: e3a09000     	mov	r9, #0
  354244: e58d1004     	str	r1, [sp, #0x4]
  354248: e2842078     	add	r2, r4, #120
  35424c: e58c3000     	str	r3, [r12]
  354250: e2853078     	add	r3, r5, #120
  354254: e58d2008     	str	r2, [sp, #0x8]
  354258: e58d300c     	str	r3, [sp, #0xc]
  35425c: e1a0b009     	mov	r11, r9
  354260: e59d100c     	ldr	r1, [sp, #0xc]
  354264: e59d2008     	ldr	r2, [sp, #0x8]
  354268: e3a07000     	mov	r7, #0
  35426c: e081a009     	add	r10, r1, r9
  354270: e0828009     	add	r8, r2, r9
  354274: e79a2007     	ldr	r2, [r10, r7]
  354278: e2877004     	add	r7, r7, #4
  35427c: e3520000     	cmp	r2, #0
  354280: 15923000     	ldrne	r3, [r2]
  354284: 12833001     	addne	r3, r3, #1
  354288: 15823000     	strne	r3, [r2]
  35428c: e5983000     	ldr	r3, [r8]
  354290: e5882000     	str	r2, [r8]
  354294: e2888004     	add	r8, r8, #4
  354298: e3530000     	cmp	r3, #0
  35429c: e1a00003     	mov	r0, r3
  3542a0: 0a00000f     	beq	0x3542e4 <LightSetManager::operator=(LightSetManager const&)+0x110> @ imm = #0x3c
  3542a4: e5932000     	ldr	r2, [r3]
  3542a8: e2422001     	sub	r2, r2, #1
  3542ac: e3520000     	cmp	r2, #0
  3542b0: e5832000     	str	r2, [r3]
  3542b4: 1a00000a     	bne	0x3542e4 <LightSetManager::operator=(LightSetManager const&)+0x110> @ imm = #0x28
  3542b8: e5d32054     	ldrb	r2, [r3, #0x54]
  3542bc: e3520000     	cmp	r2, #0
  3542c0: 1a000005     	bne	0x3542dc <LightSetManager::operator=(LightSetManager const&)+0x108> @ imm = #0x14
  3542c4: e59d1004     	ldr	r1, [sp, #0x4]
  3542c8: e7962001     	ldr	r2, [r6, r1]
  3542cc: e5931050     	ldr	r1, [r3, #0x50]
  3542d0: e592c000     	ldr	r12, [r2]
  3542d4: e581c000     	str	r12, [r1]
  3542d8: e5821000     	str	r1, [r2]
  3542dc: e583b050     	str	r11, [r3, #0x50]
  3542e0: ebfef056     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x43ea8
  3542e4: e3570014     	cmp	r7, #20
  3542e8: 1affffe1     	bne	0x354274 <LightSetManager::operator=(LightSetManager const&)+0xa0> @ imm = #-0x7c
  3542ec: e2899014     	add	r9, r9, #20
  3542f0: e3590050     	cmp	r9, #80
  3542f4: 1affffd9     	bne	0x354260 <LightSetManager::operator=(LightSetManager const&)+0x8c> @ imm = #-0x9c
  3542f8: e59fb164     	ldr	r11, [pc, #0x164]       @ 0x354464 <LightSetManager::operator=(LightSetManager const&)+0x290>
  3542fc: e3a07000     	mov	r7, #0
  354300: e28480c8     	add	r8, r4, #200
  354304: e285a0c8     	add	r10, r5, #200
  354308: e1a09007     	mov	r9, r7
  35430c: e79a2007     	ldr	r2, [r10, r7]
  354310: e2877004     	add	r7, r7, #4
  354314: e3520000     	cmp	r2, #0
  354318: 15923000     	ldrne	r3, [r2]
  35431c: 12833001     	addne	r3, r3, #1
  354320: 15823000     	strne	r3, [r2]
  354324: e5983000     	ldr	r3, [r8]
  354328: e5882000     	str	r2, [r8]
  35432c: e2888004     	add	r8, r8, #4
  354330: e3530000     	cmp	r3, #0
  354334: e1a00003     	mov	r0, r3
  354338: 0a00000d     	beq	0x354374 <LightSetManager::operator=(LightSetManager const&)+0x1a0> @ imm = #0x34
  35433c: e5932000     	ldr	r2, [r3]
  354340: e2422001     	sub	r2, r2, #1
  354344: e3520000     	cmp	r2, #0
  354348: e5832000     	str	r2, [r3]
  35434c: 1a000008     	bne	0x354374 <LightSetManager::operator=(LightSetManager const&)+0x1a0> @ imm = #0x20
  354350: e5d32054     	ldrb	r2, [r3, #0x54]
  354354: e3520000     	cmp	r2, #0
  354358: 0796200b     	ldreq	r2, [r6, r11]
  35435c: 05931050     	ldreq	r1, [r3, #0x50]
  354360: 0592c000     	ldreq	r12, [r2]
  354364: 0581c000     	streq	r12, [r1]
  354368: 05821000     	streq	r1, [r2]
  35436c: e5839050     	str	r9, [r3, #0x50]
  354370: ebfef032     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x43f38
  354374: e3570014     	cmp	r7, #20
  354378: 1affffe3     	bne	0x35430c <LightSetManager::operator=(LightSetManager const&)+0x138> @ imm = #-0x74
  35437c: e284c0dc     	add	r12, r4, #220
  354380: e28500dc     	add	r0, r5, #220
  354384: e3a03000     	mov	r3, #0
  354388: e0802003     	add	r2, r0, r3
  35438c: e5921004     	ldr	r1, [r2, #0x4]
  354390: e08c2003     	add	r2, r12, r3
  354394: e2833008     	add	r3, r3, #8
  354398: e3530040     	cmp	r3, #64
  35439c: e5821004     	str	r1, [r2, #0x4]
  3543a0: 1afffff8     	bne	0x354388 <LightSetManager::operator=(LightSetManager const&)+0x1b4> @ imm = #-0x20
  3543a4: e2843f47     	add	r3, r4, #284
  3543a8: e2852f47     	add	r2, r5, #284
  3543ac: e2840f5b     	add	r0, r4, #364
  3543b0: e5921004     	ldr	r1, [r2, #0x4]
  3543b4: e5831004     	str	r1, [r3, #0x4]
  3543b8: e5921008     	ldr	r1, [r2, #0x8]
  3543bc: e5831008     	str	r1, [r3, #0x8]
  3543c0: e592100c     	ldr	r1, [r2, #0xc]
  3543c4: e583100c     	str	r1, [r3, #0xc]
  3543c8: e5921010     	ldr	r1, [r2, #0x10]
  3543cc: e2822014     	add	r2, r2, #20
  3543d0: e5831010     	str	r1, [r3, #0x10]
  3543d4: e2833014     	add	r3, r3, #20
  3543d8: e1530000     	cmp	r3, r0
  3543dc: 1afffff3     	bne	0x3543b0 <LightSetManager::operator=(LightSetManager const&)+0x1dc> @ imm = #-0x34
  3543e0: e595316c     	ldr	r3, [r5, #0x16c]
  3543e4: e2840e19     	add	r0, r4, #400
  3543e8: e2851e19     	add	r1, r5, #400
  3543ec: e584316c     	str	r3, [r4, #0x16c]
  3543f0: e5953170     	ldr	r3, [r5, #0x170]
  3543f4: e5843170     	str	r3, [r4, #0x170]
  3543f8: e5953174     	ldr	r3, [r5, #0x174]
  3543fc: e5843174     	str	r3, [r4, #0x174]
  354400: e5953178     	ldr	r3, [r5, #0x178]
  354404: e5843178     	str	r3, [r4, #0x178]
  354408: e595317c     	ldr	r3, [r5, #0x17c]
  35440c: e584317c     	str	r3, [r4, #0x17c]
  354410: e5953180     	ldr	r3, [r5, #0x180]
  354414: e5843180     	str	r3, [r4, #0x180]
  354418: e5953184     	ldr	r3, [r5, #0x184]
  35441c: e5843184     	str	r3, [r4, #0x184]
  354420: e5953188     	ldr	r3, [r5, #0x188]
  354424: e5843188     	str	r3, [r4, #0x188]
  354428: e595318c     	ldr	r3, [r5, #0x18c]
  35442c: e584318c     	str	r3, [r4, #0x18c]
  354430: ebffff22     	bl	0x3540c0 <std::vector<LightBase*, glitch::core::SAllocator<LightBase*, (glitch::memory::E_MEMORY_HINT)0>>::operator=(std::vector<LightBase*, glitch::core::SAllocator<LightBase*, (glitch::memory::E_MEMORY_HINT)0>> const&)> @ imm = #-0x378
  354434: e5d5319c     	ldrb	r3, [r5, #0x19c]
  354438: e1a00004     	mov	r0, r4
  35443c: e5c4319c     	strb	r3, [r4, #0x19c]
  354440: e5d5319d     	ldrb	r3, [r5, #0x19d]
  354444: e5c4319d     	strb	r3, [r4, #0x19d]
  354448: e5d5319e     	ldrb	r3, [r5, #0x19e]
  35444c: e5c4319e     	strb	r3, [r4, #0x19e]
  354450: e59531a0     	ldr	r3, [r5, #0x1a0]
  354454: e58431a0     	str	r3, [r4, #0x1a0]
  354458: e28dd014     	add	sp, sp, #20
  35445c: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  354460: 94 08 64 00  	.word	0x00640894
  354464: c0 3c 00 00  	.word	0x00003cc0
