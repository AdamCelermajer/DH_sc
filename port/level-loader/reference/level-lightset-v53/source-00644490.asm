
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00644490 <virtual thunk to glitch::collada::CLightSceneNode::~CLightSceneNode()>:
  644490: e5903000     	ldr	r3, [r0]
  644494: e5133018     	ldr	r3, [r3, #-0x18]
  644498: e0800003     	add	r0, r0, r3
  64449c: eaffffe5     	b	0x644438 <glitch::collada::CLightSceneNode::~CLightSceneNode()> @ imm = #-0x6c
