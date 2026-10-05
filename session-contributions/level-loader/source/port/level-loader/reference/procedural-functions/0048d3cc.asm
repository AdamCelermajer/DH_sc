
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d3cc <rnd::RootRule::Impl::~Impl()>:
  48d3cc: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x48d3f8 <rnd::RootRule::Impl::~Impl()+0x2c>
  48d3d0: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x48d3fc <rnd::RootRule::Impl::~Impl()+0x30>
  48d3d4: e92d4010     	push	{r4, lr}
  48d3d8: e08f3003     	add	r3, pc, r3
  48d3dc: e7932002     	ldr	r2, [r3, r2]
  48d3e0: e1a04000     	mov	r4, r0
  48d3e4: e2822008     	add	r2, r2, #8
  48d3e8: e5802000     	str	r2, [r0]
  48d3ec: ebffffb4     	bl	0x48d2c4 <rnd::Rule::Impl::~Impl()> @ imm = #-0x130
  48d3f0: e1a00004     	mov	r0, r4
  48d3f4: e8bd8010     	pop	{r4, pc}
  48d3f8: b8 76 50 00  	.word	0x005076b8
  48d3fc: ac 07 00 00  	.word	0x000007ac
