
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00496bd8 <VisualFXManager::BuildLibraries()>:
  496bd8: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  496bdc: e59f40b0     	ldr	r4, [pc, #0xb0]         @ 0x496c94 <VisualFXManager::BuildLibraries()+0xbc>
  496be0: e59f60b0     	ldr	r6, [pc, #0xb0]         @ 0x496c98 <VisualFXManager::BuildLibraries()+0xc0>
  496be4: e59f20b0     	ldr	r2, [pc, #0xb0]         @ 0x496c9c <VisualFXManager::BuildLibraries()+0xc4>
  496be8: e08f4004     	add	r4, pc, r4
  496bec: e7943006     	ldr	r3, [r4, r6]
  496bf0: e7947002     	ldr	r7, [r4, r2]
  496bf4: e24dd020     	sub	sp, sp, #32
  496bf8: e5933000     	ldr	r3, [r3]
  496bfc: e1a08000     	mov	r8, r0
  496c00: e1a00007     	mov	r0, r7
  496c04: e58d301c     	str	r3, [sp, #0x1c]
  496c08: ebfa831e     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0x15f388
  496c0c: e59f108c     	ldr	r1, [pc, #0x8c]         @ 0x496ca0 <VisualFXManager::BuildLibraries()+0xc8>
  496c10: e28d5004     	add	r5, sp, #4
  496c14: e1a0200d     	mov	r2, sp
  496c18: e08f1001     	add	r1, pc, r1
  496c1c: e1a00005     	mov	r0, r5
  496c20: ebf9f531     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x182b3c
  496c24: e1a00007     	mov	r0, r7
  496c28: e1a01005     	mov	r1, r5
  496c2c: ebfa84a5     	bl	0x337ec8 <DebugSwitches::GetModule(std::string const&)> @ imm = #-0x15ed6c
  496c30: e1a07000     	mov	r7, r0
  496c34: e59d0018     	ldr	r0, [sp, #0x18]
  496c38: e1500005     	cmp	r0, r5
  496c3c: 0a000006     	beq	0x496c5c <VisualFXManager::BuildLibraries()+0x84> @ imm = #0x18
  496c40: e3500000     	cmp	r0, #0
  496c44: 0a000004     	beq	0x496c5c <VisualFXManager::BuildLibraries()+0x84> @ imm = #0x10
  496c48: e59d1004     	ldr	r1, [sp, #0x4]
  496c4c: e0601001     	rsb	r1, r0, r1
  496c50: e3510080     	cmp	r1, #128
  496c54: 8a00000b     	bhi	0x496c88 <VisualFXManager::BuildLibraries()+0xb0> @ imm = #0x2c
  496c58: eb09c8a8     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x2722a0
  496c5c: e3570000     	cmp	r7, #0
  496c60: 0a000001     	beq	0x496c6c <VisualFXManager::BuildLibraries()+0x94> @ imm = #0x4
  496c64: e1a00008     	mov	r0, r8
  496c68: ebffffcd     	bl	0x496ba4 <VisualFXManager::_BuildAnimLibrary()> @ imm = #-0xcc
  496c6c: e7943006     	ldr	r3, [r4, r6]
  496c70: e59d201c     	ldr	r2, [sp, #0x1c]
  496c74: e5933000     	ldr	r3, [r3]
  496c78: e1520003     	cmp	r2, r3
  496c7c: 1a000003     	bne	0x496c90 <VisualFXManager::BuildLibraries()+0xb8> @ imm = #0xc
  496c80: e28dd020     	add	sp, sp, #32
  496c84: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  496c88: ebf9e5ec     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x186850
  496c8c: eafffff2     	b	0x496c5c <VisualFXManager::BuildLibraries()+0x84> @ imm = #-0x38
  496c90: ebf9dd9e     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x188988
  496c94: a8 de 4f 00  	.word	0x004fdea8
  496c98: ac 40 00 00  	.word	0x000040ac
  496c9c: 84 08 00 00  	.word	0x00000884
  496ca0: 30 e4 43 00  	.word	0x0043e430

00496ca4 <_GLOBAL__I_.._.._sources_Game_SWFAnim_SWFAnim.cpp>:
  496ca4: e59f3014     	ldr	r3, [pc, #0x14]         @ 0x496cc0 <_GLOBAL__I_.._.._sources_Game_SWFAnim_SWFAnim.cpp+0x1c>
  496ca8: e3a0243f     	mov	r2, #1056964608
  496cac: e08f3003     	add	r3, pc, r3
  496cb0: e5832008     	str	r2, [r3, #0x8]
  496cb4: e5832000     	str	r2, [r3]
  496cb8: e5832004     	str	r2, [r3, #0x4]
  496cbc: e12fff1e     	bx	lr
  496cc0: 94 f6 50 00  	.word	0x0050f694

00496cc4 <SWFAnim::SetVisible(bool)>:
  496cc4: e92d4010     	push	{r4, lr}
  496cc8: e280000c     	add	r0, r0, #12
  496ccc: e1a04001     	mov	r4, r1
  496cd0: ebfe441e     	bl	0x427d50 <DebugCachedCharacter::GetChar()> @ imm = #-0x6ef88
  496cd4: e5c0409b     	strb	r4, [r0, #0x9b]
  496cd8: e8bd8010     	pop	{r4, pc}

00496cdc <SWFAnim::IsVisible()>:
  496cdc: e92d4010     	push	{r4, lr}
  496ce0: e280000c     	add	r0, r0, #12
  496ce4: ebfe4419     	bl	0x427d50 <DebugCachedCharacter::GetChar()> @ imm = #-0x6ef9c
  496ce8: e3a00001     	mov	r0, #1
  496cec: e8bd8010     	pop	{r4, pc}

00496cf0 <SWFAnim::IsAnimPlaying()>:
  496cf0: e92d4010     	push	{r4, lr}
  496cf4: e280000c     	add	r0, r0, #12
  496cf8: ebfe4414     	bl	0x427d50 <DebugCachedCharacter::GetChar()> @ imm = #-0x6efb0
  496cfc: e5903000     	ldr	r3, [r0]
  496d00: e1a0e00f     	mov	lr, pc
  496d04: e593f098     	ldr	pc, [r3, #0x98]
  496d08: e2500001     	subs	r0, r0, #1
  496d0c: 13a00001     	movne	r0, #1
  496d10: e8bd8010     	pop	{r4, pc}

00496d14 <SWFAnim::PlayAnim(char const*)>:
  496d14: e92d4070     	push	{r4, r5, r6, lr}
  496d18: e280400c     	add	r4, r0, #12
  496d1c: e5906008     	ldr	r6, [r0, #0x8]
  496d20: e1a00004     	mov	r0, r4
  496d24: e1a05001     	mov	r5, r1
  496d28: ebfe4408     	bl	0x427d50 <DebugCachedCharacter::GetChar()> @ imm = #-0x6efe0
  496d2c: e1a02005     	mov	r2, r5
  496d30: e1a01000     	mov	r1, r0
  496d34: e3a03000     	mov	r3, #0
  496d38: e1a00006     	mov	r0, r6
  496d3c: eb0c5330     	bl	0x7aba04 <RenderFX::PlayAnim(gameswf::character*, char const*, int)> @ imm = #0x314cc0
  496d40: e3500000     	cmp	r0, #0
  496d44: 0a000006     	beq	0x496d64 <SWFAnim::PlayAnim(char const*)+0x50> @ imm = #0x18
  496d48: e1a00004     	mov	r0, r4
  496d4c: ebfe43ff     	bl	0x427d50 <DebugCachedCharacter::GetChar()> @ imm = #-0x6f004
  496d50: e3a01000     	mov	r1, #0
  496d54: e5903000     	ldr	r3, [r0]
  496d58: e1a0e00f     	mov	lr, pc
  496d5c: e593f094     	ldr	pc, [r3, #0x94]
  496d60: e3a00001     	mov	r0, #1
  496d64: e8bd8070     	pop	{r4, r5, r6, pc}

00496d68 <SWFAnim::SetPosition(int, int)>:
  496d68: e92d4070     	push	{r4, r5, r6, lr}
  496d6c: e1a03000     	mov	r3, r0
  496d70: e280000c     	add	r0, r0, #12
  496d74: e1a05001     	mov	r5, r1
  496d78: e1a04002     	mov	r4, r2
  496d7c: e5936008     	ldr	r6, [r3, #0x8]
  496d80: ebfe43f2     	bl	0x427d50 <DebugCachedCharacter::GetChar()> @ imm = #-0x6f038
  496d84: e1a02005     	mov	r2, r5
  496d88: e1a01000     	mov	r1, r0
  496d8c: e1a03004     	mov	r3, r4
  496d90: e1a00006     	mov	r0, r6
  496d94: e8bd4070     	pop	{r4, r5, r6, lr}
  496d98: ea0c4d94     	b	0x7aa3f0 <RenderFX::SetPosition(gameswf::character*, int, int)> @ imm = #0x313650

00496d9c <SWFAnim::SetPosition3D(Point3D<float> const&)>:
  496d9c: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  496da0: e591e000     	ldr	lr, [r1]
  496da4: e591c004     	ldr	r12, [r1, #0x4]
  496da8: e5912008     	ldr	r2, [r1, #0x8]
  496dac: e24dd01c     	sub	sp, sp, #28
  496db0: e1a04000     	mov	r4, r0
  496db4: e28d1010     	add	r1, sp, #16
  496db8: e3a03000     	mov	r3, #0
  496dbc: e284500c     	add	r5, r4, #12
  496dc0: e28d0004     	add	r0, sp, #4
  496dc4: e58d200c     	str	r2, [sp, #0xc]
  496dc8: e58de004     	str	lr, [sp, #0x4]
  496dcc: e58dc008     	str	r12, [sp, #0x8]
  496dd0: e58d3014     	str	r3, [sp, #0x14]
