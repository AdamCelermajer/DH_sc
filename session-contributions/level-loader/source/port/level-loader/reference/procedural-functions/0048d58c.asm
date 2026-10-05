
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048d58c <rnd::Rule::~Rule()>:
  48d58c: e59f3068     	ldr	r3, [pc, #0x68]         @ 0x48d5fc <rnd::Rule::~Rule()+0x70>
  48d590: e59f2068     	ldr	r2, [pc, #0x68]         @ 0x48d600 <rnd::Rule::~Rule()+0x74>
  48d594: e92d4070     	push	{r4, r5, r6, lr}
  48d598: e08f3003     	add	r3, pc, r3
  48d59c: e7932002     	ldr	r2, [r3, r2]
  48d5a0: e1a05000     	mov	r5, r0
  48d5a4: e1a04000     	mov	r4, r0
  48d5a8: e2822008     	add	r2, r2, #8
  48d5ac: e4852070     	str	r2, [r5], #112
  48d5b0: ebfff9b7     	bl	0x48bc94 <rnd::Rule::Unload()> @ imm = #-0x1924
  48d5b4: e1a00005     	mov	r0, r5
  48d5b8: ebfa1a5c     	bl	0x313f30 <std::vector<std::string, std::allocator<std::string>>::~vector()> @ imm = #-0x179690
  48d5bc: e2843050     	add	r3, r4, #80
  48d5c0: e5930014     	ldr	r0, [r3, #0x14]
  48d5c4: e1500003     	cmp	r0, r3
  48d5c8: 0a000006     	beq	0x48d5e8 <rnd::Rule::~Rule()+0x5c> @ imm = #0x18
  48d5cc: e3500000     	cmp	r0, #0
  48d5d0: 0a000004     	beq	0x48d5e8 <rnd::Rule::~Rule()+0x5c> @ imm = #0x10
  48d5d4: e5941050     	ldr	r1, [r4, #0x50]
  48d5d8: e0601001     	rsb	r1, r0, r1
  48d5dc: e3510080     	cmp	r1, #128
  48d5e0: 8a000002     	bhi	0x48d5f0 <rnd::Rule::~Rule()+0x64> @ imm = #0x8
  48d5e4: eb09ee45     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27b914
  48d5e8: e1a00004     	mov	r0, r4
  48d5ec: e8bd8070     	pop	{r4, r5, r6, pc}
  48d5f0: ebfa0b92     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17d1b8
  48d5f4: e1a00004     	mov	r0, r4
  48d5f8: e8bd8070     	pop	{r4, r5, r6, pc}
  48d5fc: f8 74 50 00  	.word	0x005074f8
  48d600: 2c 24 00 00  	.word	0x0000242c
