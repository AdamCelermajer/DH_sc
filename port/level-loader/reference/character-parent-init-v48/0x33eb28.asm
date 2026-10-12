
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0033eb28 <ConditionData::Init()>:
  33eb28: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  33eb2c: e5905018     	ldr	r5, [r0, #0x18]
  33eb30: e5903014     	ldr	r3, [r0, #0x14]
  33eb34: e59f80bc     	ldr	r8, [pc, #0xbc]         @ 0x33ebf8 <ConditionData::Init()+0xd0>
  33eb38: e1a0a000     	mov	r10, r0
  33eb3c: e1530005     	cmp	r3, r5
  33eb40: e08f8008     	add	r8, pc, r8
  33eb44: 0a000028     	beq	0x33ebec <ConditionData::Init()+0xc4> @ imm = #0xa0
  33eb48: e59f10ac     	ldr	r1, [pc, #0xac]         @ 0x33ebfc <ConditionData::Init()+0xd4>
  33eb4c: e1a00005     	mov	r0, r5
  33eb50: e08f1001     	add	r1, pc, r1
  33eb54: ebff3df0     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x30840
  33eb58: e3500000     	cmp	r0, #0
  33eb5c: 0a000022     	beq	0x33ebec <ConditionData::Init()+0xc4> @ imm = #0x88
  33eb60: e59f3098     	ldr	r3, [pc, #0x98]         @ 0x33ec00 <ConditionData::Init()+0xd8>
  33eb64: e7983003     	ldr	r3, [r8, r3]
  33eb68: e5936000     	ldr	r6, [r3]
  33eb6c: e3560000     	cmp	r6, #0
  33eb70: 0a00001d     	beq	0x33ebec <ConditionData::Init()+0xc4> @ imm = #0x74
  33eb74: e59f3088     	ldr	r3, [pc, #0x88]         @ 0x33ec04 <ConditionData::Init()+0xdc>
  33eb78: e3a04000     	mov	r4, #0
  33eb7c: e7983003     	ldr	r3, [r8, r3]
  33eb80: e5937000     	ldr	r7, [r3]
  33eb84: ea000002     	b	0x33eb94 <ConditionData::Init()+0x6c> @ imm = #0x8
  33eb88: e2844001     	add	r4, r4, #1
  33eb8c: e1540006     	cmp	r4, r6
  33eb90: 0a000016     	beq	0x33ebf0 <ConditionData::Init()+0xc8> @ imm = #0x58
  33eb94: e7971104     	ldr	r1, [r7, r4, lsl #2]
  33eb98: e1a00005     	mov	r0, r5
  33eb9c: ebff3dde     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x30888
  33eba0: e3500000     	cmp	r0, #0
  33eba4: 1afffff7     	bne	0x33eb88 <ConditionData::Init()+0x60> @ imm = #-0x24
  33eba8: e3740001     	cmn	r4, #1
  33ebac: 0a000010     	beq	0x33ebf4 <ConditionData::Init()+0xcc> @ imm = #0x40
  33ebb0: e1a01000     	mov	r1, r0
  33ebb4: e3a0000c     	mov	r0, #12
  33ebb8: ebff466c     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x2e650
  33ebbc: e1a05000     	mov	r5, r0
  33ebc0: eb04e6ca     	bl	0x4786f0 <ConditionList::ConditionList()> @ imm = #0x139b28
  33ebc4: e59f303c     	ldr	r3, [pc, #0x3c]         @ 0x33ec08 <ConditionData::Init()+0xe0>
  33ebc8: e58a501c     	str	r5, [r10, #0x1c]
  33ebcc: e1a00005     	mov	r0, r5
  33ebd0: e7983003     	ldr	r3, [r8, r3]
  33ebd4: e5933000     	ldr	r3, [r3]
  33ebd8: e0834204     	add	r4, r3, r4, lsl #4
  33ebdc: e5942004     	ldr	r2, [r4, #0x4]
  33ebe0: e5941008     	ldr	r1, [r4, #0x8]
  33ebe4: e8bd47f0     	pop	{r4, r5, r6, r7, r8, r9, r10, lr}
  33ebe8: ea04e749     	b	0x478914 <ConditionList::AssignPyData(Structs::v2ConditionStub*, int)> @ imm = #0x139d24
  33ebec: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  33ebf0: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  33ebf4: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  33ebf8: 50 5f 65 00  	.word	0x00655f50
  33ebfc: 50 bb 59 00  	.word	0x0059bb50
  33ec00: 64 2f 00 00  	.word	0x00002f64
  33ec04: 54 35 00 00  	.word	0x00003554
  33ec08: 40 28 00 00  	.word	0x00002840
