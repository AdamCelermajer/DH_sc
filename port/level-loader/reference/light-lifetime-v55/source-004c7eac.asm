
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004c7eac <Structs::v2IsPlayerInLevel::~v2IsPlayerInLevel()>:
  4c7eac: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x4c7ed8 <Structs::v2IsPlayerInLevel::~v2IsPlayerInLevel()+0x2c>
  4c7eb0: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x4c7edc <Structs::v2IsPlayerInLevel::~v2IsPlayerInLevel()+0x30>
  4c7eb4: e92d4010     	push	{r4, lr}
  4c7eb8: e08f3003     	add	r3, pc, r3
  4c7ebc: e7932002     	ldr	r2, [r3, r2]
  4c7ec0: e1a04000     	mov	r4, r0
  4c7ec4: e2822008     	add	r2, r2, #8
  4c7ec8: e5802000     	str	r2, [r0]
  4c7ecc: ebffff7a     	bl	0x4c7cbc <Structs::v2Condition::~v2Condition()> @ imm = #-0x218
  4c7ed0: e1a00004     	mov	r0, r4
  4c7ed4: e8bd8010     	pop	{r4, pc}
  4c7ed8: d8 cb 4c 00  	.word	0x004ccbd8
  4c7edc: b8 08 00 00  	.word	0x000008b8
