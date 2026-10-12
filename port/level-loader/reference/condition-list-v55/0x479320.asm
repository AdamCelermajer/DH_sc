
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00479320 <Condition_IsQuestInState::~Condition_IsQuestInState()>:
  479320: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x479354 <Condition_IsQuestInState::~Condition_IsQuestInState()+0x34>
  479324: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x479358 <Condition_IsQuestInState::~Condition_IsQuestInState()+0x38>
  479328: e92d4010     	push	{r4, lr}
  47932c: e08f3003     	add	r3, pc, r3
  479330: e7932002     	ldr	r2, [r3, r2]
  479334: e1a04000     	mov	r4, r0
  479338: e2822008     	add	r2, r2, #8
  47933c: e5802000     	str	r2, [r0]
  479340: ebfffcd6     	bl	0x4786a0 <Condition::~Condition()> @ imm = #-0xca8
  479344: e1a00004     	mov	r0, r4
  479348: ebfa5c3c     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x168f10
  47934c: e1a00004     	mov	r0, r4
  479350: e8bd8010     	pop	{r4, pc}
  479354: 64 b7 51 00  	.word	0x0051b764
  479358: 0c 15 00 00  	.word	0x0000150c
