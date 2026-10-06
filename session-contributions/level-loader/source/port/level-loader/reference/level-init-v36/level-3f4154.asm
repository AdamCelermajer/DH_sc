
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f4154 <Level::LoadTile(rnd::Tile*, int)>:
  3f4154: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3f4158: e59f3400     	ldr	r3, [pc, #0x400]        @ 0x3f4560 <Level::LoadTile(rnd::Tile*, int)+0x40c>
  3f415c: e24ddf4b     	sub	sp, sp, #300
  3f4160: e59f43fc     	ldr	r4, [pc, #0x3fc]        @ 0x3f4564 <Level::LoadTile(rnd::Tile*, int)+0x410>
  3f4164: e08f3003     	add	r3, pc, r3
  3f4168: e58d3018     	str	r3, [sp, #0x18]
  3f416c: e59de018     	ldr	lr, [sp, #0x18]
  3f4170: e59f33f0     	ldr	r3, [pc, #0x3f0]        @ 0x3f4568 <Level::LoadTile(rnd::Tile*, int)+0x414>
  3f4174: e58d401c     	str	r4, [sp, #0x1c]
  3f4178: e1a05001     	mov	r5, r1
  3f417c: e79ec003     	ldr	r12, [lr, r3]
  3f4180: e79e3004     	ldr	r3, [lr, r4]
  3f4184: e1a04000     	mov	r4, r0
  3f4188: e59c0000     	ldr	r0, [r12]
  3f418c: e5933000     	ldr	r3, [r3]
  3f4190: e1a06002     	mov	r6, r2
  3f4194: e3500002     	cmp	r0, #2
  3f4198: e58d3124     	str	r3, [sp, #0x124]
  3f419c: 03a03000     	moveq	r3, #0
  3f41a0: 05833000     	streq	r3, [r3]
  3f41a4: 0a000001     	beq	0x3f41b0 <Level::LoadTile(rnd::Tile*, int)+0x5c> @ imm = #0x4
  3f41a8: e3500001     	cmp	r0, #1
  3f41ac: 0a0000dc     	beq	0x3f4524 <Level::LoadTile(rnd::Tile*, int)+0x3d0> @ imm = #0x370
  3f41b0: e5950084     	ldr	r0, [r5, #0x84]
  3f41b4: ebfc69ea     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0xe5858
  3f41b8: e5958030     	ldr	r8, [r5, #0x30]
  3f41bc: e1a0a000     	mov	r10, r0
  3f41c0: e28dbf43     	add	r11, sp, #268
  3f41c4: e5980054     	ldr	r0, [r8, #0x54]
  3f41c8: e28d70f4     	add	r7, sp, #244
  3f41cc: e28d90dc     	add	r9, sp, #220
  3f41d0: e2400001     	sub	r0, r0, #1
  3f41d4: ebfc69e2     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0xe5878
  3f41d8: e3a0143f     	mov	r1, #1056964608
  3f41dc: ebfc6ae2     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xe5478
  3f41e0: e1a01000     	mov	r1, r0
  3f41e4: e1a0000a     	mov	r0, r10
  3f41e8: ebfc6a6d     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0xe564c
  3f41ec: e598104c     	ldr	r1, [r8, #0x4c]
  3f41f0: ebfc6add     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xe548c
  3f41f4: e5840160     	str	r0, [r4, #0x160]
  3f41f8: e5950088     	ldr	r0, [r5, #0x88]
  3f41fc: ebfc69d8     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0xe58a0
  3f4200: e595a030     	ldr	r10, [r5, #0x30]
  3f4204: e1a03000     	mov	r3, r0
  3f4208: e28d80c4     	add	r8, sp, #196
  3f420c: e59a0058     	ldr	r0, [r10, #0x58]
  3f4210: e58d300c     	str	r3, [sp, #0xc]
  3f4214: e2400001     	sub	r0, r0, #1
  3f4218: ebfc69d1     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0xe58bc
  3f421c: e3a0143f     	mov	r1, #1056964608
  3f4220: ebfc6ad1     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xe54bc
  3f4224: e59d300c     	ldr	r3, [sp, #0xc]
  3f4228: e1a01000     	mov	r1, r0
  3f422c: e1a00003     	mov	r0, r3
  3f4230: ebfc6a5b     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0xe5694
  3f4234: e59a1050     	ldr	r1, [r10, #0x50]
  3f4238: e28d20ac     	add	r2, sp, #172
  3f423c: e28d3094     	add	r3, sp, #148
  3f4240: e2811102     	add	r1, r1, #-2147483648
  3f4244: e58d2014     	str	r2, [sp, #0x14]
  3f4248: e58d3010     	str	r3, [sp, #0x10]
  3f424c: ebfc6ac6     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0xe54e8
  3f4250: e5840164     	str	r0, [r4, #0x164]
  3f4254: e595308c     	ldr	r3, [r5, #0x8c]
  3f4258: e1a00004     	mov	r0, r4
  3f425c: e1a01006     	mov	r1, r6
  3f4260: e5843168     	str	r3, [r4, #0x168]
  3f4264: ebffec03     	bl	0x3ef278 <Level::SetObjectModuleId(int)> @ imm = #-0x4ff4
  3f4268: e5951030     	ldr	r1, [r5, #0x30]
  3f426c: e59f22f8     	ldr	r2, [pc, #0x2f8]        @ 0x3f456c <Level::LoadTile(rnd::Tile*, int)+0x418>
  3f4270: e1a0000b     	mov	r0, r11
  3f4274: e281101c     	add	r1, r1, #28
  3f4278: e08f2002     	add	r2, pc, r2
  3f427c: ebfcfd92     	bl	0x3338cc <std::basic_string<char, std::char_traits<char>, std::allocator<char>> std::operator+<char, std::char_traits<char>, std::allocator<char>>(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, char const*)> @ imm = #-0xc09b8
  3f4280: e5951030     	ldr	r1, [r5, #0x30]
  3f4284: e59f22e4     	ldr	r2, [pc, #0x2e4]        @ 0x3f4570 <Level::LoadTile(rnd::Tile*, int)+0x41c>
  3f4288: e1a00007     	mov	r0, r7
  3f428c: e2811004     	add	r1, r1, #4
  3f4290: e08f2002     	add	r2, pc, r2
  3f4294: ebfcfd8c     	bl	0x3338cc <std::basic_string<char, std::char_traits<char>, std::allocator<char>> std::operator+<char, std::char_traits<char>, std::allocator<char>>(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, char const*)> @ imm = #-0xc09d0
  3f4298: e59d1108     	ldr	r1, [sp, #0x108]
  3f429c: e59d2104     	ldr	r2, [sp, #0x104]
  3f42a0: e1a0000b     	mov	r0, r11
  3f42a4: ebfc7156     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0xe3aa8
  3f42a8: e1a00007     	mov	r0, r7
  3f42ac: ebfc7dbe     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe0908
  3f42b0: e5951030     	ldr	r1, [r5, #0x30]
  3f42b4: e59f22b8     	ldr	r2, [pc, #0x2b8]        @ 0x3f4574 <Level::LoadTile(rnd::Tile*, int)+0x420>
  3f42b8: e1a00009     	mov	r0, r9
  3f42bc: e281101c     	add	r1, r1, #28
  3f42c0: e08f2002     	add	r2, pc, r2
  3f42c4: ebfcfd80     	bl	0x3338cc <std::basic_string<char, std::char_traits<char>, std::allocator<char>> std::operator+<char, std::char_traits<char>, std::allocator<char>>(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, char const*)> @ imm = #-0xc0a00
  3f42c8: e5951030     	ldr	r1, [r5, #0x30]
  3f42cc: e59f22a4     	ldr	r2, [pc, #0x2a4]        @ 0x3f4578 <Level::LoadTile(rnd::Tile*, int)+0x424>
  3f42d0: e1a00008     	mov	r0, r8
  3f42d4: e2811004     	add	r1, r1, #4
  3f42d8: e08f2002     	add	r2, pc, r2
  3f42dc: ebfcfd7a     	bl	0x3338cc <std::basic_string<char, std::char_traits<char>, std::allocator<char>> std::operator+<char, std::char_traits<char>, std::allocator<char>>(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, char const*)> @ imm = #-0xc0a18
  3f42e0: e59d10d8     	ldr	r1, [sp, #0xd8]
  3f42e4: e59d20d4     	ldr	r2, [sp, #0xd4]
  3f42e8: e1a00009     	mov	r0, r9
  3f42ec: ebfc7144     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0xe3af0
  3f42f0: e1a00008     	mov	r0, r8
  3f42f4: ebfc7dac     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe0950
  3f42f8: e5951030     	ldr	r1, [r5, #0x30]
  3f42fc: e59f2278     	ldr	r2, [pc, #0x278]        @ 0x3f457c <Level::LoadTile(rnd::Tile*, int)+0x428>
  3f4300: e59d0014     	ldr	r0, [sp, #0x14]
  3f4304: e281101c     	add	r1, r1, #28
  3f4308: e08f2002     	add	r2, pc, r2
  3f430c: ebfcfd6e     	bl	0x3338cc <std::basic_string<char, std::char_traits<char>, std::allocator<char>> std::operator+<char, std::char_traits<char>, std::allocator<char>>(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, char const*)> @ imm = #-0xc0a48
  3f4310: e5951064     	ldr	r1, [r5, #0x64]
  3f4314: e5952060     	ldr	r2, [r5, #0x60]
  3f4318: e59d0014     	ldr	r0, [sp, #0x14]
  3f431c: ebfc7138     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0xe3b20
  3f4320: e5951030     	ldr	r1, [r5, #0x30]
  3f4324: e59f2254     	ldr	r2, [pc, #0x254]        @ 0x3f4580 <Level::LoadTile(rnd::Tile*, int)+0x42c>
  3f4328: e59d0010     	ldr	r0, [sp, #0x10]
  3f432c: e281101c     	add	r1, r1, #28
  3f4330: e08f2002     	add	r2, pc, r2
  3f4334: ebfcfd64     	bl	0x3338cc <std::basic_string<char, std::char_traits<char>, std::allocator<char>> std::operator+<char, std::char_traits<char>, std::allocator<char>>(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, char const*)> @ imm = #-0xc0a70
  3f4338: e59d0010     	ldr	r0, [sp, #0x10]
  3f433c: e595107c     	ldr	r1, [r5, #0x7c]
  3f4340: e5952078     	ldr	r2, [r5, #0x78]
  3f4344: ebfc712e     	bl	0x310804 <std::string::_M_append(char const*, char const*)> @ imm = #-0xe3b48
  3f4348: e59f8234     	ldr	r8, [pc, #0x234]        @ 0x3f4584 <Level::LoadTile(rnd::Tile*, int)+0x430>
  3f434c: e28d707c     	add	r7, sp, #124
  3f4350: e28da030     	add	r10, sp, #48
  3f4354: e08f8008     	add	r8, pc, r8
  3f4358: e1a01008     	mov	r1, r8
  3f435c: e1a0200a     	mov	r2, r10
  3f4360: e1a00007     	mov	r0, r7
  3f4364: ebfc7f60     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe0280
  3f4368: e1a01009     	mov	r1, r9
  3f436c: e1a02007     	mov	r2, r7
  3f4370: e1a00004     	mov	r0, r4
  3f4374: ebfffdf1     	bl	0x3f3b40 <Level::LoadFile(std::string const&, std::string const&)> @ imm = #-0x83c
  3f4378: e1a03000     	mov	r3, r0
  3f437c: e1a00007     	mov	r0, r7
  3f4380: e58d300c     	str	r3, [sp, #0xc]
  3f4384: ebfc7d88     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe09e0
  3f4388: e59d300c     	ldr	r3, [sp, #0xc]
  3f438c: e3530000     	cmp	r3, #0
  3f4390: 0afffff0     	beq	0x3f4358 <Level::LoadTile(rnd::Tile*, int)+0x204> @ imm = #-0x40
  3f4394: e28d7064     	add	r7, sp, #100
  3f4398: e28da02c     	add	r10, sp, #44
  3f439c: e1a01008     	mov	r1, r8
  3f43a0: e1a0200a     	mov	r2, r10
  3f43a4: e1a00007     	mov	r0, r7
  3f43a8: ebfc7f4f     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe02c4
  3f43ac: e1a0100b     	mov	r1, r11
  3f43b0: e1a02007     	mov	r2, r7
  3f43b4: e1a00004     	mov	r0, r4
  3f43b8: ebfffde0     	bl	0x3f3b40 <Level::LoadFile(std::string const&, std::string const&)> @ imm = #-0x880
  3f43bc: e1a03000     	mov	r3, r0
  3f43c0: e1a00007     	mov	r0, r7
  3f43c4: e58d300c     	str	r3, [sp, #0xc]
  3f43c8: ebfc7d77     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe0a24
  3f43cc: e59d300c     	ldr	r3, [sp, #0xc]
  3f43d0: e3530000     	cmp	r3, #0
  3f43d4: 0afffff0     	beq	0x3f439c <Level::LoadTile(rnd::Tile*, int)+0x248> @ imm = #-0x40
  3f43d8: e5952064     	ldr	r2, [r5, #0x64]
  3f43dc: e5953060     	ldr	r3, [r5, #0x60]
  3f43e0: e1520003     	cmp	r2, r3
  3f43e4: 0a000010     	beq	0x3f442c <Level::LoadTile(rnd::Tile*, int)+0x2d8> @ imm = #0x40
  3f43e8: e28d704c     	add	r7, sp, #76
  3f43ec: e28da028     	add	r10, sp, #40
  3f43f0: e1a01008     	mov	r1, r8
  3f43f4: e1a0200a     	mov	r2, r10
  3f43f8: e1a00007     	mov	r0, r7
  3f43fc: ebfc7f3a     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe0318
  3f4400: e59d1014     	ldr	r1, [sp, #0x14]
  3f4404: e1a02007     	mov	r2, r7
  3f4408: e1a00004     	mov	r0, r4
  3f440c: ebfffdcb     	bl	0x3f3b40 <Level::LoadFile(std::string const&, std::string const&)> @ imm = #-0x8d4
  3f4410: e1a03000     	mov	r3, r0
  3f4414: e1a00007     	mov	r0, r7
  3f4418: e58d300c     	str	r3, [sp, #0xc]
  3f441c: ebfc7d62     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe0a78
  3f4420: e59d300c     	ldr	r3, [sp, #0xc]
  3f4424: e3530000     	cmp	r3, #0
  3f4428: 0afffff0     	beq	0x3f43f0 <Level::LoadTile(rnd::Tile*, int)+0x29c> @ imm = #-0x40
  3f442c: e595207c     	ldr	r2, [r5, #0x7c]
  3f4430: e5953078     	ldr	r3, [r5, #0x78]
  3f4434: e1520003     	cmp	r2, r3
  3f4438: 0a000012     	beq	0x3f4488 <Level::LoadTile(rnd::Tile*, int)+0x334> @ imm = #0x48
  3f443c: e59f8144     	ldr	r8, [pc, #0x144]        @ 0x3f4588 <Level::LoadTile(rnd::Tile*, int)+0x434>
  3f4440: e28d7034     	add	r7, sp, #52
  3f4444: e28da024     	add	r10, sp, #36
  3f4448: e08f8008     	add	r8, pc, r8
  3f444c: e1a01008     	mov	r1, r8
  3f4450: e1a0200a     	mov	r2, r10
  3f4454: e1a00007     	mov	r0, r7
  3f4458: ebfc7f23     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe0374
  3f445c: e59d1010     	ldr	r1, [sp, #0x10]
  3f4460: e1a02007     	mov	r2, r7
  3f4464: e1a00004     	mov	r0, r4
  3f4468: ebfffdb4     	bl	0x3f3b40 <Level::LoadFile(std::string const&, std::string const&)> @ imm = #-0x930
  3f446c: e1a03000     	mov	r3, r0
  3f4470: e1a00007     	mov	r0, r7
  3f4474: e58d300c     	str	r3, [sp, #0xc]
  3f4478: ebfc7d4b     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe0ad4
  3f447c: e59d300c     	ldr	r3, [sp, #0xc]
  3f4480: e3530000     	cmp	r3, #0
  3f4484: 0afffff0     	beq	0x3f444c <Level::LoadTile(rnd::Tile*, int)+0x2f8> @ imm = #-0x40
  3f4488: e3a03000     	mov	r3, #0
  3f448c: e3a08000     	mov	r8, #0
  3f4490: e5c4816c     	strb	r8, [r4, #0x16c]
  3f4494: e5843168     	str	r3, [r4, #0x168]
  3f4498: e5843160     	str	r3, [r4, #0x160]
  3f449c: e5843164     	str	r3, [r4, #0x164]
  3f44a0: e595300c     	ldr	r3, [r5, #0xc]
  3f44a4: e1530008     	cmp	r3, r8
  3f44a8: da00000b     	ble	0x3f44dc <Level::LoadTile(rnd::Tile*, int)+0x388> @ imm = #0x2c
  3f44ac: e1a07005     	mov	r7, r5
  3f44b0: e1a00006     	mov	r0, r6
  3f44b4: e2802001     	add	r2, r0, #1
  3f44b8: e5971010     	ldr	r1, [r7, #0x10]
  3f44bc: e1a00004     	mov	r0, r4
  3f44c0: ebffff23     	bl	0x3f4154 <Level::LoadTile(rnd::Tile*, int)> @ imm = #-0x374
  3f44c4: e595300c     	ldr	r3, [r5, #0xc]
  3f44c8: e2888001     	add	r8, r8, #1
  3f44cc: e2877004     	add	r7, r7, #4
  3f44d0: e1530008     	cmp	r3, r8
  3f44d4: cafffff6     	bgt	0x3f44b4 <Level::LoadTile(rnd::Tile*, int)+0x360> @ imm = #-0x28
  3f44d8: e1a06000     	mov	r6, r0
  3f44dc: e59d0010     	ldr	r0, [sp, #0x10]
  3f44e0: ebfc7d31     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe0b3c
  3f44e4: e59d0014     	ldr	r0, [sp, #0x14]
  3f44e8: ebfc7d2f     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe0b44
  3f44ec: e1a00009     	mov	r0, r9
  3f44f0: ebfc7d2d     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe0b4c
  3f44f4: e1a0000b     	mov	r0, r11
  3f44f8: ebfc7d2b     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe0b54
  3f44fc: e59d1018     	ldr	r1, [sp, #0x18]
  3f4500: e59d401c     	ldr	r4, [sp, #0x1c]
  3f4504: e59d2124     	ldr	r2, [sp, #0x124]
  3f4508: e1a00006     	mov	r0, r6
  3f450c: e7913004     	ldr	r3, [r1, r4]
  3f4510: e5933000     	ldr	r3, [r3]
  3f4514: e1520003     	cmp	r2, r3
  3f4518: 1a00000f     	bne	0x3f455c <Level::LoadTile(rnd::Tile*, int)+0x408> @ imm = #0x3c
  3f451c: e28ddf4b     	add	sp, sp, #300
  3f4520: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3f4524: e59d1018     	ldr	r1, [sp, #0x18]
  3f4528: e59f005c     	ldr	r0, [pc, #0x5c]         @ 0x3f458c <Level::LoadTile(rnd::Tile*, int)+0x438>
  3f452c: e59f205c     	ldr	r2, [pc, #0x5c]         @ 0x3f4590 <Level::LoadTile(rnd::Tile*, int)+0x43c>
  3f4530: e59f305c     	ldr	r3, [pc, #0x5c]         @ 0x3f4594 <Level::LoadTile(rnd::Tile*, int)+0x440>
  3f4534: e7910000     	ldr	r0, [r1, r0]
  3f4538: e59f1058     	ldr	r1, [pc, #0x58]         @ 0x3f4598 <Level::LoadTile(rnd::Tile*, int)+0x444>
  3f453c: e300ccc5     	movw	r12, #0xcc5
  3f4540: e08f2002     	add	r2, pc, r2
  3f4544: e08f1001     	add	r1, pc, r1
  3f4548: e08f3003     	add	r3, pc, r3
  3f454c: e28000a8     	add	r0, r0, #168
  3f4550: e58dc000     	str	r12, [sp]
  3f4554: ebfc66aa     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xe6558
  3f4558: eaffff14     	b	0x3f41b0 <Level::LoadTile(rnd::Tile*, int)+0x5c> @ imm = #-0x3b0
  3f455c: ebfc676b     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xe6254
  3f4560: 2c 09 5a 00  	.word	0x005a092c
  3f4564: ac 40 00 00  	.word	0x000040ac
  3f4568: c0 39 00 00  	.word	0x000039c0
  3f456c: a8 25 4d 00  	.word	0x004d25a8
  3f4570: 98 25 4d 00  	.word	0x004d2598
  3f4574: 70 25 4d 00  	.word	0x004d2570
  3f4578: 60 25 4d 00  	.word	0x004d2560
  3f457c: 38 25 4d 00  	.word	0x004d2538
  3f4580: 18 25 4d 00  	.word	0x004d2518
  3f4584: dc c0 4c 00  	.word	0x004cc0dc
  3f4588: e8 bf 4c 00  	.word	0x004cbfe8
  3f458c: c0 19 00 00  	.word	0x000019c0
  3f4590: 28 a0 4c 00  	.word	0x004ca028
  3f4594: c8 1f 4d 00  	.word	0x004d1fc8
  3f4598: 94 9e 4c 00  	.word	0x004c9e94
