
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00340ccc <ObjectBase* GetNewInstance<SoundEmitter>()>:
  340ccc: e92d4010     	push	{r4, lr}
  340cd0: e3a01000     	mov	r1, #0
  340cd4: e3a00e3a     	mov	r0, #928
  340cd8: ebff3e24     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x30770
  340cdc: e3a01014     	mov	r1, #20
  340ce0: e1a04000     	mov	r4, r0
  340ce4: eb01520c     	bl	0x39551c <SoundEmitter::SoundEmitter(ObjectBase::GO_IDS)> @ imm = #0x54830
  340ce8: e1a00004     	mov	r0, r4
  340cec: e8bd8010     	pop	{r4, pc}


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00395718 <SoundEmitter::DeclareProperties()>:
  395718: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  39571c: e59f61e8     	ldr	r6, [pc, #0x1e8]        @ 0x39590c <SoundEmitter::DeclareProperties()+0x1f4>
  395720: e59f31e8     	ldr	r3, [pc, #0x1e8]        @ 0x395910 <SoundEmitter::DeclareProperties()+0x1f8>
  395724: e24dd04c     	sub	sp, sp, #76
  395728: e08f6006     	add	r6, pc, r6
  39572c: e796c003     	ldr	r12, [r6, r3]
  395730: e2804004     	add	r4, r0, #4
  395734: e1a05000     	mov	r5, r0
  395738: e59c3000     	ldr	r3, [r12]
  39573c: e280afdd     	add	r10, r0, #884
  395740: e58dc004     	str	r12, [sp, #0x4]
  395744: e58d3044     	str	r3, [sp, #0x44]
  395748: ebffdde6     	bl	0x38cee8 <GameObject::DeclareProperties()> @ imm = #-0x8868
  39574c: e3a01000     	mov	r1, #0
  395750: e3a00024     	mov	r0, #36
  395754: ebfdeb85     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x851ec
  395758: e59fb1b4     	ldr	r11, [pc, #0x1b4]       @ 0x395914 <SoundEmitter::DeclareProperties()+0x1fc>
  39575c: e59f81b4     	ldr	r8, [pc, #0x1b4]        @ 0x395918 <SoundEmitter::DeclareProperties()+0x200>
  395760: e1a07000     	mov	r7, r0
  395764: e796b00b     	ldr	r11, [r6, r11]
  395768: e08f8008     	add	r8, pc, r8
  39576c: e1a01008     	mov	r1, r8
  395770: e28bb008     	add	r11, r11, #8
  395774: e28d2010     	add	r2, sp, #16
  395778: e480b008     	str	r11, [r0], #8
  39577c: ebfdfa5a     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x81698
  395780: e59f3194     	ldr	r3, [pc, #0x194]        @ 0x39591c <SoundEmitter::DeclareProperties()+0x204>
  395784: e064a00a     	rsb	r10, r4, r10
  395788: e3a02001     	mov	r2, #1
  39578c: e7963003     	ldr	r3, [r6, r3]
  395790: e28d902c     	add	r9, sp, #44
  395794: e587a004     	str	r10, [r7, #0x4]
  395798: e2833008     	add	r3, r3, #8
  39579c: e5873000     	str	r3, [r7]
  3957a0: e5c72020     	strb	r2, [r7, #0x20]
  3957a4: e1a01008     	mov	r1, r8
  3957a8: e1a02007     	mov	r2, r7
  3957ac: e1a00004     	mov	r0, r4
  3957b0: eb05f94b     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x17e52c
  3957b4: e1a00009     	mov	r0, r9
  3957b8: e3a01010     	mov	r1, #16
  3957bc: e58d903c     	str	r9, [sp, #0x3c]
  3957c0: e58d9040     	str	r9, [sp, #0x40]
  3957c4: ebfdefac     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x84150
  3957c8: e59d303c     	ldr	r3, [sp, #0x3c]
  3957cc: e3a07000     	mov	r7, #0
  3957d0: e28d8014     	add	r8, sp, #20
  3957d4: e5c37000     	strb	r7, [r3]
  3957d8: e59d203c     	ldr	r2, [sp, #0x3c]
  3957dc: e1a00008     	mov	r0, r8
  3957e0: e59d1040     	ldr	r1, [sp, #0x40]
  3957e4: e58d8024     	str	r8, [sp, #0x24]
  3957e8: e58d8028     	str	r8, [sp, #0x28]
  3957ec: ebfdefbd     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0x8410c
  3957f0: e1a01007     	mov	r1, r7
  3957f4: e3a00038     	mov	r0, #56
  3957f8: ebfdeb5c     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x85290
  3957fc: e59fa11c     	ldr	r10, [pc, #0x11c]       @ 0x395920 <SoundEmitter::DeclareProperties()+0x208>
  395800: e1a07000     	mov	r7, r0
  395804: e28d200c     	add	r2, sp, #12
  395808: e08fa00a     	add	r10, pc, r10
  39580c: e1a0100a     	mov	r1, r10
  395810: e480b008     	str	r11, [r0], #8
  395814: ebfdfa34     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x81730
  395818: e59f3104     	ldr	r3, [pc, #0x104]        @ 0x395924 <SoundEmitter::DeclareProperties()+0x20c>
  39581c: e2852fde     	add	r2, r5, #888
  395820: e1a00007     	mov	r0, r7
  395824: e7963003     	ldr	r3, [r6, r3]
  395828: e0642002     	rsb	r2, r4, r2
  39582c: e5872004     	str	r2, [r7, #0x4]
  395830: e2833008     	add	r3, r3, #8
  395834: e4803020     	str	r3, [r0], #32
  395838: e5870030     	str	r0, [r7, #0x30]
  39583c: e5870034     	str	r0, [r7, #0x34]
  395840: e59d1028     	ldr	r1, [sp, #0x28]
  395844: e59d2024     	ldr	r2, [sp, #0x24]
  395848: ebfdefa6     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0x84168
  39584c: e1a0100a     	mov	r1, r10
  395850: e1a02007     	mov	r2, r7
  395854: e1a00004     	mov	r0, r4
  395858: eb05f921     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x17e484
  39585c: e1a00008     	mov	r0, r8
  395860: ebfdf851     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x81ebc
  395864: e1a00009     	mov	r0, r9
  395868: ebfdf84f     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x81ec4
  39586c: e59f30b4     	ldr	r3, [pc, #0xb4]         @ 0x395928 <SoundEmitter::DeclareProperties()+0x210>
  395870: e59f70b4     	ldr	r7, [pc, #0xb4]         @ 0x39592c <SoundEmitter::DeclareProperties()+0x214>
  395874: e59f20b4     	ldr	r2, [pc, #0xb4]         @ 0x395930 <SoundEmitter::DeclareProperties()+0x218>
  395878: e7966003     	ldr	r6, [r6, r3]
  39587c: e08f7007     	add	r7, pc, r7
  395880: e08f2002     	add	r2, pc, r2
  395884: e1a01007     	mov	r1, r7
  395888: e596002c     	ldr	r0, [r6, #0x2c]
  39588c: eb04bcd2     	bl	0x4c4bdc <PyDataConstants::getConstant(char const*, char const*) const> @ imm = #0x12f348
  395890: ebfde433     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x86f34
  395894: e59f8098     	ldr	r8, [pc, #0x98]         @ 0x395934 <SoundEmitter::DeclareProperties()+0x21c>
  395898: e285afe5     	add	r10, r5, #916
  39589c: e1a03000     	mov	r3, r0
  3958a0: e08f8008     	add	r8, pc, r8
  3958a4: e1a01008     	mov	r1, r8
  3958a8: e1a00004     	mov	r0, r4
  3958ac: e1a0200a     	mov	r2, r10
  3958b0: ebfffde1     	bl	0x39503c <void PropertyMap::AddProperty<float>(char const*, float&, float)> @ imm = #-0x87c
  3958b4: e59f207c     	ldr	r2, [pc, #0x7c]         @ 0x395938 <SoundEmitter::DeclareProperties()+0x220>
  3958b8: e1a01007     	mov	r1, r7
  3958bc: e596002c     	ldr	r0, [r6, #0x2c]
  3958c0: e08f2002     	add	r2, pc, r2
  3958c4: eb04bcc4     	bl	0x4c4bdc <PyDataConstants::getConstant(char const*, char const*) const> @ imm = #0x12f310
  3958c8: ebfde425     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x86f6c
  3958cc: e59f8068     	ldr	r8, [pc, #0x68]         @ 0x39593c <SoundEmitter::DeclareProperties()+0x224>
  3958d0: e2855fe6     	add	r5, r5, #920
  3958d4: e1a03000     	mov	r3, r0
  3958d8: e08f8008     	add	r8, pc, r8
  3958dc: e1a02005     	mov	r2, r5
  3958e0: e1a00004     	mov	r0, r4
  3958e4: e1a01008     	mov	r1, r8
  3958e8: ebfffdd3     	bl	0x39503c <void PropertyMap::AddProperty<float>(char const*, float&, float)> @ imm = #-0x8b4
  3958ec: e59dc004     	ldr	r12, [sp, #0x4]
  3958f0: e59d2044     	ldr	r2, [sp, #0x44]
  3958f4: e59c3000     	ldr	r3, [r12]
  3958f8: e1520003     	cmp	r2, r3
  3958fc: 1a000001     	bne	0x395908 <SoundEmitter::DeclareProperties()+0x1f0> @ imm = #0x4
  395900: e28dd04c     	add	sp, sp, #76
  395904: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  395908: ebfde280     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x87600
  39590c: 68 f3 5f 00  	.word	0x005ff368
  395910: ac 40 00 00  	.word	0x000040ac
  395914: 30 23 00 00  	.word	0x00002330
  395918: 28 d1 52 00  	.word	0x0052d128
  39591c: 4c 3e 00 00  	.word	0x00003e4c
  395920: 90 d0 52 00  	.word	0x0052d090
  395924: 94 34 00 00  	.word	0x00003494
  395928: f4 37 00 00  	.word	0x000037f4
  39592c: dc ca 52 00  	.word	0x0052cadc
  395930: 20 d0 52 00  	.word	0x0052d020
  395934: 18 d0 52 00  	.word	0x0052d018
  395938: a8 ca 52 00  	.word	0x0052caa8
  39593c: e8 cf 52 00  	.word	0x0052cfe8


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039551c <SoundEmitter::SoundEmitter(ObjectBase::GO_IDS)>:
  39551c: e92d4070     	push	{r4, r5, r6, lr}
  395520: e59f5078     	ldr	r5, [pc, #0x78]         @ 0x3955a0 <SoundEmitter::SoundEmitter(ObjectBase::GO_IDS)+0x84>
  395524: e1a04000     	mov	r4, r0
  395528: ebffdb9a     	bl	0x38c398 <GameObject::GameObject(ObjectBase::GO_IDS)> @ imm = #-0x9198
  39552c: e59f3070     	ldr	r3, [pc, #0x70]         @ 0x3955a4 <SoundEmitter::SoundEmitter(ObjectBase::GO_IDS)+0x88>
  395530: e08f5005     	add	r5, pc, r5
  395534: e2842fde     	add	r2, r4, #888
  395538: e7953003     	ldr	r3, [r5, r3]
  39553c: e1a00002     	mov	r0, r2
  395540: e5842388     	str	r2, [r4, #0x388]
  395544: e283c008     	add	r12, r3, #8
  395548: e28310e4     	add	r1, r3, #228
  39554c: e28330d8     	add	r3, r3, #216
  395550: e5843004     	str	r3, [r4, #0x4]
  395554: e5841024     	str	r1, [r4, #0x24]
  395558: e584238c     	str	r2, [r4, #0x38c]
  39555c: e584c000     	str	r12, [r4]
  395560: e3a01010     	mov	r1, #16
  395564: ebfdf044     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x83ef0
  395568: e5941388     	ldr	r1, [r4, #0x388]
  39556c: e3a02000     	mov	r2, #0
  395570: e3a034bf     	mov	r3, #-1090519040
  395574: e5c12000     	strb	r2, [r1]
  395578: e2833502     	add	r3, r3, #8388608
  39557c: e3e01000     	mvn	r1, #0
  395580: e5c4239c     	strb	r2, [r4, #0x39c]
  395584: e3a02001     	mov	r2, #1
  395588: e5841390     	str	r1, [r4, #0x390]
  39558c: e5843398     	str	r3, [r4, #0x398]
  395590: e5c42085     	strb	r2, [r4, #0x85]
  395594: e5843394     	str	r3, [r4, #0x394]
  395598: e1a00004     	mov	r0, r4
  39559c: e8bd8070     	pop	{r4, r5, r6, pc}
  3955a0: 60 f5 5f 00  	.word	0x005ff560
  3955a4: 58 1b 00 00  	.word	0x00001b58


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003955a8 <SoundEmitter::SoundEmitter(ObjectBase::GO_IDS)>:
  3955a8: e92d4070     	push	{r4, r5, r6, lr}
  3955ac: e59f5078     	ldr	r5, [pc, #0x78]         @ 0x39562c <SoundEmitter::SoundEmitter(ObjectBase::GO_IDS)+0x84>
  3955b0: e1a04000     	mov	r4, r0
  3955b4: ebffdb77     	bl	0x38c398 <GameObject::GameObject(ObjectBase::GO_IDS)> @ imm = #-0x9224
  3955b8: e59f3070     	ldr	r3, [pc, #0x70]         @ 0x395630 <SoundEmitter::SoundEmitter(ObjectBase::GO_IDS)+0x88>
  3955bc: e08f5005     	add	r5, pc, r5
  3955c0: e2842fde     	add	r2, r4, #888
  3955c4: e7953003     	ldr	r3, [r5, r3]
  3955c8: e1a00002     	mov	r0, r2
  3955cc: e5842388     	str	r2, [r4, #0x388]
  3955d0: e283c008     	add	r12, r3, #8
  3955d4: e28310e4     	add	r1, r3, #228
  3955d8: e28330d8     	add	r3, r3, #216
  3955dc: e5843004     	str	r3, [r4, #0x4]
  3955e0: e5841024     	str	r1, [r4, #0x24]
  3955e4: e584238c     	str	r2, [r4, #0x38c]
  3955e8: e584c000     	str	r12, [r4]
  3955ec: e3a01010     	mov	r1, #16
  3955f0: ebfdf021     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x83f7c
  3955f4: e5941388     	ldr	r1, [r4, #0x388]
  3955f8: e3a02000     	mov	r2, #0
  3955fc: e3a034bf     	mov	r3, #-1090519040
  395600: e5c12000     	strb	r2, [r1]
  395604: e2833502     	add	r3, r3, #8388608
  395608: e3e01000     	mvn	r1, #0
  39560c: e5c4239c     	strb	r2, [r4, #0x39c]
  395610: e3a02001     	mov	r2, #1
  395614: e5841390     	str	r1, [r4, #0x390]
  395618: e5843398     	str	r3, [r4, #0x398]
  39561c: e5c42085     	strb	r2, [r4, #0x85]
  395620: e5843394     	str	r3, [r4, #0x394]
  395624: e1a00004     	mov	r0, r4
  395628: e8bd8070     	pop	{r4, r5, r6, pc}
  39562c: d4 f4 5f 00  	.word	0x005ff4d4
  395630: 58 1b 00 00  	.word	0x00001b58


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00395710 <non-virtual thunk to SoundEmitter::DeclareProperties()>:
  395710: e2400004     	sub	r0, r0, #4
  395714: eaffffff     	b	0x395718 <SoundEmitter::DeclareProperties()> @ imm = #-0x4
