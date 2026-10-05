
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d6dc <rnd::RootRule::~RootRule()>:
  48d6dc: e92d4010     	push	{r4, lr}
  48d6e0: e1a04000     	mov	r4, r0
  48d6e4: ebffffed     	bl	0x48d6a0 <rnd::RootRule::~RootRule()> @ imm = #-0x4c
  48d6e8: e1a00004     	mov	r0, r4
  48d6ec: ebfa0b53     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17d2b4
  48d6f0: e1a00004     	mov	r0, r4
  48d6f4: e8bd8010     	pop	{r4, pc}
