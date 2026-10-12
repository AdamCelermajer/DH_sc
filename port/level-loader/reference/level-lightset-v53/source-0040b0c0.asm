
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040b0c0 <LightBase::InitPost()>:
  40b0c0: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  40b0c4: e3a05443     	mov	r5, #1124073472
  40b0c8: e24dd018     	sub	sp, sp, #24
  40b0cc: e285587f     	add	r5, r5, #8323072
  40b0d0: e28d1018     	add	r1, sp, #24
  40b0d4: e1a04000     	mov	r4, r0
  40b0d8: e5215004     	str	r5, [r1, #-0x4]!
  40b0dc: e2800d05     	add	r0, r0, #320
  40b0e0: ebfd07d9     	bl	0x34d04c <Point3D<float>::operator/=(float const&)> @ imm = #-0xbe09c
  40b0e4: e28d1018     	add	r1, sp, #24
  40b0e8: e5215008     	str	r5, [r1, #-0x8]!
  40b0ec: e2840f53     	add	r0, r4, #332
  40b0f0: ebfd07d5     	bl	0x34d04c <Point3D<float>::operator/=(float const&)> @ imm = #-0xbe0ac
  40b0f4: e28d1018     	add	r1, sp, #24
  40b0f8: e521500c     	str	r5, [r1, #-0xc]!
  40b0fc: e2840f56     	add	r0, r4, #344
  40b100: ebfd07d1     	bl	0x34d04c <Point3D<float>::operator/=(float const&)> @ imm = #-0xbe0bc
  40b104: e3a01311     	mov	r1, #1140850688
  40b108: e5940138     	ldr	r0, [r4, #0x138]
  40b10c: e281187a     	add	r1, r1, #7995392
  40b110: ebfc0edf     	bl	0x30ec94 <.plt+0xf20>   @ imm = #-0xfc484
  40b114: e3021400     	movw	r1, #0x2400
  40b118: e5840138     	str	r0, [r4, #0x138]
  40b11c: e3441974     	movt	r1, #0x4974
  40b120: e594013c     	ldr	r0, [r4, #0x13c]
  40b124: ebfc0eda     	bl	0x30ec94 <.plt+0xf20>   @ imm = #-0xfc498
  40b128: e594117c     	ldr	r1, [r4, #0x17c]
  40b12c: e5943178     	ldr	r3, [r4, #0x178]
  40b130: e59f5270     	ldr	r5, [pc, #0x270]        @ 0x40b3a8 <LightBase::InitPost()+0x2e8>
  40b134: e584013c     	str	r0, [r4, #0x13c]
  40b138: e1530001     	cmp	r3, r1
  40b13c: e08f5005     	add	r5, pc, r5
  40b140: 0a000048     	beq	0x40b268 <LightBase::InitPost()+0x1a8> @ imm = #0x120
  40b144: e59f6260     	ldr	r6, [pc, #0x260]        @ 0x40b3ac <LightBase::InitPost()+0x2ec>
  40b148: e59f2260     	ldr	r2, [pc, #0x260]        @ 0x40b3b0 <LightBase::InitPost()+0x2f0>
  40b14c: e3a07000     	mov	r7, #0
  40b150: e7953006     	ldr	r3, [r5, r6]
  40b154: e08f2002     	add	r2, pc, r2
  40b158: e5930010     	ldr	r0, [r3, #0x10]
  40b15c: e1a03007     	mov	r3, r7
  40b160: e590001c     	ldr	r0, [r0, #0x1c]
  40b164: e58d7000     	str	r7, [sp]
  40b168: ebfd3962     	bl	0x3596f8 <SceneManager::LoadScene(char const*, char const*, bool, bool)> @ imm = #-0xb1a78
  40b16c: e2508000     	subs	r8, r0, #0
  40b170: 0a00001f     	beq	0x40b1f4 <LightBase::InitPost()+0x134> @ imm = #0x7c
  40b174: e1a01008     	mov	r1, r8
  40b178: e5b12188     	ldr	r2, [r1, #0x188]!
  40b17c: e1520001     	cmp	r2, r1
  40b180: 01a01007     	moveq	r1, r7
  40b184: 01a03001     	moveq	r3, r1
  40b188: 0a000006     	beq	0x40b1a8 <LightBase::InitPost()+0xe8> @ imm = #0x18
  40b18c: e1a03002     	mov	r3, r2
  40b190: e5933000     	ldr	r3, [r3]
  40b194: e1510003     	cmp	r1, r3
  40b198: 1afffffc     	bne	0x40b190 <LightBase::InitPost()+0xd0> @ imm = #-0x10
  40b19c: e5923008     	ldr	r3, [r2, #0x8]
  40b1a0: e1a02001     	mov	r2, r1
  40b1a4: e1a01003     	mov	r1, r3
  40b1a8: e5843120     	str	r3, [r4, #0x120]
  40b1ac: e5983188     	ldr	r3, [r8, #0x188]
  40b1b0: e1530002     	cmp	r3, r2
  40b1b4: 0a000002     	beq	0x40b1c4 <LightBase::InitPost()+0x104> @ imm = #0x8
  40b1b8: e5933000     	ldr	r3, [r3]
  40b1bc: e1530002     	cmp	r3, r2
  40b1c0: 1afffffc     	bne	0x40b1b8 <LightBase::InitPost()+0xf8> @ imm = #-0x10
  40b1c4: e7953006     	ldr	r3, [r5, r6]
  40b1c8: e5933010     	ldr	r3, [r3, #0x10]
  40b1cc: e593301c     	ldr	r3, [r3, #0x1c]
  40b1d0: e5933004     	ldr	r3, [r3, #0x4]
  40b1d4: e1a00003     	mov	r0, r3
  40b1d8: e5933000     	ldr	r3, [r3]
  40b1dc: e1a0e00f     	mov	lr, pc
  40b1e0: e593f05c     	ldr	pc, [r3, #0x5c]
  40b1e4: e5983000     	ldr	r3, [r8]
  40b1e8: e513000c     	ldr	r0, [r3, #-0xc]
  40b1ec: e0880000     	add	r0, r8, r0
  40b1f0: ebfc48e3     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0xedc74
  40b1f4: e5941120     	ldr	r1, [r4, #0x120]
  40b1f8: e3510000     	cmp	r1, #0
  40b1fc: 0a00001b     	beq	0x40b270 <LightBase::InitPost()+0x1b0> @ imm = #0x6c
  40b200: e5912000     	ldr	r2, [r1]
  40b204: e7953006     	ldr	r3, [r5, r6]
  40b208: e512200c     	ldr	r2, [r2, #-0xc]
  40b20c: e0811002     	add	r1, r1, r2
  40b210: e5912004     	ldr	r2, [r1, #0x4]
  40b214: e2822001     	add	r2, r2, #1
  40b218: e5812004     	str	r2, [r1, #0x4]
  40b21c: e5d42164     	ldrb	r2, [r4, #0x164]
  40b220: e5933010     	ldr	r3, [r3, #0x10]
  40b224: e3520000     	cmp	r2, #0
  40b228: e593501c     	ldr	r5, [r3, #0x1c]
  40b22c: 0a000007     	beq	0x40b250 <LightBase::InitPost()+0x190> @ imm = #0x1c
  40b230: e5958428     	ldr	r8, [r5, #0x428]
  40b234: e595342c     	ldr	r3, [r5, #0x42c]
  40b238: e1580003     	cmp	r8, r3
  40b23c: 0a000038     	beq	0x40b324 <LightBase::InitPost()+0x264> @ imm = #0xe0
  40b240: e5884000     	str	r4, [r8]
  40b244: e5953428     	ldr	r3, [r5, #0x428]
  40b248: e2833004     	add	r3, r3, #4
  40b24c: e5853428     	str	r3, [r5, #0x428]
  40b250: e1a00004     	mov	r0, r4
  40b254: e5943000     	ldr	r3, [r4]
  40b258: e1a0e00f     	mov	lr, pc
  40b25c: e593f05c     	ldr	pc, [r3, #0x5c]
  40b260: e28dd018     	add	sp, sp, #24
  40b264: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  40b268: e59f613c     	ldr	r6, [pc, #0x13c]        @ 0x40b3ac <LightBase::InitPost()+0x2ec>
  40b26c: eaffffe0     	b	0x40b1f4 <LightBase::InitPost()+0x134> @ imm = #-0x80
  40b270: e3a00f57     	mov	r0, #348
  40b274: eb04a3cc     	bl	0x5341ac <operator new(unsigned int, glitch::memory::E_MEMORY_HINT)> @ imm = #0x128f30
  40b278: e3a01001     	mov	r1, #1
  40b27c: e1a07000     	mov	r7, r0
  40b280: eb05e39a     	bl	0x5840f0 <glitch::scene::CLightSceneNode::CLightSceneNode(bool)> @ imm = #0x178e68
  40b284: e7953006     	ldr	r3, [r5, r6]
  40b288: e5847120     	str	r7, [r4, #0x120]
  40b28c: e1a01007     	mov	r1, r7
  40b290: e5933010     	ldr	r3, [r3, #0x10]
  40b294: e593301c     	ldr	r3, [r3, #0x1c]
  40b298: e5933004     	ldr	r3, [r3, #0x4]
  40b29c: e1a00003     	mov	r0, r3
  40b2a0: e5933000     	ldr	r3, [r3]
  40b2a4: e1a0e00f     	mov	lr, pc
  40b2a8: e593f05c     	ldr	pc, [r3, #0x5c]
  40b2ac: e5943120     	ldr	r3, [r4, #0x120]
  40b2b0: e5932000     	ldr	r2, [r3]
  40b2b4: e512000c     	ldr	r0, [r2, #-0xc]
  40b2b8: e0830000     	add	r0, r3, r0
  40b2bc: ebfc48b0     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0xedd40
  40b2c0: e5941120     	ldr	r1, [r4, #0x120]
  40b2c4: e3510000     	cmp	r1, #0
  40b2c8: 1affffcc     	bne	0x40b200 <LightBase::InitPost()+0x140> @ imm = #-0xd0
  40b2cc: e59f30e0     	ldr	r3, [pc, #0xe0]         @ 0x40b3b4 <LightBase::InitPost()+0x2f4>
  40b2d0: e7953003     	ldr	r3, [r5, r3]
  40b2d4: e5933000     	ldr	r3, [r3]
  40b2d8: e3530002     	cmp	r3, #2
  40b2dc: 05811000     	streq	r1, [r1]
  40b2e0: 0affffc6     	beq	0x40b200 <LightBase::InitPost()+0x140> @ imm = #-0xe8
  40b2e4: e3530001     	cmp	r3, #1
  40b2e8: 1affffc4     	bne	0x40b200 <LightBase::InitPost()+0x140> @ imm = #-0xf0
  40b2ec: e59f00c4     	ldr	r0, [pc, #0xc4]         @ 0x40b3b8 <LightBase::InitPost()+0x2f8>
  40b2f0: e59f10c4     	ldr	r1, [pc, #0xc4]         @ 0x40b3bc <LightBase::InitPost()+0x2fc>
  40b2f4: e59f20c4     	ldr	r2, [pc, #0xc4]         @ 0x40b3c0 <LightBase::InitPost()+0x300>
  40b2f8: e7950000     	ldr	r0, [r5, r0]
  40b2fc: e59f30c0     	ldr	r3, [pc, #0xc0]         @ 0x40b3c4 <LightBase::InitPost()+0x304>
  40b300: e08f1001     	add	r1, pc, r1
  40b304: e3a0c05e     	mov	r12, #94
  40b308: e28000a8     	add	r0, r0, #168
  40b30c: e08f2002     	add	r2, pc, r2
  40b310: e08f3003     	add	r3, pc, r3
  40b314: e58dc000     	str	r12, [sp]
  40b318: ebfc0b39     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xfd31c
  40b31c: e5941120     	ldr	r1, [r4, #0x120]
  40b320: eaffffb6     	b	0x40b200 <LightBase::InitPost()+0x140> @ imm = #-0x128
  40b324: e5953424     	ldr	r3, [r5, #0x424]
  40b328: e0633008     	rsb	r3, r3, r8
  40b32c: e1a03143     	asr	r3, r3, #2
  40b330: e3530001     	cmp	r3, #1
  40b334: 20837003     	addhs	r7, r3, r3
  40b338: 32837001     	addlo	r7, r3, #1
  40b33c: e3770107     	cmn	r7, #-1073741823
  40b340: 9a000010     	bls	0x40b388 <LightBase::InitPost()+0x2c8> @ imm = #0x40
  40b344: e3e07003     	mvn	r7, #3
  40b348: e3a01000     	mov	r1, #0
  40b34c: e1a00007     	mov	r0, r7
  40b350: ebfc1484     	bl	0x310568 <GlitchAlloc(unsigned int, glitch::memory::E_MEMORY_HINT)> @ imm = #-0xfadf0
  40b354: e5951424     	ldr	r1, [r5, #0x424]
  40b358: e1a06000     	mov	r6, r0
  40b35c: e0588001     	subs	r8, r8, r1
  40b360: 01a08000     	moveq	r8, r0
  40b364: 1a00000b     	bne	0x40b398 <LightBase::InitPost()+0x2d8> @ imm = #0x2c
  40b368: e4884004     	str	r4, [r8], #4
  40b36c: e5950424     	ldr	r0, [r5, #0x424]
  40b370: e0867007     	add	r7, r6, r7
  40b374: ebfc1435     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0xfaf2c
  40b378: e585742c     	str	r7, [r5, #0x42c]
  40b37c: e5858428     	str	r8, [r5, #0x428]
  40b380: e5856424     	str	r6, [r5, #0x424]
  40b384: eaffffb1     	b	0x40b250 <LightBase::InitPost()+0x190> @ imm = #-0x13c
  40b388: e1530007     	cmp	r3, r7
  40b38c: 91a07107     	lslls	r7, r7, #2
  40b390: 9affffec     	bls	0x40b348 <LightBase::InitPost()+0x288> @ imm = #-0x50
  40b394: eaffffea     	b	0x40b344 <LightBase::InitPost()+0x284> @ imm = #-0x58
  40b398: e1a02008     	mov	r2, r8
  40b39c: ebfc0ae5     	bl	0x30df38 <.plt+0x1c4>   @ imm = #-0xfd46c
  40b3a0: e0808008     	add	r8, r0, r8
  40b3a4: eaffffef     	b	0x40b368 <LightBase::InitPost()+0x2a8> @ imm = #-0x44
  40b3a8: 54 99 58 00  	.word	0x00589954
  40b3ac: f4 37 00 00  	.word	0x000037f4
  40b3b0: b4 06 4c 00  	.word	0x004c06b4
  40b3b4: c0 39 00 00  	.word	0x000039c0
  40b3b8: c0 19 00 00  	.word	0x000019c0
  40b3bc: d8 30 4b 00  	.word	0x004b30d8
  40b3c0: 74 c8 4b 00  	.word	0x004bc874
  40b3c4: a0 c8 4b 00  	.word	0x004bc8a0
