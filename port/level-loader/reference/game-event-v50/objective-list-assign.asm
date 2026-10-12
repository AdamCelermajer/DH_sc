
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0047aa48 <ObjectiveList::AssignPyData(Structs::v2QuestObjectiveStub*, int)>:
  47aa48: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  47aa4c: e3520000     	cmp	r2, #0
  47aa50: e1a04000     	mov	r4, r0
  47aa54: e5841008     	str	r1, [r4, #0x8]
  47aa58: e1a05001     	mov	r5, r1
  47aa5c: e5802000     	str	r2, [r0]
  47aa60: da000012     	ble	0x47aab0 <ObjectiveList::AssignPyData(Structs::v2QuestObjectiveStub*, int)+0x68> @ imm = #0x48
  47aa64: e1a00102     	lsl	r0, r2, #2
  47aa68: e3a01000     	mov	r1, #0
  47aa6c: ebfa56be     	bl	0x31056c <operator new[](unsigned int, MemoryHintState)> @ imm = #-0x16a508
  47aa70: e5943000     	ldr	r3, [r4]
  47aa74: e1a07000     	mov	r7, r0
  47aa78: e5840004     	str	r0, [r4, #0x4]
  47aa7c: e3530000     	cmp	r3, #0
  47aa80: da00000a     	ble	0x47aab0 <ObjectiveList::AssignPyData(Structs::v2QuestObjectiveStub*, int)+0x68> @ imm = #0x28
  47aa84: e3a06000     	mov	r6, #0
  47aa88: ea000000     	b	0x47aa90 <ObjectiveList::AssignPyData(Structs::v2QuestObjectiveStub*, int)+0x48> @ imm = #0x0
  47aa8c: e5947004     	ldr	r7, [r4, #0x4]
  47aa90: e1a00005     	mov	r0, r5
  47aa94: ebfffe53     	bl	0x47a3e8 <ObjectiveList::CreateObjectiveWithPyData(Structs::v2QuestObjectiveStub*)> @ imm = #-0x6b4
  47aa98: e7870106     	str	r0, [r7, r6, lsl #2]
  47aa9c: e5943000     	ldr	r3, [r4]
  47aaa0: e2866001     	add	r6, r6, #1
  47aaa4: e285502c     	add	r5, r5, #44
  47aaa8: e1530006     	cmp	r3, r6
  47aaac: cafffff6     	bgt	0x47aa8c <ObjectiveList::AssignPyData(Structs::v2QuestObjectiveStub*, int)+0x44> @ imm = #-0x28
  47aab0: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}

0047aab4 <Objective_EventReceiver::DBG_TraceDetailedObjectiveInformation(__sFILE*) const>:
  47aab4: e92d4070     	push	{r4, r5, r6, lr}
  47aab8: e5d03008     	ldrb	r3, [r0, #0x8]
  47aabc: e1a05000     	mov	r5, r0
  47aac0: e1a04001     	mov	r4, r1
  47aac4: e3530000     	cmp	r3, #0
  47aac8: 1a00000f     	bne	0x47ab0c <Objective_EventReceiver::DBG_TraceDetailedObjectiveInformation(__sFILE*) const+0x58> @ imm = #0x3c
  47aacc: e59f2050     	ldr	r2, [pc, #0x50]         @ 0x47ab24 <Objective_EventReceiver::DBG_TraceDetailedObjectiveInformation(__sFILE*) const+0x70>
  47aad0: e08f2002     	add	r2, pc, r2
  47aad4: e59f104c     	ldr	r1, [pc, #0x4c]         @ 0x47ab28 <Objective_EventReceiver::DBG_TraceDetailedObjectiveInformation(__sFILE*) const+0x74>
  47aad8: e1a00004     	mov	r0, r4
  47aadc: e08f1001     	add	r1, pc, r1
  47aae0: ebfa4d47     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x16cae4
  47aae4: e5d5301c     	ldrb	r3, [r5, #0x1c]
  47aae8: e3530000     	cmp	r3, #0
  47aaec: 1a000009     	bne	0x47ab18 <Objective_EventReceiver::DBG_TraceDetailedObjectiveInformation(__sFILE*) const+0x64> @ imm = #0x24
  47aaf0: e59f2034     	ldr	r2, [pc, #0x34]         @ 0x47ab2c <Objective_EventReceiver::DBG_TraceDetailedObjectiveInformation(__sFILE*) const+0x78>
  47aaf4: e08f2002     	add	r2, pc, r2
  47aaf8: e59f1030     	ldr	r1, [pc, #0x30]         @ 0x47ab30 <Objective_EventReceiver::DBG_TraceDetailedObjectiveInformation(__sFILE*) const+0x7c>
  47aafc: e1a00004     	mov	r0, r4
  47ab00: e08f1001     	add	r1, pc, r1
  47ab04: e8bd4070     	pop	{r4, r5, r6, lr}
  47ab08: eafa4d3d     	b	0x30e004 <.plt+0x290>   @ imm = #-0x16cb0c
  47ab0c: e59f2020     	ldr	r2, [pc, #0x20]         @ 0x47ab34 <Objective_EventReceiver::DBG_TraceDetailedObjectiveInformation(__sFILE*) const+0x80>
  47ab10: e08f2002     	add	r2, pc, r2
  47ab14: eaffffee     	b	0x47aad4 <Objective_EventReceiver::DBG_TraceDetailedObjectiveInformation(__sFILE*) const+0x20> @ imm = #-0x48
  47ab18: e59f2018     	ldr	r2, [pc, #0x18]         @ 0x47ab38 <Objective_EventReceiver::DBG_TraceDetailedObjectiveInformation(__sFILE*) const+0x84>
  47ab1c: e08f2002     	add	r2, pc, r2
  47ab20: eafffff4     	b	0x47aaf8 <Objective_EventReceiver::DBG_TraceDetailedObjectiveInformation(__sFILE*) const+0x44> @ imm = #-0x30
  47ab24: 98 3a 44 00  	.word	0x00443a98
  47ab28: 84 30 45 00  	.word	0x00453084
  47ab2c: 74 3a 44 00  	.word	0x00443a74
  47ab30: b8 30 45 00  	.word	0x004530b8
  47ab34: e0 3d 44 00  	.word	0x00443de0
  47ab38: d4 3d 44 00  	.word	0x00443dd4

0047ab3c <Objective::_loadData(IStreamBase*)>:
  47ab3c: e92d4010     	push	{r4, lr}
  47ab40: e1a04000     	mov	r4, r0
  47ab44: e1a00001     	mov	r0, r1
  47ab48: ebfebc33     	bl	0x429c1c <bool StreamReader::readAs<bool>(IStreamBase*)> @ imm = #-0x50f34
  47ab4c: e5c40014     	strb	r0, [r4, #0x14]
  47ab50: e8bd8010     	pop	{r4, pc}

0047ab54 <Objective_SavedQty::_loadData(IStreamBase*)>:
  47ab54: e92d4070     	push	{r4, r5, r6, lr}
  47ab58: e1a05001     	mov	r5, r1
  47ab5c: e1a04000     	mov	r4, r0
  47ab60: ebfffff5     	bl	0x47ab3c <Objective::_loadData(IStreamBase*)> @ imm = #-0x2c
  47ab64: e1a00005     	mov	r0, r5
  47ab68: ebfaee5f     	bl	0x3364ec <int StreamReader::readAs<int>(IStreamBase*)> @ imm = #-0x144684
  47ab6c: e5840020     	str	r0, [r4, #0x20]
  47ab70: e8bd8070     	pop	{r4, r5, r6, pc}

0047ab74 <Objective::_saveData(IStreamBase*)>:
  47ab74: e5d03014     	ldrb	r3, [r0, #0x14]
  47ab78: e1a00001     	mov	r0, r1
  47ab7c: e1a01003     	mov	r1, r3
  47ab80: eafebc7f     	b	0x429d84 <void StreamReader::writeAs<bool>(IStreamBase*, bool)> @ imm = #-0x50e04

0047ab84 <Objective_SavedQty::_saveData(IStreamBase*)>:
  47ab84: e92d4070     	push	{r4, r5, r6, lr}
  47ab88: e1a04000     	mov	r4, r0
  47ab8c: e1a05001     	mov	r5, r1
  47ab90: ebfffff7     	bl	0x47ab74 <Objective::_saveData(IStreamBase*)> @ imm = #-0x24
  47ab94: e5941020     	ldr	r1, [r4, #0x20]
  47ab98: e1a00005     	mov	r0, r5
  47ab9c: e8bd4070     	pop	{r4, r5, r6, lr}
  47aba0: eafaeead     	b	0x33665c <void StreamReader::writeAs<int>(IStreamBase*, int)> @ imm = #-0x14454c

0047aba4 <Objective_GatherLoot::InitWithCurrentQty()>:
  47aba4: e92d4030     	push	{r4, r5, lr}
  47aba8: e1a04000     	mov	r4, r0
  47abac: e5900010     	ldr	r0, [r0, #0x10]
