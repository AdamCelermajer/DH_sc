
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:
  3f8070: e51f7660     	ldr	r7, [pc, #-0x660]       @ 0x3f7a18 <Level::_LoadProcess()+0x1088>
  3f8074: e7953007     	ldr	r3, [r5, r7]
  3f8078: e59310cc     	ldr	r1, [r3, #0xcc]
  3f807c: e59320d0     	ldr	r2, [r3, #0xd0]
  3f8080: e1510002     	cmp	r1, r2
  3f8084: 0a000029     	beq	0x3f8130 <Level::_LoadProcess()+0x17a0> @ imm = #0xa4
  3f8088: e59310e8     	ldr	r1, [r3, #0xe8]
  3f808c: e28d2f45     	add	r2, sp, #276
  3f8090: e28d0feb     	add	r0, sp, #940
  3f8094: e58d001c     	str	r0, [sp, #0x1c]
  3f8098: ebfc7013     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3fb4
  3f809c: e51f1688     	ldr	r1, [pc, #-0x688]       @ 0x3f7a1c <Level::_LoadProcess()+0x108c>
  3f80a0: e51fb688     	ldr	r11, [pc, #-0x688]      @ 0x3f7a20 <Level::_LoadProcess()+0x1090>
  3f80a4: e59d001c     	ldr	r0, [sp, #0x1c]
  3f80a8: e08f1001     	add	r1, pc, r1
  3f80ac: ebffe6b3     	bl	0x3f1b80 <std::string::append(char const*)> @ imm = #-0x6534
  3f80b0: e28d2f43     	add	r2, sp, #268
  3f80b4: e28dafe5     	add	r10, sp, #916
  3f80b8: e28d9e11     	add	r9, sp, #272
  3f80bc: e28d8fdf     	add	r8, sp, #892
  3f80c0: e08fb00b     	add	r11, pc, r11
  3f80c4: e58d2018     	str	r2, [sp, #0x18]
  3f80c8: e59d13c0     	ldr	r1, [sp, #0x3c0]
  3f80cc: e1a02009     	mov	r2, r9
  3f80d0: e1a0000a     	mov	r0, r10
  3f80d4: ebfc7004     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe3ff0
  3f80d8: e1a0100b     	mov	r1, r11
  3f80dc: e59d2018     	ldr	r2, [sp, #0x18]
  3f80e0: e1a00008     	mov	r0, r8
  3f80e4: ebfc7000     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe4000
