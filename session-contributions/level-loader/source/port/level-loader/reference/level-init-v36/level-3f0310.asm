
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f0310 <Level::LoadFileData::~LoadFileData()>:
  3f0310: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x3f033c <Level::LoadFileData::~LoadFileData()+0x2c>
  3f0314: e59f2024     	ldr	r2, [pc, #0x24]         @ 0x3f0340 <Level::LoadFileData::~LoadFileData()+0x30>
  3f0318: e92d4010     	push	{r4, lr}
  3f031c: e08f3003     	add	r3, pc, r3
  3f0320: e7932002     	ldr	r2, [r3, r2]
  3f0324: e1a04000     	mov	r4, r0
  3f0328: e2822008     	add	r2, r2, #8
  3f032c: e4802008     	str	r2, [r0], #8
  3f0330: ebfc99a3     	bl	0x3169c4 <StreamBuffer::~StreamBuffer()> @ imm = #-0xd9974
  3f0334: e1a00004     	mov	r0, r4
  3f0338: e8bd8010     	pop	{r4, pc}
  3f033c: 74 47 5a 00  	.word	0x005a4774
  3f0340: 20 22 00 00  	.word	0x00002220
