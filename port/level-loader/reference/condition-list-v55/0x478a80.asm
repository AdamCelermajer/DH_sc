
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00478a80 <Condition* GetConditionImplInstance<Condition_IsPlayerInLevel>()>:
  478a80: e92d4070     	push	{r4, r5, r6, lr}
  478a84: e3a01000     	mov	r1, #0
  478a88: e3a00008     	mov	r0, #8
  478a8c: ebfa5eb7     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x168524
  478a90: e3a03000     	mov	r3, #0
  478a94: e5803004     	str	r3, [r0, #0x4]
  478a98: e5803000     	str	r3, [r0]
  478a9c: e59f5020     	ldr	r5, [pc, #0x20]         @ 0x478ac4 <Condition* GetConditionImplInstance<Condition_IsPlayerInLevel>()+0x44>
  478aa0: e1a04000     	mov	r4, r0
  478aa4: ebfffee7     	bl	0x478648 <Condition::Condition()> @ imm = #-0x464
  478aa8: e59f3018     	ldr	r3, [pc, #0x18]         @ 0x478ac8 <Condition* GetConditionImplInstance<Condition_IsPlayerInLevel>()+0x48>
  478aac: e08f5005     	add	r5, pc, r5
  478ab0: e1a00004     	mov	r0, r4
  478ab4: e7953003     	ldr	r3, [r5, r3]
  478ab8: e2833008     	add	r3, r3, #8
  478abc: e5843000     	str	r3, [r4]
  478ac0: e8bd8070     	pop	{r4, r5, r6, pc}
  478ac4: e4 bf 51 00  	.word	0x0051bfe4
  478ac8: 68 1b 00 00  	.word	0x00001b68
