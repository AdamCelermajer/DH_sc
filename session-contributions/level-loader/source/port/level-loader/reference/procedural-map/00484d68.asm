
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00484d68 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::_M_erase(std::priv::_Rb_tree_node_base*)>:
  484d68: e92d4070     	push	{r4, r5, r6, lr}
  484d6c: e2514000     	subs	r4, r1, #0
  484d70: e1a06000     	mov	r6, r0
  484d74: 0a000008     	beq	0x484d9c <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::_M_erase(std::priv::_Rb_tree_node_base*)+0x34> @ imm = #0x20
  484d78: e594100c     	ldr	r1, [r4, #0xc]
  484d7c: e1a00006     	mov	r0, r6
  484d80: ebfffff8     	bl	0x484d68 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x20
  484d84: e5945008     	ldr	r5, [r4, #0x8]
  484d88: e1a00004     	mov	r0, r4
  484d8c: e3a01018     	mov	r1, #24
  484d90: eb0a105a     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x284168
  484d94: e2554000     	subs	r4, r5, #0
  484d98: 1afffff6     	bne	0x484d78 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::_M_erase(std::priv::_Rb_tree_node_base*)+0x10> @ imm = #-0x28
  484d9c: e8bd8070     	pop	{r4, r5, r6, pc}
