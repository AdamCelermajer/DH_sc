
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048c374 <rnd::RootRule::Impl::PlaceRootTile(rnd::Block*, rnd::ListElem)>:
  48c374: e92d4070     	push	{r4, r5, r6, lr}
  48c378: e1a04000     	mov	r4, r0
  48c37c: e1a06001     	mov	r6, r1
  48c380: e1a05002     	mov	r5, r2
  48c384: e5900004     	ldr	r0, [r0, #0x4]
  48c388: ebfffe59     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x69c
  48c38c: e1a01006     	mov	r1, r6
  48c390: e1a02005     	mov	r2, r5
  48c394: eb001574     	bl	0x49196c <rnd::Tile::NewRoot(rnd::RandomGenerator&, rnd::Block&, rnd::ListElem&)> @ imm = #0x55d0
  48c398: e3a01000     	mov	r1, #0
  48c39c: e1a05000     	mov	r5, r0
  48c3a0: e1a02001     	mov	r2, r1
  48c3a4: e3a03000     	mov	r3, #0
  48c3a8: eb00140a     	bl	0x4913d8 <rnd::Tile::PlaceTile(int, int, float)> @ imm = #0x5028
  48c3ac: e5943000     	ldr	r3, [r4]
  48c3b0: e1a00004     	mov	r0, r4
  48c3b4: e1a01005     	mov	r1, r5
  48c3b8: e3a02000     	mov	r2, #0
  48c3bc: e1a0e00f     	mov	lr, pc
  48c3c0: e593f010     	ldr	pc, [r3, #0x10]
  48c3c4: e2506000     	subs	r6, r0, #0
  48c3c8: 0a000003     	beq	0x48c3dc <rnd::RootRule::Impl::PlaceRootTile(rnd::Block*, rnd::ListElem)+0x68> @ imm = #0xc
  48c3cc: e1a00004     	mov	r0, r4
  48c3d0: ebfffe4c     	bl	0x48bd08 <rnd::RootRule::Impl::WriteDebuggingInfo()> @ imm = #-0x6d0
  48c3d4: e1a00005     	mov	r0, r5
  48c3d8: e8bd8070     	pop	{r4, r5, r6, pc}
  48c3dc: e1a00004     	mov	r0, r4
  48c3e0: ebfffe48     	bl	0x48bd08 <rnd::RootRule::Impl::WriteDebuggingInfo()> @ imm = #-0x6e0
  48c3e4: e1a00005     	mov	r0, r5
  48c3e8: e1a05006     	mov	r5, r6
  48c3ec: eb00152d     	bl	0x4918a8 <rnd::Tile::Unspawn()> @ imm = #0x54b4
  48c3f0: e1a00005     	mov	r0, r5
  48c3f4: e8bd8070     	pop	{r4, r5, r6, pc}
