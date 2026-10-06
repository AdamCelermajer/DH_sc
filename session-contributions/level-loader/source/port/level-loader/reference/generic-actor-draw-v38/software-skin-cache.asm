
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0066fe34 <glitch::collada::detail::CColladaSoftwareSkinTechnique::prepareCache()>:
  66fe34: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  66fe38: e5903010     	ldr	r3, [r0, #0x10]
  66fe3c: e24dd0d0     	sub	sp, sp, #208
  66fe40: e1a05000     	mov	r5, r0
  66fe44: e5933000     	ldr	r3, [r3]
  66fe48: e3130001     	tst	r3, #1
  66fe4c: 1a000001     	bne	0x66fe58 <glitch::collada::detail::CColladaSoftwareSkinTechnique::prepareCache()+0x24> @ imm = #0x4
  66fe50: e28dd0d0     	add	sp, sp, #208
  66fe54: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  66fe58: ebffff15     	bl	0x66fab4 <glitch::collada::detail::CColladaSoftwareSkinTechnique::preparePtrCache()> @ imm = #-0x3ac
  66fe5c: e595300c     	ldr	r3, [r5, #0xc]
  66fe60: e5958010     	ldr	r8, [r5, #0x10]
  66fe64: e28d408c     	add	r4, sp, #140
  66fe68: e5937074     	ldr	r7, [r3, #0x74]
  66fe6c: e3a06000     	mov	r6, #0
  66fe70: e1a01006     	mov	r1, r6
  66fe74: e3a02040     	mov	r2, #64
  66fe78: e1a00004     	mov	r0, r4
  66fe7c: e2888004     	add	r8, r8, #4
  66fe80: ebf27976     	bl	0x30e460 <.plt+0x6ec>   @ imm = #-0x361a28
  66fe84: e3a035fe     	mov	r3, #1065353216
  66fe88: e1a02004     	mov	r2, r4
  66fe8c: e3a0c001     	mov	r12, #1
  66fe90: e1a00008     	mov	r0, r8
  66fe94: e1a01007     	mov	r1, r7
  66fe98: e58d30c8     	str	r3, [sp, #0xc8]
  66fe9c: e58d308c     	str	r3, [sp, #0x8c]
  66fea0: e58d30a0     	str	r3, [sp, #0xa0]
  66fea4: e58d30b4     	str	r3, [sp, #0xb4]
  66fea8: e5cdc0cc     	strb	r12, [sp, #0xcc]
  66feac: ebfff37a     	bl	0x66cc9c <std::vector<glitch::core::CMatrix4<float>, std::allocator<glitch::core::CMatrix4<float>>>::resize(unsigned int, glitch::core::CMatrix4<float> const&)> @ imm = #-0x3218
  66feb0: e5953010     	ldr	r3, [r5, #0x10]
  66feb4: e5932010     	ldr	r2, [r3, #0x10]
  66feb8: e5939014     	ldr	r9, [r3, #0x14]
  66febc: e0629009     	rsb	r9, r2, r9
  66fec0: e1b09149     	asrs	r9, r9, #2
  66fec4: 0a00001b     	beq	0x66ff38 <glitch::collada::detail::CColladaSoftwareSkinTechnique::prepareCache()+0x104> @ imm = #0x6c
  66fec8: e1a04006     	mov	r4, r6
  66fecc: e28d8048     	add	r8, sp, #72
  66fed0: e28d7004     	add	r7, sp, #4
  66fed4: ea000001     	b	0x66fee0 <glitch::collada::detail::CColladaSoftwareSkinTechnique::prepareCache()+0xac> @ imm = #0x4
  66fed8: e5953010     	ldr	r3, [r5, #0x10]
  66fedc: e5932010     	ldr	r2, [r3, #0x10]
  66fee0: e595000c     	ldr	r0, [r5, #0xc]
  66fee4: e7921104     	ldr	r1, [r2, r4, lsl #2]
  66fee8: e593a004     	ldr	r10, [r3, #0x4]
  66feec: e5902004     	ldr	r2, [r0, #0x4]
  66fef0: e1a00008     	mov	r0, r8
  66fef4: e08aa006     	add	r10, r10, r6
  66fef8: e0822304     	add	r2, r2, r4, lsl #6
  66fefc: ebffd4a7     	bl	0x6651a0 <glitch::core::CMatrix4<float> glitch::core::operator*<float, glitch::collada::SMatrix>(glitch::core::CMatrix4<float> const&, glitch::collada::SMatrix const&)> @ imm = #-0xad64
  66ff00: e595200c     	ldr	r2, [r5, #0xc]
  66ff04: e1a00007     	mov	r0, r7
  66ff08: e1a01008     	mov	r1, r8
  66ff0c: e2822010     	add	r2, r2, #16
  66ff10: ebffd4a2     	bl	0x6651a0 <glitch::core::CMatrix4<float> glitch::core::operator*<float, glitch::collada::SMatrix>(glitch::core::CMatrix4<float> const&, glitch::collada::SMatrix const&)> @ imm = #-0xad78
  66ff14: e2844001     	add	r4, r4, #1
  66ff18: e1a0000a     	mov	r0, r10
  66ff1c: e1a01007     	mov	r1, r7
  66ff20: e3a02041     	mov	r2, #65
  66ff24: ebf27a4f     	bl	0x30e868 <.plt+0xaf4>   @ imm = #-0x3616c4
  66ff28: e1540009     	cmp	r4, r9
  66ff2c: e2866044     	add	r6, r6, #68
  66ff30: 1affffe8     	bne	0x66fed8 <glitch::collada::detail::CColladaSoftwareSkinTechnique::prepareCache()+0xa4> @ imm = #-0x60
  66ff34: e5953010     	ldr	r3, [r5, #0x10]
  66ff38: e5932000     	ldr	r2, [r3]
  66ff3c: e3c22001     	bic	r2, r2, #1
  66ff40: e5832000     	str	r2, [r3]
  66ff44: eaffffc1     	b	0x66fe50 <glitch::collada::detail::CColladaSoftwareSkinTechnique::prepareCache()+0x1c> @ imm = #-0xfc
