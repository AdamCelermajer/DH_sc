
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00478b14 <Condition* GetConditionImplInstance<Condition_IsQuestStateLower>()>:
  478b14: e92d4070     	push	{r4, r5, r6, lr}
  478b18: e3a01000     	mov	r1, #0
  478b1c: e3a0000c     	mov	r0, #12
  478b20: ebfa5e92     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x1685b8
  478b24: e59f4028     	ldr	r4, [pc, #0x28]         @ 0x478b54 <Condition* GetConditionImplInstance<Condition_IsQuestStateLower>()+0x40>
  478b28: e1a05000     	mov	r5, r0
  478b2c: ebfffec5     	bl	0x478648 <Condition::Condition()> @ imm = #-0x4ec
  478b30: e59f3020     	ldr	r3, [pc, #0x20]         @ 0x478b58 <Condition* GetConditionImplInstance<Condition_IsQuestStateLower>()+0x44>
  478b34: e08f4004     	add	r4, pc, r4
  478b38: e3a02001     	mov	r2, #1
  478b3c: e7943003     	ldr	r3, [r4, r3]
  478b40: e5852008     	str	r2, [r5, #0x8]
  478b44: e1a00005     	mov	r0, r5
  478b48: e2833008     	add	r3, r3, #8
  478b4c: e5853000     	str	r3, [r5]
  478b50: e8bd8070     	pop	{r4, r5, r6, pc}
  478b54: 5c bf 51 00  	.word	0x0051bf5c
  478b58: 30 1c 00 00  	.word	0x00001c30
