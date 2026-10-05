
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d49c <rnd::Path::Impl::~Impl()>:
  48d49c: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x48d4d0 <rnd::Path::Impl::~Impl()+0x34>
  48d4a0: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x48d4d4 <rnd::Path::Impl::~Impl()+0x38>
  48d4a4: e92d4010     	push	{r4, lr}
  48d4a8: e08f3003     	add	r3, pc, r3
  48d4ac: e7932002     	ldr	r2, [r3, r2]
  48d4b0: e1a04000     	mov	r4, r0
  48d4b4: e2822008     	add	r2, r2, #8
  48d4b8: e5802000     	str	r2, [r0]
  48d4bc: ebffff80     	bl	0x48d2c4 <rnd::Rule::Impl::~Impl()> @ imm = #-0x200
  48d4c0: e1a00004     	mov	r0, r4
  48d4c4: ebfa0bdd     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17d08c
  48d4c8: e1a00004     	mov	r0, r4
  48d4cc: e8bd8010     	pop	{r4, pc}
  48d4d0: e8 75 50 00  	.word	0x005075e8
  48d4d4: c8 34 00 00  	.word	0x000034c8
