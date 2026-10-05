
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d1a0 <rnd::Rule::Impl::~Impl()>:
  48d1a0: e92d4010     	push	{r4, lr}
  48d1a4: e59f30f4     	ldr	r3, [pc, #0xf4]         @ 0x48d2a0 <rnd::Rule::Impl::~Impl()+0x100>
  48d1a8: e59f20f4     	ldr	r2, [pc, #0xf4]         @ 0x48d2a4 <rnd::Rule::Impl::~Impl()+0x104>
  48d1ac: e590c008     	ldr	r12, [r0, #0x8]
  48d1b0: e08f3003     	add	r3, pc, r3
  48d1b4: e7932002     	ldr	r2, [r3, r2]
  48d1b8: e35c0000     	cmp	r12, #0
  48d1bc: e1a04000     	mov	r4, r0
  48d1c0: e2822008     	add	r2, r2, #8
  48d1c4: e5802000     	str	r2, [r0]
  48d1c8: 0a000010     	beq	0x48d210 <rnd::Rule::Impl::~Impl()+0x70> @ imm = #0x40
  48d1cc: e59c000c     	ldr	r0, [r12, #0xc]
  48d1d0: e3500000     	cmp	r0, #0
  48d1d4: da00000d     	ble	0x48d210 <rnd::Rule::Impl::~Impl()+0x70> @ imm = #0x34
  48d1d8: e59c3010     	ldr	r3, [r12, #0x10]
  48d1dc: e1540003     	cmp	r4, r3
  48d1e0: 03a03000     	moveq	r3, #0
  48d1e4: 0a000023     	beq	0x48d278 <rnd::Rule::Impl::~Impl()+0xd8> @ imm = #0x8c
  48d1e8: e1a0200c     	mov	r2, r12
  48d1ec: e3a03000     	mov	r3, #0
  48d1f0: ea000002     	b	0x48d200 <rnd::Rule::Impl::~Impl()+0x60> @ imm = #0x8
  48d1f4: e5921010     	ldr	r1, [r2, #0x10]
  48d1f8: e1540001     	cmp	r4, r1
  48d1fc: 0a00001d     	beq	0x48d278 <rnd::Rule::Impl::~Impl()+0xd8> @ imm = #0x74
  48d200: e2833001     	add	r3, r3, #1
  48d204: e1530000     	cmp	r3, r0
  48d208: e2822004     	add	r2, r2, #4
  48d20c: 1afffff8     	bne	0x48d1f4 <rnd::Rule::Impl::~Impl()+0x54> @ imm = #-0x20
  48d210: e594200c     	ldr	r2, [r4, #0xc]
  48d214: e3520000     	cmp	r2, #0
  48d218: e2422001     	sub	r2, r2, #1
  48d21c: e2823004     	add	r3, r2, #4
  48d220: da000008     	ble	0x48d248 <rnd::Rule::Impl::~Impl()+0xa8> @ imm = #0x20
  48d224: e584200c     	str	r2, [r4, #0xc]
  48d228: e7943103     	ldr	r3, [r4, r3, lsl #2]
  48d22c: e3530000     	cmp	r3, #0
  48d230: 0afffff7     	beq	0x48d214 <rnd::Rule::Impl::~Impl()+0x74> @ imm = #-0x24
  48d234: e1a00003     	mov	r0, r3
  48d238: e5933000     	ldr	r3, [r3]
  48d23c: e1a0e00f     	mov	lr, pc
  48d240: e593f004     	ldr	pc, [r3, #0x4]
  48d244: eafffff1     	b	0x48d210 <rnd::Rule::Impl::~Impl()+0x70> @ imm = #-0x3c
  48d248: e5940030     	ldr	r0, [r4, #0x30]
  48d24c: e2843030     	add	r3, r4, #48
  48d250: e3500000     	cmp	r0, #0
  48d254: 0a000005     	beq	0x48d270 <rnd::Rule::Impl::~Impl()+0xd0> @ imm = #0x14
  48d258: e5931008     	ldr	r1, [r3, #0x8]
  48d25c: e0601001     	rsb	r1, r0, r1
  48d260: e3c11003     	bic	r1, r1, #3
  48d264: e3510080     	cmp	r1, #128
  48d268: 8a000009     	bhi	0x48d294 <rnd::Rule::Impl::~Impl()+0xf4> @ imm = #0x24
  48d26c: eb09ef23     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27bc8c
  48d270: e1a00004     	mov	r0, r4
  48d274: e8bd8010     	pop	{r4, pc}
  48d278: e2402001     	sub	r2, r0, #1
  48d27c: e58c200c     	str	r2, [r12, #0xc]
  48d280: e2800003     	add	r0, r0, #3
  48d284: e79c2100     	ldr	r2, [r12, r0, lsl #2]
  48d288: e2833004     	add	r3, r3, #4
  48d28c: e78c2103     	str	r2, [r12, r3, lsl #2]
  48d290: eaffffde     	b	0x48d210 <rnd::Rule::Impl::~Impl()+0x70> @ imm = #-0x88
  48d294: ebfa0c69     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17ce5c
  48d298: e1a00004     	mov	r0, r4
  48d29c: e8bd8010     	pop	{r4, pc}
  48d2a0: e0 78 50 00  	.word	0x005078e0
  48d2a4: 00 10 00 00  	.word	0x00001000
