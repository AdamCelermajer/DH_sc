
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0047a3d4 <ObjectiveList::ObjectiveList()>:
  47a3d4: e3a02000     	mov	r2, #0
  47a3d8: e5802008     	str	r2, [r0, #0x8]
  47a3dc: e5802000     	str	r2, [r0]
  47a3e0: e5802004     	str	r2, [r0, #0x4]
  47a3e4: e12fff1e     	bx	lr

0047a3e8 <ObjectiveList::CreateObjectiveWithPyData(Structs::v2QuestObjectiveStub*)>:
  47a3e8: e92d4070     	push	{r4, r5, r6, lr}
  47a3ec: e59f301c     	ldr	r3, [pc, #0x1c]         @ 0x47a410 <ObjectiveList::CreateObjectiveWithPyData(Structs::v2QuestObjectiveStub*)+0x28>
  47a3f0: e5904004     	ldr	r4, [r0, #0x4]
  47a3f4: e1a05000     	mov	r5, r0
  47a3f8: e08f3003     	add	r3, pc, r3
  47a3fc: e1a0e00f     	mov	lr, pc
  47a400: e793f104     	ldr	pc, [r3, r4, lsl #2]
  47a404: e580500c     	str	r5, [r0, #0xc]
  47a408: e5804004     	str	r4, [r0, #0x4]
  47a40c: e8bd8070     	pop	{r4, r5, r6, pc}
  47a410: 08 f5 4e 00  	.word	0x004ef508

0047a414 <ObjectiveList::SetOwner(Character*)>:
  47a414: e5903000     	ldr	r3, [r0]
  47a418: e3530000     	cmp	r3, #0
  47a41c: d12fff1e     	bxle	lr
  47a420: e3a03000     	mov	r3, #0
  47a424: e5902004     	ldr	r2, [r0, #0x4]
  47a428: e7922103     	ldr	r2, [r2, r3, lsl #2]
  47a42c: e2833001     	add	r3, r3, #1
  47a430: e5821010     	str	r1, [r2, #0x10]
  47a434: e5902000     	ldr	r2, [r0]
  47a438: e1520003     	cmp	r2, r3
  47a43c: cafffff8     	bgt	0x47a424 <ObjectiveList::SetOwner(Character*)+0x10> @ imm = #-0x20
  47a440: e12fff1e     	bx	lr

0047a444 <ObjectiveList::Eval()>:
  47a444: e5901000     	ldr	r1, [r0]
  47a448: e3510000     	cmp	r1, #0
  47a44c: da00000d     	ble	0x47a488 <ObjectiveList::Eval()+0x44> @ imm = #0x34
  47a450: e5900004     	ldr	r0, [r0, #0x4]
  47a454: e5903000     	ldr	r3, [r0]
  47a458: e5d33014     	ldrb	r3, [r3, #0x14]
  47a45c: e3530000     	cmp	r3, #0
  47a460: 13a03000     	movne	r3, #0
  47a464: 1a000004     	bne	0x47a47c <ObjectiveList::Eval()+0x38> @ imm = #0x10
  47a468: ea000008     	b	0x47a490 <ObjectiveList::Eval()+0x4c> @ imm = #0x20
  47a46c: e7902103     	ldr	r2, [r0, r3, lsl #2]
  47a470: e5d22014     	ldrb	r2, [r2, #0x14]
