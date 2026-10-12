
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004cea04 <Structs::ClassFuncLinearLevel::~ClassFuncLinearLevel()>:
  4cea04: e92d4010     	push	{r4, lr}
  4cea08: e1a04000     	mov	r4, r0
  4cea0c: ebffdb3c     	bl	0x4c5704 <Structs::ClassFuncLinearLevel::~ClassFuncLinearLevel()> @ imm = #-0x9310
  4cea10: e1a00004     	mov	r0, r4
  4cea14: ebf90689     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1be5dc
  4cea18: e1a00004     	mov	r0, r4
  4cea1c: e8bd8010     	pop	{r4, pc}
