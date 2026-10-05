
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048cff8 <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)>:
  48cff8: e92d40f0     	push	{r4, r5, r6, r7, lr}
  48cffc: e59f50a8     	ldr	r5, [pc, #0xa8]         @ 0x48d0ac <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)+0xb4>
  48d000: e24dd00c     	sub	sp, sp, #12
  48d004: e1a06001     	mov	r6, r1
  48d008: e1a04000     	mov	r4, r0
  48d00c: ebfffbaa     	bl	0x48bebc <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)> @ imm = #-0x1158
  48d010: e59f3098     	ldr	r3, [pc, #0x98]         @ 0x48d0b0 <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)+0xb8>
  48d014: e08f5005     	add	r5, pc, r5
  48d018: e5846044     	str	r6, [r4, #0x44]
  48d01c: e7953003     	ldr	r3, [r5, r3]
  48d020: e2833008     	add	r3, r3, #8
  48d024: e5843000     	str	r3, [r4]
  48d028: e5963074     	ldr	r3, [r6, #0x74]
  48d02c: e5965070     	ldr	r5, [r6, #0x70]
  48d030: e1530005     	cmp	r3, r5
  48d034: 0a000019     	beq	0x48d0a0 <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)+0xa8> @ imm = #0x64
  48d038: e2847030     	add	r7, r4, #48
  48d03c: e28d6004     	add	r6, sp, #4
  48d040: ea000008     	b	0x48d068 <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)+0x70> @ imm = #0x20
  48d044: e5813000     	str	r3, [r1]
  48d048: e5943034     	ldr	r3, [r4, #0x34]
  48d04c: e2855018     	add	r5, r5, #24
  48d050: e2833004     	add	r3, r3, #4
  48d054: e5843034     	str	r3, [r4, #0x34]
  48d058: e5943044     	ldr	r3, [r4, #0x44]
  48d05c: e5933074     	ldr	r3, [r3, #0x74]
  48d060: e1550003     	cmp	r5, r3
  48d064: 0a00000d     	beq	0x48d0a0 <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)+0xa8> @ imm = #0x34
  48d068: e5941034     	ldr	r1, [r4, #0x34]
  48d06c: e5942038     	ldr	r2, [r4, #0x38]
  48d070: e5953014     	ldr	r3, [r5, #0x14]
  48d074: e1510002     	cmp	r1, r2
  48d078: e58d3004     	str	r3, [sp, #0x4]
  48d07c: 1afffff0     	bne	0x48d044 <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)+0x4c> @ imm = #-0x40
  48d080: e1a00007     	mov	r0, r7
  48d084: e1a02006     	mov	r2, r6
  48d088: ebffff3d     	bl	0x48cd84 <std::vector<char const*, std::allocator<char const*>>::_M_insert_overflow(char const**, char const* const&, std::__true_type const&, unsigned int, bool) (.clone.15)> @ imm = #-0x30c
  48d08c: e5943044     	ldr	r3, [r4, #0x44]
  48d090: e2855018     	add	r5, r5, #24
  48d094: e5933074     	ldr	r3, [r3, #0x74]
  48d098: e1550003     	cmp	r5, r3
  48d09c: 1afffff1     	bne	0x48d068 <rnd::ForceBlock::Impl::Impl(rnd::ForceBlock const&, rnd::Rule::Impl*)+0x70> @ imm = #-0x3c
  48d0a0: e1a00004     	mov	r0, r4
  48d0a4: e28dd00c     	add	sp, sp, #12
  48d0a8: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  48d0ac: 7c 7a 50 00  	.word	0x00507a7c
  48d0b0: 08 4c 00 00  	.word	0x00004c08
