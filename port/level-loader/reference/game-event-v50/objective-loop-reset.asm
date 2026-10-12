
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0047a540 <ObjectiveList::LoopOnAll(void (Objective::*)())>:
  47a540: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  47a544: e5903000     	ldr	r3, [r0]
  47a548: e24dd008     	sub	sp, sp, #8
  47a54c: e1a04000     	mov	r4, r0
  47a550: e3530000     	cmp	r3, #0
  47a554: e88d0006     	stm	sp, {r1, r2}
  47a558: e1a06001     	mov	r6, r1
  47a55c: da00000f     	ble	0x47a5a0 <ObjectiveList::LoopOnAll(void (Objective::*)())+0x60> @ imm = #0x3c
  47a560: e1a070c2     	asr	r7, r2, #1
  47a564: e2028001     	and	r8, r2, #1
  47a568: e3a05000     	mov	r5, #0
  47a56c: e5942004     	ldr	r2, [r4, #0x4]
  47a570: e3580000     	cmp	r8, #0
  47a574: e1a03006     	mov	r3, r6
  47a578: e7920105     	ldr	r0, [r2, r5, lsl #2]
  47a57c: e2855001     	add	r5, r5, #1
  47a580: 17903007     	ldrne	r3, [r0, r7]
  47a584: 00800007     	addeq	r0, r0, r7
  47a588: 10800007     	addne	r0, r0, r7
  47a58c: 17933006     	ldrne	r3, [r3, r6]
  47a590: e12fff33     	blx	r3
  47a594: e5943000     	ldr	r3, [r4]
  47a598: e1530005     	cmp	r3, r5
  47a59c: cafffff2     	bgt	0x47a56c <ObjectiveList::LoopOnAll(void (Objective::*)())+0x2c> @ imm = #-0x38
  47a5a0: e28dd008     	add	sp, sp, #8
  47a5a4: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
