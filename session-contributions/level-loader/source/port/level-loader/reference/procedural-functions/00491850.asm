
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00491850 <rnd::Tile::Tile(rnd::RandomGenerator&, rnd::Block&, rnd::Tile*, rnd::ListElem&)>:
  491850: e92d4070     	push	{r4, r5, r6, lr}
  491854: e2805034     	add	r5, r0, #52
  491858: e1a04000     	mov	r4, r0
  49185c: e980000a     	stmib	r0, {r1, r3}
  491860: e5802030     	str	r2, [r0, #0x30]
  491864: e1a00005     	mov	r0, r5
  491868: ebfff204     	bl	0x48e080 <rnd::ListElem::ListElem()> @ imm = #-0x37f0
  49186c: e5940008     	ldr	r0, [r4, #0x8]
  491870: e3a03000     	mov	r3, #0
  491874: e584300c     	str	r3, [r4, #0xc]
  491878: e1500003     	cmp	r0, r3
  49187c: 0a000001     	beq	0x491888 <rnd::Tile::Tile(rnd::RandomGenerator&, rnd::Block&, rnd::Tile*, rnd::ListElem&)+0x38> @ imm = #0x4
  491880: e1a01004     	mov	r1, r4
  491884: ebfffee7     	bl	0x491428 <rnd::Tile::AddChild(rnd::Tile*)> @ imm = #-0x464
  491888: e5943030     	ldr	r3, [r4, #0x30]
  49188c: e59d1010     	ldr	r1, [sp, #0x10]
  491890: e1a00005     	mov	r0, r5
  491894: e5933018     	ldr	r3, [r3, #0x18]
  491898: e5843000     	str	r3, [r4]
  49189c: ebffea06     	bl	0x48c0bc <rnd::ListElem::operator=(rnd::ListElem const&)> @ imm = #-0x57e8
  4918a0: e1a00004     	mov	r0, r4
  4918a4: e8bd8070     	pop	{r4, r5, r6, pc}
