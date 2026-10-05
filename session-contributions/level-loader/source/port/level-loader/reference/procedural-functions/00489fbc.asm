
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00489fbc <rnd::Block::~Block()>:
  489fbc: e92d4010     	push	{r4, lr}
  489fc0: e1a04000     	mov	r4, r0
  489fc4: ebffffe3     	bl	0x489f58 <rnd::Block::~Block()> @ imm = #-0x74
  489fc8: e1a00004     	mov	r0, r4
  489fcc: ebfa191b     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x179b94
  489fd0: e1a00004     	mov	r0, r4
  489fd4: e8bd8010     	pop	{r4, pc}
