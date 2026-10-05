
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d4d8 <rnd::RootRule::Impl::~Impl()>:
  48d4d8: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x48d50c <rnd::RootRule::Impl::~Impl()+0x34>
  48d4dc: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x48d510 <rnd::RootRule::Impl::~Impl()+0x38>
  48d4e0: e92d4010     	push	{r4, lr}
  48d4e4: e08f3003     	add	r3, pc, r3
  48d4e8: e7932002     	ldr	r2, [r3, r2]
  48d4ec: e1a04000     	mov	r4, r0
  48d4f0: e2822008     	add	r2, r2, #8
  48d4f4: e5802000     	str	r2, [r0]
  48d4f8: ebffff71     	bl	0x48d2c4 <rnd::Rule::Impl::~Impl()> @ imm = #-0x23c
  48d4fc: e1a00004     	mov	r0, r4
  48d500: ebfa0bce     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17d0c8
  48d504: e1a00004     	mov	r0, r4
  48d508: e8bd8010     	pop	{r4, pc}
  48d50c: ac 75 50 00  	.word	0x005075ac
  48d510: ac 07 00 00  	.word	0x000007ac
