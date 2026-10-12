
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040fee8 <CameraLevel::~CameraLevel()>:
  40fee8: e92d4010     	push	{r4, lr}
  40feec: e1a04000     	mov	r4, r0
  40fef0: ebffffe1     	bl	0x40fe7c <CameraLevel::~CameraLevel()> @ imm = #-0x7c
  40fef4: e1a00004     	mov	r0, r4
  40fef8: ebfc0150     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xffac0
  40fefc: e1a00004     	mov	r0, r4
  40ff00: e8bd8010     	pop	{r4, pc}
