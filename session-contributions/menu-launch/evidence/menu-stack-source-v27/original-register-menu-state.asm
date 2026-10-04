
R:\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

007adf50 <MenuFX::RegisterState(MenuFX::State*, char const*)>:
  7adf50: e92d40f0     	push	{r4, r5, r6, r7, lr}
  7adf54: e1a05001     	mov	r5, r1
  7adf58: e5850004     	str	r0, [r5, #0x4]
  7adf5c: e5906108     	ldr	r6, [r0, #0x108]
  7adf60: e24dd00c     	sub	sp, sp, #12
  7adf64: e1a04000     	mov	r4, r0
  7adf68: e2967001     	adds	r7, r6, #1
  7adf6c: 0a000002     	beq	0x7adf7c <MenuFX::RegisterState(MenuFX::State*, char const*)+0x2c> @ imm = #0x8
  7adf70: e590310c     	ldr	r3, [r0, #0x10c]
  7adf74: e1570003     	cmp	r7, r3
  7adf78: ca000016     	bgt	0x7adfd8 <MenuFX::RegisterState(MenuFX::State*, char const*)+0x88> @ imm = #0x58
  7adf7c: e5943104     	ldr	r3, [r4, #0x104]
  7adf80: e3a01000     	mov	r1, #0
  7adf84: e3520000     	cmp	r2, #0
  7adf88: e7831106     	str	r1, [r3, r6, lsl #2]
  7adf8c: e5943104     	ldr	r3, [r4, #0x104]
  7adf90: 02852008     	addeq	r2, r5, #8
  7adf94: e5847108     	str	r7, [r4, #0x108]
  7adf98: e1a01002     	mov	r1, r2
  7adf9c: e7835106     	str	r5, [r3, r6, lsl #2]
  7adfa0: e1a00004     	mov	r0, r4
  7adfa4: ebffec6d     	bl	0x7a9160 <RenderFX::Find(char const*)> @ imm = #-0x4e4c
  7adfa8: e1a04000     	mov	r4, r0
  7adfac: e1a01000     	mov	r1, r0
  7adfb0: e2850048     	add	r0, r5, #72
  7adfb4: ebf1e6fb     	bl	0x427ba8 <gameswf::weak_ptr<gameswf::character>::operator=(gameswf::character*)> @ imm = #-0x386414
  7adfb8: e3a03000     	mov	r3, #0
  7adfbc: e5c4309b     	strb	r3, [r4, #0x9b]
  7adfc0: e1a00005     	mov	r0, r5
  7adfc4: e5953000     	ldr	r3, [r5]
  7adfc8: e1a0e00f     	mov	lr, pc
  7adfcc: e593f008     	ldr	pc, [r3, #0x8]
  7adfd0: e28dd00c     	add	sp, sp, #12
  7adfd4: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  7adfd8: e2800f41     	add	r0, r0, #260
  7adfdc: e08710c7     	add	r1, r7, r7, asr #1
  7adfe0: e58d2004     	str	r2, [sp, #0x4]
  7adfe4: ebf2270c     	bl	0x437c1c <gameswf::array<MenuFX::State*>::reserve(int)> @ imm = #-0x3763d0
  7adfe8: e59d2004     	ldr	r2, [sp, #0x4]
  7adfec: eaffffe2     	b	0x7adf7c <MenuFX::RegisterState(MenuFX::State*, char const*)+0x2c> @ imm = #-0x78
