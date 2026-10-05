
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00491428 <_ZN3rnd4Tile8AddChildEPS0_>:
  491428: e590300c     	ldr	r3, [r0, #0xc]
  49142c: e2832001     	add	r2, r3, #1
  491430: e2833004     	add	r3, r3, #4
  491434: e7801103     	str	r1, [r0, r3, lsl #2]
  491438: e580200c     	str	r2, [r0, #0xc]
  49143c: e12fff1e     	bx	lr
