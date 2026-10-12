
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
  3f80e8: e1a0100a     	mov	r1, r10
  3f80ec: e1a02008     	mov	r2, r8
  3f80f0: e1a00004     	mov	r0, r4
  3f80f4: ebffee91     	bl	0x3f3b40 <Level::LoadFile(std::string const&, std::string const&)> @ imm = #-0x45bc
  3f80f8: e1a03000     	mov	r3, r0
  3f80fc: e1a00008     	mov	r0, r8
  3f8100: e58d3010     	str	r3, [sp, #0x10]
  3f8104: ebfc6e28     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe4760
  3f8108: e1a0000a     	mov	r0, r10
  3f810c: ebfc6e26     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe4768
  3f8110: e59d3010     	ldr	r3, [sp, #0x10]
  3f8114: e3530000     	cmp	r3, #0
  3f8118: 0affffea     	beq	0x3f80c8 <Level::_LoadProcess()+0x1738> @ imm = #-0x58
  3f811c: e59d001c     	ldr	r0, [sp, #0x1c]
  3f8120: ebfc6e21     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe477c
  3f8124: e5943038     	ldr	r3, [r4, #0x38]
  3f8128: e3530000     	cmp	r3, #0
  3f812c: 1afffc20     	bne	0x3f71b4 <Level::_LoadProcess()+0x824> @ imm = #-0xf80
  3f8130: e7951007     	ldr	r1, [r5, r7]
  3f8134: e51f2718     	ldr	r2, [pc, #-0x718]       @ 0x3f7a24 <Level::_LoadProcess()+0x1094>
  3f8138: e51f3718     	ldr	r3, [pc, #-0x718]       @ 0x3f7a28 <Level::_LoadProcess()+0x1098>
  3f813c: e28d7098     	add	r7, sp, #152
  3f8140: e2477004     	sub	r7, r7, #4
  3f8144: e5911038     	ldr	r1, [r1, #0x38]
  3f8148: e3a0c001     	mov	r12, #1
  3f814c: e08f2002     	add	r2, pc, r2
  3f8150: e08f3003     	add	r3, pc, r3
  3f8154: e3a08000     	mov	r8, #0
  3f8158: e1a00007     	mov	r0, r7
  3f815c: e88d1100     	stm	sp, {r8, r12}
  3f8160: ebfd4d6f     	bl	0x34b724 <ObjectManager::Spawn(char const*, char const*, bool, bool)> @ imm = #-0xaca44
  3f8164: e1a01008     	mov	r1, r8
  3f8168: e1a00007     	mov	r0, r7
  3f816c: ebfd1f13     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xb83b4
  3f8170: e2501000     	subs	r1, r0, #0
  3f8174: 0a000002     	beq	0x3f8184 <Level::_LoadProcess()+0x17f4> @ imm = #0x8
  3f8178: e59130f4     	ldr	r3, [r1, #0xf4]
  3f817c: e3530004     	cmp	r3, #4
  3f8180: 0a000000     	beq	0x3f8188 <Level::_LoadProcess()+0x17f8> @ imm = #0x0
  3f8184: e3a01000     	mov	r1, #0
  3f8188: e1a00004     	mov	r0, r4
  3f818c: ebffe4de     	bl	0x3f150c <Level::SetLevelConfig(LevelConfig*)> @ imm = #-0x6c88
  3f8190: e5943038     	ldr	r3, [r4, #0x38]
  3f8194: eafffc06     	b	0x3f71b4 <Level::_LoadProcess()+0x824> @ imm = #-0xfe8
