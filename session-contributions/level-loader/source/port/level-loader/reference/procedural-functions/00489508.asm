
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00489508 <rnd::RandomGenerator::LoadRuleFile(char const*)>:
  489508: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  48950c: e59f5434     	ldr	r5, [pc, #0x434]        @ 0x489948 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x440>
  489510: e59f2434     	ldr	r2, [pc, #0x434]        @ 0x48994c <rnd::RandomGenerator::LoadRuleFile(char const*)+0x444>
  489514: e24dd064     	sub	sp, sp, #100
  489518: e08f5005     	add	r5, pc, r5
  48951c: e7953002     	ldr	r3, [r5, r2]
  489520: e1a04000     	mov	r4, r0
  489524: e1a00001     	mov	r0, r1
  489528: e5933000     	ldr	r3, [r3]
  48952c: e1a08001     	mov	r8, r1
  489530: e58d2008     	str	r2, [sp, #0x8]
  489534: e58d305c     	str	r3, [sp, #0x5c]
  489538: ebfa1245     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x17b6ec
  48953c: e2846f5d     	add	r6, r4, #372
  489540: e0882000     	add	r2, r8, r0
  489544: e1a01008     	mov	r1, r8
  489548: e1a00006     	mov	r0, r6
  48954c: ebfa1d23     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x178b74
  489550: e59f33f8     	ldr	r3, [pc, #0x3f8]        @ 0x489950 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x448>
  489554: e1a00006     	mov	r0, r6
  489558: e7953003     	ldr	r3, [r5, r3]
  48955c: e5933010     	ldr	r3, [r3, #0x10]
  489560: e5937034     	ldr	r7, [r3, #0x34]
  489564: ebffec90     	bl	0x4847ac <std::string::rfind(char, unsigned int) const (.clone.10)> @ imm = #-0x4dc0
  489568: e3700001     	cmn	r0, #1
  48956c: 0a0000dd     	beq	0x4898e8 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x3e0> @ imm = #0x374
  489570: e2809001     	add	r9, r0, #1
  489574: e28da044     	add	r10, sp, #68
  489578: e28dc028     	add	r12, sp, #40
  48957c: e284bf57     	add	r11, r4, #348
  489580: e1a0000a     	mov	r0, r10
  489584: e1a01006     	mov	r1, r6
  489588: e3a02000     	mov	r2, #0
  48958c: e1a03009     	mov	r3, r9
  489590: e58dc000     	str	r12, [sp]
  489594: ebfe31cf     	bl	0x415cd8 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&, unsigned int, unsigned int, std::allocator<char> const&)> @ imm = #-0x738c4
  489598: e15b000a     	cmp	r11, r10
  48959c: 0a000003     	beq	0x4895b0 <rnd::RandomGenerator::LoadRuleFile(char const*)+0xa8> @ imm = #0xc
  4895a0: e1a0000b     	mov	r0, r11
  4895a4: e59d1058     	ldr	r1, [sp, #0x58]
  4895a8: e59d2054     	ldr	r2, [sp, #0x54]
  4895ac: ebfa1d0b     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x178bd4
  4895b0: e59d0058     	ldr	r0, [sp, #0x58]
  4895b4: e150000a     	cmp	r0, r10
  4895b8: 0a000006     	beq	0x4895d8 <rnd::RandomGenerator::LoadRuleFile(char const*)+0xd0> @ imm = #0x18
  4895bc: e3500000     	cmp	r0, #0
  4895c0: 0a000004     	beq	0x4895d8 <rnd::RandomGenerator::LoadRuleFile(char const*)+0xd0> @ imm = #0x10
  4895c4: e59d1044     	ldr	r1, [sp, #0x44]
  4895c8: e0601001     	rsb	r1, r0, r1
  4895cc: e3510080     	cmp	r1, #128
  4895d0: 8a0000d1     	bhi	0x48991c <rnd::RandomGenerator::LoadRuleFile(char const*)+0x414> @ imm = #0x344
  4895d4: eb09fe49     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27f924
  4895d8: e28da02c     	add	r10, sp, #44
  4895dc: e28dc024     	add	r12, sp, #36
  4895e0: e1a02009     	mov	r2, r9
  4895e4: e1a0000a     	mov	r0, r10
  4895e8: e1a01006     	mov	r1, r6
  4895ec: e3e03000     	mvn	r3, #0
  4895f0: e58dc000     	str	r12, [sp]
  4895f4: ebfe31b7     	bl	0x415cd8 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&, unsigned int, unsigned int, std::allocator<char> const&)> @ imm = #-0x73924
  4895f8: e156000a     	cmp	r6, r10
  4895fc: 0a000003     	beq	0x489610 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x108> @ imm = #0xc
  489600: e1a00006     	mov	r0, r6
  489604: e59d1040     	ldr	r1, [sp, #0x40]
  489608: e59d203c     	ldr	r2, [sp, #0x3c]
  48960c: ebfa1cf3     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x178c34
  489610: e59d0040     	ldr	r0, [sp, #0x40]
  489614: e150000a     	cmp	r0, r10
  489618: 0a000006     	beq	0x489638 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x130> @ imm = #0x18
  48961c: e3500000     	cmp	r0, #0
  489620: 0a000004     	beq	0x489638 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x130> @ imm = #0x10
  489624: e59d102c     	ldr	r1, [sp, #0x2c]
  489628: e0601001     	rsb	r1, r0, r1
  48962c: e3510080     	cmp	r1, #128
  489630: 8a0000b7     	bhi	0x489914 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x40c> @ imm = #0x2dc
  489634: eb09fe31     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27f8c4
  489638: e3a02000     	mov	r2, #0
  48963c: e1a01008     	mov	r1, r8
  489640: e1a03002     	mov	r3, r2
  489644: e597c000     	ldr	r12, [r7]
  489648: e1a00007     	mov	r0, r7
  48964c: e1a0e00f     	mov	lr, pc
  489650: e59cf088     	ldr	pc, [r12, #0x88]
  489654: e3500000     	cmp	r0, #0
  489658: e1a03000     	mov	r3, r0
  48965c: e58d0020     	str	r0, [sp, #0x20]
  489660: 0a000095     	beq	0x4898bc <rnd::RandomGenerator::LoadRuleFile(char const*)+0x3b4> @ imm = #0x254
  489664: e5933000     	ldr	r3, [r3]
  489668: e1a0e00f     	mov	lr, pc
  48966c: e593f008     	ldr	pc, [r3, #0x8]
  489670: e1a08000     	mov	r8, r0
  489674: ebfa141e     	bl	0x30e6f4 <.plt+0x980>   @ imm = #-0x17af88
  489678: e28d6060     	add	r6, sp, #96
  48967c: e536c040     	ldr	r12, [r6, #-0x40]!
  489680: e5840128     	str	r0, [r4, #0x128]
  489684: e1a02008     	mov	r2, r8
  489688: e1a03fc2     	asr	r3, r2, #31
  48968c: e1a01000     	mov	r1, r0
  489690: e1a0000c     	mov	r0, r12
  489694: e59cc000     	ldr	r12, [r12]
  489698: e1a0e00f     	mov	lr, pc
  48969c: e59cf018     	ldr	pc, [r12, #0x18]
  4896a0: e5973000     	ldr	r3, [r7]
  4896a4: e1a00007     	mov	r0, r7
  4896a8: e1a01006     	mov	r1, r6
  4896ac: e1a0e00f     	mov	lr, pc
  4896b0: e593f078     	ldr	pc, [r3, #0x78]
  4896b4: e3a01000     	mov	r1, #0
  4896b8: e3a00070     	mov	r0, #112
  4896bc: ebfa1bab     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x179154
  4896c0: e1a06000     	mov	r6, r0
  4896c4: eb0235fe     	bl	0x516ec4 <TiXmlDocument::TiXmlDocument()> @ imm = #0x8d7f8
  4896c8: e1a02008     	mov	r2, r8
  4896cc: e5846124     	str	r6, [r4, #0x124]
  4896d0: e1a00006     	mov	r0, r6
  4896d4: e5941128     	ldr	r1, [r4, #0x128]
  4896d8: e3a03000     	mov	r3, #0
  4896dc: eb023371     	bl	0x5164a8 <TiXmlDocument::LoadFromBuffer(void const*, int, TiXmlEncoding)> @ imm = #0x8cdc4
  4896e0: e59f226c     	ldr	r2, [pc, #0x26c]        @ 0x489954 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x44c>
  4896e4: e594c124     	ldr	r12, [r4, #0x124]
  4896e8: e28d6018     	add	r6, sp, #24
  4896ec: e08f2002     	add	r2, pc, r2
  4896f0: e28d101c     	add	r1, sp, #28
  4896f4: e3a03000     	mov	r3, #0
  4896f8: e1a00006     	mov	r0, r6
  4896fc: e58dc01c     	str	r12, [sp, #0x1c]
  489700: eb022db4     	bl	0x514dd8 <TiXmlHandle::Child(char const*, int) const> @ imm = #0x8b6d0
  489704: e1a00006     	mov	r0, r6
  489708: ebffe8ce     	bl	0x483a48 <TiXmlHandle::ToElement() const> @ imm = #-0x5cc8
  48970c: e2507000     	subs	r7, r0, #0
  489710: 0a000069     	beq	0x4898bc <rnd::RandomGenerator::LoadRuleFile(char const*)+0x3b4> @ imm = #0x1a4
  489714: e594018c     	ldr	r0, [r4, #0x18c]
  489718: e3500000     	cmp	r0, #0
  48971c: 0a000002     	beq	0x48972c <rnd::RandomGenerator::LoadRuleFile(char const*)+0x224> @ imm = #0x8
  489720: e2800004     	add	r0, r0, #4
  489724: e1a01007     	mov	r1, r7
  489728: eb0228b4     	bl	0x513a00 <PropertyMap::LoadOverridesFromXML(TiXmlElement*)> @ imm = #0x8a2d0
  48972c: e59f8224     	ldr	r8, [pc, #0x224]        @ 0x489958 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x450>
  489730: e1a00007     	mov	r0, r7
  489734: e284af4b     	add	r10, r4, #300
  489738: e08f8008     	add	r8, pc, r8
  48973c: e1a01008     	mov	r1, r8
  489740: eb022d4a     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x8b528
  489744: e3500000     	cmp	r0, #0
  489748: 0a000075     	beq	0x489924 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x41c> @ imm = #0x1d4
  48974c: e1a01008     	mov	r1, r8
  489750: e1a00007     	mov	r0, r7
  489754: eb022d45     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x8b514
  489758: e1a09000     	mov	r9, r0
  48975c: ebfa11bc     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x17b910
  489760: e0892000     	add	r2, r9, r0
  489764: e59f81f0     	ldr	r8, [pc, #0x1f0]        @ 0x48995c <rnd::RandomGenerator::LoadRuleFile(char const*)+0x454>
  489768: e1a01009     	mov	r1, r9
  48976c: e1a0000a     	mov	r0, r10
  489770: e08f8008     	add	r8, pc, r8
  489774: ebfa1c99     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x178d9c
  489778: e1a00007     	mov	r0, r7
  48977c: e1a01008     	mov	r1, r8
  489780: eb022d3a     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x8b4e8
  489784: e3500000     	cmp	r0, #0
  489788: e284af51     	add	r10, r4, #324
  48978c: 0a000068     	beq	0x489934 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x42c> @ imm = #0x1a0
  489790: e1a01008     	mov	r1, r8
  489794: e1a00007     	mov	r0, r7
  489798: eb022d34     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x8b4d0
  48979c: e1a07000     	mov	r7, r0
  4897a0: ebfa11ab     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x17b954
  4897a4: e0872000     	add	r2, r7, r0
  4897a8: e1a01007     	mov	r1, r7
  4897ac: e1a0000a     	mov	r0, r10
  4897b0: ebfa1c8a     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x178dd8
  4897b4: e1a00004     	mov	r0, r4
  4897b8: ebfffecb     	bl	0x4892ec <rnd::RandomGenerator::LoadBlocks()> @ imm = #-0x4d4
  4897bc: e59d1018     	ldr	r1, [sp, #0x18]
  4897c0: e1a08000     	mov	r8, r0
  4897c4: e1a00004     	mov	r0, r4
  4897c8: ebfff388     	bl	0x4865f0 <rnd::RandomGenerator::LoadListRules(TiXmlNode*)> @ imm = #-0x31e0
  4897cc: e59d1018     	ldr	r1, [sp, #0x18]
  4897d0: e0008008     	and	r8, r0, r8
  4897d4: e1a00004     	mov	r0, r4
  4897d8: ebfff13f     	bl	0x485cdc <rnd::RandomGenerator::LoadRoomPools(TiXmlNode*)> @ imm = #-0x3b04
  4897dc: e59f217c     	ldr	r2, [pc, #0x17c]        @ 0x489960 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x458>
  4897e0: e3580000     	cmp	r8, #0
  4897e4: 03a08000     	moveq	r8, #0
  4897e8: 12008001     	andne	r8, r0, #1
  4897ec: e3a03000     	mov	r3, #0
  4897f0: e08f2002     	add	r2, pc, r2
  4897f4: e1a01006     	mov	r1, r6
  4897f8: e28d0014     	add	r0, sp, #20
  4897fc: eb022d75     	bl	0x514dd8 <TiXmlHandle::Child(char const*, int) const> @ imm = #0x8b5d4
  489800: e284006c     	add	r0, r4, #108
  489804: e59d1014     	ldr	r1, [sp, #0x14]
  489808: eb001eef     	bl	0x4913cc <rnd::RootRule::LoadFromXml(TiXmlNode*)> @ imm = #0x7bbc
  48980c: e5943118     	ldr	r3, [r4, #0x118]
  489810: e594211c     	ldr	r2, [r4, #0x11c]
  489814: e0088000     	and	r8, r8, r0
  489818: e0632002     	rsb	r2, r3, r2
  48981c: e1b02122     	lsrs	r2, r2, #2
  489820: 0a00001c     	beq	0x489898 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x390> @ imm = #0x70
  489824: e59f1138     	ldr	r1, [pc, #0x138]        @ 0x489964 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x45c>
  489828: e59f9138     	ldr	r9, [pc, #0x138]        @ 0x489968 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x460>
  48982c: e59fb138     	ldr	r11, [pc, #0x138]       @ 0x48996c <rnd::RandomGenerator::LoadRuleFile(char const*)+0x464>
  489830: e59f7138     	ldr	r7, [pc, #0x138]        @ 0x489970 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x468>
  489834: e59fa138     	ldr	r10, [pc, #0x138]       @ 0x489974 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x46c>
  489838: e3a02000     	mov	r2, #0
  48983c: e08f1001     	add	r1, pc, r1
  489840: e08f9009     	add	r9, pc, r9
  489844: e08fb00b     	add	r11, pc, r11
  489848: e58d100c     	str	r1, [sp, #0xc]
  48984c: e1a06002     	mov	r6, r2
  489850: e7930102     	ldr	r0, [r3, r2, lsl #2]
  489854: eb001c2a     	bl	0x490904 <rnd::RoomPool::ComputeSizeOfRules()> @ imm = #0x70a8
  489858: e3500000     	cmp	r0, #0
  48985c: 1a000006     	bne	0x48987c <rnd::RandomGenerator::LoadRuleFile(char const*)+0x374> @ imm = #0x18
  489860: e7953007     	ldr	r3, [r5, r7]
  489864: e5933000     	ldr	r3, [r3]
  489868: e3530002     	cmp	r3, #2
  48986c: 05800000     	streq	r0, [r0]
  489870: 0a000001     	beq	0x48987c <rnd::RandomGenerator::LoadRuleFile(char const*)+0x374> @ imm = #0x4
  489874: e3530001     	cmp	r3, #1
  489878: 0a000011     	beq	0x4898c4 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x3bc> @ imm = #0x44
  48987c: e5943118     	ldr	r3, [r4, #0x118]
  489880: e594111c     	ldr	r1, [r4, #0x11c]
  489884: e2866001     	add	r6, r6, #1
  489888: e1a02006     	mov	r2, r6
  48988c: e0631001     	rsb	r1, r3, r1
  489890: e1560141     	cmp	r6, r1, asr #2
  489894: 3affffed     	blo	0x489850 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x348> @ imm = #-0x4c
  489898: e59d2008     	ldr	r2, [sp, #0x8]
  48989c: e1a00008     	mov	r0, r8
  4898a0: e7953002     	ldr	r3, [r5, r2]
  4898a4: e59d205c     	ldr	r2, [sp, #0x5c]
  4898a8: e5933000     	ldr	r3, [r3]
  4898ac: e1520003     	cmp	r2, r3
  4898b0: 1a000023     	bne	0x489944 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x43c> @ imm = #0x8c
  4898b4: e28dd064     	add	sp, sp, #100
  4898b8: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  4898bc: e3a08000     	mov	r8, #0
  4898c0: eafffff4     	b	0x489898 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x390> @ imm = #-0x30
  4898c4: e795000a     	ldr	r0, [r5, r10]
  4898c8: e3a0c0a6     	mov	r12, #166
  4898cc: e1a01009     	mov	r1, r9
  4898d0: e1a0200b     	mov	r2, r11
  4898d4: e59d300c     	ldr	r3, [sp, #0xc]
  4898d8: e28000a8     	add	r0, r0, #168
  4898dc: e58dc000     	str	r12, [sp]
  4898e0: ebfa11c7     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x17b8e4
  4898e4: eaffffe4     	b	0x48987c <rnd::RandomGenerator::LoadRuleFile(char const*)+0x374> @ imm = #-0x70
  4898e8: e59f6088     	ldr	r6, [pc, #0x88]         @ 0x489978 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x470>
  4898ec: e59f1088     	ldr	r1, [pc, #0x88]         @ 0x48997c <rnd::RandomGenerator::LoadRuleFile(char const*)+0x474>
  4898f0: e1a02008     	mov	r2, r8
  4898f4: e08f6006     	add	r6, pc, r6
  4898f8: e286600c     	add	r6, r6, #12
  4898fc: e08f1001     	add	r1, pc, r1
  489900: e1a00006     	mov	r0, r6
  489904: ebfa1476     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x17ae28
  489908: e3a02000     	mov	r2, #0
  48990c: e1a01006     	mov	r1, r6
  489910: eaffff4a     	b	0x489640 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x138> @ imm = #-0x2d8
  489914: ebfa1ac9     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1794dc
  489918: eaffff46     	b	0x489638 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x130> @ imm = #-0x2e8
  48991c: ebfa1ac7     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1794e4
  489920: eaffff2c     	b	0x4895d8 <rnd::RandomGenerator::LoadRuleFile(char const*)+0xd0> @ imm = #-0x350
  489924: e59f2054     	ldr	r2, [pc, #0x54]         @ 0x489980 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x478>
  489928: e08f2002     	add	r2, pc, r2
  48992c: e1a09002     	mov	r9, r2
  489930: eaffff8b     	b	0x489764 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x25c> @ imm = #-0x1d4
  489934: e59f2048     	ldr	r2, [pc, #0x48]         @ 0x489984 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x47c>
  489938: e08f2002     	add	r2, pc, r2
  48993c: e1a07002     	mov	r7, r2
  489940: eaffff98     	b	0x4897a8 <rnd::RandomGenerator::LoadRuleFile(char const*)+0x2a0> @ imm = #-0x1a0
  489944: ebfa1271     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x17b63c
  489948: 78 b5 50 00  	.word	0x0050b578
  48994c: ac 40 00 00  	.word	0x000040ac
  489950: f4 37 00 00  	.word	0x000037f4
  489954: ac b5 44 00  	.word	0x0044b5ac
  489958: 58 29 48 00  	.word	0x00482958
  48995c: 30 b5 44 00  	.word	0x0044b530
  489960: b8 b4 44 00  	.word	0x0044b4b8
  489964: 7c b4 44 00  	.word	0x0044b47c
  489968: 98 4b 43 00  	.word	0x00434b98
  48996c: 24 4d 43 00  	.word	0x00434d24
  489970: c0 39 00 00  	.word	0x000039c0
  489974: c0 19 00 00  	.word	0x000019c0
  489978: 04 c9 51 00  	.word	0x0051c904
  48997c: 8c b3 44 00  	.word	0x0044b38c
  489980: e0 1e 44 00  	.word	0x00441ee0
  489984: d0 1e 44 00  	.word	0x00441ed0
