
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00478810 <Condition_IsPlayerAtLevel::~Condition_IsPlayerAtLevel()>:
  478810: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x47883c <Condition_IsPlayerAtLevel::~Condition_IsPlayerAtLevel()+0x2c>
  478814: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x478840 <Condition_IsPlayerAtLevel::~Condition_IsPlayerAtLevel()+0x30>
  478818: e92d4010     	push	{r4, lr}
  47881c: e08f3003     	add	r3, pc, r3
  478820: e7932002     	ldr	r2, [r3, r2]
  478824: e1a04000     	mov	r4, r0
  478828: e2822008     	add	r2, r2, #8
  47882c: e5802000     	str	r2, [r0]
  478830: ebffff9a     	bl	0x4786a0 <Condition::~Condition()> @ imm = #-0x198
  478834: e1a00004     	mov	r0, r4
  478838: e8bd8010     	pop	{r4, pc}
  47883c: 74 c2 51 00  	.word	0x0051c274
  478840: dc 39 00 00  	.word	0x000039dc
