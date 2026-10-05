
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048bc00 <rnd::RPElem::FillSizes(int, int)>:
  48bc00: e1923001     	orrs	r3, r2, r1
  48bc04: 412fff1e     	bxmi	lr
  48bc08: e1520001     	cmp	r2, r1
  48bc0c: a5802010     	strge	r2, [r0, #0x10]
  48bc10: b5801010     	strlt	r1, [r0, #0x10]
  48bc14: e5801014     	str	r1, [r0, #0x14]
  48bc18: e580100c     	str	r1, [r0, #0xc]
  48bc1c: e12fff1e     	bx	lr
