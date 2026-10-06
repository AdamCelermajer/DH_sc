
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003e80d4 <Door::NetStructDoor::NetStructDoor()>:
  3e80d4: e92d4ff8     	push	{r3, r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3e80d8: e59f7184     	ldr	r7, [pc, #0x184]        @ 0x3e8264 <Door::NetStructDoor::NetStructDoor()+0x190>
  3e80dc: e1a04000     	mov	r4, r0
  3e80e0: eb10ae03     	bl	0x8138f4 <NetStruct::NetStruct()> @ imm = #0x42b80c
  3e80e4: e59f317c     	ldr	r3, [pc, #0x17c]        @ 0x3e8268 <Door::NetStructDoor::NetStructDoor()+0x194>
  3e80e8: e59f517c     	ldr	r5, [pc, #0x17c]        @ 0x3e826c <Door::NetStructDoor::NetStructDoor()+0x198>
  3e80ec: e08f7007     	add	r7, pc, r7
  3e80f0: e7973003     	ldr	r3, [r7, r3]
  3e80f4: e5d4214d     	ldrb	r2, [r4, #0x14d]
  3e80f8: e7970005     	ldr	r0, [r7, r5]
  3e80fc: e2833008     	add	r3, r3, #8
  3e8100: e3a09000     	mov	r9, #0
  3e8104: e3a08000     	mov	r8, #0
  3e8108: e3a0cf4e     	mov	r12, #312
  3e810c: e18480fc     	strd	r8, r9, [r4, r12]
  3e8110: e3520000     	cmp	r2, #0
  3e8114: e3e01000     	mvn	r1, #0
  3e8118: e3a02000     	mov	r2, #0
  3e811c: e2800008     	add	r0, r0, #8
  3e8120: e5843000     	str	r3, [r4]
  3e8124: e3a03001     	mov	r3, #1
  3e8128: e5843134     	str	r3, [r4, #0x134]
  3e812c: e5841144     	str	r1, [r4, #0x144]
  3e8130: e5840130     	str	r0, [r4, #0x130]
  3e8134: e5841140     	str	r1, [r4, #0x140]
  3e8138: e5842148     	str	r2, [r4, #0x148]
  3e813c: e5c4214c     	strb	r2, [r4, #0x14c]
  3e8140: 02849e13     	addeq	r9, r4, #304
  3e8144: 0a000003     	beq	0x3e8158 <Door::NetStructDoor::NetStructDoor()+0x84> @ imm = #0xc
  3e8148: e2849e13     	add	r9, r4, #304
  3e814c: e5c4214d     	strb	r2, [r4, #0x14d]
  3e8150: e1a00009     	mov	r0, r9
  3e8154: eb10b38a     	bl	0x814f84 <NetStructMember::SetChanged()> @ imm = #0x42ce28
  3e8158: e59f8110     	ldr	r8, [pc, #0x110]        @ 0x3e8270 <Door::NetStructDoor::NetStructDoor()+0x19c>
  3e815c: e5d4316d     	ldrb	r3, [r4, #0x16d]
  3e8160: e7971005     	ldr	r1, [r7, r5]
  3e8164: e7970008     	ldr	r0, [r7, r8]
  3e8168: e3a0cf56     	mov	r12, #344
  3e816c: e3a0a000     	mov	r10, #0
  3e8170: e2800008     	add	r0, r0, #8
  3e8174: e3a0b000     	mov	r11, #0
  3e8178: e184a0fc     	strd	r10, r11, [r4, r12]
  3e817c: e3530000     	cmp	r3, #0
  3e8180: e3e02000     	mvn	r2, #0
  3e8184: e3a03000     	mov	r3, #0
  3e8188: e2811008     	add	r1, r1, #8
  3e818c: e5840130     	str	r0, [r4, #0x130]
  3e8190: e3a00001     	mov	r0, #1
  3e8194: e5840154     	str	r0, [r4, #0x154]
  3e8198: e5842164     	str	r2, [r4, #0x164]
  3e819c: e5841150     	str	r1, [r4, #0x150]
  3e81a0: e5842160     	str	r2, [r4, #0x160]
  3e81a4: e5843168     	str	r3, [r4, #0x168]
  3e81a8: e5c4316c     	strb	r3, [r4, #0x16c]
  3e81ac: 02846e15     	addeq	r6, r4, #336
  3e81b0: 0a000003     	beq	0x3e81c4 <Door::NetStructDoor::NetStructDoor()+0xf0> @ imm = #0xc
  3e81b4: e2846e15     	add	r6, r4, #336
  3e81b8: e5c4316d     	strb	r3, [r4, #0x16d]
  3e81bc: e1a00006     	mov	r0, r6
  3e81c0: eb10b36f     	bl	0x814f84 <NetStructMember::SetChanged()> @ imm = #0x42cdbc
  3e81c4: e7970008     	ldr	r0, [r7, r8]
  3e81c8: e5d4318d     	ldrb	r3, [r4, #0x18d]
  3e81cc: e7971005     	ldr	r1, [r7, r5]
  3e81d0: e2800008     	add	r0, r0, #8
  3e81d4: e3a0cf5e     	mov	r12, #376
  3e81d8: e3a0a000     	mov	r10, #0
  3e81dc: e3a0b000     	mov	r11, #0
  3e81e0: e184a0fc     	strd	r10, r11, [r4, r12]
  3e81e4: e3530000     	cmp	r3, #0
  3e81e8: e3e02000     	mvn	r2, #0
  3e81ec: e3a03000     	mov	r3, #0
  3e81f0: e2811008     	add	r1, r1, #8
  3e81f4: e5840150     	str	r0, [r4, #0x150]
  3e81f8: e3a00001     	mov	r0, #1
  3e81fc: e5840174     	str	r0, [r4, #0x174]
  3e8200: e5842184     	str	r2, [r4, #0x184]
  3e8204: e5841170     	str	r1, [r4, #0x170]
  3e8208: e5842180     	str	r2, [r4, #0x180]
  3e820c: e5843188     	str	r3, [r4, #0x188]
  3e8210: e5c4318c     	strb	r3, [r4, #0x18c]
  3e8214: 02845e17     	addeq	r5, r4, #368
  3e8218: 0a000003     	beq	0x3e822c <Door::NetStructDoor::NetStructDoor()+0x158> @ imm = #0xc
  3e821c: e2845e17     	add	r5, r4, #368
  3e8220: e5c4318d     	strb	r3, [r4, #0x18d]
  3e8224: e1a00005     	mov	r0, r5
  3e8228: eb10b355     	bl	0x814f84 <NetStructMember::SetChanged()> @ imm = #0x42cd54
  3e822c: e7973008     	ldr	r3, [r7, r8]
  3e8230: e1a01009     	mov	r1, r9
  3e8234: e1a00004     	mov	r0, r4
  3e8238: e2833008     	add	r3, r3, #8
  3e823c: e5843170     	str	r3, [r4, #0x170]
  3e8240: eb10ac01     	bl	0x81324c <NetStruct::DeclareMember(NetStructMember*)> @ imm = #0x42b004
  3e8244: e1a00004     	mov	r0, r4
  3e8248: e1a01006     	mov	r1, r6
  3e824c: eb10abfe     	bl	0x81324c <NetStruct::DeclareMember(NetStructMember*)> @ imm = #0x42aff8
  3e8250: e1a00004     	mov	r0, r4
  3e8254: e1a01005     	mov	r1, r5
  3e8258: eb10abfb     	bl	0x81324c <NetStruct::DeclareMember(NetStructMember*)> @ imm = #0x42afec
  3e825c: e1a00004     	mov	r0, r4
  3e8260: e8bd8ff8     	pop	{r3, r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3e8264: a4 c9 5a 00  	.word	0x005ac9a4
  3e8268: 20 20 00 00  	.word	0x00002020
  3e826c: 18 30 00 00  	.word	0x00003018
  3e8270: c8 0a 00 00  	.word	0x00000ac8


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003e7ffc <void PropertyMap::AddProperty<bool>(char const*, bool&, bool) (.clone.5)>:
  3e7ffc: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3e8000: e1a07000     	mov	r7, r0
  3e8004: e24dd008     	sub	sp, sp, #8
  3e8008: e1a06001     	mov	r6, r1
  3e800c: e3a00024     	mov	r0, #36
  3e8010: e3a01000     	mov	r1, #0
  3e8014: e1a08002     	mov	r8, r2
  3e8018: ebfca154     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xd7ab0
  3e801c: e59f5058     	ldr	r5, [pc, #0x58]         @ 0x3e807c <void PropertyMap::AddProperty<bool>(char const*, bool&, bool) (.clone.5)+0x80>
  3e8020: e59f3058     	ldr	r3, [pc, #0x58]         @ 0x3e8080 <void PropertyMap::AddProperty<bool>(char const*, bool&, bool) (.clone.5)+0x84>
  3e8024: e1a04000     	mov	r4, r0
  3e8028: e08f5005     	add	r5, pc, r5
  3e802c: e7953003     	ldr	r3, [r5, r3]
  3e8030: e1a01006     	mov	r1, r6
  3e8034: e28d2004     	add	r2, sp, #4
  3e8038: e2833008     	add	r3, r3, #8
  3e803c: e4803008     	str	r3, [r0], #8
  3e8040: ebfcb029     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xd3f5c
  3e8044: e59f3038     	ldr	r3, [pc, #0x38]         @ 0x3e8084 <void PropertyMap::AddProperty<bool>(char const*, bool&, bool) (.clone.5)+0x88>
  3e8048: e0678008     	rsb	r8, r7, r8
  3e804c: e3a02001     	mov	r2, #1
  3e8050: e7953003     	ldr	r3, [r5, r3]
  3e8054: e5c42020     	strb	r2, [r4, #0x20]
  3e8058: e5848004     	str	r8, [r4, #0x4]
  3e805c: e2833008     	add	r3, r3, #8
  3e8060: e5843000     	str	r3, [r4]
  3e8064: e1a00007     	mov	r0, r7
  3e8068: e1a01006     	mov	r1, r6
  3e806c: e1a02004     	mov	r2, r4
  3e8070: eb04af1b     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #0x12bc6c
  3e8074: e28dd008     	add	sp, sp, #8
  3e8078: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3e807c: 68 ca 5a 00  	.word	0x005aca68
  3e8080: 30 23 00 00  	.word	0x00002330
  3e8084: 4c 3e 00 00  	.word	0x00003e4c


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003e7da8 <Door::InitPost()>:
  3e7da8: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  3e7dac: e24dd024     	sub	sp, sp, #36
  3e7db0: e1a04000     	mov	r4, r0
  3e7db4: ebfe8fea     	bl	0x38bd64 <GameObject::CheckSpawnProbability()> @ imm = #-0x5c058
  3e7db8: e5943274     	ldr	r3, [r4, #0x274]
  3e7dbc: e59f5210     	ldr	r5, [pc, #0x210]        @ 0x3e7fd4 <Door::InitPost()+0x22c>
  3e7dc0: e1500003     	cmp	r0, r3
  3e7dc4: e08f5005     	add	r5, pc, r5
  3e7dc8: aa00005d     	bge	0x3e7f44 <Door::InitPost()+0x19c> @ imm = #0x174
  3e7dcc: e59f3204     	ldr	r3, [pc, #0x204]        @ 0x3e7fd8 <Door::InitPost()+0x230>
  3e7dd0: e594839c     	ldr	r8, [r4, #0x39c]
  3e7dd4: e7953003     	ldr	r3, [r5, r3]
  3e7dd8: e5937000     	ldr	r7, [r3]
  3e7ddc: e3570000     	cmp	r7, #0
  3e7de0: 0a000059     	beq	0x3e7f4c <Door::InitPost()+0x1a4> @ imm = #0x164
  3e7de4: e59f31f0     	ldr	r3, [pc, #0x1f0]        @ 0x3e7fdc <Door::InitPost()+0x234>
  3e7de8: e3a06000     	mov	r6, #0
  3e7dec: e7953003     	ldr	r3, [r5, r3]
  3e7df0: e593a000     	ldr	r10, [r3]
  3e7df4: ea000002     	b	0x3e7e04 <Door::InitPost()+0x5c> @ imm = #0x8
  3e7df8: e2866001     	add	r6, r6, #1
  3e7dfc: e1560007     	cmp	r6, r7
  3e7e00: 0a000051     	beq	0x3e7f4c <Door::InitPost()+0x1a4> @ imm = #0x144
  3e7e04: e79a1106     	ldr	r1, [r10, r6, lsl #2]
  3e7e08: e1a00008     	mov	r0, r8
  3e7e0c: ebfc9942     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xd9af8
  3e7e10: e3500000     	cmp	r0, #0
  3e7e14: 1afffff7     	bne	0x3e7df8 <Door::InitPost()+0x50> @ imm = #-0x24
  3e7e18: e3760001     	cmn	r6, #1
  3e7e1c: e58463a0     	str	r6, [r4, #0x3a0]
  3e7e20: 0a000013     	beq	0x3e7e74 <Door::InitPost()+0xcc> @ imm = #0x4c
  3e7e24: e59f31b4     	ldr	r3, [pc, #0x1b4]        @ 0x3e7fe0 <Door::InitPost()+0x238>
  3e7e28: e3a02018     	mov	r2, #24
  3e7e2c: e7953003     	ldr	r3, [r5, r3]
  3e7e30: e5933000     	ldr	r3, [r3]
  3e7e34: e0263692     	mla	r6, r2, r6, r3
  3e7e38: e5963014     	ldr	r3, [r6, #0x14]
  3e7e3c: e3730001     	cmn	r3, #1
  3e7e40: 0a00000b     	beq	0x3e7e74 <Door::InitPost()+0xcc> @ imm = #0x2c
  3e7e44: e59f2198     	ldr	r2, [pc, #0x198]        @ 0x3e7fe4 <Door::InitPost()+0x23c>
  3e7e48: e3a0100c     	mov	r1, #12
  3e7e4c: e7952002     	ldr	r2, [r5, r2]
  3e7e50: e5922000     	ldr	r2, [r2]
  3e7e54: e0232391     	mla	r3, r1, r3, r2
  3e7e58: e5936008     	ldr	r6, [r3, #0x8]
  3e7e5c: e1a00006     	mov	r0, r6
  3e7e60: ebfc97fb     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0xda014
  3e7e64: e1a01006     	mov	r1, r6
  3e7e68: e0862000     	add	r2, r6, r0
  3e7e6c: e2840e29     	add	r0, r4, #656
  3e7e70: ebfca2da     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0xd7498
  3e7e74: e1a00004     	mov	r0, r4
  3e7e78: ebfebe27     	bl	0x39771c <Zone::InitPost()> @ imm = #-0x50764
  3e7e7c: e1a00004     	mov	r0, r4
  3e7e80: ebfe8b36     	bl	0x38ab60 <GameObject::MeetCondition() const> @ imm = #-0x5d328
  3e7e84: e2501000     	subs	r1, r0, #0
  3e7e88: 0a000029     	beq	0x3e7f34 <Door::InitPost()+0x18c> @ imm = #0xa4
  3e7e8c: e59462d8     	ldr	r6, [r4, #0x2d8]
  3e7e90: e3560000     	cmp	r6, #0
  3e7e94: 0a00000c     	beq	0x3e7ecc <Door::InitPost()+0x124> @ imm = #0x30
  3e7e98: e59f3148     	ldr	r3, [pc, #0x148]        @ 0x3e7fe8 <Door::InitPost()+0x240>
  3e7e9c: e5962038     	ldr	r2, [r6, #0x38]
  3e7ea0: e7951003     	ldr	r1, [r5, r3]
  3e7ea4: e59f3140     	ldr	r3, [pc, #0x140]        @ 0x3e7fec <Door::InitPost()+0x244>
  3e7ea8: e1a00002     	mov	r0, r2
  3e7eac: e592c000     	ldr	r12, [r2]
  3e7eb0: e7953003     	ldr	r3, [r5, r3]
  3e7eb4: e58d4000     	str	r4, [sp]
  3e7eb8: e1a02004     	mov	r2, r4
  3e7ebc: e1a0e00f     	mov	lr, pc
  3e7ec0: e59cf02c     	ldr	pc, [r12, #0x2c]
  3e7ec4: e1a00006     	mov	r0, r6
  3e7ec8: eb0222e1     	bl	0x470a54 <VisualObject::ApplyMeshBox()> @ imm = #0x88b84
  3e7ecc: e5d433a5     	ldrb	r3, [r4, #0x3a5]
  3e7ed0: e3530000     	cmp	r3, #0
  3e7ed4: 1a00001f     	bne	0x3e7f58 <Door::InitPost()+0x1b0> @ imm = #0x7c
  3e7ed8: e59433a0     	ldr	r3, [r4, #0x3a0]
  3e7edc: e3730001     	cmn	r3, #1
  3e7ee0: 0a000017     	beq	0x3e7f44 <Door::InitPost()+0x19c> @ imm = #0x5c
  3e7ee4: e59f2104     	ldr	r2, [pc, #0x104]        @ 0x3e7ff0 <Door::InitPost()+0x248>
  3e7ee8: e7956002     	ldr	r6, [r5, r2]
  3e7eec: e5960000     	ldr	r0, [r6]
  3e7ef0: e3500000     	cmp	r0, #0
  3e7ef4: 0a000012     	beq	0x3e7f44 <Door::InitPost()+0x19c> @ imm = #0x48
  3e7ef8: e59f20e0     	ldr	r2, [pc, #0xe0]         @ 0x3e7fe0 <Door::InitPost()+0x238>
  3e7efc: e3a07018     	mov	r7, #24
  3e7f00: e7955002     	ldr	r5, [r5, r2]
  3e7f04: e5952000     	ldr	r2, [r5]
  3e7f08: e0232397     	mla	r3, r7, r3, r2
  3e7f0c: e5931010     	ldr	r1, [r3, #0x10]
  3e7f10: ebfe06b9     	bl	0x3699fc <VoxSoundManager::LoadSound(int)> @ imm = #-0x7e51c
  3e7f14: e59423a0     	ldr	r2, [r4, #0x3a0]
  3e7f18: e5953000     	ldr	r3, [r5]
  3e7f1c: e5960000     	ldr	r0, [r6]
  3e7f20: e0273297     	mla	r7, r7, r2, r3
  3e7f24: e597100c     	ldr	r1, [r7, #0xc]
  3e7f28: e28dd024     	add	sp, sp, #36
  3e7f2c: e8bd45f0     	pop	{r4, r5, r6, r7, r8, r10, lr}
  3e7f30: eafe06b1     	b	0x3699fc <VoxSoundManager::LoadSound(int)> @ imm = #-0x7e53c
  3e7f34: e1a00004     	mov	r0, r4
  3e7f38: e5943000     	ldr	r3, [r4]
  3e7f3c: e1a0e00f     	mov	lr, pc
  3e7f40: e593f040     	ldr	pc, [r3, #0x40]
  3e7f44: e28dd024     	add	sp, sp, #36
  3e7f48: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  3e7f4c: e3e03000     	mvn	r3, #0
  3e7f50: e58433a0     	str	r3, [r4, #0x3a0]
  3e7f54: eaffffc6     	b	0x3e7e74 <Door::InitPost()+0xcc> @ imm = #-0xe8
  3e7f58: e59f3094     	ldr	r3, [pc, #0x94]         @ 0x3e7ff4 <Door::InitPost()+0x24c>
  3e7f5c: e3a01000     	mov	r1, #0
  3e7f60: e3a00028     	mov	r0, #40
  3e7f64: e7953003     	ldr	r3, [r5, r3]
  3e7f68: e1a06001     	mov	r6, r1
  3e7f6c: e5938044     	ldr	r8, [r3, #0x44]
  3e7f70: ebfca17e     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xd7a08
  3e7f74: e3a0c001     	mov	r12, #1
  3e7f78: e3a0e002     	mov	lr, #2
  3e7f7c: e1a01008     	mov	r1, r8
  3e7f80: e1a0300c     	mov	r3, r12
  3e7f84: e1a02004     	mov	r2, r4
  3e7f88: e58de010     	str	lr, [sp, #0x10]
  3e7f8c: e30fefff     	movw	lr, #0xffff
  3e7f90: e1a07000     	mov	r7, r0
  3e7f94: e58de014     	str	lr, [sp, #0x14]
  3e7f98: e58d6000     	str	r6, [sp]
  3e7f9c: e58d6004     	str	r6, [sp, #0x4]
  3e7fa0: e58d6008     	str	r6, [sp, #0x8]
  3e7fa4: e58d600c     	str	r6, [sp, #0xc]
  3e7fa8: e58dc018     	str	r12, [sp, #0x18]
  3e7fac: eb021ccf     	bl	0x46f2f0 <PhysicalObject::PhysicalObject(PhysicalWorld*, GameObject*, bool, bool, bool, bool, short, unsigned short, unsigned short, int)> @ imm = #0x8733c
  3e7fb0: e59f3040     	ldr	r3, [pc, #0x40]         @ 0x3e7ff8 <Door::InitPost()+0x250>
  3e7fb4: e1a01007     	mov	r1, r7
  3e7fb8: e1a02006     	mov	r2, r6
  3e7fbc: e7953003     	ldr	r3, [r5, r3]
  3e7fc0: e1a00004     	mov	r0, r4
  3e7fc4: e2833008     	add	r3, r3, #8
  3e7fc8: e5873000     	str	r3, [r7]
  3e7fcc: ebfeb309     	bl	0x394bf8 <GameObject::SetPhysicalObject(PhysicalObject*, bool)> @ imm = #-0x533dc
  3e7fd0: eaffffc0     	b	0x3e7ed8 <Door::InitPost()+0x130> @ imm = #-0x100
  3e7fd4: cc cc 5a 00  	.word	0x005acccc
  3e7fd8: f4 12 00 00  	.word	0x000012f4
  3e7fdc: 88 44 00 00  	.word	0x00004488
  3e7fe0: 08 1e 00 00  	.word	0x00001e08
  3e7fe4: a8 1c 00 00  	.word	0x00001ca8
  3e7fe8: 48 0a 00 00  	.word	0x00000a48
  3e7fec: 84 3a 00 00  	.word	0x00003a84
  3e7ff0: a4 0d 00 00  	.word	0x00000da4
  3e7ff4: f4 37 00 00  	.word	0x000037f4
  3e7ff8: 18 24 00 00  	.word	0x00002418


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003e7c1c <Door::InitFinal()>:
  3e7c1c: e92d4010     	push	{r4, lr}
  3e7c20: e1a04000     	mov	r4, r0
  3e7c24: ebfe904e     	bl	0x38bd64 <GameObject::CheckSpawnProbability()> @ imm = #-0x5bec8
  3e7c28: e5943274     	ldr	r3, [r4, #0x274]
  3e7c2c: e1500003     	cmp	r0, r3
  3e7c30: ba000000     	blt	0x3e7c38 <Door::InitFinal()+0x1c> @ imm = #0x0
  3e7c34: e8bd8010     	pop	{r4, pc}
  3e7c38: e1a00004     	mov	r0, r4
  3e7c3c: ebfe9441     	bl	0x38cd48 <GameObject::InitFinal()> @ imm = #-0x5aefc
  3e7c40: e1a00004     	mov	r0, r4
  3e7c44: ebfe8bc5     	bl	0x38ab60 <GameObject::MeetCondition() const> @ imm = #-0x5d0ec
  3e7c48: e3500000     	cmp	r0, #0
  3e7c4c: 0afffff8     	beq	0x3e7c34 <Door::InitFinal()+0x18> @ imm = #-0x20
  3e7c50: e5d413a4     	ldrb	r1, [r4, #0x3a4]
  3e7c54: e3510000     	cmp	r1, #0
  3e7c58: 1a000002     	bne	0x3e7c68 <Door::InitFinal()+0x4c> @ imm = #0x8
  3e7c5c: e1a00004     	mov	r0, r4
  3e7c60: e8bd4010     	pop	{r4, lr}
  3e7c64: eafffe4b     	b	0x3e7598 <Door::Closed(bool)> @ imm = #-0x6d4
  3e7c68: e1a00004     	mov	r0, r4
  3e7c6c: e3a01000     	mov	r1, #0
  3e7c70: e8bd4010     	pop	{r4, lr}
  3e7c74: eafffec3     	b	0x3e7788 <Door::Opened(bool)> @ imm = #-0x4f4


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003e8658 <Door::~Door()>:
  3e8658: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  3e865c: e59f5124     	ldr	r5, [pc, #0x124]        @ 0x3e8788 <Door::~Door()+0x130>
  3e8660: e59f3124     	ldr	r3, [pc, #0x124]        @ 0x3e878c <Door::~Door()+0x134>
  3e8664: e59f6124     	ldr	r6, [pc, #0x124]        @ 0x3e8790 <Door::~Door()+0x138>
  3e8668: e59f7124     	ldr	r7, [pc, #0x124]        @ 0x3e8794 <Door::~Door()+0x13c>
  3e866c: e08f5005     	add	r5, pc, r5
  3e8670: e590c65c     	ldr	r12, [r0, #0x65c]
  3e8674: e7953003     	ldr	r3, [r5, r3]
  3e8678: e7952006     	ldr	r2, [r5, r6]
  3e867c: e7951007     	ldr	r1, [r5, r7]
  3e8680: e1a04000     	mov	r4, r0
  3e8684: e2822008     	add	r2, r2, #8
  3e8688: e28300f4     	add	r0, r3, #244
  3e868c: e35c0000     	cmp	r12, #0
  3e8690: e2811008     	add	r1, r1, #8
  3e8694: e283c008     	add	r12, r3, #8
  3e8698: e28330e8     	add	r3, r3, #232
  3e869c: e584c000     	str	r12, [r4]
  3e86a0: e5843004     	str	r3, [r4, #0x4]
  3e86a4: e5840024     	str	r0, [r4, #0x24]
  3e86a8: e5842670     	str	r2, [r4, #0x670]
  3e86ac: e5841540     	str	r1, [r4, #0x540]
  3e86b0: e58426b0     	str	r2, [r4, #0x6b0]
  3e86b4: e5842690     	str	r2, [r4, #0x690]
  3e86b8: e2848d15     	add	r8, r4, #1344
  3e86bc: 0a000008     	beq	0x3e86e4 <Door::~Door()+0x8c> @ imm = #0x20
  3e86c0: e288af43     	add	r10, r8, #268
  3e86c4: e1a0000a     	mov	r0, r10
  3e86c8: e5981110     	ldr	r1, [r8, #0x110]
  3e86cc: ebfe223f     	bl	0x370fd0 <std::priv::_Rb_tree<int, std::less<int>, std::pair<int const, std::map<int, NetStruct::tPacketHistory, std::less<int>, std::allocator<std::pair<int const, NetStruct::tPacketHistory>>>>, std::priv::_Select1st<std::pair<int const, std::map<int, NetStruct::tPacketHistory, std::less<int>, std::allocator<std::pair<int const, NetStruct::tPacketHistory>>>>>, std::priv::_MapTraitsT<std::pair<int const, std::map<int, NetStruct::tPacketHistory, std::less<int>, std::allocator<std::pair<int const, NetStruct::tPacketHistory>>>>>, std::allocator<std::pair<int const, std::map<int, NetStruct::tPacketHistory, std::less<int>, std::allocator<std::pair<int const, NetStruct::tPacketHistory>>>>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x77704
  3e86d0: e3a03000     	mov	r3, #0
  3e86d4: e584a654     	str	r10, [r4, #0x654]
  3e86d8: e5883110     	str	r3, [r8, #0x110]
  3e86dc: e584a658     	str	r10, [r4, #0x658]
  3e86e0: e584365c     	str	r3, [r4, #0x65c]
  3e86e4: e7952007     	ldr	r2, [r5, r7]
  3e86e8: e7953006     	ldr	r3, [r5, r6]
  3e86ec: e59414cc     	ldr	r1, [r4, #0x4cc]
  3e86f0: e2822008     	add	r2, r2, #8
  3e86f4: e2833008     	add	r3, r3, #8
  3e86f8: e3510000     	cmp	r1, #0
  3e86fc: e58434e0     	str	r3, [r4, #0x4e0]
  3e8700: e58423b0     	str	r2, [r4, #0x3b0]
  3e8704: e5843520     	str	r3, [r4, #0x520]
  3e8708: e5843500     	str	r3, [r4, #0x500]
  3e870c: e2845e3b     	add	r5, r4, #944
  3e8710: 0a000008     	beq	0x3e8738 <Door::~Door()+0xe0> @ imm = #0x20
  3e8714: e2856f43     	add	r6, r5, #268
  3e8718: e1a00006     	mov	r0, r6
  3e871c: e5951110     	ldr	r1, [r5, #0x110]
  3e8720: ebfe222a     	bl	0x370fd0 <std::priv::_Rb_tree<int, std::less<int>, std::pair<int const, std::map<int, NetStruct::tPacketHistory, std::less<int>, std::allocator<std::pair<int const, NetStruct::tPacketHistory>>>>, std::priv::_Select1st<std::pair<int const, std::map<int, NetStruct::tPacketHistory, std::less<int>, std::allocator<std::pair<int const, NetStruct::tPacketHistory>>>>>, std::priv::_MapTraitsT<std::pair<int const, std::map<int, NetStruct::tPacketHistory, std::less<int>, std::allocator<std::pair<int const, NetStruct::tPacketHistory>>>>>, std::allocator<std::pair<int const, std::map<int, NetStruct::tPacketHistory, std::less<int>, std::allocator<std::pair<int const, NetStruct::tPacketHistory>>>>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x77758
  3e8724: e3a03000     	mov	r3, #0
  3e8728: e58464c4     	str	r6, [r4, #0x4c4]
  3e872c: e5853110     	str	r3, [r5, #0x110]
  3e8730: e58464c8     	str	r6, [r4, #0x4c8]
  3e8734: e58434cc     	str	r3, [r4, #0x4cc]
  3e8738: e2843fe2     	add	r3, r4, #904
  3e873c: e5930014     	ldr	r0, [r3, #0x14]
  3e8740: e1500003     	cmp	r0, r3
  3e8744: 0a000006     	beq	0x3e8764 <Door::~Door()+0x10c> @ imm = #0x18
  3e8748: e3500000     	cmp	r0, #0
  3e874c: 0a000004     	beq	0x3e8764 <Door::~Door()+0x10c> @ imm = #0x10
  3e8750: e5941388     	ldr	r1, [r4, #0x388]
  3e8754: e0601001     	rsb	r1, r0, r1
  3e8758: e3510080     	cmp	r1, #128
  3e875c: 8a000004     	bhi	0x3e8774 <Door::~Door()+0x11c> @ imm = #0x10
  3e8760: eb0c81e6     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x320798
  3e8764: e1a00004     	mov	r0, r4
  3e8768: ebfebd15     	bl	0x397bc4 <Zone::~Zone()> @ imm = #-0x50bac
  3e876c: e1a00004     	mov	r0, r4
  3e8770: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  3e8774: ebfc9f31     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xd833c
  3e8778: e1a00004     	mov	r0, r4
  3e877c: ebfebd10     	bl	0x397bc4 <Zone::~Zone()> @ imm = #-0x50bc0
  3e8780: e1a00004     	mov	r0, r4
  3e8784: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  3e8788: 24 c4 5a 00  	.word	0x005ac424
  3e878c: 84 49 00 00  	.word	0x00004984
  3e8790: a8 10 00 00  	.word	0x000010a8
  3e8794: c4 43 00 00  	.word	0x000043c4


C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003e7498 <Door::IsUpdatable() const>:
  3e7498: e3a00001     	mov	r0, #1
  3e749c: e12fff1e     	bx	lr

003e74a0 <Door::IsAnimated() const>:
  3e74a0: e3a00001     	mov	r0, #1
  3e74a4: e12fff1e     	bx	lr

003e74a8 <Door::IsInteractive(GameObject*) const>:
  3e74a8: e3a00000     	mov	r0, #0
  3e74ac: e12fff1e     	bx	lr

003e74b0 <Door::IsZonable() const>:
  3e74b0: e3a00001     	mov	r0, #1
  3e74b4: e12fff1e     	bx	lr

003e74b8 <Door::getUDTypeName() const>:
  3e74b8: e59f0004     	ldr	r0, [pc, #0x4]          @ 0x3e74c4 <Door::getUDTypeName() const+0xc>
  3e74bc: e08f0000     	add	r0, pc, r0
  3e74c0: e12fff1e     	bx	lr
  3e74c4: a4 90 4d 00  	.word	0x004d90a4
