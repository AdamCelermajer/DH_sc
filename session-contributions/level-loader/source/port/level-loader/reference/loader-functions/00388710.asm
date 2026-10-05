
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00388710 <Module::Serialize(IStreamBase*)>:
  388710: e92d4070     	push	{r4, r5, r6, lr}
  388714: e1a04000     	mov	r4, r0
  388718: e1a05001     	mov	r5, r1
  38871c: eb000cb0     	bl	0x38b9e4 <GameObject::Serialize(IStreamBase*)> @ imm = #0x32c0
  388720: e1a00005     	mov	r0, r5
  388724: e2841fff     	add	r1, r4, #1020
  388728: e8bd4070     	pop	{r4, r5, r6, lr}
  38872c: eafed681     	b	0x33e138 <void IStreamBase::writeAs<bool>(bool const&)> @ imm = #-0x4a5fc
