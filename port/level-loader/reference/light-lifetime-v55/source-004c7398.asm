
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004c7398 <Structs::IncFaeryLevel::~IncFaeryLevel()>:
  4c7398: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x4c73c4 <Structs::IncFaeryLevel::~IncFaeryLevel()+0x2c>
  4c739c: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x4c73c8 <Structs::IncFaeryLevel::~IncFaeryLevel()+0x30>
  4c73a0: e92d4010     	push	{r4, lr}
  4c73a4: e08f3003     	add	r3, pc, r3
  4c73a8: e7932002     	ldr	r2, [r3, r2]
  4c73ac: e1a04000     	mov	r4, r0
  4c73b0: e2822008     	add	r2, r2, #8
  4c73b4: e5802000     	str	r2, [r0]
  4c73b8: ebfffe28     	bl	0x4c6c60 <Structs::ScriptCmd::~ScriptCmd()> @ imm = #-0x760
  4c73bc: e1a00004     	mov	r0, r4
  4c73c0: e8bd8010     	pop	{r4, pc}
  4c73c4: ec d6 4c 00  	.word	0x004cd6ec
  4c73c8: c0 1e 00 00  	.word	0x00001ec0
