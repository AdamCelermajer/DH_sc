
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00483ac8 <rnd::RandomGenerator::GetInt(int, int)>:
  483ac8: e1510002     	cmp	r1, r2
  483acc: e92d4070     	push	{r4, r5, r6, lr}
  483ad0: e1a04001     	mov	r4, r1
  483ad4: e1a05002     	mov	r5, r2
  483ad8: ba000001     	blt	0x483ae4 <rnd::RandomGenerator::GetInt(int, int)+0x1c> @ imm = #0x4
  483adc: e1a00001     	mov	r0, r1
  483ae0: e8bd8070     	pop	{r4, r5, r6, pc}
  483ae4: ebffffea     	bl	0x483a94 <rnd::RandomGenerator::NextInt()> @ imm = #-0x58
  483ae8: e0641005     	rsb	r1, r4, r5
  483aec: ebfa2c0e     	bl	0x30eb2c <.plt+0xdb8>   @ imm = #-0x174fc8
  483af0: e0810004     	add	r0, r1, r4
  483af4: e8bd8070     	pop	{r4, r5, r6, pc}
