
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d7e8 <rnd::Rule::~Rule()>:
  48d7e8: e59f3068     	ldr	r3, [pc, #0x68]         @ 0x48d858 <rnd::Rule::~Rule()+0x70>
  48d7ec: e59f2068     	ldr	r2, [pc, #0x68]         @ 0x48d85c <rnd::Rule::~Rule()+0x74>
  48d7f0: e92d4070     	push	{r4, r5, r6, lr}
  48d7f4: e08f3003     	add	r3, pc, r3
  48d7f8: e7932002     	ldr	r2, [r3, r2]
  48d7fc: e1a05000     	mov	r5, r0
  48d800: e1a04000     	mov	r4, r0
  48d804: e2822008     	add	r2, r2, #8
  48d808: e4852070     	str	r2, [r5], #112
  48d80c: ebfff920     	bl	0x48bc94 <rnd::Rule::Unload()> @ imm = #-0x1b80
  48d810: e1a00005     	mov	r0, r5
  48d814: ebfa19c5     	bl	0x313f30 <std::vector<std::string, std::allocator<std::string>>::~vector()> @ imm = #-0x1798ec
  48d818: e2843050     	add	r3, r4, #80
  48d81c: e5930014     	ldr	r0, [r3, #0x14]
  48d820: e1500003     	cmp	r0, r3
  48d824: 0a000006     	beq	0x48d844 <rnd::Rule::~Rule()+0x5c> @ imm = #0x18
  48d828: e3500000     	cmp	r0, #0
  48d82c: 0a000004     	beq	0x48d844 <rnd::Rule::~Rule()+0x5c> @ imm = #0x10
  48d830: e5941050     	ldr	r1, [r4, #0x50]
  48d834: e0601001     	rsb	r1, r0, r1
  48d838: e3510080     	cmp	r1, #128
  48d83c: 8a000002     	bhi	0x48d84c <rnd::Rule::~Rule()+0x64> @ imm = #0x8
  48d840: eb09edae     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27b6b8
  48d844: e1a00004     	mov	r0, r4
  48d848: e8bd8070     	pop	{r4, r5, r6, pc}
  48d84c: ebfa0afb     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17d414
  48d850: e1a00004     	mov	r0, r4
  48d854: e8bd8070     	pop	{r4, r5, r6, pc}
  48d858: 9c 72 50 00  	.word	0x0050729c
  48d85c: 2c 24 00 00  	.word	0x0000242c
