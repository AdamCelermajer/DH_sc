
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003b379c <Character::InitSpawned(int, Point3D<float> const&)>:
  3b379c: e30133c8     	movw	r3, #0x13c8
  3b37a0: e92d4070     	push	{r4, r5, r6, lr}
  3b37a4: e18010b3     	strh	r1, [r0, r3]
  3b37a8: e3a05001     	mov	r5, #1
  3b37ac: e3013481     	movw	r3, #0x1481
  3b37b0: e1a04000     	mov	r4, r0
  3b37b4: e7c05003     	strb	r5, [r0, r3]
  3b37b8: e1a01002     	mov	r1, r2
  3b37bc: e5c052f0     	strb	r5, [r0, #0x2f0]
  3b37c0: ebffc84b     	bl	0x3a58f4 <Character::SetInitialPosition(Point3D<float> const&)> @ imm = #-0xded4
  3b37c4: e2841d51     	add	r1, r4, #5184
  3b37c8: e1a02005     	mov	r2, r5
  3b37cc: e2811010     	add	r1, r1, #16
  3b37d0: e1a00004     	mov	r0, r4
  3b37d4: ebff8176     	bl	0x393db4 <GameObject::SetPosition(Point3D<float> const&, bool)> @ imm = #-0x1fa28
  3b37d8: e1a00004     	mov	r0, r4
  3b37dc: e5943000     	ldr	r3, [r4]
  3b37e0: e1a0e00f     	mov	lr, pc
  3b37e4: e593f01c     	ldr	pc, [r3, #0x1c]
  3b37e8: e1a00004     	mov	r0, r4
  3b37ec: e5943000     	ldr	r3, [r4]
  3b37f0: e1a0e00f     	mov	lr, pc
  3b37f4: e593f058     	ldr	pc, [r3, #0x58]
  3b37f8: e1a01005     	mov	r1, r5
  3b37fc: e1a00004     	mov	r0, r4
  3b3800: e5943000     	ldr	r3, [r4]
  3b3804: e1a0e00f     	mov	lr, pc
  3b3808: e593f040     	ldr	pc, [r3, #0x40]
  3b380c: e2840e4f     	add	r0, r4, #1264
  3b3810: e3a01000     	mov	r1, #0
  3b3814: e280000c     	add	r0, r0, #12
  3b3818: e1a02001     	mov	r2, r1
  3b381c: e8bd4070     	pop	{r4, r5, r6, lr}
  3b3820: ea003bc3     	b	0x3c2734 <CharStateMachine::SM_SetSpawnState(bool, bool)> @ imm = #0xef0c
