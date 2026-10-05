
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048c3f8 <rnd::RoomPool::Find(unsigned char*)>:
  48c3f8: e92d4010     	push	{r4, lr}
  48c3fc: e1a04000     	mov	r4, r0
  48c400: e5900010     	ldr	r0, [r0, #0x10]
  48c404: ebffddc2     	bl	0x483b14 <rnd::RandomGenerator::Hash(unsigned char*)> @ imm = #-0x88f8
  48c408: e1a01000     	mov	r1, r0
  48c40c: e1a00004     	mov	r0, r4
  48c410: e8bd4010     	pop	{r4, lr}
  48c414: eafffe01     	b	0x48bc20 <rnd::RoomPool::Find(unsigned long)> @ imm = #-0x7fc
