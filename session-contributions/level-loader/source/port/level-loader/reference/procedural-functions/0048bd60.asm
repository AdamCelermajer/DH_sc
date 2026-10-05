
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048bd60 <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)>:
  48bd60: e92d4070     	push	{r4, r5, r6, lr}
  48bd64: e59f4134     	ldr	r4, [pc, #0x134]        @ 0x48bea0 <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0x140>
  48bd68: e59f3134     	ldr	r3, [pc, #0x134]        @ 0x48bea4 <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0x144>
  48bd6c: e1a05000     	mov	r5, r0
  48bd70: e08f4004     	add	r4, pc, r4
  48bd74: e7943003     	ldr	r3, [r4, r3]
  48bd78: e3a00000     	mov	r0, #0
  48bd7c: e3520000     	cmp	r2, #0
  48bd80: e2833008     	add	r3, r3, #8
  48bd84: e5853000     	str	r3, [r5]
  48bd88: e3a03001     	mov	r3, #1
  48bd8c: e5851004     	str	r1, [r5, #0x4]
  48bd90: e585302c     	str	r3, [r5, #0x2c]
  48bd94: e5850040     	str	r0, [r5, #0x40]
  48bd98: e5852008     	str	r2, [r5, #0x8]
  48bd9c: e585000c     	str	r0, [r5, #0xc]
  48bda0: e5850028     	str	r0, [r5, #0x28]
  48bda4: e5850030     	str	r0, [r5, #0x30]
  48bda8: e5850034     	str	r0, [r5, #0x34]
  48bdac: e5850038     	str	r0, [r5, #0x38]
  48bdb0: e585003c     	str	r0, [r5, #0x3c]
  48bdb4: 0a000012     	beq	0x48be04 <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0xa4> @ imm = #0x48
  48bdb8: e592300c     	ldr	r3, [r2, #0xc]
  48bdbc: e2831001     	add	r1, r3, #1
  48bdc0: e2833004     	add	r3, r3, #4
  48bdc4: e7825103     	str	r5, [r2, r3, lsl #2]
  48bdc8: e582100c     	str	r1, [r2, #0xc]
  48bdcc: e5950004     	ldr	r0, [r5, #0x4]
  48bdd0: e5902060     	ldr	r2, [r0, #0x60]
  48bdd4: e5903064     	ldr	r3, [r0, #0x64]
  48bdd8: e1520003     	cmp	r2, r3
  48bddc: 0a000008     	beq	0x48be04 <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0xa4> @ imm = #0x20
  48bde0: e59f10c0     	ldr	r1, [pc, #0xc0]         @ 0x48bea8 <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0x148>
  48bde4: e2800050     	add	r0, r0, #80
  48bde8: e08f1001     	add	r1, pc, r1
  48bdec: ebfc664b     	bl	0x3a5720 <std::string::compare(char const*) const> @ imm = #-0xe66d4
  48bdf0: e3500000     	cmp	r0, #0
  48bdf4: 1a000004     	bne	0x48be0c <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0xac> @ imm = #0x10
  48bdf8: e59f30ac     	ldr	r3, [pc, #0xac]         @ 0x48beac <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0x14c>
  48bdfc: e7943003     	ldr	r3, [r4, r3]
  48be00: e585303c     	str	r3, [r5, #0x3c]
  48be04: e1a00005     	mov	r0, r5
  48be08: e8bd8070     	pop	{r4, r5, r6, pc}
  48be0c: e5950004     	ldr	r0, [r5, #0x4]
  48be10: e59f1098     	ldr	r1, [pc, #0x98]         @ 0x48beb0 <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0x150>
  48be14: e2800050     	add	r0, r0, #80
  48be18: e08f1001     	add	r1, pc, r1
  48be1c: ebfc663f     	bl	0x3a5720 <std::string::compare(char const*) const> @ imm = #-0xe6704
  48be20: e3500000     	cmp	r0, #0
  48be24: 1a000005     	bne	0x48be40 <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0xe0> @ imm = #0x14
  48be28: e59f307c     	ldr	r3, [pc, #0x7c]         @ 0x48beac <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0x14c>
  48be2c: e1a00005     	mov	r0, r5
  48be30: e7943003     	ldr	r3, [r4, r3]
  48be34: e2833010     	add	r3, r3, #16
  48be38: e585303c     	str	r3, [r5, #0x3c]
  48be3c: e8bd8070     	pop	{r4, r5, r6, pc}
  48be40: e5950004     	ldr	r0, [r5, #0x4]
  48be44: e59f1068     	ldr	r1, [pc, #0x68]         @ 0x48beb4 <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0x154>
  48be48: e2800050     	add	r0, r0, #80
  48be4c: e08f1001     	add	r1, pc, r1
  48be50: ebfc6632     	bl	0x3a5720 <std::string::compare(char const*) const> @ imm = #-0xe6738
  48be54: e3500000     	cmp	r0, #0
  48be58: 1a000004     	bne	0x48be70 <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0x110> @ imm = #0x10
  48be5c: e59f3048     	ldr	r3, [pc, #0x48]         @ 0x48beac <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0x14c>
  48be60: e7943003     	ldr	r3, [r4, r3]
  48be64: e2833020     	add	r3, r3, #32
  48be68: e585303c     	str	r3, [r5, #0x3c]
  48be6c: eaffffe4     	b	0x48be04 <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0xa4> @ imm = #-0x70
  48be70: e5950004     	ldr	r0, [r5, #0x4]
  48be74: e59f103c     	ldr	r1, [pc, #0x3c]         @ 0x48beb8 <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0x158>
  48be78: e2800050     	add	r0, r0, #80
  48be7c: e08f1001     	add	r1, pc, r1
  48be80: ebfc6626     	bl	0x3a5720 <std::string::compare(char const*) const> @ imm = #-0xe6768
  48be84: e3500000     	cmp	r0, #0
  48be88: 1affffdd     	bne	0x48be04 <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0xa4> @ imm = #-0x8c
  48be8c: e59f3018     	ldr	r3, [pc, #0x18]         @ 0x48beac <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0x14c>
  48be90: e7943003     	ldr	r3, [r4, r3]
  48be94: e2833030     	add	r3, r3, #48
  48be98: e585303c     	str	r3, [r5, #0x3c]
  48be9c: eaffffd8     	b	0x48be04 <rnd::Rule::Impl::Impl(rnd::Rule const&, rnd::Rule::Impl*)+0xa4> @ imm = #-0xa0
  48bea0: 20 8d 50 00  	.word	0x00508d20
  48bea4: 00 10 00 00  	.word	0x00001000
  48bea8: d0 8f 44 00  	.word	0x00448fd0
  48beac: fc 43 00 00  	.word	0x000043fc
  48beb0: a8 8f 44 00  	.word	0x00448fa8
  48beb4: 7c 8f 44 00  	.word	0x00448f7c
  48beb8: 54 8f 44 00  	.word	0x00448f54
