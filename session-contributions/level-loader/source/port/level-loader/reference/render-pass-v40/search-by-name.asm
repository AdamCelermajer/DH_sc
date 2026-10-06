
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0035a0e4 <SceneManager::SearchByName(glitch::scene::ISceneNode*, char const*, bool)>:
  35a0e4: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  35a0e8: e59f4090     	ldr	r4, [pc, #0x90]         @ 0x35a180 <SceneManager::SearchByName(glitch::scene::ISceneNode*, char const*, bool)+0x9c>
  35a0ec: e59f6090     	ldr	r6, [pc, #0x90]         @ 0x35a184 <SceneManager::SearchByName(glitch::scene::ISceneNode*, char const*, bool)+0xa0>
  35a0f0: e3520000     	cmp	r2, #0
  35a0f4: 13510000     	cmpne	r1, #0
  35a0f8: e08f4004     	add	r4, pc, r4
  35a0fc: e794c006     	ldr	r12, [r4, r6]
  35a100: e1a05001     	mov	r5, r1
  35a104: e24dd024     	sub	sp, sp, #36
  35a108: e59cc000     	ldr	r12, [r12]
  35a10c: 03a01000     	moveq	r1, #0
  35a110: 13a01001     	movne	r1, #1
  35a114: e1a0a000     	mov	r10, r0
  35a118: e1a08003     	mov	r8, r3
  35a11c: e58dc01c     	str	r12, [sp, #0x1c]
  35a120: 01a05001     	moveq	r5, r1
  35a124: 0a00000c     	beq	0x35a15c <SceneManager::SearchByName(glitch::scene::ISceneNode*, char const*, bool)+0x78> @ imm = #0x30
  35a128: e28d7004     	add	r7, sp, #4
  35a12c: e1a01002     	mov	r1, r2
  35a130: e1a00007     	mov	r0, r7
  35a134: e1a0200d     	mov	r2, sp
  35a138: ebfee7eb     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x46054
  35a13c: e1a01005     	mov	r1, r5
  35a140: e1a0000a     	mov	r0, r10
  35a144: e1a02007     	mov	r2, r7
  35a148: e1a03008     	mov	r3, r8
  35a14c: ebffe288     	bl	0x352b74 <SceneManager::SearchByName(glitch::scene::ISceneNode*, std::string const&, bool)> @ imm = #-0x75e0
  35a150: e1a05000     	mov	r5, r0
  35a154: e1a00007     	mov	r0, r7
  35a158: ebfef83d     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x41f0c
  35a15c: e7943006     	ldr	r3, [r4, r6]
  35a160: e59d201c     	ldr	r2, [sp, #0x1c]
  35a164: e1a00005     	mov	r0, r5
  35a168: e5933000     	ldr	r3, [r3]
  35a16c: e1520003     	cmp	r2, r3
  35a170: 1a000001     	bne	0x35a17c <SceneManager::SearchByName(glitch::scene::ISceneNode*, char const*, bool)+0x98> @ imm = #0x4
  35a174: e28dd024     	add	sp, sp, #36
  35a178: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  35a17c: ebfed063     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x4be74
  35a180: 98 a9 63 00  	.word	0x0063a998
  35a184: ac 40 00 00  	.word	0x000040ac
