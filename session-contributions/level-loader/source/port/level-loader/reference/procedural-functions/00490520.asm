
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00490520 <rnd::BlockSearch::BlockSearch(rnd::BlockSearch const&)>:
  490520: e92d4070     	push	{r4, r5, r6, lr}
  490524: e1a04000     	mov	r4, r0
  490528: e1a05001     	mov	r5, r1
  49052c: ebfa6cf9     	bl	0x32b918 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&)> @ imm = #-0x164c1c
  490530: e2851018     	add	r1, r5, #24
  490534: e2840018     	add	r0, r4, #24
  490538: ebfa6cf6     	bl	0x32b918 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&)> @ imm = #-0x164c28
  49053c: e2851030     	add	r1, r5, #48
  490540: e2840030     	add	r0, r4, #48
  490544: ebfa6cf3     	bl	0x32b918 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&)> @ imm = #-0x164c34
  490548: e1a00004     	mov	r0, r4
  49054c: e8bd8070     	pop	{r4, r5, r6, pc}
