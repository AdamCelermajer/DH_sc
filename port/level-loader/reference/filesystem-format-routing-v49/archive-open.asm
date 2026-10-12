
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0056c0f8 <glitch::io::CFileSystem::createAndOpenFileFromArchives(char const*)>:
  56c0f8: e92d4070     	push	{r4, r5, r6, lr}
  56c0fc: e5903008     	ldr	r3, [r0, #0x8]
  56c100: e590200c     	ldr	r2, [r0, #0xc]
  56c104: e1a05000     	mov	r5, r0
  56c108: e1a06001     	mov	r6, r1
  56c10c: e0632002     	rsb	r2, r3, r2
  56c110: e1b02122     	lsrs	r2, r2, #2
  56c114: 0a000010     	beq	0x56c15c <glitch::io::CFileSystem::createAndOpenFileFromArchives(char const*)+0x64> @ imm = #0x40
  56c118: e3a04000     	mov	r4, #0
  56c11c: ea000004     	b	0x56c134 <glitch::io::CFileSystem::createAndOpenFileFromArchives(char const*)+0x3c> @ imm = #0x10
  56c120: e5953008     	ldr	r3, [r5, #0x8]
  56c124: e595200c     	ldr	r2, [r5, #0xc]
  56c128: e0632002     	rsb	r2, r3, r2
  56c12c: e1540142     	cmp	r4, r2, asr #2
  56c130: 2a000009     	bhs	0x56c15c <glitch::io::CFileSystem::createAndOpenFileFromArchives(char const*)+0x64> @ imm = #0x24
  56c134: e7933104     	ldr	r3, [r3, r4, lsl #2]
  56c138: e1a01006     	mov	r1, r6
  56c13c: e2844001     	add	r4, r4, #1
  56c140: e1a00003     	mov	r0, r3
  56c144: e5933000     	ldr	r3, [r3]
  56c148: e1a0e00f     	mov	lr, pc
  56c14c: e593f00c     	ldr	pc, [r3, #0xc]
  56c150: e3500000     	cmp	r0, #0
  56c154: 0afffff1     	beq	0x56c120 <glitch::io::CFileSystem::createAndOpenFileFromArchives(char const*)+0x28> @ imm = #-0x3c
  56c158: e8bd8070     	pop	{r4, r5, r6, pc}
  56c15c: e5953014     	ldr	r3, [r5, #0x14]
  56c160: e5952018     	ldr	r2, [r5, #0x18]
  56c164: e0632002     	rsb	r2, r3, r2
  56c168: e1b02122     	lsrs	r2, r2, #2
  56c16c: 0a000010     	beq	0x56c1b4 <glitch::io::CFileSystem::createAndOpenFileFromArchives(char const*)+0xbc> @ imm = #0x40
  56c170: e3a04000     	mov	r4, #0
  56c174: ea000004     	b	0x56c18c <glitch::io::CFileSystem::createAndOpenFileFromArchives(char const*)+0x94> @ imm = #0x10
  56c178: e5953014     	ldr	r3, [r5, #0x14]
  56c17c: e5952018     	ldr	r2, [r5, #0x18]
  56c180: e0632002     	rsb	r2, r3, r2
  56c184: e1540142     	cmp	r4, r2, asr #2
  56c188: 2a000009     	bhs	0x56c1b4 <glitch::io::CFileSystem::createAndOpenFileFromArchives(char const*)+0xbc> @ imm = #0x24
  56c18c: e7933104     	ldr	r3, [r3, r4, lsl #2]
  56c190: e1a01006     	mov	r1, r6
  56c194: e2844001     	add	r4, r4, #1
  56c198: e1a00003     	mov	r0, r3
  56c19c: e5933000     	ldr	r3, [r3]
  56c1a0: e1a0e00f     	mov	lr, pc
  56c1a4: e593f00c     	ldr	pc, [r3, #0xc]
  56c1a8: e3500000     	cmp	r0, #0
  56c1ac: 0afffff1     	beq	0x56c178 <glitch::io::CFileSystem::createAndOpenFileFromArchives(char const*)+0x80> @ imm = #-0x3c
  56c1b0: eaffffe8     	b	0x56c158 <glitch::io::CFileSystem::createAndOpenFileFromArchives(char const*)+0x60> @ imm = #-0x60
  56c1b4: e5953020     	ldr	r3, [r5, #0x20]
  56c1b8: e5952024     	ldr	r2, [r5, #0x24]
  56c1bc: e0632002     	rsb	r2, r3, r2
  56c1c0: e1b02122     	lsrs	r2, r2, #2
  56c1c4: 0a000010     	beq	0x56c20c <glitch::io::CFileSystem::createAndOpenFileFromArchives(char const*)+0x114> @ imm = #0x40
  56c1c8: e3a04000     	mov	r4, #0
  56c1cc: ea000004     	b	0x56c1e4 <glitch::io::CFileSystem::createAndOpenFileFromArchives(char const*)+0xec> @ imm = #0x10
  56c1d0: e5953020     	ldr	r3, [r5, #0x20]
  56c1d4: e5952024     	ldr	r2, [r5, #0x24]
  56c1d8: e0632002     	rsb	r2, r3, r2
  56c1dc: e1540142     	cmp	r4, r2, asr #2
  56c1e0: 2a000009     	bhs	0x56c20c <glitch::io::CFileSystem::createAndOpenFileFromArchives(char const*)+0x114> @ imm = #0x24
  56c1e4: e7933104     	ldr	r3, [r3, r4, lsl #2]
  56c1e8: e1a01006     	mov	r1, r6
  56c1ec: e2844001     	add	r4, r4, #1
  56c1f0: e1a00003     	mov	r0, r3
  56c1f4: e5933000     	ldr	r3, [r3]
  56c1f8: e1a0e00f     	mov	lr, pc
  56c1fc: e593f00c     	ldr	pc, [r3, #0xc]
  56c200: e3500000     	cmp	r0, #0
  56c204: 0afffff1     	beq	0x56c1d0 <glitch::io::CFileSystem::createAndOpenFileFromArchives(char const*)+0xd8> @ imm = #-0x3c
  56c208: eaffffd2     	b	0x56c158 <glitch::io::CFileSystem::createAndOpenFileFromArchives(char const*)+0x60> @ imm = #-0xb8
  56c20c: e3a00000     	mov	r0, #0
  56c210: e8bd8070     	pop	{r4, r5, r6, pc}
