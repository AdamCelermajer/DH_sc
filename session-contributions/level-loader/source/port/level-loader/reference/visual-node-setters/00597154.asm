
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00597154 <glitch::scene::ISceneNode::setPosition(float, float, float)>:
  597154: e52de004     	str	lr, [sp, #-0x4]!
  597158: e24dd014     	sub	sp, sp, #20
  59715c: e58d1004     	str	r1, [sp, #0x4]
  597160: e58d300c     	str	r3, [sp, #0xc]
  597164: e58d2008     	str	r2, [sp, #0x8]
  597168: e5903000     	ldr	r3, [r0]
  59716c: e28d1004     	add	r1, sp, #4
  597170: e1a0e00f     	mov	lr, pc
  597174: e593f0a4     	ldr	pc, [r3, #0xa4]
  597178: e28dd014     	add	sp, sp, #20
  59717c: e8bd8000     	ldm	sp!, {pc}
