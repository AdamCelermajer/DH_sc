
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0038be5c <GameObject::InitPost()>:
  38be5c: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  38be60: e1a04000     	mov	r4, r0
  38be64: ebfecb68     	bl	0x33ec0c <ObjectBase::InitPost()> @ imm = #-0x4d260
  38be68: e1a00004     	mov	r0, r4
  38be6c: ebffffbc     	bl	0x38bd64 <GameObject::CheckSpawnProbability()> @ imm = #-0x110
  38be70: e5943274     	ldr	r3, [r4, #0x274]
  38be74: e59f5204     	ldr	r5, [pc, #0x204]        @ 0x38c080 <GameObject::InitPost()+0x224>
  38be78: e1500003     	cmp	r0, r3
  38be7c: e08f5005     	add	r5, pc, r5
  38be80: aa000069     	bge	0x38c02c <GameObject::InitPost()+0x1d0> @ imm = #0x1a4
  38be84: e5940120     	ldr	r0, [r4, #0x120]
  38be88: e3a03000     	mov	r3, #0
  38be8c: e30b1717     	movw	r1, #0xb717
  38be90: e58432dc     	str	r3, [r4, #0x2dc]
  38be94: e34318d1     	movt	r1, #0x38d1
  38be98: e3c00102     	bic	r0, r0, #-2147483648
  38be9c: ebfe0a1a     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x7d798
  38bea0: e3500000     	cmp	r0, #0
  38bea4: e5940124     	ldr	r0, [r4, #0x124]
  38bea8: 13a035fe     	movne	r3, #1065353216
  38beac: e30b1717     	movw	r1, #0xb717
  38beb0: 15843120     	strne	r3, [r4, #0x120]
  38beb4: e34318d1     	movt	r1, #0x38d1
  38beb8: e3c00102     	bic	r0, r0, #-2147483648
  38bebc: ebfe0a12     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x7d7b8
  38bec0: e3500000     	cmp	r0, #0
  38bec4: e5940128     	ldr	r0, [r4, #0x128]
  38bec8: 13a035fe     	movne	r3, #1065353216
  38becc: e30b1717     	movw	r1, #0xb717
  38bed0: 15843124     	strne	r3, [r4, #0x124]
  38bed4: e34318d1     	movt	r1, #0x38d1
  38bed8: e3c00102     	bic	r0, r0, #-2147483648
  38bedc: ebfe0a0a     	bl	0x30e70c <.plt+0x998>   @ imm = #-0x7d7d8
  38bee0: e3500000     	cmp	r0, #0
  38bee4: 13a035fe     	movne	r3, #1065353216
  38bee8: e30f1a35     	movw	r1, #0xfa35
  38beec: 15843128     	strne	r3, [r4, #0x128]
  38bef0: e594016c     	ldr	r0, [r4, #0x16c]
  38bef4: e3431c8e     	movt	r1, #0x3c8e
  38bef8: ebfe0b9b     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x7d194
  38befc: e30f1a35     	movw	r1, #0xfa35
  38bf00: e584016c     	str	r0, [r4, #0x16c]
  38bf04: e3431c8e     	movt	r1, #0x3c8e
  38bf08: e5940170     	ldr	r0, [r4, #0x170]
  38bf0c: ebfe0b96     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x7d1a8
  38bf10: e30f1a35     	movw	r1, #0xfa35
  38bf14: e5840170     	str	r0, [r4, #0x170]
  38bf18: e3431c8e     	movt	r1, #0x3c8e
  38bf1c: e5940174     	ldr	r0, [r4, #0x174]
  38bf20: ebfe0b91     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x7d1bc
  38bf24: e3a02001     	mov	r2, #1
  38bf28: e5840178     	str	r0, [r4, #0x178]
  38bf2c: e5840174     	str	r0, [r4, #0x174]
  38bf30: e2841e16     	add	r1, r4, #352
  38bf34: e1a00004     	mov	r0, r4
  38bf38: eb001f9d     	bl	0x393db4 <GameObject::SetPosition(Point3D<float> const&, bool)> @ imm = #0x7e74
  38bf3c: e5940144     	ldr	r0, [r4, #0x144]
  38bf40: e5941120     	ldr	r1, [r4, #0x120]
  38bf44: ebfe0b88     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x7d1e0
  38bf48: e5941124     	ldr	r1, [r4, #0x124]
  38bf4c: e5840144     	str	r0, [r4, #0x144]
  38bf50: e5940148     	ldr	r0, [r4, #0x148]
  38bf54: ebfe0b84     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x7d1f0
  38bf58: e5941128     	ldr	r1, [r4, #0x128]
  38bf5c: e5840148     	str	r0, [r4, #0x148]
  38bf60: e594014c     	ldr	r0, [r4, #0x14c]
  38bf64: ebfe0b80     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x7d200
  38bf68: e5941120     	ldr	r1, [r4, #0x120]
  38bf6c: e584014c     	str	r0, [r4, #0x14c]
  38bf70: e5940150     	ldr	r0, [r4, #0x150]
  38bf74: ebfe0b7c     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x7d210
  38bf78: e5941124     	ldr	r1, [r4, #0x124]
  38bf7c: e5840150     	str	r0, [r4, #0x150]
  38bf80: e5940154     	ldr	r0, [r4, #0x154]
  38bf84: ebfe0b78     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x7d220
  38bf88: e5941128     	ldr	r1, [r4, #0x128]
  38bf8c: e5840154     	str	r0, [r4, #0x154]
  38bf90: e5940158     	ldr	r0, [r4, #0x158]
  38bf94: ebfe0b74     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x7d230
  38bf98: e5840158     	str	r0, [r4, #0x158]
  38bf9c: e1a00004     	mov	r0, r4
  38bfa0: ebfffac8     	bl	0x38aac8 <GameObject::UpdateAbsoluteAABB()> @ imm = #-0x14e0
  38bfa4: e59402d8     	ldr	r0, [r4, #0x2d8]
  38bfa8: e3500000     	cmp	r0, #0
  38bfac: 0a000023     	beq	0x38c040 <GameObject::InitPost()+0x1e4> @ imm = #0x8c
  38bfb0: ebfffeaf     	bl	0x38ba74 <VisualObject::Sync()> @ imm = #-0x544
  38bfb4: e5d4315c     	ldrb	r3, [r4, #0x15c]
  38bfb8: e594736c     	ldr	r7, [r4, #0x36c]
  38bfbc: e3530000     	cmp	r3, #0
  38bfc0: 13a03000     	movne	r3, #0
  38bfc4: 15c43028     	strbne	r3, [r4, #0x28]
  38bfc8: e5943368     	ldr	r3, [r4, #0x368]
  38bfcc: e1530007     	cmp	r3, r7
  38bfd0: 0a000015     	beq	0x38c02c <GameObject::InitPost()+0x1d0> @ imm = #0x54
  38bfd4: e59f30a8     	ldr	r3, [pc, #0xa8]         @ 0x38c084 <GameObject::InitPost()+0x228>
  38bfd8: e7953003     	ldr	r3, [r5, r3]
  38bfdc: e5938000     	ldr	r8, [r3]
  38bfe0: e3580000     	cmp	r8, #0
  38bfe4: 0a000011     	beq	0x38c030 <GameObject::InitPost()+0x1d4> @ imm = #0x44
  38bfe8: e59f3098     	ldr	r3, [pc, #0x98]         @ 0x38c088 <GameObject::InitPost()+0x22c>
  38bfec: e3a06000     	mov	r6, #0
  38bff0: e7953003     	ldr	r3, [r5, r3]
  38bff4: e5935000     	ldr	r5, [r3]
  38bff8: ea000002     	b	0x38c008 <GameObject::InitPost()+0x1ac> @ imm = #0x8
  38bffc: e2866001     	add	r6, r6, #1
  38c000: e1560008     	cmp	r6, r8
  38c004: 0a000009     	beq	0x38c030 <GameObject::InitPost()+0x1d4> @ imm = #0x24
  38c008: e7951106     	ldr	r1, [r5, r6, lsl #2]
  38c00c: e1a00007     	mov	r0, r7
  38c010: ebfe08c1     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x7dcfc
  38c014: e3500000     	cmp	r0, #0
  38c018: 1afffff7     	bne	0x38bffc <GameObject::InitPost()+0x1a0> @ imm = #-0x24
  38c01c: e6ff6076     	uxth	r6, r6
  38c020: e3a03e37     	mov	r3, #880
  38c024: e18460b3     	strh	r6, [r4, r3]
  38c028: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  38c02c: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  38c030: e30f6fff     	movw	r6, #0xffff
  38c034: e3a03e37     	mov	r3, #880
  38c038: e18460b3     	strh	r6, [r4, r3]
  38c03c: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  38c040: ebffd5c1     	bl	0x38174c <Device::IsHighPerformance()> @ imm = #-0xa8fc
  38c044: e3500000     	cmp	r0, #0
  38c048: 1a000006     	bne	0x38c068 <GameObject::InitPost()+0x20c> @ imm = #0x18
  38c04c: e5d43060     	ldrb	r3, [r4, #0x60]
  38c050: e3530000     	cmp	r3, #0
  38c054: 0a000003     	beq	0x38c068 <GameObject::InitPost()+0x20c> @ imm = #0xc
  38c058: e5d4310c     	ldrb	r3, [r4, #0x10c]
  38c05c: e3530000     	cmp	r3, #0
  38c060: 158402d8     	strne	r0, [r4, #0x2d8]
  38c064: 1affffd2     	bne	0x38bfb4 <GameObject::InitPost()+0x158> @ imm = #-0xb8
  38c068: e1a00004     	mov	r0, r4
  38c06c: eb00238f     	bl	0x394eb0 <GameObject::LoadVisualObject()> @ imm = #0x8e3c
  38c070: e59402d8     	ldr	r0, [r4, #0x2d8]
  38c074: e3500000     	cmp	r0, #0
  38c078: 0affffcd     	beq	0x38bfb4 <GameObject::InitPost()+0x158> @ imm = #-0xcc
  38c07c: eaffffcb     	b	0x38bfb0 <GameObject::InitPost()+0x154> @ imm = #-0xd4
  38c080: 14 8c 60 00  	.word	0x00608c14
  38c084: 38 3d 00 00  	.word	0x00003d38
  38c088: a8 39 00 00  	.word	0x000039a8
