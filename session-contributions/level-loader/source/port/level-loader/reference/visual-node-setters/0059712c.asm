
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0059712c <glitch::scene::ISceneNode::setPosition(glitch::core::vector3d<float> const&)>:
  59712c: e5913000     	ldr	r3, [r1]
  597130: e590211c     	ldr	r2, [r0, #0x11c]
  597134: e58030ac     	str	r3, [r0, #0xac]
  597138: e5913004     	ldr	r3, [r1, #0x4]
  59713c: e3822008     	orr	r2, r2, #8
  597140: e58030b0     	str	r3, [r0, #0xb0]
  597144: e5913008     	ldr	r3, [r1, #0x8]
  597148: e580211c     	str	r2, [r0, #0x11c]
  59714c: e58030b4     	str	r3, [r0, #0xb4]
  597150: e12fff1e     	bx	lr
