
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d514 <rnd::EndPath::Impl::~Impl()>:
  48d514: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x48d548 <rnd::EndPath::Impl::~Impl()+0x34>
  48d518: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x48d54c <rnd::EndPath::Impl::~Impl()+0x38>
  48d51c: e92d4010     	push	{r4, lr}
  48d520: e08f3003     	add	r3, pc, r3
  48d524: e7932002     	ldr	r2, [r3, r2]
  48d528: e1a04000     	mov	r4, r0
  48d52c: e2822008     	add	r2, r2, #8
  48d530: e5802000     	str	r2, [r0]
  48d534: ebffff62     	bl	0x48d2c4 <rnd::Rule::Impl::~Impl()> @ imm = #-0x278
  48d538: e1a00004     	mov	r0, r4
  48d53c: ebfa0bbf     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17d104
  48d540: e1a00004     	mov	r0, r4
  48d544: e8bd8010     	pop	{r4, pc}
  48d548: 70 75 50 00  	.word	0x00507570
  48d54c: 5c 37 00 00  	.word	0x0000375c
