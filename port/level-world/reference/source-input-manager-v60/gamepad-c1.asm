
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034ce88 <Gamepad::Gamepad()>:
  34ce88: e92d4070     	push	{r4, r5, r6, lr}
  34ce8c: e3a01002     	mov	r1, #2
  34ce90: e59f5160     	ldr	r5, [pc, #0x160]        @ 0x34cff8 <Gamepad::Gamepad()+0x170>
  34ce94: e1a04000     	mov	r4, r0
  34ce98: ebfffef1     	bl	0x34ca64 <InputDevice::InputDevice(InputDevice::Type)> @ imm = #-0x43c
  34ce9c: e59f0158     	ldr	r0, [pc, #0x158]        @ 0x34cffc <Gamepad::Gamepad()+0x174>
  34cea0: e08f5005     	add	r5, pc, r5
  34cea4: e1a03004     	mov	r3, r4
  34cea8: e7950000     	ldr	r0, [r5, r0]
  34ceac: e3a02000     	mov	r2, #0
  34ceb0: e3a015fe     	mov	r1, #1065353216
  34ceb4: e2800008     	add	r0, r0, #8
  34ceb8: e483000c     	str	r0, [r3], #12
  34cebc: e283ce5a     	add	r12, r3, #1440
  34cec0: e3a00000     	mov	r0, #0
  34cec4: e5832014     	str	r2, [r3, #0x14]
  34cec8: e5832010     	str	r2, [r3, #0x10]
  34cecc: e583200c     	str	r2, [r3, #0xc]
  34ced0: e5832004     	str	r2, [r3, #0x4]
  34ced4: e5832000     	str	r2, [r3]
  34ced8: e5831018     	str	r1, [r3, #0x18]
  34cedc: e5831008     	str	r1, [r3, #0x8]
  34cee0: e5c3001c     	strb	r0, [r3, #0x1c]
  34cee4: e2833020     	add	r3, r3, #32
  34cee8: e153000c     	cmp	r3, r12
  34ceec: 1afffff4     	bne	0x34cec4 <Gamepad::Gamepad()+0x3c> @ imm = #-0x30
  34cef0: e2843e5a     	add	r3, r4, #1440
  34cef4: e283cf4b     	add	r12, r3, #300
  34cef8: e3a00000     	mov	r0, #0
  34cefc: e283300c     	add	r3, r3, #12
  34cf00: e5832014     	str	r2, [r3, #0x14]
  34cf04: e5832010     	str	r2, [r3, #0x10]
  34cf08: e583200c     	str	r2, [r3, #0xc]
  34cf0c: e5832004     	str	r2, [r3, #0x4]
  34cf10: e5832000     	str	r2, [r3]
  34cf14: e5831018     	str	r1, [r3, #0x18]
  34cf18: e5831008     	str	r1, [r3, #0x8]
  34cf1c: e5c3001c     	strb	r0, [r3, #0x1c]
  34cf20: e2833020     	add	r3, r3, #32
  34cf24: e153000c     	cmp	r3, r12
  34cf28: 1afffff4     	bne	0x34cf00 <Gamepad::Gamepad()+0x78> @ imm = #-0x30
  34cf2c: e2843d1b     	add	r3, r4, #1728
  34cf30: e283103c     	add	r1, r3, #60
  34cf34: e283300c     	add	r3, r3, #12
  34cf38: e5832000     	str	r2, [r3]
  34cf3c: e5832004     	str	r2, [r3, #0x4]
  34cf40: e5832008     	str	r2, [r3, #0x8]
  34cf44: e283300c     	add	r3, r3, #12
  34cf48: e1530001     	cmp	r3, r1
  34cf4c: 1afffff9     	bne	0x34cf38 <Gamepad::Gamepad()+0xb0> @ imm = #-0x1c
  34cf50: e2843c07     	add	r3, r4, #1792
  34cf54: e283103c     	add	r1, r3, #60
  34cf58: e283300c     	add	r3, r3, #12
  34cf5c: e5832000     	str	r2, [r3]
  34cf60: e5832004     	str	r2, [r3, #0x4]
  34cf64: e5832008     	str	r2, [r3, #0x8]
  34cf68: e283300c     	add	r3, r3, #12
  34cf6c: e1530001     	cmp	r3, r1
  34cf70: 1afffff9     	bne	0x34cf5c <Gamepad::Gamepad()+0xd4> @ imm = #-0x1c
  34cf74: e59f2084     	ldr	r2, [pc, #0x84]         @ 0x34d000 <Gamepad::Gamepad()+0x178>
  34cf78: e3a03000     	mov	r3, #0
  34cf7c: e1a00003     	mov	r0, r3
  34cf80: e7952002     	ldr	r2, [r5, r2]
  34cf84: e3a01080     	mov	r1, #128
  34cf88: e5841754     	str	r1, [r4, #0x754]
  34cf8c: e5c4374c     	strb	r3, [r4, #0x74c]
  34cf90: e5c43758     	strb	r3, [r4, #0x758]
  34cf94: e5843750     	str	r3, [r4, #0x750]
  34cf98: e1a01004     	mov	r1, r4
  34cf9c: e1a03004     	mov	r3, r4
  34cfa0: e1a0c000     	mov	r12, r0
  34cfa4: e5925000     	ldr	r5, [r2]
  34cfa8: e2800001     	add	r0, r0, #1
  34cfac: e3500004     	cmp	r0, #4
  34cfb0: e58356cc     	str	r5, [r3, #0x6cc]
  34cfb4: e5925004     	ldr	r5, [r2, #0x4]
  34cfb8: e58356d0     	str	r5, [r3, #0x6d0]
  34cfbc: e5925008     	ldr	r5, [r2, #0x8]
  34cfc0: e58356d4     	str	r5, [r3, #0x6d4]
  34cfc4: e581c6fc     	str	r12, [r1, #0x6fc]
  34cfc8: e5925000     	ldr	r5, [r2]
  34cfcc: e583570c     	str	r5, [r3, #0x70c]
  34cfd0: e5925004     	ldr	r5, [r2, #0x4]
  34cfd4: e5835710     	str	r5, [r3, #0x710]
  34cfd8: e5925008     	ldr	r5, [r2, #0x8]
  34cfdc: e5835714     	str	r5, [r3, #0x714]
  34cfe0: e581c73c     	str	r12, [r1, #0x73c]
  34cfe4: e283300c     	add	r3, r3, #12
  34cfe8: e2811004     	add	r1, r1, #4
  34cfec: 1affffec     	bne	0x34cfa4 <Gamepad::Gamepad()+0x11c> @ imm = #-0x50
  34cff0: e1a00004     	mov	r0, r4
  34cff4: e8bd8070     	pop	{r4, r5, r6, pc}
  34cff8: f0 7b 64 00  	.word	0x00647bf0
  34cffc: 1c 14 00 00  	.word	0x0000141c
  34d000: 2c 3f 00 00  	.word	0x00003f2c
