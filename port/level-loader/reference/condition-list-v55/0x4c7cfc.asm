
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004c7cfc <Structs::v2ConditionStub::~v2ConditionStub()>:
  4c7cfc: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x4c7d28 <Structs::v2ConditionStub::~v2ConditionStub()+0x2c>
  4c7d00: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x4c7d2c <Structs::v2ConditionStub::~v2ConditionStub()+0x30>
  4c7d04: e92d4010     	push	{r4, lr}
  4c7d08: e08f3003     	add	r3, pc, r3
  4c7d0c: e7932002     	ldr	r2, [r3, r2]
  4c7d10: e1a04000     	mov	r4, r0
  4c7d14: e2822008     	add	r2, r2, #8
  4c7d18: e5802000     	str	r2, [r0]
  4c7d1c: ebffffe6     	bl	0x4c7cbc <Structs::v2Condition::~v2Condition()> @ imm = #-0x68
  4c7d20: e1a00004     	mov	r0, r4
  4c7d24: e8bd8010     	pop	{r4, pc}
  4c7d28: 88 cd 4c 00  	.word	0x004ccd88
  4c7d2c: 48 09 00 00  	.word	0x00000948
