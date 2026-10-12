
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00478b5c <Condition* GetConditionImplInstance<Condition_IsQuestInState>()>:
  478b5c: e92d4070     	push	{r4, r5, r6, lr}
  478b60: e3a01000     	mov	r1, #0
  478b64: e3a0000c     	mov	r0, #12
  478b68: ebfa5e80     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x168600
  478b6c: e59f4028     	ldr	r4, [pc, #0x28]         @ 0x478b9c <Condition* GetConditionImplInstance<Condition_IsQuestInState>()+0x40>
  478b70: e1a05000     	mov	r5, r0
  478b74: ebfffeb3     	bl	0x478648 <Condition::Condition()> @ imm = #-0x534
  478b78: e59f3020     	ldr	r3, [pc, #0x20]         @ 0x478ba0 <Condition* GetConditionImplInstance<Condition_IsQuestInState>()+0x44>
  478b7c: e08f4004     	add	r4, pc, r4
  478b80: e3a02000     	mov	r2, #0
  478b84: e7943003     	ldr	r3, [r4, r3]
  478b88: e5852008     	str	r2, [r5, #0x8]
  478b8c: e1a00005     	mov	r0, r5
  478b90: e2833008     	add	r3, r3, #8
  478b94: e5853000     	str	r3, [r5]
  478b98: e8bd8070     	pop	{r4, r5, r6, pc}
  478b9c: 14 bf 51 00  	.word	0x0051bf14
  478ba0: 24 28 00 00  	.word	0x00002824
