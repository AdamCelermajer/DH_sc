
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048a380 <rnd::Exit::Exit(rnd::Direction const&, rnd::Block*, int, int, float, int, std::vector<std::string, std::allocator<std::string>>&)>:
  48a380: e92d4070     	push	{r4, r5, r6, lr}
  48a384: e5801014     	str	r1, [r0, #0x14]
  48a388: e5803008     	str	r3, [r0, #0x8]
  48a38c: e59d3010     	ldr	r3, [sp, #0x10]
  48a390: e1a04000     	mov	r4, r0
  48a394: e1a05002     	mov	r5, r2
  48a398: e580300c     	str	r3, [r0, #0xc]
  48a39c: e59d3014     	ldr	r3, [sp, #0x14]
  48a3a0: e5803010     	str	r3, [r0, #0x10]
  48a3a4: e59d3018     	ldr	r3, [sp, #0x18]
  48a3a8: e5803018     	str	r3, [r0, #0x18]
  48a3ac: e5842004     	str	r2, [r4, #0x4]
  48a3b0: e59d101c     	ldr	r1, [sp, #0x1c]
  48a3b4: e280001c     	add	r0, r0, #28
  48a3b8: ebffffcc     	bl	0x48a2f0 <std::vector<std::string, std::allocator<std::string>>::vector(std::vector<std::string, std::allocator<std::string>> const&)> @ imm = #-0xd0
  48a3bc: e3a03000     	mov	r3, #0
  48a3c0: e5843028     	str	r3, [r4, #0x28]
  48a3c4: e5953018     	ldr	r3, [r5, #0x18]
  48a3c8: e1a00004     	mov	r0, r4
  48a3cc: e5843000     	str	r3, [r4]
  48a3d0: e8bd8070     	pop	{r4, r5, r6, pc}
