
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d770 <rnd::Path::~Path()>:
  48d770: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x48d7a4 <rnd::Path::~Path()+0x34>
  48d774: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x48d7a8 <rnd::Path::~Path()+0x38>
  48d778: e92d4010     	push	{r4, lr}
  48d77c: e08f3003     	add	r3, pc, r3
  48d780: e7932002     	ldr	r2, [r3, r2]
  48d784: e1a04000     	mov	r4, r0
  48d788: e2822008     	add	r2, r2, #8
  48d78c: e5802000     	str	r2, [r0]
  48d790: ebffff7d     	bl	0x48d58c <rnd::Rule::~Rule()> @ imm = #-0x20c
  48d794: e1a00004     	mov	r0, r4
  48d798: ebfa0b28     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17d360
  48d79c: e1a00004     	mov	r0, r4
  48d7a0: e8bd8010     	pop	{r4, pc}
  48d7a4: 14 73 50 00  	.word	0x00507314
  48d7a8: 34 2c 00 00  	.word	0x00002c34
