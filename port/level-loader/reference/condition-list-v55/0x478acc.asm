
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00478acc <Condition* GetConditionImplInstance<Condition_IsQuestStateHigher>()>:
  478acc: e92d4070     	push	{r4, r5, r6, lr}
  478ad0: e3a01000     	mov	r1, #0
  478ad4: e3a0000c     	mov	r0, #12
  478ad8: ebfa5ea4     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x168570
  478adc: e59f4028     	ldr	r4, [pc, #0x28]         @ 0x478b0c <Condition* GetConditionImplInstance<Condition_IsQuestStateHigher>()+0x40>
  478ae0: e1a05000     	mov	r5, r0
  478ae4: ebfffed7     	bl	0x478648 <Condition::Condition()> @ imm = #-0x4a4
  478ae8: e59f3020     	ldr	r3, [pc, #0x20]         @ 0x478b10 <Condition* GetConditionImplInstance<Condition_IsQuestStateHigher>()+0x44>
  478aec: e08f4004     	add	r4, pc, r4
  478af0: e3a02002     	mov	r2, #2
  478af4: e7943003     	ldr	r3, [r4, r3]
  478af8: e5852008     	str	r2, [r5, #0x8]
  478afc: e1a00005     	mov	r0, r5
  478b00: e2833008     	add	r3, r3, #8
  478b04: e5853000     	str	r3, [r5]
  478b08: e8bd8070     	pop	{r4, r5, r6, pc}
  478b0c: a4 bf 51 00  	.word	0x0051bfa4
  478b10: c8 38 00 00  	.word	0x000038c8
