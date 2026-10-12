
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0047917c <Condition_IsLevelInState::~Condition_IsLevelInState()>:
  47917c: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x4791b0 <Condition_IsLevelInState::~Condition_IsLevelInState()+0x34>
  479180: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x4791b4 <Condition_IsLevelInState::~Condition_IsLevelInState()+0x38>
  479184: e92d4010     	push	{r4, lr}
  479188: e08f3003     	add	r3, pc, r3
  47918c: e7932002     	ldr	r2, [r3, r2]
  479190: e1a04000     	mov	r4, r0
  479194: e2822008     	add	r2, r2, #8
  479198: e5802000     	str	r2, [r0]
  47919c: ebfffd3f     	bl	0x4786a0 <Condition::~Condition()> @ imm = #-0xb04
  4791a0: e1a00004     	mov	r0, r4
  4791a4: ebfa5ca5     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x168d6c
  4791a8: e1a00004     	mov	r0, r4
  4791ac: e8bd8010     	pop	{r4, pc}
  4791b0: 08 b9 51 00  	.word	0x0051b908
  4791b4: 1c 36 00 00  	.word	0x0000361c
