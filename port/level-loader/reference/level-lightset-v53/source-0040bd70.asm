
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040bd70 <LightPoint::~LightPoint()>:
  40bd70: e92d4010     	push	{r4, lr}
  40bd74: e1a04000     	mov	r4, r0
  40bd78: ebffffe7     	bl	0x40bd1c <LightPoint::~LightPoint()> @ imm = #-0x64
  40bd7c: e1a00004     	mov	r0, r4
  40bd80: ebfc11ae     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xfb948
  40bd84: e1a00004     	mov	r0, r4
  40bd88: e8bd8010     	pop	{r4, pc}
