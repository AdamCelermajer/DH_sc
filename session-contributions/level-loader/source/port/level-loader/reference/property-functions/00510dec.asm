
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00510dec <PropertyMap::DestroyPropertyMaps()>:
  510dec: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  510df0: e59fa1dc     	ldr	r10, [pc, #0x1dc]       @ 0x510fd4 <PropertyMap::DestroyPropertyMaps()+0x1e8>
  510df4: e59f31dc     	ldr	r3, [pc, #0x1dc]        @ 0x510fd8 <PropertyMap::DestroyPropertyMaps()+0x1ec>
  510df8: e24dd00c     	sub	sp, sp, #12
  510dfc: e08fa00a     	add	r10, pc, r10
  510e00: e79a3003     	ldr	r3, [r10, r3]
  510e04: e59f91d0     	ldr	r9, [pc, #0x1d0]        @ 0x510fdc <PropertyMap::DestroyPropertyMaps()+0x1f0>
  510e08: e58d3004     	str	r3, [sp, #0x4]
  510e0c: e5934008     	ldr	r4, [r3, #0x8]
  510e10: e59d3004     	ldr	r3, [sp, #0x4]
  510e14: e1540003     	cmp	r4, r3
  510e18: 0a000034     	beq	0x510ef0 <PropertyMap::DestroyPropertyMaps()+0x104> @ imm = #0xd0
  510e1c: e5948030     	ldr	r8, [r4, #0x30]
  510e20: e284b028     	add	r11, r4, #40
  510e24: e15b0008     	cmp	r11, r8
  510e28: 0a000024     	beq	0x510ec0 <PropertyMap::DestroyPropertyMaps()+0xd4> @ imm = #0x90
  510e2c: e5985030     	ldr	r5, [r8, #0x30]
  510e30: e2887028     	add	r7, r8, #40
  510e34: e1570005     	cmp	r7, r5
  510e38: 0a000014     	beq	0x510e90 <PropertyMap::DestroyPropertyMaps()+0xa4> @ imm = #0x50
  510e3c: e5956028     	ldr	r6, [r5, #0x28]
  510e40: e3560000     	cmp	r6, #0
  510e44: 0a000006     	beq	0x510e64 <PropertyMap::DestroyPropertyMaps()+0x78> @ imm = #0x18
  510e48: e79a3009     	ldr	r3, [r10, r9]
  510e4c: e1a00006     	mov	r0, r6
  510e50: e2833008     	add	r3, r3, #8
  510e54: e4803008     	str	r3, [r0], #8
  510e58: ebf81cfd     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x1f8c0c
  510e5c: e1a00006     	mov	r0, r6
  510e60: ebf7fd76     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x200a28
  510e64: e595200c     	ldr	r2, [r5, #0xc]
  510e68: e3520000     	cmp	r2, #0
  510e6c: 1a000001     	bne	0x510e78 <PropertyMap::DestroyPropertyMaps()+0x8c> @ imm = #0x4
  510e70: ea000023     	b	0x510f04 <PropertyMap::DestroyPropertyMaps()+0x118> @ imm = #0x8c
  510e74: e1a02003     	mov	r2, r3
  510e78: e5923008     	ldr	r3, [r2, #0x8]
  510e7c: e3530000     	cmp	r3, #0
  510e80: 1afffffb     	bne	0x510e74 <PropertyMap::DestroyPropertyMaps()+0x88> @ imm = #-0x14
  510e84: e1a05002     	mov	r5, r2
  510e88: e1570005     	cmp	r7, r5
  510e8c: 1affffea     	bne	0x510e3c <PropertyMap::DestroyPropertyMaps()+0x50> @ imm = #-0x58
  510e90: e598100c     	ldr	r1, [r8, #0xc]
  510e94: e3510000     	cmp	r1, #0
  510e98: e1a02001     	mov	r2, r1
  510e9c: 1a000001     	bne	0x510ea8 <PropertyMap::DestroyPropertyMaps()+0xbc> @ imm = #0x4
  510ea0: ea000024     	b	0x510f38 <PropertyMap::DestroyPropertyMaps()+0x14c> @ imm = #0x90
  510ea4: e1a02003     	mov	r2, r3
  510ea8: e5923008     	ldr	r3, [r2, #0x8]
  510eac: e3530000     	cmp	r3, #0
  510eb0: 1afffffb     	bne	0x510ea4 <PropertyMap::DestroyPropertyMaps()+0xb8> @ imm = #-0x14
  510eb4: e1a08002     	mov	r8, r2
  510eb8: e15b0008     	cmp	r11, r8
  510ebc: 1affffda     	bne	0x510e2c <PropertyMap::DestroyPropertyMaps()+0x40> @ imm = #-0x98
  510ec0: e594200c     	ldr	r2, [r4, #0xc]
  510ec4: e3520000     	cmp	r2, #0
  510ec8: 1a000001     	bne	0x510ed4 <PropertyMap::DestroyPropertyMaps()+0xe8> @ imm = #0x4
  510ecc: ea000026     	b	0x510f6c <PropertyMap::DestroyPropertyMaps()+0x180> @ imm = #0x98
  510ed0: e1a02003     	mov	r2, r3
  510ed4: e5923008     	ldr	r3, [r2, #0x8]
  510ed8: e3530000     	cmp	r3, #0
  510edc: 1afffffb     	bne	0x510ed0 <PropertyMap::DestroyPropertyMaps()+0xe4> @ imm = #-0x14
  510ee0: e1a04002     	mov	r4, r2
  510ee4: e59d3004     	ldr	r3, [sp, #0x4]
  510ee8: e1540003     	cmp	r4, r3
  510eec: 1affffca     	bne	0x510e1c <PropertyMap::DestroyPropertyMaps()+0x30> @ imm = #-0xd8
  510ef0: e5943010     	ldr	r3, [r4, #0x10]
  510ef4: e3530000     	cmp	r3, #0
  510ef8: 1a00002d     	bne	0x510fb4 <PropertyMap::DestroyPropertyMaps()+0x1c8> @ imm = #0xb4
  510efc: e28dd00c     	add	sp, sp, #12
  510f00: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  510f04: e5953004     	ldr	r3, [r5, #0x4]
  510f08: e593100c     	ldr	r1, [r3, #0xc]
  510f0c: e1550001     	cmp	r5, r1
  510f10: 1a000005     	bne	0x510f2c <PropertyMap::DestroyPropertyMaps()+0x140> @ imm = #0x14
  510f14: e1a05003     	mov	r5, r3
  510f18: e5933004     	ldr	r3, [r3, #0x4]
  510f1c: e593200c     	ldr	r2, [r3, #0xc]
  510f20: e1520005     	cmp	r2, r5
  510f24: 0afffffa     	beq	0x510f14 <PropertyMap::DestroyPropertyMaps()+0x128> @ imm = #-0x18
  510f28: e595200c     	ldr	r2, [r5, #0xc]
  510f2c: e1530002     	cmp	r3, r2
  510f30: 11a05003     	movne	r5, r3
  510f34: eaffffbe     	b	0x510e34 <PropertyMap::DestroyPropertyMaps()+0x48> @ imm = #-0x108
  510f38: e5983004     	ldr	r3, [r8, #0x4]
  510f3c: e593200c     	ldr	r2, [r3, #0xc]
  510f40: e1580002     	cmp	r8, r2
  510f44: 1a000005     	bne	0x510f60 <PropertyMap::DestroyPropertyMaps()+0x174> @ imm = #0x14
  510f48: e1a08003     	mov	r8, r3
  510f4c: e5933004     	ldr	r3, [r3, #0x4]
  510f50: e593200c     	ldr	r2, [r3, #0xc]
  510f54: e1520008     	cmp	r2, r8
  510f58: 0afffffa     	beq	0x510f48 <PropertyMap::DestroyPropertyMaps()+0x15c> @ imm = #-0x18
  510f5c: e598100c     	ldr	r1, [r8, #0xc]
  510f60: e1530001     	cmp	r3, r1
  510f64: 11a08003     	movne	r8, r3
  510f68: eaffffad     	b	0x510e24 <PropertyMap::DestroyPropertyMaps()+0x38> @ imm = #-0x14c
  510f6c: e5943004     	ldr	r3, [r4, #0x4]
  510f70: e593200c     	ldr	r2, [r3, #0xc]
  510f74: e1540002     	cmp	r4, r2
  510f78: 11a02004     	movne	r2, r4
  510f7c: 0a000001     	beq	0x510f88 <PropertyMap::DestroyPropertyMaps()+0x19c> @ imm = #0x4
  510f80: ea000006     	b	0x510fa0 <PropertyMap::DestroyPropertyMaps()+0x1b4> @ imm = #0x18
  510f84: e1a03001     	mov	r3, r1
  510f88: e5931004     	ldr	r1, [r3, #0x4]
  510f8c: e591200c     	ldr	r2, [r1, #0xc]
  510f90: e1520003     	cmp	r2, r3
  510f94: 0afffffa     	beq	0x510f84 <PropertyMap::DestroyPropertyMaps()+0x198> @ imm = #-0x18
  510f98: e1a02003     	mov	r2, r3
  510f9c: e1a03001     	mov	r3, r1
  510fa0: e592100c     	ldr	r1, [r2, #0xc]
  510fa4: e1530001     	cmp	r3, r1
  510fa8: 11a02003     	movne	r2, r3
  510fac: e1a04002     	mov	r4, r2
  510fb0: eaffffcb     	b	0x510ee4 <PropertyMap::DestroyPropertyMaps()+0xf8> @ imm = #-0xd4
  510fb4: e1a00004     	mov	r0, r4
  510fb8: e5941004     	ldr	r1, [r4, #0x4]
  510fbc: ebffff6d     	bl	0x510d78 <std::priv::_Rb_tree<std::string, std::less<std::string>, std::pair<std::string const, std::map<std::string, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>>>>>, std::priv::_Select1st<std::pair<std::string const, std::map<std::string, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>>>>>>, std::priv::_MapTraitsT<std::pair<std::string const, std::map<std::string, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>>>>>>, std::allocator<std::pair<std::string const, std::map<std::string, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>>>>>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x24c
  510fc0: e3a03000     	mov	r3, #0
  510fc4: e5843010     	str	r3, [r4, #0x10]
  510fc8: e9840018     	stmib	r4, {r3, r4}
  510fcc: e584400c     	str	r4, [r4, #0xc]
  510fd0: eaffffc9     	b	0x510efc <PropertyMap::DestroyPropertyMaps()+0x110> @ imm = #-0xdc
  510fd4: 94 3c 48 00  	.word	0x00483c94
  510fd8: 68 2a 00 00  	.word	0x00002a68
  510fdc: 30 23 00 00  	.word	0x00002330
