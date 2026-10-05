
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048a03c <rnd::MgxBlock::~MgxBlock()>:
  48a03c: e92d4010     	push	{r4, lr}
  48a040: e59f3048     	ldr	r3, [pc, #0x48]         @ 0x48a090 <rnd::MgxBlock::~MgxBlock()+0x54>
  48a044: e59f2048     	ldr	r2, [pc, #0x48]         @ 0x48a094 <rnd::MgxBlock::~MgxBlock()+0x58>
  48a048: e59019c4     	ldr	r1, [r0, #0x9c4]
  48a04c: e08f3003     	add	r3, pc, r3
  48a050: e7932002     	ldr	r2, [r3, r2]
  48a054: e3510000     	cmp	r1, #0
  48a058: e1a04000     	mov	r4, r0
  48a05c: e2822008     	add	r2, r2, #8
  48a060: e5802000     	str	r2, [r0]
  48a064: 0a000003     	beq	0x48a078 <rnd::MgxBlock::~MgxBlock()+0x3c> @ imm = #0xc
  48a068: e1a00001     	mov	r0, r1
  48a06c: e5913000     	ldr	r3, [r1]
  48a070: e1a0e00f     	mov	lr, pc
  48a074: e593f004     	ldr	pc, [r3, #0x4]
  48a078: e59409c0     	ldr	r0, [r4, #0x9c0]
  48a07c: ebfa0f9b     	bl	0x30def0 <.plt+0x17c>   @ imm = #-0x17c194
  48a080: e1a00004     	mov	r0, r4
  48a084: ebffffd3     	bl	0x489fd8 <rnd::Block::~Block()> @ imm = #-0xb4
  48a088: e1a00004     	mov	r0, r4
  48a08c: e8bd8010     	pop	{r4, pc}
  48a090: 44 aa 50 00  	.word	0x0050aa44
  48a094: 78 19 00 00  	.word	0x00001978
