
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0061c71c <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::io::IReadFile*, char const*, bool, glitch::collada::CColladaFactory*)>:
  61c71c: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  61c720: e59f409c     	ldr	r4, [pc, #0x9c]         @ 0x61c7c4 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::io::IReadFile*, char const*, bool, glitch::collada::CColladaFactory*)+0xa8>
  61c724: e59f509c     	ldr	r5, [pc, #0x9c]         @ 0x61c7c8 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::io::IReadFile*, char const*, bool, glitch::collada::CColladaFactory*)+0xac>
  61c728: e24dd014     	sub	sp, sp, #20
  61c72c: e08f4004     	add	r4, pc, r4
  61c730: e7946005     	ldr	r6, [r4, r5]
  61c734: e58d3000     	str	r3, [sp]
  61c738: e1a08002     	mov	r8, r2
  61c73c: e596c000     	ldr	r12, [r6]
  61c740: e3a02000     	mov	r2, #0
  61c744: e1a07000     	mov	r7, r0
  61c748: e1a03002     	mov	r3, r2
  61c74c: e1a0000c     	mov	r0, r12
  61c750: eb00f88d     	bl	0x65a98c <glitch::collada::CResFileManager::load(glitch::io::IReadFile*, bool, void (*)(char const*, glitch::collada::SCollada const*), bool)> @ imm = #0x3e234
  61c754: e3500000     	cmp	r0, #0
  61c758: 01a07000     	moveq	r7, r0
  61c75c: 0a000015     	beq	0x61c7b8 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::io::IReadFile*, char const*, bool, glitch::collada::CColladaFactory*)+0x9c> @ imm = #0x54
  61c760: e5963000     	ldr	r3, [r6]
  61c764: e3a02000     	mov	r2, #0
  61c768: e28d6008     	add	r6, sp, #8
  61c76c: e5d3a028     	ldrb	r10, [r3, #0x28]
  61c770: e5c32028     	strb	r2, [r3, #0x28]
  61c774: e59d3030     	ldr	r3, [sp, #0x30]
  61c778: e58d0008     	str	r0, [sp, #0x8]
  61c77c: e1a01007     	mov	r1, r7
  61c780: e58d300c     	str	r3, [sp, #0xc]
  61c784: e5903004     	ldr	r3, [r0, #0x4]
  61c788: e1530002     	cmp	r3, r2
  61c78c: 12833001     	addne	r3, r3, #1
  61c790: 15803004     	strne	r3, [r0, #0x4]
  61c794: e1a02008     	mov	r2, r8
  61c798: e1a00006     	mov	r0, r6
  61c79c: ebffffd4     	bl	0x61c6f4 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, char const*) const> @ imm = #-0xb0
  61c7a0: e1a07000     	mov	r7, r0
  61c7a4: e1a00006     	mov	r0, r6
  61c7a8: ebfff331     	bl	0x619474 <glitch::collada::CColladaDatabase::~CColladaDatabase()> @ imm = #-0x333c
  61c7ac: e7943005     	ldr	r3, [r4, r5]
  61c7b0: e5933000     	ldr	r3, [r3]
  61c7b4: e5c3a028     	strb	r10, [r3, #0x28]
  61c7b8: e1a00007     	mov	r0, r7
  61c7bc: e28dd014     	add	sp, sp, #20
  61c7c0: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  61c7c4: 64 83 37 00  	.word	0x00378364
  61c7c8: 48 44 00 00  	.word	0x00004448
