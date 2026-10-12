
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00488134 <_ZN3rnd15RandomGenerator8GenerateEi>:
  488134: e92d4070     	push	{r4, r5, r6, lr}
  488138: e2805008     	add	r5, r0, #8
  48813c: e24dd048     	sub	sp, sp, #72
  488140: e1a04000     	mov	r4, r0
  488144: e1a00005     	mov	r0, r5
  488148: e1a06001     	mov	r6, r1
  48814c: ebfffe97     	bl	0x487bb0 <_ZN7Array2dIPN3rnd4TileEE5ClearEv> @ imm = #-0x5a4
  488150: e1a01004     	mov	r1, r4
  488154: e5846004     	str	r6, [r4, #0x4]
  488158: e1a0000d     	mov	r0, sp
  48815c: e481606c     	str	r6, [r1], #108
  488160: eb0013d3     	bl	0x48d0b4 <_ZN3rnd8RootRule4ImplC1ERKS0_> @ imm = #0x4f4c
  488164: e1a01005     	mov	r1, r5
  488168: e1a0000d     	mov	r0, sp
  48816c: eb0019ce     	bl	0x48e8ac <_ZN3rnd8RootRule4Impl8GenerateER7Array2dIPNS_4TileEE> @ imm = #0x6738
  488170: e59f5050     	ldr	r5, [pc, #0x50]         @ 0x4881c8 <_ZN3rnd15RandomGenerator8GenerateEi+0x94>
  488174: e3500000     	cmp	r0, #0
  488178: e5840114     	str	r0, [r4, #0x114]
  48817c: e1a0600d     	mov	r6, sp
  488180: e08f5005     	add	r5, pc, r5
  488184: 0a000004     	beq	0x48819c <_ZN3rnd15RandomGenerator8GenerateEi+0x68> @ imm = #0x10
  488188: e1a00004     	mov	r0, r4
  48818c: ebfff1a3     	bl	0x484820 <_ZN3rnd15RandomGenerator8PrintMapEv> @ imm = #-0x3974
  488190: e5940114     	ldr	r0, [r4, #0x114]
  488194: eb0024c4     	bl	0x4914ac <_ZN3rnd4Tile5PrintEv> @ imm = #0x9310
  488198: e5940114     	ldr	r0, [r4, #0x114]
  48819c: e59f3028     	ldr	r3, [pc, #0x28]         @ 0x4881cc <_ZN3rnd15RandomGenerator8GenerateEi+0x98>
  4881a0: e2504000     	subs	r4, r0, #0
  4881a4: 13a04001     	movne	r4, #1
  4881a8: e1a0000d     	mov	r0, sp
  4881ac: e7953003     	ldr	r3, [r5, r3]
  4881b0: e2833008     	add	r3, r3, #8
  4881b4: e58d3000     	str	r3, [sp]
  4881b8: eb001441     	bl	0x48d2c4 <_ZN3rnd4Rule4ImplD2Ev> @ imm = #0x5104
  4881bc: e1a00004     	mov	r0, r4
  4881c0: e28dd048     	add	sp, sp, #72
  4881c4: e8bd8070     	pop	{r4, r5, r6, pc}
  4881c8: 10 c9 50 00  	.word	0x0050c910
  4881cc: ac 07 00 00  	.word	0x000007ac
