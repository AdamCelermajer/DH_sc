
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003938a0 <GameObject::SetRotation(Point3D<float> const&)>:
  3938a0: e5913000     	ldr	r3, [r1]
  3938a4: e92d4010     	push	{r4, lr}
  3938a8: e580316c     	str	r3, [r0, #0x16c]
  3938ac: e5913004     	ldr	r3, [r1, #0x4]
  3938b0: e59022d8     	ldr	r2, [r0, #0x2d8]
  3938b4: e1a04000     	mov	r4, r0
  3938b8: e5803170     	str	r3, [r0, #0x170]
  3938bc: e5913008     	ldr	r3, [r1, #0x8]
  3938c0: e3520000     	cmp	r2, #0
  3938c4: e5803174     	str	r3, [r0, #0x174]
  3938c8: e5913008     	ldr	r3, [r1, #0x8]
  3938cc: e5803178     	str	r3, [r0, #0x178]
  3938d0: 0a000007     	beq	0x3938f4 <GameObject::SetRotation(Point3D<float> const&)+0x54> @ imm = #0x1c
  3938d4: e5903000     	ldr	r3, [r0]
  3938d8: e1a0e00f     	mov	lr, pc
  3938dc: e593f070     	ldr	pc, [r3, #0x70]
  3938e0: e3500000     	cmp	r0, #0
  3938e4: 0a000002     	beq	0x3938f4 <GameObject::SetRotation(Point3D<float> const&)+0x54> @ imm = #0x8
  3938e8: e59402d8     	ldr	r0, [r4, #0x2d8]
  3938ec: e8bd4010     	pop	{r4, lr}
  3938f0: ea037c14     	b	0x472948 <VisualObject::SyncRotation()> @ imm = #0xdf050
  3938f4: e8bd8010     	pop	{r4, pc}
