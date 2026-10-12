
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004792a8 <Condition_IsQuestStateHigher::~Condition_IsQuestStateHigher()>:
  4792a8: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x4792dc <Condition_IsQuestStateHigher::~Condition_IsQuestStateHigher()+0x34>
  4792ac: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x4792e0 <Condition_IsQuestStateHigher::~Condition_IsQuestStateHigher()+0x38>
  4792b0: e92d4010     	push	{r4, lr}
  4792b4: e08f3003     	add	r3, pc, r3
  4792b8: e7932002     	ldr	r2, [r3, r2]
  4792bc: e1a04000     	mov	r4, r0
  4792c0: e2822008     	add	r2, r2, #8
  4792c4: e5802000     	str	r2, [r0]
  4792c8: ebfffcf4     	bl	0x4786a0 <Condition::~Condition()> @ imm = #-0xc30
  4792cc: e1a00004     	mov	r0, r4
  4792d0: ebfa5c5a     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x168e98
  4792d4: e1a00004     	mov	r0, r4
  4792d8: e8bd8010     	pop	{r4, pc}
  4792dc: dc b7 51 00  	.word	0x0051b7dc
  4792e0: 0c 15 00 00  	.word	0x0000150c
