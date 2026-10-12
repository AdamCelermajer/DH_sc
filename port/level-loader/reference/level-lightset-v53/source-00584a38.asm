
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00584a38 <virtual thunk to glitch::scene::CLightSceneNode::~CLightSceneNode()>:
  584a38: e5903000     	ldr	r3, [r0]
  584a3c: e5133018     	ldr	r3, [r3, #-0x18]
  584a40: e0800003     	add	r0, r0, r3
  584a44: eaffffc5     	b	0x584960 <glitch::scene::CLightSceneNode::~CLightSceneNode()> @ imm = #-0xec
