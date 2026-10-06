
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003aa1b4 <Character::Character(ObjectBase::GO_IDS)>:
  3aa1b4: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3aa1b8: e280cfdd     	add	r12, r0, #884
  3aa1bc: e24dd03c     	sub	sp, sp, #60
  3aa1c0: e1a04000     	mov	r4, r0
  3aa1c4: e58dc00c     	str	r12, [sp, #0xc]
  3aa1c8: ebff8872     	bl	0x38c398 <GameObject::GameObject(ObjectBase::GO_IDS)> @ imm = #-0x1de38
  3aa1cc: e59dc00c     	ldr	r12, [sp, #0xc]
  3aa1d0: e2845e4f     	add	r5, r4, #1264
  3aa1d4: e285500c     	add	r5, r5, #12
  3aa1d8: e1a0000c     	mov	r0, r12
  3aa1dc: eb016af5     	bl	0x404db8 <v2Controllable::v2Controllable()> @ imm = #0x5abd4
  3aa1e0: e2840fed     	add	r0, r4, #948
  3aa1e4: e58d0020     	str	r0, [sp, #0x20]
  3aa1e8: e2840fdf     	add	r0, r4, #892
  3aa1ec: eb01544f     	bl	0x3ff330 <ItemInventory::ItemInventory()> @ imm = #0x5513c
  3aa1f0: e2842e49     	add	r2, r4, #1168
  3aa1f4: e2841ff2     	add	r1, r4, #968
  3aa1f8: e282200c     	add	r2, r2, #12
  3aa1fc: e59d0020     	ldr	r0, [sp, #0x20]
  3aa200: e58d101c     	str	r1, [sp, #0x1c]
  3aa204: e58d2014     	str	r2, [sp, #0x14]
  3aa208: eb00c63f     	bl	0x3dbb0c <CharTimers::CharTimers()> @ imm = #0x318fc
  3aa20c: e59d001c     	ldr	r0, [sp, #0x1c]
  3aa210: eb009276     	bl	0x3cebf0 <CharAI::CharAI()> @ imm = #0x249d8
  3aa214: e59d0014     	ldr	r0, [sp, #0x14]
  3aa218: eb007b75     	bl	0x3c8ff4 <CharAnimator::CharAnimator()> @ imm = #0x1edd4
  3aa21c: e2843e56     	add	r3, r4, #1376
  3aa220: e1a00005     	mov	r0, r5
  3aa224: e58d3018     	str	r3, [sp, #0x18]
  3aa228: e59f950c     	ldr	r9, [pc, #0x50c]        @ 0x3aa73c <Character::Character(ObjectBase::GO_IDS)+0x588>
  3aa22c: eb005e49     	bl	0x3c1b58 <CharStateMachine::CharStateMachine()> @ imm = #0x17924
  3aa230: e59d0018     	ldr	r0, [sp, #0x18]
  3aa234: eb00d392     	bl	0x3df084 <CharProperties::CharProperties()> @ imm = #0x34e48
  3aa238: e59fe500     	ldr	lr, [pc, #0x500]        @ 0x3aa740 <Character::Character(ObjectBase::GO_IDS)+0x58c>
  3aa23c: e08f9009     	add	r9, pc, r9
  3aa240: e3a08000     	mov	r8, #0
  3aa244: e799e00e     	ldr	lr, [r9, lr]
  3aa248: e3a0b001     	mov	r11, #1
  3aa24c: e3e06000     	mvn	r6, #0
  3aa250: e28eafc9     	add	r10, lr, #804
  3aa254: e58da034     	str	r10, [sp, #0x34]
  3aa258: e28ead06     	add	r10, lr, #384
  3aa25c: e58da010     	str	r10, [sp, #0x10]
  3aa260: e28eaf7d     	add	r10, lr, #500
  3aa264: e58da024     	str	r10, [sp, #0x24]
  3aa268: e28eae22     	add	r10, lr, #544
  3aa26c: e58da028     	str	r10, [sp, #0x28]
  3aa270: e28eae23     	add	r10, lr, #560
  3aa274: e58da02c     	str	r10, [sp, #0x2c]
  3aa278: e28e0008     	add	r0, lr, #8
  3aa27c: e28e1f57     	add	r1, lr, #348
  3aa280: e28e2f5a     	add	r2, lr, #360
  3aa284: e28eafc1     	add	r10, lr, #772
  3aa288: e58da030     	str	r10, [sp, #0x30]
  3aa28c: e8840003     	stm	r4, {r0, r1}
  3aa290: e5842024     	str	r2, [r4, #0x24]
  3aa294: e59d0010     	ldr	r0, [sp, #0x10]
  3aa298: e28eefc5     	add	lr, lr, #788
  3aa29c: e2847d4e     	add	r7, r4, #4992
  3aa2a0: e5840374     	str	r0, [r4, #0x374]
  3aa2a4: e59d1024     	ldr	r1, [sp, #0x24]
  3aa2a8: e2873018     	add	r3, r7, #24
  3aa2ac: e301a3a8     	movw	r10, #0x13a8
  3aa2b0: e584137c     	str	r1, [r4, #0x37c]
  3aa2b4: e59d2028     	ldr	r2, [sp, #0x28]
  3aa2b8: e2877030     	add	r7, r7, #48
  3aa2bc: e58423b4     	str	r2, [r4, #0x3b4]
  3aa2c0: e59d002c     	ldr	r0, [sp, #0x2c]
  3aa2c4: e58403c8     	str	r0, [r4, #0x3c8]
  3aa2c8: e59d1030     	ldr	r1, [sp, #0x30]
  3aa2cc: e584e4fc     	str	lr, [r4, #0x4fc]
  3aa2d0: e1a00003     	mov	r0, r3
  3aa2d4: e584149c     	str	r1, [r4, #0x49c]
  3aa2d8: e59d2034     	ldr	r2, [sp, #0x34]
  3aa2dc: e3a01010     	mov	r1, #16
  3aa2e0: e5842560     	str	r2, [r4, #0x560]
  3aa2e4: e3012394     	movw	r2, #0x1394
  3aa2e8: e7c48002     	strb	r8, [r4, r2]
  3aa2ec: e3012395     	movw	r2, #0x1395
  3aa2f0: e7c48002     	strb	r8, [r4, r2]
  3aa2f4: e3012396     	movw	r2, #0x1396
  3aa2f8: e7c4b002     	strb	r11, [r4, r2]
  3aa2fc: e3012397     	movw	r2, #0x1397
  3aa300: e7c46002     	strb	r6, [r4, r2]
  3aa304: e30123ac     	movw	r2, #0x13ac
  3aa308: e7843002     	str	r3, [r4, r2]
  3aa30c: e784300a     	str	r3, [r4, r10]
  3aa310: ebfd9cd9     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x98c9c
  3aa314: e794300a     	ldr	r3, [r4, r10]
  3aa318: e3a0ad4f     	mov	r10, #5056
  3aa31c: e1a00007     	mov	r0, r7
  3aa320: e5c38000     	strb	r8, [r3]
  3aa324: e30133c4     	movw	r3, #0x13c4
  3aa328: e7847003     	str	r7, [r4, r3]
  3aa32c: e3a01010     	mov	r1, #16
  3aa330: e784700a     	str	r7, [r4, r10]
  3aa334: ebfd9cd0     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x98cc0
  3aa338: e794200a     	ldr	r2, [r4, r10]
  3aa33c: e084700a     	add	r7, r4, r10
  3aa340: e287300c     	add	r3, r7, #12
  3aa344: e5c28000     	strb	r8, [r2]
  3aa348: e30123c8     	movw	r2, #0x13c8
  3aa34c: e18460b2     	strh	r6, [r4, r2]
  3aa350: e30123ca     	movw	r2, #0x13ca
  3aa354: e18460b2     	strh	r6, [r4, r2]
  3aa358: e301a3dc     	movw	r10, #0x13dc
  3aa35c: e30123e0     	movw	r2, #0x13e0
  3aa360: e7843002     	str	r3, [r4, r2]
  3aa364: e1a00003     	mov	r0, r3
  3aa368: e784300a     	str	r3, [r4, r10]
  3aa36c: e3a01010     	mov	r1, #16
  3aa370: ebfd9cc1     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x98cfc
  3aa374: e794300a     	ldr	r3, [r4, r10]
  3aa378: e2877028     	add	r7, r7, #40
  3aa37c: e301a3f8     	movw	r10, #0x13f8
  3aa380: e5c38000     	strb	r8, [r3]
  3aa384: e30133e4     	movw	r3, #0x13e4
  3aa388: e7c4b003     	strb	r11, [r4, r3]
  3aa38c: e30133fc     	movw	r3, #0x13fc
  3aa390: e7847003     	str	r7, [r4, r3]
  3aa394: e1a00007     	mov	r0, r7
  3aa398: e784700a     	str	r7, [r4, r10]
  3aa39c: e3a01010     	mov	r1, #16
  3aa3a0: ebfd9cb5     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x98d2c
  3aa3a4: e794300a     	ldr	r3, [r4, r10]
  3aa3a8: e2847b05     	add	r7, r4, #5120
  3aa3ac: e301a410     	movw	r10, #0x1410
  3aa3b0: e5c38000     	strb	r8, [r3]
  3aa3b4: e3013414     	movw	r3, #0x1414
  3aa3b8: e7847003     	str	r7, [r4, r3]
  3aa3bc: e1a00007     	mov	r0, r7
  3aa3c0: e784700a     	str	r7, [r4, r10]
  3aa3c4: e3a01010     	mov	r1, #16
  3aa3c8: ebfd9cab     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x98d54
  3aa3cc: e794300a     	ldr	r3, [r4, r10]
  3aa3d0: e2877018     	add	r7, r7, #24
  3aa3d4: e301a428     	movw	r10, #0x1428
  3aa3d8: e5c38000     	strb	r8, [r3]
  3aa3dc: e301342c     	movw	r3, #0x142c
  3aa3e0: e7847003     	str	r7, [r4, r3]
  3aa3e4: e1a00007     	mov	r0, r7
  3aa3e8: e784700a     	str	r7, [r4, r10]
  3aa3ec: e3a01010     	mov	r1, #16
  3aa3f0: ebfd9ca1     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x98d7c
  3aa3f4: e794200a     	ldr	r2, [r4, r10]
  3aa3f8: e3a03000     	mov	r3, #0
  3aa3fc: e3a014bf     	mov	r1, #-1090519040
  3aa400: e5c28000     	strb	r8, [r2]
  3aa404: e30124a8     	movw	r2, #0x14a8
  3aa408: e7c46002     	strb	r6, [r4, r2]
  3aa40c: e3012430     	movw	r2, #0x1430
  3aa410: e7c4b002     	strb	r11, [r4, r2]
  3aa414: e3012434     	movw	r2, #0x1434
  3aa418: e7848002     	str	r8, [r4, r2]
  3aa41c: e3012438     	movw	r2, #0x1438
  3aa420: e7848002     	str	r8, [r4, r2]
  3aa424: e3012448     	movw	r2, #0x1448
  3aa428: e7c4b002     	strb	r11, [r4, r2]
  3aa42c: e3012449     	movw	r2, #0x1449
  3aa430: e7c48002     	strb	r8, [r4, r2]
  3aa434: e301244c     	movw	r2, #0x144c
  3aa438: e7848002     	str	r8, [r4, r2]
  3aa43c: e3012450     	movw	r2, #0x1450
  3aa440: e7843002     	str	r3, [r4, r2]
  3aa444: e3012454     	movw	r2, #0x1454
  3aa448: e7843002     	str	r3, [r4, r2]
  3aa44c: e3012458     	movw	r2, #0x1458
  3aa450: e7843002     	str	r3, [r4, r2]
  3aa454: e301245c     	movw	r2, #0x145c
  3aa458: e7843002     	str	r3, [r4, r2]
  3aa45c: e3012460     	movw	r2, #0x1460
  3aa460: e7843002     	str	r3, [r4, r2]
  3aa464: e3012464     	movw	r2, #0x1464
  3aa468: e7843002     	str	r3, [r4, r2]
  3aa46c: e3012468     	movw	r2, #0x1468
  3aa470: e7843002     	str	r3, [r4, r2]
  3aa474: e301246c     	movw	r2, #0x146c
  3aa478: e7843002     	str	r3, [r4, r2]
  3aa47c: e3012470     	movw	r2, #0x1470
  3aa480: e7843002     	str	r3, [r4, r2]
  3aa484: e3012474     	movw	r2, #0x1474
  3aa488: e7843002     	str	r3, [r4, r2]
  3aa48c: e3012478     	movw	r2, #0x1478
  3aa490: e7843002     	str	r3, [r4, r2]
  3aa494: e301247c     	movw	r2, #0x147c
  3aa498: e7843002     	str	r3, [r4, r2]
  3aa49c: e3a02d52     	mov	r2, #5248
  3aa4a0: e7c48002     	strb	r8, [r4, r2]
  3aa4a4: e3012481     	movw	r2, #0x1481
  3aa4a8: e7c48002     	strb	r8, [r4, r2]
  3aa4ac: e3012484     	movw	r2, #0x1484
  3aa4b0: e7848002     	str	r8, [r4, r2]
  3aa4b4: e3012488     	movw	r2, #0x1488
  3aa4b8: e7848002     	str	r8, [r4, r2]
  3aa4bc: e301248c     	movw	r2, #0x148c
  3aa4c0: e7848002     	str	r8, [r4, r2]
  3aa4c4: e3012490     	movw	r2, #0x1490
  3aa4c8: e7848002     	str	r8, [r4, r2]
  3aa4cc: e3012494     	movw	r2, #0x1494
  3aa4d0: e7848002     	str	r8, [r4, r2]
  3aa4d4: e3012498     	movw	r2, #0x1498
  3aa4d8: e7846002     	str	r6, [r4, r2]
  3aa4dc: e301249c     	movw	r2, #0x149c
  3aa4e0: e7848002     	str	r8, [r4, r2]
  3aa4e4: e30124a0     	movw	r2, #0x14a0
  3aa4e8: e7848002     	str	r8, [r4, r2]
  3aa4ec: e30124a4     	movw	r2, #0x14a4
  3aa4f0: e7848002     	str	r8, [r4, r2]
  3aa4f4: e30124aa     	movw	r2, #0x14aa
  3aa4f8: e18480b2     	strh	r8, [r4, r2]
  3aa4fc: e30124ac     	movw	r2, #0x14ac
  3aa500: e7c48002     	strb	r8, [r4, r2]
  3aa504: e30124d8     	movw	r2, #0x14d8
  3aa508: e7843002     	str	r3, [r4, r2]
  3aa50c: e2811502     	add	r1, r1, #8388608
  3aa510: e30124fc     	movw	r2, #0x14fc
  3aa514: e7841002     	str	r1, [r4, r2]
  3aa518: e3012504     	movw	r2, #0x1504
  3aa51c: e7846002     	str	r6, [r4, r2]
  3aa520: e30124ad     	movw	r2, #0x14ad
  3aa524: e7c48002     	strb	r8, [r4, r2]
  3aa528: e30124b0     	movw	r2, #0x14b0
  3aa52c: e7843002     	str	r3, [r4, r2]
  3aa530: e30124b4     	movw	r2, #0x14b4
  3aa534: e7843002     	str	r3, [r4, r2]
  3aa538: e30124b8     	movw	r2, #0x14b8
  3aa53c: e7843002     	str	r3, [r4, r2]
  3aa540: e30124bc     	movw	r2, #0x14bc
  3aa544: e7843002     	str	r3, [r4, r2]
  3aa548: e3a02d53     	mov	r2, #5312
  3aa54c: e7843002     	str	r3, [r4, r2]
  3aa550: e30124c4     	movw	r2, #0x14c4
  3aa554: e7843002     	str	r3, [r4, r2]
  3aa558: e30134c8     	movw	r3, #0x14c8
  3aa55c: e7c48003     	strb	r8, [r4, r3]
  3aa560: e30134ca     	movw	r3, #0x14ca
  3aa564: e18460b3     	strh	r6, [r4, r3]
  3aa568: e30134cc     	movw	r3, #0x14cc
  3aa56c: e7848003     	str	r8, [r4, r3]
  3aa570: e30134d0     	movw	r3, #0x14d0
  3aa574: e18480b3     	strh	r8, [r4, r3]
  3aa578: e30134d4     	movw	r3, #0x14d4
  3aa57c: e7848003     	str	r8, [r4, r3]
  3aa580: e30134dc     	movw	r3, #0x14dc
  3aa584: e7c48003     	strb	r8, [r4, r3]
  3aa588: e30134e4     	movw	r3, #0x14e4
  3aa58c: e7c48003     	strb	r8, [r4, r3]
  3aa590: e30134e5     	movw	r3, #0x14e5
  3aa594: e7c48003     	strb	r8, [r4, r3]
  3aa598: e30134e8     	movw	r3, #0x14e8
  3aa59c: e7848003     	str	r8, [r4, r3]
  3aa5a0: e30134ec     	movw	r3, #0x14ec
  3aa5a4: e7848003     	str	r8, [r4, r3]
  3aa5a8: e2847c15     	add	r7, r4, #5376
  3aa5ac: e30134f0     	movw	r3, #0x14f0
  3aa5b0: e2840d69     	add	r0, r4, #6720
  3aa5b4: e7c48003     	strb	r8, [r4, r3]
  3aa5b8: e2800008     	add	r0, r0, #8
  3aa5bc: e3a03c15     	mov	r3, #5376
  3aa5c0: e2877008     	add	r7, r7, #8
  3aa5c4: e7846003     	str	r6, [r4, r3]
  3aa5c8: e58d0010     	str	r0, [sp, #0x10]
  3aa5cc: e1a00007     	mov	r0, r7
  3aa5d0: ebfff113     	bl	0x3a6a24 <Character::NetStructCharacter::NetStructCharacter()> @ imm = #-0x3bb4
  3aa5d4: e59d0010     	ldr	r0, [sp, #0x10]
  3aa5d8: ebfff111     	bl	0x3a6a24 <Character::NetStructCharacter::NetStructCharacter()> @ imm = #-0x3bbc
  3aa5dc: e2840fc1     	add	r0, r4, #772
  3aa5e0: e1a01004     	mov	r1, r4
  3aa5e4: e5c4b028     	strb	r11, [r4, #0x28]
  3aa5e8: eb03dccb     	bl	0x4a191c <ObjectSearcher::TargetList::SetRefObject(GameObject*)> @ imm = #0xf732c
  3aa5ec: e5c4b1c4     	strb	r11, [r4, #0x1c4]
  3aa5f0: e5c4b085     	strb	r11, [r4, #0x85]
  3aa5f4: e3a00010     	mov	r0, #16
  3aa5f8: e1a01008     	mov	r1, r8
  3aa5fc: ebfd97db     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x9a094
  3aa600: e59f313c     	ldr	r3, [pc, #0x13c]        @ 0x3aa744 <Character::Character(ObjectBase::GO_IDS)+0x590>
  3aa604: e59dc00c     	ldr	r12, [sp, #0xc]
  3aa608: e1a06000     	mov	r6, r0
  3aa60c: e7993003     	ldr	r3, [r9, r3]
  3aa610: e15c0008     	cmp	r12, r8
  3aa614: e5c6800a     	strb	r8, [r6, #0xa]
  3aa618: e2833008     	add	r3, r3, #8
  3aa61c: e580800c     	str	r8, [r0, #0xc]
  3aa620: e8801008     	stm	r0, {r3, r12}
  3aa624: e5c68008     	strb	r8, [r6, #0x8]
  3aa628: e5c68009     	strb	r8, [r6, #0x9]
  3aa62c: 0a00002b     	beq	0x3aa6e0 <Character::Character(ObjectBase::GO_IDS)+0x52c> @ imm = #0xac
  3aa630: e1a0000c     	mov	r0, r12
  3aa634: e1a01006     	mov	r1, r6
  3aa638: eb0169f4     	bl	0x404e10 <v2Controllable::SetController(v2Controller*)> @ imm = #0x5a7d0
  3aa63c: e5943378     	ldr	r3, [r4, #0x378]
  3aa640: e59d0020     	ldr	r0, [sp, #0x20]
  3aa644: e1a01004     	mov	r1, r4
  3aa648: e583400c     	str	r4, [r3, #0xc]
  3aa64c: eb00c38b     	bl	0x3db480 <CharTimers::SetCharacter(Character*)> @ imm = #0x30e2c
  3aa650: e59d001c     	ldr	r0, [sp, #0x1c]
  3aa654: e1a01004     	mov	r1, r4
  3aa658: eb008458     	bl	0x3cb7c0 <CharAI::SetCharacter(Character*)> @ imm = #0x21160
  3aa65c: e59d0014     	ldr	r0, [sp, #0x14]
  3aa660: e1a01004     	mov	r1, r4
  3aa664: eb007c89     	bl	0x3c9890 <CharAnimator::SetCharacter(Character*)> @ imm = #0x1f224
  3aa668: e1a00005     	mov	r0, r5
  3aa66c: e1a01004     	mov	r1, r4
  3aa670: eb005be2     	bl	0x3c1600 <CharStateMachine::SetCharacter(Character*)> @ imm = #0x16f88
  3aa674: e59d0018     	ldr	r0, [sp, #0x18]
  3aa678: e1a01004     	mov	r1, r4
  3aa67c: eb00d162     	bl	0x3dec0c <CharProperties::SetCharacter(Character*)> @ imm = #0x34588
  3aa680: e3a06000     	mov	r6, #0
  3aa684: e5844380     	str	r4, [r4, #0x380]
  3aa688: e1a01006     	mov	r1, r6
  3aa68c: e1a00005     	mov	r0, r5
  3aa690: e2866001     	add	r6, r6, #1
  3aa694: eb00731f     	bl	0x3c7318 <CharStateMachine::RegisterState(int)> @ imm = #0x1cc7c
  3aa698: e3560014     	cmp	r6, #20
  3aa69c: 1afffff9     	bne	0x3aa688 <Character::Character(ObjectBase::GO_IDS)+0x4d4> @ imm = #-0x1c
  3aa6a0: e3a01000     	mov	r1, #0
  3aa6a4: e30124e0     	movw	r2, #0x14e0
  3aa6a8: e7841002     	str	r1, [r4, r2]
  3aa6ac: e3e03000     	mvn	r3, #0
  3aa6b0: e30124f4     	movw	r2, #0x14f4
  3aa6b4: e7843002     	str	r3, [r4, r2]
  3aa6b8: e5847100     	str	r7, [r4, #0x100]
  3aa6bc: e59da010     	ldr	r10, [sp, #0x10]
  3aa6c0: e30124f8     	movw	r2, #0x14f8
  3aa6c4: e1a00004     	mov	r0, r4
  3aa6c8: e584a104     	str	r10, [r4, #0x104]
  3aa6cc: e7843002     	str	r3, [r4, r2]
  3aa6d0: e3a03001     	mov	r3, #1
  3aa6d4: e5c430f8     	strb	r3, [r4, #0xf8]
  3aa6d8: e28dd03c     	add	sp, sp, #60
  3aa6dc: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3aa6e0: e59f3060     	ldr	r3, [pc, #0x60]         @ 0x3aa748 <Character::Character(ObjectBase::GO_IDS)+0x594>
  3aa6e4: e7993003     	ldr	r3, [r9, r3]
  3aa6e8: e5933000     	ldr	r3, [r3]
  3aa6ec: e3530002     	cmp	r3, #2
  3aa6f0: 0584c374     	streq	r12, [r4, #0x374]
  3aa6f4: 0affffcd     	beq	0x3aa630 <Character::Character(ObjectBase::GO_IDS)+0x47c> @ imm = #-0xcc
  3aa6f8: e3530001     	cmp	r3, #1
  3aa6fc: 1affffcb     	bne	0x3aa630 <Character::Character(ObjectBase::GO_IDS)+0x47c> @ imm = #-0xd4
  3aa700: e59f0044     	ldr	r0, [pc, #0x44]         @ 0x3aa74c <Character::Character(ObjectBase::GO_IDS)+0x598>
  3aa704: e59f1044     	ldr	r1, [pc, #0x44]         @ 0x3aa750 <Character::Character(ObjectBase::GO_IDS)+0x59c>
  3aa708: e59f2044     	ldr	r2, [pc, #0x44]         @ 0x3aa754 <Character::Character(ObjectBase::GO_IDS)+0x5a0>
  3aa70c: e7990000     	ldr	r0, [r9, r0]
  3aa710: e59f3040     	ldr	r3, [pc, #0x40]         @ 0x3aa758 <Character::Character(ObjectBase::GO_IDS)+0x5a4>
  3aa714: e3a0e044     	mov	lr, #68
  3aa718: e08f1001     	add	r1, pc, r1
  3aa71c: e28000a8     	add	r0, r0, #168
  3aa720: e08f2002     	add	r2, pc, r2
  3aa724: e08f3003     	add	r3, pc, r3
  3aa728: e58dc00c     	str	r12, [sp, #0xc]
  3aa72c: e58de000     	str	lr, [sp]
  3aa730: ebfd8e33     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x9c734
  3aa734: e59dc00c     	ldr	r12, [sp, #0xc]
  3aa738: eaffffbc     	b	0x3aa630 <Character::Character(ObjectBase::GO_IDS)+0x47c> @ imm = #-0x110
  3aa73c: 54 a8 5e 00  	.word	0x005ea854
  3aa740: 08 2e 00 00  	.word	0x00002e08
  3aa744: a4 2a 00 00  	.word	0x00002aa4
  3aa748: c0 39 00 00  	.word	0x000039c0
  3aa74c: c0 19 00 00  	.word	0x000019c0
  3aa750: c0 3c 51 00  	.word	0x00513cc0
  3aa754: a0 8d 51 00  	.word	0x00518da0
  3aa758: ac 8d 51 00  	.word	0x00518dac
