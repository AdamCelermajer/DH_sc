
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0035ebd0 <glitch::video::IVideoDriver::drawMeshBuffer(boost::intrusive_ptr<glitch::scene::CMeshBuffer const> const&)>:
  35ebd0: e92d4010     	push	{r4, lr}
  35ebd4: e5912000     	ldr	r2, [r1]
  35ebd8: e24dd010     	sub	sp, sp, #16
  35ebdc: e3520000     	cmp	r2, #0
  35ebe0: 0a000010     	beq	0x35ec28 <glitch::video::IVideoDriver::drawMeshBuffer(boost::intrusive_ptr<glitch::scene::CMeshBuffer const> const&)+0x58> @ imm = #0x40
  35ebe4: e5923014     	ldr	r3, [r2, #0x14]
  35ebe8: e590c000     	ldr	r12, [r0]
  35ebec: e28d400c     	add	r4, sp, #12
  35ebf0: e3530000     	cmp	r3, #0
  35ebf4: e59cc058     	ldr	r12, [r12, #0x58]
  35ebf8: e58d300c     	str	r3, [sp, #0xc]
  35ebfc: 15932000     	ldrne	r2, [r3]
  35ec00: 12822001     	addne	r2, r2, #1
  35ec04: 15832000     	strne	r2, [r3]
  35ec08: 15912000     	ldrne	r2, [r1]
  35ec0c: e58d1000     	str	r1, [sp]
  35ec10: e1a01004     	mov	r1, r4
  35ec14: e2823030     	add	r3, r2, #48
  35ec18: e2822018     	add	r2, r2, #24
  35ec1c: e12fff3c     	blx	r12
  35ec20: e1a00004     	mov	r0, r4
  35ec24: ebffffd9     	bl	0x35eb90 <boost::intrusive_ptr<glitch::video::CVertexStreams const>::~intrusive_ptr()> @ imm = #-0x9c
  35ec28: e28dd010     	add	sp, sp, #16
  35ec2c: e8bd8010     	pop	{r4, pc}
