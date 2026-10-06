
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f03ec <Level::LoadFileData::~LoadFileData()>:
  3f03ec: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x3f0420 <Level::LoadFileData::~LoadFileData()+0x34>
  3f03f0: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x3f0424 <Level::LoadFileData::~LoadFileData()+0x38>
  3f03f4: e92d4010     	push	{r4, lr}
  3f03f8: e08f3003     	add	r3, pc, r3
  3f03fc: e7932002     	ldr	r2, [r3, r2]
  3f0400: e1a04000     	mov	r4, r0
  3f0404: e2822008     	add	r2, r2, #8
  3f0408: e4802008     	str	r2, [r0], #8
  3f040c: ebfc996c     	bl	0x3169c4 <StreamBuffer::~StreamBuffer()> @ imm = #-0xd9a50
  3f0410: e1a00004     	mov	r0, r4
  3f0414: ebfc8009     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xdffdc
  3f0418: e1a00004     	mov	r0, r4
  3f041c: e8bd8010     	pop	{r4, pc}
  3f0420: 98 46 5a 00  	.word	0x005a4698
  3f0424: 20 22 00 00  	.word	0x00002220
