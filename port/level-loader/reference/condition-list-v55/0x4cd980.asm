
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004cd980 <Structs::v2Condition::~v2Condition()>:
  4cd980: e92d4010     	push	{r4, lr}
  4cd984: e1a04000     	mov	r4, r0
  4cd988: ebffe8cc     	bl	0x4c7cc0 <Structs::v2Condition::~v2Condition()> @ imm = #-0x5cd0
  4cd98c: e1a00004     	mov	r0, r4
  4cd990: ebf90aaa     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1bd558
  4cd994: e1a00004     	mov	r0, r4
  4cd998: e8bd8010     	pop	{r4, pc}
