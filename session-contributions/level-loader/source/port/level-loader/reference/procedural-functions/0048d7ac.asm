
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d7ac <rnd::EndPath::~EndPath()>:
  48d7ac: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x48d7e0 <rnd::EndPath::~EndPath()+0x34>
  48d7b0: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x48d7e4 <rnd::EndPath::~EndPath()+0x38>
  48d7b4: e92d4010     	push	{r4, lr}
  48d7b8: e08f3003     	add	r3, pc, r3
  48d7bc: e7932002     	ldr	r2, [r3, r2]
  48d7c0: e1a04000     	mov	r4, r0
  48d7c4: e2822008     	add	r2, r2, #8
  48d7c8: e5802000     	str	r2, [r0]
  48d7cc: ebffff6e     	bl	0x48d58c <rnd::Rule::~Rule()> @ imm = #-0x248
  48d7d0: e1a00004     	mov	r0, r4
  48d7d4: ebfa0b19     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17d39c
  48d7d8: e1a00004     	mov	r0, r4
  48d7dc: e8bd8010     	pop	{r4, pc}
  48d7e0: d8 72 50 00  	.word	0x005072d8
  48d7e4: f4 2e 00 00  	.word	0x00002ef4
