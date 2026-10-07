
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034dd18 <InputManagerWin32::InputManagerWin32()>:
  34dd18: e3a01001     	mov	r1, #1
  34dd1c: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  34dd20: e1a02001     	mov	r2, r1
  34dd24: e3a03004     	mov	r3, #4
  34dd28: e59f506c     	ldr	r5, [pc, #0x6c]         @ 0x34dd9c <InputManagerWin32::InputManagerWin32()+0x84>
  34dd2c: e1a04000     	mov	r4, r0
  34dd30: ebfffe61     	bl	0x34d6bc <InputManager::InputManager(int, int, int)> @ imm = #-0x67c
  34dd34: e59f3064     	ldr	r3, [pc, #0x64]         @ 0x34dda0 <InputManagerWin32::InputManagerWin32()+0x88>
  34dd38: e08f5005     	add	r5, pc, r5
  34dd3c: e1a00004     	mov	r0, r4
  34dd40: e7953003     	ldr	r3, [r5, r3]
  34dd44: e2847ee5     	add	r7, r4, #3664
  34dd48: e2876e75     	add	r6, r7, #1872
  34dd4c: e2833008     	add	r3, r3, #8
  34dd50: e4803018     	str	r3, [r0], #24
  34dd54: ebfffbce     	bl	0x34cc94 <Keyboard::Keyboard()> @ imm = #-0x10c8
  34dd58: e2840d31     	add	r0, r4, #3136
  34dd5c: e286600c     	add	r6, r6, #12
  34dd60: e2800004     	add	r0, r0, #4
  34dd64: ebfffb81     	bl	0x34cb70 <Mouse::Mouse()> @ imm = #-0x11fc
  34dd68: e2865e75     	add	r5, r6, #1872
  34dd6c: e1a00007     	mov	r0, r7
  34dd70: e285500c     	add	r5, r5, #12
  34dd74: ebfffc43     	bl	0x34ce88 <Gamepad::Gamepad()> @ imm = #-0xef4
  34dd78: e1a00006     	mov	r0, r6
  34dd7c: ebfffc41     	bl	0x34ce88 <Gamepad::Gamepad()> @ imm = #-0xefc
  34dd80: e1a00005     	mov	r0, r5
  34dd84: ebfffc3f     	bl	0x34ce88 <Gamepad::Gamepad()> @ imm = #-0xf04
  34dd88: e2850e75     	add	r0, r5, #1872
  34dd8c: e280000c     	add	r0, r0, #12
  34dd90: ebfffc3c     	bl	0x34ce88 <Gamepad::Gamepad()> @ imm = #-0xf10
  34dd94: e1a00004     	mov	r0, r4
  34dd98: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  34dd9c: 58 6d 64 00  	.word	0x00646d58
  34dda0: 94 18 00 00  	.word	0x00001894

0034dda4 <InputManager::GetInstance()>:
  34dda4: e92d4070     	push	{r4, r5, r6, lr}
  34dda8: e59f409c     	ldr	r4, [pc, #0x9c]         @ 0x34de4c <InputManager::GetInstance()+0xa8>
  34ddac: e59f509c     	ldr	r5, [pc, #0x9c]         @ 0x34de50 <InputManager::GetInstance()+0xac>
  34ddb0: e24dd008     	sub	sp, sp, #8
  34ddb4: e08f4004     	add	r4, pc, r4
  34ddb8: e5946004     	ldr	r6, [r4, #0x4]
  34ddbc: e08f5005     	add	r5, pc, r5
  34ddc0: e3560000     	cmp	r6, #0
  34ddc4: 0a000002     	beq	0x34ddd4 <InputManager::GetInstance()+0x30> @ imm = #0x8
  34ddc8: e1a00006     	mov	r0, r6
  34ddcc: e28dd008     	add	sp, sp, #8
  34ddd0: e8bd8070     	pop	{r4, r5, r6, pc}
  34ddd4: e1a01006     	mov	r1, r6
  34ddd8: e3a00daf     	mov	r0, #11200
  34dddc: ebff09e3     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x3d874
  34dde0: e1a06000     	mov	r6, r0
  34dde4: ebffffcb     	bl	0x34dd18 <InputManagerWin32::InputManagerWin32()> @ imm = #-0xd4
  34dde8: e3560000     	cmp	r6, #0
  34ddec: e5846004     	str	r6, [r4, #0x4]
  34ddf0: 1afffff4     	bne	0x34ddc8 <InputManager::GetInstance()+0x24> @ imm = #-0x30
  34ddf4: e59f3058     	ldr	r3, [pc, #0x58]         @ 0x34de54 <InputManager::GetInstance()+0xb0>
  34ddf8: e7953003     	ldr	r3, [r5, r3]
  34ddfc: e5933000     	ldr	r3, [r3]
  34de00: e3530002     	cmp	r3, #2
  34de04: 05866000     	streq	r6, [r6]
  34de08: 0affffee     	beq	0x34ddc8 <InputManager::GetInstance()+0x24> @ imm = #-0x48
  34de0c: e3530001     	cmp	r3, #1
  34de10: 1affffec     	bne	0x34ddc8 <InputManager::GetInstance()+0x24> @ imm = #-0x50
  34de14: e59f003c     	ldr	r0, [pc, #0x3c]         @ 0x34de58 <InputManager::GetInstance()+0xb4>
  34de18: e59f103c     	ldr	r1, [pc, #0x3c]         @ 0x34de5c <InputManager::GetInstance()+0xb8>
  34de1c: e59f203c     	ldr	r2, [pc, #0x3c]         @ 0x34de60 <InputManager::GetInstance()+0xbc>
  34de20: e7950000     	ldr	r0, [r5, r0]
  34de24: e59f3038     	ldr	r3, [pc, #0x38]         @ 0x34de64 <InputManager::GetInstance()+0xc0>
  34de28: e300c28e     	movw	r12, #0x28e
  34de2c: e08f1001     	add	r1, pc, r1
  34de30: e28000a8     	add	r0, r0, #168
  34de34: e08f2002     	add	r2, pc, r2
  34de38: e08f3003     	add	r3, pc, r3
  34de3c: e58dc000     	str	r12, [sp]
  34de40: ebff006f     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x3fe44
  34de44: e5946004     	ldr	r6, [r4, #0x4]
  34de48: eaffffde     	b	0x34ddc8 <InputManager::GetInstance()+0x24> @ imm = #-0x88
  34de4c: e4 40 65 00  	.word	0x006540e4
  34de50: d4 6c 64 00  	.word	0x00646cd4
  34de54: c0 39 00 00  	.word	0x000039c0
  34de58: c0 19 00 00  	.word	0x000019c0
  34de5c: ac 05 57 00  	.word	0x005705ac
  34de60: 8c 28 57 00  	.word	0x0057288c
  34de64: 30 28 57 00  	.word	0x00572830

0034de68 <InputManagerWin32::InputManagerWin32()>:
  34de68: e3a01001     	mov	r1, #1
  34de6c: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  34de70: e1a02001     	mov	r2, r1
  34de74: e3a03004     	mov	r3, #4
  34de78: e59f506c     	ldr	r5, [pc, #0x6c]         @ 0x34deec <InputManagerWin32::InputManagerWin32()+0x84>
  34de7c: e1a04000     	mov	r4, r0
  34de80: ebfffe0d     	bl	0x34d6bc <InputManager::InputManager(int, int, int)> @ imm = #-0x7cc
  34de84: e59f3064     	ldr	r3, [pc, #0x64]         @ 0x34def0 <InputManagerWin32::InputManagerWin32()+0x88>
  34de88: e08f5005     	add	r5, pc, r5
  34de8c: e1a00004     	mov	r0, r4
  34de90: e7953003     	ldr	r3, [r5, r3]
  34de94: e2847ee5     	add	r7, r4, #3664
  34de98: e2876e75     	add	r6, r7, #1872
  34de9c: e2833008     	add	r3, r3, #8
  34dea0: e4803018     	str	r3, [r0], #24
  34dea4: ebfffb7a     	bl	0x34cc94 <Keyboard::Keyboard()> @ imm = #-0x1218
  34dea8: e2840d31     	add	r0, r4, #3136
  34deac: e286600c     	add	r6, r6, #12
  34deb0: e2800004     	add	r0, r0, #4
  34deb4: ebfffb2d     	bl	0x34cb70 <Mouse::Mouse()> @ imm = #-0x134c
  34deb8: e2865e75     	add	r5, r6, #1872
  34debc: e1a00007     	mov	r0, r7
  34dec0: e285500c     	add	r5, r5, #12
  34dec4: ebfffbef     	bl	0x34ce88 <Gamepad::Gamepad()> @ imm = #-0x1044
  34dec8: e1a00006     	mov	r0, r6
  34decc: ebfffbed     	bl	0x34ce88 <Gamepad::Gamepad()> @ imm = #-0x104c
  34ded0: e1a00005     	mov	r0, r5
  34ded4: ebfffbeb     	bl	0x34ce88 <Gamepad::Gamepad()> @ imm = #-0x1054
  34ded8: e2850e75     	add	r0, r5, #1872
  34dedc: e280000c     	add	r0, r0, #12
  34dee0: ebfffbe8     	bl	0x34ce88 <Gamepad::Gamepad()> @ imm = #-0x1060
  34dee4: e1a00004     	mov	r0, r4
  34dee8: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  34deec: 08 6c 64 00  	.word	0x00646c08
  34def0: 94 18 00 00  	.word	0x00001894
