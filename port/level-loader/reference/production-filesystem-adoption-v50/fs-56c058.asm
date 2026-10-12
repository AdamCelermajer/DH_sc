
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0056c058 <glitch::io::CFileSystem::CFileSystem()>:
  56c058: e59f1040     	ldr	r1, [pc, #0x40]         @ 0x56c0a0 <glitch::io::CFileSystem::CFileSystem()+0x48>
  56c05c: e59fc040     	ldr	r12, [pc, #0x40]        @ 0x56c0a4 <glitch::io::CFileSystem::CFileSystem()+0x4c>
  56c060: e3a02000     	mov	r2, #0
  56c064: e08f1001     	add	r1, pc, r1
  56c068: e791c00c     	ldr	r12, [r1, r12]
  56c06c: e5802028     	str	r2, [r0, #0x28]
  56c070: e5802004     	str	r2, [r0, #0x4]
  56c074: e28cc008     	add	r12, r12, #8
  56c078: e580c000     	str	r12, [r0]
  56c07c: e5802008     	str	r2, [r0, #0x8]
  56c080: e580200c     	str	r2, [r0, #0xc]
  56c084: e5802010     	str	r2, [r0, #0x10]
  56c088: e5802014     	str	r2, [r0, #0x14]
  56c08c: e5802018     	str	r2, [r0, #0x18]
  56c090: e580201c     	str	r2, [r0, #0x1c]
  56c094: e5802020     	str	r2, [r0, #0x20]
  56c098: e5802024     	str	r2, [r0, #0x24]
  56c09c: e12fff1e     	bx	lr
  56c0a0: 2c 8a 42 00  	.word	0x00428a2c
  56c0a4: 18 09 00 00  	.word	0x00000918
