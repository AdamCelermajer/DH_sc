
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00340800 <ObjectBase* GetNewInstance<Character>()>:
  340800: e92d4010     	push	{r4, lr}
  340804: e3a01000     	mov	r1, #0
  340808: e3010f90     	movw	r0, #0x1f90
  34080c: ebff3f57     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x302a4
  340810: e3a01000     	mov	r1, #0
  340814: e1a04000     	mov	r4, r0
  340818: eb01a665     	bl	0x3aa1b4 <Character::Character(ObjectBase::GO_IDS)> @ imm = #0x69994
  34081c: e1a00004     	mov	r0, r4
  340820: e8bd8010     	pop	{r4, pc}
