
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004c7620 <Structs::RestartLevel::~RestartLevel()>:
  4c7620: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x4c764c <Structs::RestartLevel::~RestartLevel()+0x2c>
  4c7624: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x4c7650 <Structs::RestartLevel::~RestartLevel()+0x30>
  4c7628: e92d4010     	push	{r4, lr}
  4c762c: e08f3003     	add	r3, pc, r3
  4c7630: e7932002     	ldr	r2, [r3, r2]
  4c7634: e1a04000     	mov	r4, r0
  4c7638: e2822008     	add	r2, r2, #8
  4c763c: e5802000     	str	r2, [r0]
  4c7640: ebfffd86     	bl	0x4c6c60 <Structs::ScriptCmd::~ScriptCmd()> @ imm = #-0x9e8
  4c7644: e1a00004     	mov	r0, r4
  4c7648: e8bd8010     	pop	{r4, pc}
  4c764c: 64 d4 4c 00  	.word	0x004cd464
  4c7650: 08 4a 00 00  	.word	0x00004a08
