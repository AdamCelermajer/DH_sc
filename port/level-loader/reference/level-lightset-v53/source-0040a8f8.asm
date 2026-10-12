
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040a8f8 <LightBase::SyncData()>:
  40a8f8: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  40a8fc: e59f5180     	ldr	r5, [pc, #0x180]        @ 0x40aa84 <LightBase::SyncData()+0x18c>
  40a900: e59f6180     	ldr	r6, [pc, #0x180]        @ 0x40aa88 <LightBase::SyncData()+0x190>
  40a904: e5901178     	ldr	r1, [r0, #0x178]
  40a908: e08f5005     	add	r5, pc, r5
  40a90c: e7953006     	ldr	r3, [r5, r6]
  40a910: e590217c     	ldr	r2, [r0, #0x17c]
  40a914: e24dd020     	sub	sp, sp, #32
  40a918: e5933000     	ldr	r3, [r3]
  40a91c: e1510002     	cmp	r1, r2
  40a920: e1a04000     	mov	r4, r0
  40a924: e58d301c     	str	r3, [sp, #0x1c]
  40a928: 0a000018     	beq	0x40a990 <LightBase::SyncData()+0x98> @ imm = #0x60
  40a92c: e59f3158     	ldr	r3, [pc, #0x158]        @ 0x40aa8c <LightBase::SyncData()+0x194>
  40a930: e28d7004     	add	r7, sp, #4
  40a934: e7958003     	ldr	r8, [r5, r3]
  40a938: e1a00008     	mov	r0, r8
  40a93c: ebfcb3d1     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xd30bc
  40a940: e59f1148     	ldr	r1, [pc, #0x148]        @ 0x40aa90 <LightBase::SyncData()+0x198>
  40a944: e1a0200d     	mov	r2, sp
  40a948: e1a00007     	mov	r0, r7
  40a94c: e08f1001     	add	r1, pc, r1
  40a950: ebfc25e5     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xf686c
  40a954: e1a00008     	mov	r0, r8
  40a958: e1a01007     	mov	r1, r7
  40a95c: ebfcb449     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xd2edc
  40a960: e1a08000     	mov	r8, r0
  40a964: e1a00007     	mov	r0, r7
  40a968: ebfc240f     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xf6fc4
  40a96c: e3580000     	cmp	r8, #0
  40a970: 1a000006     	bne	0x40a990 <LightBase::SyncData()+0x98> @ imm = #0x18
  40a974: e7953006     	ldr	r3, [r5, r6]
  40a978: e59d201c     	ldr	r2, [sp, #0x1c]
  40a97c: e5933000     	ldr	r3, [r3]
  40a980: e1520003     	cmp	r2, r3
  40a984: 1a00003d     	bne	0x40aa80 <LightBase::SyncData()+0x188> @ imm = #0xf4
  40a988: e28dd020     	add	sp, sp, #32
  40a98c: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  40a990: e5943120     	ldr	r3, [r4, #0x120]
  40a994: e30d170a     	movw	r1, #0xd70a
  40a998: e3431c23     	movt	r1, #0x3c23
  40a99c: e5937134     	ldr	r7, [r3, #0x134]
  40a9a0: e3570000     	cmp	r7, #0
  40a9a4: 15973000     	ldrne	r3, [r7]
  40a9a8: 12833001     	addne	r3, r3, #1
  40a9ac: 15873000     	strne	r3, [r7]
  40a9b0: e5948130     	ldr	r8, [r4, #0x130]
  40a9b4: e1a00008     	mov	r0, r8
  40a9b8: ebfc0e4e     	bl	0x30e2f8 <.plt+0x584>   @ imm = #-0xfc6c8
  40a9bc: e3500000     	cmp	r0, #0
  40a9c0: 15878040     	strne	r8, [r7, #0x40]
  40a9c4: e5941138     	ldr	r1, [r4, #0x138]
  40a9c8: e594213c     	ldr	r2, [r4, #0x13c]
  40a9cc: e5943134     	ldr	r3, [r4, #0x134]
  40a9d0: e5871038     	str	r1, [r7, #0x38]
  40a9d4: e587203c     	str	r2, [r7, #0x3c]
  40a9d8: e5873034     	str	r3, [r7, #0x34]
  40a9dc: e5942140     	ldr	r2, [r4, #0x140]
  40a9e0: e5940144     	ldr	r0, [r4, #0x144]
  40a9e4: e5941148     	ldr	r1, [r4, #0x148]
  40a9e8: e3a035fe     	mov	r3, #1065353216
  40a9ec: e5872004     	str	r2, [r7, #0x4]
  40a9f0: e5870008     	str	r0, [r7, #0x8]
  40a9f4: e587100c     	str	r1, [r7, #0xc]
  40a9f8: e5873010     	str	r3, [r7, #0x10]
  40a9fc: e594114c     	ldr	r1, [r4, #0x14c]
  40aa00: e594c150     	ldr	r12, [r4, #0x150]
  40aa04: e5940154     	ldr	r0, [r4, #0x154]
  40aa08: e5972000     	ldr	r2, [r7]
  40aa0c: e587c018     	str	r12, [r7, #0x18]
  40aa10: e587001c     	str	r0, [r7, #0x1c]
  40aa14: e5871014     	str	r1, [r7, #0x14]
  40aa18: e5873020     	str	r3, [r7, #0x20]
  40aa1c: e594c160     	ldr	r12, [r4, #0x160]
  40aa20: e5941158     	ldr	r1, [r4, #0x158]
  40aa24: e594015c     	ldr	r0, [r4, #0x15c]
  40aa28: e2422001     	sub	r2, r2, #1
  40aa2c: e3520000     	cmp	r2, #0
  40aa30: e5873030     	str	r3, [r7, #0x30]
  40aa34: e587c02c     	str	r12, [r7, #0x2c]
  40aa38: e5870028     	str	r0, [r7, #0x28]
  40aa3c: e5871024     	str	r1, [r7, #0x24]
  40aa40: e5872000     	str	r2, [r7]
  40aa44: 1affffca     	bne	0x40a974 <LightBase::SyncData()+0x7c> @ imm = #-0xd8
  40aa48: e5d73054     	ldrb	r3, [r7, #0x54]
  40aa4c: e3530000     	cmp	r3, #0
  40aa50: 1a000005     	bne	0x40aa6c <LightBase::SyncData()+0x174> @ imm = #0x14
  40aa54: e59f3038     	ldr	r3, [pc, #0x38]         @ 0x40aa94 <LightBase::SyncData()+0x19c>
  40aa58: e5972050     	ldr	r2, [r7, #0x50]
  40aa5c: e7953003     	ldr	r3, [r5, r3]
  40aa60: e5931000     	ldr	r1, [r3]
  40aa64: e5821000     	str	r1, [r2]
  40aa68: e5832000     	str	r2, [r3]
  40aa6c: e3a03000     	mov	r3, #0
  40aa70: e5873050     	str	r3, [r7, #0x50]
  40aa74: e1a00007     	mov	r0, r7
  40aa78: ebfc1670     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xfa640
  40aa7c: eaffffbc     	b	0x40a974 <LightBase::SyncData()+0x7c> @ imm = #-0x110
  40aa80: ebfc0e22     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xfc778
  40aa84: 88 a1 58 00  	.word	0x0058a188
  40aa88: ac 40 00 00  	.word	0x000040ac
  40aa8c: 84 08 00 00  	.word	0x00000884
  40aa90: 8c 50 4b 00  	.word	0x004b508c
  40aa94: c0 3c 00 00  	.word	0x00003cc0
