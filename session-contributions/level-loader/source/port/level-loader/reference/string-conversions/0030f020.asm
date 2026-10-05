
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0030f020 <StrToObj(char const*, bool&)>:
  30f020: e92d4010     	push	{r4, lr}
  30f024: e1a04001     	mov	r4, r1
  30f028: ebfffc19     	bl	0x30e094 <.plt+0x320>   @ imm = #-0xf9c
  30f02c: e2500000     	subs	r0, r0, #0
  30f030: 13a00001     	movne	r0, #1
  30f034: e5c40000     	strb	r0, [r4]
  30f038: e8bd8010     	pop	{r4, pc}
