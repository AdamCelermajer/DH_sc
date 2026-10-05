
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00394d34 <GameObject::SetVisualObject(char const*, char const*, bool)>:
  394d34: e3530000     	cmp	r3, #0
  394d38: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  394d3c: e1a04000     	mov	r4, r0
  394d40: e1a05001     	mov	r5, r1
  394d44: e1a06002     	mov	r6, r2
  394d48: 0a000027     	beq	0x394dec <GameObject::SetVisualObject(char const*, char const*, bool)+0xb8> @ imm = #0x9c
  394d4c: e3510000     	cmp	r1, #0
  394d50: e2808e29     	add	r8, r0, #656
  394d54: 0a000044     	beq	0x394e6c <GameObject::SetVisualObject(char const*, char const*, bool)+0x138> @ imm = #0x110
  394d58: e1a00005     	mov	r0, r5
  394d5c: ebfde43c     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x86f10
  394d60: e1a01005     	mov	r1, r5
  394d64: e0852000     	add	r2, r5, r0
  394d68: e1a00008     	mov	r0, r8
  394d6c: ebfdef1b     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x84394
  394d70: e3560000     	cmp	r6, #0
  394d74: 13550000     	cmpne	r5, #0
  394d78: e2847faa     	add	r7, r4, #680
  394d7c: 0a000029     	beq	0x394e28 <GameObject::SetVisualObject(char const*, char const*, bool)+0xf4> @ imm = #0xa4
  394d80: e1a00006     	mov	r0, r6
  394d84: ebfde432     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x86f38
  394d88: e0862000     	add	r2, r6, r0
  394d8c: e1a01006     	mov	r1, r6
  394d90: e1a00007     	mov	r0, r7
  394d94: ebfdef11     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x843bc
  394d98: e59422a0     	ldr	r2, [r4, #0x2a0]
  394d9c: e59432a4     	ldr	r3, [r4, #0x2a4]
  394da0: e1520003     	cmp	r2, r3
  394da4: 0a000029     	beq	0x394e50 <GameObject::SetVisualObject(char const*, char const*, bool)+0x11c> @ imm = #0xa4
  394da8: e3a01000     	mov	r1, #0
  394dac: e3a000ac     	mov	r0, #172
  394db0: ebfdedee     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x84848
  394db4: e1a03007     	mov	r3, r7
  394db8: e1a05000     	mov	r5, r0
  394dbc: e1a01004     	mov	r1, r4
  394dc0: e1a02008     	mov	r2, r8
  394dc4: eb037710     	bl	0x472a0c <VisualObject::VisualObject(GameObject*, std::string const&, std::string const&)> @ imm = #0xddc40
  394dc8: e5953008     	ldr	r3, [r5, #0x8]
  394dcc: e3530000     	cmp	r3, #0
  394dd0: 0a00002f     	beq	0x394e94 <GameObject::SetVisualObject(char const*, char const*, bool)+0x160> @ imm = #0xbc
  394dd4: e1a00004     	mov	r0, r4
  394dd8: e1a01005     	mov	r1, r5
  394ddc: ebfffd55     	bl	0x394338 <GameObject::SetVisualObject(VisualObject*)> @ imm = #-0xaac
  394de0: e5953008     	ldr	r3, [r5, #0x8]
  394de4: e5834204     	str	r4, [r3, #0x204]
  394de8: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  394dec: e3510000     	cmp	r1, #0
  394df0: 0a00001c     	beq	0x394e68 <GameObject::SetVisualObject(char const*, char const*, bool)+0x134> @ imm = #0x70
  394df4: e1a00001     	mov	r0, r1
  394df8: e59412a4     	ldr	r1, [r4, #0x2a4]
  394dfc: ebfde546     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x86ae8
  394e00: e3500000     	cmp	r0, #0
  394e04: 1a000015     	bne	0x394e60 <GameObject::SetVisualObject(char const*, char const*, bool)+0x12c> @ imm = #0x54
  394e08: e3560000     	cmp	r6, #0
  394e0c: 0a000004     	beq	0x394e24 <GameObject::SetVisualObject(char const*, char const*, bool)+0xf0> @ imm = #0x10
  394e10: e1a00006     	mov	r0, r6
  394e14: e59412bc     	ldr	r1, [r4, #0x2bc]
  394e18: ebfde53f     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x86b04
  394e1c: e3500000     	cmp	r0, #0
  394e20: 1a00000e     	bne	0x394e60 <GameObject::SetVisualObject(char const*, char const*, bool)+0x12c> @ imm = #0x38
  394e24: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  394e28: e59f6078     	ldr	r6, [pc, #0x78]         @ 0x394ea8 <GameObject::SetVisualObject(char const*, char const*, bool)+0x174>
  394e2c: e1a00007     	mov	r0, r7
  394e30: e08f6006     	add	r6, pc, r6
  394e34: e1a02006     	mov	r2, r6
  394e38: e1a01006     	mov	r1, r6
  394e3c: ebfdeee7     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x84464
  394e40: e59422a0     	ldr	r2, [r4, #0x2a0]
  394e44: e59432a4     	ldr	r3, [r4, #0x2a4]
  394e48: e1520003     	cmp	r2, r3
  394e4c: 1affffd5     	bne	0x394da8 <GameObject::SetVisualObject(char const*, char const*, bool)+0x74> @ imm = #-0xac
  394e50: e1a00004     	mov	r0, r4
  394e54: e3a01000     	mov	r1, #0
  394e58: e8bd41f0     	pop	{r4, r5, r6, r7, r8, lr}
  394e5c: eafffd35     	b	0x394338 <GameObject::SetVisualObject(VisualObject*)> @ imm = #-0xb2c
  394e60: e2848e29     	add	r8, r4, #656
  394e64: eaffffbb     	b	0x394d58 <GameObject::SetVisualObject(char const*, char const*, bool)+0x24> @ imm = #-0x114
  394e68: e2808e29     	add	r8, r0, #656
  394e6c: e59f5038     	ldr	r5, [pc, #0x38]         @ 0x394eac <GameObject::SetVisualObject(char const*, char const*, bool)+0x178>
  394e70: e1a00008     	mov	r0, r8
  394e74: e2847faa     	add	r7, r4, #680
  394e78: e08f5005     	add	r5, pc, r5
  394e7c: e1a02005     	mov	r2, r5
  394e80: e1a01005     	mov	r1, r5
  394e84: ebfdeed5     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x844ac
  394e88: e1a06005     	mov	r6, r5
  394e8c: e1a02005     	mov	r2, r5
  394e90: eaffffbd     	b	0x394d8c <GameObject::SetVisualObject(char const*, char const*, bool)+0x58> @ imm = #-0x10c
  394e94: e1a00005     	mov	r0, r5
  394e98: e5953000     	ldr	r3, [r5]
  394e9c: e1a0e00f     	mov	lr, pc
  394ea0: e593f004     	ldr	pc, [r3, #0x4]
  394ea4: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  394ea8: d8 69 53 00  	.word	0x005369d8
  394eac: 90 69 53 00  	.word	0x00536990
