
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d604 <rnd::Path::~Path()>:
  48d604: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x48d630 <rnd::Path::~Path()+0x2c>
  48d608: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x48d634 <rnd::Path::~Path()+0x30>
  48d60c: e92d4010     	push	{r4, lr}
  48d610: e08f3003     	add	r3, pc, r3
  48d614: e7932002     	ldr	r2, [r3, r2]
  48d618: e1a04000     	mov	r4, r0
  48d61c: e2822008     	add	r2, r2, #8
  48d620: e5802000     	str	r2, [r0]
  48d624: ebffffd8     	bl	0x48d58c <rnd::Rule::~Rule()> @ imm = #-0xa0
  48d628: e1a00004     	mov	r0, r4
  48d62c: e8bd8010     	pop	{r4, pc}
  48d630: 80 74 50 00  	.word	0x00507480
  48d634: 34 2c 00 00  	.word	0x00002c34
