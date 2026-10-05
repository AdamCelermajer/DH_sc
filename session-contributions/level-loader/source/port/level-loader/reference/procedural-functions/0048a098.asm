
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048a098 <rnd::MgxBlock::~MgxBlock()>:
  48a098: e92d4010     	push	{r4, lr}
  48a09c: e1a04000     	mov	r4, r0
  48a0a0: ebffffe5     	bl	0x48a03c <rnd::MgxBlock::~MgxBlock()> @ imm = #-0x6c
  48a0a4: e1a00004     	mov	r0, r4
  48a0a8: ebfa18e4     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x179c70
  48a0ac: e1a00004     	mov	r0, r4
  48a0b0: e8bd8010     	pop	{r4, pc}
