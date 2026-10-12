
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004cdf68 <Structs::IncFaeryLevel::~IncFaeryLevel()>:
  4cdf68: e92d4010     	push	{r4, lr}
  4cdf6c: e1a04000     	mov	r4, r0
  4cdf70: ebffe515     	bl	0x4c73cc <Structs::IncFaeryLevel::~IncFaeryLevel()> @ imm = #-0x6bac
  4cdf74: e1a00004     	mov	r0, r4
  4cdf78: ebf90930     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1bdb40
  4cdf7c: e1a00004     	mov	r0, r4
  4cdf80: e8bd8010     	pop	{r4, pc}
