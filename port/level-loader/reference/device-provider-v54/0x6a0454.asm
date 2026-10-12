
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

006a0454 <glitch::CAndroidOSDevice::createDriver()>:
  6a0454: e92d4010     	push	{r4, lr}
  6a0458: e590305c     	ldr	r3, [r0, #0x5c]
  6a045c: e1a04000     	mov	r4, r0
  6a0460: e3530001     	cmp	r3, #1
  6a0464: 0a00000e     	beq	0x6a04a4 <glitch::CAndroidOSDevice::createDriver()+0x50> @ imm = #0x38
  6a0468: da000012     	ble	0x6a04b8 <glitch::CAndroidOSDevice::createDriver()+0x64> @ imm = #0x48
  6a046c: e3530080     	cmp	r3, #128
  6a0470: 0a000006     	beq	0x6a0490 <glitch::CAndroidOSDevice::createDriver()+0x3c> @ imm = #0x18
  6a0474: e3530c01     	cmp	r3, #256
  6a0478: 0a000004     	beq	0x6a0490 <glitch::CAndroidOSDevice::createDriver()+0x3c> @ imm = #0x10
  6a047c: e59f005c     	ldr	r0, [pc, #0x5c]         @ 0x6a04e0 <glitch::CAndroidOSDevice::createDriver()+0x8c>
  6a0480: e3a01003     	mov	r1, #3
  6a0484: e08f0000     	add	r0, pc, r0
  6a0488: e8bd4010     	pop	{r4, lr}
  6a048c: eafdaa03     	b	0x60aca0 <glitch::os::Printer::log(char const*, glitch::ELOG_LEVEL)> @ imm = #-0x957f4
  6a0490: e59f004c     	ldr	r0, [pc, #0x4c]         @ 0x6a04e4 <glitch::CAndroidOSDevice::createDriver()+0x90>
  6a0494: e3a01003     	mov	r1, #3
  6a0498: e08f0000     	add	r0, pc, r0
  6a049c: e8bd4010     	pop	{r4, lr}
  6a04a0: eafda9fe     	b	0x60aca0 <glitch::os::Printer::log(char const*, glitch::ELOG_LEVEL)> @ imm = #-0x95808
  6a04a4: ebfc5640     	bl	0x5b5dac <glitch::video::createOpenGLES2Driver(glitch::IDevice*)> @ imm = #-0xea700
  6a04a8: e3500000     	cmp	r0, #0
  6a04ac: e5840010     	str	r0, [r4, #0x10]
  6a04b0: 0a000005     	beq	0x6a04cc <glitch::CAndroidOSDevice::createDriver()+0x78> @ imm = #0x14
  6a04b4: e8bd8010     	pop	{r4, pc}
  6a04b8: e3530000     	cmp	r3, #0
  6a04bc: 1affffee     	bne	0x6a047c <glitch::CAndroidOSDevice::createDriver()+0x28> @ imm = #-0x48
  6a04c0: ebfc659c     	bl	0x5b9b38 <glitch::video::createNullDriver(glitch::IDevice*)> @ imm = #-0xe6990
  6a04c4: e5840010     	str	r0, [r4, #0x10]
  6a04c8: e8bd8010     	pop	{r4, pc}
  6a04cc: e59f0014     	ldr	r0, [pc, #0x14]         @ 0x6a04e8 <glitch::CAndroidOSDevice::createDriver()+0x94>
  6a04d0: e3a01003     	mov	r1, #3
  6a04d4: e08f0000     	add	r0, pc, r0
  6a04d8: e8bd4010     	pop	{r4, lr}
  6a04dc: eafda9ef     	b	0x60aca0 <glitch::os::Printer::log(char const*, glitch::ELOG_LEVEL)> @ imm = #-0x95844
  6a04e0: fc aa 24 00  	.word	0x0024aafc
  6a04e4: a0 aa 24 00  	.word	0x0024aaa0
  6a04e8: 44 aa 24 00  	.word	0x0024aa44
