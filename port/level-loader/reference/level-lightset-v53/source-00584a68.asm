
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00584a68 <virtual thunk to glitch::scene::CLightSceneNode::~CLightSceneNode()>:
  584a68: e5903000     	ldr	r3, [r0]
  584a6c: e513300c     	ldr	r3, [r3, #-0xc]
  584a70: e0800003     	add	r0, r0, r3
  584a74: eaffff8a     	b	0x5848a4 <glitch::scene::CLightSceneNode::~CLightSceneNode()> @ imm = #-0x1d8
