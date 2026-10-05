
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

005970f4 <glitch::scene::ISceneNode::setRotation(glitch::core::quaternion const&)>:
  5970f4: e5913000     	ldr	r3, [r1]
  5970f8: e590211c     	ldr	r2, [r0, #0x11c]
  5970fc: e58030b8     	str	r3, [r0, #0xb8]
  597100: e5913004     	ldr	r3, [r1, #0x4]
  597104: e3822004     	orr	r2, r2, #4
  597108: e58030bc     	str	r3, [r0, #0xbc]
  59710c: e5913008     	ldr	r3, [r1, #0x8]
  597110: e58030c0     	str	r3, [r0, #0xc0]
  597114: e591300c     	ldr	r3, [r1, #0xc]
  597118: e580211c     	str	r2, [r0, #0x11c]
  59711c: e58030c4     	str	r3, [r0, #0xc4]
  597120: e12fff1e     	bx	lr
