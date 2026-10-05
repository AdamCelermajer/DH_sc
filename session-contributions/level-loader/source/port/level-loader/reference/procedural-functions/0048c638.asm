
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048c638 <rnd::BlockSearch::operator()(rnd::ListElem&) const>:
  48c638: e92d4070     	push	{r4, r5, r6, lr}
  48c63c: e1a04000     	mov	r4, r0
  48c640: e5912014     	ldr	r2, [r1, #0x14]
  48c644: e5910018     	ldr	r0, [r1, #0x18]
  48c648: e1a05001     	mov	r5, r1
  48c64c: e5943010     	ldr	r3, [r4, #0x10]
  48c650: e5941014     	ldr	r1, [r4, #0x14]
  48c654: e0602002     	rsb	r2, r0, r2
  48c658: e0613003     	rsb	r3, r1, r3
  48c65c: e1520003     	cmp	r2, r3
  48c660: 0a000001     	beq	0x48c66c <rnd::BlockSearch::operator()(rnd::ListElem&) const+0x34> @ imm = #0x4
  48c664: e3a00000     	mov	r0, #0
  48c668: e8bd8070     	pop	{r4, r5, r6, pc}
  48c66c: ebfa07db     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0x17e094
  48c670: e3500000     	cmp	r0, #0
  48c674: 1afffffa     	bne	0x48c664 <rnd::BlockSearch::operator()(rnd::ListElem&) const+0x2c> @ imm = #-0x18
  48c678: e5950030     	ldr	r0, [r5, #0x30]
  48c67c: e595202c     	ldr	r2, [r5, #0x2c]
  48c680: e594102c     	ldr	r1, [r4, #0x2c]
  48c684: e5943028     	ldr	r3, [r4, #0x28]
  48c688: e0602002     	rsb	r2, r0, r2
  48c68c: e0613003     	rsb	r3, r1, r3
  48c690: e1520003     	cmp	r2, r3
  48c694: 1afffff2     	bne	0x48c664 <rnd::BlockSearch::operator()(rnd::ListElem&) const+0x2c> @ imm = #-0x38
  48c698: ebfa07d0     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0x17e0c0
  48c69c: e3500000     	cmp	r0, #0
  48c6a0: 1affffef     	bne	0x48c664 <rnd::BlockSearch::operator()(rnd::ListElem&) const+0x2c> @ imm = #-0x44
  48c6a4: e5952044     	ldr	r2, [r5, #0x44]
  48c6a8: e5943040     	ldr	r3, [r4, #0x40]
  48c6ac: e5950048     	ldr	r0, [r5, #0x48]
  48c6b0: e5941044     	ldr	r1, [r4, #0x44]
  48c6b4: e0602002     	rsb	r2, r0, r2
  48c6b8: e0613003     	rsb	r3, r1, r3
  48c6bc: e1520003     	cmp	r2, r3
  48c6c0: 1affffe7     	bne	0x48c664 <rnd::BlockSearch::operator()(rnd::ListElem&) const+0x2c> @ imm = #-0x64
  48c6c4: ebfa07c5     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0x17e0ec
  48c6c8: e2700001     	rsbs	r0, r0, #1
  48c6cc: 33a00000     	movlo	r0, #0
  48c6d0: e8bd8070     	pop	{r4, r5, r6, pc}
