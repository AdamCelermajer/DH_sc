
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004ebc38 <Structs::v2Event::read(IStreamBase*)>:
  4ebc38: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  4ebc3c: e1a04000     	mov	r4, r0
  4ebc40: e24dd008     	sub	sp, sp, #8
  4ebc44: e1a00001     	mov	r0, r1
  4ebc48: e1a05001     	mov	r5, r1
  4ebc4c: e59f6254     	ldr	r6, [pc, #0x254]        @ 0x4ebea8 <Structs::v2Event::read(IStreamBase*)+0x270>
  4ebc50: e2841004     	add	r1, r4, #4
  4ebc54: ebfdb50d     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0x92bcc
  4ebc58: e3a03001     	mov	r3, #1
  4ebc5c: e3530000     	cmp	r3, #0
  4ebc60: e58d3004     	str	r3, [sp, #0x4]
  4ebc64: e08f6006     	add	r6, pc, r6
  4ebc68: 1a00000f     	bne	0x4ebcac <Structs::v2Event::read(IStreamBase*)+0x74> @ imm = #0x3c
  4ebc6c: e2843005     	add	r3, r4, #5
  4ebc70: e2842006     	add	r2, r4, #6
  4ebc74: e5d20001     	ldrb	r0, [r2, #0x1]
  4ebc78: e5531001     	ldrb	r1, [r3, #-0x1]
  4ebc7c: e1520003     	cmp	r2, r3
  4ebc80: e0201001     	eor	r1, r0, r1
  4ebc84: e5431001     	strb	r1, [r3, #-0x1]
  4ebc88: e5d20001     	ldrb	r0, [r2, #0x1]
  4ebc8c: e0211000     	eor	r1, r1, r0
  4ebc90: e5c21001     	strb	r1, [r2, #0x1]
  4ebc94: e5530001     	ldrb	r0, [r3, #-0x1]
  4ebc98: e2422001     	sub	r2, r2, #1
  4ebc9c: e0211000     	eor	r1, r1, r0
  4ebca0: e5431001     	strb	r1, [r3, #-0x1]
  4ebca4: e2833001     	add	r3, r3, #1
  4ebca8: 8afffff1     	bhi	0x4ebc74 <Structs::v2Event::read(IStreamBase*)+0x3c> @ imm = #-0x3c
  4ebcac: e1a00005     	mov	r0, r5
  4ebcb0: e2841008     	add	r1, r4, #8
  4ebcb4: ebfbcd39     	bl	0x3df1a0 <void StreamReader::readAs<unsigned int>(IStreamBase*, unsigned int*)> @ imm = #-0x10cb1c
  4ebcb8: e3a03001     	mov	r3, #1
  4ebcbc: e3530000     	cmp	r3, #0
  4ebcc0: e58d3004     	str	r3, [sp, #0x4]
  4ebcc4: 1a00000f     	bne	0x4ebd08 <Structs::v2Event::read(IStreamBase*)+0xd0> @ imm = #0x3c
  4ebcc8: e2843009     	add	r3, r4, #9
  4ebccc: e284200a     	add	r2, r4, #10
  4ebcd0: e5d20001     	ldrb	r0, [r2, #0x1]
  4ebcd4: e5531001     	ldrb	r1, [r3, #-0x1]
  4ebcd8: e1520003     	cmp	r2, r3
  4ebcdc: e0201001     	eor	r1, r0, r1
  4ebce0: e5431001     	strb	r1, [r3, #-0x1]
  4ebce4: e5d20001     	ldrb	r0, [r2, #0x1]
  4ebce8: e0211000     	eor	r1, r1, r0
  4ebcec: e5c21001     	strb	r1, [r2, #0x1]
  4ebcf0: e5530001     	ldrb	r0, [r3, #-0x1]
  4ebcf4: e2422001     	sub	r2, r2, #1
  4ebcf8: e0211000     	eor	r1, r1, r0
  4ebcfc: e5431001     	strb	r1, [r3, #-0x1]
  4ebd00: e2833001     	add	r3, r3, #1
  4ebd04: 8afffff1     	bhi	0x4ebcd0 <Structs::v2Event::read(IStreamBase*)+0x98> @ imm = #-0x3c
  4ebd08: e594000c     	ldr	r0, [r4, #0xc]
  4ebd0c: e3500000     	cmp	r0, #0
  4ebd10: 0a000000     	beq	0x4ebd18 <Structs::v2Event::read(IStreamBase*)+0xe0> @ imm = #0x0
  4ebd14: ebf891c9     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1db8dc
  4ebd18: e5940008     	ldr	r0, [r4, #0x8]
  4ebd1c: e3a01001     	mov	r1, #1
  4ebd20: e3a07000     	mov	r7, #0
  4ebd24: e0800001     	add	r0, r0, r1
  4ebd28: ebf8920f     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x1db7c4
  4ebd2c: e5942008     	ldr	r2, [r4, #0x8]
  4ebd30: e1a01000     	mov	r1, r0
  4ebd34: e584000c     	str	r0, [r4, #0xc]
  4ebd38: e1a03007     	mov	r3, r7
  4ebd3c: e1a00005     	mov	r0, r5
  4ebd40: ebf8adc3     	bl	0x317454 <StreamReader::readStringEx(IStreamBase*, char*, unsigned long long)> @ imm = #-0x1d48f4
  4ebd44: e5943008     	ldr	r3, [r4, #0x8]
  4ebd48: e594200c     	ldr	r2, [r4, #0xc]
  4ebd4c: e1a00005     	mov	r0, r5
  4ebd50: e2841010     	add	r1, r4, #16
  4ebd54: e7c27003     	strb	r7, [r2, r3]
  4ebd58: ebfbcd10     	bl	0x3df1a0 <void StreamReader::readAs<unsigned int>(IStreamBase*, unsigned int*)> @ imm = #-0x10cbc0
  4ebd5c: e3a03001     	mov	r3, #1
  4ebd60: e1530007     	cmp	r3, r7
  4ebd64: e58d3004     	str	r3, [sp, #0x4]
  4ebd68: 1a00000f     	bne	0x4ebdac <Structs::v2Event::read(IStreamBase*)+0x174> @ imm = #0x3c
  4ebd6c: e2843011     	add	r3, r4, #17
  4ebd70: e2842012     	add	r2, r4, #18
  4ebd74: e5d20001     	ldrb	r0, [r2, #0x1]
  4ebd78: e5531001     	ldrb	r1, [r3, #-0x1]
  4ebd7c: e1520003     	cmp	r2, r3
  4ebd80: e0201001     	eor	r1, r0, r1
  4ebd84: e5431001     	strb	r1, [r3, #-0x1]
  4ebd88: e5d20001     	ldrb	r0, [r2, #0x1]
  4ebd8c: e0211000     	eor	r1, r1, r0
  4ebd90: e5c21001     	strb	r1, [r2, #0x1]
  4ebd94: e5530001     	ldrb	r0, [r3, #-0x1]
  4ebd98: e2422001     	sub	r2, r2, #1
  4ebd9c: e0211000     	eor	r1, r1, r0
  4ebda0: e5431001     	strb	r1, [r3, #-0x1]
  4ebda4: e2833001     	add	r3, r3, #1
  4ebda8: 8afffff1     	bhi	0x4ebd74 <Structs::v2Event::read(IStreamBase*)+0x13c> @ imm = #-0x3c
  4ebdac: e5943014     	ldr	r3, [r4, #0x14]
  4ebdb0: e3530000     	cmp	r3, #0
  4ebdb4: 0a000010     	beq	0x4ebdfc <Structs::v2Event::read(IStreamBase*)+0x1c4> @ imm = #0x40
  4ebdb8: e5132004     	ldr	r2, [r3, #-0x4]
  4ebdbc: e3a0002c     	mov	r0, #44
  4ebdc0: e0203290     	mla	r0, r0, r2, r3
  4ebdc4: e1530000     	cmp	r3, r0
  4ebdc8: 1a000001     	bne	0x4ebdd4 <Structs::v2Event::read(IStreamBase*)+0x19c> @ imm = #0x4
  4ebdcc: ea000008     	b	0x4ebdf4 <Structs::v2Event::read(IStreamBase*)+0x1bc> @ imm = #0x20
  4ebdd0: e1a00007     	mov	r0, r7
  4ebdd4: e240702c     	sub	r7, r0, #44
  4ebdd8: e510302c     	ldr	r3, [r0, #-0x2c]
  4ebddc: e1a00007     	mov	r0, r7
  4ebde0: e1a0e00f     	mov	lr, pc
  4ebde4: e593f000     	ldr	pc, [r3]
  4ebde8: e5940014     	ldr	r0, [r4, #0x14]
  4ebdec: e1500007     	cmp	r0, r7
  4ebdf0: 1afffff6     	bne	0x4ebdd0 <Structs::v2Event::read(IStreamBase*)+0x198> @ imm = #-0x28
  4ebdf4: e2400008     	sub	r0, r0, #8
  4ebdf8: ebf89190     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1db9c0
  4ebdfc: e5947010     	ldr	r7, [r4, #0x10]
  4ebe00: e3a0802c     	mov	r8, #44
  4ebe04: e3a01001     	mov	r1, #1
  4ebe08: e0000798     	mul	r0, r8, r7
  4ebe0c: e2800008     	add	r0, r0, #8
  4ebe10: ebf891d5     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x1db8ac
  4ebe14: e3570000     	cmp	r7, #0
  4ebe18: e5808000     	str	r8, [r0]
  4ebe1c: e5807004     	str	r7, [r0, #0x4]
  4ebe20: e2803008     	add	r3, r0, #8
  4ebe24: 0a00000b     	beq	0x4ebe58 <Structs::v2Event::read(IStreamBase*)+0x220> @ imm = #0x2c
  4ebe28: e59f107c     	ldr	r1, [pc, #0x7c]         @ 0x4ebeac <Structs::v2Event::read(IStreamBase*)+0x274>
  4ebe2c: e3a02000     	mov	r2, #0
  4ebe30: e796c001     	ldr	r12, [r6, r1]
  4ebe34: e1a01002     	mov	r1, r2
  4ebe38: e28cc008     	add	r12, r12, #8
  4ebe3c: e2822001     	add	r2, r2, #1
  4ebe40: e1520007     	cmp	r2, r7
  4ebe44: e580c008     	str	r12, [r0, #0x8]
  4ebe48: e580101c     	str	r1, [r0, #0x1c]
  4ebe4c: e5801024     	str	r1, [r0, #0x24]
  4ebe50: e280002c     	add	r0, r0, #44
  4ebe54: 1afffff8     	bne	0x4ebe3c <Structs::v2Event::read(IStreamBase*)+0x204> @ imm = #-0x20
  4ebe58: e5942010     	ldr	r2, [r4, #0x10]
  4ebe5c: e5843014     	str	r3, [r4, #0x14]
  4ebe60: e3520000     	cmp	r2, #0
  4ebe64: 0a00000d     	beq	0x4ebea0 <Structs::v2Event::read(IStreamBase*)+0x268> @ imm = #0x34
  4ebe68: e3a06000     	mov	r6, #0
  4ebe6c: e1a07006     	mov	r7, r6
  4ebe70: ea000000     	b	0x4ebe78 <Structs::v2Event::read(IStreamBase*)+0x240> @ imm = #0x0
  4ebe74: e5943014     	ldr	r3, [r4, #0x14]
  4ebe78: e0830006     	add	r0, r3, r6
  4ebe7c: e1a01005     	mov	r1, r5
  4ebe80: e7933006     	ldr	r3, [r3, r6]
  4ebe84: e1a0e00f     	mov	lr, pc
  4ebe88: e593f00c     	ldr	pc, [r3, #0xc]
  4ebe8c: e5943010     	ldr	r3, [r4, #0x10]
  4ebe90: e2877001     	add	r7, r7, #1
  4ebe94: e286602c     	add	r6, r6, #44
  4ebe98: e1530007     	cmp	r3, r7
  4ebe9c: 8afffff4     	bhi	0x4ebe74 <Structs::v2Event::read(IStreamBase*)+0x23c> @ imm = #-0x30
  4ebea0: e28dd008     	add	sp, sp, #8
  4ebea4: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  4ebea8: 2c 8e 4a 00  	.word	0x004a8e2c
  4ebeac: 28 1e 00 00  	.word	0x00001e28

004ebeb0 <Structs::Skill::read(IStreamBase*)>:
  4ebeb0: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  4ebeb4: e1a04000     	mov	r4, r0
