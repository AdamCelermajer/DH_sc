
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

005b5dac <glitch::video::createOpenGLES2Driver(glitch::IDevice*)>:
  5b5dac: e92d4070     	push	{r4, r5, r6, lr}
  5b5db0: e3a01000     	mov	r1, #0
  5b5db4: e1a05000     	mov	r5, r0
  5b5db8: e3000df4     	movw	r0, #0xdf4
  5b5dbc: ebfdf8fa     	bl	0x5341ac <operator new(unsigned int, glitch::memory::E_MEMORY_HINT)> @ imm = #-0x81c18
  5b5dc0: e1a01005     	mov	r1, r5
  5b5dc4: e1a04000     	mov	r4, r0
  5b5dc8: ebffffea     	bl	0x5b5d78 <glitch::video::COpenGLES2Driver::COpenGLES2Driver(glitch::IDevice*)> @ imm = #-0x58
  5b5dcc: e59410d4     	ldr	r1, [r4, #0xd4]
  5b5dd0: e1a00004     	mov	r0, r4
  5b5dd4: e5d1206b     	ldrb	r2, [r1, #0x6b]
  5b5dd8: e2811060     	add	r1, r1, #96
  5b5ddc: ebfff746     	bl	0x5b3afc <glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>::genericDriverInit(glitch::core::dimension2d<int> const&, bool)> @ imm = #-0x22e8
  5b5de0: e2505000     	subs	r5, r0, #0
  5b5de4: 0a000001     	beq	0x5b5df0 <glitch::video::createOpenGLES2Driver(glitch::IDevice*)+0x44> @ imm = #0x4
  5b5de8: e1a00004     	mov	r0, r4
  5b5dec: e8bd8070     	pop	{r4, r5, r6, pc}
  5b5df0: e1a00004     	mov	r0, r4
  5b5df4: ebf59de2     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x298878
  5b5df8: e1a00005     	mov	r0, r5
  5b5dfc: e8bd8070     	pop	{r4, r5, r6, pc}
