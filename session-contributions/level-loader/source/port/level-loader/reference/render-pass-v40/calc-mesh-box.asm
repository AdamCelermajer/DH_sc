
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0047211c <VisualObject::CalcMeshBox()>:
  47211c: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  472120: e59f25d4     	ldr	r2, [pc, #0x5d4]        @ 0x4726fc <VisualObject::CalcMeshBox()+0x5e0>
  472124: e590300c     	ldr	r3, [r0, #0xc]
  472128: e24dd064     	sub	sp, sp, #100
  47212c: e08f2002     	add	r2, pc, r2
  472130: e3530000     	cmp	r3, #0
  472134: e58d2008     	str	r2, [sp, #0x8]
  472138: e1a04000     	mov	r4, r0
  47213c: 0a0000b1     	beq	0x472408 <VisualObject::CalcMeshBox()+0x2ec> @ imm = #0x2c4
  472140: e1a00003     	mov	r0, r3
  472144: e5933000     	ldr	r3, [r3]
  472148: e1a0e00f     	mov	lr, pc
  47214c: e593f030     	ldr	pc, [r3, #0x30]
  472150: e5901000     	ldr	r1, [r0]
  472154: e1a03000     	mov	r3, r0
  472158: e594200c     	ldr	r2, [r4, #0xc]
  47215c: e5841010     	str	r1, [r4, #0x10]
  472160: e5901004     	ldr	r1, [r0, #0x4]
  472164: e1a00002     	mov	r0, r2
  472168: e5841014     	str	r1, [r4, #0x14]
  47216c: e5933008     	ldr	r3, [r3, #0x8]
  472170: e5843018     	str	r3, [r4, #0x18]
  472174: e5923000     	ldr	r3, [r2]
  472178: e1a0e00f     	mov	lr, pc
  47217c: e593f030     	ldr	pc, [r3, #0x30]
  472180: e590200c     	ldr	r2, [r0, #0xc]
  472184: e1a03000     	mov	r3, r0
  472188: e594000c     	ldr	r0, [r4, #0xc]
  47218c: e584201c     	str	r2, [r4, #0x1c]
  472190: e5932010     	ldr	r2, [r3, #0x10]
  472194: e5842020     	str	r2, [r4, #0x20]
  472198: e5933014     	ldr	r3, [r3, #0x14]
  47219c: e5843024     	str	r3, [r4, #0x24]
  4721a0: eb04943a     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0x1250e8
  4721a4: e5903000     	ldr	r3, [r0]
  4721a8: e1a0e00f     	mov	lr, pc
  4721ac: e593f090     	ldr	pc, [r3, #0x90]
  4721b0: e1a03000     	mov	r3, r0
  4721b4: e5901000     	ldr	r1, [r0]
  4721b8: e5940010     	ldr	r0, [r4, #0x10]
  4721bc: e5936004     	ldr	r6, [r3, #0x4]
  4721c0: e5935008     	ldr	r5, [r3, #0x8]
  4721c4: ebfa72e8     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x163460
  4721c8: e1a01006     	mov	r1, r6
  4721cc: e5840010     	str	r0, [r4, #0x10]
  4721d0: e5940014     	ldr	r0, [r4, #0x14]
  4721d4: ebfa72e4     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x163470
  4721d8: e1a01005     	mov	r1, r5
  4721dc: e5840014     	str	r0, [r4, #0x14]
  4721e0: e5940018     	ldr	r0, [r4, #0x18]
  4721e4: ebfa72e0     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x163480
  4721e8: e5840018     	str	r0, [r4, #0x18]
  4721ec: e594000c     	ldr	r0, [r4, #0xc]
  4721f0: eb049426     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0x125098
  4721f4: e5903000     	ldr	r3, [r0]
  4721f8: e1a0e00f     	mov	lr, pc
  4721fc: e593f090     	ldr	pc, [r3, #0x90]
  472200: e1a03000     	mov	r3, r0
  472204: e5901000     	ldr	r1, [r0]
  472208: e594001c     	ldr	r0, [r4, #0x1c]
  47220c: e5936004     	ldr	r6, [r3, #0x4]
  472210: e5935008     	ldr	r5, [r3, #0x8]
  472214: ebfa72d4     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x1634b0
  472218: e1a01006     	mov	r1, r6
  47221c: e584001c     	str	r0, [r4, #0x1c]
  472220: e5940020     	ldr	r0, [r4, #0x20]
  472224: ebfa72d0     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x1634c0
  472228: e1a01005     	mov	r1, r5
  47222c: e5840020     	str	r0, [r4, #0x20]
  472230: e5940024     	ldr	r0, [r4, #0x24]
  472234: ebfa72cc     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x1634d0
  472238: e5840024     	str	r0, [r4, #0x24]
  47223c: e5943008     	ldr	r3, [r4, #0x8]
  472240: e28d5010     	add	r5, sp, #16
  472244: e3a06000     	mov	r6, #0
  472248: e1a00003     	mov	r0, r3
  47224c: e5933000     	ldr	r3, [r3]
  472250: e1a0e00f     	mov	lr, pc
  472254: e593f040     	ldr	pc, [r3, #0x40]
  472258: e3a02041     	mov	r2, #65
  47225c: e1a01000     	mov	r1, r0
  472260: e1a00005     	mov	r0, r5
  472264: e5cd6050     	strb	r6, [sp, #0x50]
  472268: ebfa717e     	bl	0x30e868 <.plt+0xaf4>   @ imm = #-0x163a08
  47226c: e3a03000     	mov	r3, #0
  472270: e1a01005     	mov	r1, r5
  472274: e2840010     	add	r0, r4, #16
  472278: e58d3048     	str	r3, [sp, #0x48]
  47227c: e58d3040     	str	r3, [sp, #0x40]
  472280: e58d3044     	str	r3, [sp, #0x44]
  472284: e5cd6050     	strb	r6, [sp, #0x50]
  472288: ebfa82c6     	bl	0x312da8 <Point3D<float>::transform(glitch::core::CMatrix4<float> const&)> @ imm = #-0x15f4e8
  47228c: e1a01005     	mov	r1, r5
  472290: e284001c     	add	r0, r4, #28
  472294: ebfa82c3     	bl	0x312da8 <Point3D<float>::transform(glitch::core::CMatrix4<float> const&)> @ imm = #-0x15f4f4
  472298: e594701c     	ldr	r7, [r4, #0x1c]
  47229c: e5945010     	ldr	r5, [r4, #0x10]
  4722a0: e1a00007     	mov	r0, r7
  4722a4: e1a01005     	mov	r1, r5
  4722a8: ebfa7117     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x163ba4
  4722ac: e1a01005     	mov	r1, r5
  4722b0: e1500006     	cmp	r0, r6
  4722b4: e1a00007     	mov	r0, r7
  4722b8: 11a0b007     	movne	r11, r7
  4722bc: 01a0b005     	moveq	r11, r5
  4722c0: ebfa700c     	bl	0x30e2f8 <.plt+0x584>   @ imm = #-0x163fd0
  4722c4: e3500000     	cmp	r0, #0
  4722c8: e5946020     	ldr	r6, [r4, #0x20]
  4722cc: 01a07005     	moveq	r7, r5
  4722d0: e5945014     	ldr	r5, [r4, #0x14]
  4722d4: e1a00006     	mov	r0, r6
  4722d8: e584701c     	str	r7, [r4, #0x1c]
  4722dc: e1a01005     	mov	r1, r5
  4722e0: e584b010     	str	r11, [r4, #0x10]
  4722e4: ebfa7108     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x163be0
  4722e8: e1a01005     	mov	r1, r5
  4722ec: e3500000     	cmp	r0, #0
  4722f0: e1a00006     	mov	r0, r6
  4722f4: 11a09006     	movne	r9, r6
  4722f8: 01a09005     	moveq	r9, r5
  4722fc: ebfa6ffd     	bl	0x30e2f8 <.plt+0x584>   @ imm = #-0x16400c
  472300: e3500000     	cmp	r0, #0
  472304: e5948018     	ldr	r8, [r4, #0x18]
  472308: 01a06005     	moveq	r6, r5
  47230c: e5945024     	ldr	r5, [r4, #0x24]
  472310: e1a01008     	mov	r1, r8
  472314: e5846020     	str	r6, [r4, #0x20]
  472318: e1a00005     	mov	r0, r5
  47231c: e5849014     	str	r9, [r4, #0x14]
  472320: ebfa70f9     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x163c1c
  472324: e1a01008     	mov	r1, r8
  472328: e3500000     	cmp	r0, #0
  47232c: e1a00005     	mov	r0, r5
  472330: 11a0a005     	movne	r10, r5
  472334: 01a0a008     	moveq	r10, r8
  472338: ebfa6fee     	bl	0x30e2f8 <.plt+0x584>   @ imm = #-0x164048
  47233c: e3500000     	cmp	r0, #0
  472340: 01a05008     	moveq	r5, r8
  472344: e1a0100b     	mov	r1, r11
  472348: e5845024     	str	r5, [r4, #0x24]
  47234c: e1a00007     	mov	r0, r7
  472350: e584a018     	str	r10, [r4, #0x18]
  472354: ebfa7014     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x163fb0
  472358: e3a0143f     	mov	r1, #1056964608
  47235c: ebfa7282     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x1635f8
  472360: e1a01009     	mov	r1, r9
  472364: e1a07000     	mov	r7, r0
  472368: e1a00006     	mov	r0, r6
  47236c: ebfa700e     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x163fc8
  472370: e3a0143f     	mov	r1, #1056964608
  472374: ebfa727c     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x163610
  472378: e1a0100a     	mov	r1, r10
  47237c: e1a08000     	mov	r8, r0
  472380: e1a00005     	mov	r0, r5
  472384: ebfa7008     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x163fe0
  472388: e3a0143f     	mov	r1, #1056964608
  47238c: ebfa7276     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x163628
  472390: e59dc008     	ldr	r12, [sp, #0x8]
  472394: e59f3364     	ldr	r3, [pc, #0x364]        @ 0x472700 <VisualObject::CalcMeshBox()+0x5e4>
  472398: e1a06000     	mov	r6, r0
  47239c: e1a01007     	mov	r1, r7
  4723a0: e79c5003     	ldr	r5, [r12, r3]
  4723a4: e5950000     	ldr	r0, [r5]
  4723a8: ebfa6fff     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x164004
  4723ac: e5840010     	str	r0, [r4, #0x10]
  4723b0: e5951000     	ldr	r1, [r5]
  4723b4: e1a00007     	mov	r0, r7
  4723b8: ebfa71f9     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x16381c
  4723bc: e584001c     	str	r0, [r4, #0x1c]
  4723c0: e5950004     	ldr	r0, [r5, #0x4]
  4723c4: e1a01008     	mov	r1, r8
  4723c8: ebfa6ff7     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x164024
  4723cc: e5840014     	str	r0, [r4, #0x14]
  4723d0: e5951004     	ldr	r1, [r5, #0x4]
  4723d4: e1a00008     	mov	r0, r8
  4723d8: ebfa71f1     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x16383c
  4723dc: e5840020     	str	r0, [r4, #0x20]
  4723e0: e5950008     	ldr	r0, [r5, #0x8]
  4723e4: e1a01006     	mov	r1, r6
  4723e8: ebfa6fef     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x164044
  4723ec: e5840018     	str	r0, [r4, #0x18]
  4723f0: e5951008     	ldr	r1, [r5, #0x8]
  4723f4: e1a00006     	mov	r0, r6
  4723f8: ebfa71e9     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x16385c
  4723fc: e5840024     	str	r0, [r4, #0x24]
  472400: e28dd064     	add	sp, sp, #100
  472404: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  472408: e59dc008     	ldr	r12, [sp, #0x8]
  47240c: e59f02f0     	ldr	r0, [pc, #0x2f0]        @ 0x472704 <VisualObject::CalcMeshBox()+0x5e8>
  472410: e3e01102     	mvn	r1, #-2147483648
  472414: e2411502     	sub	r1, r1, #8388608
  472418: e79c5000     	ldr	r5, [r12, r0]
  47241c: e3e02502     	mvn	r2, #8388608
  472420: e5841018     	str	r1, [r4, #0x18]
  472424: e5841010     	str	r1, [r4, #0x10]
  472428: e5841014     	str	r1, [r4, #0x14]
  47242c: e5842024     	str	r2, [r4, #0x24]
  472430: e584201c     	str	r2, [r4, #0x1c]
  472434: e5842020     	str	r2, [r4, #0x20]
  472438: e5952010     	ldr	r2, [r5, #0x10]
  47243c: e58d305c     	str	r3, [sp, #0x5c]
  472440: e58d3054     	str	r3, [sp, #0x54]
  472444: e58d3058     	str	r3, [sp, #0x58]
  472448: e592301c     	ldr	r3, [r2, #0x1c]
  47244c: e28d6054     	add	r6, sp, #84
  472450: e3061164     	movw	r1, #0x6164
  472454: e1a00003     	mov	r0, r3
  472458: e593c000     	ldr	r12, [r3]
  47245c: e3471365     	movt	r1, #0x7365
  472460: e5943008     	ldr	r3, [r4, #0x8]
  472464: e1a02006     	mov	r2, r6
  472468: e1a0e00f     	mov	lr, pc
  47246c: e59cf020     	ldr	pc, [r12, #0x20]
  472470: e59d0054     	ldr	r0, [sp, #0x54]
  472474: e59d3058     	ldr	r3, [sp, #0x58]
  472478: e0603003     	rsb	r3, r0, r3
  47247c: e1b03143     	asrs	r3, r3, #2
  472480: e58d300c     	str	r3, [sp, #0xc]
  472484: 0a000081     	beq	0x472690 <VisualObject::CalcMeshBox()+0x574> @ imm = #0x204
  472488: e59d200c     	ldr	r2, [sp, #0xc]
  47248c: e3520000     	cmp	r2, #0
  472490: 0a00007a     	beq	0x472680 <VisualObject::CalcMeshBox()+0x564> @ imm = #0x1e8
  472494: e3a05000     	mov	r5, #0
  472498: ea000000     	b	0x4724a0 <VisualObject::CalcMeshBox()+0x384> @ imm = #0x0
  47249c: e59d0054     	ldr	r0, [sp, #0x54]
  4724a0: e7903105     	ldr	r3, [r0, r5, lsl #2]
  4724a4: e1a00003     	mov	r0, r3
  4724a8: e5933000     	ldr	r3, [r3]
  4724ac: e1a0e00f     	mov	lr, pc
  4724b0: e593f030     	ldr	pc, [r3, #0x30]
  4724b4: e59d3054     	ldr	r3, [sp, #0x54]
  4724b8: e590a008     	ldr	r10, [r0, #0x8]
  4724bc: e590b000     	ldr	r11, [r0]
  4724c0: e7933105     	ldr	r3, [r3, r5, lsl #2]
  4724c4: e5909004     	ldr	r9, [r0, #0x4]
  4724c8: e1a00003     	mov	r0, r3
  4724cc: e5933000     	ldr	r3, [r3]
  4724d0: e1a0e00f     	mov	lr, pc
  4724d4: e593f030     	ldr	pc, [r3, #0x30]
  4724d8: e59d2054     	ldr	r2, [sp, #0x54]
  4724dc: e1a03000     	mov	r3, r0
  4724e0: e5906014     	ldr	r6, [r0, #0x14]
  4724e4: e590800c     	ldr	r8, [r0, #0xc]
  4724e8: e7920105     	ldr	r0, [r2, r5, lsl #2]
  4724ec: e5937010     	ldr	r7, [r3, #0x10]
  4724f0: eb049366     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0x124d98
  4724f4: e3500000     	cmp	r0, #0
  4724f8: 0a00002f     	beq	0x4725bc <VisualObject::CalcMeshBox()+0x4a0> @ imm = #0xbc
  4724fc: e59d3054     	ldr	r3, [sp, #0x54]
  472500: e7930105     	ldr	r0, [r3, r5, lsl #2]
  472504: eb049361     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0x124d84
  472508: e5903000     	ldr	r3, [r0]
  47250c: e1a0e00f     	mov	lr, pc
  472510: e593f090     	ldr	pc, [r3, #0x90]
  472514: e1a02000     	mov	r2, r0
  472518: e5923004     	ldr	r3, [r2, #0x4]
  47251c: e5922008     	ldr	r2, [r2, #0x8]
  472520: e5901000     	ldr	r1, [r0]
  472524: e1a0000b     	mov	r0, r11
  472528: e88d000c     	stm	sp, {r2, r3}
  47252c: ebfa720e     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x1637c8
  472530: e59d3004     	ldr	r3, [sp, #0x4]
  472534: e1a0b000     	mov	r11, r0
  472538: e1a00009     	mov	r0, r9
  47253c: e1a01003     	mov	r1, r3
  472540: ebfa7209     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x1637dc
  472544: e59d2000     	ldr	r2, [sp]
  472548: e1a09000     	mov	r9, r0
  47254c: e1a0000a     	mov	r0, r10
  472550: e1a01002     	mov	r1, r2
  472554: ebfa7204     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x1637f0
  472558: e59d3054     	ldr	r3, [sp, #0x54]
  47255c: e1a0a000     	mov	r10, r0
  472560: e7930105     	ldr	r0, [r3, r5, lsl #2]
  472564: eb049349     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0x124d24
  472568: e5903000     	ldr	r3, [r0]
  47256c: e1a0e00f     	mov	lr, pc
  472570: e593f090     	ldr	pc, [r3, #0x90]
  472574: e1a02000     	mov	r2, r0
  472578: e5923004     	ldr	r3, [r2, #0x4]
  47257c: e5922008     	ldr	r2, [r2, #0x8]
  472580: e5901000     	ldr	r1, [r0]
  472584: e1a00008     	mov	r0, r8
  472588: e88d000c     	stm	sp, {r2, r3}
  47258c: ebfa71f6     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x163828
  472590: e59d3004     	ldr	r3, [sp, #0x4]
  472594: e1a08000     	mov	r8, r0
  472598: e1a00007     	mov	r0, r7
  47259c: e1a01003     	mov	r1, r3
  4725a0: ebfa71f1     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x16383c
  4725a4: e59d2000     	ldr	r2, [sp]
  4725a8: e1a07000     	mov	r7, r0
  4725ac: e1a00006     	mov	r0, r6
  4725b0: e1a01002     	mov	r1, r2
  4725b4: ebfa71ec     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x163850
  4725b8: e1a06000     	mov	r6, r0
  4725bc: e5943010     	ldr	r3, [r4, #0x10]
  4725c0: e1a0100b     	mov	r1, r11
  4725c4: e2855001     	add	r5, r5, #1
  4725c8: e1a00003     	mov	r0, r3
  4725cc: e58d3004     	str	r3, [sp, #0x4]
  4725d0: ebfa6f48     	bl	0x30e2f8 <.plt+0x584>   @ imm = #-0x1642e0
  4725d4: e3500000     	cmp	r0, #0
  4725d8: e59d3004     	ldr	r3, [sp, #0x4]
  4725dc: 11a0300b     	movne	r3, r11
  4725e0: e594b014     	ldr	r11, [r4, #0x14]
  4725e4: e5843010     	str	r3, [r4, #0x10]
  4725e8: e1a01009     	mov	r1, r9
  4725ec: e1a0000b     	mov	r0, r11
  4725f0: ebfa6f40     	bl	0x30e2f8 <.plt+0x584>   @ imm = #-0x164300
  4725f4: e3500000     	cmp	r0, #0
  4725f8: 11a0b009     	movne	r11, r9
  4725fc: e5949018     	ldr	r9, [r4, #0x18]
  472600: e1a0100a     	mov	r1, r10
  472604: e584b014     	str	r11, [r4, #0x14]
  472608: e1a00009     	mov	r0, r9
  47260c: ebfa6f39     	bl	0x30e2f8 <.plt+0x584>   @ imm = #-0x16431c
  472610: e3500000     	cmp	r0, #0
  472614: 11a0900a     	movne	r9, r10
  472618: e594a01c     	ldr	r10, [r4, #0x1c]
  47261c: e1a01008     	mov	r1, r8
  472620: e5849018     	str	r9, [r4, #0x18]
  472624: e1a0000a     	mov	r0, r10
  472628: ebfa7037     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x163f24
  47262c: e3500000     	cmp	r0, #0
  472630: 11a0a008     	movne	r10, r8
  472634: e5948020     	ldr	r8, [r4, #0x20]
  472638: e1a01007     	mov	r1, r7
  47263c: e584a01c     	str	r10, [r4, #0x1c]
  472640: e1a00008     	mov	r0, r8
  472644: ebfa7030     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x163f40
  472648: e3500000     	cmp	r0, #0
  47264c: 11a08007     	movne	r8, r7
  472650: e5947024     	ldr	r7, [r4, #0x24]
  472654: e5848020     	str	r8, [r4, #0x20]
  472658: e1a01006     	mov	r1, r6
  47265c: e1a00007     	mov	r0, r7
  472660: ebfa7029     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x163f5c
  472664: e59d300c     	ldr	r3, [sp, #0xc]
  472668: e3500000     	cmp	r0, #0
  47266c: 11a07006     	movne	r7, r6
  472670: e1550003     	cmp	r5, r3
  472674: e5847024     	str	r7, [r4, #0x24]
  472678: 1affff87     	bne	0x47249c <VisualObject::CalcMeshBox()+0x380> @ imm = #-0x1e4
  47267c: e59d0054     	ldr	r0, [sp, #0x54]
  472680: e3500000     	cmp	r0, #0
  472684: 0afffeec     	beq	0x47223c <VisualObject::CalcMeshBox()+0x120> @ imm = #-0x450
  472688: ebfa7770     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x162240
  47268c: eafffeea     	b	0x47223c <VisualObject::CalcMeshBox()+0x120> @ imm = #-0x458
  472690: e5953010     	ldr	r3, [r5, #0x10]
  472694: e3061164     	movw	r1, #0x6164
  472698: e3461d65     	movt	r1, #0x6d65
  47269c: e593c01c     	ldr	r12, [r3, #0x1c]
  4726a0: e1a02006     	mov	r2, r6
  4726a4: e5943008     	ldr	r3, [r4, #0x8]
  4726a8: e1a0000c     	mov	r0, r12
  4726ac: e59cc000     	ldr	r12, [r12]
  4726b0: e1a0e00f     	mov	lr, pc
  4726b4: e59cf020     	ldr	pc, [r12, #0x20]
  4726b8: e59d0054     	ldr	r0, [sp, #0x54]
  4726bc: e59d3058     	ldr	r3, [sp, #0x58]
  4726c0: e0603003     	rsb	r3, r0, r3
  4726c4: e1b03143     	asrs	r3, r3, #2
  4726c8: e58d300c     	str	r3, [sp, #0xc]
  4726cc: 1affff6d     	bne	0x472488 <VisualObject::CalcMeshBox()+0x36c> @ imm = #-0x24c
  4726d0: e3a03000     	mov	r3, #0
  4726d4: e3500000     	cmp	r0, #0
  4726d8: e5843024     	str	r3, [r4, #0x24]
  4726dc: e5843010     	str	r3, [r4, #0x10]
  4726e0: e5843014     	str	r3, [r4, #0x14]
  4726e4: e5843018     	str	r3, [r4, #0x18]
  4726e8: e584301c     	str	r3, [r4, #0x1c]
  4726ec: e5843020     	str	r3, [r4, #0x20]
  4726f0: 0affff42     	beq	0x472400 <VisualObject::CalcMeshBox()+0x2e4> @ imm = #-0x2f8
  4726f4: ebfa7755     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x1622ac
  4726f8: eaffff40     	b	0x472400 <VisualObject::CalcMeshBox()+0x2e4> @ imm = #-0x300
  4726fc: 64 29 52 00  	.word	0x00522964
  472700: 2c 3f 00 00  	.word	0x00003f2c
  472704: f4 37 00 00  	.word	0x000037f4
