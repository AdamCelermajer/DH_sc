
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

005134c0 <PropertyMap::GetPropertyMap()>:
  5134c0: e92d4010     	push	{r4, lr}
  5134c4: e24dd008     	sub	sp, sp, #8
  5134c8: e1a04000     	mov	r4, r0
  5134cc: ebfff59e     	bl	0x510b4c <PropertyMap::GetThisClassName()> @ imm = #-0x2988
  5134d0: e28d3008     	add	r3, sp, #8
  5134d4: e5230004     	str	r0, [r3, #-0x4]!
  5134d8: e1a00003     	mov	r0, r3
  5134dc: ebfffbfc     	bl	0x5124d4 <std::map<std::string, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>>>>& std::map<std::string, std::map<std::string, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>>>>>>>::operator[]<char const*>(char const* const&) (.clone.2)> @ imm = #-0x1010
  5134e0: e2841004     	add	r1, r4, #4
  5134e4: ebffff8b     	bl	0x513318 <std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>& std::map<std::string, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>, std::less<std::string>, std::allocator<std::pair<std::string const, std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>>>>::operator[]<std::string>(std::string const&)> @ imm = #-0x1d4
  5134e8: e28dd008     	add	sp, sp, #8
  5134ec: e8bd8010     	pop	{r4, pc}
