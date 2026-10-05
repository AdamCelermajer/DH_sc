
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0033ef7c <void PropertyMap::AddProperty<std::string>(char const*, std::string&)>:
  33ef7c: e59f3080     	ldr	r3, [pc, #0x80]         @ 0x33f004 <void PropertyMap::AddProperty<std::string>(char const*, std::string&)+0x88>
  33ef80: e59fc080     	ldr	r12, [pc, #0x80]        @ 0x33f008 <void PropertyMap::AddProperty<std::string>(char const*, std::string&)+0x8c>
  33ef84: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  33ef88: e08f3003     	add	r3, pc, r3
  33ef8c: e793500c     	ldr	r5, [r3, r12]
  33ef90: e24dd020     	sub	sp, sp, #32
  33ef94: e28d4004     	add	r4, sp, #4
  33ef98: e595c000     	ldr	r12, [r5]
  33ef9c: e1a06000     	mov	r6, r0
  33efa0: e1a07001     	mov	r7, r1
  33efa4: e1a00004     	mov	r0, r4
  33efa8: e3a01010     	mov	r1, #16
  33efac: e58dc01c     	str	r12, [sp, #0x1c]
  33efb0: e1a08002     	mov	r8, r2
  33efb4: e58d4014     	str	r4, [sp, #0x14]
  33efb8: e58d4018     	str	r4, [sp, #0x18]
  33efbc: ebff49ae     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x2d948
  33efc0: e59d3014     	ldr	r3, [sp, #0x14]
  33efc4: e3a02000     	mov	r2, #0
  33efc8: e1a01007     	mov	r1, r7
  33efcc: e5c32000     	strb	r2, [r3]
  33efd0: e1a00006     	mov	r0, r6
  33efd4: e1a02008     	mov	r2, r8
  33efd8: e1a03004     	mov	r3, r4
  33efdc: ebfffd08     	bl	0x33e404 <void PropertyMap::AddProperty<std::string>(char const*, std::string&, std::string)> @ imm = #-0xbe0
  33efe0: e1a00004     	mov	r0, r4
  33efe4: ebff5270     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x2b640
  33efe8: e59d201c     	ldr	r2, [sp, #0x1c]
  33efec: e5953000     	ldr	r3, [r5]
  33eff0: e1520003     	cmp	r2, r3
  33eff4: 1a000001     	bne	0x33f000 <void PropertyMap::AddProperty<std::string>(char const*, std::string&)+0x84> @ imm = #0x4
  33eff8: e28dd020     	add	sp, sp, #32
  33effc: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  33f000: ebff3cc2     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x30cf8
  33f004: 08 5b 65 00  	.word	0x00655b08
  33f008: ac 40 00 00  	.word	0x000040ac
