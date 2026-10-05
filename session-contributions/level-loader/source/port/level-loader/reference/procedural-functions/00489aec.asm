
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00489aec <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)>:
  489aec: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  489af0: e59f22d8     	ldr	r2, [pc, #0x2d8]        @ 0x489dd0 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x2e4>
  489af4: e24dd034     	sub	sp, sp, #52
  489af8: e58d102c     	str	r1, [sp, #0x2c]
  489afc: e08f2002     	add	r2, pc, r2
  489b00: e58d2004     	str	r2, [sp, #0x4]
  489b04: e58d000c     	str	r0, [sp, #0xc]
  489b08: e5913008     	ldr	r3, [r1, #0x8]
  489b0c: e59f42c0     	ldr	r4, [pc, #0x2c0]        @ 0x489dd4 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x2e8>
  489b10: e59fc2c0     	ldr	r12, [pc, #0x2c0]       @ 0x489dd8 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x2ec>
  489b14: e58d3024     	str	r3, [sp, #0x24]
  489b18: e58d4014     	str	r4, [sp, #0x14]
  489b1c: e58dc018     	str	r12, [sp, #0x18]
  489b20: e59d102c     	ldr	r1, [sp, #0x2c]
  489b24: e59d2024     	ldr	r2, [sp, #0x24]
  489b28: e1510002     	cmp	r1, r2
  489b2c: 0a0000a5     	beq	0x489dc8 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x2dc> @ imm = #0x294
  489b30: e59d000c     	ldr	r0, [sp, #0xc]
  489b34: e59d1024     	ldr	r1, [sp, #0x24]
  489b38: e590305c     	ldr	r3, [r0, #0x5c]
  489b3c: e5911014     	ldr	r1, [r1, #0x14]
  489b40: e3530000     	cmp	r3, #0
  489b44: e58d1020     	str	r1, [sp, #0x20]
  489b48: da000083     	ble	0x489d5c <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x270> @ imm = #0x20c
  489b4c: e3a02000     	mov	r2, #0
  489b50: e58d2028     	str	r2, [sp, #0x28]
  489b54: e591c05c     	ldr	r12, [r1, #0x5c]
  489b58: e1a09000     	mov	r9, r0
  489b5c: e35c0000     	cmp	r12, #0
  489b60: da000077     	ble	0x489d44 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x258> @ imm = #0x1dc
  489b64: e59d1028     	ldr	r1, [sp, #0x28]
  489b68: e3a0304b     	mov	r3, #75
  489b6c: e3a02000     	mov	r2, #0
  489b70: e0030193     	mul	r3, r3, r1
  489b74: e58d201c     	str	r2, [sp, #0x1c]
  489b78: e58d3010     	str	r3, [sp, #0x10]
  489b7c: e59db020     	ldr	r11, [sp, #0x20]
  489b80: e599607c     	ldr	r6, [r9, #0x7c]
  489b84: e5992080     	ldr	r2, [r9, #0x80]
  489b88: e0663002     	rsb	r3, r6, r2
  489b8c: e1a031c3     	asr	r3, r3, #3
  489b90: e59d401c     	ldr	r4, [sp, #0x1c]
  489b94: e0831103     	add	r1, r3, r3, lsl #2
  489b98: e3a00f4b     	mov	r0, #300
  489b9c: e0811201     	add	r1, r1, r1, lsl #4
  489ba0: e0000490     	mul	r0, r0, r4
  489ba4: e0811401     	add	r1, r1, r1, lsl #8
  489ba8: e59d4020     	ldr	r4, [sp, #0x20]
  489bac: e0811801     	add	r1, r1, r1, lsl #16
  489bb0: e2800060     	add	r0, r0, #96
  489bb4: e0833081     	add	r3, r3, r1, lsl #1
  489bb8: e0840000     	add	r0, r4, r0
  489bbc: e3530000     	cmp	r3, #0
  489bc0: e58d0008     	str	r0, [sp, #0x8]
  489bc4: 0a000056     	beq	0x489d24 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x238> @ imm = #0x158
  489bc8: e3a0a000     	mov	r10, #0
  489bcc: e58da000     	str	r10, [sp]
  489bd0: e59b507c     	ldr	r5, [r11, #0x7c]
  489bd4: e59b7080     	ldr	r7, [r11, #0x80]
  489bd8: e0653007     	rsb	r3, r5, r7
  489bdc: e1a031c3     	asr	r3, r3, #3
  489be0: e0831103     	add	r1, r3, r3, lsl #2
  489be4: e0811201     	add	r1, r1, r1, lsl #4
  489be8: e0811401     	add	r1, r1, r1, lsl #8
  489bec: e0811801     	add	r1, r1, r1, lsl #16
  489bf0: e0831081     	add	r1, r3, r1, lsl #1
  489bf4: e3510000     	cmp	r1, #0
  489bf8: 0a00003a     	beq	0x489ce8 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x1fc> @ imm = #0xe8
  489bfc: e3a08018     	mov	r8, #24
  489c00: e3a01000     	mov	r1, #0
  489c04: e00a0a98     	mul	r10, r8, r10
  489c08: e1a04001     	mov	r4, r1
  489c0c: ea000009     	b	0x489c38 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x14c> @ imm = #0x24
  489c10: e0653007     	rsb	r3, r5, r7
  489c14: e1a031c3     	asr	r3, r3, #3
  489c18: e1a01004     	mov	r1, r4
  489c1c: e0832103     	add	r2, r3, r3, lsl #2
  489c20: e0822202     	add	r2, r2, r2, lsl #4
  489c24: e0822402     	add	r2, r2, r2, lsl #8
  489c28: e0822802     	add	r2, r2, r2, lsl #16
  489c2c: e0832082     	add	r2, r3, r2, lsl #1
  489c30: e1520004     	cmp	r2, r4
  489c34: 9a00002a     	bls	0x489ce4 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x1f8> @ imm = #0xa8
  489c38: e086200a     	add	r2, r6, r10
  489c3c: e5923010     	ldr	r3, [r2, #0x10]
  489c40: e5920014     	ldr	r0, [r2, #0x14]
  489c44: e0215198     	mla	r1, r8, r1, r5
  489c48: e1500003     	cmp	r0, r3
  489c4c: e2844001     	add	r4, r4, #1
  489c50: e0602003     	rsb	r2, r0, r3
  489c54: 0affffed     	beq	0x489c10 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x124> @ imm = #-0x4c
  489c58: e5913010     	ldr	r3, [r1, #0x10]
  489c5c: e5911014     	ldr	r1, [r1, #0x14]
  489c60: e0613003     	rsb	r3, r1, r3
  489c64: e1520003     	cmp	r2, r3
  489c68: 1affffe8     	bne	0x489c10 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x124> @ imm = #-0x60
  489c6c: ebfa125b     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0x17b694
  489c70: e3500000     	cmp	r0, #0
  489c74: 1affffe5     	bne	0x489c10 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x124> @ imm = #-0x6c
  489c78: e59b2074     	ldr	r2, [r11, #0x74]
  489c7c: e59d0004     	ldr	r0, [sp, #0x4]
  489c80: e59dc018     	ldr	r12, [sp, #0x18]
  489c84: e59d3014     	ldr	r3, [sp, #0x14]
  489c88: e790100c     	ldr	r1, [r0, r12]
  489c8c: e5920000     	ldr	r0, [r2]
  489c90: e59dc004     	ldr	r12, [sp, #0x4]
  489c94: e7911100     	ldr	r1, [r1, r0, lsl #2]
  489c98: e79c2003     	ldr	r2, [r12, r3]
  489c9c: e599c074     	ldr	r12, [r9, #0x74]
  489ca0: e7922201     	ldr	r2, [r2, r1, lsl #4]
  489ca4: e59c3000     	ldr	r3, [r12]
  489ca8: e1530002     	cmp	r3, r2
  489cac: 1affffd7     	bne	0x489c10 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x124> @ imm = #-0xa4
  489cb0: e5993088     	ldr	r3, [r9, #0x88]
  489cb4: e59d0010     	ldr	r0, [sp, #0x10]
  489cb8: e59d100c     	ldr	r1, [sp, #0xc]
  489cbc: e59dc008     	ldr	r12, [sp, #0x8]
  489cc0: e0802003     	add	r2, r0, r3
  489cc4: e0812102     	add	r2, r1, r2, lsl #2
  489cc8: e2833001     	add	r3, r3, #1
  489ccc: e582c08c     	str	r12, [r2, #0x8c]
  489cd0: e5893088     	str	r3, [r9, #0x88]
  489cd4: e59b507c     	ldr	r5, [r11, #0x7c]
  489cd8: e59b7080     	ldr	r7, [r11, #0x80]
  489cdc: e599607c     	ldr	r6, [r9, #0x7c]
  489ce0: eaffffca     	b	0x489c10 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x124> @ imm = #-0xd8
  489ce4: e5992080     	ldr	r2, [r9, #0x80]
  489ce8: e0663002     	rsb	r3, r6, r2
  489cec: e1a031c3     	asr	r3, r3, #3
  489cf0: e59d0000     	ldr	r0, [sp]
  489cf4: e0831103     	add	r1, r3, r3, lsl #2
  489cf8: e0811201     	add	r1, r1, r1, lsl #4
  489cfc: e2800001     	add	r0, r0, #1
  489d00: e0811401     	add	r1, r1, r1, lsl #8
  489d04: e58d0000     	str	r0, [sp]
  489d08: e0811801     	add	r1, r1, r1, lsl #16
  489d0c: e1a0a000     	mov	r10, r0
  489d10: e0831081     	add	r1, r3, r1, lsl #1
  489d14: e1500001     	cmp	r0, r1
  489d18: 3affffae     	blo	0x489bd8 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0xec> @ imm = #-0x148
  489d1c: e59d1020     	ldr	r1, [sp, #0x20]
  489d20: e591c05c     	ldr	r12, [r1, #0x5c]
  489d24: e59d301c     	ldr	r3, [sp, #0x1c]
  489d28: e28bbf4b     	add	r11, r11, #300
  489d2c: e2833001     	add	r3, r3, #1
  489d30: e15c0003     	cmp	r12, r3
  489d34: e58d301c     	str	r3, [sp, #0x1c]
  489d38: caffff92     	bgt	0x489b88 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x9c> @ imm = #-0x1b8
  489d3c: e59d400c     	ldr	r4, [sp, #0xc]
  489d40: e594305c     	ldr	r3, [r4, #0x5c]
  489d44: e59d0028     	ldr	r0, [sp, #0x28]
  489d48: e2899f4b     	add	r9, r9, #300
  489d4c: e2800001     	add	r0, r0, #1
  489d50: e1530000     	cmp	r3, r0
  489d54: e58d0028     	str	r0, [sp, #0x28]
  489d58: caffff7f     	bgt	0x489b5c <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x70> @ imm = #-0x204
  489d5c: e59d4024     	ldr	r4, [sp, #0x24]
  489d60: e594300c     	ldr	r3, [r4, #0xc]
  489d64: e3530000     	cmp	r3, #0
  489d68: 1a000001     	bne	0x489d74 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x288> @ imm = #0x4
  489d6c: ea000005     	b	0x489d88 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x29c> @ imm = #0x14
  489d70: e1a03002     	mov	r3, r2
  489d74: e5932008     	ldr	r2, [r3, #0x8]
  489d78: e3520000     	cmp	r2, #0
  489d7c: 1afffffb     	bne	0x489d70 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x284> @ imm = #-0x14
  489d80: e58d3024     	str	r3, [sp, #0x24]
  489d84: eaffff65     	b	0x489b20 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x34> @ imm = #-0x26c
  489d88: e59dc024     	ldr	r12, [sp, #0x24]
  489d8c: e59c3004     	ldr	r3, [r12, #0x4]
  489d90: e1a0200c     	mov	r2, r12
  489d94: ea000001     	b	0x489da0 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x2b4> @ imm = #0x4
  489d98: e1a02003     	mov	r2, r3
  489d9c: e5933004     	ldr	r3, [r3, #0x4]
  489da0: e593100c     	ldr	r1, [r3, #0xc]
  489da4: e1510002     	cmp	r1, r2
  489da8: 0afffffa     	beq	0x489d98 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x2ac> @ imm = #-0x18
  489dac: e58d2024     	str	r2, [sp, #0x24]
  489db0: e592200c     	ldr	r2, [r2, #0xc]
  489db4: e59d0024     	ldr	r0, [sp, #0x24]
  489db8: e1530002     	cmp	r3, r2
  489dbc: 01a03000     	moveq	r3, r0
  489dc0: e58d3024     	str	r3, [sp, #0x24]
  489dc4: eaffff55     	b	0x489b20 <rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr, std::allocator<std::pair<char const* const, rnd::Block*>>> const&)+0x34> @ imm = #-0x2ac
  489dc8: e28dd034     	add	sp, sp, #52
  489dcc: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  489dd0: 94 af 50 00  	.word	0x0050af94
  489dd4: fc 43 00 00  	.word	0x000043fc
  489dd8: b8 1b 00 00  	.word	0x00001bb8
