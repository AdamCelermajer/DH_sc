
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00484554 <rnd::RandomGenerator::ValidList(char const*)>:
  484554: e59f306c     	ldr	r3, [pc, #0x6c]         @ 0x4845c8 <rnd::RandomGenerator::ValidList(char const*)+0x74>
  484558: e59f206c     	ldr	r2, [pc, #0x6c]         @ 0x4845cc <rnd::RandomGenerator::ValidList(char const*)+0x78>
  48455c: e92d4070     	push	{r4, r5, r6, lr}
  484560: e08f3003     	add	r3, pc, r3
  484564: e7936002     	ldr	r6, [r3, r2]
  484568: e24ddf82     	sub	sp, sp, #520
  48456c: e28d4004     	add	r4, sp, #4
  484570: e5962000     	ldr	r2, [r6]
  484574: e1a05000     	mov	r5, r0
  484578: e1a00004     	mov	r0, r4
  48457c: e2855054     	add	r5, r5, #84
  484580: e58d2204     	str	r2, [sp, #0x204]
  484584: ebfa27e5     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0x17606c
  484588: e3e02000     	mvn	r2, #0
  48458c: e1a00004     	mov	r0, r4
  484590: e3a01000     	mov	r1, #0
  484594: ebfb279e     	bl	0x34e414 <ToLowerCase(char*, int, int)> @ imm = #-0x136188
  484598: e1a01004     	mov	r1, r4
  48459c: e1a00005     	mov	r0, r5
  4845a0: ebffffb1     	bl	0x48446c <std::priv::_Rb_tree_node_base* std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::_M_find<char [512]>(char const (&) [512]) const> @ imm = #-0x13c
  4845a4: e59d2204     	ldr	r2, [sp, #0x204]
  4845a8: e5963000     	ldr	r3, [r6]
  4845ac: e0550000     	subs	r0, r5, r0
  4845b0: 13a00001     	movne	r0, #1
  4845b4: e1520003     	cmp	r2, r3
  4845b8: 1a000001     	bne	0x4845c4 <rnd::RandomGenerator::ValidList(char const*)+0x70> @ imm = #0x4
  4845bc: e28ddf82     	add	sp, sp, #520
  4845c0: e8bd8070     	pop	{r4, r5, r6, pc}
  4845c4: ebfa2751     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x1762bc
  4845c8: 30 05 51 00  	.word	0x00510530
  4845cc: ac 40 00 00  	.word	0x000040ac
