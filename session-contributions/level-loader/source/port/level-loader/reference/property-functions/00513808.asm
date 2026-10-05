
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00513808 <PropertyMap::GetProp(char const*)>:
  513808: e92d4010     	push	{r4, lr}
  51380c: e24dd008     	sub	sp, sp, #8
  513810: e28d4008     	add	r4, sp, #8
  513814: e5241004     	str	r1, [r4, #-0x4]!
  513818: ebffff28     	bl	0x5134c0 <PropertyMap::GetPropertyMap()> @ imm = #-0x360
  51381c: e1a01004     	mov	r1, r4
  513820: ebfffd2e     	bl	0x512ce0 <Property*& std::map<std::string, Property*, std::less<std::string>, std::allocator<std::pair<std::string const, Property*>>>::operator[]<char const*>(char const* const&)> @ imm = #-0xb48
  513824: e5900000     	ldr	r0, [r0]
  513828: e28dd008     	add	sp, sp, #8
  51382c: e8bd8010     	pop	{r4, pc}
