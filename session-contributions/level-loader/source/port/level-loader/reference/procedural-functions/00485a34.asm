
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00485a34 <rnd::RandomGenerator::UnloadRoomPools()>:
  485a34: e92d4070     	push	{r4, r5, r6, lr}
  485a38: e1a05000     	mov	r5, r0
  485a3c: e595311c     	ldr	r3, [r5, #0x11c]
  485a40: e5900118     	ldr	r0, [r0, #0x118]
  485a44: e1500003     	cmp	r0, r3
  485a48: 0a000013     	beq	0x485a9c <rnd::RandomGenerator::UnloadRoomPools()+0x68> @ imm = #0x4c
  485a4c: e1a01000     	mov	r1, r0
  485a50: e4914004     	ldr	r4, [r1], #4
  485a54: e1510003     	cmp	r1, r3
  485a58: 0a000003     	beq	0x485a6c <rnd::RandomGenerator::UnloadRoomPools()+0x38> @ imm = #0xc
  485a5c: e0532001     	subs	r2, r3, r1
  485a60: 0a000001     	beq	0x485a6c <rnd::RandomGenerator::UnloadRoomPools()+0x38> @ imm = #0x4
  485a64: ebfa2133     	bl	0x30df38 <.plt+0x1c4>   @ imm = #-0x177b34
  485a68: e595311c     	ldr	r3, [r5, #0x11c]
  485a6c: e2433004     	sub	r3, r3, #4
  485a70: e3540000     	cmp	r4, #0
  485a74: e1a00004     	mov	r0, r4
  485a78: e585311c     	str	r3, [r5, #0x11c]
  485a7c: 0a000003     	beq	0x485a90 <rnd::RandomGenerator::UnloadRoomPools()+0x5c> @ imm = #0xc
  485a80: ebffffe1     	bl	0x485a0c <rnd::RoomPool::~RoomPool()> @ imm = #-0x7c
  485a84: e1a00004     	mov	r0, r4
  485a88: ebfa2a6c     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x175650
  485a8c: e595311c     	ldr	r3, [r5, #0x11c]
  485a90: e5950118     	ldr	r0, [r5, #0x118]
  485a94: e1530000     	cmp	r3, r0
  485a98: 1affffeb     	bne	0x485a4c <rnd::RandomGenerator::UnloadRoomPools()+0x18> @ imm = #-0x54
  485a9c: e8bd8070     	pop	{r4, r5, r6, pc}
