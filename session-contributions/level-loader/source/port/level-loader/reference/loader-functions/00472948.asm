
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00472948 <VisualObject::SyncRotation()>:
  472948: e5901004     	ldr	r1, [r0, #0x4]
  47294c: e3510000     	cmp	r1, #0
  472950: 012fff1e     	bxeq	lr
  472954: e2811f5b     	add	r1, r1, #364
  472958: eaffffc5     	b	0x472874 <VisualObject::SetRotation(Point3D<float> const&)> @ imm = #-0xec
