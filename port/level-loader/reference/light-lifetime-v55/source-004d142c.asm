
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004d142c <Structs::ChangeLevel::~ChangeLevel()>:
  4d142c: e92d4010     	push	{r4, lr}
  4d1430: e59f3034     	ldr	r3, [pc, #0x34]         @ 0x4d146c <Structs::ChangeLevel::~ChangeLevel()+0x40>
  4d1434: e59f2034     	ldr	r2, [pc, #0x34]         @ 0x4d1470 <Structs::ChangeLevel::~ChangeLevel()+0x44>
  4d1438: e1a04000     	mov	r4, r0
  4d143c: e08f3003     	add	r3, pc, r3
  4d1440: e5900010     	ldr	r0, [r0, #0x10]
  4d1444: e7932002     	ldr	r2, [r3, r2]
  4d1448: e3500000     	cmp	r0, #0
  4d144c: e2822008     	add	r2, r2, #8
  4d1450: e5842000     	str	r2, [r4]
  4d1454: 0a000000     	beq	0x4d145c <Structs::ChangeLevel::~ChangeLevel()+0x30> @ imm = #0x0
  4d1458: ebf8fbf8     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1c1020
  4d145c: e1a00004     	mov	r0, r4
  4d1460: ebffd5fe     	bl	0x4c6c60 <Structs::ScriptCmd::~ScriptCmd()> @ imm = #-0xa808
  4d1464: e1a00004     	mov	r0, r4
  4d1468: e8bd8010     	pop	{r4, pc}
  4d146c: 54 36 4c 00  	.word	0x004c3654
  4d1470: 00 0b 00 00  	.word	0x00000b00
