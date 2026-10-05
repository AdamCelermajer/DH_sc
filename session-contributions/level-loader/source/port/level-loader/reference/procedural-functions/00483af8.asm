
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00483af8 <rnd::RandomGenerator::operator()(unsigned int)>:
  483af8: e92d4010     	push	{r4, lr}
  483afc: e1a04001     	mov	r4, r1
  483b00: ebffffe3     	bl	0x483a94 <rnd::RandomGenerator::NextInt()> @ imm = #-0x74
  483b04: e1a01004     	mov	r1, r4
  483b08: ebfa2c07     	bl	0x30eb2c <.plt+0xdb8>   @ imm = #-0x174fe4
  483b0c: e1a00001     	mov	r0, r1
  483b10: e8bd8010     	pop	{r4, pc}
