
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048f47c <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)>:
  48f47c: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  48f480: e59f34c4     	ldr	r3, [pc, #0x4c4]        @ 0x48f94c <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x4d0>
  48f484: e24dde46     	sub	sp, sp, #1120
  48f488: e24dd00c     	sub	sp, sp, #12
  48f48c: e58d3010     	str	r3, [sp, #0x10]
  48f490: e1a08000     	mov	r8, r0
  48f494: e59d0010     	ldr	r0, [sp, #0x10]
  48f498: e59f34b0     	ldr	r3, [pc, #0x4b0]        @ 0x48f950 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x4d4>
  48f49c: e1a0b001     	mov	r11, r1
  48f4a0: e08f0000     	add	r0, pc, r0
  48f4a4: e790c003     	ldr	r12, [r0, r3]
  48f4a8: e58d301c     	str	r3, [sp, #0x1c]
  48f4ac: e58d0010     	str	r0, [sp, #0x10]
  48f4b0: e59cc000     	ldr	r12, [r12]
  48f4b4: e5980044     	ldr	r0, [r8, #0x44]
  48f4b8: e3a03000     	mov	r3, #0
  48f4bc: e58d3038     	str	r3, [sp, #0x38]
  48f4c0: e58d3030     	str	r3, [sp, #0x30]
  48f4c4: e58dc464     	str	r12, [sp, #0x464]
  48f4c8: e58d3034     	str	r3, [sp, #0x34]
  48f4cc: e5903068     	ldr	r3, [r0, #0x68]
  48f4d0: e58d2018     	str	r2, [sp, #0x18]
  48f4d4: e5823000     	str	r3, [r2]
  48f4d8: e8910280     	ldm	r1, {r7, r9}
  48f4dc: e1a03009     	mov	r3, r9
  48f4e0: e1570009     	cmp	r7, r9
  48f4e4: 0a000113     	beq	0x48f938 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x4bc> @ imm = #0x44c
  48f4e8: e28d2038     	add	r2, sp, #56
  48f4ec: e28d3048     	add	r3, sp, #72
  48f4f0: e28d0090     	add	r0, sp, #144
  48f4f4: e28d10e4     	add	r1, sp, #228
  48f4f8: e2422008     	sub	r2, r2, #8
  48f4fc: e58d3020     	str	r3, [sp, #0x20]
  48f500: e58d0024     	str	r0, [sp, #0x24]
  48f504: e58d2000     	str	r2, [sp]
  48f508: e243200c     	sub	r2, r3, #12
  48f50c: e2803004     	add	r3, r0, #4
  48f510: e2810004     	add	r0, r1, #4
  48f514: e58d1014     	str	r1, [sp, #0x14]
  48f518: e58d2028     	str	r2, [sp, #0x28]
  48f51c: e58d302c     	str	r3, [sp, #0x2c]
  48f520: e58d0008     	str	r0, [sp, #0x8]
  48f524: ea00000c     	b	0x48f55c <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0xe0> @ imm = #0x30
  48f528: e3a01050     	mov	r1, #80
  48f52c: e0244291     	mla	r4, r1, r2, r4
  48f530: e5930018     	ldr	r0, [r3, #0x18]
  48f534: e5932014     	ldr	r2, [r3, #0x14]
  48f538: e5941018     	ldr	r1, [r4, #0x18]
  48f53c: e5943014     	ldr	r3, [r4, #0x14]
  48f540: e0602002     	rsb	r2, r0, r2
  48f544: e0613003     	rsb	r3, r1, r3
  48f548: e1520003     	cmp	r2, r3
  48f54c: 0a0000b3     	beq	0x48f820 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x3a4> @ imm = #0x2cc
  48f550: e2877054     	add	r7, r7, #84
  48f554: e1570009     	cmp	r7, r9
  48f558: 0a000050     	beq	0x48f6a0 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x224> @ imm = #0x140
  48f55c: e5975000     	ldr	r5, [r7]
  48f560: e3550000     	cmp	r5, #0
  48f564: 0afffff9     	beq	0x48f550 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0xd4> @ imm = #-0x1c
  48f568: e5953004     	ldr	r3, [r5, #0x4]
  48f56c: e3530000     	cmp	r3, #0
  48f570: 0afffff6     	beq	0x48f550 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0xd4> @ imm = #-0x28
  48f574: e598a044     	ldr	r10, [r8, #0x44]
  48f578: e59a1068     	ldr	r1, [r10, #0x68]
  48f57c: e3510000     	cmp	r1, #0
  48f580: 0a000070     	beq	0x48f748 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x2cc> @ imm = #0x1c0
  48f584: e59a206c     	ldr	r2, [r10, #0x6c]
  48f588: e3720001     	cmn	r2, #1
  48f58c: 0a00006a     	beq	0x48f73c <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x2c0> @ imm = #0x1a8
  48f590: e591401c     	ldr	r4, [r1, #0x1c]
  48f594: e5916020     	ldr	r6, [r1, #0x20]
  48f598: e0641006     	rsb	r1, r4, r6
  48f59c: e1a01241     	asr	r1, r1, #4
  48f5a0: e0810081     	add	r0, r1, r1, lsl #1
  48f5a4: e0800200     	add	r0, r0, r0, lsl #4
  48f5a8: e0800400     	add	r0, r0, r0, lsl #8
  48f5ac: e0800800     	add	r0, r0, r0, lsl #16
  48f5b0: e0810100     	add	r0, r1, r0, lsl #2
  48f5b4: e1520000     	cmp	r2, r0
  48f5b8: 3affffda     	blo	0x48f528 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0xac> @ imm = #-0x98
  48f5bc: e1560004     	cmp	r6, r4
  48f5c0: 0affffe2     	beq	0x48f550 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0xd4> @ imm = #-0x78
  48f5c4: e28d9f4e     	add	r9, sp, #312
  48f5c8: e28d1fdd     	add	r1, sp, #884
  48f5cc: e2892004     	add	r2, r9, #4
  48f5d0: e28dafc9     	add	r10, sp, #804
  48f5d4: e58d1004     	str	r1, [sp, #0x4]
  48f5d8: e58d200c     	str	r2, [sp, #0xc]
  48f5dc: ea000003     	b	0x48f5f0 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x174> @ imm = #0xc
  48f5e0: e2844050     	add	r4, r4, #80
  48f5e4: e1560004     	cmp	r6, r4
  48f5e8: 0a000028     	beq	0x48f690 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x214> @ imm = #0xa0
  48f5ec: e5953004     	ldr	r3, [r5, #0x4]
  48f5f0: e5932014     	ldr	r2, [r3, #0x14]
  48f5f4: e5930018     	ldr	r0, [r3, #0x18]
  48f5f8: e5941018     	ldr	r1, [r4, #0x18]
  48f5fc: e5943014     	ldr	r3, [r4, #0x14]
  48f600: e0602002     	rsb	r2, r0, r2
  48f604: e0613003     	rsb	r3, r1, r3
  48f608: e1520003     	cmp	r2, r3
  48f60c: 1afffff3     	bne	0x48f5e0 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x164> @ imm = #-0x34
  48f610: ebf9fbf2     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0x181038
  48f614: e3500000     	cmp	r0, #0
  48f618: 1afffff0     	bne	0x48f5e0 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x164> @ imm = #-0x40
  48f61c: e5981044     	ldr	r1, [r8, #0x44]
  48f620: e5912088     	ldr	r2, [r1, #0x88]
  48f624: e3520004     	cmp	r2, #4
  48f628: 0a000034     	beq	0x48f700 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x284> @ imm = #0xd0
  48f62c: e5953014     	ldr	r3, [r5, #0x14]
  48f630: e5933000     	ldr	r3, [r3]
  48f634: e1520003     	cmp	r2, r3
  48f638: 1affffe8     	bne	0x48f5e0 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x164> @ imm = #-0x60
  48f63c: e591208c     	ldr	r2, [r1, #0x8c]
  48f640: e5953018     	ldr	r3, [r5, #0x18]
  48f644: e1520003     	cmp	r2, r3
  48f648: 1affffe4     	bne	0x48f5e0 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x164> @ imm = #-0x70
  48f64c: e1a01004     	mov	r1, r4
  48f650: e59d0004     	ldr	r0, [sp, #0x4]
  48f654: ebfffb09     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x13dc
  48f658: e59d1004     	ldr	r1, [sp, #0x4]
  48f65c: e59d000c     	ldr	r0, [sp, #0xc]
  48f660: e58d5138     	str	r5, [sp, #0x138]
  48f664: ebfffb05     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x13ec
  48f668: e1a01009     	mov	r1, r9
  48f66c: e59d0000     	ldr	r0, [sp]
  48f670: ebfffe2c     	bl	0x48ef28 <std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>::push_back(std::pair<rnd::Exit const*, rnd::ListElem> const&)> @ imm = #-0x750
  48f674: e59d000c     	ldr	r0, [sp, #0xc]
  48f678: ebffe2d4     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x74b0
  48f67c: e2844050     	add	r4, r4, #80
  48f680: e59d0004     	ldr	r0, [sp, #0x4]
  48f684: ebffe2d1     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x74bc
  48f688: e1560004     	cmp	r6, r4
  48f68c: 1affffd6     	bne	0x48f5ec <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x170> @ imm = #-0xa8
  48f690: e59b9004     	ldr	r9, [r11, #0x4]
  48f694: e2877054     	add	r7, r7, #84
  48f698: e1570009     	cmp	r7, r9
  48f69c: 1affffae     	bne	0x48f55c <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0xe0> @ imm = #-0x148
  48f6a0: e59b3000     	ldr	r3, [r11]
  48f6a4: e28d0030     	add	r0, sp, #48
  48f6a8: e8900007     	ldm	r0, {r0, r1, r2}
  48f6ac: e59bc008     	ldr	r12, [r11, #0x8]
  48f6b0: e88b0007     	stm	r11, {r0, r1, r2}
  48f6b4: e59d2018     	ldr	r2, [sp, #0x18]
  48f6b8: e1a0100b     	mov	r1, r11
  48f6bc: e1a00008     	mov	r0, r8
  48f6c0: e58d3030     	str	r3, [sp, #0x30]
  48f6c4: e58dc038     	str	r12, [sp, #0x38]
  48f6c8: e58d9034     	str	r9, [sp, #0x34]
  48f6cc: ebfffbef     	bl	0x48e690 <rnd::Rule::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)> @ imm = #-0x1044
  48f6d0: e59d0000     	ldr	r0, [sp]
  48f6d4: ebfff887     	bl	0x48d8f8 <std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>::~vector()> @ imm = #-0x1de4
  48f6d8: e59d201c     	ldr	r2, [sp, #0x1c]
  48f6dc: e59d0010     	ldr	r0, [sp, #0x10]
  48f6e0: e7903002     	ldr	r3, [r0, r2]
  48f6e4: e59d2464     	ldr	r2, [sp, #0x464]
  48f6e8: e5933000     	ldr	r3, [r3]
  48f6ec: e1520003     	cmp	r2, r3
  48f6f0: 1a000094     	bne	0x48f948 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x4cc> @ imm = #0x250
  48f6f4: e28dd06c     	add	sp, sp, #108
  48f6f8: e28ddb01     	add	sp, sp, #1024
  48f6fc: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  48f700: e1a01004     	mov	r1, r4
  48f704: e1a0000a     	mov	r0, r10
  48f708: ebfffadc     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x1490
  48f70c: e1a0100a     	mov	r1, r10
  48f710: e59d0008     	ldr	r0, [sp, #0x8]
  48f714: e58d50e4     	str	r5, [sp, #0xe4]
  48f718: ebfffad8     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x14a0
  48f71c: e59d0000     	ldr	r0, [sp]
  48f720: e59d1014     	ldr	r1, [sp, #0x14]
  48f724: ebfffdff     	bl	0x48ef28 <std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>::push_back(std::pair<rnd::Exit const*, rnd::ListElem> const&)> @ imm = #-0x804
  48f728: e59d0008     	ldr	r0, [sp, #0x8]
  48f72c: ebffe2a7     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x7564
  48f730: e1a0000a     	mov	r0, r10
  48f734: ebffe2a5     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x756c
  48f738: eaffffa8     	b	0x48f5e0 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x164> @ imm = #-0x160
  48f73c: e591401c     	ldr	r4, [r1, #0x1c]
  48f740: e5916020     	ldr	r6, [r1, #0x20]
  48f744: eaffff9c     	b	0x48f5bc <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x140> @ imm = #-0x190
  48f748: e5984030     	ldr	r4, [r8, #0x30]
  48f74c: e5986034     	ldr	r6, [r8, #0x34]
  48f750: e1560004     	cmp	r6, r4
  48f754: 0affff7d     	beq	0x48f550 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0xd4> @ imm = #-0x20c
  48f758: e59d0020     	ldr	r0, [sp, #0x20]
  48f75c: e28d1fa1     	add	r1, sp, #644
  48f760: e28dafb5     	add	r10, sp, #724
  48f764: e2400008     	sub	r0, r0, #8
  48f768: e28d9f8d     	add	r9, sp, #564
  48f76c: e58d0004     	str	r0, [sp, #0x4]
  48f770: e58d100c     	str	r1, [sp, #0xc]
  48f774: ea000003     	b	0x48f788 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x30c> @ imm = #0xc
  48f778: e2844004     	add	r4, r4, #4
  48f77c: e1540006     	cmp	r4, r6
  48f780: 0affffc2     	beq	0x48f690 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x214> @ imm = #-0xf8
  48f784: e5953004     	ldr	r3, [r5, #0x4]
  48f788: e5930018     	ldr	r0, [r3, #0x18]
  48f78c: e5941000     	ldr	r1, [r4]
  48f790: ebf9fbd4     	bl	0x30e6e8 <.plt+0x974>   @ imm = #-0x1810b0
  48f794: e3500000     	cmp	r0, #0
  48f798: 1afffff6     	bne	0x48f778 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x2fc> @ imm = #-0x28
  48f79c: e1a0000a     	mov	r0, r10
  48f7a0: ebfffa36     	bl	0x48e080 <rnd::ListElem::ListElem()> @ imm = #-0x1728
  48f7a4: e5982044     	ldr	r2, [r8, #0x44]
  48f7a8: e5923088     	ldr	r3, [r2, #0x88]
  48f7ac: e3530004     	cmp	r3, #4
  48f7b0: 0a00003d     	beq	0x48f8ac <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x430> @ imm = #0xf4
  48f7b4: e5951014     	ldr	r1, [r5, #0x14]
  48f7b8: e5911000     	ldr	r1, [r1]
  48f7bc: e1530001     	cmp	r3, r1
  48f7c0: 0a000003     	beq	0x48f7d4 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x358> @ imm = #0xc
  48f7c4: e1a0000a     	mov	r0, r10
  48f7c8: ebffe280     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x7600
  48f7cc: e5986034     	ldr	r6, [r8, #0x34]
  48f7d0: eaffffe8     	b	0x48f778 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x2fc> @ imm = #-0x60
  48f7d4: e592208c     	ldr	r2, [r2, #0x8c]
  48f7d8: e5953018     	ldr	r3, [r5, #0x18]
  48f7dc: e1520003     	cmp	r2, r3
  48f7e0: 1afffff7     	bne	0x48f7c4 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x348> @ imm = #-0x24
  48f7e4: e1a0100a     	mov	r1, r10
  48f7e8: e59d000c     	ldr	r0, [sp, #0xc]
  48f7ec: ebfffaa3     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x1574
  48f7f0: e59d100c     	ldr	r1, [sp, #0xc]
  48f7f4: e59d002c     	ldr	r0, [sp, #0x2c]
  48f7f8: e58d5090     	str	r5, [sp, #0x90]
  48f7fc: ebfffa9f     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x1584
  48f800: e59d0000     	ldr	r0, [sp]
  48f804: e59d1024     	ldr	r1, [sp, #0x24]
  48f808: ebfffdc6     	bl	0x48ef28 <std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>::push_back(std::pair<rnd::Exit const*, rnd::ListElem> const&)> @ imm = #-0x8e8
  48f80c: e59d002c     	ldr	r0, [sp, #0x2c]
  48f810: ebffe26e     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x7648
  48f814: e59d000c     	ldr	r0, [sp, #0xc]
  48f818: ebffe26c     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x7650
  48f81c: eaffffe8     	b	0x48f7c4 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x348> @ imm = #-0x60
  48f820: ebf9fb6e     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0x181248
  48f824: e3500000     	cmp	r0, #0
  48f828: 1affff48     	bne	0x48f550 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0xd4> @ imm = #-0x2e0
  48f82c: e59a2088     	ldr	r2, [r10, #0x88]
  48f830: e3520004     	cmp	r2, #4
  48f834: 0a00002b     	beq	0x48f8e8 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x46c> @ imm = #0xac
  48f838: e5953014     	ldr	r3, [r5, #0x14]
  48f83c: e5933000     	ldr	r3, [r3]
  48f840: e1520003     	cmp	r2, r3
  48f844: 1affff41     	bne	0x48f550 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0xd4> @ imm = #-0x2fc
  48f848: e59a208c     	ldr	r2, [r10, #0x8c]
  48f84c: e5953018     	ldr	r3, [r5, #0x18]
  48f850: e1520003     	cmp	r2, r3
  48f854: 1affff3d     	bne	0x48f550 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0xd4> @ imm = #-0x30c
  48f858: e28dae41     	add	r10, sp, #1040
  48f85c: e28aa004     	add	r10, r10, #4
  48f860: e28d6e46     	add	r6, sp, #1120
  48f864: e1a01004     	mov	r1, r4
  48f868: e2866008     	add	r6, r6, #8
  48f86c: e28d4f79     	add	r4, sp, #484
  48f870: e1a0000a     	mov	r0, r10
  48f874: ebfffa81     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x15fc
  48f878: e5265288     	str	r5, [r6, #-0x288]!
  48f87c: e1a0100a     	mov	r1, r10
  48f880: e1a00004     	mov	r0, r4
  48f884: ebfffa7d     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x160c
  48f888: e1a01006     	mov	r1, r6
  48f88c: e59d0000     	ldr	r0, [sp]
  48f890: ebfffda4     	bl	0x48ef28 <std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>::push_back(std::pair<rnd::Exit const*, rnd::ListElem> const&)> @ imm = #-0x970
  48f894: e1a00004     	mov	r0, r4
  48f898: ebffe24c     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x76d0
  48f89c: e1a0000a     	mov	r0, r10
  48f8a0: ebffe24a     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x76d8
  48f8a4: e59b9004     	ldr	r9, [r11, #0x4]
  48f8a8: eaffff28     	b	0x48f550 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0xd4> @ imm = #-0x360
  48f8ac: e1a0100a     	mov	r1, r10
  48f8b0: e1a00009     	mov	r0, r9
  48f8b4: ebfffa71     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x163c
  48f8b8: e1a01009     	mov	r1, r9
  48f8bc: e59d0004     	ldr	r0, [sp, #0x4]
  48f8c0: e58d503c     	str	r5, [sp, #0x3c]
  48f8c4: ebfffa6d     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x164c
  48f8c8: e59d0000     	ldr	r0, [sp]
  48f8cc: e59d1028     	ldr	r1, [sp, #0x28]
  48f8d0: ebfffd94     	bl	0x48ef28 <std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>::push_back(std::pair<rnd::Exit const*, rnd::ListElem> const&)> @ imm = #-0x9b0
  48f8d4: e59d0004     	ldr	r0, [sp, #0x4]
  48f8d8: ebffe23c     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x7710
  48f8dc: e1a00009     	mov	r0, r9
  48f8e0: ebffe23a     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x7718
  48f8e4: eaffffb6     	b	0x48f7c4 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x348> @ imm = #-0x128
  48f8e8: e28d6ff1     	add	r6, sp, #964
  48f8ec: e1a01004     	mov	r1, r4
  48f8f0: e28d4e46     	add	r4, sp, #1120
  48f8f4: e2844008     	add	r4, r4, #8
  48f8f8: e1a00006     	mov	r0, r6
  48f8fc: ebfffa5f     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x1684
  48f900: e52452dc     	str	r5, [r4, #-0x2dc]!
  48f904: e2845004     	add	r5, r4, #4
  48f908: e1a01006     	mov	r1, r6
  48f90c: e1a00005     	mov	r0, r5
  48f910: ebfffa5a     	bl	0x48e280 <rnd::ListElem::ListElem(rnd::ListElem const&)> @ imm = #-0x1698
  48f914: e1a01004     	mov	r1, r4
  48f918: e59d0000     	ldr	r0, [sp]
  48f91c: ebfffd81     	bl	0x48ef28 <std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>::push_back(std::pair<rnd::Exit const*, rnd::ListElem> const&)> @ imm = #-0x9fc
  48f920: e1a00005     	mov	r0, r5
  48f924: ebffe229     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x775c
  48f928: e1a00006     	mov	r0, r6
  48f92c: ebffe227     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x7764
  48f930: e59b9004     	ldr	r9, [r11, #0x4]
  48f934: eaffff05     	b	0x48f550 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0xd4> @ imm = #-0x3ec
  48f938: e28d1038     	add	r1, sp, #56
  48f93c: e2411008     	sub	r1, r1, #8
  48f940: e58d1000     	str	r1, [sp]
  48f944: eaffff56     	b	0x48f6a4 <rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>, std::allocator<std::pair<rnd::Exit const*, rnd::ListElem>>>&, rnd::ListRule*&)+0x228> @ imm = #-0x2a8
  48f948: ebf9fa70     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x181640
  48f94c: f0 55 50 00  	.word	0x005055f0
  48f950: ac 40 00 00  	.word	0x000040ac
