
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d6f8 <rnd::RootRule::~RootRule()>:
  48d6f8: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x48d72c <rnd::RootRule::~RootRule()+0x34>
  48d6fc: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x48d730 <rnd::RootRule::~RootRule()+0x38>
  48d700: e92d4010     	push	{r4, lr}
  48d704: e08f3003     	add	r3, pc, r3
  48d708: e7932002     	ldr	r2, [r3, r2]
  48d70c: e1a04000     	mov	r4, r0
  48d710: e2822008     	add	r2, r2, #8
  48d714: e5802000     	str	r2, [r0]
  48d718: ebfff978     	bl	0x48bd00 <rnd::RootRule::Unload()> @ imm = #-0x1a20
  48d71c: e1a00004     	mov	r0, r4
  48d720: ebffff99     	bl	0x48d58c <rnd::Rule::~Rule()> @ imm = #-0x19c
  48d724: e1a00004     	mov	r0, r4
  48d728: e8bd8010     	pop	{r4, pc}
  48d72c: 8c 73 50 00  	.word	0x0050738c
  48d730: a8 48 00 00  	.word	0x000048a8
