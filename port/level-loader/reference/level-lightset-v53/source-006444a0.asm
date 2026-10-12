
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

006444a0 <virtual thunk to glitch::collada::CLightSceneNode::~CLightSceneNode()>:
  6444a0: e5903000     	ldr	r3, [r0]
  6444a4: e513300c     	ldr	r3, [r3, #-0xc]
  6444a8: e0800003     	add	r0, r0, r3
  6444ac: eaffffe1     	b	0x644438 <glitch::collada::CLightSceneNode::~CLightSceneNode()> @ imm = #-0x7c
