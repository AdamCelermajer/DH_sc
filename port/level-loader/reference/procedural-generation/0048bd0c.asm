
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048bd0c <_ZNK3rnd4Path4Impl9IsDeadEndEv>:
  48bd0c: e5902028     	ldr	r2, [r0, #0x28]
  48bd10: e590302c     	ldr	r3, [r0, #0x2c]
  48bd14: e2822001     	add	r2, r2, #1
  48bd18: e1520003     	cmp	r2, r3
  48bd1c: 13a00000     	movne	r0, #0
  48bd20: 112fff1e     	bxne	lr
  48bd24: e5903004     	ldr	r3, [r0, #0x4]
  48bd28: e593000c     	ldr	r0, [r3, #0xc]
  48bd2c: e2700001     	rsbs	r0, r0, #1
  48bd30: 33a00000     	movlo	r0, #0
  48bd34: e12fff1e     	bx	lr
