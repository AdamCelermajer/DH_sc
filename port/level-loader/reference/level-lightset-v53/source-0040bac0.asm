
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040bac0 <LightPoint::RefreshAttachment()>:
  40bac0: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  40bac4: e59f51f4     	ldr	r5, [pc, #0x1f4]        @ 0x40bcc0 <LightPoint::RefreshAttachment()+0x200>
  40bac8: e59f81f4     	ldr	r8, [pc, #0x1f4]        @ 0x40bcc4 <LightPoint::RefreshAttachment()+0x204>
  40bacc: e59f61f4     	ldr	r6, [pc, #0x1f4]        @ 0x40bcc8 <LightPoint::RefreshAttachment()+0x208>
  40bad0: e08f5005     	add	r5, pc, r5
  40bad4: e7952008     	ldr	r2, [r5, r8]
  40bad8: e7953006     	ldr	r3, [r5, r6]
  40badc: e1a04000     	mov	r4, r0
  40bae0: e5922000     	ldr	r2, [r2]
  40bae4: e59f01e0     	ldr	r0, [pc, #0x1e0]        @ 0x40bccc <LightPoint::RefreshAttachment()+0x20c>
  40bae8: e24dd060     	sub	sp, sp, #96
  40baec: e5933010     	ldr	r3, [r3, #0x10]
  40baf0: e58d205c     	str	r2, [sp, #0x5c]
  40baf4: e08f0000     	add	r0, pc, r0
  40baf8: e5941044     	ldr	r1, [r4, #0x44]
  40bafc: e3a02012     	mov	r2, #18
  40bb00: e593901c     	ldr	r9, [r3, #0x1c]
  40bb04: ebfc0c5c     	bl	0x30ec7c <.plt+0xf08>   @ imm = #-0xfce90
  40bb08: e3500000     	cmp	r0, #0
  40bb0c: 1a000021     	bne	0x40bb98 <LightPoint::RefreshAttachment()+0xd8> @ imm = #0x84
  40bb10: e59f11b8     	ldr	r1, [pc, #0x1b8]        @ 0x40bcd0 <LightPoint::RefreshAttachment()+0x210>
  40bb14: e28d7044     	add	r7, sp, #68
  40bb18: e284ad06     	add	r10, r4, #384
  40bb1c: e08f1001     	add	r1, pc, r1
  40bb20: e1a00007     	mov	r0, r7
  40bb24: e28d2028     	add	r2, sp, #40
  40bb28: ebfc216f     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xf7a44
  40bb2c: e15a0007     	cmp	r10, r7
  40bb30: 0a000003     	beq	0x40bb44 <LightPoint::RefreshAttachment()+0x84> @ imm = #0xc
  40bb34: e1a0000a     	mov	r0, r10
  40bb38: e59d1058     	ldr	r1, [sp, #0x58]
  40bb3c: e59d2054     	ldr	r2, [sp, #0x54]
  40bb40: ebfc13a6     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0xfb168
  40bb44: e1a00007     	mov	r0, r7
  40bb48: ebfc1f97     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xf81a4
  40bb4c: e7953006     	ldr	r3, [r5, r6]
  40bb50: e3a01000     	mov	r1, #0
  40bb54: e3a02001     	mov	r2, #1
  40bb58: e5930040     	ldr	r0, [r3, #0x40]
  40bb5c: ebfd8a45     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x9d6ec
  40bb60: e5903660     	ldr	r3, [r0, #0x660]
  40bb64: e3530000     	cmp	r3, #0
  40bb68: 0a000006     	beq	0x40bb88 <LightPoint::RefreshAttachment()+0xc8> @ imm = #0x18
  40bb6c: e5937044     	ldr	r7, [r3, #0x44]
  40bb70: e1a00007     	mov	r0, r7
  40bb74: ebfc08b6     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0xfdd28
  40bb78: e1a01007     	mov	r1, r7
  40bb7c: e0872000     	add	r2, r7, r0
  40bb80: e1a0000a     	mov	r0, r10
  40bb84: ebfc1395     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0xfb1ac
  40bb88: e3a01000     	mov	r1, #0
  40bb8c: e1a00004     	mov	r0, r4
  40bb90: e1a02001     	mov	r2, r1
  40bb94: ebffff23     	bl	0x40b828 <LightPoint::AssignTweaker(int, int)> @ imm = #-0x374
  40bb98: e5942194     	ldr	r2, [r4, #0x194]
  40bb9c: e5943190     	ldr	r3, [r4, #0x190]
  40bba0: e1530002     	cmp	r3, r2
  40bba4: 0a000019     	beq	0x40bc10 <LightPoint::RefreshAttachment()+0x150> @ imm = #0x64
  40bba8: e7951006     	ldr	r1, [r5, r6]
  40bbac: e28d6018     	add	r6, sp, #24
  40bbb0: e5943064     	ldr	r3, [r4, #0x64]
  40bbb4: e5911038     	ldr	r1, [r1, #0x38]
  40bbb8: e3a07000     	mov	r7, #0
  40bbbc: e1a00006     	mov	r0, r6
  40bbc0: e58d7000     	str	r7, [sp]
  40bbc4: e58d7004     	str	r7, [sp, #0x4]
  40bbc8: ebfcfc34     	bl	0x34aca0 <ObjectManager::GetObjectByName(char const*, int, bool, char const*)> @ imm = #-0xc0f30
  40bbcc: e1a00006     	mov	r0, r6
  40bbd0: e1a01007     	mov	r1, r7
  40bbd4: ebfcd079     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xcbe1c
  40bbd8: e1500007     	cmp	r0, r7
  40bbdc: 0a000005     	beq	0x40bbf8 <LightPoint::RefreshAttachment()+0x138> @ imm = #0x14
  40bbe0: e59d201c     	ldr	r2, [sp, #0x1c]
  40bbe4: e5963008     	ldr	r3, [r6, #0x8]
  40bbe8: e59d1018     	ldr	r1, [sp, #0x18]
  40bbec: e584219c     	str	r2, [r4, #0x19c]
  40bbf0: e58431a0     	str	r3, [r4, #0x1a0]
  40bbf4: e5841198     	str	r1, [r4, #0x198]
  40bbf8: e5d431b0     	ldrb	r3, [r4, #0x1b0]
  40bbfc: e3530000     	cmp	r3, #0
  40bc00: 0a000002     	beq	0x40bc10 <LightPoint::RefreshAttachment()+0x150> @ imm = #0x8
  40bc04: e5d431b1     	ldrb	r3, [r4, #0x1b1]
  40bc08: e3530000     	cmp	r3, #0
  40bc0c: 0a000006     	beq	0x40bc2c <LightPoint::RefreshAttachment()+0x16c> @ imm = #0x18
  40bc10: e7953008     	ldr	r3, [r5, r8]
  40bc14: e59d205c     	ldr	r2, [sp, #0x5c]
  40bc18: e5933000     	ldr	r3, [r3]
  40bc1c: e1520003     	cmp	r2, r3
  40bc20: 1a000025     	bne	0x40bcbc <LightPoint::RefreshAttachment()+0x1fc> @ imm = #0x94
  40bc24: e28dd060     	add	sp, sp, #96
  40bc28: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  40bc2c: e59f30a0     	ldr	r3, [pc, #0xa0]         @ 0x40bcd4 <LightPoint::RefreshAttachment()+0x214>
  40bc30: e28d702c     	add	r7, sp, #44
  40bc34: e795a003     	ldr	r10, [r5, r3]
  40bc38: e1a0000a     	mov	r0, r10
  40bc3c: ebfcaf11     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xd43bc
  40bc40: e59f1090     	ldr	r1, [pc, #0x90]         @ 0x40bcd8 <LightPoint::RefreshAttachment()+0x218>
  40bc44: e28d2024     	add	r2, sp, #36
  40bc48: e1a00007     	mov	r0, r7
  40bc4c: e08f1001     	add	r1, pc, r1
  40bc50: ebfc2125     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xf7b6c
  40bc54: e1a0000a     	mov	r0, r10
  40bc58: e1a01007     	mov	r1, r7
  40bc5c: ebfcaf89     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xd41dc
  40bc60: e1a0a000     	mov	r10, r0
  40bc64: e1a00007     	mov	r0, r7
  40bc68: ebfc1f4f     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xf82c4
  40bc6c: e35a0000     	cmp	r10, #0
  40bc70: 1affffe6     	bne	0x40bc10 <LightPoint::RefreshAttachment()+0x150> @ imm = #-0x68
  40bc74: e594c120     	ldr	r12, [r4, #0x120]
  40bc78: e896000e     	ldm	r6, {r1, r2, r3}
  40bc7c: e2890fa5     	add	r0, r9, #660
  40bc80: e58dc000     	str	r12, [sp]
  40bc84: e28d600c     	add	r6, sp, #12
  40bc88: eb000252     	bl	0x40c5d8 <LightSetManager::AddActiveLight(ObjectHandle, glitch::scene::CLightSceneNode*)> @ imm = #0x948
  40bc8c: e3a03001     	mov	r3, #1
  40bc90: e5c431b1     	strb	r3, [r4, #0x1b1]
  40bc94: e1a0100a     	mov	r1, r10
  40bc98: e1a00006     	mov	r0, r6
  40bc9c: ebfcce20     	bl	0x33f524 <ObjectHandle::ObjectHandle(ObjectBase*)> @ imm = #-0xcc780
  40bca0: e5961008     	ldr	r1, [r6, #0x8]
  40bca4: e59d200c     	ldr	r2, [sp, #0xc]
  40bca8: e59d3010     	ldr	r3, [sp, #0x10]
  40bcac: e58411a0     	str	r1, [r4, #0x1a0]
  40bcb0: e5842198     	str	r2, [r4, #0x198]
  40bcb4: e584319c     	str	r3, [r4, #0x19c]
  40bcb8: eaffffd4     	b	0x40bc10 <LightPoint::RefreshAttachment()+0x150> @ imm = #-0xb0
  40bcbc: ebfc0993     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xfd9b4
  40bcc0: c0 8f 58 00  	.word	0x00588fc0
  40bcc4: ac 40 00 00  	.word	0x000040ac
  40bcc8: f4 37 00 00  	.word	0x000037f4
  40bccc: 04 c1 4b 00  	.word	0x004bc104
  40bcd0: f4 c0 4b 00  	.word	0x004bc0f4
  40bcd4: 84 08 00 00  	.word	0x00000884
  40bcd8: 74 3d 4b 00  	.word	0x004b3d74
