
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004d1410 <Structs::ChangeLevel::~ChangeLevel()>:
  4d1410: e92d4010     	push	{r4, lr}
  4d1414: e1a04000     	mov	r4, r0
  4d1418: ebffffea     	bl	0x4d13c8 <Structs::ChangeLevel::~ChangeLevel()> @ imm = #-0x58
  4d141c: e1a00004     	mov	r0, r4
  4d1420: ebf8fc06     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1c0fe8
  4d1424: e1a00004     	mov	r0, r4
  4d1428: e8bd8010     	pop	{r4, pc}
