
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0038952c <Module::~Module()>:
  38952c: e92d4010     	push	{r4, lr}
  389530: e1a04000     	mov	r4, r0
  389534: ebffffc2     	bl	0x389444 <Module::~Module()> @ imm = #-0xf8
  389538: e1a00004     	mov	r0, r4
  38953c: ebfe1bbf     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x79104
  389540: e1a00004     	mov	r0, r4
  389544: e8bd8010     	pop	{r4, pc}
