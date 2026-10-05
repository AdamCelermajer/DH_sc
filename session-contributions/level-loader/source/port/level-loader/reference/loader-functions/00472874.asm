
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00472874 <VisualObject::SetRotation(Point3D<float> const&)>:
  472874: e92d4070     	push	{r4, r5, r6, lr}
  472878: e5903008     	ldr	r3, [r0, #0x8]
  47287c: e24dd010     	sub	sp, sp, #16
  472880: e1a04000     	mov	r4, r0
  472884: e3530000     	cmp	r3, #0
  472888: 0a00001c     	beq	0x472900 <VisualObject::SetRotation(Point3D<float> const&)+0x8c> @ imm = #0x70
  47288c: e5912000     	ldr	r2, [r1]
  472890: e5913008     	ldr	r3, [r1, #0x8]
  472894: e1a0000d     	mov	r0, sp
  472898: e2822102     	add	r2, r2, #-2147483648
  47289c: e5911004     	ldr	r1, [r1, #0x4]
  4728a0: e2833102     	add	r3, r3, #-2147483648
  4728a4: ebfba84b     	bl	0x35c9d8 <glitch::core::quaternion::set(float, float, float)> @ imm = #-0x115ed4
  4728a8: e5943008     	ldr	r3, [r4, #0x8]
  4728ac: e1a0500d     	mov	r5, sp
  4728b0: e1a00003     	mov	r0, r3
  4728b4: e5933000     	ldr	r3, [r3]
  4728b8: e1a0e00f     	mov	lr, pc
  4728bc: e593f098     	ldr	pc, [r3, #0x98]
  4728c0: e59d1000     	ldr	r1, [sp]
  4728c4: e1a06000     	mov	r6, r0
  4728c8: e5900000     	ldr	r0, [r0]
  4728cc: ebfa6dae     	bl	0x30df8c <.plt+0x218>   @ imm = #-0x164948
  4728d0: e3500000     	cmp	r0, #0
  4728d4: 1a00000b     	bne	0x472908 <VisualObject::SetRotation(Point3D<float> const&)+0x94> @ imm = #0x2c
  4728d8: e5943008     	ldr	r3, [r4, #0x8]
  4728dc: e1a0100d     	mov	r1, sp
  4728e0: e1a00003     	mov	r0, r3
  4728e4: e5933000     	ldr	r3, [r3]
  4728e8: e1a0e00f     	mov	lr, pc
  4728ec: e593f09c     	ldr	pc, [r3, #0x9c]
  4728f0: e1a00004     	mov	r0, r4
  4728f4: ebfffe08     	bl	0x47211c <VisualObject::CalcMeshBox()> @ imm = #-0x7e0
  4728f8: e1a00004     	mov	r0, r4
  4728fc: ebfff854     	bl	0x470a54 <VisualObject::ApplyMeshBox()> @ imm = #-0x1eb0
  472900: e28dd010     	add	sp, sp, #16
  472904: e8bd8070     	pop	{r4, r5, r6, pc}
  472908: e5960004     	ldr	r0, [r6, #0x4]
  47290c: e59d1004     	ldr	r1, [sp, #0x4]
  472910: ebfa6d9d     	bl	0x30df8c <.plt+0x218>   @ imm = #-0x16498c
  472914: e3500000     	cmp	r0, #0
  472918: 0affffee     	beq	0x4728d8 <VisualObject::SetRotation(Point3D<float> const&)+0x64> @ imm = #-0x48
  47291c: e5960008     	ldr	r0, [r6, #0x8]
  472920: e59d1008     	ldr	r1, [sp, #0x8]
  472924: ebfa6d98     	bl	0x30df8c <.plt+0x218>   @ imm = #-0x1649a0
  472928: e3500000     	cmp	r0, #0
  47292c: 0affffe9     	beq	0x4728d8 <VisualObject::SetRotation(Point3D<float> const&)+0x64> @ imm = #-0x5c
  472930: e596000c     	ldr	r0, [r6, #0xc]
  472934: e59d100c     	ldr	r1, [sp, #0xc]
  472938: ebfa6d93     	bl	0x30df8c <.plt+0x218>   @ imm = #-0x1649b4
  47293c: e3500000     	cmp	r0, #0
  472940: 1affffee     	bne	0x472900 <VisualObject::SetRotation(Point3D<float> const&)+0x8c> @ imm = #-0x48
  472944: eaffffe3     	b	0x4728d8 <VisualObject::SetRotation(Point3D<float> const&)+0x64> @ imm = #-0x74
