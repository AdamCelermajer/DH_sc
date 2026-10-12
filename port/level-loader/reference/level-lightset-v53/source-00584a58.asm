
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00584a58 <virtual thunk to glitch::scene::CLightSceneNode::~CLightSceneNode()>:
  584a58: e5903000     	ldr	r3, [r0]
  584a5c: e5133018     	ldr	r3, [r3, #-0x18]
  584a60: e0800003     	add	r0, r0, r3
  584a64: eaffff8e     	b	0x5848a4 <glitch::scene::CLightSceneNode::~CLightSceneNode()> @ imm = #-0x1c8
