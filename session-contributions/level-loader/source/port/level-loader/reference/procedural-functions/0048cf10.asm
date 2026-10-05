
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048cf10 <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)>:
  48cf10: e92d40f0     	push	{r4, r5, r6, r7, lr}
  48cf14: e59f50a8     	ldr	r5, [pc, #0xa8]         @ 0x48cfc4 <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)+0xb4>
  48cf18: e24dd00c     	sub	sp, sp, #12
  48cf1c: e1a06001     	mov	r6, r1
  48cf20: e1a04000     	mov	r4, r0
  48cf24: ebfffbe4     	bl	0x48bebc <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)> @ imm = #-0x1070
  48cf28: e59f3098     	ldr	r3, [pc, #0x98]         @ 0x48cfc8 <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)+0xb8>
  48cf2c: e08f5005     	add	r5, pc, r5
  48cf30: e5846044     	str	r6, [r4, #0x44]
  48cf34: e7953003     	ldr	r3, [r5, r3]
  48cf38: e2833008     	add	r3, r3, #8
  48cf3c: e5843000     	str	r3, [r4]
  48cf40: e5963074     	ldr	r3, [r6, #0x74]
  48cf44: e5965070     	ldr	r5, [r6, #0x70]
  48cf48: e1530005     	cmp	r3, r5
  48cf4c: 0a000019     	beq	0x48cfb8 <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)+0xa8> @ imm = #0x64
  48cf50: e2847030     	add	r7, r4, #48
  48cf54: e28d6004     	add	r6, sp, #4
  48cf58: ea000008     	b	0x48cf80 <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)+0x70> @ imm = #0x20
  48cf5c: e5813000     	str	r3, [r1]
  48cf60: e5943034     	ldr	r3, [r4, #0x34]
  48cf64: e2855018     	add	r5, r5, #24
  48cf68: e2833004     	add	r3, r3, #4
  48cf6c: e5843034     	str	r3, [r4, #0x34]
  48cf70: e5943044     	ldr	r3, [r4, #0x44]
  48cf74: e5933074     	ldr	r3, [r3, #0x74]
  48cf78: e1550003     	cmp	r5, r3
  48cf7c: 0a00000d     	beq	0x48cfb8 <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)+0xa8> @ imm = #0x34
  48cf80: e5941034     	ldr	r1, [r4, #0x34]
  48cf84: e5942038     	ldr	r2, [r4, #0x38]
  48cf88: e5953014     	ldr	r3, [r5, #0x14]
  48cf8c: e1510002     	cmp	r1, r2
  48cf90: e58d3004     	str	r3, [sp, #0x4]
  48cf94: 1afffff0     	bne	0x48cf5c <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)+0x4c> @ imm = #-0x40
  48cf98: e1a00007     	mov	r0, r7
  48cf9c: e1a02006     	mov	r2, r6
  48cfa0: ebffff77     	bl	0x48cd84 <std::vector<char const*, std::allocator<char const*>>::_M_insert_overflow(char const**, char const* const&, std::__true_type const&, unsigned int, bool) (.clone.15)> @ imm = #-0x224
  48cfa4: e5943044     	ldr	r3, [r4, #0x44]
  48cfa8: e2855018     	add	r5, r5, #24
  48cfac: e5933074     	ldr	r3, [r3, #0x74]
  48cfb0: e1550003     	cmp	r5, r3
  48cfb4: 1afffff1     	bne	0x48cf80 <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)+0x70> @ imm = #-0x3c
  48cfb8: e1a00004     	mov	r0, r4
  48cfbc: e28dd00c     	add	sp, sp, #12
  48cfc0: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  48cfc4: 64 7b 50 00  	.word	0x00507b64
  48cfc8: 08 4c 00 00  	.word	0x00004c08
