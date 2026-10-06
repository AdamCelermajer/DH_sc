
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003a3094 <Character::IsFaerie() const>:
  3a3094: e92d4010     	push	{r4, lr}
  3a3098: ebffffed     	bl	0x3a3054 <Character::GetCharType() const> @ imm = #-0x4c
  3a309c: e3500003     	cmp	r0, #3
  3a30a0: 13a00000     	movne	r0, #0
  3a30a4: 03a00001     	moveq	r0, #1
  3a30a8: e8bd8010     	pop	{r4, pc}
