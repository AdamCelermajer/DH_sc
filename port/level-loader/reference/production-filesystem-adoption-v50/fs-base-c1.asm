
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034e504 <FileSystemBase::FileSystemBase()>:
  34e504: e92d4070     	push	{r4, r5, r6, lr}
  34e508: e59f4020     	ldr	r4, [pc, #0x20]         @ 0x34e530 <FileSystemBase::FileSystemBase()+0x2c>
  34e50c: e1a05000     	mov	r5, r0
  34e510: eb0876d0     	bl	0x56c058 <glitch::io::CFileSystem::CFileSystem()> @ imm = #0x21db40
  34e514: e59f3018     	ldr	r3, [pc, #0x18]         @ 0x34e534 <FileSystemBase::FileSystemBase()+0x30>
  34e518: e08f4004     	add	r4, pc, r4
  34e51c: e1a00005     	mov	r0, r5
  34e520: e7943003     	ldr	r3, [r4, r3]
  34e524: e2833008     	add	r3, r3, #8
  34e528: e5853000     	str	r3, [r5]
  34e52c: e8bd8070     	pop	{r4, r5, r6, pc}
  34e530: 78 65 64 00  	.word	0x00646578
  34e534: 8c 2e 00 00  	.word	0x00002e8c

0034e538 <FileSystemBase::FileSystemBase()>:
  34e538: e92d4070     	push	{r4, r5, r6, lr}
  34e53c: e59f4020     	ldr	r4, [pc, #0x20]         @ 0x34e564 <FileSystemBase::FileSystemBase()+0x2c>
  34e540: e1a05000     	mov	r5, r0
  34e544: eb0876c3     	bl	0x56c058 <glitch::io::CFileSystem::CFileSystem()> @ imm = #0x21db0c
  34e548: e59f3018     	ldr	r3, [pc, #0x18]         @ 0x34e568 <FileSystemBase::FileSystemBase()+0x30>
  34e54c: e08f4004     	add	r4, pc, r4
  34e550: e1a00005     	mov	r0, r5
  34e554: e7943003     	ldr	r3, [r4, r3]
  34e558: e2833008     	add	r3, r3, #8
  34e55c: e5853000     	str	r3, [r5]
  34e560: e8bd8070     	pop	{r4, r5, r6, pc}
  34e564: 44 65 64 00  	.word	0x00646544
  34e568: 8c 2e 00 00  	.word	0x00002e8c
