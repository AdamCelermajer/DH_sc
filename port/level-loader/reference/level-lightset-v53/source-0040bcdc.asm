
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040bcdc <LightPoint::InitPost()>:
  40bcdc: e92d4070     	push	{r4, r5, r6, lr}
  40bce0: e3a05000     	mov	r5, #0
  40bce4: e1a04000     	mov	r4, r0
  40bce8: e5c051b0     	strb	r5, [r0, #0x1b0]
  40bcec: e5c051b1     	strb	r5, [r0, #0x1b1]
  40bcf0: ebfffcf2     	bl	0x40b0c0 <LightBase::InitPost()> @ imm = #-0xc38
  40bcf4: e5943120     	ldr	r3, [r4, #0x120]
  40bcf8: e1a00004     	mov	r0, r4
  40bcfc: e5933134     	ldr	r3, [r3, #0x134]
  40bd00: e1c355b8     	strh	r5, [r3, #88]
  40bd04: e5943000     	ldr	r3, [r4]
  40bd08: e1a0e00f     	mov	lr, pc
  40bd0c: e593f058     	ldr	pc, [r3, #0x58]
  40bd10: e8bd8070     	pop	{r4, r5, r6, pc}
