
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0050600c <Structs::v2ConditionStub::read(IStreamBase*)>:
  50600c: e92d4030     	push	{r4, r5, lr}
  506010: e1a04000     	mov	r4, r0
  506014: e24dd00c     	sub	sp, sp, #12
  506018: e1a05001     	mov	r5, r1
  50601c: ebfffec1     	bl	0x505b28 <Structs::v2Condition::read(IStreamBase*)> @ imm = #-0x4fc
  506020: e1a00005     	mov	r0, r5
  506024: e2841008     	add	r1, r4, #8
  506028: ebfd4c18     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0xacfa0
  50602c: e3a03001     	mov	r3, #1
  506030: e3530000     	cmp	r3, #0
  506034: e58d3004     	str	r3, [sp, #0x4]
  506038: 1a00000f     	bne	0x50607c <Structs::v2ConditionStub::read(IStreamBase*)+0x70> @ imm = #0x3c
  50603c: e2843009     	add	r3, r4, #9
  506040: e284200a     	add	r2, r4, #10
  506044: e5d20001     	ldrb	r0, [r2, #0x1]
  506048: e5531001     	ldrb	r1, [r3, #-0x1]
  50604c: e1530002     	cmp	r3, r2
  506050: e0201001     	eor	r1, r0, r1
  506054: e5431001     	strb	r1, [r3, #-0x1]
  506058: e5d20001     	ldrb	r0, [r2, #0x1]
  50605c: e0211000     	eor	r1, r1, r0
  506060: e5c21001     	strb	r1, [r2, #0x1]
  506064: e5530001     	ldrb	r0, [r3, #-0x1]
  506068: e2422001     	sub	r2, r2, #1
  50606c: e0211000     	eor	r1, r1, r0
  506070: e5431001     	strb	r1, [r3, #-0x1]
  506074: e2833001     	add	r3, r3, #1
  506078: 3afffff1     	blo	0x506044 <Structs::v2ConditionStub::read(IStreamBase*)+0x38> @ imm = #-0x3c
  50607c: e1a00005     	mov	r0, r5
  506080: e284100c     	add	r1, r4, #12
  506084: ebfd4c01     	bl	0x459090 <void StreamReader::readAs<int>(IStreamBase*, int*)> @ imm = #-0xacffc
  506088: e3a03001     	mov	r3, #1
  50608c: e3530000     	cmp	r3, #0
  506090: e58d3004     	str	r3, [sp, #0x4]
  506094: 1a00000f     	bne	0x5060d8 <Structs::v2ConditionStub::read(IStreamBase*)+0xcc> @ imm = #0x3c
  506098: e284300e     	add	r3, r4, #14
  50609c: e284400d     	add	r4, r4, #13
  5060a0: e5d31001     	ldrb	r1, [r3, #0x1]
  5060a4: e5542001     	ldrb	r2, [r4, #-0x1]
  5060a8: e1530004     	cmp	r3, r4
  5060ac: e0212002     	eor	r2, r1, r2
  5060b0: e5442001     	strb	r2, [r4, #-0x1]
  5060b4: e5d31001     	ldrb	r1, [r3, #0x1]
  5060b8: e0222001     	eor	r2, r2, r1
  5060bc: e5c32001     	strb	r2, [r3, #0x1]
  5060c0: e5541001     	ldrb	r1, [r4, #-0x1]
  5060c4: e2433001     	sub	r3, r3, #1
  5060c8: e0222001     	eor	r2, r2, r1
  5060cc: e5442001     	strb	r2, [r4, #-0x1]
  5060d0: e2844001     	add	r4, r4, #1
  5060d4: 8afffff1     	bhi	0x5060a0 <Structs::v2ConditionStub::read(IStreamBase*)+0x94> @ imm = #-0x3c
  5060d8: e28dd00c     	add	sp, sp, #12
  5060dc: e8bd8030     	pop	{r4, r5, pc}
