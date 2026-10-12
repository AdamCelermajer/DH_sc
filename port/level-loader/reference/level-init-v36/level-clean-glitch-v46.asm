
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0031f55c <Application::CleanGlitch()>:
  31f55c: e59f3028     	ldr	r3, [pc, #0x28]         @ 0x31f58c <Application::CleanGlitch()+0x30>
  31f560: e59f2028     	ldr	r2, [pc, #0x28]         @ 0x31f590 <Application::CleanGlitch()+0x34>
  31f564: e92d4010     	push	{r4, lr}
  31f568: e08f3003     	add	r3, pc, r3
  31f56c: e7932002     	ldr	r2, [r3, r2]
  31f570: e5923010     	ldr	r3, [r2, #0x10]
  31f574: e5933010     	ldr	r3, [r3, #0x10]
  31f578: e1a00003     	mov	r0, r3
  31f57c: e5933000     	ldr	r3, [r3]
  31f580: e1a0e00f     	mov	lr, pc
  31f584: e593f0ac     	ldr	pc, [r3, #0xac]
  31f588: e8bd8010     	pop	{r4, pc}
  31f58c: 28 55 67 00  	.word	0x00675528
  31f590: f4 37 00 00  	.word	0x000037f4
