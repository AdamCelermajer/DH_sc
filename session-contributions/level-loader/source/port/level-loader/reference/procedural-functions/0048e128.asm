
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048e128 <rnd::ListElem::ListElem(rnd::ListRule*)>:
  48e128: e92d4030     	push	{r4, r5, lr}
  48e12c: e59f504c     	ldr	r5, [pc, #0x4c]         @ 0x48e180 <rnd::ListElem::ListElem(rnd::ListRule*)+0x58>
  48e130: e24dd014     	sub	sp, sp, #20
  48e134: e1a04000     	mov	r4, r0
  48e138: e08f5005     	add	r5, pc, r5
  48e13c: e28d200c     	add	r2, sp, #12
  48e140: e4801004     	str	r1, [r0], #4
  48e144: e1a01005     	mov	r1, r5
  48e148: ebfa17e7     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x17a064
  48e14c: e1a01005     	mov	r1, r5
  48e150: e28d2008     	add	r2, sp, #8
  48e154: e284001c     	add	r0, r4, #28
  48e158: ebfa17e3     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x17a074
  48e15c: e1a01005     	mov	r1, r5
  48e160: e2840034     	add	r0, r4, #52
  48e164: e28d2004     	add	r2, sp, #4
  48e168: ebfa17df     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x17a084
  48e16c: e3a03064     	mov	r3, #100
  48e170: e584304c     	str	r3, [r4, #0x4c]
  48e174: e1a00004     	mov	r0, r4
  48e178: e28dd014     	add	sp, sp, #20
  48e17c: e8bd8030     	pop	{r4, r5, pc}
  48e180: d0 d6 43 00  	.word	0x0043d6d0
