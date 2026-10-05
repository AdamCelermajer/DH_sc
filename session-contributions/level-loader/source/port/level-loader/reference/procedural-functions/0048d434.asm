
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d434 <rnd::ForceBlock::Impl::~Impl()>:
  48d434: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x48d460 <rnd::ForceBlock::Impl::~Impl()+0x2c>
  48d438: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x48d464 <rnd::ForceBlock::Impl::~Impl()+0x30>
  48d43c: e92d4010     	push	{r4, lr}
  48d440: e08f3003     	add	r3, pc, r3
  48d444: e7932002     	ldr	r2, [r3, r2]
  48d448: e1a04000     	mov	r4, r0
  48d44c: e2822008     	add	r2, r2, #8
  48d450: e5802000     	str	r2, [r0]
  48d454: ebffff9a     	bl	0x48d2c4 <rnd::Rule::Impl::~Impl()> @ imm = #-0x198
  48d458: e1a00004     	mov	r0, r4
  48d45c: e8bd8010     	pop	{r4, pc}
  48d460: 50 76 50 00  	.word	0x00507650
  48d464: 08 4c 00 00  	.word	0x00004c08
