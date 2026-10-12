
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0056c0a8 <glitch::io::CFileSystem::CFileSystem()>:
  56c0a8: e59f1040     	ldr	r1, [pc, #0x40]         @ 0x56c0f0 <glitch::io::CFileSystem::CFileSystem()+0x48>
  56c0ac: e59fc040     	ldr	r12, [pc, #0x40]        @ 0x56c0f4 <glitch::io::CFileSystem::CFileSystem()+0x4c>
  56c0b0: e3a02000     	mov	r2, #0
  56c0b4: e08f1001     	add	r1, pc, r1
  56c0b8: e791c00c     	ldr	r12, [r1, r12]
  56c0bc: e5802028     	str	r2, [r0, #0x28]
  56c0c0: e5802004     	str	r2, [r0, #0x4]
  56c0c4: e28cc008     	add	r12, r12, #8
  56c0c8: e580c000     	str	r12, [r0]
  56c0cc: e5802008     	str	r2, [r0, #0x8]
  56c0d0: e580200c     	str	r2, [r0, #0xc]
  56c0d4: e5802010     	str	r2, [r0, #0x10]
  56c0d8: e5802014     	str	r2, [r0, #0x14]
  56c0dc: e5802018     	str	r2, [r0, #0x18]
  56c0e0: e580201c     	str	r2, [r0, #0x1c]
  56c0e4: e5802020     	str	r2, [r0, #0x20]
  56c0e8: e5802024     	str	r2, [r0, #0x24]
  56c0ec: e12fff1e     	bx	lr
  56c0f0: dc 89 42 00  	.word	0x004289dc
  56c0f4: 18 09 00 00  	.word	0x00000918
