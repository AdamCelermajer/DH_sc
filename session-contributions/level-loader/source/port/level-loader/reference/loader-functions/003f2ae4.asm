
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f2ae4 <Level::_LoadBatching()>:
  3f2ae4: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  3f2ae8: e59f422c     	ldr	r4, [pc, #0x22c]        @ 0x3f2d1c <Level::_LoadBatching()+0x238>
  3f2aec: e59f622c     	ldr	r6, [pc, #0x22c]        @ 0x3f2d20 <Level::_LoadBatching()+0x23c>
  3f2af0: e59f222c     	ldr	r2, [pc, #0x22c]        @ 0x3f2d24 <Level::_LoadBatching()+0x240>
  3f2af4: e08f4004     	add	r4, pc, r4
  3f2af8: e7943006     	ldr	r3, [r4, r6]
  3f2afc: e7948002     	ldr	r8, [r4, r2]
  3f2b00: e24dd020     	sub	sp, sp, #32
  3f2b04: e5933000     	ldr	r3, [r3]
  3f2b08: e1a05000     	mov	r5, r0
  3f2b0c: e1a00008     	mov	r0, r8
  3f2b10: e58d301c     	str	r3, [sp, #0x1c]
  3f2b14: ebfd135b     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0xbb294
  3f2b18: e59f1208     	ldr	r1, [pc, #0x208]        @ 0x3f2d28 <Level::_LoadBatching()+0x244>
  3f2b1c: e28d7004     	add	r7, sp, #4
  3f2b20: e1a0200d     	mov	r2, sp
  3f2b24: e08f1001     	add	r1, pc, r1
  3f2b28: e1a00007     	mov	r0, r7
  3f2b2c: ebfc856e     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0xdea48
  3f2b30: e1a00008     	mov	r0, r8
  3f2b34: e1a01007     	mov	r1, r7
  3f2b38: ebfd13d2     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0xbb0b8
  3f2b3c: e1a08000     	mov	r8, r0
  3f2b40: e1a00007     	mov	r0, r7
  3f2b44: ebfc8398     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xdf1a0
  3f2b48: e3580000     	cmp	r8, #0
  3f2b4c: 0a000006     	beq	0x3f2b6c <Level::_LoadBatching()+0x88> @ imm = #0x18
  3f2b50: e7943006     	ldr	r3, [r4, r6]
  3f2b54: e59d201c     	ldr	r2, [sp, #0x1c]
  3f2b58: e5933000     	ldr	r3, [r3]
  3f2b5c: e1520003     	cmp	r2, r3
  3f2b60: 1a00006c     	bne	0x3f2d18 <Level::_LoadBatching()+0x234> @ imm = #0x1b0
  3f2b64: e28dd020     	add	sp, sp, #32
  3f2b68: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  3f2b6c: e59f81b8     	ldr	r8, [pc, #0x1b8]        @ 0x3f2d2c <Level::_LoadBatching()+0x248>
  3f2b70: e7947008     	ldr	r7, [r4, r8]
  3f2b74: e5973010     	ldr	r3, [r7, #0x10]
  3f2b78: e593001c     	ldr	r0, [r3, #0x1c]
  3f2b7c: ebfd948f     	bl	0x357dc0 <SceneManager::clearRenderLists()> @ imm = #-0x9adc4
  3f2b80: e5973010     	ldr	r3, [r7, #0x10]
  3f2b84: e3a02001     	mov	r2, #1
  3f2b88: e593301c     	ldr	r3, [r3, #0x1c]
  3f2b8c: e5c3243c     	strb	r2, [r3, #0x43c]
  3f2b90: ebfe3aed     	bl	0x38174c <Device::IsHighPerformance()> @ imm = #-0x7144c
  3f2b94: e3500000     	cmp	r0, #0
  3f2b98: 1a000031     	bne	0x3f2c64 <Level::_LoadBatching()+0x180> @ imm = #0xc4
  3f2b9c: e5951038     	ldr	r1, [r5, #0x38]
  3f2ba0: e59f0188     	ldr	r0, [pc, #0x188]        @ 0x3f2d30 <Level::_LoadBatching()+0x24c>
  3f2ba4: e5d132b4     	ldrb	r3, [r1, #0x2b4]
  3f2ba8: e59122b0     	ldr	r2, [r1, #0x2b0]
  3f2bac: e08f0000     	add	r0, pc, r0
  3f2bb0: e59112ac     	ldr	r1, [r1, #0x2ac]
  3f2bb4: ebfc6cb2     	bl	0x30de84 <.plt+0x110>   @ imm = #-0xe4d38
  3f2bb8: e595a038     	ldr	r10, [r5, #0x38]
  3f2bbc: e59a02b0     	ldr	r0, [r10, #0x2b0]
  3f2bc0: ebfc705a     	bl	0x30ed30 <.plt+0xfbc>   @ imm = #-0xe3e98
  3f2bc4: e30b2852     	movw	r2, #0xb852
  3f2bc8: e30531eb     	movw	r3, #0x51eb
  3f2bcc: e348251e     	movt	r2, #0x851e
  3f2bd0: e3433ff0     	movt	r3, #0x3ff0
  3f2bd4: ebfc6fb6     	bl	0x30eab4 <.plt+0xd40>   @ imm = #-0xe4128
  3f2bd8: ebfc6f7c     	bl	0x30e9d0 <.plt+0xc5c>   @ imm = #-0xe4210
  3f2bdc: e5973010     	ldr	r3, [r7, #0x10]
  3f2be0: e1a09000     	mov	r9, r0
  3f2be4: e59a02ac     	ldr	r0, [r10, #0x2ac]
  3f2be8: e593301c     	ldr	r3, [r3, #0x1c]
  3f2bec: e593701c     	ldr	r7, [r3, #0x1c]
  3f2bf0: ebfc704e     	bl	0x30ed30 <.plt+0xfbc>   @ imm = #-0xe3ec8
  3f2bf4: e30b2852     	movw	r2, #0xb852
  3f2bf8: e30531eb     	movw	r3, #0x51eb
  3f2bfc: e3433ff0     	movt	r3, #0x3ff0
  3f2c00: e348251e     	movt	r2, #0x851e
  3f2c04: ebfc6faa     	bl	0x30eab4 <.plt+0xd40>   @ imm = #-0xe4158
  3f2c08: ebfc6f70     	bl	0x30e9d0 <.plt+0xc5c>   @ imm = #-0xe4240
  3f2c0c: e5879230     	str	r9, [r7, #0x230]
  3f2c10: e587022c     	str	r0, [r7, #0x22c]
  3f2c14: e5953038     	ldr	r3, [r5, #0x38]
  3f2c18: e5950158     	ldr	r0, [r5, #0x158]
  3f2c1c: e5d312b4     	ldrb	r1, [r3, #0x2b4]
  3f2c20: eb046c17     	bl	0x50dc84 <batch::BatchNodeCompiler::Compile(bool)> @ imm = #0x11b05c
  3f2c24: e7943008     	ldr	r3, [r4, r8]
  3f2c28: e3a01000     	mov	r1, #0
  3f2c2c: e5932010     	ldr	r2, [r3, #0x10]
  3f2c30: e592201c     	ldr	r2, [r2, #0x1c]
  3f2c34: e5c2143c     	strb	r1, [r2, #0x43c]
  3f2c38: e5952158     	ldr	r2, [r5, #0x158]
  3f2c3c: e5d21000     	ldrb	r1, [r2]
  3f2c40: e3510000     	cmp	r1, #0
  3f2c44: 1a000029     	bne	0x3f2cf0 <Level::_LoadBatching()+0x20c> @ imm = #0xa4
  3f2c48: e5923034     	ldr	r3, [r2, #0x34]
  3f2c4c: e3a01001     	mov	r1, #1
  3f2c50: e1a00003     	mov	r0, r3
  3f2c54: e5933000     	ldr	r3, [r3]
  3f2c58: e1a0e00f     	mov	lr, pc
  3f2c5c: e593f048     	ldr	pc, [r3, #0x48]
  3f2c60: eaffffba     	b	0x3f2b50 <Level::_LoadBatching()+0x6c> @ imm = #-0x118
  3f2c64: e5951038     	ldr	r1, [r5, #0x38]
  3f2c68: e59f00c4     	ldr	r0, [pc, #0xc4]         @ 0x3f2d34 <Level::_LoadBatching()+0x250>
  3f2c6c: e5d132a8     	ldrb	r3, [r1, #0x2a8]
  3f2c70: e59122a4     	ldr	r2, [r1, #0x2a4]
  3f2c74: e08f0000     	add	r0, pc, r0
  3f2c78: e59112a0     	ldr	r1, [r1, #0x2a0]
  3f2c7c: ebfc6c80     	bl	0x30de84 <.plt+0x110>   @ imm = #-0xe4e00
  3f2c80: e595a038     	ldr	r10, [r5, #0x38]
  3f2c84: e59a02a4     	ldr	r0, [r10, #0x2a4]
  3f2c88: ebfc7028     	bl	0x30ed30 <.plt+0xfbc>   @ imm = #-0xe3f60
  3f2c8c: e30b2852     	movw	r2, #0xb852
  3f2c90: e30531eb     	movw	r3, #0x51eb
  3f2c94: e348251e     	movt	r2, #0x851e
  3f2c98: e3433ff0     	movt	r3, #0x3ff0
  3f2c9c: ebfc6f84     	bl	0x30eab4 <.plt+0xd40>   @ imm = #-0xe41f0
  3f2ca0: ebfc6f4a     	bl	0x30e9d0 <.plt+0xc5c>   @ imm = #-0xe42d8
  3f2ca4: e5973010     	ldr	r3, [r7, #0x10]
  3f2ca8: e1a09000     	mov	r9, r0
  3f2cac: e59a02a0     	ldr	r0, [r10, #0x2a0]
  3f2cb0: e593301c     	ldr	r3, [r3, #0x1c]
  3f2cb4: e593701c     	ldr	r7, [r3, #0x1c]
  3f2cb8: ebfc701c     	bl	0x30ed30 <.plt+0xfbc>   @ imm = #-0xe3f90
  3f2cbc: e30b2852     	movw	r2, #0xb852
  3f2cc0: e30531eb     	movw	r3, #0x51eb
  3f2cc4: e348251e     	movt	r2, #0x851e
  3f2cc8: e3433ff0     	movt	r3, #0x3ff0
  3f2ccc: ebfc6f78     	bl	0x30eab4 <.plt+0xd40>   @ imm = #-0xe4220
  3f2cd0: ebfc6f3e     	bl	0x30e9d0 <.plt+0xc5c>   @ imm = #-0xe4308
  3f2cd4: e5879230     	str	r9, [r7, #0x230]
  3f2cd8: e587022c     	str	r0, [r7, #0x22c]
  3f2cdc: e5953038     	ldr	r3, [r5, #0x38]
  3f2ce0: e5950158     	ldr	r0, [r5, #0x158]
  3f2ce4: e5d312a8     	ldrb	r1, [r3, #0x2a8]
  3f2ce8: eb046be5     	bl	0x50dc84 <batch::BatchNodeCompiler::Compile(bool)> @ imm = #0x11af94
  3f2cec: eaffffcc     	b	0x3f2c24 <Level::_LoadBatching()+0x140> @ imm = #-0xd0
  3f2cf0: e5933010     	ldr	r3, [r3, #0x10]
  3f2cf4: e5921034     	ldr	r1, [r2, #0x34]
  3f2cf8: e593301c     	ldr	r3, [r3, #0x1c]
  3f2cfc: e5933004     	ldr	r3, [r3, #0x4]
  3f2d00: e1a00003     	mov	r0, r3
  3f2d04: e5933000     	ldr	r3, [r3]
  3f2d08: e1a0e00f     	mov	lr, pc
  3f2d0c: e593f05c     	ldr	pc, [r3, #0x5c]
  3f2d10: e5952158     	ldr	r2, [r5, #0x158]
  3f2d14: eaffffcb     	b	0x3f2c48 <Level::_LoadBatching()+0x164> @ imm = #-0xd4
  3f2d18: ebfc6d7c     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0xe4a10
  3f2d1c: 9c 1f 5a 00  	.word	0x005a1f9c
  3f2d20: ac 40 00 00  	.word	0x000040ac
  3f2d24: 84 08 00 00  	.word	0x00000884
  3f2d28: d4 3a 4d 00  	.word	0x004d3ad4
  3f2d2c: f4 37 00 00  	.word	0x000037f4
  3f2d30: fc 3a 4d 00  	.word	0x004d3afc
  3f2d34: 34 3a 4d 00  	.word	0x004d3a34
