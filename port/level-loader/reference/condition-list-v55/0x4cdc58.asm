
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004cdc58 <Structs::v2ConditionStub::~v2ConditionStub()>:
  4cdc58: e92d4010     	push	{r4, lr}
  4cdc5c: e1a04000     	mov	r4, r0
  4cdc60: ebffe825     	bl	0x4c7cfc <Structs::v2ConditionStub::~v2ConditionStub()> @ imm = #-0x5f6c
  4cdc64: e1a00004     	mov	r0, r4
  4cdc68: ebf909f4     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1bd830
  4cdc6c: e1a00004     	mov	r0, r4
  4cdc70: e8bd8010     	pop	{r4, pc}
