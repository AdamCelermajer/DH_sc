
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004c73cc <Structs::IncFaeryLevel::~IncFaeryLevel()>:
  4c73cc: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x4c73f8 <Structs::IncFaeryLevel::~IncFaeryLevel()+0x2c>
  4c73d0: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x4c73fc <Structs::IncFaeryLevel::~IncFaeryLevel()+0x30>
  4c73d4: e92d4010     	push	{r4, lr}
  4c73d8: e08f3003     	add	r3, pc, r3
  4c73dc: e7932002     	ldr	r2, [r3, r2]
  4c73e0: e1a04000     	mov	r4, r0
  4c73e4: e2822008     	add	r2, r2, #8
  4c73e8: e5802000     	str	r2, [r0]
  4c73ec: ebfffe1b     	bl	0x4c6c60 <Structs::ScriptCmd::~ScriptCmd()> @ imm = #-0x794
  4c73f0: e1a00004     	mov	r0, r4
  4c73f4: e8bd8010     	pop	{r4, pc}
  4c73f8: b8 d6 4c 00  	.word	0x004cd6b8
  4c73fc: c0 1e 00 00  	.word	0x00001ec0
