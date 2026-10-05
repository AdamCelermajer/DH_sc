
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d0b4 <rnd::RootRule::Impl::Impl(rnd::RootRule const&)>:
  48d0b4: e92d40f0     	push	{r4, r5, r6, r7, lr}
  48d0b8: e3a02000     	mov	r2, #0
  48d0bc: e24dd00c     	sub	sp, sp, #12
  48d0c0: e59f60ac     	ldr	r6, [pc, #0xac]         @ 0x48d174 <rnd::RootRule::Impl::Impl(rnd::RootRule const&)+0xc0>
  48d0c4: e1a04000     	mov	r4, r0
  48d0c8: e1a05001     	mov	r5, r1
  48d0cc: ebfffb7a     	bl	0x48bebc <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)> @ imm = #-0x1218
  48d0d0: e59f30a0     	ldr	r3, [pc, #0xa0]         @ 0x48d178 <rnd::RootRule::Impl::Impl(rnd::RootRule const&)+0xc4>
  48d0d4: e08f6006     	add	r6, pc, r6
  48d0d8: e3a02000     	mov	r2, #0
  48d0dc: e7963003     	ldr	r3, [r6, r3]
  48d0e0: e584202c     	str	r2, [r4, #0x2c]
  48d0e4: e5845044     	str	r5, [r4, #0x44]
  48d0e8: e2833008     	add	r3, r3, #8
  48d0ec: e5843000     	str	r3, [r4]
  48d0f0: e5953074     	ldr	r3, [r5, #0x74]
  48d0f4: e5955070     	ldr	r5, [r5, #0x70]
  48d0f8: e1530005     	cmp	r3, r5
  48d0fc: 0a000019     	beq	0x48d168 <rnd::RootRule::Impl::Impl(rnd::RootRule const&)+0xb4> @ imm = #0x64
  48d100: e2847030     	add	r7, r4, #48
  48d104: e28d6004     	add	r6, sp, #4
  48d108: ea000008     	b	0x48d130 <rnd::RootRule::Impl::Impl(rnd::RootRule const&)+0x7c> @ imm = #0x20
  48d10c: e5813000     	str	r3, [r1]
  48d110: e5943034     	ldr	r3, [r4, #0x34]
  48d114: e2855018     	add	r5, r5, #24
  48d118: e2833004     	add	r3, r3, #4
  48d11c: e5843034     	str	r3, [r4, #0x34]
  48d120: e5943044     	ldr	r3, [r4, #0x44]
  48d124: e5933074     	ldr	r3, [r3, #0x74]
  48d128: e1550003     	cmp	r5, r3
  48d12c: 0a00000d     	beq	0x48d168 <rnd::RootRule::Impl::Impl(rnd::RootRule const&)+0xb4> @ imm = #0x34
  48d130: e5941034     	ldr	r1, [r4, #0x34]
  48d134: e5942038     	ldr	r2, [r4, #0x38]
  48d138: e5953014     	ldr	r3, [r5, #0x14]
  48d13c: e1510002     	cmp	r1, r2
  48d140: e58d3004     	str	r3, [sp, #0x4]
  48d144: 1afffff0     	bne	0x48d10c <rnd::RootRule::Impl::Impl(rnd::RootRule const&)+0x58> @ imm = #-0x40
  48d148: e1a00007     	mov	r0, r7
  48d14c: e1a02006     	mov	r2, r6
  48d150: ebffff0b     	bl	0x48cd84 <std::vector<char const*, std::allocator<char const*>>::_M_insert_overflow(char const**, char const* const&, std::__true_type const&, unsigned int, bool) (.clone.15)> @ imm = #-0x3d4
  48d154: e5943044     	ldr	r3, [r4, #0x44]
  48d158: e2855018     	add	r5, r5, #24
  48d15c: e5933074     	ldr	r3, [r3, #0x74]
  48d160: e1550003     	cmp	r5, r3
  48d164: 1afffff1     	bne	0x48d130 <rnd::RootRule::Impl::Impl(rnd::RootRule const&)+0x7c> @ imm = #-0x3c
  48d168: e1a00004     	mov	r0, r4
  48d16c: e28dd00c     	add	sp, sp, #12
  48d170: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  48d174: bc 79 50 00  	.word	0x005079bc
  48d178: ac 07 00 00  	.word	0x000007ac
