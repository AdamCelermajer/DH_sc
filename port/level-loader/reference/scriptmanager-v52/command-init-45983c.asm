
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0045983c <Script_ShowFlash::Init()>:
  45983c: e92d4070     	push	{r4, r5, r6, lr}
  459840: e1a04000     	mov	r4, r0
  459844: ebff4c90     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #-0x2cdc0
  459848: e594300c     	ldr	r3, [r4, #0xc]
  45984c: e59f1038     	ldr	r1, [pc, #0x38]         @ 0x45988c <Script_ShowFlash::Init()+0x50>
  459850: e1a06000     	mov	r6, r0
  459854: e5935010     	ldr	r5, [r3, #0x10]
  459858: e08f1001     	add	r1, pc, r1
  45985c: e1a00005     	mov	r0, r5
  459860: ebfad4db     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x14ac94
  459864: e3500000     	cmp	r0, #0
  459868: 0a000002     	beq	0x459878 <Script_ShowFlash::Init()+0x3c> @ imm = #0x8
  45986c: e3a03000     	mov	r3, #0
  459870: e5843010     	str	r3, [r4, #0x10]
  459874: e8bd8070     	pop	{r4, r5, r6, pc}
  459878: e1a00006     	mov	r0, r6
  45987c: e1a01005     	mov	r1, r5
  459880: ebff4e5a     	bl	0x42d1f0 <MenuManager::GetMenuByName(char const*)> @ imm = #-0x2c698
  459884: e5840010     	str	r0, [r4, #0x10]
  459888: e8bd8070     	pop	{r4, r5, r6, pc}
  45988c: b0 36 47 00  	.word	0x004736b0
