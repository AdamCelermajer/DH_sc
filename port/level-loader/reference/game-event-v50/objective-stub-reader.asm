
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00504818 <Structs::v2QuestObjectiveStub::read(IStreamBase*)>:
  504818: e92d4070     	push	{r4, r5, r6, lr}
  50481c: e1a04000     	mov	r4, r0
  504820: e24dd008     	sub	sp, sp, #8
  504824: e1a05001     	mov	r5, r1
  504828: ebfffb57     	bl	0x50358c <Structs::v2QuestObjective::read(IStreamBase*)> @ imm = #-0x12a4
  50482c: e1a00005     	mov	r0, r5
  504830: e2841010     	add	r1, r4, #16
  504834: ebfb6a59     	bl	0x3df1a0 <void StreamReader::readAs<unsigned int>(IStreamBase*, unsigned int*)> @ imm = #-0x12569c
  504838: e3a03001     	mov	r3, #1
  50483c: e3530000     	cmp	r3, #0
  504840: e58d3004     	str	r3, [sp, #0x4]
  504844: 1a00000f     	bne	0x504888 <Structs::v2QuestObjectiveStub::read(IStreamBase*)+0x70> @ imm = #0x3c
  504848: e2843011     	add	r3, r4, #17
  50484c: e2842012     	add	r2, r4, #18
  504850: e5d20001     	ldrb	r0, [r2, #0x1]
  504854: e5531001     	ldrb	r1, [r3, #-0x1]
  504858: e1520003     	cmp	r2, r3
  50485c: e0201001     	eor	r1, r0, r1
  504860: e5431001     	strb	r1, [r3, #-0x1]
  504864: e5d20001     	ldrb	r0, [r2, #0x1]
  504868: e0211000     	eor	r1, r1, r0
  50486c: e5c21001     	strb	r1, [r2, #0x1]
  504870: e5530001     	ldrb	r0, [r3, #-0x1]
  504874: e2422001     	sub	r2, r2, #1
  504878: e0211000     	eor	r1, r1, r0
  50487c: e5431001     	strb	r1, [r3, #-0x1]
  504880: e2833001     	add	r3, r3, #1
  504884: 8afffff1     	bhi	0x504850 <Structs::v2QuestObjectiveStub::read(IStreamBase*)+0x38> @ imm = #-0x3c
  504888: e5940014     	ldr	r0, [r4, #0x14]
  50488c: e3500000     	cmp	r0, #0
  504890: 0a000000     	beq	0x504898 <Structs::v2QuestObjectiveStub::read(IStreamBase*)+0x80> @ imm = #0x0
  504894: ebf82ee9     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1f445c
  504898: e5940010     	ldr	r0, [r4, #0x10]
  50489c: e3a01001     	mov	r1, #1
  5048a0: e3a06000     	mov	r6, #0
  5048a4: e0800001     	add	r0, r0, r1
  5048a8: ebf82f2f     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x1f4344
  5048ac: e5942010     	ldr	r2, [r4, #0x10]
  5048b0: e1a01000     	mov	r1, r0
  5048b4: e5840014     	str	r0, [r4, #0x14]
  5048b8: e1a03006     	mov	r3, r6
  5048bc: e1a00005     	mov	r0, r5
  5048c0: ebf84ae3     	bl	0x317454 <StreamReader::readStringEx(IStreamBase*, char*, unsigned long long)> @ imm = #-0x1ed474
  5048c4: e5943010     	ldr	r3, [r4, #0x10]
  5048c8: e5942014     	ldr	r2, [r4, #0x14]
  5048cc: e1a00005     	mov	r0, r5
  5048d0: e2841018     	add	r1, r4, #24
  5048d4: e7c26003     	strb	r6, [r2, r3]
  5048d8: ebfb6a30     	bl	0x3df1a0 <void StreamReader::readAs<unsigned int>(IStreamBase*, unsigned int*)> @ imm = #-0x125740
  5048dc: e3a03001     	mov	r3, #1
  5048e0: e1530006     	cmp	r3, r6
  5048e4: e58d3004     	str	r3, [sp, #0x4]
  5048e8: 1a00000f     	bne	0x50492c <Structs::v2QuestObjectiveStub::read(IStreamBase*)+0x114> @ imm = #0x3c
  5048ec: e2843019     	add	r3, r4, #25
  5048f0: e284201a     	add	r2, r4, #26
  5048f4: e5d20001     	ldrb	r0, [r2, #0x1]
  5048f8: e5531001     	ldrb	r1, [r3, #-0x1]
  5048fc: e1520003     	cmp	r2, r3
  504900: e0201001     	eor	r1, r0, r1
  504904: e5431001     	strb	r1, [r3, #-0x1]
  504908: e5d20001     	ldrb	r0, [r2, #0x1]
  50490c: e0211000     	eor	r1, r1, r0
  504910: e5c21001     	strb	r1, [r2, #0x1]
  504914: e5530001     	ldrb	r0, [r3, #-0x1]
  504918: e2422001     	sub	r2, r2, #1
  50491c: e0211000     	eor	r1, r1, r0
  504920: e5431001     	strb	r1, [r3, #-0x1]
  504924: e2833001     	add	r3, r3, #1
  504928: 8afffff1     	bhi	0x5048f4 <Structs::v2QuestObjectiveStub::read(IStreamBase*)+0xdc> @ imm = #-0x3c
  50492c: e594001c     	ldr	r0, [r4, #0x1c]
  504930: e3500000     	cmp	r0, #0
  504934: 0a000000     	beq	0x50493c <Structs::v2QuestObjectiveStub::read(IStreamBase*)+0x124> @ imm = #0x0
  504938: ebf82ec0     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1f4500
  50493c: e5940018     	ldr	r0, [r4, #0x18]
  504940: e3a01001     	mov	r1, #1
  504944: e3a06000     	mov	r6, #0
  504948: e0800001     	add	r0, r0, r1
  50494c: ebf82f06     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x1f43e8
  504950: e5942018     	ldr	r2, [r4, #0x18]
  504954: e1a01000     	mov	r1, r0
  504958: e584001c     	str	r0, [r4, #0x1c]
  50495c: e1a03006     	mov	r3, r6
  504960: e1a00005     	mov	r0, r5
  504964: ebf84aba     	bl	0x317454 <StreamReader::readStringEx(IStreamBase*, char*, unsigned long long)> @ imm = #-0x1ed518
  504968: e5943018     	ldr	r3, [r4, #0x18]
  50496c: e594201c     	ldr	r2, [r4, #0x1c]
  504970: e1a00005     	mov	r0, r5
  504974: e2841020     	add	r1, r4, #32
  504978: e7c26003     	strb	r6, [r2, r3]
  50497c: ebfd51c3     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0xab8f4
  504980: e3a03001     	mov	r3, #1
  504984: e1530006     	cmp	r3, r6
  504988: e58d3004     	str	r3, [sp, #0x4]
  50498c: 1a00000f     	bne	0x5049d0 <Structs::v2QuestObjectiveStub::read(IStreamBase*)+0x1b8> @ imm = #0x3c
  504990: e2843021     	add	r3, r4, #33
  504994: e2842022     	add	r2, r4, #34
  504998: e5d20001     	ldrb	r0, [r2, #0x1]
  50499c: e5531001     	ldrb	r1, [r3, #-0x1]
  5049a0: e1520003     	cmp	r2, r3
  5049a4: e0201001     	eor	r1, r0, r1
  5049a8: e5431001     	strb	r1, [r3, #-0x1]
  5049ac: e5d20001     	ldrb	r0, [r2, #0x1]
  5049b0: e0211000     	eor	r1, r1, r0
  5049b4: e5c21001     	strb	r1, [r2, #0x1]
  5049b8: e5530001     	ldrb	r0, [r3, #-0x1]
  5049bc: e2422001     	sub	r2, r2, #1
  5049c0: e0211000     	eor	r1, r1, r0
  5049c4: e5431001     	strb	r1, [r3, #-0x1]
  5049c8: e2833001     	add	r3, r3, #1
  5049cc: 8afffff1     	bhi	0x504998 <Structs::v2QuestObjectiveStub::read(IStreamBase*)+0x180> @ imm = #-0x3c
  5049d0: e1a00005     	mov	r0, r5
  5049d4: e2841024     	add	r1, r4, #36
  5049d8: ebfd51ac     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0xab950
  5049dc: e3a03001     	mov	r3, #1
  5049e0: e3530000     	cmp	r3, #0
  5049e4: e58d3004     	str	r3, [sp, #0x4]
  5049e8: 1a00000f     	bne	0x504a2c <Structs::v2QuestObjectiveStub::read(IStreamBase*)+0x214> @ imm = #0x3c
  5049ec: e2843025     	add	r3, r4, #37
  5049f0: e2842026     	add	r2, r4, #38
  5049f4: e5d20001     	ldrb	r0, [r2, #0x1]
  5049f8: e5531001     	ldrb	r1, [r3, #-0x1]
  5049fc: e1520003     	cmp	r2, r3
  504a00: e0201001     	eor	r1, r0, r1
  504a04: e5431001     	strb	r1, [r3, #-0x1]
  504a08: e5d20001     	ldrb	r0, [r2, #0x1]
  504a0c: e0211000     	eor	r1, r1, r0
  504a10: e5c21001     	strb	r1, [r2, #0x1]
  504a14: e5530001     	ldrb	r0, [r3, #-0x1]
  504a18: e2422001     	sub	r2, r2, #1
  504a1c: e0211000     	eor	r1, r1, r0
  504a20: e5431001     	strb	r1, [r3, #-0x1]
  504a24: e2833001     	add	r3, r3, #1
  504a28: 8afffff1     	bhi	0x5049f4 <Structs::v2QuestObjectiveStub::read(IStreamBase*)+0x1dc> @ imm = #-0x3c
  504a2c: e1a00005     	mov	r0, r5
  504a30: e2841028     	add	r1, r4, #40
  504a34: ebfd5195     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0xab9ac
  504a38: e3a03001     	mov	r3, #1
  504a3c: e3530000     	cmp	r3, #0
  504a40: e58d3004     	str	r3, [sp, #0x4]
  504a44: 1a00000f     	bne	0x504a88 <Structs::v2QuestObjectiveStub::read(IStreamBase*)+0x270> @ imm = #0x3c
  504a48: e284302a     	add	r3, r4, #42
  504a4c: e2844029     	add	r4, r4, #41
  504a50: e5d31001     	ldrb	r1, [r3, #0x1]
  504a54: e5542001     	ldrb	r2, [r4, #-0x1]
  504a58: e1530004     	cmp	r3, r4
  504a5c: e0212002     	eor	r2, r1, r2
  504a60: e5442001     	strb	r2, [r4, #-0x1]
  504a64: e5d31001     	ldrb	r1, [r3, #0x1]
  504a68: e0222001     	eor	r2, r2, r1
  504a6c: e5c32001     	strb	r2, [r3, #0x1]
  504a70: e5541001     	ldrb	r1, [r4, #-0x1]
  504a74: e2433001     	sub	r3, r3, #1
  504a78: e0222001     	eor	r2, r2, r1
  504a7c: e5442001     	strb	r2, [r4, #-0x1]
  504a80: e2844001     	add	r4, r4, #1
  504a84: 8afffff1     	bhi	0x504a50 <Structs::v2QuestObjectiveStub::read(IStreamBase*)+0x238> @ imm = #-0x3c
  504a88: e28dd008     	add	sp, sp, #8
  504a8c: e8bd8070     	pop	{r4, r5, r6, pc}

00504a90 <Structs::v2QuestAutomatic::read(IStreamBase*)>:
  504a90: eaffff60     	b	0x504818 <Structs::v2QuestObjectiveStub::read(IStreamBase*)> @ imm = #-0x280

00504a94 <Structs::v2Quest::read(IStreamBase*)>:
  504a94: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  504a98: e1a04000     	mov	r4, r0
  504a9c: e24dd008     	sub	sp, sp, #8
  504aa0: e1a00001     	mov	r0, r1
  504aa4: e1a05001     	mov	r5, r1
  504aa8: e59f893c     	ldr	r8, [pc, #0x93c]        @ 0x5053ec <Structs::v2Quest::read(IStreamBase*)+0x958>
  504aac: e2841004     	add	r1, r4, #4
  504ab0: ebfd5176     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0xaba28
  504ab4: e3a03001     	mov	r3, #1
  504ab8: e3530000     	cmp	r3, #0
  504abc: e58d3004     	str	r3, [sp, #0x4]
  504ac0: e08f8008     	add	r8, pc, r8
  504ac4: 1a00000f     	bne	0x504b08 <Structs::v2Quest::read(IStreamBase*)+0x74> @ imm = #0x3c
  504ac8: e2843005     	add	r3, r4, #5
  504acc: e2842006     	add	r2, r4, #6
  504ad0: e5d20001     	ldrb	r0, [r2, #0x1]
  504ad4: e5531001     	ldrb	r1, [r3, #-0x1]
  504ad8: e1530002     	cmp	r3, r2
  504adc: e0201001     	eor	r1, r0, r1
  504ae0: e5431001     	strb	r1, [r3, #-0x1]
  504ae4: e5d20001     	ldrb	r0, [r2, #0x1]
  504ae8: e0211000     	eor	r1, r1, r0
  504aec: e5c21001     	strb	r1, [r2, #0x1]
  504af0: e5530001     	ldrb	r0, [r3, #-0x1]
  504af4: e2422001     	sub	r2, r2, #1
  504af8: e0211000     	eor	r1, r1, r0
  504afc: e5431001     	strb	r1, [r3, #-0x1]
  504b00: e2833001     	add	r3, r3, #1
  504b04: 3afffff1     	blo	0x504ad0 <Structs::v2Quest::read(IStreamBase*)+0x3c> @ imm = #-0x3c
  504b08: e1a00005     	mov	r0, r5
  504b0c: e2841008     	add	r1, r4, #8
  504b10: ebfd515e     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0xaba88
  504b14: e3a03001     	mov	r3, #1
  504b18: e3530000     	cmp	r3, #0
  504b1c: e58d3004     	str	r3, [sp, #0x4]
  504b20: 1a00000f     	bne	0x504b64 <Structs::v2Quest::read(IStreamBase*)+0xd0> @ imm = #0x3c
  504b24: e2843009     	add	r3, r4, #9
  504b28: e284200a     	add	r2, r4, #10
  504b2c: e5d20001     	ldrb	r0, [r2, #0x1]
  504b30: e5531001     	ldrb	r1, [r3, #-0x1]
  504b34: e1530002     	cmp	r3, r2
  504b38: e0201001     	eor	r1, r0, r1
  504b3c: e5431001     	strb	r1, [r3, #-0x1]
  504b40: e5d20001     	ldrb	r0, [r2, #0x1]
  504b44: e0211000     	eor	r1, r1, r0
  504b48: e5c21001     	strb	r1, [r2, #0x1]
  504b4c: e5530001     	ldrb	r0, [r3, #-0x1]
  504b50: e2422001     	sub	r2, r2, #1
  504b54: e0211000     	eor	r1, r1, r0
  504b58: e5431001     	strb	r1, [r3, #-0x1]
  504b5c: e2833001     	add	r3, r3, #1
  504b60: 3afffff1     	blo	0x504b2c <Structs::v2Quest::read(IStreamBase*)+0x98> @ imm = #-0x3c
  504b64: e1a00005     	mov	r0, r5
  504b68: e284100c     	add	r1, r4, #12
  504b6c: ebfd5147     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0xabae4
  504b70: e3a03001     	mov	r3, #1
  504b74: e3530000     	cmp	r3, #0
  504b78: e58d3004     	str	r3, [sp, #0x4]
  504b7c: 1a00000f     	bne	0x504bc0 <Structs::v2Quest::read(IStreamBase*)+0x12c> @ imm = #0x3c
  504b80: e284300d     	add	r3, r4, #13
  504b84: e284200e     	add	r2, r4, #14
  504b88: e5d20001     	ldrb	r0, [r2, #0x1]
  504b8c: e5531001     	ldrb	r1, [r3, #-0x1]
  504b90: e1530002     	cmp	r3, r2
  504b94: e0201001     	eor	r1, r0, r1
  504b98: e5431001     	strb	r1, [r3, #-0x1]
  504b9c: e5d20001     	ldrb	r0, [r2, #0x1]
  504ba0: e0211000     	eor	r1, r1, r0
  504ba4: e5c21001     	strb	r1, [r2, #0x1]
  504ba8: e5530001     	ldrb	r0, [r3, #-0x1]
  504bac: e2422001     	sub	r2, r2, #1
  504bb0: e0211000     	eor	r1, r1, r0
  504bb4: e5431001     	strb	r1, [r3, #-0x1]
  504bb8: e2833001     	add	r3, r3, #1
  504bbc: 3afffff1     	blo	0x504b88 <Structs::v2Quest::read(IStreamBase*)+0xf4> @ imm = #-0x3c
  504bc0: e1a00005     	mov	r0, r5
  504bc4: e2841010     	add	r1, r4, #16
  504bc8: ebfd5130     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0xabb40
  504bcc: e3a03001     	mov	r3, #1
  504bd0: e3530000     	cmp	r3, #0
  504bd4: e58d3004     	str	r3, [sp, #0x4]
  504bd8: 1a00000f     	bne	0x504c1c <Structs::v2Quest::read(IStreamBase*)+0x188> @ imm = #0x3c
  504bdc: e2843011     	add	r3, r4, #17
  504be0: e2842012     	add	r2, r4, #18
  504be4: e5d20001     	ldrb	r0, [r2, #0x1]
  504be8: e5531001     	ldrb	r1, [r3, #-0x1]
  504bec: e1530002     	cmp	r3, r2
  504bf0: e0201001     	eor	r1, r0, r1
  504bf4: e5431001     	strb	r1, [r3, #-0x1]
  504bf8: e5d20001     	ldrb	r0, [r2, #0x1]
  504bfc: e0211000     	eor	r1, r1, r0
  504c00: e5c21001     	strb	r1, [r2, #0x1]
  504c04: e5530001     	ldrb	r0, [r3, #-0x1]
  504c08: e2422001     	sub	r2, r2, #1
  504c0c: e0211000     	eor	r1, r1, r0
  504c10: e5431001     	strb	r1, [r3, #-0x1]
  504c14: e2833001     	add	r3, r3, #1
  504c18: 3afffff1     	blo	0x504be4 <Structs::v2Quest::read(IStreamBase*)+0x150> @ imm = #-0x3c
  504c1c: e1a00005     	mov	r0, r5
  504c20: e2841014     	add	r1, r4, #20
  504c24: ebfb695d     	bl	0x3df1a0 <void StreamReader::readAs<unsigned int>(IStreamBase*, unsigned int*)> @ imm = #-0x125a8c
  504c28: e3a03001     	mov	r3, #1
  504c2c: e3530000     	cmp	r3, #0
  504c30: e58d3004     	str	r3, [sp, #0x4]
  504c34: 1a00000f     	bne	0x504c78 <Structs::v2Quest::read(IStreamBase*)+0x1e4> @ imm = #0x3c
  504c38: e2843015     	add	r3, r4, #21
  504c3c: e2842016     	add	r2, r4, #22
  504c40: e5d20001     	ldrb	r0, [r2, #0x1]
  504c44: e5531001     	ldrb	r1, [r3, #-0x1]
  504c48: e1530002     	cmp	r3, r2
  504c4c: e0201001     	eor	r1, r0, r1
  504c50: e5431001     	strb	r1, [r3, #-0x1]
  504c54: e5d20001     	ldrb	r0, [r2, #0x1]
  504c58: e0211000     	eor	r1, r1, r0
  504c5c: e5c21001     	strb	r1, [r2, #0x1]
  504c60: e5530001     	ldrb	r0, [r3, #-0x1]
  504c64: e2422001     	sub	r2, r2, #1
  504c68: e0211000     	eor	r1, r1, r0
  504c6c: e5431001     	strb	r1, [r3, #-0x1]
  504c70: e2833001     	add	r3, r3, #1
  504c74: 3afffff1     	blo	0x504c40 <Structs::v2Quest::read(IStreamBase*)+0x1ac> @ imm = #-0x3c
  504c78: e5943018     	ldr	r3, [r4, #0x18]
  504c7c: e3530000     	cmp	r3, #0
  504c80: 0a00000f     	beq	0x504cc4 <Structs::v2Quest::read(IStreamBase*)+0x230> @ imm = #0x3c
  504c84: e5130004     	ldr	r0, [r3, #-0x4]
  504c88: e0830200     	add	r0, r3, r0, lsl #4
  504c8c: e1530000     	cmp	r3, r0
  504c90: 1a000001     	bne	0x504c9c <Structs::v2Quest::read(IStreamBase*)+0x208> @ imm = #0x4
  504c94: ea000008     	b	0x504cbc <Structs::v2Quest::read(IStreamBase*)+0x228> @ imm = #0x20
  504c98: e1a00006     	mov	r0, r6
  504c9c: e2406010     	sub	r6, r0, #16
  504ca0: e5103010     	ldr	r3, [r0, #-0x10]
  504ca4: e1a00006     	mov	r0, r6
  504ca8: e1a0e00f     	mov	lr, pc
  504cac: e593f000     	ldr	pc, [r3]
  504cb0: e5940018     	ldr	r0, [r4, #0x18]
  504cb4: e1500006     	cmp	r0, r6
  504cb8: 1afffff6     	bne	0x504c98 <Structs::v2Quest::read(IStreamBase*)+0x204> @ imm = #-0x28
  504cbc: e2400008     	sub	r0, r0, #8
  504cc0: ebf82dde     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1f4888
  504cc4: e5946014     	ldr	r6, [r4, #0x14]
  504cc8: e3a01001     	mov	r1, #1
  504ccc: e1a00206     	lsl	r0, r6, #4
  504cd0: e2800008     	add	r0, r0, #8
  504cd4: ebf82e24     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x1f4770
  504cd8: e3a03010     	mov	r3, #16
  504cdc: e3560000     	cmp	r6, #0
  504ce0: e8800048     	stm	r0, {r3, r6}
  504ce4: e2803008     	add	r3, r0, #8
  504ce8: 0a000008     	beq	0x504d10 <Structs::v2Quest::read(IStreamBase*)+0x27c> @ imm = #0x20
  504cec: e59f16fc     	ldr	r1, [pc, #0x6fc]        @ 0x5053f0 <Structs::v2Quest::read(IStreamBase*)+0x95c>
  504cf0: e3a02000     	mov	r2, #0
  504cf4: e7981001     	ldr	r1, [r8, r1]
  504cf8: e2811008     	add	r1, r1, #8
  504cfc: e2822001     	add	r2, r2, #1
  504d00: e1520006     	cmp	r2, r6
  504d04: e5801008     	str	r1, [r0, #0x8]
  504d08: e2800010     	add	r0, r0, #16
  504d0c: 1afffffa     	bne	0x504cfc <Structs::v2Quest::read(IStreamBase*)+0x268> @ imm = #-0x18
  504d10: e5942014     	ldr	r2, [r4, #0x14]
  504d14: e5843018     	str	r3, [r4, #0x18]
  504d18: e3520000     	cmp	r2, #0
  504d1c: 0a00000b     	beq	0x504d50 <Structs::v2Quest::read(IStreamBase*)+0x2bc> @ imm = #0x2c
  504d20: e3a06000     	mov	r6, #0
  504d24: ea000000     	b	0x504d2c <Structs::v2Quest::read(IStreamBase*)+0x298> @ imm = #0x0
  504d28: e5943018     	ldr	r3, [r4, #0x18]
  504d2c: e0830206     	add	r0, r3, r6, lsl #4
  504d30: e1a01005     	mov	r1, r5
  504d34: e7933206     	ldr	r3, [r3, r6, lsl #4]
  504d38: e1a0e00f     	mov	lr, pc
  504d3c: e593f00c     	ldr	pc, [r3, #0xc]
  504d40: e5943014     	ldr	r3, [r4, #0x14]
  504d44: e2866001     	add	r6, r6, #1
  504d48: e1530006     	cmp	r3, r6
  504d4c: 8afffff5     	bhi	0x504d28 <Structs::v2Quest::read(IStreamBase*)+0x294> @ imm = #-0x2c
  504d50: e1a00005     	mov	r0, r5
  504d54: e284101c     	add	r1, r4, #28
  504d58: ebfb6910     	bl	0x3df1a0 <void StreamReader::readAs<unsigned int>(IStreamBase*, unsigned int*)> @ imm = #-0x125bc0
  504d5c: e3a03001     	mov	r3, #1
  504d60: e3530000     	cmp	r3, #0
  504d64: e58d3004     	str	r3, [sp, #0x4]
  504d68: 1a00000f     	bne	0x504dac <Structs::v2Quest::read(IStreamBase*)+0x318> @ imm = #0x3c
  504d6c: e284301d     	add	r3, r4, #29
  504d70: e284201e     	add	r2, r4, #30
  504d74: e5d20001     	ldrb	r0, [r2, #0x1]
  504d78: e5531001     	ldrb	r1, [r3, #-0x1]
  504d7c: e1530002     	cmp	r3, r2
  504d80: e0201001     	eor	r1, r0, r1
  504d84: e5431001     	strb	r1, [r3, #-0x1]
  504d88: e5d20001     	ldrb	r0, [r2, #0x1]
  504d8c: e0211000     	eor	r1, r1, r0
  504d90: e5c21001     	strb	r1, [r2, #0x1]
  504d94: e5530001     	ldrb	r0, [r3, #-0x1]
  504d98: e2422001     	sub	r2, r2, #1
  504d9c: e0211000     	eor	r1, r1, r0
  504da0: e5431001     	strb	r1, [r3, #-0x1]
  504da4: e2833001     	add	r3, r3, #1
  504da8: 3afffff1     	blo	0x504d74 <Structs::v2Quest::read(IStreamBase*)+0x2e0> @ imm = #-0x3c
  504dac: e5943020     	ldr	r3, [r4, #0x20]
  504db0: e3530000     	cmp	r3, #0
  504db4: 0a000010     	beq	0x504dfc <Structs::v2Quest::read(IStreamBase*)+0x368> @ imm = #0x40
  504db8: e5132004     	ldr	r2, [r3, #-0x4]
  504dbc: e3a0002c     	mov	r0, #44
  504dc0: e0203290     	mla	r0, r0, r2, r3
  504dc4: e1530000     	cmp	r3, r0
  504dc8: 1a000001     	bne	0x504dd4 <Structs::v2Quest::read(IStreamBase*)+0x340> @ imm = #0x4
  504dcc: ea000008     	b	0x504df4 <Structs::v2Quest::read(IStreamBase*)+0x360> @ imm = #0x20
  504dd0: e1a00006     	mov	r0, r6
  504dd4: e240602c     	sub	r6, r0, #44
  504dd8: e510302c     	ldr	r3, [r0, #-0x2c]
  504ddc: e1a00006     	mov	r0, r6
  504de0: e1a0e00f     	mov	lr, pc
  504de4: e593f000     	ldr	pc, [r3]
  504de8: e5940020     	ldr	r0, [r4, #0x20]
  504dec: e1500006     	cmp	r0, r6
  504df0: 1afffff6     	bne	0x504dd0 <Structs::v2Quest::read(IStreamBase*)+0x33c> @ imm = #-0x28
  504df4: e2400008     	sub	r0, r0, #8
  504df8: ebf82d90     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1f49c0
  504dfc: e594601c     	ldr	r6, [r4, #0x1c]
  504e00: e3a0702c     	mov	r7, #44
  504e04: e3a01001     	mov	r1, #1
  504e08: e0000697     	mul	r0, r7, r6
  504e0c: e2800008     	add	r0, r0, #8
  504e10: ebf82dd5     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x1f48ac
  504e14: e3560000     	cmp	r6, #0
  504e18: e5807000     	str	r7, [r0]
  504e1c: e5806004     	str	r6, [r0, #0x4]
  504e20: e2803008     	add	r3, r0, #8
  504e24: 0a00000b     	beq	0x504e58 <Structs::v2Quest::read(IStreamBase*)+0x3c4> @ imm = #0x2c
  504e28: e59f15c4     	ldr	r1, [pc, #0x5c4]        @ 0x5053f4 <Structs::v2Quest::read(IStreamBase*)+0x960>
  504e2c: e3a02000     	mov	r2, #0
  504e30: e798c001     	ldr	r12, [r8, r1]
  504e34: e1a01002     	mov	r1, r2
  504e38: e28cc008     	add	r12, r12, #8
  504e3c: e2822001     	add	r2, r2, #1
  504e40: e1520006     	cmp	r2, r6
  504e44: e580c008     	str	r12, [r0, #0x8]
  504e48: e580101c     	str	r1, [r0, #0x1c]
  504e4c: e5801024     	str	r1, [r0, #0x24]
  504e50: e280002c     	add	r0, r0, #44
  504e54: 1afffff8     	bne	0x504e3c <Structs::v2Quest::read(IStreamBase*)+0x3a8> @ imm = #-0x20
  504e58: e594201c     	ldr	r2, [r4, #0x1c]
  504e5c: e5843020     	str	r3, [r4, #0x20]
  504e60: e3520000     	cmp	r2, #0
  504e64: 0a00000d     	beq	0x504ea0 <Structs::v2Quest::read(IStreamBase*)+0x40c> @ imm = #0x34
