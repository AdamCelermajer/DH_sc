
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034ca64 <InputDevice::InputDevice(InputDevice::Type)>:
  34ca64: e59f3020     	ldr	r3, [pc, #0x20]         @ 0x34ca8c <InputDevice::InputDevice(InputDevice::Type)+0x28>
  34ca68: e59fc020     	ldr	r12, [pc, #0x20]        @ 0x34ca90 <InputDevice::InputDevice(InputDevice::Type)+0x2c>
  34ca6c: e5801004     	str	r1, [r0, #0x4]
  34ca70: e08f3003     	add	r3, pc, r3
  34ca74: e793c00c     	ldr	r12, [r3, r12]
  34ca78: e3a01000     	mov	r1, #0
  34ca7c: e5801008     	str	r1, [r0, #0x8]
  34ca80: e28cc008     	add	r12, r12, #8
  34ca84: e580c000     	str	r12, [r0]
  34ca88: e12fff1e     	bx	lr
  34ca8c: 20 80 64 00  	.word	0x00648020
  34ca90: d4 18 00 00  	.word	0x000018d4

0034ca94 <InputDevice::InputDevice(InputDevice::Type)>:
  34ca94: e59f3020     	ldr	r3, [pc, #0x20]         @ 0x34cabc <InputDevice::InputDevice(InputDevice::Type)+0x28>
  34ca98: e59fc020     	ldr	r12, [pc, #0x20]        @ 0x34cac0 <InputDevice::InputDevice(InputDevice::Type)+0x2c>
  34ca9c: e5801004     	str	r1, [r0, #0x4]
  34caa0: e08f3003     	add	r3, pc, r3
  34caa4: e793c00c     	ldr	r12, [r3, r12]
  34caa8: e3a01000     	mov	r1, #0
  34caac: e5801008     	str	r1, [r0, #0x8]
  34cab0: e28cc008     	add	r12, r12, #8
  34cab4: e580c000     	str	r12, [r0]
  34cab8: e12fff1e     	bx	lr
  34cabc: f0 7f 64 00  	.word	0x00647ff0
  34cac0: d4 18 00 00  	.word	0x000018d4

0034cac4 <Mouse::Mouse()>:
  34cac4: e92d4070     	push	{r4, r5, r6, lr}
  34cac8: e3a01001     	mov	r1, #1
  34cacc: e59f5094     	ldr	r5, [pc, #0x94]         @ 0x34cb68 <Mouse::Mouse()+0xa4>
  34cad0: e1a04000     	mov	r4, r0
  34cad4: ebffffe2     	bl	0x34ca64 <InputDevice::InputDevice(InputDevice::Type)> @ imm = #-0x78
  34cad8: e59f008c     	ldr	r0, [pc, #0x8c]         @ 0x34cb6c <Mouse::Mouse()+0xa8>
  34cadc: e08f5005     	add	r5, pc, r5
  34cae0: e1a03004     	mov	r3, r4
  34cae4: e7950000     	ldr	r0, [r5, r0]
  34cae8: e3a02000     	mov	r2, #0
  34caec: e3a015fe     	mov	r1, #1065353216
  34caf0: e2800008     	add	r0, r0, #8
  34caf4: e483000c     	str	r0, [r3], #12
  34caf8: e284cf43     	add	r12, r4, #268
  34cafc: e3a00000     	mov	r0, #0
  34cb00: e5832014     	str	r2, [r3, #0x14]
  34cb04: e5832010     	str	r2, [r3, #0x10]
  34cb08: e583200c     	str	r2, [r3, #0xc]
  34cb0c: e5832004     	str	r2, [r3, #0x4]
  34cb10: e5832000     	str	r2, [r3]
  34cb14: e5831018     	str	r1, [r3, #0x18]
  34cb18: e5831008     	str	r1, [r3, #0x8]
  34cb1c: e5c3001c     	strb	r0, [r3, #0x1c]
  34cb20: e2833020     	add	r3, r3, #32
  34cb24: e153000c     	cmp	r3, r12
  34cb28: 1afffff4     	bne	0x34cb00 <Mouse::Mouse()+0x3c> @ imm = #-0x30
  34cb2c: e284cf83     	add	r12, r4, #524
  34cb30: e3a00000     	mov	r0, #0
  34cb34: e5832014     	str	r2, [r3, #0x14]
  34cb38: e5832010     	str	r2, [r3, #0x10]
  34cb3c: e583200c     	str	r2, [r3, #0xc]
  34cb40: e5832004     	str	r2, [r3, #0x4]
  34cb44: e5832000     	str	r2, [r3]
  34cb48: e5831018     	str	r1, [r3, #0x18]
  34cb4c: e5831008     	str	r1, [r3, #0x8]
  34cb50: e5c3001c     	strb	r0, [r3, #0x1c]
  34cb54: e2833020     	add	r3, r3, #32
  34cb58: e153000c     	cmp	r3, r12
  34cb5c: 1afffff4     	bne	0x34cb34 <Mouse::Mouse()+0x70> @ imm = #-0x30
  34cb60: e1a00004     	mov	r0, r4
  34cb64: e8bd8070     	pop	{r4, r5, r6, pc}
  34cb68: b4 7f 64 00  	.word	0x00647fb4
  34cb6c: 04 2f 00 00  	.word	0x00002f04

0034cb70 <Mouse::Mouse()>:
  34cb70: e92d4070     	push	{r4, r5, r6, lr}
  34cb74: e3a01001     	mov	r1, #1
  34cb78: e59f5094     	ldr	r5, [pc, #0x94]         @ 0x34cc14 <Mouse::Mouse()+0xa4>
  34cb7c: e1a04000     	mov	r4, r0
  34cb80: ebffffb7     	bl	0x34ca64 <InputDevice::InputDevice(InputDevice::Type)> @ imm = #-0x124
  34cb84: e59f008c     	ldr	r0, [pc, #0x8c]         @ 0x34cc18 <Mouse::Mouse()+0xa8>
  34cb88: e08f5005     	add	r5, pc, r5
  34cb8c: e1a03004     	mov	r3, r4
  34cb90: e7950000     	ldr	r0, [r5, r0]
  34cb94: e3a02000     	mov	r2, #0
  34cb98: e3a015fe     	mov	r1, #1065353216
  34cb9c: e2800008     	add	r0, r0, #8
  34cba0: e483000c     	str	r0, [r3], #12
  34cba4: e284cf43     	add	r12, r4, #268
  34cba8: e3a00000     	mov	r0, #0
  34cbac: e5832014     	str	r2, [r3, #0x14]
  34cbb0: e5832010     	str	r2, [r3, #0x10]
  34cbb4: e583200c     	str	r2, [r3, #0xc]
  34cbb8: e5832004     	str	r2, [r3, #0x4]
  34cbbc: e5832000     	str	r2, [r3]
  34cbc0: e5831018     	str	r1, [r3, #0x18]
  34cbc4: e5831008     	str	r1, [r3, #0x8]
  34cbc8: e5c3001c     	strb	r0, [r3, #0x1c]
  34cbcc: e2833020     	add	r3, r3, #32
  34cbd0: e153000c     	cmp	r3, r12
  34cbd4: 1afffff4     	bne	0x34cbac <Mouse::Mouse()+0x3c> @ imm = #-0x30
  34cbd8: e284cf83     	add	r12, r4, #524
  34cbdc: e3a00000     	mov	r0, #0
  34cbe0: e5832014     	str	r2, [r3, #0x14]
  34cbe4: e5832010     	str	r2, [r3, #0x10]
  34cbe8: e583200c     	str	r2, [r3, #0xc]
  34cbec: e5832004     	str	r2, [r3, #0x4]
  34cbf0: e5832000     	str	r2, [r3]
  34cbf4: e5831018     	str	r1, [r3, #0x18]
  34cbf8: e5831008     	str	r1, [r3, #0x8]
  34cbfc: e5c3001c     	strb	r0, [r3, #0x1c]
  34cc00: e2833020     	add	r3, r3, #32
  34cc04: e153000c     	cmp	r3, r12
  34cc08: 1afffff4     	bne	0x34cbe0 <Mouse::Mouse()+0x70> @ imm = #-0x30
  34cc0c: e1a00004     	mov	r0, r4
  34cc10: e8bd8070     	pop	{r4, r5, r6, pc}
  34cc14: 08 7f 64 00  	.word	0x00647f08
  34cc18: 04 2f 00 00  	.word	0x00002f04

0034cc1c <Keyboard::Keyboard()>:
  34cc1c: e92d4070     	push	{r4, r5, r6, lr}
  34cc20: e3a01001     	mov	r1, #1
  34cc24: e59f4060     	ldr	r4, [pc, #0x60]         @ 0x34cc8c <Keyboard::Keyboard()+0x70>
  34cc28: e1a05000     	mov	r5, r0
  34cc2c: ebffff8c     	bl	0x34ca64 <InputDevice::InputDevice(InputDevice::Type)> @ imm = #-0x1d0
  34cc30: e59f0058     	ldr	r0, [pc, #0x58]         @ 0x34cc90 <Keyboard::Keyboard()+0x74>
  34cc34: e08f4004     	add	r4, pc, r4
  34cc38: e1a03005     	mov	r3, r5
  34cc3c: e7940000     	ldr	r0, [r4, r0]
  34cc40: e3a02000     	mov	r2, #0
  34cc44: e3a015fe     	mov	r1, #1065353216
  34cc48: e2800008     	add	r0, r0, #8
  34cc4c: e483000c     	str	r0, [r3], #12
  34cc50: e283cec2     	add	r12, r3, #3104
  34cc54: e3a00000     	mov	r0, #0
  34cc58: e5832014     	str	r2, [r3, #0x14]
  34cc5c: e5832010     	str	r2, [r3, #0x10]
  34cc60: e583200c     	str	r2, [r3, #0xc]
  34cc64: e5832004     	str	r2, [r3, #0x4]
  34cc68: e5832000     	str	r2, [r3]
  34cc6c: e5831018     	str	r1, [r3, #0x18]
  34cc70: e5831008     	str	r1, [r3, #0x8]
  34cc74: e5c3001c     	strb	r0, [r3, #0x1c]
  34cc78: e2833020     	add	r3, r3, #32
  34cc7c: e153000c     	cmp	r3, r12
  34cc80: 1afffff4     	bne	0x34cc58 <Keyboard::Keyboard()+0x3c> @ imm = #-0x30
  34cc84: e1a00005     	mov	r0, r5
  34cc88: e8bd8070     	pop	{r4, r5, r6, pc}
  34cc8c: 5c 7e 64 00  	.word	0x00647e5c
  34cc90: 10 3e 00 00  	.word	0x00003e10

0034cc94 <Keyboard::Keyboard()>:
  34cc94: e92d4070     	push	{r4, r5, r6, lr}
  34cc98: e3a01001     	mov	r1, #1
  34cc9c: e59f4060     	ldr	r4, [pc, #0x60]         @ 0x34cd04 <Keyboard::Keyboard()+0x70>
  34cca0: e1a05000     	mov	r5, r0
  34cca4: ebffff6e     	bl	0x34ca64 <InputDevice::InputDevice(InputDevice::Type)> @ imm = #-0x248
  34cca8: e59f0058     	ldr	r0, [pc, #0x58]         @ 0x34cd08 <Keyboard::Keyboard()+0x74>
  34ccac: e08f4004     	add	r4, pc, r4
  34ccb0: e1a03005     	mov	r3, r5
  34ccb4: e7940000     	ldr	r0, [r4, r0]
  34ccb8: e3a02000     	mov	r2, #0
  34ccbc: e3a015fe     	mov	r1, #1065353216
  34ccc0: e2800008     	add	r0, r0, #8
  34ccc4: e483000c     	str	r0, [r3], #12
  34ccc8: e283cec2     	add	r12, r3, #3104
  34cccc: e3a00000     	mov	r0, #0
  34ccd0: e5832014     	str	r2, [r3, #0x14]
  34ccd4: e5832010     	str	r2, [r3, #0x10]
  34ccd8: e583200c     	str	r2, [r3, #0xc]
  34ccdc: e5832004     	str	r2, [r3, #0x4]
  34cce0: e5832000     	str	r2, [r3]
  34cce4: e5831018     	str	r1, [r3, #0x18]
  34cce8: e5831008     	str	r1, [r3, #0x8]
  34ccec: e5c3001c     	strb	r0, [r3, #0x1c]
  34ccf0: e2833020     	add	r3, r3, #32
  34ccf4: e153000c     	cmp	r3, r12
  34ccf8: 1afffff4     	bne	0x34ccd0 <Keyboard::Keyboard()+0x3c> @ imm = #-0x30
  34ccfc: e1a00005     	mov	r0, r5
  34cd00: e8bd8070     	pop	{r4, r5, r6, pc}
  34cd04: e4 7d 64 00  	.word	0x00647de4
  34cd08: 10 3e 00 00  	.word	0x00003e10
