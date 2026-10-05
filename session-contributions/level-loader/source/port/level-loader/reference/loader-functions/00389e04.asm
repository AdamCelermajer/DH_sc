
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00389e04 <Module::DeclareProperties()>:
  389e04: e92d4070     	push	{r4, r5, r6, lr}
  389e08: e24dd010     	sub	sp, sp, #16
  389e0c: e1a05000     	mov	r5, r0
  389e10: ebfffeee     	bl	0x3899d0 <Decor::DeclareProperties()> @ imm = #-0x448
  389e14: e59f10a8     	ldr	r1, [pc, #0xa8]         @ 0x389ec4 <Module::DeclareProperties()+0xc0>
  389e18: e2856004     	add	r6, r5, #4
  389e1c: e1a00006     	mov	r0, r6
  389e20: e2852fde     	add	r2, r5, #888
  389e24: e08f1001     	add	r1, pc, r1
  389e28: ebfed453     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x4aeb4
  389e2c: e59f1094     	ldr	r1, [pc, #0x94]         @ 0x389ec8 <Module::DeclareProperties()+0xc4>
  389e30: e1a00006     	mov	r0, r6
  389e34: e2852e39     	add	r2, r5, #912
  389e38: e08f1001     	add	r1, pc, r1
  389e3c: ebfed44e     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x4aec8
  389e40: e59f1084     	ldr	r1, [pc, #0x84]         @ 0x389ecc <Module::DeclareProperties()+0xc8>
  389e44: e1a00006     	mov	r0, r6
  389e48: e2852fea     	add	r2, r5, #936
  389e4c: e08f1001     	add	r1, pc, r1
  389e50: ebfed449     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x4aedc
  389e54: e59f1074     	ldr	r1, [pc, #0x74]         @ 0x389ed0 <Module::DeclareProperties()+0xcc>
  389e58: e1a00006     	mov	r0, r6
  389e5c: e2852d0f     	add	r2, r5, #960
  389e60: e08f1001     	add	r1, pc, r1
  389e64: ebfed444     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x4aef0
  389e68: e59f1064     	ldr	r1, [pc, #0x64]         @ 0x389ed4 <Module::DeclareProperties()+0xd0>
  389e6c: e1a00006     	mov	r0, r6
  389e70: e2852ff6     	add	r2, r5, #984
  389e74: e08f1001     	add	r1, pc, r1
  389e78: e59f4058     	ldr	r4, [pc, #0x58]         @ 0x389ed8 <Module::DeclareProperties()+0xd4>
  389e7c: ebfed43e     	bl	0x33ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)> @ imm = #-0x4af08
  389e80: e59f3054     	ldr	r3, [pc, #0x54]         @ 0x389edc <Module::DeclareProperties()+0xd8>
  389e84: e08f4004     	add	r4, pc, r4
  389e88: e59f1050     	ldr	r1, [pc, #0x50]         @ 0x389ee0 <Module::DeclareProperties()+0xdc>
  389e8c: e7943003     	ldr	r3, [r4, r3]
  389e90: e1a00006     	mov	r0, r6
  389e94: e08f1001     	add	r1, pc, r1
  389e98: e593c008     	ldr	r12, [r3, #0x8]
  389e9c: e5936000     	ldr	r6, [r3]
  389ea0: e593e004     	ldr	lr, [r3, #0x4]
  389ea4: e2852e3f     	add	r2, r5, #1008
  389ea8: e28d3004     	add	r3, sp, #4
  389eac: e58d6004     	str	r6, [sp, #0x4]
  389eb0: e58de008     	str	lr, [sp, #0x8]
  389eb4: e58dc00c     	str	r12, [sp, #0xc]
  389eb8: ebfffe75     	bl	0x389894 <void PropertyMap::AddProperty<Point3D<float>>(char const*, Point3D<float>&, Point3D<float>)> @ imm = #-0x62c
  389ebc: e28dd010     	add	sp, sp, #16
  389ec0: e8bd8070     	pop	{r4, r5, r6, pc}
  389ec4: ec 84 53 00  	.word	0x005384ec
  389ec8: e0 84 53 00  	.word	0x005384e0
  389ecc: d4 84 53 00  	.word	0x005384d4
  389ed0: c8 84 53 00  	.word	0x005384c8
  389ed4: bc 84 53 00  	.word	0x005384bc
  389ed8: 0c ac 60 00  	.word	0x0060ac0c
  389edc: 98 24 00 00  	.word	0x00002498
  389ee0: ac 84 53 00  	.word	0x005384ac
