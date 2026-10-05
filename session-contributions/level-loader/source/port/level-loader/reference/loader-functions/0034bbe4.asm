
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034bbe4 <ObjectManager::LoadFromXML(TiXmlElement*, char const*)>:
  34bbe4: e52de004     	str	lr, [sp, #-0x4]!
  34bbe8: e24dd01c     	sub	sp, sp, #28
  34bbec: e3a0c000     	mov	r12, #0
  34bbf0: e3e0e000     	mvn	lr, #0
  34bbf4: e28d300c     	add	r3, sp, #12
  34bbf8: e58dc014     	str	r12, [sp, #0x14]
  34bbfc: e58de000     	str	lr, [sp]
  34bc00: e58dc00c     	str	r12, [sp, #0xc]
  34bc04: e58dc010     	str	r12, [sp, #0x10]
  34bc08: ebffff16     	bl	0x34b868 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)> @ imm = #-0x3a8
  34bc0c: e28dd01c     	add	sp, sp, #28
  34bc10: e8bd8000     	ldm	sp!, {pc}
