
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00385fbc <GSLevel::~GSLevel()>:
  385fbc: e92d4010     	push	{r4, lr}
  385fc0: e1a04000     	mov	r4, r0
  385fc4: ebffffef     	bl	0x385f88 <GSLevel::~GSLevel()> @ imm = #-0x44
  385fc8: e1a00004     	mov	r0, r4
  385fcc: ebfe291b     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x75b94
  385fd0: e1a00004     	mov	r0, r4
  385fd4: e8bd8010     	pop	{r4, pc}
