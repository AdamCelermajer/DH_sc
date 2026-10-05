
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d550 <rnd::ForceBlock::Impl::~Impl()>:
  48d550: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x48d584 <rnd::ForceBlock::Impl::~Impl()+0x34>
  48d554: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x48d588 <rnd::ForceBlock::Impl::~Impl()+0x38>
  48d558: e92d4010     	push	{r4, lr}
  48d55c: e08f3003     	add	r3, pc, r3
  48d560: e7932002     	ldr	r2, [r3, r2]
  48d564: e1a04000     	mov	r4, r0
  48d568: e2822008     	add	r2, r2, #8
  48d56c: e5802000     	str	r2, [r0]
  48d570: ebffff53     	bl	0x48d2c4 <rnd::Rule::Impl::~Impl()> @ imm = #-0x2b4
  48d574: e1a00004     	mov	r0, r4
  48d578: ebfa0bb0     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17d140
  48d57c: e1a00004     	mov	r0, r4
  48d580: e8bd8010     	pop	{r4, pc}
  48d584: 34 75 50 00  	.word	0x00507534
  48d588: 08 4c 00 00  	.word	0x00004c08
