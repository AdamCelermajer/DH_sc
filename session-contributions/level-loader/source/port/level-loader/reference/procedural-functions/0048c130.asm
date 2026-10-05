
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048c130 <rnd::Path::Impl::Impl(rnd::Path const&, rnd::Rule::Impl*)>:
  48c130: e92d4070     	push	{r4, r5, r6, lr}
  48c134: e59f50cc     	ldr	r5, [pc, #0xcc]         @ 0x48c208 <rnd::Path::Impl::Impl(rnd::Path const&, rnd::Rule::Impl*)+0xd8>
  48c138: e1a04000     	mov	r4, r0
  48c13c: e1a06001     	mov	r6, r1
  48c140: ebffff5d     	bl	0x48bebc <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)> @ imm = #-0x28c
  48c144: e59f30c0     	ldr	r3, [pc, #0xc0]         @ 0x48c20c <rnd::Path::Impl::Impl(rnd::Path const&, rnd::Rule::Impl*)+0xdc>
  48c148: e08f5005     	add	r5, pc, r5
  48c14c: e5940004     	ldr	r0, [r4, #0x4]
  48c150: e7953003     	ldr	r3, [r5, r3]
  48c154: e5846044     	str	r6, [r4, #0x44]
  48c158: e2833008     	add	r3, r3, #8
  48c15c: e5843000     	str	r3, [r4]
  48c160: e590307c     	ldr	r3, [r0, #0x7c]
  48c164: e3530000     	cmp	r3, #0
  48c168: 13a05000     	movne	r5, #0
  48c16c: 1a00000b     	bne	0x48c1a0 <rnd::Path::Impl::Impl(rnd::Path const&, rnd::Rule::Impl*)+0x70> @ imm = #0x2c
  48c170: ea00001a     	b	0x48c1e0 <rnd::Path::Impl::Impl(rnd::Path const&, rnd::Rule::Impl*)+0xb0> @ imm = #0x68
  48c174: e5940004     	ldr	r0, [r4, #0x4]
  48c178: ebfffedd     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x48c
  48c17c: e5943004     	ldr	r3, [r4, #0x4]
  48c180: e5902118     	ldr	r2, [r0, #0x118]
  48c184: e593107c     	ldr	r1, [r3, #0x7c]
  48c188: e7920105     	ldr	r0, [r2, r5, lsl #2]
  48c18c: ebfffea3     	bl	0x48bc20 <rnd::RoomPool::Find(unsigned long)> @ imm = #-0x574
  48c190: e3500000     	cmp	r0, #0
  48c194: 1a00000b     	bne	0x48c1c8 <rnd::Path::Impl::Impl(rnd::Path const&, rnd::Rule::Impl*)+0x98> @ imm = #0x2c
  48c198: e5940004     	ldr	r0, [r4, #0x4]
  48c19c: e2855001     	add	r5, r5, #1
  48c1a0: ebfffed3     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x4b4
  48c1a4: e5903118     	ldr	r3, [r0, #0x118]
  48c1a8: e590211c     	ldr	r2, [r0, #0x11c]
  48c1ac: e0633002     	rsb	r3, r3, r2
  48c1b0: e1550143     	cmp	r5, r3, asr #2
  48c1b4: 3affffee     	blo	0x48c174 <rnd::Path::Impl::Impl(rnd::Path const&, rnd::Rule::Impl*)+0x44> @ imm = #-0x48
  48c1b8: e3a03004     	mov	r3, #4
  48c1bc: e5843048     	str	r3, [r4, #0x48]
  48c1c0: e1a00004     	mov	r0, r4
  48c1c4: e8bd8070     	pop	{r4, r5, r6, pc}
  48c1c8: e5903014     	ldr	r3, [r0, #0x14]
  48c1cc: e1a00004     	mov	r0, r4
  48c1d0: e584302c     	str	r3, [r4, #0x2c]
  48c1d4: e3a03004     	mov	r3, #4
  48c1d8: e5843048     	str	r3, [r4, #0x48]
  48c1dc: e8bd8070     	pop	{r4, r5, r6, pc}
  48c1e0: ebfffec3     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x4f4
  48c1e4: e5943044     	ldr	r3, [r4, #0x44]
  48c1e8: e5932084     	ldr	r2, [r3, #0x84]
  48c1ec: e5931080     	ldr	r1, [r3, #0x80]
  48c1f0: ebffde34     	bl	0x483ac8 <rnd::RandomGenerator::GetInt(int, int)> @ imm = #-0x8730
  48c1f4: e3a03004     	mov	r3, #4
  48c1f8: e584002c     	str	r0, [r4, #0x2c]
  48c1fc: e5843048     	str	r3, [r4, #0x48]
  48c200: e1a00004     	mov	r0, r4
  48c204: e8bd8070     	pop	{r4, r5, r6, pc}
  48c208: 48 89 50 00  	.word	0x00508948
  48c20c: c8 34 00 00  	.word	0x000034c8
