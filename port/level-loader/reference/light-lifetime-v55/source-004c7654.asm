
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004c7654 <Structs::RestartLevel::~RestartLevel()>:
  4c7654: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x4c7680 <Structs::RestartLevel::~RestartLevel()+0x2c>
  4c7658: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x4c7684 <Structs::RestartLevel::~RestartLevel()+0x30>
  4c765c: e92d4010     	push	{r4, lr}
  4c7660: e08f3003     	add	r3, pc, r3
  4c7664: e7932002     	ldr	r2, [r3, r2]
  4c7668: e1a04000     	mov	r4, r0
  4c766c: e2822008     	add	r2, r2, #8
  4c7670: e5802000     	str	r2, [r0]
  4c7674: ebfffd79     	bl	0x4c6c60 <Structs::ScriptCmd::~ScriptCmd()> @ imm = #-0xa1c
  4c7678: e1a00004     	mov	r0, r4
  4c767c: e8bd8010     	pop	{r4, pc}
  4c7680: 30 d4 4c 00  	.word	0x004cd430
  4c7684: 08 4a 00 00  	.word	0x00004a08
