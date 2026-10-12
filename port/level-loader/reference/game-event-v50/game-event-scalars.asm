
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:
  4794f0: bc 46 45 00  	.word	0x004546bc
  4794f4: 80 46 45 00  	.word	0x00454680
  4794f8: 24 5e 44 00  	.word	0x00445e24

004794fc <GameEvent::Compile()>:
  4794fc: e92d4070     	push	{r4, r5, r6, lr}
  479500: e280500c     	add	r5, r0, #12
  479504: e1a04000     	mov	r4, r0
  479508: e1a00005     	mov	r0, r5
  47950c: eb000449     	bl	0x47a638 <ObjectiveList::InvalidateCompile()> @ imm = #0x1124
  479510: e1a00005     	mov	r0, r5
  479514: eb000454     	bl	0x47a66c <ObjectiveList::Compile()> @ imm = #0x1150
  479518: e5d43018     	ldrb	r3, [r4, #0x18]
  47951c: e3530000     	cmp	r3, #0
  479520: 1a000003     	bne	0x479534 <GameEvent::Compile()+0x38> @ imm = #0xc
  479524: e1a00005     	mov	r0, r5
  479528: eb000439     	bl	0x47a614 <ObjectiveList::Register()> @ imm = #0x10e4
  47952c: e3a03001     	mov	r3, #1
  479530: e5c43018     	strb	r3, [r4, #0x18]
  479534: e8bd8070     	pop	{r4, r5, r6, pc}

00479538 <GameEvent::ReInit()>:
  479538: e3a03000     	mov	r3, #0
  47953c: e480300c     	str	r3, [r0], #12
  479540: ea000418     	b	0x47a5a8 <ObjectiveList::_resetData()> @ imm = #0x1060

00479544 <GameEvent::DBG_TraceDetailedInformation() const>:
  479544: e59f3014     	ldr	r3, [pc, #0x14]         @ 0x479560 <GameEvent::DBG_TraceDetailedInformation() const+0x1c>
  479548: e59f1014     	ldr	r1, [pc, #0x14]         @ 0x479564 <GameEvent::DBG_TraceDetailedInformation() const+0x20>
  47954c: e280000c     	add	r0, r0, #12
  479550: e08f3003     	add	r3, pc, r3
  479554: e7931001     	ldr	r1, [r3, r1]
  479558: e2811054     	add	r1, r1, #84
  47955c: ea0004b5     	b	0x47a838 <ObjectiveList::DBG_TraceDetailedObjectiveListInformation(__sFILE*) const> @ imm = #0x12d4
  479560: 40 b5 51 00  	.word	0x0051b540
  479564: c0 19 00 00  	.word	0x000019c0

00479568 <GameEvent::AssignPyData(Structs::v2Event*)>:
  479568: e580101c     	str	r1, [r0, #0x1c]
  47956c: e5912010     	ldr	r2, [r1, #0x10]
  479570: e5911014     	ldr	r1, [r1, #0x14]
  479574: e280000c     	add	r0, r0, #12
  479578: ea000532     	b	0x47aa48 <ObjectiveList::AssignPyData(Structs::v2QuestObjectiveStub*, int)> @ imm = #0x14c8

0047957c <GameEvent::~GameEvent()>:
  47957c: e92d4010     	push	{r4, lr}
  479580: e1a04000     	mov	r4, r0
  479584: e280000c     	add	r0, r0, #12
  479588: eb0004cf     	bl	0x47a8cc <ObjectiveList::~ObjectiveList()> @ imm = #0x133c
  47958c: e1a00004     	mov	r0, r4
  479590: e8bd8010     	pop	{r4, pc}

00479594 <GameEvent::~GameEvent()>:
  479594: e92d4010     	push	{r4, lr}
  479598: e1a04000     	mov	r4, r0
  47959c: e280000c     	add	r0, r0, #12
  4795a0: eb0004c9     	bl	0x47a8cc <ObjectiveList::~ObjectiveList()> @ imm = #0x1324
  4795a4: e1a00004     	mov	r0, r4
  4795a8: e8bd8010     	pop	{r4, pc}

004795ac <GameEvent::GameEvent()>:
  4795ac: e92d4070     	push	{r4, r5, r6, lr}
  4795b0: e3e03000     	mvn	r3, #0
  4795b4: e3a05000     	mov	r5, #0
  4795b8: e1a04000     	mov	r4, r0
  4795bc: e5803004     	str	r3, [r0, #0x4]
  4795c0: e5803000     	str	r3, [r0]
  4795c4: e5805008     	str	r5, [r0, #0x8]
  4795c8: e280000c     	add	r0, r0, #12
  4795cc: eb000380     	bl	0x47a3d4 <ObjectiveList::ObjectiveList()> @ imm = #0xe00
  4795d0: e584501c     	str	r5, [r4, #0x1c]
  4795d4: e5c45018     	strb	r5, [r4, #0x18]
  4795d8: e1a00004     	mov	r0, r4
  4795dc: e8bd8070     	pop	{r4, r5, r6, pc}

004795e0 <GameEvent::GameEvent()>:
  4795e0: e92d4070     	push	{r4, r5, r6, lr}
  4795e4: e3e03000     	mvn	r3, #0
  4795e8: e3a05000     	mov	r5, #0
  4795ec: e1a04000     	mov	r4, r0
  4795f0: e5803004     	str	r3, [r0, #0x4]
  4795f4: e5803000     	str	r3, [r0]
  4795f8: e5805008     	str	r5, [r0, #0x8]
  4795fc: e280000c     	add	r0, r0, #12
  479600: eb000373     	bl	0x47a3d4 <ObjectiveList::ObjectiveList()> @ imm = #0xdcc
  479604: e584501c     	str	r5, [r4, #0x1c]
  479608: e5c45018     	strb	r5, [r4, #0x18]
  47960c: e1a00004     	mov	r0, r4
  479610: e8bd8070     	pop	{r4, r5, r6, pc}

00479614 <_GLOBAL__I_.._.._sources_Game_Progression_GameEvent.cpp>:
  479614: e92d4070     	push	{r4, r5, r6, lr}
  479618: e59f40ac     	ldr	r4, [pc, #0xac]         @ 0x4796cc <_GLOBAL__I_.._.._sources_Game_Progression_GameEvent.cpp+0xb8>
  47961c: e59f20ac     	ldr	r2, [pc, #0xac]         @ 0x4796d0 <_GLOBAL__I_.._.._sources_Game_Progression_GameEvent.cpp+0xbc>
  479620: e59f30ac     	ldr	r3, [pc, #0xac]         @ 0x4796d4 <_GLOBAL__I_.._.._sources_Game_Progression_GameEvent.cpp+0xc0>
  479624: e08f4004     	add	r4, pc, r4
  479628: e7941002     	ldr	r1, [r4, r2]
  47962c: e08f3003     	add	r3, pc, r3
  479630: e3a0243f     	mov	r2, #1056964608
  479634: e5910000     	ldr	r0, [r1]
  479638: e5832008     	str	r2, [r3, #0x8]
  47963c: e5832000     	str	r2, [r3]
  479640: e3100001     	tst	r0, #1
  479644: e5832004     	str	r2, [r3, #0x4]
  479648: 0a000012     	beq	0x479698 <_GLOBAL__I_.._.._sources_Game_Progression_GameEvent.cpp+0x84> @ imm = #0x48
  47964c: e59f3084     	ldr	r3, [pc, #0x84]         @ 0x4796d8 <_GLOBAL__I_.._.._sources_Game_Progression_GameEvent.cpp+0xc4>
  479650: e7943003     	ldr	r3, [r4, r3]
  479654: e5932000     	ldr	r2, [r3]
  479658: e3120001     	tst	r2, #1
  47965c: 0a000000     	beq	0x479664 <_GLOBAL__I_.._.._sources_Game_Progression_GameEvent.cpp+0x50> @ imm = #0x0
  479660: e8bd8070     	pop	{r4, r5, r6, pc}
  479664: e3a02001     	mov	r2, #1
  479668: e5832000     	str	r2, [r3]
  47966c: e59f3068     	ldr	r3, [pc, #0x68]         @ 0x4796dc <_GLOBAL__I_.._.._sources_Game_Progression_GameEvent.cpp+0xc8>
  479670: e7945003     	ldr	r5, [r4, r3]
  479674: e1a00005     	mov	r0, r5
  479678: ebfad047     	bl	0x32d79c <Application::Application()> @ imm = #-0x14bee4
  47967c: e59f305c     	ldr	r3, [pc, #0x5c]         @ 0x4796e0 <_GLOBAL__I_.._.._sources_Game_Progression_GameEvent.cpp+0xcc>
  479680: e1a00005     	mov	r0, r5
  479684: e7941003     	ldr	r1, [r4, r3]
  479688: e59f3054     	ldr	r3, [pc, #0x54]         @ 0x4796e4 <_GLOBAL__I_.._.._sources_Game_Progression_GameEvent.cpp+0xd0>
  47968c: e7942003     	ldr	r2, [r4, r3]
  479690: e8bd4070     	pop	{r4, r5, r6, lr}
  479694: eafa531a     	b	0x30e304 <.plt+0x590>   @ imm = #-0x16b398
  479698: e3a03001     	mov	r3, #1
  47969c: e5813000     	str	r3, [r1]
  4796a0: e59f3040     	ldr	r3, [pc, #0x40]         @ 0x4796e8 <_GLOBAL__I_.._.._sources_Game_Progression_GameEvent.cpp+0xd4>
  4796a4: e7945003     	ldr	r5, [r4, r3]
  4796a8: e1a00005     	mov	r0, r5
