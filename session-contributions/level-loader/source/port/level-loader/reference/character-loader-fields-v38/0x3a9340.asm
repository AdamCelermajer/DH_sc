
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003a9340 <Character::Character(ObjectBase::GO_IDS)>:
  3a9340: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3a9344: e280cfdd     	add	r12, r0, #884
  3a9348: e24dd03c     	sub	sp, sp, #60
  3a934c: e1a04000     	mov	r4, r0
  3a9350: e58dc00c     	str	r12, [sp, #0xc]
  3a9354: ebff8c0f     	bl	0x38c398 <GameObject::GameObject(ObjectBase::GO_IDS)> @ imm = #-0x1cfc4
  3a9358: e59dc00c     	ldr	r12, [sp, #0xc]
  3a935c: e2845e4f     	add	r5, r4, #1264
  3a9360: e285500c     	add	r5, r5, #12
  3a9364: e1a0000c     	mov	r0, r12
  3a9368: eb016e92     	bl	0x404db8 <v2Controllable::v2Controllable()> @ imm = #0x5ba48
  3a936c: e2840fed     	add	r0, r4, #948
  3a9370: e58d0020     	str	r0, [sp, #0x20]
  3a9374: e2840fdf     	add	r0, r4, #892
  3a9378: eb0157ec     	bl	0x3ff330 <ItemInventory::ItemInventory()> @ imm = #0x55fb0
  3a937c: e2842e49     	add	r2, r4, #1168
  3a9380: e2841ff2     	add	r1, r4, #968
  3a9384: e282200c     	add	r2, r2, #12
  3a9388: e59d0020     	ldr	r0, [sp, #0x20]
  3a938c: e58d101c     	str	r1, [sp, #0x1c]
  3a9390: e58d2014     	str	r2, [sp, #0x14]
  3a9394: eb00c9dc     	bl	0x3dbb0c <CharTimers::CharTimers()> @ imm = #0x32770
  3a9398: e59d001c     	ldr	r0, [sp, #0x1c]
  3a939c: eb009613     	bl	0x3cebf0 <CharAI::CharAI()> @ imm = #0x2584c
  3a93a0: e59d0014     	ldr	r0, [sp, #0x14]
  3a93a4: eb007f12     	bl	0x3c8ff4 <CharAnimator::CharAnimator()> @ imm = #0x1fc48
  3a93a8: e2843e56     	add	r3, r4, #1376
  3a93ac: e1a00005     	mov	r0, r5
  3a93b0: e58d3018     	str	r3, [sp, #0x18]
  3a93b4: e59f950c     	ldr	r9, [pc, #0x50c]        @ 0x3a98c8 <Character::Character(ObjectBase::GO_IDS)+0x588>
  3a93b8: eb0061e6     	bl	0x3c1b58 <CharStateMachine::CharStateMachine()> @ imm = #0x18798
  3a93bc: e59d0018     	ldr	r0, [sp, #0x18]
  3a93c0: eb00d72f     	bl	0x3df084 <CharProperties::CharProperties()> @ imm = #0x35cbc
  3a93c4: e59fe500     	ldr	lr, [pc, #0x500]        @ 0x3a98cc <Character::Character(ObjectBase::GO_IDS)+0x58c>
  3a93c8: e08f9009     	add	r9, pc, r9
  3a93cc: e3a08000     	mov	r8, #0
  3a93d0: e799e00e     	ldr	lr, [r9, lr]
  3a93d4: e3a0b001     	mov	r11, #1
  3a93d8: e3e06000     	mvn	r6, #0
  3a93dc: e28eafc9     	add	r10, lr, #804
  3a93e0: e58da034     	str	r10, [sp, #0x34]
  3a93e4: e28ead06     	add	r10, lr, #384
  3a93e8: e58da010     	str	r10, [sp, #0x10]
  3a93ec: e28eaf7d     	add	r10, lr, #500
  3a93f0: e58da024     	str	r10, [sp, #0x24]
  3a93f4: e28eae22     	add	r10, lr, #544
  3a93f8: e58da028     	str	r10, [sp, #0x28]
  3a93fc: e28eae23     	add	r10, lr, #560
  3a9400: e58da02c     	str	r10, [sp, #0x2c]
  3a9404: e28e0008     	add	r0, lr, #8
  3a9408: e28e1f57     	add	r1, lr, #348
  3a940c: e28e2f5a     	add	r2, lr, #360
  3a9410: e28eafc1     	add	r10, lr, #772
  3a9414: e58da030     	str	r10, [sp, #0x30]
  3a9418: e8840003     	stm	r4, {r0, r1}
  3a941c: e5842024     	str	r2, [r4, #0x24]
  3a9420: e59d0010     	ldr	r0, [sp, #0x10]
  3a9424: e28eefc5     	add	lr, lr, #788
  3a9428: e2847d4e     	add	r7, r4, #4992
  3a942c: e5840374     	str	r0, [r4, #0x374]
  3a9430: e59d1024     	ldr	r1, [sp, #0x24]
  3a9434: e2873018     	add	r3, r7, #24
  3a9438: e301a3a8     	movw	r10, #0x13a8
  3a943c: e584137c     	str	r1, [r4, #0x37c]
  3a9440: e59d2028     	ldr	r2, [sp, #0x28]
  3a9444: e2877030     	add	r7, r7, #48
  3a9448: e58423b4     	str	r2, [r4, #0x3b4]
  3a944c: e59d002c     	ldr	r0, [sp, #0x2c]
  3a9450: e58403c8     	str	r0, [r4, #0x3c8]
  3a9454: e59d1030     	ldr	r1, [sp, #0x30]
  3a9458: e584e4fc     	str	lr, [r4, #0x4fc]
  3a945c: e1a00003     	mov	r0, r3
  3a9460: e584149c     	str	r1, [r4, #0x49c]
  3a9464: e59d2034     	ldr	r2, [sp, #0x34]
  3a9468: e3a01010     	mov	r1, #16
  3a946c: e5842560     	str	r2, [r4, #0x560]
  3a9470: e3012394     	movw	r2, #0x1394
  3a9474: e7c48002     	strb	r8, [r4, r2]
  3a9478: e3012395     	movw	r2, #0x1395
  3a947c: e7c48002     	strb	r8, [r4, r2]
  3a9480: e3012396     	movw	r2, #0x1396
  3a9484: e7c4b002     	strb	r11, [r4, r2]
  3a9488: e3012397     	movw	r2, #0x1397
  3a948c: e7c46002     	strb	r6, [r4, r2]
  3a9490: e30123ac     	movw	r2, #0x13ac
  3a9494: e7843002     	str	r3, [r4, r2]
  3a9498: e784300a     	str	r3, [r4, r10]
  3a949c: ebfda076     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x97e28
  3a94a0: e794300a     	ldr	r3, [r4, r10]
  3a94a4: e3a0ad4f     	mov	r10, #5056
  3a94a8: e1a00007     	mov	r0, r7
  3a94ac: e5c38000     	strb	r8, [r3]
  3a94b0: e30133c4     	movw	r3, #0x13c4
  3a94b4: e7847003     	str	r7, [r4, r3]
  3a94b8: e3a01010     	mov	r1, #16
  3a94bc: e784700a     	str	r7, [r4, r10]
  3a94c0: ebfda06d     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x97e4c
  3a94c4: e794200a     	ldr	r2, [r4, r10]
  3a94c8: e084700a     	add	r7, r4, r10
  3a94cc: e287300c     	add	r3, r7, #12
  3a94d0: e5c28000     	strb	r8, [r2]
  3a94d4: e30123c8     	movw	r2, #0x13c8
  3a94d8: e18460b2     	strh	r6, [r4, r2]
  3a94dc: e30123ca     	movw	r2, #0x13ca
  3a94e0: e18460b2     	strh	r6, [r4, r2]
  3a94e4: e301a3dc     	movw	r10, #0x13dc
  3a94e8: e30123e0     	movw	r2, #0x13e0
  3a94ec: e7843002     	str	r3, [r4, r2]
  3a94f0: e1a00003     	mov	r0, r3
  3a94f4: e784300a     	str	r3, [r4, r10]
  3a94f8: e3a01010     	mov	r1, #16
  3a94fc: ebfda05e     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x97e88
  3a9500: e794300a     	ldr	r3, [r4, r10]
  3a9504: e2877028     	add	r7, r7, #40
  3a9508: e301a3f8     	movw	r10, #0x13f8
  3a950c: e5c38000     	strb	r8, [r3]
  3a9510: e30133e4     	movw	r3, #0x13e4
  3a9514: e7c4b003     	strb	r11, [r4, r3]
  3a9518: e30133fc     	movw	r3, #0x13fc
  3a951c: e7847003     	str	r7, [r4, r3]
  3a9520: e1a00007     	mov	r0, r7
  3a9524: e784700a     	str	r7, [r4, r10]
  3a9528: e3a01010     	mov	r1, #16
  3a952c: ebfda052     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x97eb8
  3a9530: e794300a     	ldr	r3, [r4, r10]
  3a9534: e2847b05     	add	r7, r4, #5120
  3a9538: e301a410     	movw	r10, #0x1410
  3a953c: e5c38000     	strb	r8, [r3]
  3a9540: e3013414     	movw	r3, #0x1414
  3a9544: e7847003     	str	r7, [r4, r3]
  3a9548: e1a00007     	mov	r0, r7
  3a954c: e784700a     	str	r7, [r4, r10]
  3a9550: e3a01010     	mov	r1, #16
  3a9554: ebfda048     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x97ee0
  3a9558: e794300a     	ldr	r3, [r4, r10]
  3a955c: e2877018     	add	r7, r7, #24
  3a9560: e301a428     	movw	r10, #0x1428
  3a9564: e5c38000     	strb	r8, [r3]
  3a9568: e301342c     	movw	r3, #0x142c
  3a956c: e7847003     	str	r7, [r4, r3]
  3a9570: e1a00007     	mov	r0, r7
  3a9574: e784700a     	str	r7, [r4, r10]
  3a9578: e3a01010     	mov	r1, #16
  3a957c: ebfda03e     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x97f08
  3a9580: e794200a     	ldr	r2, [r4, r10]
  3a9584: e3a03000     	mov	r3, #0
  3a9588: e3a014bf     	mov	r1, #-1090519040
  3a958c: e5c28000     	strb	r8, [r2]
  3a9590: e30124a8     	movw	r2, #0x14a8
  3a9594: e7c46002     	strb	r6, [r4, r2]
  3a9598: e3012430     	movw	r2, #0x1430
  3a959c: e7c4b002     	strb	r11, [r4, r2]
  3a95a0: e3012434     	movw	r2, #0x1434
  3a95a4: e7848002     	str	r8, [r4, r2]
  3a95a8: e3012438     	movw	r2, #0x1438
  3a95ac: e7848002     	str	r8, [r4, r2]
  3a95b0: e3012448     	movw	r2, #0x1448
  3a95b4: e7c4b002     	strb	r11, [r4, r2]
  3a95b8: e3012449     	movw	r2, #0x1449
  3a95bc: e7c48002     	strb	r8, [r4, r2]
  3a95c0: e301244c     	movw	r2, #0x144c
  3a95c4: e7848002     	str	r8, [r4, r2]
  3a95c8: e3012450     	movw	r2, #0x1450
  3a95cc: e7843002     	str	r3, [r4, r2]
  3a95d0: e3012454     	movw	r2, #0x1454
  3a95d4: e7843002     	str	r3, [r4, r2]
  3a95d8: e3012458     	movw	r2, #0x1458
  3a95dc: e7843002     	str	r3, [r4, r2]
  3a95e0: e301245c     	movw	r2, #0x145c
  3a95e4: e7843002     	str	r3, [r4, r2]
  3a95e8: e3012460     	movw	r2, #0x1460
  3a95ec: e7843002     	str	r3, [r4, r2]
  3a95f0: e3012464     	movw	r2, #0x1464
  3a95f4: e7843002     	str	r3, [r4, r2]
  3a95f8: e3012468     	movw	r2, #0x1468
  3a95fc: e7843002     	str	r3, [r4, r2]
  3a9600: e301246c     	movw	r2, #0x146c
  3a9604: e7843002     	str	r3, [r4, r2]
  3a9608: e3012470     	movw	r2, #0x1470
  3a960c: e7843002     	str	r3, [r4, r2]
  3a9610: e3012474     	movw	r2, #0x1474
  3a9614: e7843002     	str	r3, [r4, r2]
  3a9618: e3012478     	movw	r2, #0x1478
  3a961c: e7843002     	str	r3, [r4, r2]
  3a9620: e301247c     	movw	r2, #0x147c
  3a9624: e7843002     	str	r3, [r4, r2]
  3a9628: e3a02d52     	mov	r2, #5248
  3a962c: e7c48002     	strb	r8, [r4, r2]
  3a9630: e3012481     	movw	r2, #0x1481
  3a9634: e7c48002     	strb	r8, [r4, r2]
  3a9638: e3012484     	movw	r2, #0x1484
  3a963c: e7848002     	str	r8, [r4, r2]
  3a9640: e3012488     	movw	r2, #0x1488
  3a9644: e7848002     	str	r8, [r4, r2]
  3a9648: e301248c     	movw	r2, #0x148c
  3a964c: e7848002     	str	r8, [r4, r2]
  3a9650: e3012490     	movw	r2, #0x1490
  3a9654: e7848002     	str	r8, [r4, r2]
  3a9658: e3012494     	movw	r2, #0x1494
  3a965c: e7848002     	str	r8, [r4, r2]
  3a9660: e3012498     	movw	r2, #0x1498
  3a9664: e7846002     	str	r6, [r4, r2]
  3a9668: e301249c     	movw	r2, #0x149c
  3a966c: e7848002     	str	r8, [r4, r2]
  3a9670: e30124a0     	movw	r2, #0x14a0
  3a9674: e7848002     	str	r8, [r4, r2]
  3a9678: e30124a4     	movw	r2, #0x14a4
  3a967c: e7848002     	str	r8, [r4, r2]
  3a9680: e30124aa     	movw	r2, #0x14aa
  3a9684: e18480b2     	strh	r8, [r4, r2]
  3a9688: e30124ac     	movw	r2, #0x14ac
  3a968c: e7c48002     	strb	r8, [r4, r2]
  3a9690: e30124d8     	movw	r2, #0x14d8
  3a9694: e7843002     	str	r3, [r4, r2]
  3a9698: e2811502     	add	r1, r1, #8388608
  3a969c: e30124fc     	movw	r2, #0x14fc
  3a96a0: e7841002     	str	r1, [r4, r2]
  3a96a4: e3012504     	movw	r2, #0x1504
  3a96a8: e7846002     	str	r6, [r4, r2]
  3a96ac: e30124ad     	movw	r2, #0x14ad
  3a96b0: e7c48002     	strb	r8, [r4, r2]
  3a96b4: e30124b0     	movw	r2, #0x14b0
  3a96b8: e7843002     	str	r3, [r4, r2]
  3a96bc: e30124b4     	movw	r2, #0x14b4
  3a96c0: e7843002     	str	r3, [r4, r2]
  3a96c4: e30124b8     	movw	r2, #0x14b8
  3a96c8: e7843002     	str	r3, [r4, r2]
  3a96cc: e30124bc     	movw	r2, #0x14bc
  3a96d0: e7843002     	str	r3, [r4, r2]
  3a96d4: e3a02d53     	mov	r2, #5312
  3a96d8: e7843002     	str	r3, [r4, r2]
  3a96dc: e30124c4     	movw	r2, #0x14c4
  3a96e0: e7843002     	str	r3, [r4, r2]
  3a96e4: e30134c8     	movw	r3, #0x14c8
  3a96e8: e7c48003     	strb	r8, [r4, r3]
  3a96ec: e30134ca     	movw	r3, #0x14ca
  3a96f0: e18460b3     	strh	r6, [r4, r3]
  3a96f4: e30134cc     	movw	r3, #0x14cc
  3a96f8: e7848003     	str	r8, [r4, r3]
  3a96fc: e30134d0     	movw	r3, #0x14d0
  3a9700: e18480b3     	strh	r8, [r4, r3]
  3a9704: e30134d4     	movw	r3, #0x14d4
  3a9708: e7848003     	str	r8, [r4, r3]
  3a970c: e30134dc     	movw	r3, #0x14dc
  3a9710: e7c48003     	strb	r8, [r4, r3]
  3a9714: e30134e4     	movw	r3, #0x14e4
  3a9718: e7c48003     	strb	r8, [r4, r3]
  3a971c: e30134e5     	movw	r3, #0x14e5
  3a9720: e7c48003     	strb	r8, [r4, r3]
  3a9724: e30134e8     	movw	r3, #0x14e8
  3a9728: e7848003     	str	r8, [r4, r3]
  3a972c: e30134ec     	movw	r3, #0x14ec
  3a9730: e7848003     	str	r8, [r4, r3]
  3a9734: e2847c15     	add	r7, r4, #5376
  3a9738: e30134f0     	movw	r3, #0x14f0
  3a973c: e2840d69     	add	r0, r4, #6720
  3a9740: e7c48003     	strb	r8, [r4, r3]
  3a9744: e2800008     	add	r0, r0, #8
  3a9748: e3a03c15     	mov	r3, #5376
  3a974c: e2877008     	add	r7, r7, #8
  3a9750: e7846003     	str	r6, [r4, r3]
  3a9754: e58d0010     	str	r0, [sp, #0x10]
  3a9758: e1a00007     	mov	r0, r7
  3a975c: ebfff4b0     	bl	0x3a6a24 <Character::NetStructCharacter::NetStructCharacter()> @ imm = #-0x2d40
  3a9760: e59d0010     	ldr	r0, [sp, #0x10]
  3a9764: ebfff4ae     	bl	0x3a6a24 <Character::NetStructCharacter::NetStructCharacter()> @ imm = #-0x2d48
  3a9768: e2840fc1     	add	r0, r4, #772
  3a976c: e1a01004     	mov	r1, r4
  3a9770: e5c4b028     	strb	r11, [r4, #0x28]
  3a9774: eb03e068     	bl	0x4a191c <ObjectSearcher::TargetList::SetRefObject(GameObject*)> @ imm = #0xf81a0
  3a9778: e5c4b1c4     	strb	r11, [r4, #0x1c4]
  3a977c: e5c4b085     	strb	r11, [r4, #0x85]
  3a9780: e3a00010     	mov	r0, #16
  3a9784: e1a01008     	mov	r1, r8
  3a9788: ebfd9b78     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x99220
  3a978c: e59f313c     	ldr	r3, [pc, #0x13c]        @ 0x3a98d0 <Character::Character(ObjectBase::GO_IDS)+0x590>
  3a9790: e59dc00c     	ldr	r12, [sp, #0xc]
  3a9794: e1a06000     	mov	r6, r0
  3a9798: e7993003     	ldr	r3, [r9, r3]
  3a979c: e15c0008     	cmp	r12, r8
  3a97a0: e5c6800a     	strb	r8, [r6, #0xa]
  3a97a4: e2833008     	add	r3, r3, #8
  3a97a8: e580800c     	str	r8, [r0, #0xc]
  3a97ac: e8801008     	stm	r0, {r3, r12}
  3a97b0: e5c68008     	strb	r8, [r6, #0x8]
  3a97b4: e5c68009     	strb	r8, [r6, #0x9]
  3a97b8: 0a00002b     	beq	0x3a986c <Character::Character(ObjectBase::GO_IDS)+0x52c> @ imm = #0xac
  3a97bc: e1a0000c     	mov	r0, r12
  3a97c0: e1a01006     	mov	r1, r6
  3a97c4: eb016d91     	bl	0x404e10 <v2Controllable::SetController(v2Controller*)> @ imm = #0x5b644
  3a97c8: e5943378     	ldr	r3, [r4, #0x378]
  3a97cc: e59d0020     	ldr	r0, [sp, #0x20]
  3a97d0: e1a01004     	mov	r1, r4
  3a97d4: e583400c     	str	r4, [r3, #0xc]
  3a97d8: eb00c728     	bl	0x3db480 <CharTimers::SetCharacter(Character*)> @ imm = #0x31ca0
  3a97dc: e59d001c     	ldr	r0, [sp, #0x1c]
  3a97e0: e1a01004     	mov	r1, r4
  3a97e4: eb0087f5     	bl	0x3cb7c0 <CharAI::SetCharacter(Character*)> @ imm = #0x21fd4
  3a97e8: e59d0014     	ldr	r0, [sp, #0x14]
  3a97ec: e1a01004     	mov	r1, r4
  3a97f0: eb008026     	bl	0x3c9890 <CharAnimator::SetCharacter(Character*)> @ imm = #0x20098
  3a97f4: e1a00005     	mov	r0, r5
  3a97f8: e1a01004     	mov	r1, r4
  3a97fc: eb005f7f     	bl	0x3c1600 <CharStateMachine::SetCharacter(Character*)> @ imm = #0x17dfc
  3a9800: e59d0018     	ldr	r0, [sp, #0x18]
  3a9804: e1a01004     	mov	r1, r4
  3a9808: eb00d4ff     	bl	0x3dec0c <CharProperties::SetCharacter(Character*)> @ imm = #0x353fc
  3a980c: e3a06000     	mov	r6, #0
  3a9810: e5844380     	str	r4, [r4, #0x380]
  3a9814: e1a01006     	mov	r1, r6
  3a9818: e1a00005     	mov	r0, r5
  3a981c: e2866001     	add	r6, r6, #1
  3a9820: eb0076bc     	bl	0x3c7318 <CharStateMachine::RegisterState(int)> @ imm = #0x1daf0
  3a9824: e3560014     	cmp	r6, #20
  3a9828: 1afffff9     	bne	0x3a9814 <Character::Character(ObjectBase::GO_IDS)+0x4d4> @ imm = #-0x1c
  3a982c: e3a01000     	mov	r1, #0
  3a9830: e30124e0     	movw	r2, #0x14e0
  3a9834: e7841002     	str	r1, [r4, r2]
  3a9838: e3e03000     	mvn	r3, #0
  3a983c: e30124f4     	movw	r2, #0x14f4
  3a9840: e7843002     	str	r3, [r4, r2]
  3a9844: e5847100     	str	r7, [r4, #0x100]
  3a9848: e59da010     	ldr	r10, [sp, #0x10]
  3a984c: e30124f8     	movw	r2, #0x14f8
  3a9850: e1a00004     	mov	r0, r4
  3a9854: e584a104     	str	r10, [r4, #0x104]
  3a9858: e7843002     	str	r3, [r4, r2]
  3a985c: e3a03001     	mov	r3, #1
  3a9860: e5c430f8     	strb	r3, [r4, #0xf8]
  3a9864: e28dd03c     	add	sp, sp, #60
  3a9868: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3a986c: e59f3060     	ldr	r3, [pc, #0x60]         @ 0x3a98d4 <Character::Character(ObjectBase::GO_IDS)+0x594>
  3a9870: e7993003     	ldr	r3, [r9, r3]
  3a9874: e5933000     	ldr	r3, [r3]
  3a9878: e3530002     	cmp	r3, #2
  3a987c: 0584c374     	streq	r12, [r4, #0x374]
  3a9880: 0affffcd     	beq	0x3a97bc <Character::Character(ObjectBase::GO_IDS)+0x47c> @ imm = #-0xcc
  3a9884: e3530001     	cmp	r3, #1
  3a9888: 1affffcb     	bne	0x3a97bc <Character::Character(ObjectBase::GO_IDS)+0x47c> @ imm = #-0xd4
  3a988c: e59f0044     	ldr	r0, [pc, #0x44]         @ 0x3a98d8 <Character::Character(ObjectBase::GO_IDS)+0x598>
  3a9890: e59f1044     	ldr	r1, [pc, #0x44]         @ 0x3a98dc <Character::Character(ObjectBase::GO_IDS)+0x59c>
  3a9894: e59f2044     	ldr	r2, [pc, #0x44]         @ 0x3a98e0 <Character::Character(ObjectBase::GO_IDS)+0x5a0>
  3a9898: e7990000     	ldr	r0, [r9, r0]
  3a989c: e59f3040     	ldr	r3, [pc, #0x40]         @ 0x3a98e4 <Character::Character(ObjectBase::GO_IDS)+0x5a4>
  3a98a0: e3a0e044     	mov	lr, #68
  3a98a4: e08f1001     	add	r1, pc, r1
  3a98a8: e28000a8     	add	r0, r0, #168
  3a98ac: e08f2002     	add	r2, pc, r2
  3a98b0: e08f3003     	add	r3, pc, r3
  3a98b4: e58dc00c     	str	r12, [sp, #0xc]
  3a98b8: e58de000     	str	lr, [sp]
  3a98bc: ebfd91d0     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x9b8c0
  3a98c0: e59dc00c     	ldr	r12, [sp, #0xc]
  3a98c4: eaffffbc     	b	0x3a97bc <Character::Character(ObjectBase::GO_IDS)+0x47c> @ imm = #-0x110
  3a98c8: c8 b6 5e 00  	.word	0x005eb6c8
  3a98cc: 08 2e 00 00  	.word	0x00002e08
  3a98d0: a4 2a 00 00  	.word	0x00002aa4
  3a98d4: c0 39 00 00  	.word	0x000039c0
  3a98d8: c0 19 00 00  	.word	0x000019c0
  3a98dc: 34 4b 51 00  	.word	0x00514b34
  3a98e0: 14 9c 51 00  	.word	0x00519c14
  3a98e4: 20 9c 51 00  	.word	0x00519c20
