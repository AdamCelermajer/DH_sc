
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0038ba74 <VisualObject::Sync()>:
  38ba74: e92d4010     	push	{r4, lr}
  38ba78: e1a04000     	mov	r4, r0
  38ba7c: eb03948d     	bl	0x470cb8 <VisualObject::SyncPosition()> @ imm = #0xe5234
  38ba80: e1a00004     	mov	r0, r4
  38ba84: eb039baf     	bl	0x472948 <VisualObject::SyncRotation()> @ imm = #0xe6ebc
  38ba88: e1a00004     	mov	r0, r4
  38ba8c: e8bd4010     	pop	{r4, lr}
  38ba90: ea039b72     	b	0x472860 <VisualObject::SyncScaling()> @ imm = #0xe6dc8
