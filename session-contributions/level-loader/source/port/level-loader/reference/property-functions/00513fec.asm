
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00513fec <PropertyMap::SetTemplate(std::string const&)>:
  513fec: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  513ff0: e5913014     	ldr	r3, [r1, #0x14]
  513ff4: e5912010     	ldr	r2, [r1, #0x10]
  513ff8: e59f7164     	ldr	r7, [pc, #0x164]        @ 0x514164 <PropertyMap::SetTemplate(std::string const&)+0x178>
  513ffc: e24dd014     	sub	sp, sp, #20
  514000: e1530002     	cmp	r3, r2
  514004: e1a04000     	mov	r4, r0
  514008: e08f7007     	add	r7, pc, r7
  51400c: 0a00000b     	beq	0x514040 <PropertyMap::SetTemplate(std::string const&)+0x54> @ imm = #0x2c
  514010: e2800004     	add	r0, r0, #4
  514014: e1510000     	cmp	r1, r0
  514018: 0a000001     	beq	0x514024 <PropertyMap::SetTemplate(std::string const&)+0x38> @ imm = #0x4
  51401c: e1a01003     	mov	r1, r3
  514020: ebf7f26e     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x203648
  514024: e1a00004     	mov	r0, r4
  514028: ebfffd24     	bl	0x5134c0 <PropertyMap::GetPropertyMap()> @ imm = #-0xb70
  51402c: e5908010     	ldr	r8, [r0, #0x10]
  514030: e3580000     	cmp	r8, #0
  514034: 0a000003     	beq	0x514048 <PropertyMap::SetTemplate(std::string const&)+0x5c> @ imm = #0xc
  514038: e1a00004     	mov	r0, r4
  51403c: eb000056     	bl	0x51419c <PropertyMap::LoadTemplate()> @ imm = #0x158
  514040: e28dd014     	add	sp, sp, #20
  514044: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  514048: e1a00004     	mov	r0, r4
  51404c: ebfff2be     	bl	0x510b4c <PropertyMap::GetThisClassName()> @ imm = #-0x3508
  514050: e28d3010     	add	r3, sp, #16
  514054: e5230004     	str	r0, [r3, #-0x4]!
  514058: e1a00003     	mov	r0, r3
  51405c: ebfff91c     	bl	0x5124d4 <std::map<std::string, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>>>>& std::map<std::string, std::map<std::string, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>>>>>>>::operator[]<char const*>(char const* const&) (.clone.2)> @ imm = #-0x1b90
  514060: ebfffe90     	bl	0x513aa8 <std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>& std::map<std::string, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>>>>::operator[]<char [1]>(char const (&) [1]) (.clone.3)> @ imm = #-0x5c0
  514064: e1a05000     	mov	r5, r0
  514068: e1a00004     	mov	r0, r4
  51406c: ebfffd13     	bl	0x5134c0 <PropertyMap::GetPropertyMap()> @ imm = #-0xbb4
  514070: e5903010     	ldr	r3, [r0, #0x10]
  514074: e1a06000     	mov	r6, r0
  514078: e3530000     	cmp	r3, #0
  51407c: 1a000023     	bne	0x514110 <PropertyMap::SetTemplate(std::string const&)+0x124> @ imm = #0x8c
  514080: e5957008     	ldr	r7, [r5, #0x8]
  514084: e1550007     	cmp	r5, r7
  514088: 0affffea     	beq	0x514038 <PropertyMap::SetTemplate(std::string const&)+0x4c> @ imm = #-0x58
  51408c: e2871010     	add	r1, r7, #16
  514090: e1a00006     	mov	r0, r6
  514094: e5978028     	ldr	r8, [r7, #0x28]
  514098: ebfffab5     	bl	0x512b74 <Property*& std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>::operator[]<std::string>(std::string const&)> @ imm = #-0x152c
  51409c: e5983000     	ldr	r3, [r8]
  5140a0: e1a0a000     	mov	r10, r0
  5140a4: e1a00008     	mov	r0, r8
  5140a8: e1a0e00f     	mov	lr, pc
  5140ac: e593f014     	ldr	pc, [r3, #0x14]
  5140b0: e58a0000     	str	r0, [r10]
  5140b4: e597200c     	ldr	r2, [r7, #0xc]
  5140b8: e3520000     	cmp	r2, #0
  5140bc: 1a000001     	bne	0x5140c8 <PropertyMap::SetTemplate(std::string const&)+0xdc> @ imm = #0x4
  5140c0: ea000005     	b	0x5140dc <PropertyMap::SetTemplate(std::string const&)+0xf0> @ imm = #0x14
  5140c4: e1a02003     	mov	r2, r3
  5140c8: e5923008     	ldr	r3, [r2, #0x8]
  5140cc: e3530000     	cmp	r3, #0
  5140d0: 1afffffb     	bne	0x5140c4 <PropertyMap::SetTemplate(std::string const&)+0xd8> @ imm = #-0x14
  5140d4: e1a07002     	mov	r7, r2
  5140d8: eaffffe9     	b	0x514084 <PropertyMap::SetTemplate(std::string const&)+0x98> @ imm = #-0x5c
  5140dc: e5973004     	ldr	r3, [r7, #0x4]
  5140e0: e593100c     	ldr	r1, [r3, #0xc]
  5140e4: e1570001     	cmp	r7, r1
  5140e8: 1a000005     	bne	0x514104 <PropertyMap::SetTemplate(std::string const&)+0x118> @ imm = #0x14
  5140ec: e1a07003     	mov	r7, r3
  5140f0: e5933004     	ldr	r3, [r3, #0x4]
  5140f4: e593200c     	ldr	r2, [r3, #0xc]
  5140f8: e1520007     	cmp	r2, r7
  5140fc: 0afffffa     	beq	0x5140ec <PropertyMap::SetTemplate(std::string const&)+0x100> @ imm = #-0x18
  514100: e597200c     	ldr	r2, [r7, #0xc]
  514104: e1520003     	cmp	r2, r3
  514108: 11a07003     	movne	r7, r3
  51410c: eaffffdc     	b	0x514084 <PropertyMap::SetTemplate(std::string const&)+0x98> @ imm = #-0x90
  514110: e59f3050     	ldr	r3, [pc, #0x50]         @ 0x514168 <PropertyMap::SetTemplate(std::string const&)+0x17c>
  514114: e7973003     	ldr	r3, [r7, r3]
  514118: e5933000     	ldr	r3, [r3]
  51411c: e3530002     	cmp	r3, #2
  514120: 05888000     	streq	r8, [r8]
  514124: 0affffd5     	beq	0x514080 <PropertyMap::SetTemplate(std::string const&)+0x94> @ imm = #-0xac
  514128: e3530001     	cmp	r3, #1
  51412c: 1affffd3     	bne	0x514080 <PropertyMap::SetTemplate(std::string const&)+0x94> @ imm = #-0xb4
  514130: e59f0034     	ldr	r0, [pc, #0x34]         @ 0x51416c <PropertyMap::SetTemplate(std::string const&)+0x180>
  514134: e59f1034     	ldr	r1, [pc, #0x34]         @ 0x514170 <PropertyMap::SetTemplate(std::string const&)+0x184>
  514138: e59f2034     	ldr	r2, [pc, #0x34]         @ 0x514174 <PropertyMap::SetTemplate(std::string const&)+0x188>
  51413c: e7970000     	ldr	r0, [r7, r0]
  514140: e59f3030     	ldr	r3, [pc, #0x30]         @ 0x514178 <PropertyMap::SetTemplate(std::string const&)+0x18c>
  514144: e3a0c070     	mov	r12, #112
  514148: e08f1001     	add	r1, pc, r1
  51414c: e08f2002     	add	r2, pc, r2
  514150: e08f3003     	add	r3, pc, r3
  514154: e28000a8     	add	r0, r0, #168
  514158: e58dc000     	str	r12, [sp]
  51415c: ebf7e7a8     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x206160
  514160: eaffffc6     	b	0x514080 <PropertyMap::SetTemplate(std::string const&)+0x94> @ imm = #-0xe8
  514164: 88 0a 48 00  	.word	0x00480a88
  514168: c0 39 00 00  	.word	0x000039c0
  51416c: c0 19 00 00  	.word	0x000019c0
  514170: 90 a2 3a 00  	.word	0x003aa290
  514174: fc 7e 3c 00  	.word	0x003c7efc
  514178: a8 7e 3c 00  	.word	0x003c7ea8
