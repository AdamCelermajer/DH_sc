
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0056c214 <glitch::io::CFileSystem::getWorkingDirectory()>:
  56c214: e59f300c     	ldr	r3, [pc, #0xc]          @ 0x56c228 <glitch::io::CFileSystem::getWorkingDirectory()+0x14>
  56c218: e59f200c     	ldr	r2, [pc, #0xc]          @ 0x56c22c <glitch::io::CFileSystem::getWorkingDirectory()+0x18>
  56c21c: e08f3003     	add	r3, pc, r3
  56c220: e7930002     	ldr	r0, [r3, r2]
  56c224: e12fff1e     	bx	lr
  56c228: 74 88 42 00  	.word	0x00428874
  56c22c: 68 0e 00 00  	.word	0x00000e68
