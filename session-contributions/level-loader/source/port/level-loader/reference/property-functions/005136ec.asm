
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

005136ec <PropertyMap::LoadDefaultProperties()>:
  5136ec: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  5136f0: e59f4108     	ldr	r4, [pc, #0x108]        @ 0x513800 <PropertyMap::LoadDefaultProperties()+0x114>
  5136f4: e59f6108     	ldr	r6, [pc, #0x108]        @ 0x513804 <PropertyMap::LoadDefaultProperties()+0x118>
  5136f8: e24dd020     	sub	sp, sp, #32
  5136fc: e08f4004     	add	r4, pc, r4
  513700: e7943006     	ldr	r3, [r4, r6]
  513704: e2809004     	add	r9, r0, #4
  513708: e1a08000     	mov	r8, r0
  51370c: e5933000     	ldr	r3, [r3]
  513710: e28d5004     	add	r5, sp, #4
  513714: e58d301c     	str	r3, [sp, #0x1c]
  513718: ebffff68     	bl	0x5134c0 <PropertyMap::GetPropertyMap()> @ imm = #-0x260
  51371c: e1a01009     	mov	r1, r9
  513720: e1a0a000     	mov	r10, r0
  513724: e1a00005     	mov	r0, r5
  513728: ebf8607a     	bl	0x32b918 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&)> @ imm = #-0x1e7e18
  51372c: e59a7008     	ldr	r7, [r10, #0x8]
  513730: e15a0007     	cmp	r10, r7
  513734: 0a000014     	beq	0x51378c <PropertyMap::LoadDefaultProperties()+0xa0> @ imm = #0x50
  513738: e5973028     	ldr	r3, [r7, #0x28]
  51373c: e3530000     	cmp	r3, #0
  513740: 0a000006     	beq	0x513760 <PropertyMap::LoadDefaultProperties()+0x74> @ imm = #0x18
  513744: e3580000     	cmp	r8, #0
  513748: 0a000004     	beq	0x513760 <PropertyMap::LoadDefaultProperties()+0x74> @ imm = #0x10
  51374c: e1a00003     	mov	r0, r3
  513750: e1a01008     	mov	r1, r8
  513754: e5933000     	ldr	r3, [r3]
  513758: e1a0e00f     	mov	lr, pc
  51375c: e593f00c     	ldr	pc, [r3, #0xc]
  513760: e597200c     	ldr	r2, [r7, #0xc]
  513764: e3520000     	cmp	r2, #0
  513768: 1a000001     	bne	0x513774 <PropertyMap::LoadDefaultProperties()+0x88> @ imm = #0x4
  51376c: ea000015     	b	0x5137c8 <PropertyMap::LoadDefaultProperties()+0xdc> @ imm = #0x54
  513770: e1a02003     	mov	r2, r3
  513774: e5923008     	ldr	r3, [r2, #0x8]
  513778: e3530000     	cmp	r3, #0
  51377c: 1afffffb     	bne	0x513770 <PropertyMap::LoadDefaultProperties()+0x84> @ imm = #-0x14
  513780: e1a07002     	mov	r7, r2
  513784: e15a0007     	cmp	r10, r7
  513788: 1affffea     	bne	0x513738 <PropertyMap::LoadDefaultProperties()+0x4c> @ imm = #-0x58
  51378c: e1590005     	cmp	r9, r5
  513790: 0a000003     	beq	0x5137a4 <PropertyMap::LoadDefaultProperties()+0xb8> @ imm = #0xc
  513794: e1a00009     	mov	r0, r9
  513798: e59d1018     	ldr	r1, [sp, #0x18]
  51379c: e59d2014     	ldr	r2, [sp, #0x14]
  5137a0: ebf7f48e     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x202dc8
  5137a4: e1a00005     	mov	r0, r5
  5137a8: ebf812a9     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x1fb55c
  5137ac: e7943006     	ldr	r3, [r4, r6]
  5137b0: e59d201c     	ldr	r2, [sp, #0x1c]
  5137b4: e5933000     	ldr	r3, [r3]
  5137b8: e1520003     	cmp	r2, r3
  5137bc: 1a00000e     	bne	0x5137fc <PropertyMap::LoadDefaultProperties()+0x110> @ imm = #0x38
  5137c0: e28dd020     	add	sp, sp, #32
  5137c4: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  5137c8: e5973004     	ldr	r3, [r7, #0x4]
  5137cc: e593100c     	ldr	r1, [r3, #0xc]
  5137d0: e1570001     	cmp	r7, r1
  5137d4: 1a000005     	bne	0x5137f0 <PropertyMap::LoadDefaultProperties()+0x104> @ imm = #0x14
  5137d8: e1a07003     	mov	r7, r3
  5137dc: e5933004     	ldr	r3, [r3, #0x4]
  5137e0: e593200c     	ldr	r2, [r3, #0xc]
  5137e4: e1520007     	cmp	r2, r7
  5137e8: 0afffffa     	beq	0x5137d8 <PropertyMap::LoadDefaultProperties()+0xec> @ imm = #-0x18
  5137ec: e597200c     	ldr	r2, [r7, #0xc]
  5137f0: e1520003     	cmp	r2, r3
  5137f4: 11a07003     	movne	r7, r3
  5137f8: eaffffcc     	b	0x513730 <PropertyMap::LoadDefaultProperties()+0x44> @ imm = #-0xd0
  5137fc: ebf7eac3     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x2054f4
  513800: 94 13 48 00  	.word	0x00481394
  513804: ac 40 00 00  	.word	0x000040ac
