
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

005b5d78 <glitch::video::COpenGLES2Driver::COpenGLES2Driver(glitch::IDevice*)>:
  5b5d78: e92d4070     	push	{r4, r5, r6, lr}
  5b5d7c: e59f4020     	ldr	r4, [pc, #0x20]         @ 0x5b5da4 <glitch::video::COpenGLES2Driver::COpenGLES2Driver(glitch::IDevice*)+0x2c>
  5b5d80: e1a05000     	mov	r5, r0
  5b5d84: ebffffad     	bl	0x5b5c40 <glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>::CProgrammableGLDriver(glitch::IDevice*)> @ imm = #-0x14c
  5b5d88: e59f3018     	ldr	r3, [pc, #0x18]         @ 0x5b5da8 <glitch::video::COpenGLES2Driver::COpenGLES2Driver(glitch::IDevice*)+0x30>
  5b5d8c: e08f4004     	add	r4, pc, r4
  5b5d90: e1a00005     	mov	r0, r5
  5b5d94: e7943003     	ldr	r3, [r4, r3]
  5b5d98: e2833008     	add	r3, r3, #8
  5b5d9c: e5853000     	str	r3, [r5]
  5b5da0: e8bd8070     	pop	{r4, r5, r6, pc}
  5b5da4: 04 ed 3d 00  	.word	0x003ded04
  5b5da8: 6c 2e 00 00  	.word	0x00002e6c
