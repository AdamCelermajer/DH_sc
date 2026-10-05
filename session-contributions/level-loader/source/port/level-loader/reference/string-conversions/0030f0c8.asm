
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0030f0c8 <StrToObj(char const*, glitch::core::dimension2d<int>&)>:
  30f0c8: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  30f0cc: e24dd008     	sub	sp, sp, #8
  30f0d0: e3a06000     	mov	r6, #0
  30f0d4: e3a0302c     	mov	r3, #44
  30f0d8: e28d4008     	add	r4, sp, #8
  30f0dc: e16430b4     	strh	r3, [r4, #-4]!
  30f0e0: e1a05001     	mov	r5, r1
  30f0e4: e1a01000     	mov	r1, r0
  30f0e8: e1a00006     	mov	r0, r6
  30f0ec: ebfffd0b     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0xbd4
  30f0f0: e1a00006     	mov	r0, r6
  30f0f4: e1a01004     	mov	r1, r4
  30f0f8: ebfffbca     	bl	0x30e028 <.plt+0x2b4>   @ imm = #-0x10d8
  30f0fc: e1500006     	cmp	r0, r6
  30f100: 0a000001     	beq	0x30f10c <StrToObj(char const*, glitch::core::dimension2d<int>&)+0x44> @ imm = #0x4
  30f104: ebfffbe2     	bl	0x30e094 <.plt+0x320>   @ imm = #-0x1078
  30f108: e1a08000     	mov	r8, r0
  30f10c: e1a01004     	mov	r1, r4
  30f110: e3a00000     	mov	r0, #0
  30f114: ebfffbc3     	bl	0x30e028 <.plt+0x2b4>   @ imm = #-0x10f4
  30f118: e3500000     	cmp	r0, #0
  30f11c: 0a000001     	beq	0x30f128 <StrToObj(char const*, glitch::core::dimension2d<int>&)+0x60> @ imm = #0x4
  30f120: ebfffbdb     	bl	0x30e094 <.plt+0x320>   @ imm = #-0x1094
  30f124: e1a07000     	mov	r7, r0
  30f128: e3a00000     	mov	r0, #0
  30f12c: eb0004c3     	bl	0x310440 <CustomFree(void*)> @ imm = #0x130c
  30f130: e5857004     	str	r7, [r5, #0x4]
  30f134: e5858000     	str	r8, [r5]
  30f138: e28dd008     	add	sp, sp, #8
  30f13c: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
