
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034f068 <FileSystemWin32::_getPaths()>:
  34f068: e59f308c     	ldr	r3, [pc, #0x8c]         @ 0x34f0fc <FileSystemWin32::_getPaths()+0x94>
  34f06c: e59f208c     	ldr	r2, [pc, #0x8c]         @ 0x34f100 <FileSystemWin32::_getPaths()+0x98>
  34f070: e92d40f0     	push	{r4, r5, r6, r7, lr}
  34f074: e08f3003     	add	r3, pc, r3
  34f078: e7936002     	ldr	r6, [r3, r2]
  34f07c: e59f2080     	ldr	r2, [pc, #0x80]         @ 0x34f104 <FileSystemWin32::_getPaths()+0x9c>
  34f080: e24ddf43     	sub	sp, sp, #268
  34f084: e1a05000     	mov	r5, r0
  34f088: e7934002     	ldr	r4, [r3, r2]
  34f08c: e5962000     	ldr	r2, [r6]
  34f090: e280002c     	add	r0, r0, #44
  34f094: e5941000     	ldr	r1, [r4]
  34f098: e58d2104     	str	r2, [sp, #0x104]
  34f09c: ebfefd1f     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0x40b84
  34f0a0: e5941000     	ldr	r1, [r4]
  34f0a4: e2850e13     	add	r0, r5, #304
  34f0a8: ebfefd1c     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0x40b90
  34f0ac: e59f1054     	ldr	r1, [pc, #0x54]         @ 0x34f108 <FileSystemWin32::_getPaths()+0xa0>
  34f0b0: e28d7004     	add	r7, sp, #4
  34f0b4: e5942000     	ldr	r2, [r4]
  34f0b8: e08f1001     	add	r1, pc, r1
  34f0bc: e1a00007     	mov	r0, r7
  34f0c0: ebfefe87     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x405e4
  34f0c4: e1a01007     	mov	r1, r7
  34f0c8: e2850fce     	add	r0, r5, #824
  34f0cc: ebfefd13     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0x40bb4
  34f0d0: e2850e43     	add	r0, r5, #1072
  34f0d4: e280000c     	add	r0, r0, #12
  34f0d8: e5941000     	ldr	r1, [r4]
  34f0dc: ebfefd0f     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0x40bc4
  34f0e0: e59d2104     	ldr	r2, [sp, #0x104]
  34f0e4: e5963000     	ldr	r3, [r6]
  34f0e8: e1520003     	cmp	r2, r3
  34f0ec: 1a000001     	bne	0x34f0f8 <FileSystemWin32::_getPaths()+0x90> @ imm = #0x4
  34f0f0: e28ddf43     	add	sp, sp, #268
  34f0f4: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  34f0f8: ebfefc84     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x40df0
  34f0fc: 1c 5a 64 00  	.word	0x00645a1c
  34f100: ac 40 00 00  	.word	0x000040ac
  34f104: 00 06 00 00  	.word	0x00000600
  34f108: a0 16 57 00  	.word	0x005716a0
