
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004787dc <Condition_IsPlayerInLevel::~Condition_IsPlayerInLevel()>:
  4787dc: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x478808 <Condition_IsPlayerInLevel::~Condition_IsPlayerInLevel()+0x2c>
  4787e0: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x47880c <Condition_IsPlayerInLevel::~Condition_IsPlayerInLevel()+0x30>
  4787e4: e92d4010     	push	{r4, lr}
  4787e8: e08f3003     	add	r3, pc, r3
  4787ec: e7932002     	ldr	r2, [r3, r2]
  4787f0: e1a04000     	mov	r4, r0
  4787f4: e2822008     	add	r2, r2, #8
  4787f8: e5802000     	str	r2, [r0]
  4787fc: ebffffa7     	bl	0x4786a0 <Condition::~Condition()> @ imm = #-0x164
  478800: e1a00004     	mov	r0, r4
  478804: e8bd8010     	pop	{r4, pc}
  478808: a8 c2 51 00  	.word	0x0051c2a8
  47880c: 68 1b 00 00  	.word	0x00001b68
