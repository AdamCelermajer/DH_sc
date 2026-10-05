
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00388f00 <Module::Deserialize(IStreamBase*)>:
  388f00: e92d4070     	push	{r4, r5, r6, lr}
  388f04: e1a04000     	mov	r4, r0
  388f08: e1a05001     	mov	r5, r1
  388f0c: eb000a82     	bl	0x38b91c <GameObject::Deserialize(IStreamBase*)> @ imm = #0x2a08
  388f10: e1a00005     	mov	r0, r5
  388f14: e2841fff     	add	r1, r4, #1020
  388f18: ebfed448     	bl	0x33e040 <void IStreamBase::readAs<bool>(bool&)> @ imm = #-0x4aee0
  388f1c: e2840b01     	add	r0, r4, #1024
  388f20: e3a01000     	mov	r1, #0
  388f24: ebfedba5     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0x4916c
  388f28: e2503000     	subs	r3, r0, #0
  388f2c: 1a000000     	bne	0x388f34 <Module::Deserialize(IStreamBase*)+0x34> @ imm = #0x0
  388f30: e8bd8070     	pop	{r4, r5, r6, pc}
  388f34: e59330f4     	ldr	r3, [r3, #0xf4]
  388f38: e353000b     	cmp	r3, #11
  388f3c: 1afffffb     	bne	0x388f30 <Module::Deserialize(IStreamBase*)+0x30> @ imm = #-0x14
  388f40: e5d413fc     	ldrb	r1, [r4, #0x3fc]
  388f44: e8bd4070     	pop	{r4, r5, r6, lr}
  388f48: ea00355b     	b	0x3964bc <RoomZone::SetVisited(bool)> @ imm = #0xd56c
