
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0030f288 <StrToObj(char const*, float&)>:
  30f288: e92d4010     	push	{r4, lr}
  30f28c: e1a04001     	mov	r4, r1
  30f290: e3a01000     	mov	r1, #0
  30f294: ebfffd04     	bl	0x30e6ac <.plt+0x938>   @ imm = #-0xbf0
  30f298: ebfffd00     	bl	0x30e6a0 <.plt+0x92c>   @ imm = #-0xc00
  30f29c: e5840000     	str	r0, [r4]
  30f2a0: e8bd8010     	pop	{r4, pc}
