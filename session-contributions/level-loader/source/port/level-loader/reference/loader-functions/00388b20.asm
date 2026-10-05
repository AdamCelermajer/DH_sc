
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00388b20 <Module::InitPost()>:
  388b20: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  388b24: e59f4114     	ldr	r4, [pc, #0x114]        @ 0x388c40 <Module::InitPost()+0x120>
  388b28: e59f5114     	ldr	r5, [pc, #0x114]        @ 0x388c44 <Module::InitPost()+0x124>
  388b2c: e24dd054     	sub	sp, sp, #84
  388b30: e08f4004     	add	r4, pc, r4
  388b34: e7943005     	ldr	r3, [r4, r5]
  388b38: e1a08000     	mov	r8, r0
  388b3c: e5933000     	ldr	r3, [r3]
  388b40: e58d304c     	str	r3, [sp, #0x4c]
  388b44: ebffffd3     	bl	0x388a98 <Decor::InitPost()> @ imm = #-0xb4
  388b48: e59832d8     	ldr	r3, [r8, #0x2d8]
  388b4c: e3530000     	cmp	r3, #0
  388b50: 0a000022     	beq	0x388be0 <Module::InitPost()+0xc0> @ imm = #0x88
  388b54: e5933008     	ldr	r3, [r3, #0x8]
  388b58: e59f70e8     	ldr	r7, [pc, #0xe8]         @ 0x388c48 <Module::InitPost()+0x128>
  388b5c: e28da018     	add	r10, sp, #24
  388b60: e1a00003     	mov	r0, r3
  388b64: e5933000     	ldr	r3, [r3]
  388b68: e1a0e00f     	mov	lr, pc
  388b6c: e593f034     	ldr	pc, [r3, #0x34]
  388b70: e59f30d4     	ldr	r3, [pc, #0xd4]         @ 0x388c4c <Module::InitPost()+0x12c>
  388b74: e59f10d4     	ldr	r1, [pc, #0xd4]         @ 0x388c50 <Module::InitPost()+0x130>
  388b78: e08f7007     	add	r7, pc, r7
  388b7c: e7943003     	ldr	r3, [r4, r3]
  388b80: e597200c     	ldr	r2, [r7, #0xc]
  388b84: e08f1001     	add	r1, pc, r1
  388b88: e1a0b000     	mov	r11, r0
  388b8c: e1a0000a     	mov	r0, r10
  388b90: e5939038     	ldr	r9, [r3, #0x38]
  388b94: ebfe17d2     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x7a0b8
  388b98: e597300c     	ldr	r3, [r7, #0xc]
  388b9c: e59f20b0     	ldr	r2, [pc, #0xb0]         @ 0x388c54 <Module::InitPost()+0x134>
  388ba0: e28d600c     	add	r6, sp, #12
  388ba4: e2833001     	add	r3, r3, #1
  388ba8: e587300c     	str	r3, [r7, #0xc]
  388bac: e3a0c001     	mov	r12, #1
  388bb0: e3a07000     	mov	r7, #0
  388bb4: e08f2002     	add	r2, pc, r2
  388bb8: e1a0300a     	mov	r3, r10
  388bbc: e1a01009     	mov	r1, r9
  388bc0: e1a00006     	mov	r0, r6
  388bc4: e88d1080     	stm	sp, {r7, r12}
  388bc8: ebff0ad5     	bl	0x34b724 <ObjectManager::Spawn(char const*, char const*, bool, bool)> @ imm = #-0x3d4ac
  388bcc: e1a01007     	mov	r1, r7
  388bd0: e1a00006     	mov	r0, r6
  388bd4: ebfedc79     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0x48e1c
  388bd8: e2507000     	subs	r7, r0, #0
  388bdc: 1a000006     	bne	0x388bfc <Module::InitPost()+0xdc> @ imm = #0x18
  388be0: e7943005     	ldr	r3, [r4, r5]
  388be4: e59d204c     	ldr	r2, [sp, #0x4c]
  388be8: e5933000     	ldr	r3, [r3]
  388bec: e1520003     	cmp	r2, r3
  388bf0: 1a000011     	bne	0x388c3c <Module::InitPost()+0x11c> @ imm = #0x44
  388bf4: e28dd054     	add	sp, sp, #84
  388bf8: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  388bfc: e59730f4     	ldr	r3, [r7, #0xf4]
  388c00: e353000b     	cmp	r3, #11
  388c04: 1afffff5     	bne	0x388be0 <Module::InitPost()+0xc0> @ imm = #-0x2c
  388c08: e1a02006     	mov	r2, r6
  388c0c: e492c004     	ldr	r12, [r2], #4
  388c10: e5961004     	ldr	r1, [r6, #0x4]
  388c14: e2883b01     	add	r3, r8, #1024
  388c18: e5922004     	ldr	r2, [r2, #0x4]
  388c1c: e2833004     	add	r3, r3, #4
  388c20: e588c400     	str	r12, [r8, #0x400]
  388c24: e4831004     	str	r1, [r3], #4
  388c28: e5832000     	str	r2, [r3]
  388c2c: e1a0100b     	mov	r1, r11
  388c30: eb003a57     	bl	0x397594 <Zone::InitWithBoundingBox(glitch::core::aabbox3d<float> const&)> @ imm = #0xe95c
  388c34: e587838c     	str	r8, [r7, #0x38c]
  388c38: eaffffe8     	b	0x388be0 <Module::InitPost()+0xc0> @ imm = #-0x60
  388c3c: ebfe15b3     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x7a934
  388c40: 60 bf 60 00  	.word	0x0060bf60
  388c44: ac 40 00 00  	.word	0x000040ac
  388c48: 34 9b 61 00  	.word	0x00619b34
  388c4c: f4 37 00 00  	.word	0x000037f4
  388c50: c4 96 53 00  	.word	0x005396c4
  388c54: bc 77 53 00  	.word	0x005377bc
