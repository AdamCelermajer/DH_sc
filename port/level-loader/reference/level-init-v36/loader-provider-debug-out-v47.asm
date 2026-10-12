
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00324114 <_DEBUG_OUT(char const*, ...)>:
  324114: e92d000f     	push	{r0, r1, r2, r3}
  324118: e59f3088     	ldr	r3, [pc, #0x88]         @ 0x3241a8 <_DEBUG_OUT(char const*, ...)+0x94>
  32411c: e92d4030     	push	{r4, r5, lr}
  324120: e59f2084     	ldr	r2, [pc, #0x84]         @ 0x3241ac <_DEBUG_OUT(char const*, ...)+0x98>
  324124: e08f3003     	add	r3, pc, r3
  324128: e24ddb01     	sub	sp, sp, #1024
