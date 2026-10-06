
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00399ff8 <TriggerObject::InitPost()>:
  399ff8: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  399ffc: e24dd024     	sub	sp, sp, #36
  39a000: e1a04000     	mov	r4, r0
  39a004: ebffc756     	bl	0x38bd64 <GameObject::CheckSpawnProbability()> @ imm = #-0xe2a8
  39a008: e5943274     	ldr	r3, [r4, #0x274]
  39a00c: e59f533c     	ldr	r5, [pc, #0x33c]        @ 0x39a350 <TriggerObject::InitPost()+0x358>
  39a010: e1500003     	cmp	r0, r3
  39a014: e08f5005     	add	r5, pc, r5
  39a018: aa0000b6     	bge	0x39a2f8 <TriggerObject::InitPost()+0x300> @ imm = #0x2d8
  39a01c: e59f3330     	ldr	r3, [pc, #0x330]        @ 0x39a354 <TriggerObject::InitPost()+0x35c>
  39a020: e594872c     	ldr	r8, [r4, #0x72c]
  39a024: e7953003     	ldr	r3, [r5, r3]
  39a028: e5937000     	ldr	r7, [r3]
  39a02c: e3570000     	cmp	r7, #0
  39a030: 0a0000b5     	beq	0x39a30c <TriggerObject::InitPost()+0x314> @ imm = #0x2d4
  39a034: e59f331c     	ldr	r3, [pc, #0x31c]        @ 0x39a358 <TriggerObject::InitPost()+0x360>
  39a038: e3a06000     	mov	r6, #0
  39a03c: e7953003     	ldr	r3, [r5, r3]
  39a040: e593a000     	ldr	r10, [r3]
  39a044: ea000002     	b	0x39a054 <TriggerObject::InitPost()+0x5c> @ imm = #0x8
  39a048: e2866001     	add	r6, r6, #1
  39a04c: e1560007     	cmp	r6, r7
  39a050: 0a0000ad     	beq	0x39a30c <TriggerObject::InitPost()+0x314> @ imm = #0x2b4
  39a054: e79a1106     	ldr	r1, [r10, r6, lsl #2]
  39a058: e1a00008     	mov	r0, r8
  39a05c: ebfdd0ae     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x8bd48
  39a060: e3500000     	cmp	r0, #0
  39a064: 1afffff7     	bne	0x39a048 <TriggerObject::InitPost()+0x50> @ imm = #-0x24
  39a068: e3760001     	cmn	r6, #1
  39a06c: e5846730     	str	r6, [r4, #0x730]
  39a070: 0a000013     	beq	0x39a0c4 <TriggerObject::InitPost()+0xcc> @ imm = #0x4c
  39a074: e59f72e0     	ldr	r7, [pc, #0x2e0]        @ 0x39a35c <TriggerObject::InitPost()+0x364>
  39a078: e3a02018     	mov	r2, #24
  39a07c: e7953007     	ldr	r3, [r5, r7]
  39a080: e5933000     	ldr	r3, [r3]
  39a084: e0263692     	mla	r6, r2, r6, r3
  39a088: e5963014     	ldr	r3, [r6, #0x14]
  39a08c: e3730001     	cmn	r3, #1
  39a090: 0a00000b     	beq	0x39a0c4 <TriggerObject::InitPost()+0xcc> @ imm = #0x2c
  39a094: e59f22c4     	ldr	r2, [pc, #0x2c4]        @ 0x39a360 <TriggerObject::InitPost()+0x368>
  39a098: e3a0100c     	mov	r1, #12
  39a09c: e7952002     	ldr	r2, [r5, r2]
  39a0a0: e5922000     	ldr	r2, [r2]
  39a0a4: e0232391     	mla	r3, r1, r3, r2
  39a0a8: e5936008     	ldr	r6, [r3, #0x8]
  39a0ac: e1a00006     	mov	r0, r6
  39a0b0: ebfdcf67     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x8c264
  39a0b4: e1a01006     	mov	r1, r6
  39a0b8: e0862000     	add	r2, r6, r0
  39a0bc: e2840e29     	add	r0, r4, #656
  39a0c0: ebfdda46     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x896e8
  39a0c4: e59f3298     	ldr	r3, [pc, #0x298]        @ 0x39a364 <TriggerObject::InitPost()+0x36c>
  39a0c8: e5941748     	ldr	r1, [r4, #0x748]
  39a0cc: e3a02000     	mov	r2, #0
  39a0d0: e7956003     	ldr	r6, [r5, r3]
  39a0d4: e1a00006     	mov	r0, r6
  39a0d8: eb02fc44     	bl	0x4591f0 <ScriptManager::GetIDFromName(char const*, bool) const> @ imm = #0xbf110
  39a0dc: e5941764     	ldr	r1, [r4, #0x764]
  39a0e0: e584074c     	str	r0, [r4, #0x74c]
  39a0e4: e3a02000     	mov	r2, #0
  39a0e8: e1a00006     	mov	r0, r6
  39a0ec: eb02fc3f     	bl	0x4591f0 <ScriptManager::GetIDFromName(char const*, bool) const> @ imm = #0xbf0fc
  39a0f0: e5840768     	str	r0, [r4, #0x768]
  39a0f4: e1a00004     	mov	r0, r4
  39a0f8: ebfff9dd     	bl	0x398874 <Trigger::InitPost()> @ imm = #-0x188c
  39a0fc: e5947780     	ldr	r7, [r4, #0x780]
  39a100: e594377c     	ldr	r3, [r4, #0x77c]
  39a104: e1530007     	cmp	r3, r7
  39a108: 0a00007c     	beq	0x39a300 <TriggerObject::InitPost()+0x308> @ imm = #0x1f0
  39a10c: e59f1254     	ldr	r1, [pc, #0x254]        @ 0x39a368 <TriggerObject::InitPost()+0x370>
  39a110: e1a00007     	mov	r0, r7
  39a114: e08f1001     	add	r1, pc, r1
  39a118: ebfdd07f     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x8be04
  39a11c: e3500000     	cmp	r0, #0
  39a120: 0a000076     	beq	0x39a300 <TriggerObject::InitPost()+0x308> @ imm = #0x1d8
  39a124: e59f3240     	ldr	r3, [pc, #0x240]        @ 0x39a36c <TriggerObject::InitPost()+0x374>
  39a128: e7953003     	ldr	r3, [r5, r3]
  39a12c: e5938000     	ldr	r8, [r3]
  39a130: e3580000     	cmp	r8, #0
  39a134: 0a00001c     	beq	0x39a1ac <TriggerObject::InitPost()+0x1b4> @ imm = #0x70
  39a138: e59f3230     	ldr	r3, [pc, #0x230]        @ 0x39a370 <TriggerObject::InitPost()+0x378>
  39a13c: e3a06000     	mov	r6, #0
  39a140: e7953003     	ldr	r3, [r5, r3]
  39a144: e593a000     	ldr	r10, [r3]
  39a148: ea000002     	b	0x39a158 <TriggerObject::InitPost()+0x160> @ imm = #0x8
  39a14c: e2866001     	add	r6, r6, #1
  39a150: e1560008     	cmp	r6, r8
  39a154: 0a000014     	beq	0x39a1ac <TriggerObject::InitPost()+0x1b4> @ imm = #0x50
  39a158: e79a1106     	ldr	r1, [r10, r6, lsl #2]
  39a15c: e1a00007     	mov	r0, r7
  39a160: ebfdd06d     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x8be4c
  39a164: e3500000     	cmp	r0, #0
  39a168: 1afffff7     	bne	0x39a14c <TriggerObject::InitPost()+0x154> @ imm = #-0x24
  39a16c: e3760001     	cmn	r6, #1
  39a170: 0a00000d     	beq	0x39a1ac <TriggerObject::InitPost()+0x1b4> @ imm = #0x34
  39a174: e1a01000     	mov	r1, r0
  39a178: e3a0000c     	mov	r0, #12
  39a17c: ebfdd8fb     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x89c14
  39a180: e1a07000     	mov	r7, r0
  39a184: eb037959     	bl	0x4786f0 <ConditionList::ConditionList()> @ imm = #0xde564
  39a188: e59f31e4     	ldr	r3, [pc, #0x1e4]        @ 0x39a374 <TriggerObject::InitPost()+0x37c>
  39a18c: e5847788     	str	r7, [r4, #0x788]
  39a190: e1a00007     	mov	r0, r7
  39a194: e7953003     	ldr	r3, [r5, r3]
  39a198: e5933000     	ldr	r3, [r3]
  39a19c: e0836206     	add	r6, r3, r6, lsl #4
  39a1a0: e5962004     	ldr	r2, [r6, #0x4]
  39a1a4: e5961008     	ldr	r1, [r6, #0x8]
  39a1a8: eb0379d9     	bl	0x478914 <ConditionList::AssignPyData(Structs::v2ConditionStub*, int)> @ imm = #0xde764
  39a1ac: e5943730     	ldr	r3, [r4, #0x730]
  39a1b0: e3730001     	cmn	r3, #1
  39a1b4: 0a00004a     	beq	0x39a2e4 <TriggerObject::InitPost()+0x2ec> @ imm = #0x128
  39a1b8: e1a00004     	mov	r0, r4
  39a1bc: ebffc267     	bl	0x38ab60 <GameObject::MeetCondition() const> @ imm = #-0xf664
  39a1c0: e3500000     	cmp	r0, #0
  39a1c4: 0a000046     	beq	0x39a2e4 <TriggerObject::InitPost()+0x2ec> @ imm = #0x118
  39a1c8: e59432d8     	ldr	r3, [r4, #0x2d8]
  39a1cc: e3530000     	cmp	r3, #0
  39a1d0: 0a00000c     	beq	0x39a208 <TriggerObject::InitPost()+0x210> @ imm = #0x30
  39a1d4: e5d42784     	ldrb	r2, [r4, #0x784]
  39a1d8: e3520000     	cmp	r2, #0
  39a1dc: 1a00004d     	bne	0x39a318 <TriggerObject::InitPost()+0x320> @ imm = #0x134
  39a1e0: e593c038     	ldr	r12, [r3, #0x38]
  39a1e4: e59f118c     	ldr	r1, [pc, #0x18c]        @ 0x39a378 <TriggerObject::InitPost()+0x380>
  39a1e8: e1a03002     	mov	r3, r2
  39a1ec: e1a0000c     	mov	r0, r12
  39a1f0: e08f1001     	add	r1, pc, r1
  39a1f4: e59cc000     	ldr	r12, [r12]
  39a1f8: e58d2000     	str	r2, [sp]
  39a1fc: e3a02001     	mov	r2, #1
  39a200: e1a0e00f     	mov	lr, pc
  39a204: e59cf020     	ldr	pc, [r12, #0x20]
  39a208: e59f316c     	ldr	r3, [pc, #0x16c]        @ 0x39a37c <TriggerObject::InitPost()+0x384>
  39a20c: e3a01000     	mov	r1, #0
  39a210: e3a00028     	mov	r0, #40
  39a214: e7953003     	ldr	r3, [r5, r3]
  39a218: e1a06001     	mov	r6, r1
  39a21c: e5938044     	ldr	r8, [r3, #0x44]
  39a220: ebfdd8d2     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x89cb8
  39a224: e3a0c001     	mov	r12, #1
  39a228: e3a0e002     	mov	lr, #2
  39a22c: e1a0300c     	mov	r3, r12
  39a230: e1a01008     	mov	r1, r8
  39a234: e1a02004     	mov	r2, r4
  39a238: e58de010     	str	lr, [sp, #0x10]
  39a23c: e30fefff     	movw	lr, #0xffff
  39a240: e1a07000     	mov	r7, r0
  39a244: e58de014     	str	lr, [sp, #0x14]
  39a248: e58dc018     	str	r12, [sp, #0x18]
  39a24c: e58d6000     	str	r6, [sp]
  39a250: e58d6004     	str	r6, [sp, #0x4]
  39a254: e58d6008     	str	r6, [sp, #0x8]
  39a258: e58d600c     	str	r6, [sp, #0xc]
  39a25c: eb035423     	bl	0x46f2f0 <PhysicalObject::PhysicalObject(PhysicalWorld*, GameObject*, bool, bool, bool, bool, short, unsigned short, unsigned short, int)> @ imm = #0xd508c
  39a260: e59f3118     	ldr	r3, [pc, #0x118]        @ 0x39a380 <TriggerObject::InitPost()+0x388>
  39a264: e1a00004     	mov	r0, r4
  39a268: e1a01007     	mov	r1, r7
  39a26c: e7953003     	ldr	r3, [r5, r3]
  39a270: e1a02006     	mov	r2, r6
  39a274: e2833008     	add	r3, r3, #8
  39a278: e5873000     	str	r3, [r7]
  39a27c: ebffea5d     	bl	0x394bf8 <GameObject::SetPhysicalObject(PhysicalObject*, bool)> @ imm = #-0x568c
  39a280: e59f30fc     	ldr	r3, [pc, #0xfc]         @ 0x39a384 <TriggerObject::InitPost()+0x38c>
  39a284: e7953003     	ldr	r3, [r5, r3]
  39a288: e5930000     	ldr	r0, [r3]
  39a28c: e1500006     	cmp	r0, r6
  39a290: 0a00002c     	beq	0x39a348 <TriggerObject::InitPost()+0x350> @ imm = #0xb0
  39a294: e59f70c0     	ldr	r7, [pc, #0xc0]         @ 0x39a35c <TriggerObject::InitPost()+0x364>
  39a298: e5943730     	ldr	r3, [r4, #0x730]
  39a29c: e3a01018     	mov	r1, #24
  39a2a0: e7952007     	ldr	r2, [r5, r7]
  39a2a4: e5922000     	ldr	r2, [r2]
  39a2a8: e0232391     	mla	r3, r1, r3, r2
  39a2ac: e5931010     	ldr	r1, [r3, #0x10]
  39a2b0: ebff3dd1     	bl	0x3699fc <VoxSoundManager::LoadSound(int)> @ imm = #-0x308bc
  39a2b4: e7952007     	ldr	r2, [r5, r7]
  39a2b8: e5943730     	ldr	r3, [r4, #0x730]
  39a2bc: e3a01018     	mov	r1, #24
  39a2c0: e5922000     	ldr	r2, [r2]
  39a2c4: e1a00004     	mov	r0, r4
  39a2c8: e0232391     	mla	r3, r1, r3, r2
  39a2cc: e59f20b4     	ldr	r2, [pc, #0xb4]         @ 0x39a388 <TriggerObject::InitPost()+0x390>
  39a2d0: e593100c     	ldr	r1, [r3, #0xc]
  39a2d4: e08f2002     	add	r2, pc, r2
  39a2d8: e28dd024     	add	sp, sp, #36
  39a2dc: e8bd45f0     	pop	{r4, r5, r6, r7, r8, r10, lr}
  39a2e0: eaffd31e     	b	0x38ef60 <GameObject::LoadExternalScript(char const*, char const*)> @ imm = #-0xb388
  39a2e4: e1a00004     	mov	r0, r4
  39a2e8: e5943000     	ldr	r3, [r4]
  39a2ec: e3a01000     	mov	r1, #0
  39a2f0: e1a0e00f     	mov	lr, pc
  39a2f4: e593f040     	ldr	pc, [r3, #0x40]
  39a2f8: e28dd024     	add	sp, sp, #36
  39a2fc: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  39a300: e3a03001     	mov	r3, #1
  39a304: e5c43784     	strb	r3, [r4, #0x784]
  39a308: eaffffa7     	b	0x39a1ac <TriggerObject::InitPost()+0x1b4> @ imm = #-0x164
  39a30c: e3e03000     	mvn	r3, #0
  39a310: e5843730     	str	r3, [r4, #0x730]
  39a314: eaffff6a     	b	0x39a0c4 <TriggerObject::InitPost()+0xcc> @ imm = #-0x258
  39a318: e593c038     	ldr	r12, [r3, #0x38]
  39a31c: e59f1068     	ldr	r1, [pc, #0x68]         @ 0x39a38c <TriggerObject::InitPost()+0x394>
  39a320: e3a02000     	mov	r2, #0
  39a324: e1a03002     	mov	r3, r2
  39a328: e1a0000c     	mov	r0, r12
  39a32c: e08f1001     	add	r1, pc, r1
  39a330: e59cc000     	ldr	r12, [r12]
  39a334: e58d2000     	str	r2, [sp]
  39a338: e3a02001     	mov	r2, #1
  39a33c: e1a0e00f     	mov	lr, pc
  39a340: e59cf020     	ldr	pc, [r12, #0x20]
  39a344: eaffffaf     	b	0x39a208 <TriggerObject::InitPost()+0x210> @ imm = #-0x144
  39a348: e59f700c     	ldr	r7, [pc, #0xc]          @ 0x39a35c <TriggerObject::InitPost()+0x364>
  39a34c: eaffffd8     	b	0x39a2b4 <TriggerObject::InitPost()+0x2bc> @ imm = #-0xa0
  39a350: 7c aa 5f 00  	.word	0x005faa7c
  39a354: a8 0c 00 00  	.word	0x00000ca8
  39a358: 58 42 00 00  	.word	0x00004258
  39a35c: 1c 0e 00 00  	.word	0x00000e1c
  39a360: a8 1c 00 00  	.word	0x00001ca8
  39a364: 20 1a 00 00  	.word	0x00001a20
  39a368: 8c 05 54 00  	.word	0x0054058c
  39a36c: 64 2f 00 00  	.word	0x00002f64
  39a370: 54 35 00 00  	.word	0x00003554
  39a374: 40 28 00 00  	.word	0x00002840
  39a378: c8 88 52 00  	.word	0x005288c8
  39a37c: f4 37 00 00  	.word	0x000037f4
  39a380: 18 24 00 00  	.word	0x00002418
  39a384: a4 0d 00 00  	.word	0x00000da4
  39a388: ac 88 52 00  	.word	0x005288ac
  39a38c: 84 7f 52 00  	.word	0x00527f84


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003999d8 <TriggerObject::~TriggerObject()>:
  3999d8: e92d4010     	push	{r4, lr}
  3999dc: e1a04000     	mov	r4, r0
  3999e0: ebffffd5     	bl	0x39993c <TriggerObject::~TriggerObject()> @ imm = #-0xac
  3999e4: e1a00004     	mov	r0, r4
  3999e8: ebfdda94     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x895b0
  3999ec: e1a00004     	mov	r0, r4
  3999f0: e8bd8010     	pop	{r4, pc}


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0039993c <TriggerObject::~TriggerObject()>:
  39993c: e92d4070     	push	{r4, r5, r6, lr}
  399940: e59f2080     	ldr	r2, [pc, #0x80]         @ 0x3999c8 <TriggerObject::~TriggerObject()+0x8c>
  399944: e59f3080     	ldr	r3, [pc, #0x80]         @ 0x3999cc <TriggerObject::~TriggerObject()+0x90>
  399948: e5905788     	ldr	r5, [r0, #0x788]
  39994c: e08f2002     	add	r2, pc, r2
  399950: e7923003     	ldr	r3, [r2, r3]
  399954: e3550000     	cmp	r5, #0
  399958: e1a04000     	mov	r4, r0
  39995c: e28320f4     	add	r2, r3, #244
  399960: e2831008     	add	r1, r3, #8
  399964: e28330e8     	add	r3, r3, #232
  399968: e880000a     	stm	r0, {r1, r3}
  39996c: e5802024     	str	r2, [r0, #0x24]
  399970: 0a000005     	beq	0x39998c <TriggerObject::~TriggerObject()+0x50> @ imm = #0x14
  399974: e1a00005     	mov	r0, r5
  399978: eb037d4b     	bl	0x478eac <ConditionList::~ConditionList()> @ imm = #0xdf52c
  39997c: e1a00005     	mov	r0, r5
  399980: ebfddaae     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x89548
  399984: e3a03000     	mov	r3, #0
  399988: e5843788     	str	r3, [r4, #0x788]
  39998c: e2840e76     	add	r0, r4, #1888
  399990: e280000c     	add	r0, r0, #12
  399994: ebfdfa2e     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x81748
  399998: e2840e75     	add	r0, r4, #1872
  39999c: ebfdfa2c     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x81750
  3999a0: e2840e73     	add	r0, r4, #1840
  3999a4: e2800004     	add	r0, r0, #4
  3999a8: ebfdfa29     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x8175c
  3999ac: e2840e71     	add	r0, r4, #1808
  3999b0: e2800008     	add	r0, r0, #8
  3999b4: ebfdfa26     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x81768
  3999b8: e1a00004     	mov	r0, r4
  3999bc: ebfffe14     	bl	0x399214 <Trigger::~Trigger()> @ imm = #-0x7b0
  3999c0: e1a00004     	mov	r0, r4
  3999c4: e8bd8070     	pop	{r4, r5, r6, pc}
  3999c8: 44 b1 5f 00  	.word	0x005fb144
  3999cc: 64 2b 00 00  	.word	0x00002b64


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003999f4 <TriggerObject::~TriggerObject()>:
  3999f4: e92d4070     	push	{r4, r5, r6, lr}
  3999f8: e59f2080     	ldr	r2, [pc, #0x80]         @ 0x399a80 <TriggerObject::~TriggerObject()+0x8c>
  3999fc: e59f3080     	ldr	r3, [pc, #0x80]         @ 0x399a84 <TriggerObject::~TriggerObject()+0x90>
  399a00: e5905788     	ldr	r5, [r0, #0x788]
  399a04: e08f2002     	add	r2, pc, r2
  399a08: e7923003     	ldr	r3, [r2, r3]
  399a0c: e3550000     	cmp	r5, #0
  399a10: e1a04000     	mov	r4, r0
  399a14: e28320f4     	add	r2, r3, #244
  399a18: e2831008     	add	r1, r3, #8
  399a1c: e28330e8     	add	r3, r3, #232
  399a20: e880000a     	stm	r0, {r1, r3}
  399a24: e5802024     	str	r2, [r0, #0x24]
  399a28: 0a000005     	beq	0x399a44 <TriggerObject::~TriggerObject()+0x50> @ imm = #0x14
  399a2c: e1a00005     	mov	r0, r5
  399a30: eb037d1d     	bl	0x478eac <ConditionList::~ConditionList()> @ imm = #0xdf474
  399a34: e1a00005     	mov	r0, r5
  399a38: ebfdda80     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x89600
  399a3c: e3a03000     	mov	r3, #0
  399a40: e5843788     	str	r3, [r4, #0x788]
  399a44: e2840e76     	add	r0, r4, #1888
  399a48: e280000c     	add	r0, r0, #12
  399a4c: ebfdfa00     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x81800
  399a50: e2840e75     	add	r0, r4, #1872
  399a54: ebfdf9fe     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x81808
  399a58: e2840e73     	add	r0, r4, #1840
  399a5c: e2800004     	add	r0, r0, #4
  399a60: ebfdf9fb     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x81814
  399a64: e2840e71     	add	r0, r4, #1808
  399a68: e2800008     	add	r0, r0, #8
  399a6c: ebfdf9f8     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x81820
  399a70: e1a00004     	mov	r0, r4
  399a74: ebfffde6     	bl	0x399214 <Trigger::~Trigger()> @ imm = #-0x868
  399a78: e1a00004     	mov	r0, r4
  399a7c: e8bd8070     	pop	{r4, r5, r6, pc}
  399a80: 8c b0 5f 00  	.word	0x005fb08c
  399a84: 64 2b 00 00  	.word	0x00002b64


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004d9d04 <Structs::TriggerObject::~TriggerObject()>:
  4d9d04: e92d4010     	push	{r4, lr}
  4d9d08: e1a04000     	mov	r4, r0
  4d9d0c: ebffffec     	bl	0x4d9cc4 <Structs::TriggerObject::~TriggerObject()> @ imm = #-0x50
  4d9d10: e1a00004     	mov	r0, r4
  4d9d14: ebf8d9c9     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1c98dc
  4d9d18: e1a00004     	mov	r0, r4
  4d9d1c: e8bd8010     	pop	{r4, pc}


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004d9cc4 <Structs::TriggerObject::~TriggerObject()>:
  4d9cc4: e92d4010     	push	{r4, lr}
  4d9cc8: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x4d9cfc <Structs::TriggerObject::~TriggerObject()+0x38>
  4d9ccc: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x4d9d00 <Structs::TriggerObject::~TriggerObject()+0x3c>
  4d9cd0: e1a04000     	mov	r4, r0
  4d9cd4: e08f3003     	add	r3, pc, r3
  4d9cd8: e590000c     	ldr	r0, [r0, #0xc]
  4d9cdc: e7932002     	ldr	r2, [r3, r2]
  4d9ce0: e3500000     	cmp	r0, #0
  4d9ce4: e2822008     	add	r2, r2, #8
  4d9ce8: e5842000     	str	r2, [r4]
  4d9cec: 0a000000     	beq	0x4d9cf4 <Structs::TriggerObject::~TriggerObject()+0x30> @ imm = #0x0
  4d9cf0: ebf8d9d2     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1c98b8
  4d9cf4: e1a00004     	mov	r0, r4
  4d9cf8: e8bd8010     	pop	{r4, pc}
  4d9cfc: bc ad 4b 00  	.word	0x004badbc
  4d9d00: f4 10 00 00  	.word	0x000010f4


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004d9d20 <Structs::TriggerObject::~TriggerObject()>:
  4d9d20: e92d4010     	push	{r4, lr}
  4d9d24: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x4d9d58 <Structs::TriggerObject::~TriggerObject()+0x38>
  4d9d28: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x4d9d5c <Structs::TriggerObject::~TriggerObject()+0x3c>
  4d9d2c: e1a04000     	mov	r4, r0
  4d9d30: e08f3003     	add	r3, pc, r3
  4d9d34: e590000c     	ldr	r0, [r0, #0xc]
  4d9d38: e7932002     	ldr	r2, [r3, r2]
  4d9d3c: e3500000     	cmp	r0, #0
  4d9d40: e2822008     	add	r2, r2, #8
  4d9d44: e5842000     	str	r2, [r4]
  4d9d48: 0a000000     	beq	0x4d9d50 <Structs::TriggerObject::~TriggerObject()+0x30> @ imm = #0x0
  4d9d4c: ebf8d9bb     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1c9914
  4d9d50: e1a00004     	mov	r0, r4
  4d9d54: e8bd8010     	pop	{r4, pc}
  4d9d58: 60 ad 4b 00  	.word	0x004bad60
  4d9d5c: f4 10 00 00  	.word	0x000010f4


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00398da4 <Trigger::NetStructTrigger::NetStructTrigger()>:
  398da4: e92d4ff8     	push	{r3, r4, r5, r6, r7, r8, r9, r10, r11, lr}
  398da8: e59f7184     	ldr	r7, [pc, #0x184]        @ 0x398f34 <Trigger::NetStructTrigger::NetStructTrigger()+0x190>
  398dac: e1a04000     	mov	r4, r0
  398db0: eb11eacf     	bl	0x8138f4 <NetStruct::NetStruct()> @ imm = #0x47ab3c
  398db4: e59f317c     	ldr	r3, [pc, #0x17c]        @ 0x398f38 <Trigger::NetStructTrigger::NetStructTrigger()+0x194>
  398db8: e59f517c     	ldr	r5, [pc, #0x17c]        @ 0x398f3c <Trigger::NetStructTrigger::NetStructTrigger()+0x198>
  398dbc: e08f7007     	add	r7, pc, r7
  398dc0: e7973003     	ldr	r3, [r7, r3]
  398dc4: e5942150     	ldr	r2, [r4, #0x150]
  398dc8: e7970005     	ldr	r0, [r7, r5]
  398dcc: e2833008     	add	r3, r3, #8
  398dd0: e3a09000     	mov	r9, #0
  398dd4: e3a08000     	mov	r8, #0
  398dd8: e3a0cf4e     	mov	r12, #312
  398ddc: e18480fc     	strd	r8, r9, [r4, r12]
  398de0: e3520000     	cmp	r2, #0
  398de4: e3e01000     	mvn	r1, #0
  398de8: e3a02000     	mov	r2, #0
  398dec: e2800008     	add	r0, r0, #8
  398df0: e5843000     	str	r3, [r4]
  398df4: e3a03020     	mov	r3, #32
  398df8: e5843134     	str	r3, [r4, #0x134]
  398dfc: e5841144     	str	r1, [r4, #0x144]
  398e00: e5840130     	str	r0, [r4, #0x130]
  398e04: e5841140     	str	r1, [r4, #0x140]
  398e08: e5842148     	str	r2, [r4, #0x148]
  398e0c: e5c4214c     	strb	r2, [r4, #0x14c]
  398e10: 02849e13     	addeq	r9, r4, #304
  398e14: 0a000003     	beq	0x398e28 <Trigger::NetStructTrigger::NetStructTrigger()+0x84> @ imm = #0xc
  398e18: e2849e13     	add	r9, r4, #304
  398e1c: e5842150     	str	r2, [r4, #0x150]
  398e20: e1a00009     	mov	r0, r9
  398e24: eb11f056     	bl	0x814f84 <NetStructMember::SetChanged()> @ imm = #0x47c158
  398e28: e59f8110     	ldr	r8, [pc, #0x110]        @ 0x398f40 <Trigger::NetStructTrigger::NetStructTrigger()+0x19c>
  398e2c: e5943178     	ldr	r3, [r4, #0x178]
  398e30: e7971005     	ldr	r1, [r7, r5]
  398e34: e7970008     	ldr	r0, [r7, r8]
  398e38: e3a0ce16     	mov	r12, #352
  398e3c: e3a0a000     	mov	r10, #0
  398e40: e2800008     	add	r0, r0, #8
  398e44: e3a0b000     	mov	r11, #0
  398e48: e184a0fc     	strd	r10, r11, [r4, r12]
  398e4c: e3530000     	cmp	r3, #0
  398e50: e3e02000     	mvn	r2, #0
  398e54: e3a03000     	mov	r3, #0
  398e58: e2811008     	add	r1, r1, #8
  398e5c: e5840130     	str	r0, [r4, #0x130]
  398e60: e3a00020     	mov	r0, #32
  398e64: e584015c     	str	r0, [r4, #0x15c]
  398e68: e584216c     	str	r2, [r4, #0x16c]
  398e6c: e5841158     	str	r1, [r4, #0x158]
  398e70: e5842168     	str	r2, [r4, #0x168]
  398e74: e5843170     	str	r3, [r4, #0x170]
  398e78: e5c43174     	strb	r3, [r4, #0x174]
  398e7c: 02846f56     	addeq	r6, r4, #344
  398e80: 0a000003     	beq	0x398e94 <Trigger::NetStructTrigger::NetStructTrigger()+0xf0> @ imm = #0xc
  398e84: e2846f56     	add	r6, r4, #344
  398e88: e5843178     	str	r3, [r4, #0x178]
  398e8c: e1a00006     	mov	r0, r6
  398e90: eb11f03b     	bl	0x814f84 <NetStructMember::SetChanged()> @ imm = #0x47c0ec
  398e94: e7970008     	ldr	r0, [r7, r8]
  398e98: e59431a0     	ldr	r3, [r4, #0x1a0]
  398e9c: e7971005     	ldr	r1, [r7, r5]
  398ea0: e2800008     	add	r0, r0, #8
  398ea4: e3a0cf62     	mov	r12, #392
  398ea8: e3a0a000     	mov	r10, #0
  398eac: e3a0b000     	mov	r11, #0
  398eb0: e184a0fc     	strd	r10, r11, [r4, r12]
  398eb4: e3530000     	cmp	r3, #0
  398eb8: e3e02000     	mvn	r2, #0
  398ebc: e3a03000     	mov	r3, #0
  398ec0: e2811008     	add	r1, r1, #8
  398ec4: e5840158     	str	r0, [r4, #0x158]
  398ec8: e3a00020     	mov	r0, #32
  398ecc: e5840184     	str	r0, [r4, #0x184]
  398ed0: e5842194     	str	r2, [r4, #0x194]
  398ed4: e5841180     	str	r1, [r4, #0x180]
  398ed8: e5842190     	str	r2, [r4, #0x190]
  398edc: e5843198     	str	r3, [r4, #0x198]
  398ee0: e5c4319c     	strb	r3, [r4, #0x19c]
  398ee4: 02845d06     	addeq	r5, r4, #384
  398ee8: 0a000003     	beq	0x398efc <Trigger::NetStructTrigger::NetStructTrigger()+0x158> @ imm = #0xc
  398eec: e2845d06     	add	r5, r4, #384
  398ef0: e58431a0     	str	r3, [r4, #0x1a0]
  398ef4: e1a00005     	mov	r0, r5
  398ef8: eb11f021     	bl	0x814f84 <NetStructMember::SetChanged()> @ imm = #0x47c084
  398efc: e7973008     	ldr	r3, [r7, r8]
  398f00: e1a01009     	mov	r1, r9
  398f04: e1a00004     	mov	r0, r4
  398f08: e2833008     	add	r3, r3, #8
  398f0c: e5843180     	str	r3, [r4, #0x180]
  398f10: eb11e8cd     	bl	0x81324c <NetStruct::DeclareMember(NetStructMember*)> @ imm = #0x47a334
  398f14: e1a00004     	mov	r0, r4
  398f18: e1a01006     	mov	r1, r6
  398f1c: eb11e8ca     	bl	0x81324c <NetStruct::DeclareMember(NetStructMember*)> @ imm = #0x47a328
  398f20: e1a00004     	mov	r0, r4
  398f24: e1a01005     	mov	r1, r5
  398f28: eb11e8c7     	bl	0x81324c <NetStruct::DeclareMember(NetStructMember*)> @ imm = #0x47a31c
  398f2c: e1a00004     	mov	r0, r4
  398f30: e8bd8ff8     	pop	{r3, r4, r5, r6, r7, r8, r9, r10, r11, pc}
  398f34: d4 bc 5f 00  	.word	0x005fbcd4
  398f38: 14 0d 00 00  	.word	0x00000d14
  398f3c: 84 29 00 00  	.word	0x00002984
  398f40: c8 10 00 00  	.word	0x000010c8


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00398a88 <Trigger::DeclareProperties()>:
  398a88: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  398a8c: e24dd008     	sub	sp, sp, #8
  398a90: e1a05000     	mov	r5, r0
  398a94: ebfffcd7     	bl	0x397df8 <Zone::DeclareProperties()> @ imm = #-0xca4
  398a98: e59f10a4     	ldr	r1, [pc, #0xa4]         @ 0x398b44 <Trigger::DeclareProperties()+0xbc>
  398a9c: e2854004     	add	r4, r5, #4
  398aa0: e1a00004     	mov	r0, r4
  398aa4: e2852fea     	add	r2, r5, #936
  398aa8: e08f1001     	add	r1, pc, r1
  398aac: e3a03001     	mov	r3, #1
  398ab0: ebffff70     	bl	0x398878 <void PropertyMap::AddProperty<int>(char const*, int&, int)> @ imm = #-0x240
  398ab4: e59f108c     	ldr	r1, [pc, #0x8c]         @ 0x398b48 <Trigger::DeclareProperties()+0xc0>
  398ab8: e2852feb     	add	r2, r5, #940
  398abc: e3a03000     	mov	r3, #0
  398ac0: e1a00004     	mov	r0, r4
  398ac4: e08f1001     	add	r1, pc, r1
  398ac8: ebffff6a     	bl	0x398878 <void PropertyMap::AddProperty<int>(char const*, int&, int)> @ imm = #-0x258
  398acc: e3a01000     	mov	r1, #0
  398ad0: e3a00024     	mov	r0, #36
  398ad4: ebfddea5     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x8856c
  398ad8: e59f706c     	ldr	r7, [pc, #0x6c]         @ 0x398b4c <Trigger::DeclareProperties()+0xc4>
  398adc: e59f306c     	ldr	r3, [pc, #0x6c]         @ 0x398b50 <Trigger::DeclareProperties()+0xc8>
  398ae0: e59f806c     	ldr	r8, [pc, #0x6c]         @ 0x398b54 <Trigger::DeclareProperties()+0xcc>
  398ae4: e08f7007     	add	r7, pc, r7
  398ae8: e7973003     	ldr	r3, [r7, r3]
  398aec: e08f8008     	add	r8, pc, r8
  398af0: e1a06000     	mov	r6, r0
  398af4: e2833008     	add	r3, r3, #8
  398af8: e1a01008     	mov	r1, r8
  398afc: e28d2004     	add	r2, sp, #4
  398b00: e4803008     	str	r3, [r0], #8
  398b04: ebfded78     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x84a20
  398b08: e59f3048     	ldr	r3, [pc, #0x48]         @ 0x398b58 <Trigger::DeclareProperties()+0xd0>
  398b0c: e2855e3b     	add	r5, r5, #944
  398b10: e0645005     	rsb	r5, r4, r5
  398b14: e7973003     	ldr	r3, [r7, r3]
  398b18: e5865004     	str	r5, [r6, #0x4]
  398b1c: e1a00004     	mov	r0, r4
  398b20: e2833008     	add	r3, r3, #8
  398b24: e5863000     	str	r3, [r6]
  398b28: e3a03000     	mov	r3, #0
  398b2c: e5c63020     	strb	r3, [r6, #0x20]
  398b30: e1a01008     	mov	r1, r8
  398b34: e1a02006     	mov	r2, r6
  398b38: eb05ec69     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x17b1a4
  398b3c: e28dd008     	add	sp, sp, #8
  398b40: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  398b44: d0 9f 52 00  	.word	0x00529fd0
  398b48: c4 9f 52 00  	.word	0x00529fc4
  398b4c: ac bf 5f 00  	.word	0x005fbfac
  398b50: 30 23 00 00  	.word	0x00002330
  398b54: ac 9f 52 00  	.word	0x00529fac
  398b58: 4c 3e 00 00  	.word	0x00003e4c


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00398f44 <Trigger::Trigger(ObjectBase::GO_IDS, bool, bool)>:
  398f44: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  398f48: e59f507c     	ldr	r5, [pc, #0x7c]         @ 0x398fcc <Trigger::Trigger(ObjectBase::GO_IDS, bool, bool)+0x88>
  398f4c: e1a04000     	mov	r4, r0
  398f50: ebfffbf4     	bl	0x397f28 <ZoneEx::ZoneEx(ObjectBase::GO_IDS, bool, bool)> @ imm = #-0x1030
  398f54: e59f2074     	ldr	r2, [pc, #0x74]         @ 0x398fd0 <Trigger::Trigger(ObjectBase::GO_IDS, bool, bool)+0x8c>
  398f58: e08f5005     	add	r5, pc, r5
  398f5c: e3a03000     	mov	r3, #0
  398f60: e7952002     	ldr	r2, [r5, r2]
  398f64: e3a06001     	mov	r6, #1
  398f68: e2848ff2     	add	r8, r4, #968
  398f6c: e28210f4     	add	r1, r2, #244
  398f70: e2820008     	add	r0, r2, #8
  398f74: e28220e8     	add	r2, r2, #232
  398f78: e58433c0     	str	r3, [r4, #0x3c0]
  398f7c: e58433ac     	str	r3, [r4, #0x3ac]
  398f80: e5c433b0     	strb	r3, [r4, #0x3b0]
  398f84: e58433b4     	str	r3, [r4, #0x3b4]
  398f88: e58433b8     	str	r3, [r4, #0x3b8]
  398f8c: e5c433bc     	strb	r3, [r4, #0x3bc]
  398f90: e8840005     	stm	r4, {r0, r2}
  398f94: e5841024     	str	r1, [r4, #0x24]
  398f98: e2847e57     	add	r7, r4, #1392
  398f9c: e58463a8     	str	r6, [r4, #0x3a8]
  398fa0: e1a00008     	mov	r0, r8
  398fa4: ebffff7e     	bl	0x398da4 <Trigger::NetStructTrigger::NetStructTrigger()> @ imm = #-0x208
  398fa8: e1a00007     	mov	r0, r7
  398fac: ebffff7c     	bl	0x398da4 <Trigger::NetStructTrigger::NetStructTrigger()> @ imm = #-0x210
  398fb0: e3a03004     	mov	r3, #4
  398fb4: e5848100     	str	r8, [r4, #0x100]
  398fb8: e5847104     	str	r7, [r4, #0x104]
  398fbc: e5c46028     	strb	r6, [r4, #0x28]
  398fc0: e5c430f8     	strb	r3, [r4, #0xf8]
  398fc4: e1a00004     	mov	r0, r4
  398fc8: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  398fcc: 38 bb 5f 00  	.word	0x005fbb38
  398fd0: f0 16 00 00  	.word	0x000016f0


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00398fd4 <Trigger::Trigger(ObjectBase::GO_IDS, bool, bool)>:
  398fd4: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  398fd8: e59f507c     	ldr	r5, [pc, #0x7c]         @ 0x39905c <Trigger::Trigger(ObjectBase::GO_IDS, bool, bool)+0x88>
  398fdc: e1a04000     	mov	r4, r0
  398fe0: ebfffbd0     	bl	0x397f28 <ZoneEx::ZoneEx(ObjectBase::GO_IDS, bool, bool)> @ imm = #-0x10c0
  398fe4: e59f2074     	ldr	r2, [pc, #0x74]         @ 0x399060 <Trigger::Trigger(ObjectBase::GO_IDS, bool, bool)+0x8c>
  398fe8: e08f5005     	add	r5, pc, r5
  398fec: e3a03000     	mov	r3, #0
  398ff0: e7952002     	ldr	r2, [r5, r2]
  398ff4: e3a06001     	mov	r6, #1
  398ff8: e2848ff2     	add	r8, r4, #968
  398ffc: e28210f4     	add	r1, r2, #244
  399000: e2820008     	add	r0, r2, #8
  399004: e28220e8     	add	r2, r2, #232
  399008: e58433c0     	str	r3, [r4, #0x3c0]
  39900c: e58433ac     	str	r3, [r4, #0x3ac]
  399010: e5c433b0     	strb	r3, [r4, #0x3b0]
  399014: e58433b4     	str	r3, [r4, #0x3b4]
  399018: e58433b8     	str	r3, [r4, #0x3b8]
  39901c: e5c433bc     	strb	r3, [r4, #0x3bc]
  399020: e8840005     	stm	r4, {r0, r2}
  399024: e5841024     	str	r1, [r4, #0x24]
  399028: e2847e57     	add	r7, r4, #1392
  39902c: e58463a8     	str	r6, [r4, #0x3a8]
  399030: e1a00008     	mov	r0, r8
  399034: ebffff5a     	bl	0x398da4 <Trigger::NetStructTrigger::NetStructTrigger()> @ imm = #-0x298
  399038: e1a00007     	mov	r0, r7
  39903c: ebffff58     	bl	0x398da4 <Trigger::NetStructTrigger::NetStructTrigger()> @ imm = #-0x2a0
  399040: e3a03004     	mov	r3, #4
  399044: e5848100     	str	r8, [r4, #0x100]
  399048: e5847104     	str	r7, [r4, #0x104]
  39904c: e5c46028     	strb	r6, [r4, #0x28]
  399050: e5c430f8     	strb	r3, [r4, #0xf8]
  399054: e1a00004     	mov	r0, r4
  399058: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  39905c: a8 ba 5f 00  	.word	0x005fbaa8
  399060: f0 16 00 00  	.word	0x000016f0


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00399328 <TriggerObject::IsAnimated() const>:
  399328: e3a00001     	mov	r0, #1
  39932c: e12fff1e     	bx	lr


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00399318 <TriggerObject::IsUpdatable() const>:
  399318: e3a00001     	mov	r0, #1
  39931c: e12fff1e     	bx	lr


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00399490 <TriggerObject::IsInteractive(GameObject*) const>:
  399490: e92d4010     	push	{r4, lr}
  399494: e1a04000     	mov	r4, r0
  399498: ebfffcc3     	bl	0x3987ac <Trigger::CanActivate() const> @ imm = #-0xcf4
  39949c: e3500000     	cmp	r0, #0
  3994a0: 15d40784     	ldrbne	r0, [r4, #0x784]
  3994a4: e8bd8010     	pop	{r4, pc}


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00399330 <TriggerObject::GetInteractionType(GameObject*) const>:
  399330: e5900730     	ldr	r0, [r0, #0x730]
  399334: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x399360 <TriggerObject::GetInteractionType(GameObject*) const+0x30>
  399338: e3700001     	cmn	r0, #1
  39933c: e08f3003     	add	r3, pc, r3
  399340: 012fff1e     	bxeq	lr
  399344: e59f2018     	ldr	r2, [pc, #0x18]         @ 0x399364 <TriggerObject::GetInteractionType(GameObject*) const+0x34>
  399348: e7933002     	ldr	r3, [r3, r2]
  39934c: e3a02018     	mov	r2, #24
  399350: e5933000     	ldr	r3, [r3]
  399354: e0203092     	mla	r0, r2, r0, r3
  399358: e5900004     	ldr	r0, [r0, #0x4]
  39935c: e12fff1e     	bx	lr
  399360: 54 b7 5f 00  	.word	0x005fb754
  399364: 1c 0e 00 00  	.word	0x00000e1c


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00399320 <TriggerObject::IsZonable() const>:
  399320: e3a00001     	mov	r0, #1
  399324: e12fff1e     	bx	lr


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003999d0 <non-virtual thunk to TriggerObject::~TriggerObject()>:
  3999d0: e2400024     	sub	r0, r0, #36
  3999d4: eaffffff     	b	0x3999d8 <TriggerObject::~TriggerObject()> @ imm = #-0x4


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00399934 <non-virtual thunk to TriggerObject::~TriggerObject()>:
  399934: e2400024     	sub	r0, r0, #36
  399938: eaffffff     	b	0x39993c <TriggerObject::~TriggerObject()> @ imm = #-0x4


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00398a80 <non-virtual thunk to Trigger::DeclareProperties()>:
  398a80: e2400004     	sub	r0, r0, #4
  398a84: eaffffff     	b	0x398a88 <Trigger::DeclareProperties()> @ imm = #-0x4
