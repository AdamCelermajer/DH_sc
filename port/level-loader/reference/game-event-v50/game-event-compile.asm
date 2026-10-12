
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004796f0 <GameEventManager::GetEventByID(int) const>:
  479700: e0632002     	rsb	r2, r3, r2
  479704: e1510142     	cmp	r1, r2, asr #2
  479708: b7930101     	ldrlt	r0, [r3, r1, lsl #2]
  47970c: b12fff1e     	bxlt	lr
  479710: e3a00000     	mov	r0, #0
  479714: e12fff1e     	bx	lr

00479718 <GameEventManager::LoopOnAllEvents(void (GameEvent::*)())>:
  479718: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  47971c: e5904000     	ldr	r4, [r0]
  479720: e5903004     	ldr	r3, [r0, #0x4]
  479724: e24dd008     	sub	sp, sp, #8
  479728: e1a05000     	mov	r5, r0
  47972c: e1540003     	cmp	r4, r3
  479730: e88d0006     	stm	sp, {r1, r2}
  479734: e1a07001     	mov	r7, r1
  479738: 0a00000d     	beq	0x479774 <GameEventManager::LoopOnAllEvents(void (GameEvent::*)())+0x5c> @ imm = #0x34
  47973c: e1a060c2     	asr	r6, r2, #1
  479740: e2028001     	and	r8, r2, #1
  479744: e5940000     	ldr	r0, [r4]
  479748: e3580000     	cmp	r8, #0
  47974c: 01a03007     	moveq	r3, r7
  479750: 17903006     	ldrne	r3, [r0, r6]
  479754: 00800006     	addeq	r0, r0, r6
  479758: 10800006     	addne	r0, r0, r6
  47975c: 17933007     	ldrne	r3, [r3, r7]
  479760: e12fff33     	blx	r3
  479764: e5953004     	ldr	r3, [r5, #0x4]
  479768: e2844004     	add	r4, r4, #4
  47976c: e1540003     	cmp	r4, r3
  479770: 1afffff3     	bne	0x479744 <GameEventManager::LoopOnAllEvents(void (GameEvent::*)())+0x2c> @ imm = #-0x34
  479774: e28dd008     	add	sp, sp, #8
  479778: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}

0047977c <GameEventManager::Compile()>:
  47977c: e59f1024     	ldr	r1, [pc, #0x24]         @ 0x4797a8 <GameEventManager::Compile()+0x2c>
  479780: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x4797ac <GameEventManager::Compile()+0x30>
  479784: e3a0c000     	mov	r12, #0
  479788: e08f1001     	add	r1, pc, r1
  47978c: e7913003     	ldr	r3, [r1, r3]
  479790: e24dd008     	sub	sp, sp, #8
  479794: e1a0200c     	mov	r2, r12
  479798: e1a01003     	mov	r1, r3
  47979c: e88d1008     	stm	sp, {r3, r12}
  4797a0: e28dd008     	add	sp, sp, #8
  4797a4: eaffffdb     	b	0x479718 <GameEventManager::LoopOnAllEvents(void (GameEvent::*)())> @ imm = #-0x94
  4797a8: 08 b3 51 00  	.word	0x0051b308
  4797ac: f4 18 00 00  	.word	0x000018f4

004797b0 <GameEventManager::LoopOnAllEvents(void (GameEvent::*)() const) const>:
  4797b0: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  4797b4: e5904000     	ldr	r4, [r0]
  4797b8: e5903004     	ldr	r3, [r0, #0x4]
  4797bc: e24dd008     	sub	sp, sp, #8
  4797c0: e1a05000     	mov	r5, r0
  4797c4: e1540003     	cmp	r4, r3
  4797c8: e88d0006     	stm	sp, {r1, r2}
  4797cc: e1a07001     	mov	r7, r1
  4797d0: 0a00000d     	beq	0x47980c <GameEventManager::LoopOnAllEvents(void (GameEvent::*)() const) const+0x5c> @ imm = #0x34
  4797d4: e1a060c2     	asr	r6, r2, #1
  4797d8: e2028001     	and	r8, r2, #1
  4797dc: e5940000     	ldr	r0, [r4]
  4797e0: e3580000     	cmp	r8, #0
  4797e4: 01a03007     	moveq	r3, r7
  4797e8: 17903006     	ldrne	r3, [r0, r6]
  4797ec: 00800006     	addeq	r0, r0, r6
  4797f0: 10800006     	addne	r0, r0, r6
  4797f4: 17933007     	ldrne	r3, [r3, r7]
  4797f8: e12fff33     	blx	r3
  4797fc: e5953004     	ldr	r3, [r5, #0x4]
  479800: e2844004     	add	r4, r4, #4
  479804: e1540003     	cmp	r4, r3
  479808: 1afffff3     	bne	0x4797dc <GameEventManager::LoopOnAllEvents(void (GameEvent::*)() const) const+0x2c> @ imm = #-0x34
  47980c: e28dd008     	add	sp, sp, #8
  479810: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}

00479814 <GameEventManager::DBG_TraceDetailedInformation() const>:
  479814: e59f1024     	ldr	r1, [pc, #0x24]         @ 0x479840 <GameEventManager::DBG_TraceDetailedInformation() const+0x2c>
  479818: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x479844 <GameEventManager::DBG_TraceDetailedInformation() const+0x30>
  47981c: e3a0c000     	mov	r12, #0
  479820: e08f1001     	add	r1, pc, r1
  479824: e7913003     	ldr	r3, [r1, r3]
  479828: e24dd008     	sub	sp, sp, #8
  47982c: e1a0200c     	mov	r2, r12
  479830: e1a01003     	mov	r1, r3
  479834: e88d1008     	stm	sp, {r3, r12}
  479838: e28dd008     	add	sp, sp, #8
  47983c: eaffffdb     	b	0x4797b0 <GameEventManager::LoopOnAllEvents(void (GameEvent::*)() const) const> @ imm = #-0x94
  479840: 70 b2 51 00  	.word	0x0051b270
  479844: b0 41 00 00  	.word	0x000041b0

00479848 <GameEventManager::GetEventByName(char const*) const>:
  479848: e59f306c     	ldr	r3, [pc, #0x6c]         @ 0x4798bc <GameEventManager::GetEventByName(char const*) const+0x74>
  47984c: e59f206c     	ldr	r2, [pc, #0x6c]         @ 0x4798c0 <GameEventManager::GetEventByName(char const*) const+0x78>
  479850: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  479854: e08f3003     	add	r3, pc, r3
  479858: e7932002     	ldr	r2, [r3, r2]
  47985c: e1a08000     	mov	r8, r0
  479860: e1a06001     	mov	r6, r1
  479864: e5925000     	ldr	r5, [r2]
  479868: e3550000     	cmp	r5, #0
  47986c: 0a00000e     	beq	0x4798ac <GameEventManager::GetEventByName(char const*) const+0x64> @ imm = #0x38
  479870: e59f204c     	ldr	r2, [pc, #0x4c]         @ 0x4798c4 <GameEventManager::GetEventByName(char const*) const+0x7c>
  479874: e3a04000     	mov	r4, #0
  479878: e7933002     	ldr	r3, [r3, r2]
  47987c: e5937000     	ldr	r7, [r3]
  479880: ea000002     	b	0x479890 <GameEventManager::GetEventByName(char const*) const+0x48> @ imm = #0x8
  479884: e2844001     	add	r4, r4, #1
  479888: e1540005     	cmp	r4, r5
  47988c: 0a000006     	beq	0x4798ac <GameEventManager::GetEventByName(char const*) const+0x64> @ imm = #0x18
  479890: e7971104     	ldr	r1, [r7, r4, lsl #2]
  479894: e1a00006     	mov	r0, r6
  479898: ebfa529f     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x16b584
  47989c: e3500000     	cmp	r0, #0
  4798a0: 1afffff7     	bne	0x479884 <GameEventManager::GetEventByName(char const*) const+0x3c> @ imm = #-0x24
  4798a4: e1a01004     	mov	r1, r4
  4798a8: ea000000     	b	0x4798b0 <GameEventManager::GetEventByName(char const*) const+0x68> @ imm = #0x0
  4798ac: e3e01000     	mvn	r1, #0
  4798b0: e1a00008     	mov	r0, r8
  4798b4: e8bd41f0     	pop	{r4, r5, r6, r7, r8, lr}
  4798b8: eaffff8c     	b	0x4796f0 <GameEventManager::GetEventByID(int) const> @ imm = #-0x1d0
  4798bc: 3c b2 51 00  	.word	0x0051b23c
  4798c0: 60 08 00 00  	.word	0x00000860
  4798c4: d0 37 00 00  	.word	0x000037d0

004798c8 <GameEventManager::Update()>:
  4798c8: e92d4070     	push	{r4, r5, r6, lr}
  4798cc: e59f504c     	ldr	r5, [pc, #0x4c]         @ 0x479920 <GameEventManager::Update()+0x58>
  4798d0: e24dd008     	sub	sp, sp, #8
  4798d4: e1a06000     	mov	r6, r0
  4798d8: e08f5005     	add	r5, pc, r5
  4798dc: e1a00005     	mov	r0, r5
  4798e0: e59f403c     	ldr	r4, [pc, #0x3c]         @ 0x479924 <GameEventManager::Update()+0x5c>
  4798e4: ebfa6772     	bl	0x3136b4 <PushProfilingContext(char const*)> @ imm = #-0x166238
  4798e8: e59f2038     	ldr	r2, [pc, #0x38]         @ 0x479928 <GameEventManager::Update()+0x60>
  4798ec: e08f4004     	add	r4, pc, r4
  4798f0: e3a03000     	mov	r3, #0
  4798f4: e794c002     	ldr	r12, [r4, r2]
  4798f8: e1a00006     	mov	r0, r6
  4798fc: e1a02003     	mov	r2, r3
  479900: e1a0100c     	mov	r1, r12
  479904: e58dc000     	str	r12, [sp]
  479908: e58d3004     	str	r3, [sp, #0x4]
  47990c: ebffff81     	bl	0x479718 <GameEventManager::LoopOnAllEvents(void (GameEvent::*)())> @ imm = #-0x1fc
  479910: e1a00005     	mov	r0, r5
  479914: e28dd008     	add	sp, sp, #8
  479918: e8bd4070     	pop	{r4, r5, r6, lr}
  47991c: eafa6765     	b	0x3136b8 <PopProfilingContext(char const*)> @ imm = #-0x16626c
  479920: 68 42 45 00  	.word	0x00454268
  479924: a4 b1 51 00  	.word	0x0051b1a4
  479928: e4 40 00 00  	.word	0x000040e4

0047992c <GameEventManager::ReInit()>:
  47992c: e92d4070     	push	{r4, r5, r6, lr}
  479930: e5904000     	ldr	r4, [r0]
  479934: e5903004     	ldr	r3, [r0, #0x4]
  479938: e1a06000     	mov	r6, r0
  47993c: e1540003     	cmp	r4, r3
  479940: 0a000007     	beq	0x479964 <GameEventManager::ReInit()+0x38> @ imm = #0x1c
  479944: e4945004     	ldr	r5, [r4], #4
  479948: e1a00005     	mov	r0, r5
  47994c: ebfffef9     	bl	0x479538 <GameEvent::ReInit()> @ imm = #-0x41c
  479950: e1a00005     	mov	r0, r5
  479954: ebfffee8     	bl	0x4794fc <GameEvent::Compile()> @ imm = #-0x460
  479958: e5963004     	ldr	r3, [r6, #0x4]
  47995c: e1540003     	cmp	r4, r3
  479960: 1afffff7     	bne	0x479944 <GameEventManager::ReInit()+0x18> @ imm = #-0x24
  479964: e8bd8070     	pop	{r4, r5, r6, pc}
