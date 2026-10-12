
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0045632c <ScriptManager::ScriptCmds::Free()>:
  45632c: e92d4070     	push	{r4, r5, r6, lr}
  456330: e1a06000     	mov	r6, r0
  456334: e5900008     	ldr	r0, [r0, #0x8]
  456338: e3500000     	cmp	r0, #0
  45633c: 0a000010     	beq	0x456384 <ScriptManager::ScriptCmds::Free()+0x58> @ imm = #0x40
  456340: e5963000     	ldr	r3, [r6]
  456344: e3530000     	cmp	r3, #0
  456348: da00000c     	ble	0x456380 <ScriptManager::ScriptCmds::Free()+0x54> @ imm = #0x30
  45634c: e3a04000     	mov	r4, #0
  456350: e7905104     	ldr	r5, [r0, r4, lsl #2]
  456354: e2844001     	add	r4, r4, #1
  456358: e3550000     	cmp	r5, #0
  45635c: 0a000005     	beq	0x456378 <ScriptManager::ScriptCmds::Free()+0x4c> @ imm = #0x14
  456360: e1a00005     	mov	r0, r5
  456364: ebfffdbf     	bl	0x455a68 <ScriptCmdImpl::~ScriptCmdImpl()> @ imm = #-0x904
  456368: e1a00005     	mov	r0, r5
  45636c: ebfae833     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x145f34
  456370: e5963000     	ldr	r3, [r6]
  456374: e5960008     	ldr	r0, [r6, #0x8]
  456378: e1530004     	cmp	r3, r4
  45637c: cafffff3     	bgt	0x456350 <ScriptManager::ScriptCmds::Free()+0x24> @ imm = #-0x34
  456380: ebfae82e     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x145f48
  456384: e3a03000     	mov	r3, #0
  456388: e5863000     	str	r3, [r6]
  45638c: e5863008     	str	r3, [r6, #0x8]
  456390: e8bd8070     	pop	{r4, r5, r6, pc}

00456394 <Structs::ScriptCmd* GetNewScriptCmdDataInstance<Structs::FlushMessages>()>:
  456394: e92d4010     	push	{r4, lr}
  456398: e3a01000     	mov	r1, #0
  45639c: e3a00008     	mov	r0, #8
  4563a0: ebfae872     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x145e38
  4563a4: e59f401c     	ldr	r4, [pc, #0x1c]         @ 0x4563c8 <Structs::ScriptCmd* GetNewScriptCmdDataInstance<Structs::FlushMessages>()+0x34>
  4563a8: e59f301c     	ldr	r3, [pc, #0x1c]         @ 0x4563cc <Structs::ScriptCmd* GetNewScriptCmdDataInstance<Structs::FlushMessages>()+0x38>
  4563ac: e3a01000     	mov	r1, #0
  4563b0: e08f4004     	add	r4, pc, r4
  4563b4: e7943003     	ldr	r3, [r4, r3]
  4563b8: e5801004     	str	r1, [r0, #0x4]
  4563bc: e2833008     	add	r3, r3, #8
  4563c0: e5803000     	str	r3, [r0]
  4563c4: e8bd8010     	pop	{r4, pc}
  4563c8: e0 e6 53 00  	.word	0x0053e6e0
  4563cc: 6c 06 00 00  	.word	0x0000066c

004563d0 <Structs::ScriptCmd* GetNewScriptCmdDataInstance<Structs::DoTutorial>()>:
  4563d0: e92d4010     	push	{r4, lr}
  4563d4: e3a01000     	mov	r1, #0
  4563d8: e3a00014     	mov	r0, #20
  4563dc: ebfae863     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x145e74
  4563e0: e59f401c     	ldr	r4, [pc, #0x1c]         @ 0x456404 <Structs::ScriptCmd* GetNewScriptCmdDataInstance<Structs::DoTutorial>()+0x34>
  4563e4: e59f301c     	ldr	r3, [pc, #0x1c]         @ 0x456408 <Structs::ScriptCmd* GetNewScriptCmdDataInstance<Structs::DoTutorial>()+0x38>
  4563e8: e3a01000     	mov	r1, #0
  4563ec: e08f4004     	add	r4, pc, r4
  4563f0: e7943003     	ldr	r3, [r4, r3]
  4563f4: e580100c     	str	r1, [r0, #0xc]
  4563f8: e2833008     	add	r3, r3, #8
  4563fc: e5803000     	str	r3, [r0]
  456400: e8bd8010     	pop	{r4, pc}
  456404: a4 e6 53 00  	.word	0x0053e6a4
  456408: 68 3c 00 00  	.word	0x00003c68

0045640c <Structs::ScriptCmd* GetNewScriptCmdDataInstance<Structs::LockTutorial>()>:
  45640c: e92d4010     	push	{r4, lr}
  456410: e3a01000     	mov	r1, #0
  456414: e3a0000c     	mov	r0, #12
  456418: ebfae854     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x145eb0
  45641c: e59f4020     	ldr	r4, [pc, #0x20]         @ 0x456444 <Structs::ScriptCmd* GetNewScriptCmdDataInstance<Structs::LockTutorial>()+0x38>
  456420: e59f2020     	ldr	r2, [pc, #0x20]         @ 0x456448 <Structs::ScriptCmd* GetNewScriptCmdDataInstance<Structs::LockTutorial>()+0x3c>
  456424: e3a01000     	mov	r1, #0
  456428: e08f4004     	add	r4, pc, r4
  45642c: e7942002     	ldr	r2, [r4, r2]
  456430: e5801008     	str	r1, [r0, #0x8]
  456434: e5801004     	str	r1, [r0, #0x4]
  456438: e2822008     	add	r2, r2, #8
  45643c: e5802000     	str	r2, [r0]
