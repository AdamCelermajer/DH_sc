
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0030f050 <StrToObj(char const*, Point2D<int>&)>:
  30f050: e92d40f0     	push	{r4, r5, r6, r7, lr}
  30f054: e24dd00c     	sub	sp, sp, #12
  30f058: e3a0302c     	mov	r3, #44
  30f05c: e1a07000     	mov	r7, r0
  30f060: e28d4008     	add	r4, sp, #8
  30f064: e3a00c01     	mov	r0, #256
  30f068: e16430b4     	strh	r3, [r4, #-4]!
  30f06c: e1a06001     	mov	r6, r1
  30f070: eb0004f7     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #0x13dc
  30f074: e1a01007     	mov	r1, r7
  30f078: e1a05000     	mov	r5, r0
  30f07c: ebfffd27     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0xb64
  30f080: e1a00005     	mov	r0, r5
  30f084: e1a01004     	mov	r1, r4
  30f088: ebfffbe6     	bl	0x30e028 <.plt+0x2b4>   @ imm = #-0x1068
  30f08c: e3500000     	cmp	r0, #0
  30f090: 0a000001     	beq	0x30f09c <StrToObj(char const*, Point2D<int>&)+0x4c> @ imm = #0x4
  30f094: ebfffbfe     	bl	0x30e094 <.plt+0x320>   @ imm = #-0x1008
  30f098: e5860000     	str	r0, [r6]
  30f09c: e1a01004     	mov	r1, r4
  30f0a0: e3a00000     	mov	r0, #0
  30f0a4: ebfffbdf     	bl	0x30e028 <.plt+0x2b4>   @ imm = #-0x1084
  30f0a8: e3500000     	cmp	r0, #0
  30f0ac: 0a000001     	beq	0x30f0b8 <StrToObj(char const*, Point2D<int>&)+0x68> @ imm = #0x4
  30f0b0: ebfffbf7     	bl	0x30e094 <.plt+0x320>   @ imm = #-0x1024
  30f0b4: e5860004     	str	r0, [r6, #0x4]
  30f0b8: e1a00005     	mov	r0, r5
  30f0bc: eb0004df     	bl	0x310440 <CustomFree(void*)> @ imm = #0x137c
  30f0c0: e28dd00c     	add	sp, sp, #12
  30f0c4: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
