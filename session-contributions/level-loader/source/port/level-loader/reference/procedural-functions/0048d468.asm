
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d468 <rnd::EndPath::Impl::~Impl()>:
  48d468: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x48d494 <rnd::EndPath::Impl::~Impl()+0x2c>
  48d46c: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x48d498 <rnd::EndPath::Impl::~Impl()+0x30>
  48d470: e92d4010     	push	{r4, lr}
  48d474: e08f3003     	add	r3, pc, r3
  48d478: e7932002     	ldr	r2, [r3, r2]
  48d47c: e1a04000     	mov	r4, r0
  48d480: e2822008     	add	r2, r2, #8
  48d484: e5802000     	str	r2, [r0]
  48d488: ebffff8d     	bl	0x48d2c4 <rnd::Rule::Impl::~Impl()> @ imm = #-0x1cc
  48d48c: e1a00004     	mov	r0, r4
  48d490: e8bd8010     	pop	{r4, pc}
  48d494: 1c 76 50 00  	.word	0x0050761c
  48d498: 5c 37 00 00  	.word	0x0000375c
