
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0033e97c <ObjectBase::~ObjectBase()>:
  33e97c: e92d4010     	push	{r4, lr}
  33e980: e1a04000     	mov	r4, r0
  33e984: ebffffad     	bl	0x33e840 <ObjectBase::~ObjectBase()> @ imm = #-0x14c
  33e988: e1a00004     	mov	r0, r4
  33e98c: ebff46ab     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x2e554
  33e990: e1a00004     	mov	r0, r4
  33e994: e8bd8010     	pop	{r4, pc}
