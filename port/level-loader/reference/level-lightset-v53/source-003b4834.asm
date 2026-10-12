
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003b4834 <Character::InitLightSetAndMaterial()>:
  3b4834: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  3b4838: e59f5128     	ldr	r5, [pc, #0x128]        @ 0x3b4968 <Character::InitLightSetAndMaterial()+0x134>
  3b483c: e59f7128     	ldr	r7, [pc, #0x128]        @ 0x3b496c <Character::InitLightSetAndMaterial()+0x138>
  3b4840: e59022d8     	ldr	r2, [r0, #0x2d8]
  3b4844: e08f5005     	add	r5, pc, r5
  3b4848: e7953007     	ldr	r3, [r5, r7]
  3b484c: e24dd03c     	sub	sp, sp, #60
  3b4850: e3520000     	cmp	r2, #0
  3b4854: e5933000     	ldr	r3, [r3]
  3b4858: e1a04000     	mov	r4, r0
  3b485c: e58d3034     	str	r3, [sp, #0x34]
  3b4860: 0a000036     	beq	0x3b4940 <Character::InitLightSetAndMaterial()+0x10c> @ imm = #0xd8
  3b4864: e59f2104     	ldr	r2, [pc, #0x104]        @ 0x3b4970 <Character::InitLightSetAndMaterial()+0x13c>
  3b4868: e3a03000     	mov	r3, #0
  3b486c: e58d3014     	str	r3, [sp, #0x14]
  3b4870: e7956002     	ldr	r6, [r5, r2]
  3b4874: e58d3004     	str	r3, [sp, #0x4]
  3b4878: e58d3008     	str	r3, [sp, #0x8]
  3b487c: e5962010     	ldr	r2, [r6, #0x10]
  3b4880: e58d300c     	str	r3, [sp, #0xc]
  3b4884: e58d3010     	str	r3, [sp, #0x10]
  3b4888: e592001c     	ldr	r0, [r2, #0x1c]
  3b488c: e28d8004     	add	r8, sp, #4
  3b4890: e1a01008     	mov	r1, r8
  3b4894: e2800fa5     	add	r0, r0, #660
  3b4898: e3a02001     	mov	r2, #1
  3b489c: eb0161d3     	bl	0x40cff0 <LightSetManager::InitLightFilter(std::vector<bool, std::allocator<bool>>&, bool)> @ imm = #0x5874c
  3b48a0: e5963010     	ldr	r3, [r6, #0x10]
  3b48a4: e59f10c8     	ldr	r1, [pc, #0xc8]         @ 0x3b4974 <Character::InitLightSetAndMaterial()+0x140>
  3b48a8: e28d601c     	add	r6, sp, #28
  3b48ac: e593a01c     	ldr	r10, [r3, #0x1c]
  3b48b0: e28d2018     	add	r2, sp, #24
  3b48b4: e08f1001     	add	r1, pc, r1
  3b48b8: e28aafa5     	add	r10, r10, #660
  3b48bc: e1a00006     	mov	r0, r6
  3b48c0: ebfd7e09     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xa07dc
  3b48c4: e1a01006     	mov	r1, r6
  3b48c8: e1a0000a     	mov	r0, r10
  3b48cc: eb015ebe     	bl	0x40c3cc <LightSetManager::GetLightSetIdFromName(std::string)> @ imm = #0x57af8
  3b48d0: e1a0a000     	mov	r10, r0
  3b48d4: e1a00006     	mov	r0, r6
  3b48d8: ebfd8e5d     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x9c68c
  3b48dc: e59432d8     	ldr	r3, [r4, #0x2d8]
  3b48e0: e1a01008     	mov	r1, r8
  3b48e4: e583a040     	str	r10, [r3, #0x40]
  3b48e8: e59402d8     	ldr	r0, [r4, #0x2d8]
  3b48ec: e2800044     	add	r0, r0, #68
  3b48f0: ebffff43     	bl	0x3b4604 <std::vector<bool, std::allocator<bool>>::operator=(std::vector<bool, std::allocator<bool>> const&)> @ imm = #-0x2f4
  3b48f4: e59402d8     	ldr	r0, [r4, #0x2d8]
  3b48f8: eb02f245     	bl	0x471214 <VisualObject::ApplyLightSet()> @ imm = #0xbc914
  3b48fc: e59402d8     	ldr	r0, [r4, #0x2d8]
  3b4900: eb02f0f3     	bl	0x470cd4 <VisualObject::ApplyMaterial()> @ imm = #0xbc3cc
  3b4904: e59402d8     	ldr	r0, [r4, #0x2d8]
  3b4908: eb02fe82     	bl	0x474318 <VisualObject::ApplyMaterialBaseParam()> @ imm = #0xbfa08
  3b490c: e59402d8     	ldr	r0, [r4, #0x2d8]
  3b4910: eb02f1f6     	bl	0x4710f0 <VisualObject::ApplyShadowMaterial()> @ imm = #0xbc7d8
  3b4914: e59402d8     	ldr	r0, [r4, #0x2d8]
  3b4918: eb02fd60     	bl	0x473ea0 <VisualObject::ApplyXrayMaterial()> @ imm = #0xbf580
  3b491c: e59d0004     	ldr	r0, [sp, #0x4]
  3b4920: e3500000     	cmp	r0, #0
  3b4924: 0a000005     	beq	0x3b4940 <Character::InitLightSetAndMaterial()+0x10c> @ imm = #0x14
  3b4928: e59d1014     	ldr	r1, [sp, #0x14]
  3b492c: e0601001     	rsb	r1, r0, r1
  3b4930: e3c11003     	bic	r1, r1, #3
  3b4934: e3510080     	cmp	r1, #128
  3b4938: 8a000007     	bhi	0x3b495c <Character::InitLightSetAndMaterial()+0x128> @ imm = #0x1c
  3b493c: eb0d516f     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x3545bc
  3b4940: e7953007     	ldr	r3, [r5, r7]
  3b4944: e59d2034     	ldr	r2, [sp, #0x34]
  3b4948: e5933000     	ldr	r3, [r3]
  3b494c: e1520003     	cmp	r2, r3
  3b4950: 1a000003     	bne	0x3b4964 <Character::InitLightSetAndMaterial()+0x130> @ imm = #0xc
  3b4954: e28dd03c     	add	sp, sp, #60
  3b4958: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  3b495c: ebfd6eb7     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xa4524
  3b4960: eafffff6     	b	0x3b4940 <Character::InitLightSetAndMaterial()+0x10c> @ imm = #-0x28
  3b4964: ebfd6669     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xa665c
  3b4968: 4c 02 5e 00  	.word	0x005e024c
  3b496c: ac 40 00 00  	.word	0x000040ac
  3b4970: f4 37 00 00  	.word	0x000037f4
  3b4974: 74 f5 50 00  	.word	0x0050f574
