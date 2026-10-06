
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0035a914 <SkinnedMeshSceneNode::onRegisterSceneNode()>:
  35a914: e92d4010     	push	{r4, lr}
  35a918: e5903110     	ldr	r3, [r0, #0x110]
  35a91c: e1a04000     	mov	r4, r0
  35a920: e2801d06     	add	r1, r0, #384
  35a924: e1a00003     	mov	r0, r3
  35a928: e5933000     	ldr	r3, [r3]
  35a92c: e1a0e00f     	mov	lr, pc
  35a930: e593f088     	ldr	pc, [r3, #0x88]
  35a934: e1a00004     	mov	r0, r4
  35a938: e8bd4010     	pop	{r4, lr}
  35a93c: ea0baea1     	b	0x6463c8 <glitch::collada::CMeshSceneNode::onRegisterSceneNode()> @ imm = #0x2eba84
