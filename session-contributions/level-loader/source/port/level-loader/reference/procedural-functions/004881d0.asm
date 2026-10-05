
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004881d0 <rnd::ListElem::~ListElem()>:
  4881d0: e92d4010     	push	{r4, lr}
  4881d4: e2803034     	add	r3, r0, #52
  4881d8: e1a04000     	mov	r4, r0
  4881dc: e5930014     	ldr	r0, [r3, #0x14]
  4881e0: e1500003     	cmp	r0, r3
  4881e4: 0a000006     	beq	0x488204 <rnd::ListElem::~ListElem()+0x34> @ imm = #0x18
  4881e8: e3500000     	cmp	r0, #0
  4881ec: 0a000004     	beq	0x488204 <rnd::ListElem::~ListElem()+0x34> @ imm = #0x10
  4881f0: e5941034     	ldr	r1, [r4, #0x34]
  4881f4: e0601001     	rsb	r1, r0, r1
  4881f8: e3510080     	cmp	r1, #128
  4881fc: 8a000018     	bhi	0x488264 <rnd::ListElem::~ListElem()+0x94> @ imm = #0x60
  488200: eb0a033e     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x280cf8
  488204: e284301c     	add	r3, r4, #28
  488208: e5930014     	ldr	r0, [r3, #0x14]
  48820c: e1500003     	cmp	r0, r3
  488210: 0a000006     	beq	0x488230 <rnd::ListElem::~ListElem()+0x60> @ imm = #0x18
  488214: e3500000     	cmp	r0, #0
  488218: 0a000004     	beq	0x488230 <rnd::ListElem::~ListElem()+0x60> @ imm = #0x10
  48821c: e594101c     	ldr	r1, [r4, #0x1c]
  488220: e0601001     	rsb	r1, r0, r1
  488224: e3510080     	cmp	r1, #128
  488228: 8a00000f     	bhi	0x48826c <rnd::ListElem::~ListElem()+0x9c> @ imm = #0x3c
  48822c: eb0a0333     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x280ccc
  488230: e2843004     	add	r3, r4, #4
  488234: e5930014     	ldr	r0, [r3, #0x14]
  488238: e1500003     	cmp	r0, r3
  48823c: 0a000006     	beq	0x48825c <rnd::ListElem::~ListElem()+0x8c> @ imm = #0x18
  488240: e3500000     	cmp	r0, #0
  488244: 0a000004     	beq	0x48825c <rnd::ListElem::~ListElem()+0x8c> @ imm = #0x10
  488248: e5941004     	ldr	r1, [r4, #0x4]
  48824c: e0601001     	rsb	r1, r0, r1
  488250: e3510080     	cmp	r1, #128
  488254: 8a000006     	bhi	0x488274 <rnd::ListElem::~ListElem()+0xa4> @ imm = #0x18
  488258: eb0a0328     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x280ca0
  48825c: e1a00004     	mov	r0, r4
  488260: e8bd8010     	pop	{r4, pc}
  488264: ebfa2075     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x177e2c
  488268: eaffffe5     	b	0x488204 <rnd::ListElem::~ListElem()+0x34> @ imm = #-0x6c
  48826c: ebfa2073     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x177e34
  488270: eaffffee     	b	0x488230 <rnd::ListElem::~ListElem()+0x60> @ imm = #-0x48
  488274: ebfa2071     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x177e3c
  488278: e1a00004     	mov	r0, r4
  48827c: e8bd8010     	pop	{r4, pc}
