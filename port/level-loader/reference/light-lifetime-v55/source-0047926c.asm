
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0047926c <Condition_IsPlayerAtLevel::~Condition_IsPlayerAtLevel()>:
  47926c: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x4792a0 <Condition_IsPlayerAtLevel::~Condition_IsPlayerAtLevel()+0x34>
  479270: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x4792a4 <Condition_IsPlayerAtLevel::~Condition_IsPlayerAtLevel()+0x38>
  479274: e92d4010     	push	{r4, lr}
  479278: e08f3003     	add	r3, pc, r3
  47927c: e7932002     	ldr	r2, [r3, r2]
  479280: e1a04000     	mov	r4, r0
  479284: e2822008     	add	r2, r2, #8
  479288: e5802000     	str	r2, [r0]
  47928c: ebfffd03     	bl	0x4786a0 <Condition::~Condition()> @ imm = #-0xbf4
  479290: e1a00004     	mov	r0, r4
  479294: ebfa5c69     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x168e5c
  479298: e1a00004     	mov	r0, r4
  47929c: e8bd8010     	pop	{r4, pc}
  4792a0: 18 b8 51 00  	.word	0x0051b818
  4792a4: dc 39 00 00  	.word	0x000039dc
