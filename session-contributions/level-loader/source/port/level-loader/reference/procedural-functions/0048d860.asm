
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d860 <rnd::Rule::~Rule()>:
  48d860: e92d4010     	push	{r4, lr}
  48d864: e1a04000     	mov	r4, r0
  48d868: ebffffde     	bl	0x48d7e8 <rnd::Rule::~Rule()> @ imm = #-0x88
  48d86c: e1a00004     	mov	r0, r4
  48d870: ebfa0af2     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17d438
  48d874: e1a00004     	mov	r0, r4
  48d878: e8bd8010     	pop	{r4, pc}
