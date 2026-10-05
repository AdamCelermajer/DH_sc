
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048ce48 <rnd::RootRule::Impl::Impl(rnd::RootRule const&)>:
  48ce48: e92d40f0     	push	{r4, r5, r6, r7, lr}
  48ce4c: e3a02000     	mov	r2, #0
  48ce50: e24dd00c     	sub	sp, sp, #12
  48ce54: e59f60ac     	ldr	r6, [pc, #0xac]         @ 0x48cf08 <rnd::RootRule::Impl::Impl(rnd::RootRule const&)+0xc0>
  48ce58: e1a04000     	mov	r4, r0
  48ce5c: e1a05001     	mov	r5, r1
  48ce60: ebfffc15     	bl	0x48bebc <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)> @ imm = #-0xfac
  48ce64: e59f30a0     	ldr	r3, [pc, #0xa0]         @ 0x48cf0c <rnd::RootRule::Impl::Impl(rnd::RootRule const&)+0xc4>
  48ce68: e08f6006     	add	r6, pc, r6
  48ce6c: e3a02000     	mov	r2, #0
  48ce70: e7963003     	ldr	r3, [r6, r3]
  48ce74: e584202c     	str	r2, [r4, #0x2c]
  48ce78: e5845044     	str	r5, [r4, #0x44]
  48ce7c: e2833008     	add	r3, r3, #8
  48ce80: e5843000     	str	r3, [r4]
  48ce84: e5953074     	ldr	r3, [r5, #0x74]
  48ce88: e5955070     	ldr	r5, [r5, #0x70]
  48ce8c: e1530005     	cmp	r3, r5
  48ce90: 0a000019     	beq	0x48cefc <rnd::RootRule::Impl::Impl(rnd::RootRule const&)+0xb4> @ imm = #0x64
  48ce94: e2847030     	add	r7, r4, #48
  48ce98: e28d6004     	add	r6, sp, #4
  48ce9c: ea000008     	b	0x48cec4 <rnd::RootRule::Impl::Impl(rnd::RootRule const&)+0x7c> @ imm = #0x20
  48cea0: e5813000     	str	r3, [r1]
  48cea4: e5943034     	ldr	r3, [r4, #0x34]
  48cea8: e2855018     	add	r5, r5, #24
  48ceac: e2833004     	add	r3, r3, #4
  48ceb0: e5843034     	str	r3, [r4, #0x34]
  48ceb4: e5943044     	ldr	r3, [r4, #0x44]
  48ceb8: e5933074     	ldr	r3, [r3, #0x74]
  48cebc: e1550003     	cmp	r5, r3
  48cec0: 0a00000d     	beq	0x48cefc <rnd::RootRule::Impl::Impl(rnd::RootRule const&)+0xb4> @ imm = #0x34
  48cec4: e5941034     	ldr	r1, [r4, #0x34]
  48cec8: e5942038     	ldr	r2, [r4, #0x38]
  48cecc: e5953014     	ldr	r3, [r5, #0x14]
  48ced0: e1510002     	cmp	r1, r2
  48ced4: e58d3004     	str	r3, [sp, #0x4]
  48ced8: 1afffff0     	bne	0x48cea0 <rnd::RootRule::Impl::Impl(rnd::RootRule const&)+0x58> @ imm = #-0x40
  48cedc: e1a00007     	mov	r0, r7
  48cee0: e1a02006     	mov	r2, r6
  48cee4: ebffffa6     	bl	0x48cd84 <std::vector<char const*, std::allocator<char const*>>::_M_insert_overflow(char const**, char const* const&, std::__true_type const&, unsigned int, bool) (.clone.15)> @ imm = #-0x168
  48cee8: e5943044     	ldr	r3, [r4, #0x44]
  48ceec: e2855018     	add	r5, r5, #24
  48cef0: e5933074     	ldr	r3, [r3, #0x74]
  48cef4: e1550003     	cmp	r5, r3
  48cef8: 1afffff1     	bne	0x48cec4 <rnd::RootRule::Impl::Impl(rnd::RootRule const&)+0x7c> @ imm = #-0x3c
  48cefc: e1a00004     	mov	r0, r4
  48cf00: e28dd00c     	add	sp, sp, #12
  48cf04: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  48cf08: 28 7c 50 00  	.word	0x00507c28
  48cf0c: ac 07 00 00  	.word	0x000007ac
