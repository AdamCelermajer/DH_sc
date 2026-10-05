
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d2a8 <rnd::Rule::Impl::~Impl()>:
  48d2a8: e92d4010     	push	{r4, lr}
  48d2ac: e1a04000     	mov	r4, r0
  48d2b0: ebffffba     	bl	0x48d1a0 <rnd::Rule::Impl::~Impl()> @ imm = #-0x118
  48d2b4: e1a00004     	mov	r0, r4
  48d2b8: ebfa0c60     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17ce80
  48d2bc: e1a00004     	mov	r0, r4
  48d2c0: e8bd8010     	pop	{r4, pc}
