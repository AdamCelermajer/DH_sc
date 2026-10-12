
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00330740 <DebugSwitches::GetInstance()>:
  330740: e59f301c     	ldr	r3, [pc, #0x1c]         @ 0x330764 <DebugSwitches::GetInstance()+0x24>
  330744: e59f201c     	ldr	r2, [pc, #0x1c]         @ 0x330768 <DebugSwitches::GetInstance()+0x28>
  330748: e92d4010     	push	{r4, lr}
  33074c: e08f3003     	add	r3, pc, r3
  330750: e7934002     	ldr	r4, [r3, r2]
  330754: e1a00004     	mov	r0, r4
  330758: eb001c4a     	bl	0x337888 <DebugSwitches::load()> @ imm = #0x7128
  33075c: e1a00004     	mov	r0, r4
  330760: e8bd8010     	pop	{r4, pc}
  330764: 44 43 66 00  	.word	0x00664344
  330768: 84 08 00 00  	.word	0x00000884

0033076c <std::string::operator=(char const*)>:
  33076c: e92d4070     	push	{r4, r5, r6, lr}
  330770: e1a04000     	mov	r4, r0
  330774: e1a00001     	mov	r0, r1
  330778: e1a05001     	mov	r5, r1
  33077c: ebff75b4     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x22930
  330780: e1a01005     	mov	r1, r5
  330784: e0852000     	add	r2, r5, r0
