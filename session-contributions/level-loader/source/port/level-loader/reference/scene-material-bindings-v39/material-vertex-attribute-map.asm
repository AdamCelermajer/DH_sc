
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00634520 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)>:
  634520: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  634524: e1a04003     	mov	r4, r3
  634528: e5933034     	ldr	r3, [r3, #0x34]
  63452c: e24dd04c     	sub	sp, sp, #76
  634530: e58d0018     	str	r0, [sp, #0x18]
  634534: e3530000     	cmp	r3, #0
  634538: e58d3044     	str	r3, [sp, #0x44]
  63453c: 15931000     	ldrne	r1, [r3]
  634540: e1a06002     	mov	r6, r2
  634544: e5dd207c     	ldrb	r2, [sp, #0x7c]
  634548: 12811001     	addne	r1, r1, #1
  63454c: 15831000     	strne	r1, [r3]
  634550: 15943034     	ldrne	r3, [r4, #0x34]
  634554: e3530000     	cmp	r3, #0
  634558: 0a000001     	beq	0x634564 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x44> @ imm = #0x4
  63455c: e3520000     	cmp	r2, #0
  634560: 0a0000d1     	beq	0x6348ac <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x38c> @ imm = #0x344
  634564: e59d3074     	ldr	r3, [sp, #0x74]
  634568: e5933000     	ldr	r3, [r3]
  63456c: e5933004     	ldr	r3, [r3, #0x4]
  634570: e3530000     	cmp	r3, #0
  634574: e58d3040     	str	r3, [sp, #0x40]
  634578: 15932000     	ldrne	r2, [r3]
  63457c: 12822001     	addne	r2, r2, #1
  634580: 15832000     	strne	r2, [r3]
  634584: 159d3040     	ldrne	r3, [sp, #0x40]
  634588: e5933004     	ldr	r3, [r3, #0x4]
  63458c: e1a00003     	mov	r0, r3
  634590: e5933000     	ldr	r3, [r3]
  634594: e1a0e00f     	mov	lr, pc
  634598: e593f05c     	ldr	pc, [r3, #0x5c]
  63459c: e3100007     	tst	r0, #7
  6345a0: 1284701c     	addne	r7, r4, #28
  6345a4: 1a000002     	bne	0x6345b4 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x94> @ imm = #0x8
  6345a8: e3100018     	tst	r0, #24
  6345ac: 12847024     	addne	r7, r4, #36
  6345b0: 0a0000df     	beq	0x634934 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x414> @ imm = #0x37c
  6345b4: e28d2040     	add	r2, sp, #64
  6345b8: e28d503c     	add	r5, sp, #60
  6345bc: e1a01002     	mov	r1, r2
  6345c0: e1a00005     	mov	r0, r5
  6345c4: e58d201c     	str	r2, [sp, #0x1c]
  6345c8: ebfeab5b     	bl	0x5df33c <glitch::video::CMaterialVertexAttributeMap::allocate(boost::intrusive_ptr<glitch::video::CMaterialRenderer> const&)> @ imm = #-0x55294
  6345cc: e59d203c     	ldr	r2, [sp, #0x3c]
  6345d0: e3520000     	cmp	r2, #0
  6345d4: e58d2028     	str	r2, [sp, #0x28]
  6345d8: 0a000003     	beq	0x6345ec <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0xcc> @ imm = #0xc
  6345dc: e5923000     	ldr	r3, [r2]
  6345e0: e2833001     	add	r3, r3, #1
  6345e4: e5823000     	str	r3, [r2]
  6345e8: e59d2028     	ldr	r2, [sp, #0x28]
  6345ec: e59d3044     	ldr	r3, [sp, #0x44]
  6345f0: e28d0028     	add	r0, sp, #40
  6345f4: e58d2044     	str	r2, [sp, #0x44]
  6345f8: e58d3028     	str	r3, [sp, #0x28]
  6345fc: ebfd171a     	bl	0x57a26c <boost::intrusive_ptr<glitch::video::CMaterialVertexAttributeMap>::~intrusive_ptr()> @ imm = #-0xba398
  634600: e1a00005     	mov	r0, r5
  634604: ebfd1718     	bl	0x57a26c <boost::intrusive_ptr<glitch::video::CMaterialVertexAttributeMap>::~intrusive_ptr()> @ imm = #-0xba3a0
  634608: e5943034     	ldr	r3, [r4, #0x34]
  63460c: e3530000     	cmp	r3, #0
  634610: 0a0000d2     	beq	0x634960 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x440> @ imm = #0x348
  634614: e59d3070     	ldr	r3, [sp, #0x70]
  634618: e59d2078     	ldr	r2, [sp, #0x78]
  63461c: e28d0034     	add	r0, sp, #52
  634620: e5933000     	ldr	r3, [r3]
  634624: e1a01003     	mov	r1, r3
  634628: e5933000     	ldr	r3, [r3]
  63462c: e1a0e00f     	mov	lr, pc
  634630: e593f014     	ldr	pc, [r3, #0x14]
  634634: e59d0034     	ldr	r0, [sp, #0x34]
  634638: e5903014     	ldr	r3, [r0, #0x14]
  63463c: e3530000     	cmp	r3, #0
  634640: e58d3038     	str	r3, [sp, #0x38]
  634644: 15932000     	ldrne	r2, [r3]
  634648: 12822001     	addne	r2, r2, #1
  63464c: 15832000     	strne	r2, [r3]
  634650: 159d0034     	ldrne	r0, [sp, #0x34]
  634654: e3500000     	cmp	r0, #0
  634658: 0a000000     	beq	0x634660 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x140> @ imm = #0x0
  63465c: ebf3a3c8     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x3170e0
  634660: e597c000     	ldr	r12, [r7]
  634664: e35c0000     	cmp	r12, #0
  634668: e58dc014     	str	r12, [sp, #0x14]
  63466c: d28d8038     	addle	r8, sp, #56
  634670: da000042     	ble	0x634780 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x260> @ imm = #0x108
  634674: e3a06000     	mov	r6, #0
  634678: e28d2030     	add	r2, sp, #48
  63467c: e58d6010     	str	r6, [sp, #0x10]
  634680: e28d8038     	add	r8, sp, #56
  634684: e58d200c     	str	r2, [sp, #0xc]
  634688: e5973004     	ldr	r3, [r7, #0x4]
  63468c: e59d0040     	ldr	r0, [sp, #0x40]
  634690: e7931006     	ldr	r1, [r3, r6]
  634694: ebfe801e     	bl	0x5d4714 <glitch::video::CMaterialRenderer::getTechniqueID(char const*) const> @ imm = #-0x5ff88
  634698: e35000ff     	cmp	r0, #255
  63469c: e1a0a000     	mov	r10, r0
  6346a0: 0a00002f     	beq	0x634764 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x244> @ imm = #0xbc
  6346a4: e5973004     	ldr	r3, [r7, #0x4]
  6346a8: e0833006     	add	r3, r3, r6
  6346ac: e5939004     	ldr	r9, [r3, #0x4]
  6346b0: e3590000     	cmp	r9, #0
  6346b4: da00002a     	ble	0x634764 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x244> @ imm = #0xa8
  6346b8: e3a05000     	mov	r5, #0
  6346bc: e1a04005     	mov	r4, r5
  6346c0: e3a01000     	mov	r1, #0
  6346c4: e3a00024     	mov	r0, #36
  6346c8: ebfbfeb7     	bl	0x5341ac <operator new(unsigned int, glitch::memory::E_MEMORY_HINT)> @ imm = #-0x100524
  6346cc: e1a01008     	mov	r1, r8
  6346d0: e1a0b000     	mov	r11, r0
  6346d4: ebfdb09f     	bl	0x5a0958 <glitch::video::CVertexAttributeMap::CVertexAttributeMap(boost::intrusive_ptr<glitch::video::CVertexStreams const> const&)> @ imm = #-0x93d84
  6346d8: e35b0000     	cmp	r11, #0
  6346dc: e58db030     	str	r11, [sp, #0x30]
  6346e0: 159b3000     	ldrne	r3, [r11]
  6346e4: 01a0000b     	moveq	r0, r11
  6346e8: e3a0c000     	mov	r12, #0
  6346ec: 12833001     	addne	r3, r3, #1
  6346f0: 158b3000     	strne	r3, [r11]
  6346f4: e5973004     	ldr	r3, [r7, #0x4]
  6346f8: 159d0030     	ldrne	r0, [sp, #0x30]
  6346fc: e1a01008     	mov	r1, r8
  634700: e0833006     	add	r3, r3, r6
  634704: e5932008     	ldr	r2, [r3, #0x8]
  634708: e0822005     	add	r2, r2, r5
  63470c: e992000c     	ldmib	r2, {r2, r3}
  634710: e58dc000     	str	r12, [sp]
  634714: ebfdaffb     	bl	0x5a0708 <glitch::video::CVertexAttributeMap::set(boost::intrusive_ptr<glitch::video::CVertexStreams const> const&, unsigned int, unsigned char const*, bool)> @ imm = #-0x94014
  634718: e6ef2074     	uxtb	r2, r4
  63471c: e59d0044     	ldr	r0, [sp, #0x44]
  634720: e59d300c     	ldr	r3, [sp, #0xc]
  634724: e1a0100a     	mov	r1, r10
  634728: ebfeac39     	bl	0x5df814 <glitch::video::CMaterialVertexAttributeMap::set(unsigned char, unsigned char, boost::intrusive_ptr<glitch::video::CVertexAttributeMap> const&)> @ imm = #-0x54f1c
  63472c: e59d3030     	ldr	r3, [sp, #0x30]
  634730: e2844001     	add	r4, r4, #1
  634734: e285500c     	add	r5, r5, #12
  634738: e3530000     	cmp	r3, #0
  63473c: e1a00003     	mov	r0, r3
  634740: 0a000005     	beq	0x63475c <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x23c> @ imm = #0x14
  634744: e5932000     	ldr	r2, [r3]
  634748: e2422001     	sub	r2, r2, #1
  63474c: e3520000     	cmp	r2, #0
  634750: e5832000     	str	r2, [r3]
  634754: 1a000000     	bne	0x63475c <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x23c> @ imm = #0x0
  634758: ebf366d4     	bl	0x30e2b0 <.plt+0x53c>   @ imm = #-0x3264b0
  63475c: e1540009     	cmp	r4, r9
  634760: 1affffd6     	bne	0x6346c0 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x1a0> @ imm = #-0xa8
  634764: e59d2010     	ldr	r2, [sp, #0x10]
  634768: e59d3014     	ldr	r3, [sp, #0x14]
  63476c: e286600c     	add	r6, r6, #12
  634770: e2822001     	add	r2, r2, #1
  634774: e1520003     	cmp	r2, r3
  634778: e58d2010     	str	r2, [sp, #0x10]
  63477c: 1affffc1     	bne	0x634688 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x168> @ imm = #-0xfc
  634780: e59d3040     	ldr	r3, [sp, #0x40]
  634784: e3a06000     	mov	r6, #0
  634788: e58d602c     	str	r6, [sp, #0x2c]
  63478c: e5d32010     	ldrb	r2, [r3, #0x10]
  634790: e1520006     	cmp	r2, r6
  634794: 0a000040     	beq	0x63489c <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x37c> @ imm = #0x100
  634798: e304aec5     	movw	r10, #0x4ec5
  63479c: e58d800c     	str	r8, [sp, #0xc]
  6347a0: e34ca4ec     	movt	r10, #0xc4ec
  6347a4: e1a01006     	mov	r1, r6
  6347a8: e1a09006     	mov	r9, r6
  6347ac: e28db02c     	add	r11, sp, #44
  6347b0: e1a08002     	mov	r8, r2
  6347b4: e5933018     	ldr	r3, [r3, #0x18]
  6347b8: e0833006     	add	r3, r3, r6
  6347bc: e5d37004     	ldrb	r7, [r3, #0x4]
  6347c0: e3570000     	cmp	r7, #0
  6347c4: 0a000022     	beq	0x634854 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x334> @ imm = #0x88
  6347c8: e3a05000     	mov	r5, #0
  6347cc: e1a04005     	mov	r4, r5
  6347d0: ea000005     	b	0x6347ec <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x2cc> @ imm = #0x14
  6347d4: e2844001     	add	r4, r4, #1
  6347d8: e6ef4074     	uxtb	r4, r4
  6347dc: e1540007     	cmp	r4, r7
  6347e0: e2855034     	add	r5, r5, #52
  6347e4: 0a00001a     	beq	0x634854 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x334> @ imm = #0x68
  6347e8: e59d102c     	ldr	r1, [sp, #0x2c]
  6347ec: e59d0044     	ldr	r0, [sp, #0x44]
  6347f0: e5903004     	ldr	r3, [r0, #0x4]
  6347f4: e5932018     	ldr	r2, [r3, #0x18]
  6347f8: e593301c     	ldr	r3, [r3, #0x1c]
  6347fc: e0822006     	add	r2, r2, r6
  634800: e5922008     	ldr	r2, [r2, #0x8]
  634804: e0822005     	add	r2, r2, r5
  634808: e0633002     	rsb	r3, r3, r2
  63480c: e1a03143     	asr	r3, r3, #2
  634810: e003039a     	mul	r3, r10, r3
  634814: e0803103     	add	r3, r0, r3, lsl #2
  634818: e5933008     	ldr	r3, [r3, #0x8]
  63481c: e3530000     	cmp	r3, #0
  634820: 1affffeb     	bne	0x6347d4 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x2b4> @ imm = #-0x54
  634824: e3510000     	cmp	r1, #0
  634828: 0a00002b     	beq	0x6348dc <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x3bc> @ imm = #0xac
  63482c: e1a02004     	mov	r2, r4
  634830: e2844001     	add	r4, r4, #1
  634834: e1a01009     	mov	r1, r9
  634838: e1a0300b     	mov	r3, r11
  63483c: e6ef4074     	uxtb	r4, r4
  634840: ebfeabf3     	bl	0x5df814 <glitch::video::CMaterialVertexAttributeMap::set(unsigned char, unsigned char, boost::intrusive_ptr<glitch::video::CVertexAttributeMap> const&)> @ imm = #-0x55034
  634844: e1540007     	cmp	r4, r7
  634848: e59d102c     	ldr	r1, [sp, #0x2c]
  63484c: e2855034     	add	r5, r5, #52
  634850: 1affffe4     	bne	0x6347e8 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x2c8> @ imm = #-0x70
  634854: e2899001     	add	r9, r9, #1
  634858: e6ef9079     	uxtb	r9, r9
  63485c: e1590008     	cmp	r9, r8
  634860: e286600c     	add	r6, r6, #12
  634864: 0a000002     	beq	0x634874 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x354> @ imm = #0x8
  634868: e59d3040     	ldr	r3, [sp, #0x40]
  63486c: e59d102c     	ldr	r1, [sp, #0x2c]
  634870: eaffffcf     	b	0x6347b4 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x294> @ imm = #-0xc4
  634874: e3510000     	cmp	r1, #0
  634878: e59d800c     	ldr	r8, [sp, #0xc]
  63487c: 0a000006     	beq	0x63489c <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x37c> @ imm = #0x18
  634880: e5913000     	ldr	r3, [r1]
  634884: e2433001     	sub	r3, r3, #1
  634888: e3530000     	cmp	r3, #0
  63488c: e5813000     	str	r3, [r1]
  634890: 1a000001     	bne	0x63489c <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x37c> @ imm = #0x4
  634894: e1a00001     	mov	r0, r1
  634898: ebf36684     	bl	0x30e2b0 <.plt+0x53c>   @ imm = #-0x3265f0
  63489c: e1a00008     	mov	r0, r8
  6348a0: ebf4a8ba     	bl	0x35eb90 <boost::intrusive_ptr<glitch::video::CVertexStreams const>::~intrusive_ptr()> @ imm = #-0x2d5d18
  6348a4: e59d001c     	ldr	r0, [sp, #0x1c]
  6348a8: ebf47682     	bl	0x3522b8 <boost::intrusive_ptr<glitch::video::CMaterialRenderer>::~intrusive_ptr()> @ imm = #-0x2e25f8
  6348ac: e59d3044     	ldr	r3, [sp, #0x44]
  6348b0: e59dc018     	ldr	r12, [sp, #0x18]
  6348b4: e3530000     	cmp	r3, #0
  6348b8: e58c3000     	str	r3, [r12]
  6348bc: 15932000     	ldrne	r2, [r3]
  6348c0: 12822001     	addne	r2, r2, #1
  6348c4: 15832000     	strne	r2, [r3]
  6348c8: e28d0044     	add	r0, sp, #68
  6348cc: ebfd1666     	bl	0x57a26c <boost::intrusive_ptr<glitch::video::CMaterialVertexAttributeMap>::~intrusive_ptr()> @ imm = #-0xba668
  6348d0: e59d0018     	ldr	r0, [sp, #0x18]
  6348d4: e28dd04c     	add	sp, sp, #76
  6348d8: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  6348dc: e3a00024     	mov	r0, #36
  6348e0: ebfbfe31     	bl	0x5341ac <operator new(unsigned int, glitch::memory::E_MEMORY_HINT)> @ imm = #-0x10073c
  6348e4: e59d100c     	ldr	r1, [sp, #0xc]
  6348e8: e58d0008     	str	r0, [sp, #0x8]
  6348ec: ebfdb019     	bl	0x5a0958 <glitch::video::CVertexAttributeMap::CVertexAttributeMap(boost::intrusive_ptr<glitch::video::CVertexStreams const> const&)> @ imm = #-0x93f9c
  6348f0: e59d3008     	ldr	r3, [sp, #0x8]
  6348f4: e3530000     	cmp	r3, #0
  6348f8: 15932000     	ldrne	r2, [r3]
  6348fc: 12822001     	addne	r2, r2, #1
  634900: 15832000     	strne	r2, [r3]
  634904: e59d002c     	ldr	r0, [sp, #0x2c]
  634908: e58d302c     	str	r3, [sp, #0x2c]
  63490c: e3500000     	cmp	r0, #0
  634910: 0a000005     	beq	0x63492c <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x40c> @ imm = #0x14
  634914: e5903000     	ldr	r3, [r0]
  634918: e2433001     	sub	r3, r3, #1
  63491c: e3530000     	cmp	r3, #0
  634920: e5803000     	str	r3, [r0]
  634924: 1a000000     	bne	0x63492c <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x40c> @ imm = #0x0
  634928: ebf36660     	bl	0x30e2b0 <.plt+0x53c>   @ imm = #-0x326680
  63492c: e59d0044     	ldr	r0, [sp, #0x44]
  634930: eaffffbd     	b	0x63482c <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x30c> @ imm = #-0x10c
  634934: e3100060     	tst	r0, #96
  634938: 12847014     	addne	r7, r4, #20
  63493c: 1affff1c     	bne	0x6345b4 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x94> @ imm = #-0x390
  634940: e2100c03     	ands	r0, r0, #768
  634944: 1284702c     	addne	r7, r4, #44
  634948: 1affff19     	bne	0x6345b4 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x94> @ imm = #-0x39c
  63494c: e59d3018     	ldr	r3, [sp, #0x18]
  634950: e5830000     	str	r0, [r3]
  634954: e28d0040     	add	r0, sp, #64
  634958: ebf47656     	bl	0x3522b8 <boost::intrusive_ptr<glitch::video::CMaterialRenderer>::~intrusive_ptr()> @ imm = #-0x2e26a8
  63495c: eaffffd9     	b	0x6348c8 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0x3a8> @ imm = #-0x9c
  634960: e59d2044     	ldr	r2, [sp, #0x44]
  634964: e28d0048     	add	r0, sp, #72
  634968: e58d2024     	str	r2, [sp, #0x24]
  63496c: e3520000     	cmp	r2, #0
  634970: 15923000     	ldrne	r3, [r2]
  634974: 12833001     	addne	r3, r3, #1
  634978: 15823000     	strne	r3, [r2]
  63497c: 15943034     	ldrne	r3, [r4, #0x34]
  634980: e59d2024     	ldr	r2, [sp, #0x24]
  634984: e5203024     	str	r3, [r0, #-0x24]!
  634988: e5842034     	str	r2, [r4, #0x34]
  63498c: ebfd1636     	bl	0x57a26c <boost::intrusive_ptr<glitch::video::CMaterialVertexAttributeMap>::~intrusive_ptr()> @ imm = #-0xba728
  634990: e1a00006     	mov	r0, r6
  634994: e1a01004     	mov	r1, r4
  634998: ebff663b     	bl	0x60e28c <glitch::collada::CColladaDatabase::linkInstanceMaterial(glitch::collada::SInstanceMaterial*) const> @ imm = #-0x26714
  63499c: eaffff1c     	b	0x634614 <glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)+0xf4> @ imm = #-0x390
