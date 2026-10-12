
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034fe50 <FileSystemWin32::_createHandle(char const*, char const*, bool, bool)>:
  34fe50: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  34fe54: e1a04001     	mov	r4, r1
  34fe58: e1d110d0     	ldrsb	r1, [r1]
  34fe5c: e24dd00c     	sub	sp, sp, #12
  34fe60: e1a08002     	mov	r8, r2
  34fe64: e351002e     	cmp	r1, #46
  34fe68: e1a07003     	mov	r7, r3
  34fe6c: e5dd6028     	ldrb	r6, [sp, #0x28]
  34fe70: 0a00000f     	beq	0x34feb4 <FileSystemWin32::_createHandle(char const*, char const*, bool, bool)+0x64> @ imm = #0x3c
  34fe74: e3a0a000     	mov	r10, #0
  34fe78: e3a01000     	mov	r1, #0
  34fe7c: e3a00010     	mov	r0, #16
  34fe80: ebff01ba     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x3f918
  34fe84: e084100a     	add	r1, r4, r10
  34fe88: e1a05000     	mov	r5, r0
  34fe8c: e1a02008     	mov	r2, r8
  34fe90: e1a03007     	mov	r3, r7
  34fe94: e58d6000     	str	r6, [sp]
  34fe98: ebffff5e     	bl	0x34fc18 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)> @ imm = #-0x288
  34fe9c: e5954004     	ldr	r4, [r5, #0x4]
  34fea0: e3540000     	cmp	r4, #0
  34fea4: 11a00005     	movne	r0, r5
  34fea8: 0a000007     	beq	0x34fecc <FileSystemWin32::_createHandle(char const*, char const*, bool, bool)+0x7c> @ imm = #0x1c
  34feac: e28dd00c     	add	sp, sp, #12
  34feb0: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  34feb4: e1d430d1     	ldrsb	r3, [r4, #1]
  34feb8: e353002f     	cmp	r3, #47
  34febc: 1353005c     	cmpne	r3, #92
  34fec0: 03a0a002     	moveq	r10, #2
  34fec4: 0affffeb     	beq	0x34fe78 <FileSystemWin32::_createHandle(char const*, char const*, bool, bool)+0x28> @ imm = #-0x54
  34fec8: eaffffe9     	b	0x34fe74 <FileSystemWin32::_createHandle(char const*, char const*, bool, bool)+0x24> @ imm = #-0x5c
  34fecc: e1a00005     	mov	r0, r5
  34fed0: e5953000     	ldr	r3, [r5]
  34fed4: e1a0e00f     	mov	lr, pc
  34fed8: e593f004     	ldr	pc, [r3, #0x4]
  34fedc: e1a00004     	mov	r0, r4
  34fee0: eafffff1     	b	0x34feac <FileSystemWin32::_createHandle(char const*, char const*, bool, bool)+0x5c> @ imm = #-0x3c
