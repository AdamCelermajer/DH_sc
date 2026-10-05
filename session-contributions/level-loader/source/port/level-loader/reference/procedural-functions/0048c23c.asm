
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048c23c <rnd::Path::Impl::Impl(rnd::Path const&, rnd::Rule::Impl*)>:
  48c23c: e92d4070     	push	{r4, r5, r6, lr}
  48c240: e59f50cc     	ldr	r5, [pc, #0xcc]         @ 0x48c314 <rnd::Path::Impl::Impl(rnd::Path const&, rnd::Rule::Impl*)+0xd8>
  48c244: e1a04000     	mov	r4, r0
  48c248: e1a06001     	mov	r6, r1
  48c24c: ebffff1a     	bl	0x48bebc <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)> @ imm = #-0x398
  48c250: e59f30c0     	ldr	r3, [pc, #0xc0]         @ 0x48c318 <rnd::Path::Impl::Impl(rnd::Path const&, rnd::Rule::Impl*)+0xdc>
  48c254: e08f5005     	add	r5, pc, r5
  48c258: e5940004     	ldr	r0, [r4, #0x4]
  48c25c: e7953003     	ldr	r3, [r5, r3]
  48c260: e5846044     	str	r6, [r4, #0x44]
  48c264: e2833008     	add	r3, r3, #8
  48c268: e5843000     	str	r3, [r4]
  48c26c: e590307c     	ldr	r3, [r0, #0x7c]
  48c270: e3530000     	cmp	r3, #0
  48c274: 13a05000     	movne	r5, #0
  48c278: 1a00000b     	bne	0x48c2ac <rnd::Path::Impl::Impl(rnd::Path const&, rnd::Rule::Impl*)+0x70> @ imm = #0x2c
  48c27c: ea00001a     	b	0x48c2ec <rnd::Path::Impl::Impl(rnd::Path const&, rnd::Rule::Impl*)+0xb0> @ imm = #0x68
  48c280: e5940004     	ldr	r0, [r4, #0x4]
  48c284: ebfffe9a     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x598
  48c288: e5943004     	ldr	r3, [r4, #0x4]
  48c28c: e5902118     	ldr	r2, [r0, #0x118]
  48c290: e593107c     	ldr	r1, [r3, #0x7c]
  48c294: e7920105     	ldr	r0, [r2, r5, lsl #2]
  48c298: ebfffe60     	bl	0x48bc20 <rnd::RoomPool::Find(unsigned long)> @ imm = #-0x680
  48c29c: e3500000     	cmp	r0, #0
  48c2a0: 1a00000b     	bne	0x48c2d4 <rnd::Path::Impl::Impl(rnd::Path const&, rnd::Rule::Impl*)+0x98> @ imm = #0x2c
  48c2a4: e5940004     	ldr	r0, [r4, #0x4]
  48c2a8: e2855001     	add	r5, r5, #1
  48c2ac: ebfffe90     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x5c0
  48c2b0: e5903118     	ldr	r3, [r0, #0x118]
  48c2b4: e590211c     	ldr	r2, [r0, #0x11c]
  48c2b8: e0633002     	rsb	r3, r3, r2
  48c2bc: e1550143     	cmp	r5, r3, asr #2
  48c2c0: 3affffee     	blo	0x48c280 <rnd::Path::Impl::Impl(rnd::Path const&, rnd::Rule::Impl*)+0x44> @ imm = #-0x48
  48c2c4: e3a03004     	mov	r3, #4
  48c2c8: e5843048     	str	r3, [r4, #0x48]
  48c2cc: e1a00004     	mov	r0, r4
  48c2d0: e8bd8070     	pop	{r4, r5, r6, pc}
  48c2d4: e5903014     	ldr	r3, [r0, #0x14]
  48c2d8: e1a00004     	mov	r0, r4
  48c2dc: e584302c     	str	r3, [r4, #0x2c]
  48c2e0: e3a03004     	mov	r3, #4
  48c2e4: e5843048     	str	r3, [r4, #0x48]
  48c2e8: e8bd8070     	pop	{r4, r5, r6, pc}
  48c2ec: ebfffe80     	bl	0x48bcf4 <rnd::Rule::GetApp() const> @ imm = #-0x600
  48c2f0: e5943044     	ldr	r3, [r4, #0x44]
  48c2f4: e5932084     	ldr	r2, [r3, #0x84]
  48c2f8: e5931080     	ldr	r1, [r3, #0x80]
  48c2fc: ebffddf1     	bl	0x483ac8 <rnd::RandomGenerator::GetInt(int, int)> @ imm = #-0x883c
  48c300: e3a03004     	mov	r3, #4
  48c304: e584002c     	str	r0, [r4, #0x2c]
  48c308: e5843048     	str	r3, [r4, #0x48]
  48c30c: e1a00004     	mov	r0, r4
  48c310: e8bd8070     	pop	{r4, r5, r6, pc}
  48c314: 3c 88 50 00  	.word	0x0050883c
  48c318: c8 34 00 00  	.word	0x000034c8
