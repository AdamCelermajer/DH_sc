
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0047899c <Condition* GetConditionImplInstance<Condition_IsEventInState>()>:
  47899c: e92d4070     	push	{r4, r5, r6, lr}
  4789a0: e3a01000     	mov	r1, #0
  4789a4: e3a00008     	mov	r0, #8
  4789a8: ebfa5ef0     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x168440
  4789ac: e3a03000     	mov	r3, #0
  4789b0: e5803004     	str	r3, [r0, #0x4]
  4789b4: e5803000     	str	r3, [r0]
  4789b8: e59f5020     	ldr	r5, [pc, #0x20]         @ 0x4789e0 <Condition* GetConditionImplInstance<Condition_IsEventInState>()+0x44>
  4789bc: e1a04000     	mov	r4, r0
  4789c0: ebffff20     	bl	0x478648 <Condition::Condition()> @ imm = #-0x380
  4789c4: e59f3018     	ldr	r3, [pc, #0x18]         @ 0x4789e4 <Condition* GetConditionImplInstance<Condition_IsEventInState>()+0x48>
  4789c8: e08f5005     	add	r5, pc, r5
  4789cc: e1a00004     	mov	r0, r4
  4789d0: e7953003     	ldr	r3, [r5, r3]
  4789d4: e2833008     	add	r3, r3, #8
  4789d8: e5843000     	str	r3, [r4]
  4789dc: e8bd8070     	pop	{r4, r5, r6, pc}
  4789e0: c8 c0 51 00  	.word	0x0051c0c8
  4789e4: 64 0c 00 00  	.word	0x00000c64
