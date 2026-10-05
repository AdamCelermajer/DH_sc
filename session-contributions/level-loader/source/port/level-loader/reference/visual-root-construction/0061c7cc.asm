
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0061c7cc <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::io::IReadFile*, char const*, glitch::collada::CColladaFactory*)>:
  61c7cc: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  61c7d0: e59f4098     	ldr	r4, [pc, #0x98]         @ 0x61c870 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::io::IReadFile*, char const*, glitch::collada::CColladaFactory*)+0xa4>
  61c7d4: e59f6098     	ldr	r6, [pc, #0x98]         @ 0x61c874 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::io::IReadFile*, char const*, glitch::collada::CColladaFactory*)+0xa8>
  61c7d8: e3a05000     	mov	r5, #0
  61c7dc: e08f4004     	add	r4, pc, r4
  61c7e0: e7947006     	ldr	r7, [r4, r6]
  61c7e4: e24dd010     	sub	sp, sp, #16
  61c7e8: e1a08003     	mov	r8, r3
  61c7ec: e1a0a000     	mov	r10, r0
  61c7f0: e1a09002     	mov	r9, r2
  61c7f4: e5970000     	ldr	r0, [r7]
  61c7f8: e1a02005     	mov	r2, r5
  61c7fc: e1a03005     	mov	r3, r5
  61c800: e58d5000     	str	r5, [sp]
  61c804: eb00f860     	bl	0x65a98c <glitch::collada::CResFileManager::load(glitch::io::IReadFile*, bool, void (*)(char const*, glitch::collada::SCollada const*), bool)> @ imm = #0x3e180
  61c808: e3500000     	cmp	r0, #0
  61c80c: 01a08000     	moveq	r8, r0
  61c810: 0a000013     	beq	0x61c864 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::io::IReadFile*, char const*, glitch::collada::CColladaFactory*)+0x98> @ imm = #0x4c
  61c814: e5973000     	ldr	r3, [r7]
  61c818: e1a0100a     	mov	r1, r10
  61c81c: e1a02009     	mov	r2, r9
  61c820: e5d37028     	ldrb	r7, [r3, #0x28]
  61c824: e5c35028     	strb	r5, [r3, #0x28]
  61c828: e58d800c     	str	r8, [sp, #0xc]
  61c82c: e58d0008     	str	r0, [sp, #0x8]
  61c830: e5903004     	ldr	r3, [r0, #0x4]
  61c834: e28d5008     	add	r5, sp, #8
  61c838: e3530000     	cmp	r3, #0
  61c83c: 12833001     	addne	r3, r3, #1
  61c840: 15803004     	strne	r3, [r0, #0x4]
  61c844: e1a00005     	mov	r0, r5
  61c848: ebffffa9     	bl	0x61c6f4 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, char const*) const> @ imm = #-0x15c
  61c84c: e1a08000     	mov	r8, r0
  61c850: e1a00005     	mov	r0, r5
  61c854: ebfff306     	bl	0x619474 <glitch::collada::CColladaDatabase::~CColladaDatabase()> @ imm = #-0x33e8
  61c858: e7943006     	ldr	r3, [r4, r6]
  61c85c: e5933000     	ldr	r3, [r3]
  61c860: e5c37028     	strb	r7, [r3, #0x28]
  61c864: e1a00008     	mov	r0, r8
  61c868: e28dd010     	add	sp, sp, #16
  61c86c: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  61c870: b4 82 37 00  	.word	0x003782b4
  61c874: 48 44 00 00  	.word	0x00004448
