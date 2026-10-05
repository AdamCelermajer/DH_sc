
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d734 <rnd::ForceBlock::~ForceBlock()>:
  48d734: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x48d768 <rnd::ForceBlock::~ForceBlock()+0x34>
  48d738: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x48d76c <rnd::ForceBlock::~ForceBlock()+0x38>
  48d73c: e92d4010     	push	{r4, lr}
  48d740: e08f3003     	add	r3, pc, r3
  48d744: e7932002     	ldr	r2, [r3, r2]
  48d748: e1a04000     	mov	r4, r0
  48d74c: e2822008     	add	r2, r2, #8
  48d750: e5802000     	str	r2, [r0]
  48d754: ebffff8c     	bl	0x48d58c <rnd::Rule::~Rule()> @ imm = #-0x1d0
  48d758: e1a00004     	mov	r0, r4
  48d75c: ebfa0b37     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17d324
  48d760: e1a00004     	mov	r0, r4
  48d764: e8bd8010     	pop	{r4, pc}
  48d768: 50 73 50 00  	.word	0x00507350
  48d76c: 0c 09 00 00  	.word	0x0000090c
