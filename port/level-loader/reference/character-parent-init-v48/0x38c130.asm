
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0038c130 <GameObject::GameObject(ObjectBase::GO_IDS)>:
  38c130: e92d40f0     	push	{r4, r5, r6, r7, lr}
  38c134: e59f6254     	ldr	r6, [pc, #0x254]        @ 0x38c390 <GameObject::GameObject(ObjectBase::GO_IDS)+0x260>
  38c138: e24dd00c     	sub	sp, sp, #12
  38c13c: e1a04000     	mov	r4, r0
  38c140: ebfecc72     	bl	0x33f310 <ObjectBase::ObjectBase(ObjectBase::GO_IDS)> @ imm = #-0x4ce38
  38c144: e59f2248     	ldr	r2, [pc, #0x248]        @ 0x38c394 <GameObject::GameObject(ObjectBase::GO_IDS)+0x264>
  38c148: e08f6006     	add	r6, pc, r6
  38c14c: e3a03000     	mov	r3, #0
  38c150: e7962002     	ldr	r2, [r6, r2]
  38c154: e3a05000     	mov	r5, #0
  38c158: e5843120     	str	r3, [r4, #0x120]
  38c15c: e28210e4     	add	r1, r2, #228
  38c160: e2820008     	add	r0, r2, #8
  38c164: e28220d8     	add	r2, r2, #216
  38c168: e5842004     	str	r2, [r4, #0x4]
  38c16c: e5841024     	str	r1, [r4, #0x24]
  38c170: e5840000     	str	r0, [r4]
  38c174: e5843124     	str	r3, [r4, #0x124]
  38c178: e5843128     	str	r3, [r4, #0x128]
  38c17c: e584312c     	str	r3, [r4, #0x12c]
  38c180: e5843130     	str	r3, [r4, #0x130]
  38c184: e5843134     	str	r3, [r4, #0x134]
  38c188: e5843138     	str	r3, [r4, #0x138]
  38c18c: e584313c     	str	r3, [r4, #0x13c]
  38c190: e5843140     	str	r3, [r4, #0x140]
  38c194: e5843144     	str	r3, [r4, #0x144]
  38c198: e5843148     	str	r3, [r4, #0x148]
  38c19c: e584314c     	str	r3, [r4, #0x14c]
  38c1a0: e5843150     	str	r3, [r4, #0x150]
  38c1a4: e5843154     	str	r3, [r4, #0x154]
  38c1a8: e5843158     	str	r3, [r4, #0x158]
  38c1ac: e5843160     	str	r3, [r4, #0x160]
  38c1b0: e5843164     	str	r3, [r4, #0x164]
  38c1b4: e5843168     	str	r3, [r4, #0x168]
  38c1b8: e584316c     	str	r3, [r4, #0x16c]
  38c1bc: e5843170     	str	r3, [r4, #0x170]
  38c1c0: e5843174     	str	r3, [r4, #0x174]
  38c1c4: e5843178     	str	r3, [r4, #0x178]
  38c1c8: e5843184     	str	r3, [r4, #0x184]
  38c1cc: e5843188     	str	r3, [r4, #0x188]
  38c1d0: e584318c     	str	r3, [r4, #0x18c]
  38c1d4: e5843190     	str	r3, [r4, #0x190]
  38c1d8: e5843194     	str	r3, [r4, #0x194]
  38c1dc: e5c4515c     	strb	r5, [r4, #0x15c]
  38c1e0: e5845180     	str	r5, [r4, #0x180]
  38c1e4: e2840f72     	add	r0, r4, #456
  38c1e8: e5843198     	str	r3, [r4, #0x198]
  38c1ec: e58431c0     	str	r3, [r4, #0x1c0]
  38c1f0: e584319c     	str	r3, [r4, #0x19c]
  38c1f4: e58431a0     	str	r3, [r4, #0x1a0]
  38c1f8: e58431a4     	str	r3, [r4, #0x1a4]
  38c1fc: e58431a8     	str	r3, [r4, #0x1a8]
  38c200: e58431ac     	str	r3, [r4, #0x1ac]
  38c204: e58431b0     	str	r3, [r4, #0x1b0]
  38c208: e5c451b4     	strb	r5, [r4, #0x1b4]
  38c20c: e5c451b5     	strb	r5, [r4, #0x1b5]
  38c210: e58431b8     	str	r3, [r4, #0x1b8]
  38c214: e58431bc     	str	r3, [r4, #0x1bc]
  38c218: e5c451c4     	strb	r5, [r4, #0x1c4]
  38c21c: eb066108     	bl	0x524644 <PFObject::PFObject()> @ imm = #0x198420
  38c220: e2843f9e     	add	r3, r4, #632
  38c224: e3e07000     	mvn	r7, #0
  38c228: e3a02064     	mov	r2, #100
  38c22c: e5842274     	str	r2, [r4, #0x274]
  38c230: e1a00003     	mov	r0, r3
  38c234: e5843288     	str	r3, [r4, #0x288]
  38c238: e584328c     	str	r3, [r4, #0x28c]
  38c23c: e584526c     	str	r5, [r4, #0x26c]
  38c240: e5847270     	str	r7, [r4, #0x270]
  38c244: e3a01010     	mov	r1, #16
  38c248: ebfe150b     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x7abd4
  38c24c: e5942288     	ldr	r2, [r4, #0x288]
  38c250: e2843e29     	add	r3, r4, #656
  38c254: e1a00003     	mov	r0, r3
  38c258: e5c25000     	strb	r5, [r2]
  38c25c: e3a01010     	mov	r1, #16
  38c260: e58432a0     	str	r3, [r4, #0x2a0]
  38c264: e58432a4     	str	r3, [r4, #0x2a4]
  38c268: ebfe1503     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x7abf4
  38c26c: e59422a0     	ldr	r2, [r4, #0x2a0]
  38c270: e2843faa     	add	r3, r4, #680
  38c274: e1a00003     	mov	r0, r3
  38c278: e5c25000     	strb	r5, [r2]
  38c27c: e3a01010     	mov	r1, #16
  38c280: e58432b8     	str	r3, [r4, #0x2b8]
  38c284: e58432bc     	str	r3, [r4, #0x2bc]
  38c288: ebfe14fb     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x7ac14
  38c28c: e59422b8     	ldr	r2, [r4, #0x2b8]
  38c290: e2843d0b     	add	r3, r4, #704
  38c294: e1a00003     	mov	r0, r3
  38c298: e5c25000     	strb	r5, [r2]
  38c29c: e3a01010     	mov	r1, #16
  38c2a0: e58432d0     	str	r3, [r4, #0x2d0]
  38c2a4: e58432d4     	str	r3, [r4, #0x2d4]
  38c2a8: ebfe14f3     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x7ac34
  38c2ac: e59432d0     	ldr	r3, [r4, #0x2d0]
  38c2b0: e3a0c001     	mov	r12, #1
  38c2b4: e2846fc1     	add	r6, r4, #772
  38c2b8: e5c35000     	strb	r5, [r3]
  38c2bc: e1a0200c     	mov	r2, r12
  38c2c0: e5c4c2ee     	strb	r12, [r4, #0x2ee]
  38c2c4: e5c4c2fb     	strb	r12, [r4, #0x2fb]
  38c2c8: e58452d8     	str	r5, [r4, #0x2d8]
  38c2cc: e58452dc     	str	r5, [r4, #0x2dc]
  38c2d0: e58452e0     	str	r5, [r4, #0x2e0]
  38c2d4: e58452e4     	str	r5, [r4, #0x2e4]
  38c2d8: e58452e8     	str	r5, [r4, #0x2e8]
  38c2dc: e5c452ec     	strb	r5, [r4, #0x2ec]
  38c2e0: e5c452ed     	strb	r5, [r4, #0x2ed]
  38c2e4: e5c452ef     	strb	r5, [r4, #0x2ef]
  38c2e8: e5c452f0     	strb	r5, [r4, #0x2f0]
  38c2ec: e58452f4     	str	r5, [r4, #0x2f4]
  38c2f0: e5c452f8     	strb	r5, [r4, #0x2f8]
  38c2f4: e5c452f9     	strb	r5, [r4, #0x2f9]
  38c2f8: e5c452fa     	strb	r5, [r4, #0x2fa]
  38c2fc: e5c452fc     	strb	r5, [r4, #0x2fc]
  38c300: e5845300     	str	r5, [r4, #0x300]
  38c304: e1a03005     	mov	r3, r5
  38c308: e1a01005     	mov	r1, r5
  38c30c: e1a00006     	mov	r0, r6
  38c310: e58dc000     	str	r12, [sp]
  38c314: eb045905     	bl	0x4a2730 <ObjectSearcher::TargetList::TargetList(GameObject*, int, int, int)> @ imm = #0x116414
  38c318: e2843fd6     	add	r3, r4, #856
  38c31c: e1a00003     	mov	r0, r3
  38c320: e5843368     	str	r3, [r4, #0x368]
  38c324: e584336c     	str	r3, [r4, #0x36c]
  38c328: e3a01010     	mov	r1, #16
  38c32c: ebfe14d2     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x7acb8
  38c330: e5941368     	ldr	r1, [r4, #0x368]
  38c334: e3a024c2     	mov	r2, #-1040187392
  38c338: e3a03442     	mov	r3, #1107296256
  38c33c: e5c15000     	strb	r5, [r1]
  38c340: e2822732     	add	r2, r2, #13107200
  38c344: e2833732     	add	r3, r3, #13107200
  38c348: e3a01e37     	mov	r1, #880
  38c34c: e18470b1     	strh	r7, [r4, r1]
  38c350: e1a00004     	mov	r0, r4
  38c354: e584214c     	str	r2, [r4, #0x14c]
  38c358: e5843158     	str	r3, [r4, #0x158]
  38c35c: e5842144     	str	r2, [r4, #0x144]
  38c360: e5842148     	str	r2, [r4, #0x148]
  38c364: e5843150     	str	r3, [r4, #0x150]
  38c368: e5843154     	str	r3, [r4, #0x154]
  38c36c: e5c45373     	strb	r5, [r4, #0x373]
  38c370: e5c45372     	strb	r5, [r4, #0x372]
  38c374: ebfff9d3     	bl	0x38aac8 <GameObject::UpdateAbsoluteAABB()> @ imm = #-0x18b4
  38c378: e1a00006     	mov	r0, r6
  38c37c: e1a01004     	mov	r1, r4
  38c380: eb045565     	bl	0x4a191c <ObjectSearcher::TargetList::SetRefObject(GameObject*)> @ imm = #0x115594
  38c384: e1a00004     	mov	r0, r4
  38c388: e28dd00c     	add	sp, sp, #12
  38c38c: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  38c390: 48 89 60 00  	.word	0x00608948
  38c394: 70 2d 00 00  	.word	0x00002d70
