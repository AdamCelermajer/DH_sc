
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048a3d4 <rnd::Exit::Exit(rnd::Direction const&, rnd::Block*, int, int, float, int, std::vector<std::string, std::allocator<std::string>>&)>:
  48a3d4: e92d4070     	push	{r4, r5, r6, lr}
  48a3d8: e5801014     	str	r1, [r0, #0x14]
  48a3dc: e5803008     	str	r3, [r0, #0x8]
  48a3e0: e59d3010     	ldr	r3, [sp, #0x10]
  48a3e4: e1a04000     	mov	r4, r0
  48a3e8: e1a05002     	mov	r5, r2
  48a3ec: e580300c     	str	r3, [r0, #0xc]
  48a3f0: e59d3014     	ldr	r3, [sp, #0x14]
  48a3f4: e5803010     	str	r3, [r0, #0x10]
  48a3f8: e59d3018     	ldr	r3, [sp, #0x18]
  48a3fc: e5803018     	str	r3, [r0, #0x18]
  48a400: e5842004     	str	r2, [r4, #0x4]
  48a404: e59d101c     	ldr	r1, [sp, #0x1c]
  48a408: e280001c     	add	r0, r0, #28
  48a40c: ebffffb7     	bl	0x48a2f0 <std::vector<std::string, std::allocator<std::string>>::vector(std::vector<std::string, std::allocator<std::string>> const&)> @ imm = #-0x124
  48a410: e3a03000     	mov	r3, #0
  48a414: e5843028     	str	r3, [r4, #0x28]
  48a418: e5953018     	ldr	r3, [r5, #0x18]
  48a41c: e1a00004     	mov	r0, r4
  48a420: e5843000     	str	r3, [r4]
  48a424: e8bd8070     	pop	{r4, r5, r6, pc}
