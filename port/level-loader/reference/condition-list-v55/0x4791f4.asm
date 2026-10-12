
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004791f4 <Condition_IsEventInState::~Condition_IsEventInState()>:
  4791f4: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x479228 <Condition_IsEventInState::~Condition_IsEventInState()+0x34>
  4791f8: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x47922c <Condition_IsEventInState::~Condition_IsEventInState()+0x38>
  4791fc: e92d4010     	push	{r4, lr}
  479200: e08f3003     	add	r3, pc, r3
  479204: e7932002     	ldr	r2, [r3, r2]
  479208: e1a04000     	mov	r4, r0
  47920c: e2822008     	add	r2, r2, #8
  479210: e5802000     	str	r2, [r0]
  479214: ebfffd21     	bl	0x4786a0 <Condition::~Condition()> @ imm = #-0xb7c
  479218: e1a00004     	mov	r0, r4
  47921c: ebfa5c87     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x168de4
  479220: e1a00004     	mov	r0, r4
  479224: e8bd8010     	pop	{r4, pc}
  479228: 90 b8 51 00  	.word	0x0051b890
  47922c: 64 0c 00 00  	.word	0x00000c64
