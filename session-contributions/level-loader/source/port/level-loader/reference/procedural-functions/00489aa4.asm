
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00489aa4 <rnd::TryIsZero(char const*)>:
  489aa4: e59f1038     	ldr	r1, [pc, #0x38]         @ 0x489ae4 <rnd::TryIsZero(char const*)+0x40>
  489aa8: e92d4010     	push	{r4, lr}
  489aac: e08f1001     	add	r1, pc, r1
  489ab0: e1a04000     	mov	r4, r0
  489ab4: ebfa130b     	bl	0x30e6e8 <.plt+0x974>   @ imm = #-0x17b3d4
  489ab8: e3500000     	cmp	r0, #0
  489abc: 0a000006     	beq	0x489adc <rnd::TryIsZero(char const*)+0x38> @ imm = #0x18
  489ac0: e59f1020     	ldr	r1, [pc, #0x20]         @ 0x489ae8 <rnd::TryIsZero(char const*)+0x44>
  489ac4: e1a00004     	mov	r0, r4
  489ac8: e08f1001     	add	r1, pc, r1
  489acc: ebfa1305     	bl	0x30e6e8 <.plt+0x974>   @ imm = #-0x17b3ec
  489ad0: e2700001     	rsbs	r0, r0, #1
  489ad4: 33a00000     	movlo	r0, #0
  489ad8: e8bd8010     	pop	{r4, pc}
  489adc: e3a00001     	mov	r0, #1
  489ae0: e8bd8010     	pop	{r4, pc}
  489ae4: e4 39 44 00  	.word	0x004439e4
  489ae8: 40 1d 44 00  	.word	0x00441d40
