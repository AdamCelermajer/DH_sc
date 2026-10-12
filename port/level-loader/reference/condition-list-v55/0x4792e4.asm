
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004792e4 <Condition_IsQuestStateLower::~Condition_IsQuestStateLower()>:
  4792e4: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x479318 <Condition_IsQuestStateLower::~Condition_IsQuestStateLower()+0x34>
  4792e8: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x47931c <Condition_IsQuestStateLower::~Condition_IsQuestStateLower()+0x38>
  4792ec: e92d4010     	push	{r4, lr}
  4792f0: e08f3003     	add	r3, pc, r3
  4792f4: e7932002     	ldr	r2, [r3, r2]
  4792f8: e1a04000     	mov	r4, r0
  4792fc: e2822008     	add	r2, r2, #8
  479300: e5802000     	str	r2, [r0]
  479304: ebfffce5     	bl	0x4786a0 <Condition::~Condition()> @ imm = #-0xc6c
  479308: e1a00004     	mov	r0, r4
  47930c: ebfa5c4b     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x168ed4
  479310: e1a00004     	mov	r0, r4
  479314: e8bd8010     	pop	{r4, pc}
  479318: a0 b7 51 00  	.word	0x0051b7a0
  47931c: 0c 15 00 00  	.word	0x0000150c
