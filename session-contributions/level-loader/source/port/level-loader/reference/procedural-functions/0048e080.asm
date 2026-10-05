
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048e080 <rnd::ListElem::ListElem()>:
  48e080: e92d4030     	push	{r4, r5, lr}
  48e084: e59f5050     	ldr	r5, [pc, #0x50]         @ 0x48e0dc <rnd::ListElem::ListElem()+0x5c>
  48e088: e24dd014     	sub	sp, sp, #20
  48e08c: e3a03000     	mov	r3, #0
  48e090: e08f5005     	add	r5, pc, r5
  48e094: e1a04000     	mov	r4, r0
  48e098: e1a01005     	mov	r1, r5
  48e09c: e4803004     	str	r3, [r0], #4
  48e0a0: e28d200c     	add	r2, sp, #12
  48e0a4: ebfa1810     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x179fc0
  48e0a8: e1a01005     	mov	r1, r5
  48e0ac: e28d2008     	add	r2, sp, #8
  48e0b0: e284001c     	add	r0, r4, #28
  48e0b4: ebfa180c     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x179fd0
  48e0b8: e1a01005     	mov	r1, r5
  48e0bc: e2840034     	add	r0, r4, #52
  48e0c0: e28d2004     	add	r2, sp, #4
  48e0c4: ebfa1808     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x179fe0
  48e0c8: e3a03064     	mov	r3, #100
  48e0cc: e584304c     	str	r3, [r4, #0x4c]
  48e0d0: e1a00004     	mov	r0, r4
  48e0d4: e28dd014     	add	sp, sp, #20
  48e0d8: e8bd8030     	pop	{r4, r5, pc}
  48e0dc: 78 d7 43 00  	.word	0x0043d778
