
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048e0e0 <rnd::BlockSearch::BlockSearch(char const*, char const*, char const*)>:
  48e0e0: e92d4070     	push	{r4, r5, r6, lr}
  48e0e4: e24dd010     	sub	sp, sp, #16
  48e0e8: e1a04000     	mov	r4, r0
  48e0ec: e1a05002     	mov	r5, r2
  48e0f0: e28d200c     	add	r2, sp, #12
  48e0f4: e1a06003     	mov	r6, r3
  48e0f8: ebfa17fb     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x17a014
  48e0fc: e1a01005     	mov	r1, r5
  48e100: e28d2008     	add	r2, sp, #8
  48e104: e2840018     	add	r0, r4, #24
  48e108: ebfa17f7     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x17a024
  48e10c: e1a01006     	mov	r1, r6
  48e110: e2840030     	add	r0, r4, #48
  48e114: e28d2004     	add	r2, sp, #4
  48e118: ebfa17f3     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x17a034
  48e11c: e1a00004     	mov	r0, r4
  48e120: e28dd010     	add	sp, sp, #16
  48e124: e8bd8070     	pop	{r4, r5, r6, pc}
