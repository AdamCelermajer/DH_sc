
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004845d0 <rnd::RandomGenerator::ValidBlock(char const*)>:
  4845d0: e59f306c     	ldr	r3, [pc, #0x6c]         @ 0x484644 <rnd::RandomGenerator::ValidBlock(char const*)+0x74>
  4845d4: e59f206c     	ldr	r2, [pc, #0x6c]         @ 0x484648 <rnd::RandomGenerator::ValidBlock(char const*)+0x78>
  4845d8: e92d4070     	push	{r4, r5, r6, lr}
  4845dc: e08f3003     	add	r3, pc, r3
  4845e0: e7936002     	ldr	r6, [r3, r2]
  4845e4: e24ddf82     	sub	sp, sp, #520
  4845e8: e28d4004     	add	r4, sp, #4
  4845ec: e5962000     	ldr	r2, [r6]
  4845f0: e1a05000     	mov	r5, r0
  4845f4: e1a00004     	mov	r0, r4
  4845f8: e285503c     	add	r5, r5, #60
  4845fc: e58d2204     	str	r2, [sp, #0x204]
  484600: ebfa27c6     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0x1760e8
  484604: e3e02000     	mvn	r2, #0
  484608: e1a00004     	mov	r0, r4
  48460c: e3a01000     	mov	r1, #0
  484610: ebfb277f     	bl	0x34e414 <ToLowerCase(char*, int, int)> @ imm = #-0x136204
  484614: e1a01004     	mov	r1, r4
  484618: e1a00005     	mov	r0, r5
  48461c: ebffffaf     	bl	0x4844e0 <std::priv::_Rb_tree_node_base* std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::_M_find<char [512]>(char const (&) [512]) const> @ imm = #-0x144
  484620: e59d2204     	ldr	r2, [sp, #0x204]
  484624: e5963000     	ldr	r3, [r6]
  484628: e0550000     	subs	r0, r5, r0
  48462c: 13a00001     	movne	r0, #1
  484630: e1520003     	cmp	r2, r3
  484634: 1a000001     	bne	0x484640 <rnd::RandomGenerator::ValidBlock(char const*)+0x70> @ imm = #0x4
  484638: e28ddf82     	add	sp, sp, #520
  48463c: e8bd8070     	pop	{r4, r5, r6, pc}
  484640: ebfa2732     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x176338
  484644: b4 04 51 00  	.word	0x005104b4
  484648: ac 40 00 00  	.word	0x000040ac
