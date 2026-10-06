
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f94d8 <Level::~Level()>:
  3f94d8: e92d4010     	push	{r4, lr}
  3f94dc: e1a04000     	mov	r4, r0
  3f94e0: ebffff77     	bl	0x3f92c4 <Level::~Level()> @ imm = #-0x224
  3f94e4: e1a00004     	mov	r0, r4
  3f94e8: ebfc5bd4     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xe90b0
  3f94ec: e1a00004     	mov	r0, r4
  3f94f0: e8bd8010     	pop	{r4, pc}
