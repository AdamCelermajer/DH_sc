
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f5ea0 <Level::_LoadBatchMap()>:
  3f5ea0: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  3f5ea4: e59f50cc     	ldr	r5, [pc, #0xcc]         @ 0x3f5f78 <Level::_LoadBatchMap()+0xd8>
  3f5ea8: e59f80cc     	ldr	r8, [pc, #0xcc]         @ 0x3f5f7c <Level::_LoadBatchMap()+0xdc>
  3f5eac: e59f20cc     	ldr	r2, [pc, #0xcc]         @ 0x3f5f80 <Level::_LoadBatchMap()+0xe0>
  3f5eb0: e08f5005     	add	r5, pc, r5
  3f5eb4: e7953008     	ldr	r3, [r5, r8]
  3f5eb8: e7956002     	ldr	r6, [r5, r2]
  3f5ebc: e24dd020     	sub	sp, sp, #32
  3f5ec0: e5933000     	ldr	r3, [r3]
  3f5ec4: e1a07000     	mov	r7, r0
  3f5ec8: e1a00006     	mov	r0, r6
  3f5ecc: e58d301c     	str	r3, [sp, #0x1c]
  3f5ed0: ebfd066c     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xbe650
  3f5ed4: e59f10a8     	ldr	r1, [pc, #0xa8]         @ 0x3f5f84 <Level::_LoadBatchMap()+0xe4>
  3f5ed8: e28d4004     	add	r4, sp, #4
  3f5edc: e1a0200d     	mov	r2, sp
  3f5ee0: e08f1001     	add	r1, pc, r1
  3f5ee4: e1a00004     	mov	r0, r4
  3f5ee8: ebfc787f     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xe1e04
  3f5eec: e1a00006     	mov	r0, r6
  3f5ef0: e1a01004     	mov	r1, r4
  3f5ef4: ebfd06e3     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbe474
  3f5ef8: e1a06000     	mov	r6, r0
  3f5efc: e1a00004     	mov	r0, r4
  3f5f00: ebfc76a9     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xe255c
  3f5f04: e3560000     	cmp	r6, #0
  3f5f08: 1a000012     	bne	0x3f5f58 <Level::_LoadBatchMap()+0xb8> @ imm = #0x48
  3f5f0c: e5977158     	ldr	r7, [r7, #0x158]
  3f5f10: e3570000     	cmp	r7, #0
  3f5f14: 0a00000f     	beq	0x3f5f58 <Level::_LoadBatchMap()+0xb8> @ imm = #0x3c
  3f5f18: e5974010     	ldr	r4, [r7, #0x10]
  3f5f1c: e5976014     	ldr	r6, [r7, #0x14]
  3f5f20: e1540006     	cmp	r4, r6
  3f5f24: 0a00000b     	beq	0x3f5f58 <Level::_LoadBatchMap()+0xb8> @ imm = #0x2c
  3f5f28: e5941000     	ldr	r1, [r4]
  3f5f2c: e59132d8     	ldr	r3, [r1, #0x2d8]
  3f5f30: e3530000     	cmp	r3, #0
  3f5f34: 0a000004     	beq	0x3f5f4c <Level::_LoadBatchMap()+0xac> @ imm = #0x10
  3f5f38: e5932008     	ldr	r2, [r3, #0x8]
  3f5f3c: e3520000     	cmp	r2, #0
  3f5f40: 0a000001     	beq	0x3f5f4c <Level::_LoadBatchMap()+0xac> @ imm = #0x4
  3f5f44: e1a00007     	mov	r0, r7
  3f5f48: ebffff9f     	bl	0x3f5dcc <batch::BatchNodeCompiler::_MapMeshNode(GameObject*, glitch::scene::ISceneNode*)> @ imm = #-0x184
  3f5f4c: e2844004     	add	r4, r4, #4
  3f5f50: e1560004     	cmp	r6, r4
  3f5f54: 1afffff3     	bne	0x3f5f28 <Level::_LoadBatchMap()+0x88> @ imm = #-0x34
  3f5f58: e7953008     	ldr	r3, [r5, r8]
  3f5f5c: e59d201c     	ldr	r2, [sp, #0x1c]
  3f5f60: e5933000     	ldr	r3, [r3]
  3f5f64: e1520003     	cmp	r2, r3
  3f5f68: 1a000001     	bne	0x3f5f74 <Level::_LoadBatchMap()+0xd4> @ imm = #0x4
  3f5f6c: e28dd020     	add	sp, sp, #32
  3f5f70: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  3f5f74: ebfc60e5     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xe7c6c
  3f5f78: e0 eb 59 00  	.word	0x0059ebe0
  3f5f7c: ac 40 00 00  	.word	0x000040ac
  3f5f80: 84 08 00 00  	.word	0x00000884
  3f5f84: 18 07 4d 00  	.word	0x004d0718
