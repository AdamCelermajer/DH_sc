
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

005970c4 <glitch::scene::ISceneNode::setScale(glitch::core::vector3d<float> const&)>:
  5970c4: e5913000     	ldr	r3, [r1]
  5970c8: e590211c     	ldr	r2, [r0, #0x11c]
  5970cc: e58030c8     	str	r3, [r0, #0xc8]
  5970d0: e5913004     	ldr	r3, [r1, #0x4]
  5970d4: e3822002     	orr	r2, r2, #2
  5970d8: e58030cc     	str	r3, [r0, #0xcc]
  5970dc: e5913008     	ldr	r3, [r1, #0x8]
  5970e0: e580211c     	str	r2, [r0, #0x11c]
  5970e4: e58030d0     	str	r3, [r0, #0xd0]
  5970e8: e12fff1e     	bx	lr
