
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00478a34 <Condition* GetConditionImplInstance<Condition_IsLevelInState>()>:
  478a34: e92d4070     	push	{r4, r5, r6, lr}
  478a38: e3a01000     	mov	r1, #0
  478a3c: e3a00008     	mov	r0, #8
  478a40: ebfa5eca     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x1684d8
  478a44: e3a03000     	mov	r3, #0
  478a48: e5803004     	str	r3, [r0, #0x4]
  478a4c: e5803000     	str	r3, [r0]
  478a50: e59f5020     	ldr	r5, [pc, #0x20]         @ 0x478a78 <Condition* GetConditionImplInstance<Condition_IsLevelInState>()+0x44>
  478a54: e1a04000     	mov	r4, r0
  478a58: ebfffefa     	bl	0x478648 <Condition::Condition()> @ imm = #-0x418
  478a5c: e59f3018     	ldr	r3, [pc, #0x18]         @ 0x478a7c <Condition* GetConditionImplInstance<Condition_IsLevelInState>()+0x48>
  478a60: e08f5005     	add	r5, pc, r5
  478a64: e1a00004     	mov	r0, r4
  478a68: e7953003     	ldr	r3, [r5, r3]
  478a6c: e2833008     	add	r3, r3, #8
  478a70: e5843000     	str	r3, [r4]
  478a74: e8bd8070     	pop	{r4, r5, r6, pc}
  478a78: 30 c0 51 00  	.word	0x0051c030
  478a7c: 1c 36 00 00  	.word	0x0000361c
