
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00513830 <PropertyMap::SetTemplateParameter(char const*, char const*)>:
  513830: e92d4010     	push	{r4, lr}
  513834: e1a04002     	mov	r4, r2
  513838: ebfffff2     	bl	0x513808 <PropertyMap::GetProp(char const*)> @ imm = #-0x38
  51383c: e2503000     	subs	r3, r0, #0
  513840: 0a000003     	beq	0x513854 <PropertyMap::SetTemplateParameter(char const*, char const*)+0x24> @ imm = #0xc
  513844: e5933000     	ldr	r3, [r3]
  513848: e1a01004     	mov	r1, r4
  51384c: e1a0e00f     	mov	lr, pc
  513850: e593f010     	ldr	pc, [r3, #0x10]
  513854: e8bd8010     	pop	{r4, pc}
