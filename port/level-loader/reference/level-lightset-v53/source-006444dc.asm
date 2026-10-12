
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

006444dc <virtual thunk to glitch::collada::CLightSceneNode::~CLightSceneNode()>:
  6444dc: e5903000     	ldr	r3, [r0]
  6444e0: e513300c     	ldr	r3, [r3, #-0xc]
  6444e4: e0800003     	add	r0, r0, r3
  6444e8: eafffff0     	b	0x6444b0 <glitch::collada::CLightSceneNode::~CLightSceneNode()> @ imm = #-0x40
