
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00472860 <VisualObject::SyncScaling()>:
  472860: e5901004     	ldr	r1, [r0, #0x4]
  472864: e3510000     	cmp	r1, #0
  472868: 012fff1e     	bxeq	lr
  47286c: e2811e12     	add	r1, r1, #288
  472870: eaffffcd     	b	0x4727ac <VisualObject::SetScaling(Point3D<float> const&)> @ imm = #-0xcc
