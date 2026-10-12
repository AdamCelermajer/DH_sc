
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004c7e78 <Structs::v2IsPlayerInLevel::~v2IsPlayerInLevel()>:
  4c7e78: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x4c7ea4 <Structs::v2IsPlayerInLevel::~v2IsPlayerInLevel()+0x2c>
  4c7e7c: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x4c7ea8 <Structs::v2IsPlayerInLevel::~v2IsPlayerInLevel()+0x30>
  4c7e80: e92d4010     	push	{r4, lr}
  4c7e84: e08f3003     	add	r3, pc, r3
  4c7e88: e7932002     	ldr	r2, [r3, r2]
  4c7e8c: e1a04000     	mov	r4, r0
  4c7e90: e2822008     	add	r2, r2, #8
  4c7e94: e5802000     	str	r2, [r0]
  4c7e98: ebffff87     	bl	0x4c7cbc <Structs::v2Condition::~v2Condition()> @ imm = #-0x1e4
  4c7e9c: e1a00004     	mov	r0, r4
  4c7ea0: e8bd8010     	pop	{r4, pc}
  4c7ea4: 0c cc 4c 00  	.word	0x004ccc0c
  4c7ea8: b8 08 00 00  	.word	0x000008b8
