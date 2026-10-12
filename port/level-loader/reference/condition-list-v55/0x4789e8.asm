
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004789e8 <Condition* GetConditionImplInstance<Condition_IsPlayerAtLevel>()>:
  4789e8: e92d4070     	push	{r4, r5, r6, lr}
  4789ec: e3a01000     	mov	r1, #0
  4789f0: e3a00008     	mov	r0, #8
  4789f4: ebfa5edd     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x16848c
  4789f8: e3a03000     	mov	r3, #0
  4789fc: e5803004     	str	r3, [r0, #0x4]
  478a00: e5803000     	str	r3, [r0]
  478a04: e59f5020     	ldr	r5, [pc, #0x20]         @ 0x478a2c <Condition* GetConditionImplInstance<Condition_IsPlayerAtLevel>()+0x44>
  478a08: e1a04000     	mov	r4, r0
  478a0c: ebffff0d     	bl	0x478648 <Condition::Condition()> @ imm = #-0x3cc
  478a10: e59f3018     	ldr	r3, [pc, #0x18]         @ 0x478a30 <Condition* GetConditionImplInstance<Condition_IsPlayerAtLevel>()+0x48>
  478a14: e08f5005     	add	r5, pc, r5
  478a18: e1a00004     	mov	r0, r4
  478a1c: e7953003     	ldr	r3, [r5, r3]
  478a20: e2833008     	add	r3, r3, #8
  478a24: e5843000     	str	r3, [r4]
  478a28: e8bd8070     	pop	{r4, r5, r6, pc}
  478a2c: 7c c0 51 00  	.word	0x0051c07c
  478a30: dc 39 00 00  	.word	0x000039dc
