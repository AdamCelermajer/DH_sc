
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d2c4 <rnd::Rule::Impl::~Impl()>:
  48d2c4: e92d4010     	push	{r4, lr}
  48d2c8: e59f30f4     	ldr	r3, [pc, #0xf4]         @ 0x48d3c4 <rnd::Rule::Impl::~Impl()+0x100>
  48d2cc: e59f20f4     	ldr	r2, [pc, #0xf4]         @ 0x48d3c8 <rnd::Rule::Impl::~Impl()+0x104>
  48d2d0: e590c008     	ldr	r12, [r0, #0x8]
  48d2d4: e08f3003     	add	r3, pc, r3
  48d2d8: e7932002     	ldr	r2, [r3, r2]
  48d2dc: e35c0000     	cmp	r12, #0
  48d2e0: e1a04000     	mov	r4, r0
  48d2e4: e2822008     	add	r2, r2, #8
  48d2e8: e5802000     	str	r2, [r0]
  48d2ec: 0a000010     	beq	0x48d334 <rnd::Rule::Impl::~Impl()+0x70> @ imm = #0x40
  48d2f0: e59c000c     	ldr	r0, [r12, #0xc]
  48d2f4: e3500000     	cmp	r0, #0
  48d2f8: da00000d     	ble	0x48d334 <rnd::Rule::Impl::~Impl()+0x70> @ imm = #0x34
  48d2fc: e59c3010     	ldr	r3, [r12, #0x10]
  48d300: e1540003     	cmp	r4, r3
  48d304: 03a03000     	moveq	r3, #0
  48d308: 0a000023     	beq	0x48d39c <rnd::Rule::Impl::~Impl()+0xd8> @ imm = #0x8c
  48d30c: e1a0200c     	mov	r2, r12
  48d310: e3a03000     	mov	r3, #0
  48d314: ea000002     	b	0x48d324 <rnd::Rule::Impl::~Impl()+0x60> @ imm = #0x8
  48d318: e5921010     	ldr	r1, [r2, #0x10]
  48d31c: e1540001     	cmp	r4, r1
  48d320: 0a00001d     	beq	0x48d39c <rnd::Rule::Impl::~Impl()+0xd8> @ imm = #0x74
  48d324: e2833001     	add	r3, r3, #1
  48d328: e1530000     	cmp	r3, r0
  48d32c: e2822004     	add	r2, r2, #4
  48d330: 1afffff8     	bne	0x48d318 <rnd::Rule::Impl::~Impl()+0x54> @ imm = #-0x20
  48d334: e594200c     	ldr	r2, [r4, #0xc]
  48d338: e3520000     	cmp	r2, #0
  48d33c: e2422001     	sub	r2, r2, #1
  48d340: e2823004     	add	r3, r2, #4
  48d344: da000008     	ble	0x48d36c <rnd::Rule::Impl::~Impl()+0xa8> @ imm = #0x20
  48d348: e584200c     	str	r2, [r4, #0xc]
  48d34c: e7943103     	ldr	r3, [r4, r3, lsl #2]
  48d350: e3530000     	cmp	r3, #0
  48d354: 0afffff7     	beq	0x48d338 <rnd::Rule::Impl::~Impl()+0x74> @ imm = #-0x24
  48d358: e1a00003     	mov	r0, r3
  48d35c: e5933000     	ldr	r3, [r3]
  48d360: e1a0e00f     	mov	lr, pc
  48d364: e593f004     	ldr	pc, [r3, #0x4]
  48d368: eafffff1     	b	0x48d334 <rnd::Rule::Impl::~Impl()+0x70> @ imm = #-0x3c
  48d36c: e5940030     	ldr	r0, [r4, #0x30]
  48d370: e2843030     	add	r3, r4, #48
  48d374: e3500000     	cmp	r0, #0
  48d378: 0a000005     	beq	0x48d394 <rnd::Rule::Impl::~Impl()+0xd0> @ imm = #0x14
  48d37c: e5931008     	ldr	r1, [r3, #0x8]
  48d380: e0601001     	rsb	r1, r0, r1
  48d384: e3c11003     	bic	r1, r1, #3
  48d388: e3510080     	cmp	r1, #128
  48d38c: 8a000009     	bhi	0x48d3b8 <rnd::Rule::Impl::~Impl()+0xf4> @ imm = #0x24
  48d390: eb09eeda     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27bb68
  48d394: e1a00004     	mov	r0, r4
  48d398: e8bd8010     	pop	{r4, pc}
  48d39c: e2402001     	sub	r2, r0, #1
  48d3a0: e58c200c     	str	r2, [r12, #0xc]
  48d3a4: e2800003     	add	r0, r0, #3
  48d3a8: e79c2100     	ldr	r2, [r12, r0, lsl #2]
  48d3ac: e2833004     	add	r3, r3, #4
  48d3b0: e78c2103     	str	r2, [r12, r3, lsl #2]
  48d3b4: eaffffde     	b	0x48d334 <rnd::Rule::Impl::~Impl()+0x70> @ imm = #-0x88
  48d3b8: ebfa0c20     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17cf80
  48d3bc: e1a00004     	mov	r0, r4
  48d3c0: e8bd8010     	pop	{r4, pc}
  48d3c4: bc 77 50 00  	.word	0x005077bc
  48d3c8: 00 10 00 00  	.word	0x00001000
