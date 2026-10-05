
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00513d78 <PropertyMap::InitProperties()>:
  513d78: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  513d7c: e59fb250     	ldr	r11, [pc, #0x250]       @ 0x513fd4 <PropertyMap::InitProperties()+0x25c>
  513d80: e59f2250     	ldr	r2, [pc, #0x250]        @ 0x513fd8 <PropertyMap::InitProperties()+0x260>
  513d84: e59f3250     	ldr	r3, [pc, #0x250]        @ 0x513fdc <PropertyMap::InitProperties()+0x264>
  513d88: e24dd084     	sub	sp, sp, #132
  513d8c: e08fb00b     	add	r11, pc, r11
  513d90: e58d3004     	str	r3, [sp, #0x4]
  513d94: e79b3002     	ldr	r3, [r11, r2]
  513d98: e58d2008     	str	r2, [sp, #0x8]
  513d9c: e58d000c     	str	r0, [sp, #0xc]
  513da0: e5933000     	ldr	r3, [r3]
  513da4: e58d307c     	str	r3, [sp, #0x7c]
  513da8: ebfff367     	bl	0x510b4c <PropertyMap::GetThisClassName()> @ imm = #-0x3264
  513dac: e59d2004     	ldr	r2, [sp, #0x4]
  513db0: e1a0a000     	mov	r10, r0
  513db4: e79b8002     	ldr	r8, [r11, r2]
  513db8: e5984004     	ldr	r4, [r8, #0x4]
  513dbc: e3540000     	cmp	r4, #0
  513dc0: 0a000046     	beq	0x513ee0 <PropertyMap::InitProperties()+0x168> @ imm = #0x118
  513dc4: e28d704c     	add	r7, sp, #76
  513dc8: e28d9018     	add	r9, sp, #24
  513dcc: ea000001     	b	0x513dd8 <PropertyMap::InitProperties()+0x60> @ imm = #0x4
  513dd0: e1a08004     	mov	r8, r4
  513dd4: e1a04003     	mov	r4, r3
  513dd8: e1a0100a     	mov	r1, r10
  513ddc: e1a02009     	mov	r2, r9
  513de0: e1a00007     	mov	r0, r7
  513de4: ebf800c0     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x1ffd00
  513de8: e5943024     	ldr	r3, [r4, #0x24]
  513dec: e59d1060     	ldr	r1, [sp, #0x60]
  513df0: e5946020     	ldr	r6, [r4, #0x20]
  513df4: e59d505c     	ldr	r5, [sp, #0x5c]
  513df8: e1a00003     	mov	r0, r3
  513dfc: e0636006     	rsb	r6, r3, r6
  513e00: e0615005     	rsb	r5, r1, r5
  513e04: e1550006     	cmp	r5, r6
  513e08: b1a02005     	movlt	r2, r5
  513e0c: a1a02006     	movge	r2, r6
  513e10: ebf7e9f2     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0x205838
  513e14: e2503000     	subs	r3, r0, #0
  513e18: 1a000004     	bne	0x513e30 <PropertyMap::InitProperties()+0xb8> @ imm = #0x10
  513e1c: e1560005     	cmp	r6, r5
  513e20: b3e03000     	mvnlt	r3, #0
  513e24: ba000001     	blt	0x513e30 <PropertyMap::InitProperties()+0xb8> @ imm = #0x4
  513e28: d3a03000     	movle	r3, #0
  513e2c: c3a03001     	movgt	r3, #1
  513e30: e1a00007     	mov	r0, r7
  513e34: e58d3000     	str	r3, [sp]
  513e38: ebf81105     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x1fbbec
  513e3c: e59d3000     	ldr	r3, [sp]
  513e40: e3530000     	cmp	r3, #0
  513e44: b594300c     	ldrlt	r3, [r4, #0xc]
  513e48: a5943008     	ldrge	r3, [r4, #0x8]
  513e4c: b1a04008     	movlt	r4, r8
  513e50: e3530000     	cmp	r3, #0
  513e54: 1affffdd     	bne	0x513dd0 <PropertyMap::InitProperties()+0x58> @ imm = #-0x8c
  513e58: e59d2004     	ldr	r2, [sp, #0x4]
  513e5c: e1a08004     	mov	r8, r4
  513e60: e79b3002     	ldr	r3, [r11, r2]
  513e64: e1540003     	cmp	r4, r3
  513e68: 0a00001c     	beq	0x513ee0 <PropertyMap::InitProperties()+0x168> @ imm = #0x70
  513e6c: e28d5034     	add	r5, sp, #52
  513e70: e1a0100a     	mov	r1, r10
  513e74: e28d2014     	add	r2, sp, #20
  513e78: e1a00005     	mov	r0, r5
  513e7c: ebf8009a     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x1ffd98
  513e80: e59d3048     	ldr	r3, [sp, #0x48]
  513e84: e5941024     	ldr	r1, [r4, #0x24]
  513e88: e5946020     	ldr	r6, [r4, #0x20]
  513e8c: e59d7044     	ldr	r7, [sp, #0x44]
  513e90: e1a00003     	mov	r0, r3
  513e94: e0616006     	rsb	r6, r1, r6
  513e98: e0637007     	rsb	r7, r3, r7
  513e9c: e1560007     	cmp	r6, r7
  513ea0: b1a02006     	movlt	r2, r6
  513ea4: a1a02007     	movge	r2, r7
  513ea8: ebf7e9cc     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0x2058d0
  513eac: e2508000     	subs	r8, r0, #0
  513eb0: 1a000004     	bne	0x513ec8 <PropertyMap::InitProperties()+0x150> @ imm = #0x10
  513eb4: e1570006     	cmp	r7, r6
  513eb8: b3e08000     	mvnlt	r8, #0
  513ebc: ba000001     	blt	0x513ec8 <PropertyMap::InitProperties()+0x150> @ imm = #0x4
  513ec0: d3a08000     	movle	r8, #0
  513ec4: c3a08001     	movgt	r8, #1
  513ec8: e1a00005     	mov	r0, r5
  513ecc: ebf810e0     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x1fbc80
  513ed0: e3580000     	cmp	r8, #0
  513ed4: b59d3004     	ldrlt	r3, [sp, #0x4]
  513ed8: a1a08004     	movge	r8, r4
  513edc: b79b8003     	ldrlt	r8, [r11, r3]
  513ee0: e59d2004     	ldr	r2, [sp, #0x4]
  513ee4: e79b3002     	ldr	r3, [r11, r2]
  513ee8: e1580003     	cmp	r8, r3
  513eec: 0a000007     	beq	0x513f10 <PropertyMap::InitProperties()+0x198> @ imm = #0x1c
  513ef0: e59d2008     	ldr	r2, [sp, #0x8]
  513ef4: e79b3002     	ldr	r3, [r11, r2]
  513ef8: e59d207c     	ldr	r2, [sp, #0x7c]
  513efc: e5933000     	ldr	r3, [r3]
  513f00: e1520003     	cmp	r2, r3
  513f04: 1a000031     	bne	0x513fd0 <PropertyMap::InitProperties()+0x258> @ imm = #0xc4
  513f08: e28dd084     	add	sp, sp, #132
  513f0c: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  513f10: e28d4064     	add	r4, sp, #100
  513f14: e1a00004     	mov	r0, r4
  513f18: e3a01010     	mov	r1, #16
  513f1c: e58d4074     	str	r4, [sp, #0x74]
  513f20: e58d4078     	str	r4, [sp, #0x78]
  513f24: ebf7f5d4     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x2028b0
  513f28: e59d3074     	ldr	r3, [sp, #0x74]
  513f2c: e28d701c     	add	r7, sp, #28
  513f30: e3a05000     	mov	r5, #0
  513f34: e5c35000     	strb	r5, [r3]
  513f38: e1a01004     	mov	r1, r4
  513f3c: e1a00007     	mov	r0, r7
  513f40: ebf85e74     	bl	0x32b918 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&)> @ imm = #-0x1e8630
  513f44: e1a01005     	mov	r1, r5
  513f48: e3a00038     	mov	r0, #56
  513f4c: ebf7f187     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x2039e4
  513f50: e59f3088     	ldr	r3, [pc, #0x88]         @ 0x513fe0 <PropertyMap::InitProperties()+0x268>
  513f54: e59f6088     	ldr	r6, [pc, #0x88]         @ 0x513fe4 <PropertyMap::InitProperties()+0x26c>
  513f58: e1a05000     	mov	r5, r0
  513f5c: e79b3003     	ldr	r3, [r11, r3]
  513f60: e08f6006     	add	r6, pc, r6
  513f64: e1a01006     	mov	r1, r6
  513f68: e2833008     	add	r3, r3, #8
  513f6c: e28d2010     	add	r2, sp, #16
  513f70: e4803008     	str	r3, [r0], #8
  513f74: ebf8005c     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x1ffe90
  513f78: e59f3068     	ldr	r3, [pc, #0x68]         @ 0x513fe8 <PropertyMap::InitProperties()+0x270>
  513f7c: e1a00005     	mov	r0, r5
  513f80: e3a02004     	mov	r2, #4
  513f84: e79b3003     	ldr	r3, [r11, r3]
  513f88: e5852004     	str	r2, [r5, #0x4]
  513f8c: e1a01007     	mov	r1, r7
  513f90: e2833008     	add	r3, r3, #8
  513f94: e4803020     	str	r3, [r0], #32
  513f98: ebf85e5e     	bl	0x32b918 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&)> @ imm = #-0x1e8688
  513f9c: e1a01006     	mov	r1, r6
  513fa0: e1a02005     	mov	r2, r5
  513fa4: e59d000c     	ldr	r0, [sp, #0xc]
  513fa8: ebffff4d     	bl	0x513ce4 <PropertyMap::AddProperty(char const*, Property*)> @ imm = #-0x2cc
  513fac: e1a00007     	mov	r0, r7
  513fb0: ebf810a7     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x1fbd64
  513fb4: e1a00004     	mov	r0, r4
  513fb8: ebf810a5     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x1fbd6c
  513fbc: e59d000c     	ldr	r0, [sp, #0xc]
  513fc0: e5903000     	ldr	r3, [r0]
  513fc4: e1a0e00f     	mov	lr, pc
  513fc8: e593f000     	ldr	pc, [r3]
  513fcc: eaffffc7     	b	0x513ef0 <PropertyMap::InitProperties()+0x178> @ imm = #-0xe4
  513fd0: ebf7e8ce     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x205cc8
  513fd4: 04 0d 48 00  	.word	0x00480d04
  513fd8: ac 40 00 00  	.word	0x000040ac
  513fdc: 68 2a 00 00  	.word	0x00002a68
  513fe0: 30 23 00 00  	.word	0x00002330
  513fe4: 00 c5 3a 00  	.word	0x003ac500
  513fe8: 94 34 00 00  	.word	0x00003494
