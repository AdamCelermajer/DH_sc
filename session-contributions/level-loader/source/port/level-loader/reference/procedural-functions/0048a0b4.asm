
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048a0b4 <rnd::MgxBlock::~MgxBlock()>:
  48a0b4: e92d4010     	push	{r4, lr}
  48a0b8: e59f3048     	ldr	r3, [pc, #0x48]         @ 0x48a108 <rnd::MgxBlock::~MgxBlock()+0x54>
  48a0bc: e59f2048     	ldr	r2, [pc, #0x48]         @ 0x48a10c <rnd::MgxBlock::~MgxBlock()+0x58>
  48a0c0: e59019c4     	ldr	r1, [r0, #0x9c4]
  48a0c4: e08f3003     	add	r3, pc, r3
  48a0c8: e7932002     	ldr	r2, [r3, r2]
  48a0cc: e3510000     	cmp	r1, #0
  48a0d0: e1a04000     	mov	r4, r0
  48a0d4: e2822008     	add	r2, r2, #8
  48a0d8: e5802000     	str	r2, [r0]
  48a0dc: 0a000003     	beq	0x48a0f0 <rnd::MgxBlock::~MgxBlock()+0x3c> @ imm = #0xc
  48a0e0: e1a00001     	mov	r0, r1
  48a0e4: e5913000     	ldr	r3, [r1]
  48a0e8: e1a0e00f     	mov	lr, pc
  48a0ec: e593f004     	ldr	pc, [r3, #0x4]
  48a0f0: e59409c0     	ldr	r0, [r4, #0x9c0]
  48a0f4: ebfa0f7d     	bl	0x30def0 <.plt+0x17c>   @ imm = #-0x17c20c
  48a0f8: e1a00004     	mov	r0, r4
  48a0fc: ebffffb5     	bl	0x489fd8 <rnd::Block::~Block()> @ imm = #-0x12c
  48a100: e1a00004     	mov	r0, r4
  48a104: e8bd8010     	pop	{r4, pc}
  48a108: cc a9 50 00  	.word	0x0050a9cc
  48a10c: 78 19 00 00  	.word	0x00001978
