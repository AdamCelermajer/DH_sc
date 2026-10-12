
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004c7cc8 <Structs::v2ConditionStub::~v2ConditionStub()>:
  4c7cc8: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x4c7cf4 <Structs::v2ConditionStub::~v2ConditionStub()+0x2c>
  4c7ccc: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x4c7cf8 <Structs::v2ConditionStub::~v2ConditionStub()+0x30>
  4c7cd0: e92d4010     	push	{r4, lr}
  4c7cd4: e08f3003     	add	r3, pc, r3
  4c7cd8: e7932002     	ldr	r2, [r3, r2]
  4c7cdc: e1a04000     	mov	r4, r0
  4c7ce0: e2822008     	add	r2, r2, #8
  4c7ce4: e5802000     	str	r2, [r0]
  4c7ce8: ebfffff3     	bl	0x4c7cbc <Structs::v2Condition::~v2Condition()> @ imm = #-0x34
  4c7cec: e1a00004     	mov	r0, r4
  4c7cf0: e8bd8010     	pop	{r4, pc}
  4c7cf4: bc cd 4c 00  	.word	0x004ccdbc
  4c7cf8: 48 09 00 00  	.word	0x00000948
