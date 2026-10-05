
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048bc20 <rnd::RoomPool::Find(unsigned long)>:
  48bc20: e92d0030     	push	{r4, r5}
  48bc24: e9900009     	ldmib	r0, {r0, r3}
  48bc28: e0603003     	rsb	r3, r0, r3
  48bc2c: e1a021c3     	asr	r2, r3, #3
  48bc30: e0823102     	add	r3, r2, r2, lsl #2
  48bc34: e0833203     	add	r3, r3, r3, lsl #4
  48bc38: e0833403     	add	r3, r3, r3, lsl #8
  48bc3c: e0833803     	add	r3, r3, r3, lsl #16
  48bc40: e0923083     	adds	r3, r2, r3, lsl #1
  48bc44: 0a00000d     	beq	0x48bc80 <rnd::RoomPool::Find(unsigned long)+0x60> @ imm = #0x34
  48bc48: e5902004     	ldr	r2, [r0, #0x4]
  48bc4c: e1520001     	cmp	r2, r1
  48bc50: 13a0c018     	movne	r12, #24
  48bc54: 13a02000     	movne	r2, #0
  48bc58: 1a000004     	bne	0x48bc70 <rnd::RoomPool::Find(unsigned long)+0x50> @ imm = #0x10
  48bc5c: ea000008     	b	0x48bc84 <rnd::RoomPool::Find(unsigned long)+0x64> @ imm = #0x20
  48bc60: e5945004     	ldr	r5, [r4, #0x4]
  48bc64: e28cc018     	add	r12, r12, #24
  48bc68: e1550001     	cmp	r5, r1
  48bc6c: 0a000006     	beq	0x48bc8c <rnd::RoomPool::Find(unsigned long)+0x6c> @ imm = #0x18
  48bc70: e2822001     	add	r2, r2, #1
  48bc74: e1520003     	cmp	r2, r3
  48bc78: e080400c     	add	r4, r0, r12
  48bc7c: 1afffff7     	bne	0x48bc60 <rnd::RoomPool::Find(unsigned long)+0x40> @ imm = #-0x24
  48bc80: e3a00000     	mov	r0, #0
  48bc84: e8bd0030     	pop	{r4, r5}
  48bc88: e12fff1e     	bx	lr
  48bc8c: e1a00004     	mov	r0, r4
  48bc90: eafffffb     	b	0x48bc84 <rnd::RoomPool::Find(unsigned long)+0x64> @ imm = #-0x14
