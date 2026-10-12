
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00479230 <Condition_IsPlayerInLevel::~Condition_IsPlayerInLevel()>:
  479230: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x479264 <Condition_IsPlayerInLevel::~Condition_IsPlayerInLevel()+0x34>
  479234: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x479268 <Condition_IsPlayerInLevel::~Condition_IsPlayerInLevel()+0x38>
  479238: e92d4010     	push	{r4, lr}
  47923c: e08f3003     	add	r3, pc, r3
  479240: e7932002     	ldr	r2, [r3, r2]
  479244: e1a04000     	mov	r4, r0
  479248: e2822008     	add	r2, r2, #8
  47924c: e5802000     	str	r2, [r0]
  479250: ebfffd12     	bl	0x4786a0 <Condition::~Condition()> @ imm = #-0xbb8
  479254: e1a00004     	mov	r0, r4
  479258: ebfa5c78     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x168e20
  47925c: e1a00004     	mov	r0, r4
  479260: e8bd8010     	pop	{r4, pc}
  479264: 54 b8 51 00  	.word	0x0051b854
  479268: 68 1b 00 00  	.word	0x00001b68
