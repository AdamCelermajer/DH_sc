
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00513ce4 <PropertyMap::AddProperty(char const*, Property*)>:
  513ce4: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  513ce8: e24dd010     	sub	sp, sp, #16
  513cec: e58d1004     	str	r1, [sp, #0x4]
  513cf0: e1a07002     	mov	r7, r2
  513cf4: ebfff394     	bl	0x510b4c <PropertyMap::GetThisClassName()> @ imm = #-0x31b0
  513cf8: e28d3010     	add	r3, sp, #16
  513cfc: e5230004     	str	r0, [r3, #-0x4]!
  513d00: e1a00003     	mov	r0, r3
  513d04: ebfff9f2     	bl	0x5124d4 <std::map<std::string, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>>>>& std::map<std::string, std::map<std::string, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>>>>>>>::operator[]<char const*>(char const* const&) (.clone.2)> @ imm = #-0x1838
  513d08: ebffff66     	bl	0x513aa8 <std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>& std::map<std::string, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>>>>::operator[]<char [1]>(char const (&) [1]) (.clone.3)> @ imm = #-0x268
  513d0c: e28d5004     	add	r5, sp, #4
  513d10: e1a01005     	mov	r1, r5
  513d14: e1a06000     	mov	r6, r0
  513d18: ebfff850     	bl	0x511e60 <std::priv::_Rb_tree_node_base* std::priv::_Rb_tree<std::string, std::less<std::string>, std::pair<std::string const, Property*>, std::priv::_Select1st<std::pair<std::string const, Property*>>, std::priv::_MapTraitsT<std::pair<std::string const, Property*>>, std::allocator<std::pair<std::string const, Property*>>>::_M_find<char const*>(char const* const&) const> @ imm = #-0x1ec0
  513d1c: e59f404c     	ldr	r4, [pc, #0x4c]         @ 0x513d70 <PropertyMap::AddProperty(char const*, Property*)+0x8c>
  513d20: e1500006     	cmp	r0, r6
  513d24: e08f4004     	add	r4, pc, r4
  513d28: 0a00000a     	beq	0x513d58 <PropertyMap::AddProperty(char const*, Property*)+0x74> @ imm = #0x28
  513d2c: e5908028     	ldr	r8, [r0, #0x28]
  513d30: e3580000     	cmp	r8, #0
  513d34: 0a000007     	beq	0x513d58 <PropertyMap::AddProperty(char const*, Property*)+0x74> @ imm = #0x1c
  513d38: e59f3034     	ldr	r3, [pc, #0x34]         @ 0x513d74 <PropertyMap::AddProperty(char const*, Property*)+0x90>
  513d3c: e1a00008     	mov	r0, r8
  513d40: e7943003     	ldr	r3, [r4, r3]
  513d44: e2833008     	add	r3, r3, #8
  513d48: e4803008     	str	r3, [r0], #8
  513d4c: ebf81140     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x1fbb00
  513d50: e1a00008     	mov	r0, r8
  513d54: ebf7f1b9     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x20391c
  513d58: e1a00006     	mov	r0, r6
  513d5c: e1a01005     	mov	r1, r5
  513d60: ebfffbde     	bl	0x512ce0 <Property*& std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>::operator[]<char const*>(char const* const&)> @ imm = #-0x1088
  513d64: e5807000     	str	r7, [r0]
  513d68: e28dd010     	add	sp, sp, #16
  513d6c: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  513d70: 6c 0d 48 00  	.word	0x00480d6c
  513d74: 30 23 00 00  	.word	0x00002330
