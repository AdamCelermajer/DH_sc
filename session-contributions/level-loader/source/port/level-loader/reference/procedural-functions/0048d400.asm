
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d400 <rnd::Path::Impl::~Impl()>:
  48d400: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x48d42c <rnd::Path::Impl::~Impl()+0x2c>
  48d404: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x48d430 <rnd::Path::Impl::~Impl()+0x30>
  48d408: e92d4010     	push	{r4, lr}
  48d40c: e08f3003     	add	r3, pc, r3
  48d410: e7932002     	ldr	r2, [r3, r2]
  48d414: e1a04000     	mov	r4, r0
  48d418: e2822008     	add	r2, r2, #8
  48d41c: e5802000     	str	r2, [r0]
  48d420: ebffffa7     	bl	0x48d2c4 <rnd::Rule::Impl::~Impl()> @ imm = #-0x164
  48d424: e1a00004     	mov	r0, r4
  48d428: e8bd8010     	pop	{r4, pc}
  48d42c: 84 76 50 00  	.word	0x00507684
  48d430: c8 34 00 00  	.word	0x000034c8
