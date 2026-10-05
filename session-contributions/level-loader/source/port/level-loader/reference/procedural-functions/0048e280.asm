
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048e280 <rnd::ListElem::ListElem(rnd::ListElem const&)>:
  48e280: e92d4070     	push	{r4, r5, r6, lr}
  48e284: e1a05001     	mov	r5, r1
  48e288: e4913004     	ldr	r3, [r1], #4
  48e28c: e1a04000     	mov	r4, r0
  48e290: e4803004     	str	r3, [r0], #4
  48e294: ebfa759f     	bl	0x32b918 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&)> @ imm = #-0x162984
  48e298: e285101c     	add	r1, r5, #28
  48e29c: e284001c     	add	r0, r4, #28
  48e2a0: ebfa759c     	bl	0x32b918 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&)> @ imm = #-0x162990
  48e2a4: e2840034     	add	r0, r4, #52
  48e2a8: e2851034     	add	r1, r5, #52
  48e2ac: ebfa7599     	bl	0x32b918 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&)> @ imm = #-0x16299c
  48e2b0: e595304c     	ldr	r3, [r5, #0x4c]
  48e2b4: e1a00004     	mov	r0, r4
  48e2b8: e584304c     	str	r3, [r4, #0x4c]
  48e2bc: e8bd8070     	pop	{r4, r5, r6, pc}
