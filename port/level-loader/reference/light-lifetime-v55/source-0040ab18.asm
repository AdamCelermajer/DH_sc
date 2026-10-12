
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040ab18 <LightBase::~LightBase()>:
  40ab18: e92d4010     	push	{r4, lr}
  40ab1c: e1a04000     	mov	r4, r0
  40ab20: ebffffde     	bl	0x40aaa0 <LightBase::~LightBase()> @ imm = #-0x88
  40ab24: e1a00004     	mov	r0, r4
  40ab28: ebfc1644     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xfa6f0
  40ab2c: e1a00004     	mov	r0, r4
  40ab30: e8bd8010     	pop	{r4, pc}
