
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004905d0 <rnd::ListRule::Find(char const*, char const*, char const*, bool)>:
  4905d0: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  4905d4: e59f40d8     	ldr	r4, [pc, #0xd8]         @ 0x4906b4 <rnd::ListRule::Find(char const*, char const*, char const*, bool)+0xe4>
  4905d8: e59f70d8     	ldr	r7, [pc, #0xd8]         @ 0x4906b8 <rnd::ListRule::Find(char const*, char const*, char const*, bool)+0xe8>
  4905dc: e24dd05c     	sub	sp, sp, #92
  4905e0: e08f4004     	add	r4, pc, r4
  4905e4: e794c007     	ldr	r12, [r4, r7]
  4905e8: e59d9080     	ldr	r9, [sp, #0x80]
  4905ec: e1a0a003     	mov	r10, r3
  4905f0: e1a0b002     	mov	r11, r2
  4905f4: e5dd3084     	ldrb	r3, [sp, #0x84]
  4905f8: e59c2000     	ldr	r2, [r12]
  4905fc: e1a05001     	mov	r5, r1
  490600: e58d3004     	str	r3, [sp, #0x4]
  490604: e58d2054     	str	r2, [sp, #0x54]
  490608: e1a08000     	mov	r8, r0
  49060c: ebfff69b     	bl	0x48e080 <rnd::ListElem::ListElem()> @ imm = #-0x2594
  490610: e1a03009     	mov	r3, r9
  490614: e1a0200a     	mov	r2, r10
  490618: e5959020     	ldr	r9, [r5, #0x20]
  49061c: e595a01c     	ldr	r10, [r5, #0x1c]
  490620: e28d600c     	add	r6, sp, #12
  490624: e1a0100b     	mov	r1, r11
  490628: e1a00006     	mov	r0, r6
  49062c: ebfff6ab     	bl	0x48e0e0 <rnd::BlockSearch::BlockSearch(char const*, char const*, char const*)> @ imm = #-0x2554
  490630: e1a0000a     	mov	r0, r10
  490634: e1a01009     	mov	r1, r9
  490638: e1a02006     	mov	r2, r6
  49063c: ebffffc3     	bl	0x490550 <rnd::ListElem* std::find_if<rnd::ListElem*, rnd::BlockSearch>(rnd::ListElem*, rnd::ListElem*, rnd::BlockSearch)> @ imm = #-0xf4
  490640: e1a0a000     	mov	r10, r0
  490644: e1a00006     	mov	r0, r6
  490648: ebfff543     	bl	0x48db5c <rnd::BlockSearch::~BlockSearch()> @ imm = #-0x2af4
  49064c: e5953020     	ldr	r3, [r5, #0x20]
  490650: e15a0003     	cmp	r10, r3
  490654: 0a000008     	beq	0x49067c <rnd::ListRule::Find(char const*, char const*, char const*, bool)+0xac> @ imm = #0x20
  490658: e1a00008     	mov	r0, r8
  49065c: e1a0100a     	mov	r1, r10
  490660: ebffee95     	bl	0x48c0bc <rnd::ListElem::operator=(rnd::ListElem const&)> @ imm = #-0x45ac
  490664: e59d3004     	ldr	r3, [sp, #0x4]
  490668: e3530000     	cmp	r3, #0
  49066c: 0a000002     	beq	0x49067c <rnd::ListRule::Find(char const*, char const*, char const*, bool)+0xac> @ imm = #0x8
  490670: e5d53018     	ldrb	r3, [r5, #0x18]
  490674: e3530000     	cmp	r3, #0
  490678: 0a000007     	beq	0x49069c <rnd::ListRule::Find(char const*, char const*, char const*, bool)+0xcc> @ imm = #0x1c
  49067c: e7943007     	ldr	r3, [r4, r7]
  490680: e59d2054     	ldr	r2, [sp, #0x54]
  490684: e1a00008     	mov	r0, r8
  490688: e5933000     	ldr	r3, [r3]
  49068c: e1520003     	cmp	r2, r3
  490690: 1a000006     	bne	0x4906b0 <rnd::ListRule::Find(char const*, char const*, char const*, bool)+0xe0> @ imm = #0x18
  490694: e28dd05c     	add	sp, sp, #92
  490698: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  49069c: e285001c     	add	r0, r5, #28
  4906a0: e1a0100a     	mov	r1, r10
  4906a4: e28d2008     	add	r2, sp, #8
  4906a8: ebfff50e     	bl	0x48dae8 <std::vector<rnd::ListElem, std::allocator<rnd::ListElem>>::_M_erase(rnd::ListElem*, std::__false_type const&)> @ imm = #-0x2bc8
  4906ac: eafffff2     	b	0x49067c <rnd::ListRule::Find(char const*, char const*, char const*, bool)+0xac> @ imm = #-0x38
  4906b0: ebf9f716     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x1823a8
  4906b4: b0 44 50 00  	.word	0x005044b0
  4906b8: ac 40 00 00  	.word	0x000040ac
