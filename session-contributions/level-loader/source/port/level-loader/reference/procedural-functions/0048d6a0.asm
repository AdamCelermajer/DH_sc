
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d6a0 <rnd::RootRule::~RootRule()>:
  48d6a0: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x48d6d4 <rnd::RootRule::~RootRule()+0x34>
  48d6a4: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x48d6d8 <rnd::RootRule::~RootRule()+0x38>
  48d6a8: e92d4010     	push	{r4, lr}
  48d6ac: e08f3003     	add	r3, pc, r3
  48d6b0: e7932002     	ldr	r2, [r3, r2]
  48d6b4: e1a04000     	mov	r4, r0
  48d6b8: e2822008     	add	r2, r2, #8
  48d6bc: e5802000     	str	r2, [r0]
  48d6c0: ebfff98e     	bl	0x48bd00 <rnd::RootRule::Unload()> @ imm = #-0x19c8
  48d6c4: e1a00004     	mov	r0, r4
  48d6c8: ebffffaf     	bl	0x48d58c <rnd::Rule::~Rule()> @ imm = #-0x144
  48d6cc: e1a00004     	mov	r0, r4
  48d6d0: e8bd8010     	pop	{r4, pc}
  48d6d4: e4 73 50 00  	.word	0x005073e4
  48d6d8: a8 48 00 00  	.word	0x000048a8
