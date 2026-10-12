
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00479e5c <GameEventManager::Load()>:
  479e5c: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  479e60: e8900408     	ldm	r0, {r3, r10}
  479e64: e59f40b8     	ldr	r4, [pc, #0xb8]         @ 0x479f24 <GameEventManager::Load()+0xc8>
  479e68: e24dd014     	sub	sp, sp, #20
  479e6c: e063a00a     	rsb	r10, r3, r10
  479e70: e1b0a14a     	asrs	r10, r10, #2
  479e74: e1a05000     	mov	r5, r0
  479e78: e08f4004     	add	r4, pc, r4
  479e7c: 0a000001     	beq	0x479e88 <GameEventManager::Load()+0x2c> @ imm = #0x4
  479e80: e28dd014     	add	sp, sp, #20
  479e84: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  479e88: e59f3098     	ldr	r3, [pc, #0x98]         @ 0x479f28 <GameEventManager::Load()+0xcc>
  479e8c: e28d2010     	add	r2, sp, #16
  479e90: e522a004     	str	r10, [r2, #-0x4]!
  479e94: e7943003     	ldr	r3, [r4, r3]
  479e98: e5936000     	ldr	r6, [r3]
  479e9c: e1a01006     	mov	r1, r6
  479ea0: ebffffdc     	bl	0x479e18 <std::vector<GameEvent*, std::allocator<GameEvent*>>::resize(unsigned int, GameEvent* const&)> @ imm = #-0x90
  479ea4: e3560000     	cmp	r6, #0
  479ea8: 0afffff4     	beq	0x479e80 <GameEventManager::Load()+0x24> @ imm = #-0x30
  479eac: e59f3078     	ldr	r3, [pc, #0x78]         @ 0x479f2c <GameEventManager::Load()+0xd0>
  479eb0: e59f2078     	ldr	r2, [pc, #0x78]         @ 0x479f30 <GameEventManager::Load()+0xd4>
  479eb4: e1a0700a     	mov	r7, r10
  479eb8: e794b003     	ldr	r11, [r4, r3]
  479ebc: e58d2004     	str	r2, [sp, #0x4]
  479ec0: e3a01000     	mov	r1, #0
  479ec4: e3a00020     	mov	r0, #32
  479ec8: e59b9000     	ldr	r9, [r11]
  479ecc: ebfa59a7     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x169964
  479ed0: e1a08000     	mov	r8, r0
  479ed4: ebfffdb4     	bl	0x4795ac <GameEvent::GameEvent()> @ imm = #-0x930
  479ed8: e59d2004     	ldr	r2, [sp, #0x4]
  479edc: e089900a     	add	r9, r9, r10
  479ee0: e1a01009     	mov	r1, r9
  479ee4: e7943002     	ldr	r3, [r4, r2]
  479ee8: e1a00008     	mov	r0, r8
  479eec: e28aa018     	add	r10, r10, #24
  479ef0: e5933000     	ldr	r3, [r3]
  479ef4: e7933107     	ldr	r3, [r3, r7, lsl #2]
  479ef8: e5887004     	str	r7, [r8, #0x4]
  479efc: e5883008     	str	r3, [r8, #0x8]
  479f00: ebfffd98     	bl	0x479568 <GameEvent::AssignPyData(Structs::v2Event*)> @ imm = #-0x9a0
  479f04: e1a00008     	mov	r0, r8
  479f08: ebfffd8a     	bl	0x479538 <GameEvent::ReInit()> @ imm = #-0x9d8
  479f0c: e5953000     	ldr	r3, [r5]
  479f10: e7838107     	str	r8, [r3, r7, lsl #2]
  479f14: e2877001     	add	r7, r7, #1
  479f18: e1570006     	cmp	r7, r6
  479f1c: 1affffe7     	bne	0x479ec0 <GameEventManager::Load()+0x64> @ imm = #-0x64
  479f20: eaffffd6     	b	0x479e80 <GameEventManager::Load()+0x24> @ imm = #-0xa8
  479f24: 18 ac 51 00  	.word	0x0051ac18
  479f28: 60 08 00 00  	.word	0x00000860
  479f2c: 98 1c 00 00  	.word	0x00001c98
  479f30: d0 37 00 00  	.word	0x000037d0

00479f34 <GameEventManager::~GameEventManager()>:
  479f34: e92d4010     	push	{r4, lr}
  479f38: e1a04000     	mov	r4, r0
  479f3c: ebfffe89     	bl	0x479968 <GameEventManager::Unload()> @ imm = #-0x5dc
  479f40: e5943000     	ldr	r3, [r4]
  479f44: e3530000     	cmp	r3, #0
  479f48: 0a000005     	beq	0x479f64 <GameEventManager::~GameEventManager()+0x30> @ imm = #0x14
  479f4c: e5942008     	ldr	r2, [r4, #0x8]
  479f50: e1a01003     	mov	r1, r3
  479f54: e2840008     	add	r0, r4, #8
  479f58: e0633002     	rsb	r3, r3, r2
  479f5c: e1a02143     	asr	r2, r3, #2
  479f60: ebffff1a     	bl	0x479bd0 <std::allocator<GameEvent*>::deallocate(GameEvent**, unsigned int)> @ imm = #-0x398
  479f64: e1a00004     	mov	r0, r4
  479f68: e8bd8010     	pop	{r4, pc}

00479f6c <Objective::Compile()>:
  479f6c: e12fff1e     	bx	lr

00479f70 <Objective::InvalidateCompile()>:
  479f70: e3a03000     	mov	r3, #0
  479f74: e5c03014     	strb	r3, [r0, #0x14]
  479f78: e5c03008     	strb	r3, [r0, #0x8]
  479f7c: e12fff1e     	bx	lr

00479f80 <Objective::InstallObjectiveMarker(int, int)>:
  479f80: e12fff1e     	bx	lr

00479f84 <Objective::RemoveObjectiveMarker()>:
  479f84: e12fff1e     	bx	lr

00479f88 <Vector3DFList::operator[](int)>:
  479f88: e5903000     	ldr	r3, [r0]
  479f8c: e3a0000c     	mov	r0, #12
  479f90: e0203190     	mla	r0, r0, r1, r3
  479f94: e12fff1e     	bx	lr

00479f98 <Vector3DFList::getCount() const>:
  479f98: e590000c     	ldr	r0, [r0, #0xc]
  479f9c: e12fff1e     	bx	lr

00479fa0 <Vector3DFList::setCharacterPosition(glitch::core::vector3d<float> const&)>:
  479fa0: e5913000     	ldr	r3, [r1]
  479fa4: e5803014     	str	r3, [r0, #0x14]
  479fa8: e591c004     	ldr	r12, [r1, #0x4]
  479fac: e580c018     	str	r12, [r0, #0x18]
  479fb0: e5912008     	ldr	r2, [r1, #0x8]
  479fb4: e580303c     	str	r3, [r0, #0x3c]
  479fb8: e580c040     	str	r12, [r0, #0x40]
  479fbc: e5802044     	str	r2, [r0, #0x44]
  479fc0: e580201c     	str	r2, [r0, #0x1c]
  479fc4: e12fff1e     	bx	lr

00479fc8 <Vector3DFList::addClosest(glitch::core::vector3d<float> const&)>:
  479fc8: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  479fcc: e1a09001     	mov	r9, r1
  479fd0: e5911000     	ldr	r1, [r1]
  479fd4: e24dd00c     	sub	sp, sp, #12
  479fd8: e1a07000     	mov	r7, r0
  479fdc: e58d1000     	str	r1, [sp]
  479fe0: e5901014     	ldr	r1, [r0, #0x14]
  479fe4: e59d0000     	ldr	r0, [sp]
  479fe8: ebfa50ef     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x16bc44
  479fec: e5971018     	ldr	r1, [r7, #0x18]
  479ff0: e1a06000     	mov	r6, r0
  479ff4: e5990004     	ldr	r0, [r9, #0x4]
  479ff8: ebfa50eb     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x16bc54
  479ffc: e597101c     	ldr	r1, [r7, #0x1c]
  47a000: e1a05000     	mov	r5, r0
  47a004: e5990008     	ldr	r0, [r9, #0x8]
  47a008: ebfa50e7     	bl	0x30e3ac <.plt+0x638>   @ imm = #-0x16bc64
  47a00c: e597300c     	ldr	r3, [r7, #0xc]
  47a010: e5978008     	ldr	r8, [r7, #0x8]
  47a014: e1a04000     	mov	r4, r0
  47a018: e1530008     	cmp	r3, r8
  47a01c: ba000034     	blt	0x47a0f4 <Vector3DFList::addClosest(glitch::core::vector3d<float> const&)+0x12c> @ imm = #0xd0
  47a020: e3580000     	cmp	r8, #0
  47a024: da000030     	ble	0x47a0ec <Vector3DFList::addClosest(glitch::core::vector3d<float> const&)+0x124> @ imm = #0xc0
  47a028: e1a01006     	mov	r1, r6
  47a02c: e1a00006     	mov	r0, r6
  47a030: ebfa534d     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x16b2cc
  47a034: e1a01005     	mov	r1, r5
  47a038: e1a06000     	mov	r6, r0
  47a03c: e1a00005     	mov	r0, r5
  47a040: ebfa5349     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x16b2dc
  47a044: e1a01000     	mov	r1, r0
  47a048: e1a00006     	mov	r0, r6
  47a04c: ebfa52d4     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x16b4b0
  47a050: e1a01004     	mov	r1, r4
  47a054: e1a05000     	mov	r5, r0
  47a058: e1a00004     	mov	r0, r4
  47a05c: ebfa5342     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x16b2f8
  47a060: e1a01000     	mov	r1, r0
  47a064: e1a00005     	mov	r0, r5
  47a068: ebfa52cd     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x16b4cc
  47a06c: e58d0004     	str	r0, [sp, #0x4]
  47a070: e597a004     	ldr	r10, [r7, #0x4]
  47a074: e1a06000     	mov	r6, r0
  47a078: e3a04000     	mov	r4, #0
  47a07c: e3e0b000     	mvn	r11, #0
  47a080: ea000000     	b	0x47a088 <Vector3DFList::addClosest(glitch::core::vector3d<float> const&)+0xc0> @ imm = #0x0
  47a084: e1a06005     	mov	r6, r5
  47a088: e79a5104     	ldr	r5, [r10, r4, lsl #2]
  47a08c: e1a01006     	mov	r1, r6
  47a090: e1a00005     	mov	r0, r5
  47a094: ebfa5097     	bl	0x30e2f8 <.plt+0x584>   @ imm = #-0x16bda4
  47a098: e3500000     	cmp	r0, #0
  47a09c: 11a0b004     	movne	r11, r4
  47a0a0: e2844001     	add	r4, r4, #1
  47a0a4: 01a05006     	moveq	r5, r6
  47a0a8: e1540008     	cmp	r4, r8
  47a0ac: 1afffff4     	bne	0x47a084 <Vector3DFList::addClosest(glitch::core::vector3d<float> const&)+0xbc> @ imm = #-0x30
  47a0b0: e37b0001     	cmn	r11, #1
  47a0b4: 0a00000c     	beq	0x47a0ec <Vector3DFList::addClosest(glitch::core::vector3d<float> const&)+0x124> @ imm = #0x30
  47a0b8: e3a0300c     	mov	r3, #12
  47a0bc: e5972000     	ldr	r2, [r7]
  47a0c0: e59d1000     	ldr	r1, [sp]
  47a0c4: e0030b93     	mul	r3, r3, r11
  47a0c8: e7821003     	str	r1, [r2, r3]
  47a0cc: e5991004     	ldr	r1, [r9, #0x4]
  47a0d0: e0823003     	add	r3, r2, r3
  47a0d4: e5831004     	str	r1, [r3, #0x4]
  47a0d8: e5992008     	ldr	r2, [r9, #0x8]
  47a0dc: e5832008     	str	r2, [r3, #0x8]
  47a0e0: e5973004     	ldr	r3, [r7, #0x4]
  47a0e4: e59d2004     	ldr	r2, [sp, #0x4]
  47a0e8: e783210b     	str	r2, [r3, r11, lsl #2]
  47a0ec: e28dd00c     	add	sp, sp, #12
  47a0f0: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  47a0f4: e3a0200c     	mov	r2, #12
  47a0f8: e59dc000     	ldr	r12, [sp]
  47a0fc: e0030392     	mul	r3, r2, r3
  47a100: e5972000     	ldr	r2, [r7]
  47a104: e1a01006     	mov	r1, r6
  47a108: e1a00006     	mov	r0, r6
  47a10c: e782c003     	str	r12, [r2, r3]
  47a110: e599c004     	ldr	r12, [r9, #0x4]
  47a114: e0823003     	add	r3, r2, r3
  47a118: e583c004     	str	r12, [r3, #0x4]
  47a11c: e5992008     	ldr	r2, [r9, #0x8]
  47a120: e5832008     	str	r2, [r3, #0x8]
  47a124: ebfa5310     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x16b3c0
  47a128: e1a01005     	mov	r1, r5
  47a12c: e1a06000     	mov	r6, r0
  47a130: e1a00005     	mov	r0, r5
  47a134: ebfa530c     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x16b3d0
  47a138: e1a01000     	mov	r1, r0
  47a13c: e1a00006     	mov	r0, r6
  47a140: ebfa5297     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x16b5a4
  47a144: e1a01004     	mov	r1, r4
  47a148: e1a05000     	mov	r5, r0
  47a14c: e1a00004     	mov	r0, r4
  47a150: ebfa5305     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x16b3ec
  47a154: e1a01000     	mov	r1, r0
  47a158: e1a00005     	mov	r0, r5
  47a15c: ebfa5290     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x16b5c0
  47a160: e597300c     	ldr	r3, [r7, #0xc]
  47a164: e5971004     	ldr	r1, [r7, #0x4]
  47a168: e2832001     	add	r2, r3, #1
  47a16c: e7810103     	str	r0, [r1, r3, lsl #2]
  47a170: e587200c     	str	r2, [r7, #0xc]
  47a174: eaffffdc     	b	0x47a0ec <Vector3DFList::addClosest(glitch::core::vector3d<float> const&)+0x124> @ imm = #-0x90

0047a178 <Vector3DFList::add(glitch::core::vector3d<float> const&)>:
  47a178: e590300c     	ldr	r3, [r0, #0xc]
  47a17c: e5902008     	ldr	r2, [r0, #0x8]
