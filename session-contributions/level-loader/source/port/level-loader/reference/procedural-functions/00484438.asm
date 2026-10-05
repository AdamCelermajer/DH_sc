
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00484438 <rnd::RandomGenerator::GetList(char const*) const>:
  484438: e92d4010     	push	{r4, lr}
  48443c: e24dd008     	sub	sp, sp, #8
  484440: e28d3008     	add	r3, sp, #8
  484444: e5231004     	str	r1, [r3, #-0x4]!
  484448: e2804054     	add	r4, r0, #84
  48444c: e1a01003     	mov	r1, r3
  484450: e1a00004     	mov	r0, r4
  484454: ebffffda     	bl	0x4843c4 <std::priv::_Rb_tree_node_base* std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::_M_find<char const*>(char const* const&) const> @ imm = #-0x98
  484458: e1500004     	cmp	r0, r4
  48445c: 03a00000     	moveq	r0, #0
  484460: 15900014     	ldrne	r0, [r0, #0x14]
  484464: e28dd008     	add	sp, sp, #8
  484468: e8bd8010     	pop	{r4, pc}
