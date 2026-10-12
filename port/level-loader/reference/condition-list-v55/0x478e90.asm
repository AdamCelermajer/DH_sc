
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00478e90 <Condition::~Condition()>:
  478e90: e92d4010     	push	{r4, lr}
  478e94: e1a04000     	mov	r4, r0
  478e98: ebfffe0e     	bl	0x4786d8 <Condition::~Condition()> @ imm = #-0x7c8
  478e9c: e1a00004     	mov	r0, r4
  478ea0: ebfa5d66     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x168a68
  478ea4: e1a00004     	mov	r0, r4
  478ea8: e8bd8010     	pop	{r4, pc}
