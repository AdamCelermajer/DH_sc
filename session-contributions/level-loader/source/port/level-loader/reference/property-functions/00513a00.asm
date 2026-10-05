
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00513a00 <PropertyMap::LoadOverridesFromXML(TiXmlElement*)>:
  513a00: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  513a04: e2517000     	subs	r7, r1, #0
  513a08: e1a06000     	mov	r6, r0
  513a0c: 0a000017     	beq	0x513a70 <PropertyMap::LoadOverridesFromXML(TiXmlElement*)+0x70> @ imm = #0x5c
  513a10: ebfffeaa     	bl	0x5134c0 <PropertyMap::GetPropertyMap()> @ imm = #-0x558
  513a14: e5904008     	ldr	r4, [r0, #0x8]
  513a18: e1a05000     	mov	r5, r0
  513a1c: e1550004     	cmp	r5, r4
  513a20: 0a000012     	beq	0x513a70 <PropertyMap::LoadOverridesFromXML(TiXmlElement*)+0x70> @ imm = #0x48
  513a24: e5948024     	ldr	r8, [r4, #0x24]
  513a28: e1a00007     	mov	r0, r7
  513a2c: e1a01008     	mov	r1, r8
  513a30: eb00048e     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x1238
  513a34: e1a01008     	mov	r1, r8
  513a38: e1a02000     	mov	r2, r0
  513a3c: e1a00006     	mov	r0, r6
  513a40: ebffff8d     	bl	0x51387c <PropertyMap::SetProperty(char const*, char const*)> @ imm = #-0x1cc
  513a44: e594200c     	ldr	r2, [r4, #0xc]
  513a48: e3520000     	cmp	r2, #0
  513a4c: 1a000001     	bne	0x513a58 <PropertyMap::LoadOverridesFromXML(TiXmlElement*)+0x58> @ imm = #0x4
  513a50: ea000007     	b	0x513a74 <PropertyMap::LoadOverridesFromXML(TiXmlElement*)+0x74> @ imm = #0x1c
  513a54: e1a02003     	mov	r2, r3
  513a58: e5923008     	ldr	r3, [r2, #0x8]
  513a5c: e3530000     	cmp	r3, #0
  513a60: 1afffffb     	bne	0x513a54 <PropertyMap::LoadOverridesFromXML(TiXmlElement*)+0x54> @ imm = #-0x14
  513a64: e1a04002     	mov	r4, r2
  513a68: e1550004     	cmp	r5, r4
  513a6c: 1affffec     	bne	0x513a24 <PropertyMap::LoadOverridesFromXML(TiXmlElement*)+0x24> @ imm = #-0x50
  513a70: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  513a74: e5943004     	ldr	r3, [r4, #0x4]
  513a78: e593100c     	ldr	r1, [r3, #0xc]
  513a7c: e1540001     	cmp	r4, r1
  513a80: 1a000005     	bne	0x513a9c <PropertyMap::LoadOverridesFromXML(TiXmlElement*)+0x9c> @ imm = #0x14
  513a84: e1a04003     	mov	r4, r3
  513a88: e5933004     	ldr	r3, [r3, #0x4]
  513a8c: e593200c     	ldr	r2, [r3, #0xc]
  513a90: e1520004     	cmp	r2, r4
  513a94: 0afffffa     	beq	0x513a84 <PropertyMap::LoadOverridesFromXML(TiXmlElement*)+0x84> @ imm = #-0x18
  513a98: e594200c     	ldr	r2, [r4, #0xc]
  513a9c: e1520003     	cmp	r2, r3
  513aa0: 11a04003     	movne	r4, r3
  513aa4: eaffffdc     	b	0x513a1c <PropertyMap::LoadOverridesFromXML(TiXmlElement*)+0x1c> @ imm = #-0x90
