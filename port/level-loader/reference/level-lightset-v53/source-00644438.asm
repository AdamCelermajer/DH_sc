
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00644438 <glitch::collada::CLightSceneNode::~CLightSceneNode()>:
  644438: e92d4070     	push	{r4, r5, r6, lr}
  64443c: e59f5040     	ldr	r5, [pc, #0x40]         @ 0x644484 <glitch::collada::CLightSceneNode::~CLightSceneNode()+0x4c>
  644440: e59f3040     	ldr	r3, [pc, #0x40]         @ 0x644488 <glitch::collada::CLightSceneNode::~CLightSceneNode()+0x50>
  644444: e1a04000     	mov	r4, r0
  644448: e08f5005     	add	r5, pc, r5
  64444c: e7953003     	ldr	r3, [r5, r3]
  644450: e2800f56     	add	r0, r0, #344
  644454: e2832e12     	add	r2, r3, #288
  644458: e283301c     	add	r3, r3, #28
  64445c: e5843000     	str	r3, [r4]
  644460: e5842164     	str	r2, [r4, #0x164]
  644464: ebff5402     	bl	0x619474 <glitch::collada::CColladaDatabase::~CColladaDatabase()> @ imm = #-0x2aff8
  644468: e59f101c     	ldr	r1, [pc, #0x1c]         @ 0x64448c <glitch::collada::CLightSceneNode::~CLightSceneNode()+0x54>
  64446c: e1a00004     	mov	r0, r4
  644470: e7951001     	ldr	r1, [r5, r1]
  644474: e2811004     	add	r1, r1, #4
  644478: ebfd013f     	bl	0x58497c <glitch::scene::CLightSceneNode::~CLightSceneNode()> @ imm = #-0xbfb04
  64447c: e1a00004     	mov	r0, r4
  644480: e8bd8070     	pop	{r4, r5, r6, pc}
  644484: 48 06 35 00  	.word	0x00350648
  644488: 48 10 00 00  	.word	0x00001048
  64448c: dc 3a 00 00  	.word	0x00003adc
