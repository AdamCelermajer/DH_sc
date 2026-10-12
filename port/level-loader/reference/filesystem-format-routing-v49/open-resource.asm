
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034ff28 <FileSystemWin32::openResource(char const*)>:
  34ff28: e92d4030     	push	{r4, r5, lr}
  34ff2c: e3a0c000     	mov	r12, #0
  34ff30: e1a04001     	mov	r4, r1
  34ff34: e24dd00c     	sub	sp, sp, #12
  34ff38: e1a0300c     	mov	r3, r12
  34ff3c: e2801f8d     	add	r1, r0, #564
  34ff40: e1a02004     	mov	r2, r4
  34ff44: e58dc000     	str	r12, [sp]
  34ff48: e1a05000     	mov	r5, r0
  34ff4c: ebffffbf     	bl	0x34fe50 <FileSystemWin32::_createHandle(char const*, char const*, bool, bool)> @ imm = #-0x104
  34ff50: e250c000     	subs	r12, r0, #0
  34ff54: 0a000002     	beq	0x34ff64 <FileSystemWin32::openResource(char const*)+0x3c> @ imm = #0x8
  34ff58: e1a0000c     	mov	r0, r12
  34ff5c: e28dd00c     	add	sp, sp, #12
  34ff60: e8bd8030     	pop	{r4, r5, pc}
  34ff64: e1a0300c     	mov	r3, r12
  34ff68: e1a00005     	mov	r0, r5
  34ff6c: e1a02004     	mov	r2, r4
  34ff70: e2851fce     	add	r1, r5, #824
  34ff74: e58dc000     	str	r12, [sp]
  34ff78: ebffffb4     	bl	0x34fe50 <FileSystemWin32::_createHandle(char const*, char const*, bool, bool)> @ imm = #-0x130
  34ff7c: e1a0c000     	mov	r12, r0
  34ff80: eafffff4     	b	0x34ff58 <FileSystemWin32::openResource(char const*)+0x30> @ imm = #-0x30
