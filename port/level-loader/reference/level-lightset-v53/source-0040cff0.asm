
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040cff0 <LightSetManager::InitLightFilter(std::vector<bool, std::allocator<bool>>&, bool)>:
  40cff0: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  40cff4: e9911048     	ldmib	r1, {r3, r6, r12}
  40cff8: e5917000     	ldr	r7, [r1]
  40cffc: e1a05002     	mov	r5, r2
  40d000: e063200c     	rsb	r2, r3, r12
  40d004: e0679006     	rsb	r9, r7, r6
  40d008: e0822189     	add	r2, r2, r9, lsl #3
  40d00c: e3520005     	cmp	r2, #5
  40d010: e24dd0b0     	sub	sp, sp, #176
  40d014: e1a04001     	mov	r4, r1
  40d018: 8a000066     	bhi	0x40d1b8 <LightSetManager::InitLightFilter(std::vector<bool, std::allocator<bool>>&, bool)+0x1c8> @ imm = #0x198
  40d01c: e2728005     	rsbs	r8, r2, #5
  40d020: e58dc078     	str	r12, [sp, #0x78]
  40d024: e58d6074     	str	r6, [sp, #0x74]
  40d028: 0a000045     	beq	0x40d144 <LightSetManager::InitLightFilter(std::vector<bool, std::allocator<bool>>&, bool)+0x154> @ imm = #0x114
  40d02c: e5911010     	ldr	r1, [r1, #0x10]
  40d030: e0677001     	rsb	r7, r7, r1
  40d034: e0633187     	rsb	r3, r3, r7, lsl #3
  40d038: e0623003     	rsb	r3, r2, r3
  40d03c: e1580003     	cmp	r8, r3
  40d040: 8a000078     	bhi	0x40d228 <LightSetManager::InitLightFilter(std::vector<bool, std::allocator<bool>>&, bool)+0x238> @ imm = #0x1e0
  40d044: e28d7054     	add	r7, sp, #84
  40d048: e1a00007     	mov	r0, r7
  40d04c: e2841008     	add	r1, r4, #8
  40d050: e1a02008     	mov	r2, r8
  40d054: e58d605c     	str	r6, [sp, #0x5c]
  40d058: e58dc060     	str	r12, [sp, #0x60]
  40d05c: e58d6064     	str	r6, [sp, #0x64]
  40d060: e58dc068     	str	r12, [sp, #0x68]
  40d064: ebfffe3f     	bl	0x40c968 <std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>::operator+(int) const> @ imm = #-0x704
  40d068: e28dc0ac     	add	r12, sp, #172
  40d06c: e1a03007     	mov	r3, r7
  40d070: e28d0008     	add	r0, sp, #8
  40d074: e28d1064     	add	r1, sp, #100
  40d078: e28d205c     	add	r2, sp, #92
  40d07c: e58dc000     	str	r12, [sp]
  40d080: e3a0c000     	mov	r12, #0
  40d084: e58dc004     	str	r12, [sp, #0x4]
  40d088: ebfffc3c     	bl	0x40c180 <std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*> std::priv::__copy_backward<std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>, std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>, int>(std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>, std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>, std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>, std::random_access_iterator_tag const&, int*)> @ imm = #-0xf10
  40d08c: e1a02008     	mov	r2, r8
  40d090: e28d004c     	add	r0, sp, #76
  40d094: e28d1074     	add	r1, sp, #116
  40d098: e59d6078     	ldr	r6, [sp, #0x78]
  40d09c: e59d7074     	ldr	r7, [sp, #0x74]
  40d0a0: ebfffe30     	bl	0x40c968 <std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>::operator+(int) const> @ imm = #-0x740
  40d0a4: e59d2050     	ldr	r2, [sp, #0x50]
  40d0a8: e59d304c     	ldr	r3, [sp, #0x4c]
  40d0ac: e0662002     	rsb	r2, r6, r2
  40d0b0: e0673003     	rsb	r3, r7, r3
  40d0b4: e0823183     	add	r3, r2, r3, lsl #3
  40d0b8: e3530000     	cmp	r3, #0
  40d0bc: da000009     	ble	0x40d0e8 <LightSetManager::InitLightFilter(std::vector<bool, std::allocator<bool>>&, bool)+0xf8> @ imm = #0x24
  40d0c0: e3a01001     	mov	r1, #1
  40d0c4: e5972000     	ldr	r2, [r7]
  40d0c8: e356001f     	cmp	r6, #31
  40d0cc: e1c22611     	bic	r2, r2, r1, lsl r6
  40d0d0: 12866001     	addne	r6, r6, #1
  40d0d4: e5872000     	str	r2, [r7]
  40d0d8: 03a06000     	moveq	r6, #0
  40d0dc: 02877004     	addeq	r7, r7, #4
  40d0e0: e2533001     	subs	r3, r3, #1
  40d0e4: 1afffff6     	bne	0x40d0c4 <LightSetManager::InitLightFilter(std::vector<bool, std::allocator<bool>>&, bool)+0xd4> @ imm = #-0x28
  40d0e8: e594300c     	ldr	r3, [r4, #0xc]
  40d0ec: e5941008     	ldr	r1, [r4, #0x8]
  40d0f0: e0888003     	add	r8, r8, r3
  40d0f4: e1a03fc8     	asr	r3, r8, #31
  40d0f8: e3580000     	cmp	r8, #0
  40d0fc: e1a03da3     	lsr	r3, r3, #27
  40d100: e288201f     	add	r2, r8, #31
  40d104: a1a02008     	movge	r2, r8
  40d108: e0888003     	add	r8, r8, r3
  40d10c: e1a022c2     	asr	r2, r2, #5
  40d110: e208801f     	and	r8, r8, #31
  40d114: e0588003     	subs	r8, r8, r3
  40d118: e0812102     	add	r2, r1, r2, lsl #2
  40d11c: 45943004     	ldrmi	r3, [r4, #0x4]
  40d120: 45947000     	ldrmi	r7, [r4]
  40d124: 55943004     	ldrpl	r3, [r4, #0x4]
  40d128: 55947000     	ldrpl	r7, [r4]
  40d12c: e5842008     	str	r2, [r4, #0x8]
  40d130: 42888020     	addmi	r8, r8, #32
  40d134: 42422004     	submi	r2, r2, #4
  40d138: 4584800c     	strmi	r8, [r4, #0xc]
  40d13c: 45842008     	strmi	r2, [r4, #0x8]
  40d140: 5584800c     	strpl	r8, [r4, #0xc]
  40d144: e3a02000     	mov	r2, #0
  40d148: e3a0c001     	mov	r12, #1
  40d14c: e0823003     	add	r3, r2, r3
  40d150: e1a01fc3     	asr	r1, r3, #31
  40d154: e3530000     	cmp	r3, #0
  40d158: e1a01da1     	lsr	r1, r1, #27
  40d15c: e283001f     	add	r0, r3, #31
  40d160: a1a00003     	movge	r0, r3
  40d164: e0833001     	add	r3, r3, r1
  40d168: e203301f     	and	r3, r3, #31
  40d16c: e0533001     	subs	r3, r3, r1
  40d170: 42833020     	addmi	r3, r3, #32
  40d174: e1a0331c     	lsl	r3, r12, r3
  40d178: e1a002c0     	asr	r0, r0, #5
  40d17c: e2822001     	add	r2, r2, #1
  40d180: e0877100     	add	r7, r7, r0, lsl #2
  40d184: 42477004     	submi	r7, r7, #4
  40d188: e5971000     	ldr	r1, [r7]
  40d18c: e3550000     	cmp	r5, #0
  40d190: 11813003     	orrne	r3, r1, r3
  40d194: 01c13003     	biceq	r3, r1, r3
  40d198: e3520005     	cmp	r2, #5
  40d19c: e5873000     	str	r3, [r7]
  40d1a0: 0a000002     	beq	0x40d1b0 <LightSetManager::InitLightFilter(std::vector<bool, std::allocator<bool>>&, bool)+0x1c0> @ imm = #0x8
  40d1a4: e5943004     	ldr	r3, [r4, #0x4]
  40d1a8: e5947000     	ldr	r7, [r4]
  40d1ac: eaffffe6     	b	0x40d14c <LightSetManager::InitLightFilter(std::vector<bool, std::allocator<bool>>&, bool)+0x15c> @ imm = #-0x68
  40d1b0: e28dd0b0     	add	sp, sp, #176
  40d1b4: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  40d1b8: e28d00a4     	add	r0, sp, #164
  40d1bc: e28d109c     	add	r1, sp, #156
  40d1c0: e3a02005     	mov	r2, #5
  40d1c4: e58d709c     	str	r7, [sp, #0x9c]
  40d1c8: e58d30a0     	str	r3, [sp, #0xa0]
  40d1cc: ebfffde5     	bl	0x40c968 <std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>::operator+(int) const> @ imm = #-0x86c
  40d1d0: e59d60a4     	ldr	r6, [sp, #0xa4]
  40d1d4: e594c008     	ldr	r12, [r4, #0x8]
  40d1d8: e594e00c     	ldr	lr, [r4, #0xc]
  40d1dc: e58d6094     	str	r6, [sp, #0x94]
  40d1e0: e59d60a8     	ldr	r6, [sp, #0xa8]
  40d1e4: e28d1084     	add	r1, sp, #132
  40d1e8: e28d208c     	add	r2, sp, #140
  40d1ec: e28d3094     	add	r3, sp, #148
  40d1f0: e28d007c     	add	r0, sp, #124
  40d1f4: e58dc08c     	str	r12, [sp, #0x8c]
  40d1f8: e58de090     	str	lr, [sp, #0x90]
  40d1fc: e58d6098     	str	r6, [sp, #0x98]
  40d200: e58de088     	str	lr, [sp, #0x88]
  40d204: e58dc084     	str	r12, [sp, #0x84]
  40d208: ebfffc3b     	bl	0x40c2fc <std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*> std::copy<std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>, std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>>(std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>, std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>, std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>)> @ imm = #-0xf14
  40d20c: e59d2080     	ldr	r2, [sp, #0x80]
  40d210: e59d107c     	ldr	r1, [sp, #0x7c]
  40d214: e5943004     	ldr	r3, [r4, #0x4]
  40d218: e584200c     	str	r2, [r4, #0xc]
  40d21c: e5841008     	str	r1, [r4, #0x8]
  40d220: e5947000     	ldr	r7, [r4]
  40d224: eaffffc6     	b	0x40d144 <LightSetManager::InitLightFilter(std::vector<bool, std::allocator<bool>>&, bool)+0x154> @ imm = #-0xe8
  40d228: e282901f     	add	r9, r2, #31
  40d22c: e1580002     	cmp	r8, r2
  40d230: 20892008     	addhs	r2, r9, r8
  40d234: 30892002     	addlo	r2, r9, r2
  40d238: e1a092a2     	lsr	r9, r2, #5
  40d23c: e1a01009     	mov	r1, r9
  40d240: e3a02000     	mov	r2, #0
  40d244: e2840010     	add	r0, r4, #16
  40d248: ebfe9cbb     	bl	0x3b453c <std::allocator<unsigned int>::allocate(unsigned int, void const*)> @ imm = #-0x58d14
  40d24c: e594c004     	ldr	r12, [r4, #0x4]
  40d250: e594e000     	ldr	lr, [r4]
  40d254: e28d606c     	add	r6, sp, #108
  40d258: e58dc048     	str	r12, [sp, #0x48]
  40d25c: e59dc074     	ldr	r12, [sp, #0x74]
  40d260: e1a07000     	mov	r7, r0
  40d264: e28d1044     	add	r1, sp, #68
  40d268: e58dc03c     	str	r12, [sp, #0x3c]
  40d26c: e59dc078     	ldr	r12, [sp, #0x78]
  40d270: e28d203c     	add	r2, sp, #60
  40d274: e28d3034     	add	r3, sp, #52
  40d278: e58dc040     	str	r12, [sp, #0x40]
  40d27c: e1a00006     	mov	r0, r6
  40d280: e3a0c000     	mov	r12, #0
  40d284: e58dc038     	str	r12, [sp, #0x38]
  40d288: e58de044     	str	lr, [sp, #0x44]
  40d28c: e58d7034     	str	r7, [sp, #0x34]
  40d290: ebfffc19     	bl	0x40c2fc <std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*> std::copy<std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>, std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>>(std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>, std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>, std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>)> @ imm = #-0xf9c
  40d294: e59d3070     	ldr	r3, [sp, #0x70]
  40d298: e59d106c     	ldr	r1, [sp, #0x6c]
  40d29c: e1a02008     	mov	r2, r8
  40d2a0: e3a0c001     	mov	r12, #1
  40d2a4: e5910000     	ldr	r0, [r1]
  40d2a8: e353001f     	cmp	r3, #31
  40d2ac: e2422001     	sub	r2, r2, #1
  40d2b0: e1c0031c     	bic	r0, r0, r12, lsl r3
  40d2b4: 12833001     	addne	r3, r3, #1
  40d2b8: e5810000     	str	r0, [r1]
  40d2bc: 03a03000     	moveq	r3, #0
  40d2c0: 02811004     	addeq	r1, r1, #4
  40d2c4: e3520000     	cmp	r2, #0
  40d2c8: 1afffff5     	bne	0x40d2a4 <LightSetManager::InitLightFilter(std::vector<bool, std::allocator<bool>>&, bool)+0x2b4> @ imm = #-0x2c
  40d2cc: e59de074     	ldr	lr, [sp, #0x74]
  40d2d0: e5943008     	ldr	r3, [r4, #0x8]
  40d2d4: e594c00c     	ldr	r12, [r4, #0xc]
  40d2d8: e58de024     	str	lr, [sp, #0x24]
  40d2dc: e59de078     	ldr	lr, [sp, #0x78]
  40d2e0: e28da014     	add	r10, sp, #20
  40d2e4: e1a01006     	mov	r1, r6
  40d2e8: e1a02008     	mov	r2, r8
  40d2ec: e1a0000a     	mov	r0, r10
  40d2f0: e58de028     	str	lr, [sp, #0x28]
  40d2f4: e58d301c     	str	r3, [sp, #0x1c]
  40d2f8: e58dc020     	str	r12, [sp, #0x20]
  40d2fc: ebfffd99     	bl	0x40c968 <std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>::operator+(int) const> @ imm = #-0x99c
  40d300: e1a0300a     	mov	r3, r10
  40d304: e28d002c     	add	r0, sp, #44
  40d308: e28d201c     	add	r2, sp, #28
  40d30c: e28d1024     	add	r1, sp, #36
  40d310: ebfffbf9     	bl	0x40c2fc <std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*> std::copy<std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>, std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>>(std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>, std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>, std::priv::_Bit_iter<std::priv::_Bit_reference, std::priv::_Bit_reference*>)> @ imm = #-0x101c
  40d314: e5940000     	ldr	r0, [r4]
  40d318: e59d3030     	ldr	r3, [sp, #0x30]
  40d31c: e59d202c     	ldr	r2, [sp, #0x2c]
  40d320: e3500000     	cmp	r0, #0
  40d324: e584300c     	str	r3, [r4, #0xc]
  40d328: e5842008     	str	r2, [r4, #0x8]
  40d32c: 0a000005     	beq	0x40d348 <LightSetManager::InitLightFilter(std::vector<bool, std::allocator<bool>>&, bool)+0x358> @ imm = #0x14
  40d330: e5941010     	ldr	r1, [r4, #0x10]
  40d334: e0601001     	rsb	r1, r0, r1
  40d338: e3c11003     	bic	r1, r1, #3
  40d33c: e3510080     	cmp	r1, #128
  40d340: 8a000006     	bhi	0x40d360 <LightSetManager::InitLightFilter(std::vector<bool, std::allocator<bool>>&, bool)+0x370> @ imm = #0x18
  40d344: eb0beeed     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x2fbbb4
  40d348: e0879109     	add	r9, r7, r9, lsl #2
  40d34c: e3a03000     	mov	r3, #0
  40d350: e5849010     	str	r9, [r4, #0x10]
  40d354: e5843004     	str	r3, [r4, #0x4]
  40d358: e5847000     	str	r7, [r4]
  40d35c: eaffff78     	b	0x40d144 <LightSetManager::InitLightFilter(std::vector<bool, std::allocator<bool>>&, bool)+0x154> @ imm = #-0x220
  40d360: ebfc0c36     	bl	0x310440 <CustomFree(void*)> @ imm = #-0xfcf28
  40d364: eafffff7     	b	0x40d348 <LightSetManager::InitLightFilter(std::vector<bool, std::allocator<bool>>&, bool)+0x358> @ imm = #-0x24
