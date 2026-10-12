
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

005b5e00 <glitch::video::COpenGLES2Driver::COpenGLES2Driver(glitch::IDevice*)>:
  5b5e00: e92d4070     	push	{r4, r5, r6, lr}
  5b5e04: e59f4020     	ldr	r4, [pc, #0x20]         @ 0x5b5e2c <glitch::video::COpenGLES2Driver::COpenGLES2Driver(glitch::IDevice*)+0x2c>
  5b5e08: e1a05000     	mov	r5, r0
  5b5e0c: ebffff8b     	bl	0x5b5c40 <glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>::CProgrammableGLDriver(glitch::IDevice*)> @ imm = #-0x1d4
  5b5e10: e59f3018     	ldr	r3, [pc, #0x18]         @ 0x5b5e30 <glitch::video::COpenGLES2Driver::COpenGLES2Driver(glitch::IDevice*)+0x30>
  5b5e14: e08f4004     	add	r4, pc, r4
  5b5e18: e1a00005     	mov	r0, r5
  5b5e1c: e7943003     	ldr	r3, [r4, r3]
  5b5e20: e2833008     	add	r3, r3, #8
  5b5e24: e5853000     	str	r3, [r5]
  5b5e28: e8bd8070     	pop	{r4, r5, r6, pc}
  5b5e2c: 7c ec 3d 00  	.word	0x003dec7c
  5b5e30: 6c 2e 00 00  	.word	0x00002e6c
