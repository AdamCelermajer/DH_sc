
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00340f50 <ObjectBase* GetNewInstance<QuestMoveInZone>()>:
  340f50: e92d4010     	push	{r4, lr}
  340f54: e3a01000     	mov	r1, #0
  340f58: e3a00fe2     	mov	r0, #904
  340f5c: ebff3d83     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x309f4
  340f60: e3a01014     	mov	r1, #20
  340f64: e1a04000     	mov	r4, r0
  340f68: eb0154f0     	bl	0x396330 <QuestMoveInZone::QuestMoveInZone(ObjectBase::GO_IDS)> @ imm = #0x553c0
  340f6c: e1a00004     	mov	r0, r4
  340f70: e8bd8010     	pop	{r4, pc}


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00396330 <QuestMoveInZone::QuestMoveInZone(ObjectBase::GO_IDS)>:
  396330: e92d4070     	push	{r4, r5, r6, lr}
  396334: e3a02001     	mov	r2, #1
  396338: e3a03000     	mov	r3, #0
  39633c: e59f502c     	ldr	r5, [pc, #0x2c]         @ 0x396370 <QuestMoveInZone::QuestMoveInZone(ObjectBase::GO_IDS)+0x40>
  396340: e1a04000     	mov	r4, r0
  396344: eb000655     	bl	0x397ca0 <Zone::Zone(ObjectBase::GO_IDS, bool, bool)> @ imm = #0x1954
  396348: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x396374 <QuestMoveInZone::QuestMoveInZone(ObjectBase::GO_IDS)+0x44>
  39634c: e08f5005     	add	r5, pc, r5
  396350: e1a00004     	mov	r0, r4
  396354: e7953003     	ldr	r3, [r5, r3]
  396358: e28320f4     	add	r2, r3, #244
  39635c: e2831008     	add	r1, r3, #8
  396360: e28330e8     	add	r3, r3, #232
  396364: e884000a     	stm	r4, {r1, r3}
  396368: e5842024     	str	r2, [r4, #0x24]
  39636c: e8bd8070     	pop	{r4, r5, r6, pc}
  396370: 44 e7 5f 00  	.word	0x005fe744
  396374: 74 2e 00 00  	.word	0x00002e74


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00396378 <QuestMoveInZone::QuestMoveInZone(ObjectBase::GO_IDS)>:
  396378: e92d4070     	push	{r4, r5, r6, lr}
  39637c: e3a02001     	mov	r2, #1
  396380: e3a03000     	mov	r3, #0
  396384: e59f502c     	ldr	r5, [pc, #0x2c]         @ 0x3963b8 <QuestMoveInZone::QuestMoveInZone(ObjectBase::GO_IDS)+0x40>
  396388: e1a04000     	mov	r4, r0
  39638c: eb000643     	bl	0x397ca0 <Zone::Zone(ObjectBase::GO_IDS, bool, bool)> @ imm = #0x190c
  396390: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x3963bc <QuestMoveInZone::QuestMoveInZone(ObjectBase::GO_IDS)+0x44>
  396394: e08f5005     	add	r5, pc, r5
  396398: e1a00004     	mov	r0, r4
  39639c: e7953003     	ldr	r3, [r5, r3]
  3963a0: e28320f4     	add	r2, r3, #244
  3963a4: e2831008     	add	r1, r3, #8
  3963a8: e28330e8     	add	r3, r3, #232
  3963ac: e884000a     	stm	r4, {r1, r3}
  3963b0: e5842024     	str	r2, [r4, #0x24]
  3963b4: e8bd8070     	pop	{r4, r5, r6, pc}
  3963b8: fc e6 5f 00  	.word	0x005fe6fc
  3963bc: 74 2e 00 00  	.word	0x00002e74
