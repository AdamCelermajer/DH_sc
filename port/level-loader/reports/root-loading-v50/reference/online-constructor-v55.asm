
.local-inputs/libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

007fd744 <COnline::GetInstance()>:
  7fd744: e59f3040     	ldr	r3, [pc, #0x40]         @ 0x7fd78c <COnline::GetInstance()+0x48>
  7fd748: e59f2040     	ldr	r2, [pc, #0x40]         @ 0x7fd790 <COnline::GetInstance()+0x4c>
  7fd74c: e92d4070     	push	{r4, r5, r6, lr}
  7fd750: e08f3003     	add	r3, pc, r3
  7fd754: e7934002     	ldr	r4, [r3, r2]
  7fd758: e5945000     	ldr	r5, [r4]
  7fd75c: e3550000     	cmp	r5, #0
  7fd760: 0a000001     	beq	0x7fd76c <COnline::GetInstance()+0x28> @ imm = #0x4
  7fd764: e1a00005     	mov	r0, r5
  7fd768: e8bd8070     	pop	{r4, r5, r6, pc}
  7fd76c: e3a01002     	mov	r1, #2
  7fd770: e3a00048     	mov	r0, #72
  7fd774: ebec4b7d     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x4ed20c
  7fd778: e1a05000     	mov	r5, r0
  7fd77c: eb009e1f     	bl	0x825000 <COnlineImpl::COnlineImpl()> @ imm = #0x2787c
  7fd780: e5845000     	str	r5, [r4]
  7fd784: e1a00005     	mov	r0, r5
  7fd788: e8bd8070     	pop	{r4, r5, r6, pc}
  7fd78c: 40 73 19 00  	.word	0x00197340
  7fd790: 2c 18 00 00  	.word	0x0000182c

007fd794 <GetOnline()>:
  7fd794: eaffffea     	b	0x7fd744 <COnline::GetInstance()> @ imm = #-0x58

007fd798 <COnline::COnline()>:
  7fd798: e92d4070     	push	{r4, r5, r6, lr}
  7fd79c: e59f5084     	ldr	r5, [pc, #0x84]         @ 0x7fd828 <COnline::COnline()+0x90>
  7fd7a0: e59f2084     	ldr	r2, [pc, #0x84]         @ 0x7fd82c <COnline::COnline()+0x94>
  7fd7a4: e59f3084     	ldr	r3, [pc, #0x84]         @ 0x7fd830 <COnline::COnline()+0x98>
  7fd7a8: e08f5005     	add	r5, pc, r5
  7fd7ac: e7952002     	ldr	r2, [r5, r2]
  7fd7b0: e7953003     	ldr	r3, [r5, r3]
  7fd7b4: e3a06000     	mov	r6, #0
  7fd7b8: e2822008     	add	r2, r2, #8
  7fd7bc: e2833008     	add	r3, r3, #8
  7fd7c0: e1a04000     	mov	r4, r0
  7fd7c4: e5802000     	str	r2, [r0]
  7fd7c8: e5803008     	str	r3, [r0, #0x8]
  7fd7cc: e5c06004     	strb	r6, [r0, #0x4]
  7fd7d0: e5c06005     	strb	r6, [r0, #0x5]
  7fd7d4: e280000c     	add	r0, r0, #12
  7fd7d8: eb0042ee     	bl	0x80e398 <CNetMutex::CNetMutex()> @ imm = #0x10bb8
  7fd7dc: e59f3050     	ldr	r3, [pc, #0x50]         @ 0x7fd834 <COnline::COnline()+0x9c>
  7fd7e0: e2842010     	add	r2, r4, #16
  7fd7e4: e5842014     	str	r2, [r4, #0x14]
  7fd7e8: e7953003     	ldr	r3, [r5, r3]
  7fd7ec: e5846028     	str	r6, [r4, #0x28]
  7fd7f0: e5842010     	str	r2, [r4, #0x10]
  7fd7f4: e2833008     	add	r3, r3, #8
  7fd7f8: e5843008     	str	r3, [r4, #0x8]
  7fd7fc: e3a03001     	mov	r3, #1
  7fd800: e5c43030     	strb	r3, [r4, #0x30]
  7fd804: e5846018     	str	r6, [r4, #0x18]
  7fd808: e584601c     	str	r6, [r4, #0x1c]
  7fd80c: e2840034     	add	r0, r4, #52
  7fd810: eb0042e0     	bl	0x80e398 <CNetMutex::CNetMutex()> @ imm = #0x10b80
  7fd814: e2843038     	add	r3, r4, #56
  7fd818: e584303c     	str	r3, [r4, #0x3c]
  7fd81c: e5843038     	str	r3, [r4, #0x38]
  7fd820: e1a00004     	mov	r0, r4
  7fd824: e8bd8070     	pop	{r4, r5, r6, pc}
  7fd828: e8 72 19 00  	.word	0x001972e8
  7fd82c: 88 2b 00 00  	.word	0x00002b88
  7fd830: 4c 0a 00 00  	.word	0x00000a4c
  7fd834: a0 34 00 00  	.word	0x000034a0

007fd838 <COnline::COnline()>:
  7fd838: e92d4070     	push	{r4, r5, r6, lr}
  7fd83c: e59f5084     	ldr	r5, [pc, #0x84]         @ 0x7fd8c8 <COnline::COnline()+0x90>
  7fd840: e59f2084     	ldr	r2, [pc, #0x84]         @ 0x7fd8cc <COnline::COnline()+0x94>
  7fd844: e59f3084     	ldr	r3, [pc, #0x84]         @ 0x7fd8d0 <COnline::COnline()+0x98>
  7fd848: e08f5005     	add	r5, pc, r5
  7fd84c: e7952002     	ldr	r2, [r5, r2]
  7fd850: e7953003     	ldr	r3, [r5, r3]
  7fd854: e3a06000     	mov	r6, #0
  7fd858: e2822008     	add	r2, r2, #8
  7fd85c: e2833008     	add	r3, r3, #8
  7fd860: e1a04000     	mov	r4, r0
  7fd864: e5802000     	str	r2, [r0]
  7fd868: e5803008     	str	r3, [r0, #0x8]
  7fd86c: e5c06004     	strb	r6, [r0, #0x4]
  7fd870: e5c06005     	strb	r6, [r0, #0x5]
  7fd874: e280000c     	add	r0, r0, #12
  7fd878: eb0042c6     	bl	0x80e398 <CNetMutex::CNetMutex()> @ imm = #0x10b18
  7fd87c: e59f3050     	ldr	r3, [pc, #0x50]         @ 0x7fd8d4 <COnline::COnline()+0x9c>
  7fd880: e2842010     	add	r2, r4, #16
  7fd884: e5842014     	str	r2, [r4, #0x14]
  7fd888: e7953003     	ldr	r3, [r5, r3]
  7fd88c: e5846028     	str	r6, [r4, #0x28]
  7fd890: e5842010     	str	r2, [r4, #0x10]
  7fd894: e2833008     	add	r3, r3, #8
  7fd898: e5843008     	str	r3, [r4, #0x8]
  7fd89c: e3a03001     	mov	r3, #1
  7fd8a0: e5c43030     	strb	r3, [r4, #0x30]
  7fd8a4: e5846018     	str	r6, [r4, #0x18]
  7fd8a8: e584601c     	str	r6, [r4, #0x1c]
  7fd8ac: e2840034     	add	r0, r4, #52
  7fd8b0: eb0042b8     	bl	0x80e398 <CNetMutex::CNetMutex()> @ imm = #0x10ae0
  7fd8b4: e2843038     	add	r3, r4, #56
  7fd8b8: e584303c     	str	r3, [r4, #0x3c]
  7fd8bc: e5843038     	str	r3, [r4, #0x38]
  7fd8c0: e1a00004     	mov	r0, r4
  7fd8c4: e8bd8070     	pop	{r4, r5, r6, pc}
  7fd8c8: 48 72 19 00  	.word	0x00197248
  7fd8cc: 88 2b 00 00  	.word	0x00002b88
  7fd8d0: 4c 0a 00 00  	.word	0x00000a4c
  7fd8d4: a0 34 00 00  	.word	0x000034a0
