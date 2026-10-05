
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048b7fc <_ZN7Array2dIPN3rnd4TileEEclEii>:
  48b7fc: e92d40f0     	push	{r4, r5, r6, r7, lr}
  48b800: e1a05000     	mov	r5, r0
  48b804: e24dd014     	sub	sp, sp, #20
  48b808: e1a06001     	mov	r6, r1
  48b80c: e1a07002     	mov	r7, r2
  48b810: ebffff58     	bl	0x48b578 <_ZN7Array2dIPN3rnd4TileEE14EnsurePositionEii> @ imm = #-0x2a0
  48b814: e595c004     	ldr	r12, [r5, #0x4]
  48b818: e1a0400d     	mov	r4, sp
  48b81c: e285300c     	add	r3, r5, #12
  48b820: e893000f     	ldm	r3, {r0, r1, r2, r3}
  48b824: e884000f     	stm	r4, {r0, r1, r2, r3}
  48b828: e06c1007     	rsb	r1, r12, r7
  48b82c: e1a0000d     	mov	r0, sp
  48b830: ebffe187     	bl	0x483e54 <_ZNSt4priv20_Deque_iterator_baseISt5dequeIPN3rnd4TileESaIS4_EEE10_M_advanceEi> @ imm = #-0x79e4
  48b834: e595c000     	ldr	r12, [r5]
  48b838: e59d3000     	ldr	r3, [sp]
  48b83c: e893000f     	ldm	r3, {r0, r1, r2, r3}
  48b840: e884000f     	stm	r4, {r0, r1, r2, r3}
  48b844: e1a0000d     	mov	r0, sp
  48b848: e06c1006     	rsb	r1, r12, r6
  48b84c: ebffe148     	bl	0x483d74 <_ZNSt4priv20_Deque_iterator_baseIPN3rnd4TileEE10_M_advanceEi> @ imm = #-0x7ae0
  48b850: e59d0000     	ldr	r0, [sp]
  48b854: e28dd014     	add	sp, sp, #20
  48b858: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
