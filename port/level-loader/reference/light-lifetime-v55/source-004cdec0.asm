
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004cdec0 <Structs::RestartLevel::~RestartLevel()>:
  4cdec0: e92d4010     	push	{r4, lr}
  4cdec4: e1a04000     	mov	r4, r0
  4cdec8: ebffe5e1     	bl	0x4c7654 <Structs::RestartLevel::~RestartLevel()> @ imm = #-0x687c
  4cdecc: e1a00004     	mov	r0, r4
  4cded0: ebf9095a     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1bda98
  4cded4: e1a00004     	mov	r0, r4
  4cded8: e8bd8010     	pop	{r4, pc}
