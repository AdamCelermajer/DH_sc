
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00521eb8 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)>:
  521eb8: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  521ebc: e59f6360     	ldr	r6, [pc, #0x360]        @ 0x522224 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x36c>
  521ec0: e59f7360     	ldr	r7, [pc, #0x360]        @ 0x522228 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x370>
  521ec4: e24dd030     	sub	sp, sp, #48
  521ec8: e08f6006     	add	r6, pc, r6
  521ecc: e7963007     	ldr	r3, [r6, r7]
  521ed0: e2519000     	subs	r9, r1, #0
  521ed4: e1a04000     	mov	r4, r0
  521ed8: e5933000     	ldr	r3, [r3]
  521edc: e1a08002     	mov	r8, r2
  521ee0: e58d302c     	str	r3, [sp, #0x2c]
  521ee4: 0a00008d     	beq	0x522120 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x268> @ imm = #0x234
  521ee8: e3a01000     	mov	r1, #0
  521eec: e3a000cc     	mov	r0, #204
  521ef0: ebf7b99e     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x211988
  521ef4: e594c02c     	ldr	r12, [r4, #0x2c]
  521ef8: e5943028     	ldr	r3, [r4, #0x28]
  521efc: e1a01008     	mov	r1, r8
  521f00: e58dc000     	str	r12, [sp]
  521f04: e1a02004     	mov	r2, r4
  521f08: e3a0c001     	mov	r12, #1
  521f0c: e1a05000     	mov	r5, r0
  521f10: e58dc004     	str	r12, [sp, #0x4]
  521f14: ebffeca6     	bl	0x51d1b4 <PFFloor::PFFloor(char const*, PFRoom*, PFGOuterGraph*, PFGInnerGraph*, unsigned int)> @ imm = #-0x4d68
  521f18: e5948034     	ldr	r8, [r4, #0x34]
  521f1c: e5943038     	ldr	r3, [r4, #0x38]
  521f20: e1580003     	cmp	r8, r3
  521f24: 0a000092     	beq	0x522174 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x2bc> @ imm = #0x248
  521f28: e5885000     	str	r5, [r8]
  521f2c: e5943034     	ldr	r3, [r4, #0x34]
  521f30: e2833004     	add	r3, r3, #4
  521f34: e5843034     	str	r3, [r4, #0x34]
  521f38: e59f32ec     	ldr	r3, [pc, #0x2ec]        @ 0x52222c <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x374>
  521f3c: e28d8014     	add	r8, sp, #20
  521f40: e796a003     	ldr	r10, [r6, r3]
  521f44: e1a0000a     	mov	r0, r10
  521f48: ebf8564e     	bl	0x337888 <DebugSwitches::load()> @ imm = #-0x1ea6c8
  521f4c: e59f12dc     	ldr	r1, [pc, #0x2dc]        @ 0x522230 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x378>
  521f50: e28d2010     	add	r2, sp, #16
  521f54: e1a00008     	mov	r0, r8
  521f58: e08f1001     	add	r1, pc, r1
  521f5c: ebf7c862     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x20de78
  521f60: e1a0000a     	mov	r0, r10
  521f64: e1a01008     	mov	r1, r8
  521f68: ebf856c6     	bl	0x337a88 <DebugSwitches::GetSwitch(std::string const&)> @ imm = #-0x1ea4e8
  521f6c: e1a0a000     	mov	r10, r0
  521f70: e59d0028     	ldr	r0, [sp, #0x28]
  521f74: e1500008     	cmp	r0, r8
  521f78: 0a000006     	beq	0x521f98 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0xe0> @ imm = #0x18
  521f7c: e3500000     	cmp	r0, #0
  521f80: 0a000004     	beq	0x521f98 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0xe0> @ imm = #0x10
  521f84: e59d1014     	ldr	r1, [sp, #0x14]
  521f88: e0601001     	rsb	r1, r0, r1
  521f8c: e3510080     	cmp	r1, #128
  521f90: 8a000060     	bhi	0x522118 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x260> @ imm = #0x180
  521f94: eb079bd9     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x1e6f64
  521f98: e35a0000     	cmp	r10, #0
  521f9c: 0a000047     	beq	0x5220c0 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x208> @ imm = #0x11c
  521fa0: eb03a449     	bl	0x60b0cc <glitch::os::Timer::getRealTime()> @ imm = #0xe9124
  521fa4: e1a00005     	mov	r0, r5
  521fa8: e1a01009     	mov	r1, r9
  521fac: ebfffae3     	bl	0x520b40 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)> @ imm = #-0x1474
  521fb0: eb03a445     	bl	0x60b0cc <glitch::os::Timer::getRealTime()> @ imm = #0xe9114
  521fb4: e5942034     	ldr	r2, [r4, #0x34]
  521fb8: e5943030     	ldr	r3, [r4, #0x30]
  521fbc: e0633002     	rsb	r3, r3, r2
  521fc0: e1a03143     	asr	r3, r3, #2
  521fc4: e3530001     	cmp	r3, #1
  521fc8: 0a000045     	beq	0x5220e4 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x22c> @ imm = #0x114
  521fcc: e5958044     	ldr	r8, [r5, #0x44]
  521fd0: e594a03c     	ldr	r10, [r4, #0x3c]
  521fd4: e1a00008     	mov	r0, r8
  521fd8: e1a0100a     	mov	r1, r10
  521fdc: ebf7b1ca     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x2138d8
  521fe0: e3500000     	cmp	r0, #0
  521fe4: 01a0800a     	moveq	r8, r10
  521fe8: e584803c     	str	r8, [r4, #0x3c]
  521fec: e5958048     	ldr	r8, [r5, #0x48]
  521ff0: e594a040     	ldr	r10, [r4, #0x40]
  521ff4: e1a00008     	mov	r0, r8
  521ff8: e1a0100a     	mov	r1, r10
  521ffc: ebf7b1c2     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x2138f8
  522000: e3500000     	cmp	r0, #0
  522004: 01a0800a     	moveq	r8, r10
  522008: e5848040     	str	r8, [r4, #0x40]
  52200c: e595804c     	ldr	r8, [r5, #0x4c]
  522010: e594a044     	ldr	r10, [r4, #0x44]
  522014: e1a00008     	mov	r0, r8
  522018: e1a0100a     	mov	r1, r10
  52201c: ebf7b1ba     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x213918
  522020: e3500000     	cmp	r0, #0
  522024: 01a0800a     	moveq	r8, r10
  522028: e5848044     	str	r8, [r4, #0x44]
  52202c: e5958050     	ldr	r8, [r5, #0x50]
  522030: e594a048     	ldr	r10, [r4, #0x48]
  522034: e1a01008     	mov	r1, r8
  522038: e1a0000a     	mov	r0, r10
  52203c: ebf7b1b2     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x213938
  522040: e3500000     	cmp	r0, #0
  522044: 01a0800a     	moveq	r8, r10
  522048: e5848048     	str	r8, [r4, #0x48]
  52204c: e5958054     	ldr	r8, [r5, #0x54]
  522050: e594a04c     	ldr	r10, [r4, #0x4c]
  522054: e1a01008     	mov	r1, r8
  522058: e1a0000a     	mov	r0, r10
  52205c: ebf7b1aa     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x213958
  522060: e3500000     	cmp	r0, #0
  522064: 01a0800a     	moveq	r8, r10
  522068: e584804c     	str	r8, [r4, #0x4c]
  52206c: e594a050     	ldr	r10, [r4, #0x50]
  522070: e5958058     	ldr	r8, [r5, #0x58]
  522074: e1a0000a     	mov	r0, r10
  522078: e1a01008     	mov	r1, r8
  52207c: ebf7b1a2     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x213978
  522080: e3500000     	cmp	r0, #0
  522084: 01a0800a     	moveq	r8, r10
  522088: e5848050     	str	r8, [r4, #0x50]
  52208c: e59f31a0     	ldr	r3, [pc, #0x1a0]        @ 0x522234 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x37c>
  522090: e5951040     	ldr	r1, [r5, #0x40]
  522094: e7963003     	ldr	r3, [r6, r3]
  522098: e5933010     	ldr	r3, [r3, #0x10]
  52209c: e593001c     	ldr	r0, [r3, #0x1c]
  5220a0: ebf8c0fe     	bl	0x3524a0 <SceneManager::AddNodeToMap(glitch::scene::ISceneNode*)> @ imm = #-0x1cfc08
  5220a4: e7963007     	ldr	r3, [r6, r7]
  5220a8: e59d202c     	ldr	r2, [sp, #0x2c]
  5220ac: e5933000     	ldr	r3, [r3]
  5220b0: e1520003     	cmp	r2, r3
  5220b4: 1a000059     	bne	0x522220 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x368> @ imm = #0x164
  5220b8: e28dd030     	add	sp, sp, #48
  5220bc: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  5220c0: e1a01009     	mov	r1, r9
  5220c4: e1a00005     	mov	r0, r5
  5220c8: ebfffa9c     	bl	0x520b40 <PFFloor::_LoadNavMesh(glitch::scene::IMeshSceneNode*)> @ imm = #-0x1590
  5220cc: e5942034     	ldr	r2, [r4, #0x34]
  5220d0: e5943030     	ldr	r3, [r4, #0x30]
  5220d4: e0633002     	rsb	r3, r3, r2
  5220d8: e1a03143     	asr	r3, r3, #2
  5220dc: e3530001     	cmp	r3, #1
  5220e0: 1affffb9     	bne	0x521fcc <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x114> @ imm = #-0x11c
  5220e4: e5953044     	ldr	r3, [r5, #0x44]
  5220e8: e584303c     	str	r3, [r4, #0x3c]
  5220ec: e5953048     	ldr	r3, [r5, #0x48]
  5220f0: e5843040     	str	r3, [r4, #0x40]
  5220f4: e595304c     	ldr	r3, [r5, #0x4c]
  5220f8: e5843044     	str	r3, [r4, #0x44]
  5220fc: e5953050     	ldr	r3, [r5, #0x50]
  522100: e5843048     	str	r3, [r4, #0x48]
  522104: e5953054     	ldr	r3, [r5, #0x54]
  522108: e584304c     	str	r3, [r4, #0x4c]
  52210c: e5953058     	ldr	r3, [r5, #0x58]
  522110: e5843050     	str	r3, [r4, #0x50]
  522114: eaffffdc     	b	0x52208c <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x1d4> @ imm = #-0x90
  522118: ebf7b8c8     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x211ce0
  52211c: eaffff9d     	b	0x521f98 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0xe0> @ imm = #-0x18c
  522120: e59f3110     	ldr	r3, [pc, #0x110]        @ 0x522238 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x380>
  522124: e7963003     	ldr	r3, [r6, r3]
  522128: e5933000     	ldr	r3, [r3]
  52212c: e3530002     	cmp	r3, #2
  522130: 05899000     	streq	r9, [r9]
  522134: 0affff6b     	beq	0x521ee8 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x30> @ imm = #-0x254
  522138: e3530001     	cmp	r3, #1
  52213c: 1affff69     	bne	0x521ee8 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x30> @ imm = #-0x25c
  522140: e59f00f4     	ldr	r0, [pc, #0xf4]         @ 0x52223c <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x384>
  522144: e59f10f4     	ldr	r1, [pc, #0xf4]         @ 0x522240 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x388>
  522148: e59f20f4     	ldr	r2, [pc, #0xf4]         @ 0x522244 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x38c>
  52214c: e7960000     	ldr	r0, [r6, r0]
  522150: e59f30f0     	ldr	r3, [pc, #0xf0]         @ 0x522248 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x390>
  522154: e3a0c035     	mov	r12, #53
  522158: e08f1001     	add	r1, pc, r1
  52215c: e08f2002     	add	r2, pc, r2
  522160: e08f3003     	add	r3, pc, r3
  522164: e28000a8     	add	r0, r0, #168
  522168: e58dc000     	str	r12, [sp]
  52216c: ebf7afa4     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x214170
  522170: eaffff5c     	b	0x521ee8 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x30> @ imm = #-0x290
  522174: e5943030     	ldr	r3, [r4, #0x30]
  522178: e0633008     	rsb	r3, r3, r8
  52217c: e1a03143     	asr	r3, r3, #2
  522180: e3530001     	cmp	r3, #1
  522184: 20831003     	addhs	r1, r3, r3
  522188: 32831001     	addlo	r1, r3, #1
  52218c: e3710107     	cmn	r1, #-1073741823
  522190: 9a000019     	bls	0x5221fc <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x344> @ imm = #0x64
  522194: e3e01103     	mvn	r1, #-1073741824
  522198: e28d2030     	add	r2, sp, #48
  52219c: e5221024     	str	r1, [r2, #-0x24]!
  5221a0: e2840038     	add	r0, r4, #56
  5221a4: ebfffee5     	bl	0x521d40 <std::allocator<PFFloor*>::_M_allocate(unsigned int, unsigned int&)> @ imm = #-0x46c
  5221a8: e5941030     	ldr	r1, [r4, #0x30]
  5221ac: e1a0a000     	mov	r10, r0
  5221b0: e0588001     	subs	r8, r8, r1
  5221b4: 01a08000     	moveq	r8, r0
  5221b8: 1a000014     	bne	0x522210 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x358> @ imm = #0x50
  5221bc: e4885004     	str	r5, [r8], #4
  5221c0: e5940030     	ldr	r0, [r4, #0x30]
  5221c4: e5941038     	ldr	r1, [r4, #0x38]
  5221c8: e3500000     	cmp	r0, #0
  5221cc: 0a000004     	beq	0x5221e4 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x32c> @ imm = #0x10
  5221d0: e0601001     	rsb	r1, r0, r1
  5221d4: e3c11003     	bic	r1, r1, #3
  5221d8: e3510080     	cmp	r1, #128
  5221dc: 8a000009     	bhi	0x522208 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x350> @ imm = #0x24
  5221e0: eb079b46     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x1e6d18
  5221e4: e59d300c     	ldr	r3, [sp, #0xc]
  5221e8: e584a030     	str	r10, [r4, #0x30]
  5221ec: e5848034     	str	r8, [r4, #0x34]
  5221f0: e08aa103     	add	r10, r10, r3, lsl #2
  5221f4: e584a038     	str	r10, [r4, #0x38]
  5221f8: eaffff4e     	b	0x521f38 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x80> @ imm = #-0x2c8
  5221fc: e1530001     	cmp	r3, r1
  522200: 9affffe4     	bls	0x522198 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x2e0> @ imm = #-0x70
  522204: eaffffe2     	b	0x522194 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x2dc> @ imm = #-0x78
  522208: ebf7b88c     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x211dd0
  52220c: eafffff4     	b	0x5221e4 <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x32c> @ imm = #-0x30
  522210: e1a02008     	mov	r2, r8
  522214: ebf7af47     	bl	0x30df38 <.plt+0x1c4>   @ imm = #-0x2142e4
  522218: e0808008     	add	r8, r0, r8
  52221c: eaffffe6     	b	0x5221bc <PFRoom::_LoadFloor(glitch::scene::IMeshSceneNode*, char const*)+0x304> @ imm = #-0x68
  522220: ebf7b03a     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x213f18
  522224: c8 2b 47 00  	.word	0x00472bc8
  522228: ac 40 00 00  	.word	0x000040ac
  52222c: 84 08 00 00  	.word	0x00000884
  522230: 98 aa 3b 00  	.word	0x003baa98
  522234: f4 37 00 00  	.word	0x000037f4
  522238: c0 39 00 00  	.word	0x000039c0
  52223c: c0 19 00 00  	.word	0x000019c0
  522240: 80 c2 39 00  	.word	0x0039c280
  522244: 8c a8 3b 00  	.word	0x003ba88c
  522248: 20 a8 3b 00  	.word	0x003ba820
