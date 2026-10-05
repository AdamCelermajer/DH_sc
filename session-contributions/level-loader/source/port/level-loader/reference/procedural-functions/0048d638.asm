
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d638 <rnd::ForceBlock::~ForceBlock()>:
  48d638: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x48d664 <rnd::ForceBlock::~ForceBlock()+0x2c>
  48d63c: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x48d668 <rnd::ForceBlock::~ForceBlock()+0x30>
  48d640: e92d4010     	push	{r4, lr}
  48d644: e08f3003     	add	r3, pc, r3
  48d648: e7932002     	ldr	r2, [r3, r2]
  48d64c: e1a04000     	mov	r4, r0
  48d650: e2822008     	add	r2, r2, #8
  48d654: e5802000     	str	r2, [r0]
  48d658: ebffffcb     	bl	0x48d58c <rnd::Rule::~Rule()> @ imm = #-0xd4
  48d65c: e1a00004     	mov	r0, r4
  48d660: e8bd8010     	pop	{r4, pc}
  48d664: 4c 74 50 00  	.word	0x0050744c
  48d668: 0c 09 00 00  	.word	0x0000090c
