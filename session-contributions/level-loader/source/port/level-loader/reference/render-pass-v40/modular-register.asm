
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0035a8e8 <ModularSkinnedMeshSceneNode::onRegisterSceneNode()>:
  35a8e8: e92d4010     	push	{r4, lr}
  35a8ec: e5903110     	ldr	r3, [r0, #0x110]
  35a8f0: e1a04000     	mov	r4, r0
  35a8f4: e2801d06     	add	r1, r0, #384
  35a8f8: e1a00003     	mov	r0, r3
  35a8fc: e5933000     	ldr	r3, [r3]
  35a900: e1a0e00f     	mov	lr, pc
  35a904: e593f088     	ldr	pc, [r3, #0x88]
  35a908: e1a00004     	mov	r0, r4
  35a90c: e8bd4010     	pop	{r4, lr}
  35a910: ea0baeac     	b	0x6463c8 <glitch::collada::CMeshSceneNode::onRegisterSceneNode()> @ imm = #0x2ebab0
