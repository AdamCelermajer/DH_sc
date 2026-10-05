
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003181d8 <UserProperties::~UserProperties()>:
  3181d8: e92d4010     	push	{r4, lr}
  3181dc: e1a04000     	mov	r4, r0
  3181e0: ebffffe4     	bl	0x318178 <UserProperties::~UserProperties()> @ imm = #-0x70
  3181e4: e1a00004     	mov	r0, r4
  3181e8: ebffe094     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x7db0
  3181ec: e1a00004     	mov	r0, r4
  3181f0: e8bd8010     	pop	{r4, pc}
