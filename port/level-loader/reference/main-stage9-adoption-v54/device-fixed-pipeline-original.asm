
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00381668 <Device::IsFixedPipeline()>:
  381668: e59f3030     	ldr	r3, [pc, #0x30]         @ 0x3816a0 <Device::IsFixedPipeline()+0x38>
  38166c: e59f2030     	ldr	r2, [pc, #0x30]         @ 0x3816a4 <Device::IsFixedPipeline()+0x3c>
  381670: e92d4010     	push	{r4, lr}
  381674: e08f3003     	add	r3, pc, r3
  381678: e7932002     	ldr	r2, [r3, r2]
  38167c: e5923010     	ldr	r3, [r2, #0x10]
  381680: e5933010     	ldr	r3, [r3, #0x10]
  381684: e1a00003     	mov	r0, r3
  381688: e5933000     	ldr	r3, [r3]
  38168c: e1a0e00f     	mov	lr, pc
  381690: e593f05c     	ldr	pc, [r3, #0x5c]
  381694: e2100007     	ands	r0, r0, #7
  381698: 13a00001     	movne	r0, #1
  38169c: e8bd8010     	pop	{r4, pc}
  3816a0: 1c 34 61 00  	.word	0x0061341c
  3816a4: f4 37 00 00  	.word	0x000037f4
