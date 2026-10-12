
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0031f594 <Application::GetCurrentLevel() const>:
  31f594: e59f3010     	ldr	r3, [pc, #0x10]         @ 0x31f5ac <Application::GetCurrentLevel() const+0x18>
  31f598: e59f2010     	ldr	r2, [pc, #0x10]         @ 0x31f5b0 <Application::GetCurrentLevel() const+0x1c>
  31f59c: e08f3003     	add	r3, pc, r3
  31f5a0: e7932002     	ldr	r2, [r3, r2]
  31f5a4: e5920000     	ldr	r0, [r2]
  31f5a8: e12fff1e     	bx	lr
  31f5ac: f4 54 67 00  	.word	0x006754f4
  31f5b0: 64 1d 00 00  	.word	0x00001d64

0031f5b4 <Application::IsLevelLoaded()>:
  31f5b4: e92d4010     	push	{r4, lr}
