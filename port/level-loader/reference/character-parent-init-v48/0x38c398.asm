
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0038c398 <GameObject::GameObject(ObjectBase::GO_IDS)>:
  38c398: e92d40f0     	push	{r4, r5, r6, r7, lr}
  38c39c: e59f6254     	ldr	r6, [pc, #0x254]        @ 0x38c5f8 <GameObject::GameObject(ObjectBase::GO_IDS)+0x260>
  38c3a0: e24dd00c     	sub	sp, sp, #12
  38c3a4: e1a04000     	mov	r4, r0
  38c3a8: ebfecbd8     	bl	0x33f310 <ObjectBase::ObjectBase(ObjectBase::GO_IDS)> @ imm = #-0x4d0a0
  38c3ac: e59f2248     	ldr	r2, [pc, #0x248]        @ 0x38c5fc <GameObject::GameObject(ObjectBase::GO_IDS)+0x264>
  38c3b0: e08f6006     	add	r6, pc, r6
  38c3b4: e3a03000     	mov	r3, #0
  38c3b8: e7962002     	ldr	r2, [r6, r2]
  38c3bc: e3a05000     	mov	r5, #0
  38c3c0: e5843120     	str	r3, [r4, #0x120]
  38c3c4: e28210e4     	add	r1, r2, #228
  38c3c8: e2820008     	add	r0, r2, #8
  38c3cc: e28220d8     	add	r2, r2, #216
  38c3d0: e5842004     	str	r2, [r4, #0x4]
  38c3d4: e5841024     	str	r1, [r4, #0x24]
  38c3d8: e5840000     	str	r0, [r4]
  38c3dc: e5843124     	str	r3, [r4, #0x124]
  38c3e0: e5843128     	str	r3, [r4, #0x128]
  38c3e4: e584312c     	str	r3, [r4, #0x12c]
  38c3e8: e5843130     	str	r3, [r4, #0x130]
  38c3ec: e5843134     	str	r3, [r4, #0x134]
  38c3f0: e5843138     	str	r3, [r4, #0x138]
  38c3f4: e584313c     	str	r3, [r4, #0x13c]
  38c3f8: e5843140     	str	r3, [r4, #0x140]
  38c3fc: e5843144     	str	r3, [r4, #0x144]
  38c400: e5843148     	str	r3, [r4, #0x148]
  38c404: e584314c     	str	r3, [r4, #0x14c]
  38c408: e5843150     	str	r3, [r4, #0x150]
  38c40c: e5843154     	str	r3, [r4, #0x154]
  38c410: e5843158     	str	r3, [r4, #0x158]
  38c414: e5843160     	str	r3, [r4, #0x160]
  38c418: e5843164     	str	r3, [r4, #0x164]
  38c41c: e5843168     	str	r3, [r4, #0x168]
  38c420: e584316c     	str	r3, [r4, #0x16c]
  38c424: e5843170     	str	r3, [r4, #0x170]
  38c428: e5843174     	str	r3, [r4, #0x174]
  38c42c: e5843178     	str	r3, [r4, #0x178]
  38c430: e5843184     	str	r3, [r4, #0x184]
  38c434: e5843188     	str	r3, [r4, #0x188]
  38c438: e584318c     	str	r3, [r4, #0x18c]
  38c43c: e5843190     	str	r3, [r4, #0x190]
  38c440: e5843194     	str	r3, [r4, #0x194]
  38c444: e5c4515c     	strb	r5, [r4, #0x15c]
  38c448: e5845180     	str	r5, [r4, #0x180]
  38c44c: e2840f72     	add	r0, r4, #456
  38c450: e5843198     	str	r3, [r4, #0x198]
  38c454: e58431c0     	str	r3, [r4, #0x1c0]
  38c458: e584319c     	str	r3, [r4, #0x19c]
  38c45c: e58431a0     	str	r3, [r4, #0x1a0]
  38c460: e58431a4     	str	r3, [r4, #0x1a4]
  38c464: e58431a8     	str	r3, [r4, #0x1a8]
  38c468: e58431ac     	str	r3, [r4, #0x1ac]
  38c46c: e58431b0     	str	r3, [r4, #0x1b0]
  38c470: e5c451b4     	strb	r5, [r4, #0x1b4]
  38c474: e5c451b5     	strb	r5, [r4, #0x1b5]
  38c478: e58431b8     	str	r3, [r4, #0x1b8]
  38c47c: e58431bc     	str	r3, [r4, #0x1bc]
  38c480: e5c451c4     	strb	r5, [r4, #0x1c4]
  38c484: eb06606e     	bl	0x524644 <PFObject::PFObject()> @ imm = #0x1981b8
  38c488: e2843f9e     	add	r3, r4, #632
  38c48c: e3e07000     	mvn	r7, #0
  38c490: e3a02064     	mov	r2, #100
  38c494: e5842274     	str	r2, [r4, #0x274]
  38c498: e1a00003     	mov	r0, r3
  38c49c: e5843288     	str	r3, [r4, #0x288]
  38c4a0: e584328c     	str	r3, [r4, #0x28c]
  38c4a4: e584526c     	str	r5, [r4, #0x26c]
  38c4a8: e5847270     	str	r7, [r4, #0x270]
  38c4ac: e3a01010     	mov	r1, #16
  38c4b0: ebfe1471     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x7ae3c
  38c4b4: e5942288     	ldr	r2, [r4, #0x288]
  38c4b8: e2843e29     	add	r3, r4, #656
  38c4bc: e1a00003     	mov	r0, r3
  38c4c0: e5c25000     	strb	r5, [r2]
  38c4c4: e3a01010     	mov	r1, #16
  38c4c8: e58432a0     	str	r3, [r4, #0x2a0]
  38c4cc: e58432a4     	str	r3, [r4, #0x2a4]
  38c4d0: ebfe1469     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x7ae5c
  38c4d4: e59422a0     	ldr	r2, [r4, #0x2a0]
  38c4d8: e2843faa     	add	r3, r4, #680
  38c4dc: e1a00003     	mov	r0, r3
  38c4e0: e5c25000     	strb	r5, [r2]
  38c4e4: e3a01010     	mov	r1, #16
  38c4e8: e58432b8     	str	r3, [r4, #0x2b8]
  38c4ec: e58432bc     	str	r3, [r4, #0x2bc]
  38c4f0: ebfe1461     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x7ae7c
  38c4f4: e59422b8     	ldr	r2, [r4, #0x2b8]
  38c4f8: e2843d0b     	add	r3, r4, #704
  38c4fc: e1a00003     	mov	r0, r3
  38c500: e5c25000     	strb	r5, [r2]
  38c504: e3a01010     	mov	r1, #16
  38c508: e58432d0     	str	r3, [r4, #0x2d0]
  38c50c: e58432d4     	str	r3, [r4, #0x2d4]
  38c510: ebfe1459     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x7ae9c
  38c514: e59432d0     	ldr	r3, [r4, #0x2d0]
  38c518: e3a0c001     	mov	r12, #1
  38c51c: e2846fc1     	add	r6, r4, #772
  38c520: e5c35000     	strb	r5, [r3]
  38c524: e1a0200c     	mov	r2, r12
  38c528: e5c4c2ee     	strb	r12, [r4, #0x2ee]
  38c52c: e5c4c2fb     	strb	r12, [r4, #0x2fb]
  38c530: e58452d8     	str	r5, [r4, #0x2d8]
  38c534: e58452dc     	str	r5, [r4, #0x2dc]
  38c538: e58452e0     	str	r5, [r4, #0x2e0]
  38c53c: e58452e4     	str	r5, [r4, #0x2e4]
  38c540: e58452e8     	str	r5, [r4, #0x2e8]
  38c544: e5c452ec     	strb	r5, [r4, #0x2ec]
  38c548: e5c452ed     	strb	r5, [r4, #0x2ed]
  38c54c: e5c452ef     	strb	r5, [r4, #0x2ef]
  38c550: e5c452f0     	strb	r5, [r4, #0x2f0]
  38c554: e58452f4     	str	r5, [r4, #0x2f4]
  38c558: e5c452f8     	strb	r5, [r4, #0x2f8]
  38c55c: e5c452f9     	strb	r5, [r4, #0x2f9]
  38c560: e5c452fa     	strb	r5, [r4, #0x2fa]
  38c564: e5c452fc     	strb	r5, [r4, #0x2fc]
  38c568: e5845300     	str	r5, [r4, #0x300]
  38c56c: e1a03005     	mov	r3, r5
  38c570: e1a01005     	mov	r1, r5
  38c574: e1a00006     	mov	r0, r6
  38c578: e58dc000     	str	r12, [sp]
  38c57c: eb04586b     	bl	0x4a2730 <ObjectSearcher::TargetList::TargetList(GameObject*, int, int, int)> @ imm = #0x1161ac
  38c580: e2843fd6     	add	r3, r4, #856
  38c584: e1a00003     	mov	r0, r3
  38c588: e5843368     	str	r3, [r4, #0x368]
  38c58c: e584336c     	str	r3, [r4, #0x36c]
  38c590: e3a01010     	mov	r1, #16
  38c594: ebfe1438     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x7af20
  38c598: e5941368     	ldr	r1, [r4, #0x368]
  38c59c: e3a024c2     	mov	r2, #-1040187392
  38c5a0: e3a03442     	mov	r3, #1107296256
  38c5a4: e5c15000     	strb	r5, [r1]
  38c5a8: e2822732     	add	r2, r2, #13107200
  38c5ac: e2833732     	add	r3, r3, #13107200
  38c5b0: e3a01e37     	mov	r1, #880
  38c5b4: e18470b1     	strh	r7, [r4, r1]
  38c5b8: e1a00004     	mov	r0, r4
  38c5bc: e584214c     	str	r2, [r4, #0x14c]
  38c5c0: e5843158     	str	r3, [r4, #0x158]
  38c5c4: e5842144     	str	r2, [r4, #0x144]
  38c5c8: e5842148     	str	r2, [r4, #0x148]
  38c5cc: e5843150     	str	r3, [r4, #0x150]
  38c5d0: e5843154     	str	r3, [r4, #0x154]
  38c5d4: e5c45373     	strb	r5, [r4, #0x373]
  38c5d8: e5c45372     	strb	r5, [r4, #0x372]
  38c5dc: ebfff939     	bl	0x38aac8 <GameObject::UpdateAbsoluteAABB()> @ imm = #-0x1b1c
  38c5e0: e1a00006     	mov	r0, r6
  38c5e4: e1a01004     	mov	r1, r4
  38c5e8: eb0454cb     	bl	0x4a191c <ObjectSearcher::TargetList::SetRefObject(GameObject*)> @ imm = #0x11532c
  38c5ec: e1a00004     	mov	r0, r4
  38c5f0: e28dd00c     	add	sp, sp, #12
  38c5f4: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  38c5f8: e0 86 60 00  	.word	0x006086e0
  38c5fc: 70 2d 00 00  	.word	0x00002d70
