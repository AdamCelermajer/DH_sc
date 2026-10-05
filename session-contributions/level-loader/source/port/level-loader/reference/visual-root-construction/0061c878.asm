
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0061c878 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, char const*, char const*, glitch::collada::CColladaFactory*)>:
  61c878: e92d45f0     	push	{r4, r5, r6, r7, r8, r10, lr}
  61c87c: e59f4090     	ldr	r4, [pc, #0x90]         @ 0x61c914 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, char const*, char const*, glitch::collada::CColladaFactory*)+0x9c>
  61c880: e59f5090     	ldr	r5, [pc, #0x90]         @ 0x61c918 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, char const*, char const*, glitch::collada::CColladaFactory*)+0xa0>
  61c884: e1a0a002     	mov	r10, r2
  61c888: e08f4004     	add	r4, pc, r4
  61c88c: e7946005     	ldr	r6, [r4, r5]
  61c890: e3a02000     	mov	r2, #0
  61c894: e1a08000     	mov	r8, r0
  61c898: e24dd00c     	sub	sp, sp, #12
  61c89c: e1a07003     	mov	r7, r3
  61c8a0: e5960000     	ldr	r0, [r6]
  61c8a4: e1a03002     	mov	r3, r2
  61c8a8: eb00f8eb     	bl	0x65ac5c <glitch::collada::CResFileManager::load(char const*, bool, void (*)(char const*, glitch::collada::SCollada const*))> @ imm = #0x3e3ac
  61c8ac: e3500000     	cmp	r0, #0
  61c8b0: 01a08000     	moveq	r8, r0
  61c8b4: 0a000013     	beq	0x61c908 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, char const*, char const*, glitch::collada::CColladaFactory*)+0x90> @ imm = #0x4c
  61c8b8: e5963000     	ldr	r3, [r6]
  61c8bc: e3a02000     	mov	r2, #0
  61c8c0: e1a01008     	mov	r1, r8
  61c8c4: e5d36028     	ldrb	r6, [r3, #0x28]
  61c8c8: e5c32028     	strb	r2, [r3, #0x28]
  61c8cc: e88d0081     	stm	sp, {r0, r7}
  61c8d0: e5903004     	ldr	r3, [r0, #0x4]
  61c8d4: e1a0700d     	mov	r7, sp
  61c8d8: e1530002     	cmp	r3, r2
  61c8dc: 12833001     	addne	r3, r3, #1
  61c8e0: 15803004     	strne	r3, [r0, #0x4]
  61c8e4: e1a0200a     	mov	r2, r10
  61c8e8: e1a0000d     	mov	r0, sp
  61c8ec: ebffff80     	bl	0x61c6f4 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, char const*) const> @ imm = #-0x200
  61c8f0: e1a08000     	mov	r8, r0
  61c8f4: e1a0000d     	mov	r0, sp
  61c8f8: ebfff2dd     	bl	0x619474 <glitch::collada::CColladaDatabase::~CColladaDatabase()> @ imm = #-0x348c
  61c8fc: e7943005     	ldr	r3, [r4, r5]
  61c900: e5933000     	ldr	r3, [r3]
  61c904: e5c36028     	strb	r6, [r3, #0x28]
  61c908: e1a00008     	mov	r0, r8
  61c90c: e28dd00c     	add	sp, sp, #12
  61c910: e8bd85f0     	pop	{r4, r5, r6, r7, r8, r10, pc}
  61c914: 08 82 37 00  	.word	0x00378208
  61c918: 48 44 00 00  	.word	0x00004448
