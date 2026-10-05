
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

005134f0 <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)>:
  5134f0: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  5134f4: e59f316c     	ldr	r3, [pc, #0x16c]        @ 0x513668 <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)+0x178>
  5134f8: e59f416c     	ldr	r4, [pc, #0x16c]        @ 0x51366c <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)+0x17c>
  5134fc: e24dd054     	sub	sp, sp, #84
  513500: e08f3003     	add	r3, pc, r3
  513504: e58d3004     	str	r3, [sp, #0x4]
  513508: e7933004     	ldr	r3, [r3, r4]
  51350c: e3520000     	cmp	r2, #0
  513510: e58d4008     	str	r4, [sp, #0x8]
  513514: e5933000     	ldr	r3, [r3]
  513518: e1a08000     	mov	r8, r0
  51351c: e58d100c     	str	r1, [sp, #0xc]
  513520: e58d304c     	str	r3, [sp, #0x4c]
  513524: 11a04002     	movne	r4, r2
  513528: 0a00004a     	beq	0x513658 <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)+0x168> @ imm = #0x128
  51352c: e59dc00c     	ldr	r12, [sp, #0xc]
  513530: e35c0000     	cmp	r12, #0
  513534: 0a000031     	beq	0x513600 <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)+0x110> @ imm = #0xc4
  513538: e3a01000     	mov	r1, #0
  51353c: e3a0008c     	mov	r0, #140
  513540: ebf7f40a     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x202fd8
  513544: e1a01004     	mov	r1, r4
  513548: e1a0a000     	mov	r10, r0
  51354c: eb000fc2     	bl	0x51745c <TiXmlElement::TiXmlElement(char const*)> @ imm = #0x3f08
  513550: e1a00008     	mov	r0, r8
  513554: ebffffd9     	bl	0x5134c0 <PropertyMap::GetPropertyMap()> @ imm = #-0x9c
  513558: e5904008     	ldr	r4, [r0, #0x8]
  51355c: e1a07000     	mov	r7, r0
  513560: e28d601c     	add	r6, sp, #28
  513564: e28d9018     	add	r9, sp, #24
  513568: e28d5034     	add	r5, sp, #52
  51356c: e28db010     	add	r11, sp, #16
  513570: e1570004     	cmp	r7, r4
  513574: 0a00001e     	beq	0x5135f4 <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)+0x104> @ imm = #0x78
  513578: e5943028     	ldr	r3, [r4, #0x28]
  51357c: e3530000     	cmp	r3, #0
  513580: 0a000010     	beq	0x5135c8 <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)+0xd8> @ imm = #0x40
  513584: e58d3010     	str	r3, [sp, #0x10]
  513588: e58d8014     	str	r8, [sp, #0x14]
  51358c: e593101c     	ldr	r1, [r3, #0x1c]
  513590: e1a02009     	mov	r2, r9
  513594: e1a00006     	mov	r0, r6
  513598: ebf802d3     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x1ff4b4
  51359c: e1a00005     	mov	r0, r5
  5135a0: e1a0100b     	mov	r1, r11
  5135a4: ebfff68d     	bl	0x510fe0 <PropertyInstance::ToString()> @ imm = #-0x25cc
  5135a8: e1a0000a     	mov	r0, r10
  5135ac: e1a01006     	mov	r1, r6
  5135b0: e1a02005     	mov	r2, r5
  5135b4: eb000b91     	bl	0x516400 <TiXmlElement::SetAttribute(std::string const&, std::string const&)> @ imm = #0x2e44
  5135b8: e1a00005     	mov	r0, r5
  5135bc: ebf81324     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x1fb370
  5135c0: e1a00006     	mov	r0, r6
  5135c4: ebf81322     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x1fb378
  5135c8: e594200c     	ldr	r2, [r4, #0xc]
  5135cc: e3520000     	cmp	r2, #0
  5135d0: 1a000001     	bne	0x5135dc <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)+0xec> @ imm = #0x4
  5135d4: ea000012     	b	0x513624 <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)+0x134> @ imm = #0x48
  5135d8: e1a02003     	mov	r2, r3
  5135dc: e5923008     	ldr	r3, [r2, #0x8]
  5135e0: e3530000     	cmp	r3, #0
  5135e4: 1afffffb     	bne	0x5135d8 <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)+0xe8> @ imm = #-0x14
  5135e8: e1a04002     	mov	r4, r2
  5135ec: e1570004     	cmp	r7, r4
  5135f0: 1affffe0     	bne	0x513578 <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)+0x88> @ imm = #-0x80
  5135f4: e59d000c     	ldr	r0, [sp, #0xc]
  5135f8: e1a0100a     	mov	r1, r10
  5135fc: eb0008d8     	bl	0x515964 <TiXmlNode::LinkEndChild(TiXmlNode*)> @ imm = #0x2360
  513600: e59d2004     	ldr	r2, [sp, #0x4]
  513604: e59d1008     	ldr	r1, [sp, #0x8]
  513608: e7923001     	ldr	r3, [r2, r1]
  51360c: e59d204c     	ldr	r2, [sp, #0x4c]
  513610: e5933000     	ldr	r3, [r3]
  513614: e1520003     	cmp	r2, r3
  513618: 1a000011     	bne	0x513664 <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)+0x174> @ imm = #0x44
  51361c: e28dd054     	add	sp, sp, #84
  513620: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  513624: e5943004     	ldr	r3, [r4, #0x4]
  513628: e593100c     	ldr	r1, [r3, #0xc]
  51362c: e1540001     	cmp	r4, r1
  513630: 1a000005     	bne	0x51364c <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)+0x15c> @ imm = #0x14
  513634: e1a04003     	mov	r4, r3
  513638: e5933004     	ldr	r3, [r3, #0x4]
  51363c: e593200c     	ldr	r2, [r3, #0xc]
  513640: e1520004     	cmp	r2, r4
  513644: 0afffffa     	beq	0x513634 <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)+0x144> @ imm = #-0x18
  513648: e594200c     	ldr	r2, [r4, #0xc]
  51364c: e1520003     	cmp	r2, r3
  513650: 11a04003     	movne	r4, r3
  513654: eaffffc5     	b	0x513570 <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)+0x80> @ imm = #-0xec
  513658: e59f4010     	ldr	r4, [pc, #0x10]         @ 0x513670 <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)+0x180>
  51365c: e08f4004     	add	r4, pc, r4
  513660: eaffffb1     	b	0x51352c <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)+0x3c> @ imm = #-0x13c
  513664: ebf7eb29     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x20535c
  513668: 90 15 48 00  	.word	0x00481590
  51366c: ac 40 00 00  	.word	0x000040ac
  513670: 0c cc 3a 00  	.word	0x003acc0c
