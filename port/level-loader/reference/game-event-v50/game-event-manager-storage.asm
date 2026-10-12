
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00479968 <GameEventManager::Unload()>:
  479968: e92d4070     	push	{r4, r5, r6, lr}
  47996c: e5904000     	ldr	r4, [r0]
  479970: e5903004     	ldr	r3, [r0, #0x4]
  479974: e1a06000     	mov	r6, r0
  479978: e1540003     	cmp	r4, r3
  47997c: 0a00000d     	beq	0x4799b8 <GameEventManager::Unload()+0x50> @ imm = #0x34
  479980: e5945000     	ldr	r5, [r4]
  479984: e2844004     	add	r4, r4, #4
  479988: e3550000     	cmp	r5, #0
  47998c: 0a000004     	beq	0x4799a4 <GameEventManager::Unload()+0x3c> @ imm = #0x10
  479990: e1a00005     	mov	r0, r5
  479994: ebfffef8     	bl	0x47957c <GameEvent::~GameEvent()> @ imm = #-0x420
  479998: e1a00005     	mov	r0, r5
  47999c: ebfa5aa7     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x169564
  4799a0: e5963004     	ldr	r3, [r6, #0x4]
  4799a4: e1540003     	cmp	r4, r3
  4799a8: 1afffff4     	bne	0x479980 <GameEventManager::Unload()+0x18> @ imm = #-0x30
  4799ac: e5963000     	ldr	r3, [r6]
  4799b0: e1540003     	cmp	r4, r3
  4799b4: 15863004     	strne	r3, [r6, #0x4]
  4799b8: e8bd8070     	pop	{r4, r5, r6, pc}

004799bc <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)>:
  4799bc: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  4799c0: e590c000     	ldr	r12, [r0]
  4799c4: e1a05003     	mov	r5, r3
  4799c8: e24dd014     	sub	sp, sp, #20
  4799cc: e153000c     	cmp	r3, r12
  4799d0: e1a04000     	mov	r4, r0
  4799d4: e1a06001     	mov	r6, r1
  4799d8: e1a03002     	mov	r3, r2
  4799dc: 35907004     	ldrlo	r7, [r0, #0x4]
  4799e0: 3a00000a     	blo	0x479a10 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)+0x54> @ imm = #0x28
  4799e4: e5907004     	ldr	r7, [r0, #0x4]
  4799e8: e1550007     	cmp	r5, r7
  4799ec: 2a000007     	bhs	0x479a10 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)+0x54> @ imm = #0x1c
  4799f0: e595c000     	ldr	r12, [r5]
  4799f4: e28d3010     	add	r3, sp, #16
  4799f8: e523c008     	str	r12, [r3, #-0x8]!
  4799fc: e28dc00c     	add	r12, sp, #12
  479a00: e58dc000     	str	r12, [sp]
  479a04: ebffffec     	bl	0x4799bc <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)> @ imm = #-0x50
  479a08: e28dd014     	add	sp, sp, #20
  479a0c: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  479a10: e0662007     	rsb	r2, r6, r7
  479a14: e1a08142     	asr	r8, r2, #2
  479a18: e1530008     	cmp	r3, r8
  479a1c: 2a00001c     	bhs	0x479a94 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)+0xd8> @ imm = #0x70
  479a20: e1a08103     	lsl	r8, r3, #2
  479a24: e0683007     	rsb	r3, r8, r7
  479a28: e1530007     	cmp	r3, r7
  479a2c: 01a0a007     	moveq	r10, r7
  479a30: 0a000005     	beq	0x479a4c <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)+0x90> @ imm = #0x14
  479a34: e1a01003     	mov	r1, r3
  479a38: e0632007     	rsb	r2, r3, r7
  479a3c: e1a00007     	mov	r0, r7
  479a40: e1a0a003     	mov	r10, r3
  479a44: ebfa5387     	bl	0x30e868 <.plt+0xaf4>   @ imm = #-0x16b1e4
  479a48: e5943004     	ldr	r3, [r4, #0x4]
  479a4c: e066200a     	rsb	r2, r6, r10
  479a50: e0833008     	add	r3, r3, r8
  479a54: e3520000     	cmp	r2, #0
  479a58: e5843004     	str	r3, [r4, #0x4]
  479a5c: da000002     	ble	0x479a6c <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)+0xb0> @ imm = #0x8
  479a60: e0620007     	rsb	r0, r2, r7
  479a64: e1a01006     	mov	r1, r6
  479a68: ebfa5132     	bl	0x30df38 <.plt+0x1c4>   @ imm = #-0x16bb38
  479a6c: e1a08148     	asr	r8, r8, #2
  479a70: e3580000     	cmp	r8, #0
  479a74: daffffe3     	ble	0x479a08 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)+0x4c> @ imm = #-0x74
  479a78: e3a02000     	mov	r2, #0
  479a7c: e5951000     	ldr	r1, [r5]
  479a80: e7861102     	str	r1, [r6, r2, lsl #2]
  479a84: e2822001     	add	r2, r2, #1
  479a88: e1520008     	cmp	r2, r8
  479a8c: 1afffffa     	bne	0x479a7c <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)+0xc0> @ imm = #-0x18
  479a90: eaffffdc     	b	0x479a08 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)+0x4c> @ imm = #-0x90
  479a94: e0683003     	rsb	r3, r8, r3
  479a98: e7bda053     	sbfx	r10, r3, #0x0, #0x1e
  479a9c: e35a0000     	cmp	r10, #0
  479aa0: e0870103     	add	r0, r7, r3, lsl #2
  479aa4: da000005     	ble	0x479ac0 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)+0x104> @ imm = #0x14
  479aa8: e3a01000     	mov	r1, #0
  479aac: e595c000     	ldr	r12, [r5]
  479ab0: e787c101     	str	r12, [r7, r1, lsl #2]
  479ab4: e2811001     	add	r1, r1, #1
  479ab8: e151000a     	cmp	r1, r10
  479abc: 1afffffa     	bne	0x479aac <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)+0xf0> @ imm = #-0x18
  479ac0: e1560007     	cmp	r6, r7
  479ac4: e5840004     	str	r0, [r4, #0x4]
  479ac8: 0a000002     	beq	0x479ad8 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)+0x11c> @ imm = #0x8
  479acc: e1a01006     	mov	r1, r6
  479ad0: ebfa5364     	bl	0x30e868 <.plt+0xaf4>   @ imm = #-0x16b270
  479ad4: e5940004     	ldr	r0, [r4, #0x4]
  479ad8: e0800108     	add	r0, r0, r8, lsl #2
  479adc: e3580000     	cmp	r8, #0
  479ae0: e5840004     	str	r0, [r4, #0x4]
  479ae4: daffffc7     	ble	0x479a08 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)+0x4c> @ imm = #-0xe4
  479ae8: e3a03000     	mov	r3, #0
  479aec: e5952000     	ldr	r2, [r5]
  479af0: e7862103     	str	r2, [r6, r3, lsl #2]
  479af4: e2833001     	add	r3, r3, #1
  479af8: e1580003     	cmp	r8, r3
  479afc: 1afffffa     	bne	0x479aec <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)+0x130> @ imm = #-0x18
  479b00: eaffffc0     	b	0x479a08 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)+0x4c> @ imm = #-0x100

00479b04 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_compute_next_size(unsigned int)>:
  479b04: e92d4070     	push	{r4, r5, r6, lr}
  479b08: e8900014     	ldm	r0, {r2, r4}
  479b0c: e30f3fff     	movw	r3, #0xffff
  479b10: e3433fff     	movt	r3, #0x3fff
  479b14: e0624004     	rsb	r4, r2, r4
  479b18: e1a04144     	asr	r4, r4, #2
  479b1c: e0643003     	rsb	r3, r4, r3
  479b20: e1530001     	cmp	r3, r1
  479b24: e1a05001     	mov	r5, r1
  479b28: 3a000008     	blo	0x479b50 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_compute_next_size(unsigned int)+0x4c> @ imm = #0x20
  479b2c: e1540005     	cmp	r4, r5
  479b30: 20840004     	addhs	r0, r4, r4
  479b34: 30840005     	addlo	r0, r4, r5
  479b38: e3700107     	cmn	r0, #-1073741823
  479b3c: 8a000001     	bhi	0x479b48 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_compute_next_size(unsigned int)+0x44> @ imm = #0x4
  479b40: e1500004     	cmp	r0, r4
  479b44: 2a000000     	bhs	0x479b4c <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_compute_next_size(unsigned int)+0x48> @ imm = #0x0
  479b48: e3e00103     	mvn	r0, #-1073741824
  479b4c: e8bd8070     	pop	{r4, r5, r6, pc}
  479b50: e59f0008     	ldr	r0, [pc, #0x8]          @ 0x479b60 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_compute_next_size(unsigned int)+0x5c>
  479b54: e08f0000     	add	r0, pc, r0
  479b58: eb0a3cb8     	bl	0x708e40 <___ZSt24__stl_throw_length_errorPKc_veneer> @ imm = #0x28f2e0
  479b5c: eafffff2     	b	0x479b2c <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_compute_next_size(unsigned int)+0x28> @ imm = #-0x38
  479b60: 14 49 44 00  	.word	0x00444914

00479b64 <std::allocator<GameEvent*>::_M_allocate(unsigned int, unsigned int&)>:
  479b64: e92d4010     	push	{r4, lr}
  479b68: e3710107     	cmn	r1, #-1073741823
  479b6c: e24dd008     	sub	sp, sp, #8
  479b70: e1a04002     	mov	r4, r2
  479b74: 8a00000f     	bhi	0x479bb8 <std::allocator<GameEvent*>::_M_allocate(unsigned int, unsigned int&)+0x54> @ imm = #0x3c
  479b78: e3510000     	cmp	r1, #0
  479b7c: 01a00001     	moveq	r0, r1
  479b80: 0a000008     	beq	0x479ba8 <std::allocator<GameEvent*>::_M_allocate(unsigned int, unsigned int&)+0x44> @ imm = #0x20
  479b84: e1a00101     	lsl	r0, r1, #2
  479b88: e3500080     	cmp	r0, #128
  479b8c: e58d0004     	str	r0, [sp, #0x4]
  479b90: 8a000006     	bhi	0x479bb0 <std::allocator<GameEvent*>::_M_allocate(unsigned int, unsigned int&)+0x4c> @ imm = #0x18
  479b94: e28d0004     	add	r0, sp, #4
  479b98: eb0a3cc8     	bl	0x708ec0 <___ZNSt12__node_alloc11_M_allocateERj_veneer> @ imm = #0x28f320
  479b9c: e59d3004     	ldr	r3, [sp, #0x4]
  479ba0: e1a03123     	lsr	r3, r3, #2
  479ba4: e5843000     	str	r3, [r4]
  479ba8: e28dd008     	add	sp, sp, #8
  479bac: e8bd8010     	pop	{r4, pc}
  479bb0: ebfa5a27     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #-0x169764
  479bb4: eafffff8     	b	0x479b9c <std::allocator<GameEvent*>::_M_allocate(unsigned int, unsigned int&)+0x38> @ imm = #-0x20
  479bb8: e59f000c     	ldr	r0, [pc, #0xc]          @ 0x479bcc <std::allocator<GameEvent*>::_M_allocate(unsigned int, unsigned int&)+0x68>
  479bbc: e08f0000     	add	r0, pc, r0
  479bc0: ebfa513f     	bl	0x30e0c4 <.plt+0x350>   @ imm = #-0x16bb04
  479bc4: e3a00001     	mov	r0, #1
  479bc8: ebfa509e     	bl	0x30de48 <.plt+0xd4>    @ imm = #-0x16bd88
  479bcc: b4 48 44 00  	.word	0x004448b4

00479bd0 <std::allocator<GameEvent*>::deallocate(GameEvent**, unsigned int)>:
  479bd0: e2510000     	subs	r0, r1, #0
  479bd4: 012fff1e     	bxeq	lr
  479bd8: e1a01102     	lsl	r1, r2, #2
  479bdc: e3510080     	cmp	r1, #128
  479be0: 8a000000     	bhi	0x479be8 <std::allocator<GameEvent*>::deallocate(GameEvent**, unsigned int)+0x18> @ imm = #0x0
  479be4: ea0a3cc5     	b	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x28f314
  479be8: eafa5a14     	b	0x310440 <CustomFree(void*)> @ imm = #-0x1697b0

00479bec <GameEventManager::~GameEventManager()>:
  479bec: e92d4010     	push	{r4, lr}
  479bf0: e1a04000     	mov	r4, r0
  479bf4: ebffff5b     	bl	0x479968 <GameEventManager::Unload()> @ imm = #-0x294
  479bf8: e5943000     	ldr	r3, [r4]
  479bfc: e3530000     	cmp	r3, #0
  479c00: 0a000005     	beq	0x479c1c <GameEventManager::~GameEventManager()+0x30> @ imm = #0x14
  479c04: e5942008     	ldr	r2, [r4, #0x8]
  479c08: e1a01003     	mov	r1, r3
  479c0c: e2840008     	add	r0, r4, #8
  479c10: e0633002     	rsb	r3, r3, r2
  479c14: e1a02143     	asr	r2, r3, #2
  479c18: ebffffec     	bl	0x479bd0 <std::allocator<GameEvent*>::deallocate(GameEvent**, unsigned int)> @ imm = #-0x50
  479c1c: e1a00004     	mov	r0, r4
  479c20: e8bd8010     	pop	{r4, pc}

00479c24 <_GLOBAL__I_.._.._sources_Game_Progression_GameEventManager.cpp>:
  479c24: e92d4070     	push	{r4, r5, r6, lr}
  479c28: e59f4064     	ldr	r4, [pc, #0x64]         @ 0x479c94 <_GLOBAL__I_.._.._sources_Game_Progression_GameEventManager.cpp+0x70>
  479c2c: e59f2064     	ldr	r2, [pc, #0x64]         @ 0x479c98 <_GLOBAL__I_.._.._sources_Game_Progression_GameEventManager.cpp+0x74>
  479c30: e59f3064     	ldr	r3, [pc, #0x64]         @ 0x479c9c <_GLOBAL__I_.._.._sources_Game_Progression_GameEventManager.cpp+0x78>
  479c34: e08f4004     	add	r4, pc, r4
  479c38: e7941002     	ldr	r1, [r4, r2]
  479c3c: e08f3003     	add	r3, pc, r3
  479c40: e3a0243f     	mov	r2, #1056964608
  479c44: e5910000     	ldr	r0, [r1]
  479c48: e5832008     	str	r2, [r3, #0x8]
  479c4c: e5832000     	str	r2, [r3]
  479c50: e3100001     	tst	r0, #1
  479c54: e5832004     	str	r2, [r3, #0x4]
  479c58: 0a000000     	beq	0x479c60 <_GLOBAL__I_.._.._sources_Game_Progression_GameEventManager.cpp+0x3c> @ imm = #0x0
  479c5c: e8bd8070     	pop	{r4, r5, r6, pc}
  479c60: e3a03001     	mov	r3, #1
  479c64: e5813000     	str	r3, [r1]
  479c68: e59f3030     	ldr	r3, [pc, #0x30]         @ 0x479ca0 <_GLOBAL__I_.._.._sources_Game_Progression_GameEventManager.cpp+0x7c>
  479c6c: e7945003     	ldr	r5, [r4, r3]
  479c70: e1a00005     	mov	r0, r5
  479c74: ebfacec8     	bl	0x32d79c <Application::Application()> @ imm = #-0x14c4e0
  479c78: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x479ca4 <_GLOBAL__I_.._.._sources_Game_Progression_GameEventManager.cpp+0x80>
  479c7c: e1a00005     	mov	r0, r5
  479c80: e7941003     	ldr	r1, [r4, r3]
  479c84: e59f301c     	ldr	r3, [pc, #0x1c]         @ 0x479ca8 <_GLOBAL__I_.._.._sources_Game_Progression_GameEventManager.cpp+0x84>
  479c88: e7942003     	ldr	r2, [r4, r3]
  479c8c: e8bd4070     	pop	{r4, r5, r6, lr}
  479c90: eafa519b     	b	0x30e304 <.plt+0x590>   @ imm = #-0x16b994
  479c94: 5c ae 51 00  	.word	0x0051ae5c
  479c98: ac 0f 00 00  	.word	0x00000fac
  479c9c: 60 c5 52 00  	.word	0x0052c560
  479ca0: f4 37 00 00  	.word	0x000037f4
  479ca4: c0 08 00 00  	.word	0x000008c0
  479ca8: 90 18 00 00  	.word	0x00001890

00479cac <std::vector<GameEvent*, std::allocator<GameEvent*>>::vector(unsigned int) (.clone.1)>:
  479cac: e92d4010     	push	{r4, lr}
  479cb0: e24dd008     	sub	sp, sp, #8
  479cb4: e1a04000     	mov	r4, r0
  479cb8: e3a01000     	mov	r1, #0
  479cbc: e28d2008     	add	r2, sp, #8
  479cc0: e5221004     	str	r1, [r2, #-0x4]!
  479cc4: e5841000     	str	r1, [r4]
  479cc8: e5841004     	str	r1, [r4, #0x4]
  479ccc: e5a01008     	str	r1, [r0, #0x8]!
  479cd0: ebffffa3     	bl	0x479b64 <std::allocator<GameEvent*>::_M_allocate(unsigned int, unsigned int&)> @ imm = #-0x174
  479cd4: e59d3004     	ldr	r3, [sp, #0x4]
  479cd8: e5840000     	str	r0, [r4]
  479cdc: e5840004     	str	r0, [r4, #0x4]
  479ce0: e0800103     	add	r0, r0, r3, lsl #2
  479ce4: e5840008     	str	r0, [r4, #0x8]
  479ce8: e1a00004     	mov	r0, r4
  479cec: e28dd008     	add	sp, sp, #8
  479cf0: e8bd8010     	pop	{r4, pc}

00479cf4 <GameEventManager::GameEventManager()>:
  479cf4: e92d4010     	push	{r4, lr}
  479cf8: e1a04000     	mov	r4, r0
  479cfc: ebffffea     	bl	0x479cac <std::vector<GameEvent*, std::allocator<GameEvent*>>::vector(unsigned int) (.clone.1)> @ imm = #-0x58
  479d00: e1a00004     	mov	r0, r4
  479d04: e8bd8010     	pop	{r4, pc}

00479d08 <GameEventManager::GameEventManager()>:
  479d08: e92d4010     	push	{r4, lr}
  479d0c: e1a04000     	mov	r4, r0
  479d10: ebffffe5     	bl	0x479cac <std::vector<GameEvent*, std::allocator<GameEvent*>>::vector(unsigned int) (.clone.1)> @ imm = #-0x6c
  479d14: e1a00004     	mov	r0, r4
  479d18: e8bd8010     	pop	{r4, pc}

00479d1c <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert(GameEvent**, unsigned int, GameEvent* const&)>:
  479d1c: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  479d20: e2526000     	subs	r6, r2, #0
  479d24: e24dd010     	sub	sp, sp, #16
  479d28: e1a04000     	mov	r4, r0
  479d2c: e1a07001     	mov	r7, r1
  479d30: e1a05003     	mov	r5, r3
  479d34: 0a000027     	beq	0x479dd8 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert(GameEvent**, unsigned int, GameEvent* const&)+0xbc> @ imm = #0x9c
  479d38: e9905000     	ldmib	r0, {r12, lr}
  479d3c: e06cc00e     	rsb	r12, r12, lr
  479d40: e156014c     	cmp	r6, r12, asr #2
  479d44: 9a000025     	bls	0x479de0 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert(GameEvent**, unsigned int, GameEvent* const&)+0xc4> @ imm = #0x94
  479d48: e1a01006     	mov	r1, r6
  479d4c: ebffff6c     	bl	0x479b04 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_compute_next_size(unsigned int)> @ imm = #-0x250
  479d50: e2849008     	add	r9, r4, #8
  479d54: e28d2010     	add	r2, sp, #16
  479d58: e1a01000     	mov	r1, r0
  479d5c: e5220008     	str	r0, [r2, #-0x8]!
  479d60: e1a00009     	mov	r0, r9
  479d64: ebffff7e     	bl	0x479b64 <std::allocator<GameEvent*>::_M_allocate(unsigned int, unsigned int&)> @ imm = #-0x208
  479d68: e5941000     	ldr	r1, [r4]
  479d6c: e1a08000     	mov	r8, r0
  479d70: e057a001     	subs	r10, r7, r1
  479d74: 01a00000     	moveq	r0, r0
  479d78: 1a00001c     	bne	0x479df0 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert(GameEvent**, unsigned int, GameEvent* const&)+0xd4> @ imm = #0x70
  479d7c: e1a02006     	mov	r2, r6
  479d80: e3a03000     	mov	r3, #0
  479d84: e5951000     	ldr	r1, [r5]
  479d88: e2522001     	subs	r2, r2, #1
  479d8c: e7801003     	str	r1, [r0, r3]
  479d90: e2833004     	add	r3, r3, #4
  479d94: 1afffffa     	bne	0x479d84 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert(GameEvent**, unsigned int, GameEvent* const&)+0x68> @ imm = #-0x18
  479d98: e5945004     	ldr	r5, [r4, #0x4]
  479d9c: e0806106     	add	r6, r0, r6, lsl #2
  479da0: e0555007     	subs	r5, r5, r7
  479da4: 1a000015     	bne	0x479e00 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert(GameEvent**, unsigned int, GameEvent* const&)+0xe4> @ imm = #0x54
  479da8: e5943000     	ldr	r3, [r4]
  479dac: e5942008     	ldr	r2, [r4, #0x8]
  479db0: e1a00009     	mov	r0, r9
  479db4: e1a01003     	mov	r1, r3
  479db8: e0633002     	rsb	r3, r3, r2
  479dbc: e1a02143     	asr	r2, r3, #2
  479dc0: ebffff82     	bl	0x479bd0 <std::allocator<GameEvent*>::deallocate(GameEvent**, unsigned int)> @ imm = #-0x1f8
  479dc4: e59d3008     	ldr	r3, [sp, #0x8]
  479dc8: e5848000     	str	r8, [r4]
  479dcc: e5846004     	str	r6, [r4, #0x4]
  479dd0: e0888103     	add	r8, r8, r3, lsl #2
  479dd4: e5848008     	str	r8, [r4, #0x8]
  479dd8: e28dd010     	add	sp, sp, #16
  479ddc: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  479de0: e28dc00c     	add	r12, sp, #12
  479de4: e58dc000     	str	r12, [sp]
  479de8: ebfffef3     	bl	0x4799bc <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert_aux(GameEvent**, unsigned int, GameEvent* const&, std::__false_type const&)> @ imm = #-0x434
  479dec: eafffff9     	b	0x479dd8 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert(GameEvent**, unsigned int, GameEvent* const&)+0xbc> @ imm = #-0x1c
  479df0: e1a0200a     	mov	r2, r10
  479df4: ebfa504f     	bl	0x30df38 <.plt+0x1c4>   @ imm = #-0x16bec4
  479df8: e080000a     	add	r0, r0, r10
  479dfc: eaffffde     	b	0x479d7c <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert(GameEvent**, unsigned int, GameEvent* const&)+0x60> @ imm = #-0x88
  479e00: e1a00006     	mov	r0, r6
  479e04: e1a01007     	mov	r1, r7
  479e08: e1a02005     	mov	r2, r5
  479e0c: ebfa5049     	bl	0x30df38 <.plt+0x1c4>   @ imm = #-0x16bedc
  479e10: e0806005     	add	r6, r0, r5
  479e14: eaffffe3     	b	0x479da8 <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert(GameEvent**, unsigned int, GameEvent* const&)+0x8c> @ imm = #-0x74

00479e18 <std::vector<GameEvent*, std::allocator<GameEvent*>>::resize(unsigned int, GameEvent* const&)>:
  479e18: e92d0030     	push	{r4, r5}
  479e1c: e5904004     	ldr	r4, [r0, #0x4]
  479e20: e5905000     	ldr	r5, [r0]
  479e24: e1a03002     	mov	r3, r2
  479e28: e0652004     	rsb	r2, r5, r4
  479e2c: e1a02142     	asr	r2, r2, #2
  479e30: e1510002     	cmp	r1, r2
  479e34: 2a000004     	bhs	0x479e4c <std::vector<GameEvent*, std::allocator<GameEvent*>>::resize(unsigned int, GameEvent* const&)+0x34> @ imm = #0x10
  479e38: e0855101     	add	r5, r5, r1, lsl #2
  479e3c: e1550004     	cmp	r5, r4
  479e40: 15805004     	strne	r5, [r0, #0x4]
  479e44: e8bd0030     	pop	{r4, r5}
  479e48: e12fff1e     	bx	lr
  479e4c: e0622001     	rsb	r2, r2, r1
  479e50: e1a01004     	mov	r1, r4
  479e54: e8bd0030     	pop	{r4, r5}
  479e58: eaffffaf     	b	0x479d1c <std::vector<GameEvent*, std::allocator<GameEvent*>>::_M_fill_insert(GameEvent**, unsigned int, GameEvent* const&)> @ imm = #-0x144

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
