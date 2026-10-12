
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004ffc24 <Structs::ExecScript::read(IStreamBase*)>:
  4ffc24: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  4ffc28: e1a05000     	mov	r5, r0
  4ffc2c: e24dd008     	sub	sp, sp, #8
  4ffc30: e1a08001     	mov	r8, r1
  4ffc34: ebfffefb     	bl	0x4ff828 <Structs::ScriptCmd::read(IStreamBase*)> @ imm = #-0x414
  4ffc38: e1a00008     	mov	r0, r8
  4ffc3c: e2851008     	add	r1, r5, #8
  4ffc40: ebff6f15     	bl	0x4db89c <void StreamReader::readAs<bool>(IStreamBase*, bool*)> @ imm = #-0x243ac
  4ffc44: e1a00008     	mov	r0, r8
  4ffc48: e285100c     	add	r1, r5, #12
  4ffc4c: ebfd650f     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0xa6bc4
  4ffc50: e3a03001     	mov	r3, #1
  4ffc54: e3530000     	cmp	r3, #0
  4ffc58: e58d3004     	str	r3, [sp, #0x4]
  4ffc5c: 1a00000f     	bne	0x4ffca0 <Structs::ExecScript::read(IStreamBase*)+0x7c> @ imm = #0x3c
  4ffc60: e285300d     	add	r3, r5, #13
  4ffc64: e285200e     	add	r2, r5, #14
  4ffc68: e5d20001     	ldrb	r0, [r2, #0x1]
  4ffc6c: e5531001     	ldrb	r1, [r3, #-0x1]
  4ffc70: e1530002     	cmp	r3, r2
  4ffc74: e0201001     	eor	r1, r0, r1
  4ffc78: e5431001     	strb	r1, [r3, #-0x1]
  4ffc7c: e5d20001     	ldrb	r0, [r2, #0x1]
  4ffc80: e0211000     	eor	r1, r1, r0
  4ffc84: e5c21001     	strb	r1, [r2, #0x1]
  4ffc88: e5530001     	ldrb	r0, [r3, #-0x1]
  4ffc8c: e2422001     	sub	r2, r2, #1
  4ffc90: e0211000     	eor	r1, r1, r0
  4ffc94: e5431001     	strb	r1, [r3, #-0x1]
  4ffc98: e2833001     	add	r3, r3, #1
  4ffc9c: 3afffff1     	blo	0x4ffc68 <Structs::ExecScript::read(IStreamBase*)+0x44> @ imm = #-0x3c
  4ffca0: e1a00008     	mov	r0, r8
  4ffca4: e2851010     	add	r1, r5, #16
  4ffca8: ebfb7d3c     	bl	0x3df1a0 <void StreamReader::readAs<unsigned int>(IStreamBase*, unsigned int*)> @ imm = #-0x120b10
  4ffcac: e3a03001     	mov	r3, #1
  4ffcb0: e3530000     	cmp	r3, #0
  4ffcb4: e58d3004     	str	r3, [sp, #0x4]
  4ffcb8: 1a00000f     	bne	0x4ffcfc <Structs::ExecScript::read(IStreamBase*)+0xd8> @ imm = #0x3c
  4ffcbc: e2853011     	add	r3, r5, #17
  4ffcc0: e2852012     	add	r2, r5, #18
  4ffcc4: e5d20001     	ldrb	r0, [r2, #0x1]
  4ffcc8: e5531001     	ldrb	r1, [r3, #-0x1]
  4ffccc: e1520003     	cmp	r2, r3
  4ffcd0: e0201001     	eor	r1, r0, r1
  4ffcd4: e5431001     	strb	r1, [r3, #-0x1]
  4ffcd8: e5d20001     	ldrb	r0, [r2, #0x1]
  4ffcdc: e0211000     	eor	r1, r1, r0
  4ffce0: e5c21001     	strb	r1, [r2, #0x1]
  4ffce4: e5530001     	ldrb	r0, [r3, #-0x1]
  4ffce8: e2422001     	sub	r2, r2, #1
  4ffcec: e0211000     	eor	r1, r1, r0
  4ffcf0: e5431001     	strb	r1, [r3, #-0x1]
  4ffcf4: e2833001     	add	r3, r3, #1
  4ffcf8: 8afffff1     	bhi	0x4ffcc4 <Structs::ExecScript::read(IStreamBase*)+0xa0> @ imm = #-0x3c
  4ffcfc: e5950014     	ldr	r0, [r5, #0x14]
  4ffd00: e3500000     	cmp	r0, #0
  4ffd04: 0a000000     	beq	0x4ffd0c <Structs::ExecScript::read(IStreamBase*)+0xe8> @ imm = #0x0
  4ffd08: ebf841cc     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1ef8d0
  4ffd0c: e5950010     	ldr	r0, [r5, #0x10]
  4ffd10: e3a01001     	mov	r1, #1
  4ffd14: e1a00100     	lsl	r0, r0, #2
  4ffd18: ebf84213     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x1ef7b4
  4ffd1c: e5953010     	ldr	r3, [r5, #0x10]
  4ffd20: e5850014     	str	r0, [r5, #0x14]
  4ffd24: e3530000     	cmp	r3, #0
  4ffd28: 0a00001f     	beq	0x4ffdac <Structs::ExecScript::read(IStreamBase*)+0x188> @ imm = #0x7c
  4ffd2c: e3a04000     	mov	r4, #0
  4ffd30: e3a07001     	mov	r7, #1
  4ffd34: e1a06104     	lsl	r6, r4, #2
  4ffd38: e0801006     	add	r1, r0, r6
  4ffd3c: e1a00008     	mov	r0, r8
  4ffd40: ebfd64d2     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0xa6cb8
  4ffd44: e58d7004     	str	r7, [sp, #0x4]
  4ffd48: e3570000     	cmp	r7, #0
  4ffd4c: e5953014     	ldr	r3, [r5, #0x14]
  4ffd50: 1a000010     	bne	0x4ffd98 <Structs::ExecScript::read(IStreamBase*)+0x174> @ imm = #0x40
  4ffd54: e0836006     	add	r6, r3, r6
  4ffd58: e2863002     	add	r3, r6, #2
  4ffd5c: e2866001     	add	r6, r6, #1
  4ffd60: e5d31001     	ldrb	r1, [r3, #0x1]
  4ffd64: e5562001     	ldrb	r2, [r6, #-0x1]
  4ffd68: e1560003     	cmp	r6, r3
  4ffd6c: e0212002     	eor	r2, r1, r2
  4ffd70: e5462001     	strb	r2, [r6, #-0x1]
  4ffd74: e5d31001     	ldrb	r1, [r3, #0x1]
  4ffd78: e0222001     	eor	r2, r2, r1
  4ffd7c: e5c32001     	strb	r2, [r3, #0x1]
  4ffd80: e5561001     	ldrb	r1, [r6, #-0x1]
  4ffd84: e2433001     	sub	r3, r3, #1
  4ffd88: e0222001     	eor	r2, r2, r1
  4ffd8c: e5462001     	strb	r2, [r6, #-0x1]
  4ffd90: e2866001     	add	r6, r6, #1
  4ffd94: 3afffff1     	blo	0x4ffd60 <Structs::ExecScript::read(IStreamBase*)+0x13c> @ imm = #-0x3c
  4ffd98: e5953010     	ldr	r3, [r5, #0x10]
  4ffd9c: e2844001     	add	r4, r4, #1
  4ffda0: e1530004     	cmp	r3, r4
  4ffda4: 85950014     	ldrhi	r0, [r5, #0x14]
  4ffda8: 8affffe1     	bhi	0x4ffd34 <Structs::ExecScript::read(IStreamBase*)+0x110> @ imm = #-0x7c
  4ffdac: e1a00008     	mov	r0, r8
  4ffdb0: e2851018     	add	r1, r5, #24
  4ffdb4: ebff6eb8     	bl	0x4db89c <void StreamReader::readAs<bool>(IStreamBase*, bool*)> @ imm = #-0x24520
  4ffdb8: e28dd008     	add	sp, sp, #8
  4ffdbc: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}

004ffdc0 <Structs::WaitCamera::read(IStreamBase*)>:
  4ffdc0: e92d4030     	push	{r4, r5, lr}
  4ffdc4: e1a04000     	mov	r4, r0
  4ffdc8: e24dd00c     	sub	sp, sp, #12
  4ffdcc: e1a05001     	mov	r5, r1
  4ffdd0: ebfffe94     	bl	0x4ff828 <Structs::ScriptCmd::read(IStreamBase*)> @ imm = #-0x5b0
  4ffdd4: e1a00005     	mov	r0, r5
  4ffdd8: e2841008     	add	r1, r4, #8
  4ffddc: ebff6eda     	bl	0x4db94c <void StreamReader::readAs<float>(IStreamBase*, float*)> @ imm = #-0x24498
  4ffde0: e3a03001     	mov	r3, #1
  4ffde4: e3530000     	cmp	r3, #0
  4ffde8: e58d3004     	str	r3, [sp, #0x4]
  4ffdec: 1a00000f     	bne	0x4ffe30 <Structs::WaitCamera::read(IStreamBase*)+0x70> @ imm = #0x3c
  4ffdf0: e284300a     	add	r3, r4, #10
  4ffdf4: e2844009     	add	r4, r4, #9
  4ffdf8: e5d31001     	ldrb	r1, [r3, #0x1]
  4ffdfc: e5542001     	ldrb	r2, [r4, #-0x1]
  4ffe00: e1540003     	cmp	r4, r3
  4ffe04: e0212002     	eor	r2, r1, r2
  4ffe08: e5442001     	strb	r2, [r4, #-0x1]
  4ffe0c: e5d31001     	ldrb	r1, [r3, #0x1]
  4ffe10: e0222001     	eor	r2, r2, r1
  4ffe14: e5c32001     	strb	r2, [r3, #0x1]
  4ffe18: e5541001     	ldrb	r1, [r4, #-0x1]
  4ffe1c: e2433001     	sub	r3, r3, #1
  4ffe20: e0222001     	eor	r2, r2, r1
  4ffe24: e5442001     	strb	r2, [r4, #-0x1]
  4ffe28: e2844001     	add	r4, r4, #1
  4ffe2c: 3afffff1     	blo	0x4ffdf8 <Structs::WaitCamera::read(IStreamBase*)+0x38> @ imm = #-0x3c
  4ffe30: e28dd00c     	add	sp, sp, #12
  4ffe34: e8bd8030     	pop	{r4, r5, pc}

004ffe38 <Structs::DoTutorial::read(IStreamBase*)>:
  4ffe38: e92d4070     	push	{r4, r5, r6, lr}
  4ffe3c: e1a04000     	mov	r4, r0
