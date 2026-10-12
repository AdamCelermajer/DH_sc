
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

006444cc <virtual thunk to glitch::collada::CLightSceneNode::~CLightSceneNode()>:
  6444cc: e5903000     	ldr	r3, [r0]
  6444d0: e5133018     	ldr	r3, [r3, #-0x18]
  6444d4: e0800003     	add	r0, r0, r3
  6444d8: eafffff4     	b	0x6444b0 <glitch::collada::CLightSceneNode::~CLightSceneNode()> @ imm = #-0x30
