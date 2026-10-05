
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00513858 <PropertyMap::GetProperty(char const*)>:
  513858: e92d4070     	push	{r4, r5, r6, lr}
  51385c: e1a05001     	mov	r5, r1
  513860: e1a04000     	mov	r4, r0
  513864: e1a01002     	mov	r1, r2
  513868: e1a00005     	mov	r0, r5
  51386c: ebffffe5     	bl	0x513808 <PropertyMap::GetProp(char const*)> @ imm = #-0x6c
  513870: e8840021     	stm	r4, {r0, r5}
  513874: e1a00004     	mov	r0, r4
  513878: e8bd8070     	pop	{r4, r5, r6, pc}
