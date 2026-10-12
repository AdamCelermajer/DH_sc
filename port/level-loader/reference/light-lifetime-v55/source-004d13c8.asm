
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004d13c8 <Structs::ChangeLevel::~ChangeLevel()>:
  4d13c8: e92d4010     	push	{r4, lr}
  4d13cc: e59f3034     	ldr	r3, [pc, #0x34]         @ 0x4d1408 <Structs::ChangeLevel::~ChangeLevel()+0x40>
  4d13d0: e59f2034     	ldr	r2, [pc, #0x34]         @ 0x4d140c <Structs::ChangeLevel::~ChangeLevel()+0x44>
  4d13d4: e1a04000     	mov	r4, r0
  4d13d8: e08f3003     	add	r3, pc, r3
  4d13dc: e5900010     	ldr	r0, [r0, #0x10]
  4d13e0: e7932002     	ldr	r2, [r3, r2]
  4d13e4: e3500000     	cmp	r0, #0
  4d13e8: e2822008     	add	r2, r2, #8
  4d13ec: e5842000     	str	r2, [r4]
  4d13f0: 0a000000     	beq	0x4d13f8 <Structs::ChangeLevel::~ChangeLevel()+0x30> @ imm = #0x0
  4d13f4: ebf8fc11     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1c0fbc
  4d13f8: e1a00004     	mov	r0, r4
  4d13fc: ebffd617     	bl	0x4c6c60 <Structs::ScriptCmd::~ScriptCmd()> @ imm = #-0xa7a4
  4d1400: e1a00004     	mov	r0, r4
  4d1404: e8bd8010     	pop	{r4, pc}
  4d1408: b8 36 4c 00  	.word	0x004c36b8
  4d140c: 00 0b 00 00  	.word	0x00000b00
