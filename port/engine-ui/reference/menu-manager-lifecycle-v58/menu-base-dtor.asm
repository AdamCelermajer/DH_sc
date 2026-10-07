
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00422824 <_ZN8MenuBaseD1Ev>:
  422824: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  422828: e59f711c     	ldr	r7, [pc, #0x11c]        @ 0x42294c <_ZN8MenuBaseD1Ev+0x128>
  42282c: e59f311c     	ldr	r3, [pc, #0x11c]        @ 0x422950 <_ZN8MenuBaseD1Ev+0x12c>
  422830: e1a05000     	mov	r5, r0
  422834: e08f7007     	add	r7, pc, r7
  422838: e7973003     	ldr	r3, [r7, r3]
  42283c: e2833008     	add	r3, r3, #8
  422840: e5803000     	str	r3, [r0]
  422844: ebfff32b     	bl	0x41f4f8 <_ZN8MenuBase16ClearSlideEventsEv> @ imm = #-0x3354
  422848: e595405c     	ldr	r4, [r5, #0x5c]
  42284c: e3540000     	cmp	r4, #0
  422850: 0a000005     	beq	0x42286c <_ZN8MenuBaseD1Ev+0x48> @ imm = #0x14
  422854: e1a00004     	mov	r0, r4
  422858: ebffbf09     	bl	0x412484 <_ZN11DragAndDropD1Ev> @ imm = #-0x103dc
  42285c: e1a00004     	mov	r0, r4
  422860: ebfbb6f6     	bl	0x310440 <_Z10CustomFreePv> @ imm = #-0x112428
  422864: e3a03000     	mov	r3, #0
  422868: e585305c     	str	r3, [r5, #0x5c]
  42286c: e28500b8     	add	r0, r5, #184
  422870: ebffff5a     	bl	0x4225e0 <_ZNSt6vectorIN7gameswf4rectESaIS1_EED1Ev> @ imm = #-0x298
  422874: e285009c     	add	r0, r5, #156
  422878: ebfbd675     	bl	0x318254 <_ZNSsD1Ev>    @ imm = #-0x10a62c
  42287c: e2850080     	add	r0, r5, #128
  422880: ebfbd673     	bl	0x318254 <_ZNSsD1Ev>    @ imm = #-0x10a634
  422884: e595006c     	ldr	r0, [r5, #0x6c]
  422888: e285606c     	add	r6, r5, #108
  42288c: e1500006     	cmp	r0, r6
  422890: 1a000001     	bne	0x42289c <_ZN8MenuBaseD1Ev+0x78> @ imm = #0x4
  422894: ea000006     	b	0x4228b4 <_ZN8MenuBaseD1Ev+0x90> @ imm = #0x18
  422898: e1a00004     	mov	r0, r4
  42289c: e5904000     	ldr	r4, [r0]
  4228a0: e3a01014     	mov	r1, #20
  4228a4: eb0b9995     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x2e6654
  4228a8: e1540006     	cmp	r4, r6
  4228ac: 1afffff9     	bne	0x422898 <_ZN8MenuBaseD1Ev+0x74> @ imm = #-0x1c
  4228b0: e1a00006     	mov	r0, r6
  4228b4: e585006c     	str	r0, [r5, #0x6c]
  4228b8: e5860004     	str	r0, [r6, #0x4]
  4228bc: e5950060     	ldr	r0, [r5, #0x60]
  4228c0: e2853060     	add	r3, r5, #96
  4228c4: e3500000     	cmp	r0, #0
  4228c8: 0a000005     	beq	0x4228e4 <_ZN8MenuBaseD1Ev+0xc0> @ imm = #0x14
  4228cc: e5931008     	ldr	r1, [r3, #0x8]
  4228d0: e0601001     	rsb	r1, r0, r1
  4228d4: e3c11003     	bic	r1, r1, #3
  4228d8: e3510080     	cmp	r1, #128
  4228dc: 8a000018     	bhi	0x422944 <_ZN8MenuBaseD1Ev+0x120> @ imm = #0x60
  4228e0: eb0b9986     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x2e6618
  4228e4: e59f3068     	ldr	r3, [pc, #0x68]         @ 0x422954 <_ZN8MenuBaseD1Ev+0x130>
  4228e8: e5950050     	ldr	r0, [r5, #0x50]
  4228ec: e7973003     	ldr	r3, [r7, r3]
  4228f0: e3500000     	cmp	r0, #0
  4228f4: e2833008     	add	r3, r3, #8
  4228f8: e5853000     	str	r3, [r5]
  4228fc: 0a000005     	beq	0x422918 <_ZN8MenuBaseD1Ev+0xf4> @ imm = #0x14
  422900: e5901000     	ldr	r1, [r0]
  422904: e2411001     	sub	r1, r1, #1
  422908: e3510000     	cmp	r1, #0
  42290c: e5801000     	str	r1, [r0]
  422910: 1a000000     	bne	0x422918 <_ZN8MenuBaseD1Ev+0xf4> @ imm = #0x0
  422914: eb0cc087     	bl	0x752b38 <_ZN7gameswf13free_internalEPvj> @ imm = #0x33021c
  422918: e5950048     	ldr	r0, [r5, #0x48]
  42291c: e3500000     	cmp	r0, #0
  422920: 0a000005     	beq	0x42293c <_ZN8MenuBaseD1Ev+0x118> @ imm = #0x14
  422924: e5901000     	ldr	r1, [r0]
  422928: e2411001     	sub	r1, r1, #1
  42292c: e3510000     	cmp	r1, #0
  422930: e5801000     	str	r1, [r0]
  422934: 1a000000     	bne	0x42293c <_ZN8MenuBaseD1Ev+0x118> @ imm = #0x0
  422938: eb0cc07e     	bl	0x752b38 <_ZN7gameswf13free_internalEPvj> @ imm = #0x3301f8
  42293c: e1a00005     	mov	r0, r5
  422940: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  422944: ebfbb6bd     	bl	0x310440 <_Z10CustomFreePv> @ imm = #-0x11250c
  422948: eaffffe5     	b	0x4228e4 <_ZN8MenuBaseD1Ev+0xc0> @ imm = #-0x6c
  42294c: 5c 22 57 00  	.word	0x0057225c
  422950: 1c 4a 00 00  	.word	0x00004a1c
  422954: 30 17 00 00  	.word	0x00001730

00422958 <_ZN8MenuBaseD0Ev>:
  422958: e92d4010     	push	{r4, lr}
  42295c: e1a04000     	mov	r4, r0
  422960: ebffffaf     	bl	0x422824 <_ZN8MenuBaseD1Ev> @ imm = #-0x144
  422964: e1a00004     	mov	r0, r4
  422968: ebfbb6b4     	bl	0x310440 <_Z10CustomFreePv> @ imm = #-0x112530
  42296c: e1a00004     	mov	r0, r4
  422970: e8bd8010     	pop	{r4, pc}
