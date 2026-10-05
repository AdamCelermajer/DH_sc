
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048db5c <rnd::BlockSearch::~BlockSearch()>:
  48db5c: e92d4010     	push	{r4, lr}
  48db60: e2803030     	add	r3, r0, #48
  48db64: e1a04000     	mov	r4, r0
  48db68: e5930014     	ldr	r0, [r3, #0x14]
  48db6c: e1500003     	cmp	r0, r3
  48db70: 0a000006     	beq	0x48db90 <rnd::BlockSearch::~BlockSearch()+0x34> @ imm = #0x18
  48db74: e3500000     	cmp	r0, #0
  48db78: 0a000004     	beq	0x48db90 <rnd::BlockSearch::~BlockSearch()+0x34> @ imm = #0x10
  48db7c: e5941030     	ldr	r1, [r4, #0x30]
  48db80: e0601001     	rsb	r1, r0, r1
  48db84: e3510080     	cmp	r1, #128
  48db88: 8a000017     	bhi	0x48dbec <rnd::BlockSearch::~BlockSearch()+0x90> @ imm = #0x5c
  48db8c: eb09ecdb     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27b36c
  48db90: e2843018     	add	r3, r4, #24
  48db94: e5930014     	ldr	r0, [r3, #0x14]
  48db98: e1500003     	cmp	r0, r3
  48db9c: 0a000006     	beq	0x48dbbc <rnd::BlockSearch::~BlockSearch()+0x60> @ imm = #0x18
  48dba0: e3500000     	cmp	r0, #0
  48dba4: 0a000004     	beq	0x48dbbc <rnd::BlockSearch::~BlockSearch()+0x60> @ imm = #0x10
  48dba8: e5941018     	ldr	r1, [r4, #0x18]
  48dbac: e0601001     	rsb	r1, r0, r1
  48dbb0: e3510080     	cmp	r1, #128
  48dbb4: 8a00000e     	bhi	0x48dbf4 <rnd::BlockSearch::~BlockSearch()+0x98> @ imm = #0x38
  48dbb8: eb09ecd0     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27b340
  48dbbc: e5940014     	ldr	r0, [r4, #0x14]
  48dbc0: e1500004     	cmp	r0, r4
  48dbc4: 0a000006     	beq	0x48dbe4 <rnd::BlockSearch::~BlockSearch()+0x88> @ imm = #0x18
  48dbc8: e3500000     	cmp	r0, #0
  48dbcc: 0a000004     	beq	0x48dbe4 <rnd::BlockSearch::~BlockSearch()+0x88> @ imm = #0x10
  48dbd0: e5941000     	ldr	r1, [r4]
  48dbd4: e0601001     	rsb	r1, r0, r1
  48dbd8: e3510080     	cmp	r1, #128
  48dbdc: 8a000006     	bhi	0x48dbfc <rnd::BlockSearch::~BlockSearch()+0xa0> @ imm = #0x18
  48dbe0: eb09ecc6     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27b318
  48dbe4: e1a00004     	mov	r0, r4
  48dbe8: e8bd8010     	pop	{r4, pc}
  48dbec: ebfa0a13     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17d7b4
  48dbf0: eaffffe6     	b	0x48db90 <rnd::BlockSearch::~BlockSearch()+0x34> @ imm = #-0x68
  48dbf4: ebfa0a11     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17d7bc
  48dbf8: eaffffef     	b	0x48dbbc <rnd::BlockSearch::~BlockSearch()+0x60> @ imm = #-0x44
  48dbfc: ebfa0a0f     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17d7c4
  48dc00: e1a00004     	mov	r0, r4
  48dc04: e8bd8010     	pop	{r4, pc}
