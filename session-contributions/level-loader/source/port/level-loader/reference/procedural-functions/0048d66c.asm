
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d66c <rnd::EndPath::~EndPath()>:
  48d66c: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x48d698 <rnd::EndPath::~EndPath()+0x2c>
  48d670: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x48d69c <rnd::EndPath::~EndPath()+0x30>
  48d674: e92d4010     	push	{r4, lr}
  48d678: e08f3003     	add	r3, pc, r3
  48d67c: e7932002     	ldr	r2, [r3, r2]
  48d680: e1a04000     	mov	r4, r0
  48d684: e2822008     	add	r2, r2, #8
  48d688: e5802000     	str	r2, [r0]
  48d68c: ebffffbe     	bl	0x48d58c <rnd::Rule::~Rule()> @ imm = #-0x108
  48d690: e1a00004     	mov	r0, r4
  48d694: e8bd8010     	pop	{r4, pc}
  48d698: 18 74 50 00  	.word	0x00507418
  48d69c: f4 2e 00 00  	.word	0x00002ef4
