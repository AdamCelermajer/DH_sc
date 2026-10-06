
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f0018 <Level::_LoadPlayer()>:
  3f0018: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3f001c: e59f819c     	ldr	r8, [pc, #0x19c]        @ 0x3f01c0 <Level::_LoadPlayer()+0x1a8>
  3f0020: e59f219c     	ldr	r2, [pc, #0x19c]        @ 0x3f01c4 <Level::_LoadPlayer()+0x1ac>
  3f0024: e1a07000     	mov	r7, r0
  3f0028: e08f8008     	add	r8, pc, r8
  3f002c: e7983002     	ldr	r3, [r8, r2]
  3f0030: e24dd01c     	sub	sp, sp, #28
  3f0034: e58d2004     	str	r2, [sp, #0x4]
  3f0038: e5930040     	ldr	r0, [r3, #0x40]
  3f003c: e59036c4     	ldr	r3, [r0, #0x6c4]
  3f0040: e3530000     	cmp	r3, #0
  3f0044: da000032     	ble	0x3f0114 <Level::_LoadPlayer()+0xfc> @ imm = #0xc8
  3f0048: e3a06000     	mov	r6, #0
  3f004c: e28d400c     	add	r4, sp, #12
  3f0050: e1a01006     	mov	r1, r6
  3f0054: e3a02001     	mov	r2, #1
  3f0058: ebfdf906     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x81be8
  3f005c: e590b660     	ldr	r11, [r0, #0x660]
  3f0060: e35b0000     	cmp	r11, #0
  3f0064: 0a000045     	beq	0x3f0180 <Level::_LoadPlayer()+0x168> @ imm = #0x114
  3f0068: e59d2004     	ldr	r2, [sp, #0x4]
  3f006c: e7983002     	ldr	r3, [r8, r2]
  3f0070: e5933038     	ldr	r3, [r3, #0x38]
  3f0074: e593a014     	ldr	r10, [r3, #0x14]
  3f0078: e283900c     	add	r9, r3, #12
  3f007c: e15a0009     	cmp	r10, r9
  3f0080: 0a00003e     	beq	0x3f0180 <Level::_LoadPlayer()+0x168> @ imm = #0xf8
  3f0084: e59a102c     	ldr	r1, [r10, #0x2c]
  3f0088: e3510000     	cmp	r1, #0
  3f008c: 0a00000a     	beq	0x3f00bc <Level::_LoadPlayer()+0xa4> @ imm = #0x28
  3f0090: e1a00004     	mov	r0, r4
  3f0094: ebfd3724     	bl	0x33dd2c <ObjectBase::GetHandle()> @ imm = #-0xb2370
  3f0098: e1a00004     	mov	r0, r4
  3f009c: e3a01000     	mov	r1, #0
  3f00a0: ebfd3f46     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xb02e8
  3f00a4: e3500000     	cmp	r0, #0
  3f00a8: 0a000002     	beq	0x3f00b8 <Level::_LoadPlayer()+0xa0> @ imm = #0x8
  3f00ac: e59030f4     	ldr	r3, [r0, #0xf4]
  3f00b0: e353000d     	cmp	r3, #13
  3f00b4: 0a000018     	beq	0x3f011c <Level::_LoadPlayer()+0x104> @ imm = #0x60
  3f00b8: e3a05000     	mov	r5, #0
  3f00bc: e59a200c     	ldr	r2, [r10, #0xc]
  3f00c0: e3a01000     	mov	r1, #0
  3f00c4: e3520000     	cmp	r2, #0
  3f00c8: 0a00001f     	beq	0x3f014c <Level::_LoadPlayer()+0x134> @ imm = #0x7c
  3f00cc: e1a0a002     	mov	r10, r2
  3f00d0: ea000000     	b	0x3f00d8 <Level::_LoadPlayer()+0xc0> @ imm = #0x0
  3f00d4: e1a0a003     	mov	r10, r3
  3f00d8: e59a3008     	ldr	r3, [r10, #0x8]
  3f00dc: e3530000     	cmp	r3, #0
  3f00e0: 1afffffb     	bne	0x3f00d4 <Level::_LoadPlayer()+0xbc> @ imm = #-0x14
  3f00e4: e3510000     	cmp	r1, #0
  3f00e8: 0affffe3     	beq	0x3f007c <Level::_LoadPlayer()+0x64> @ imm = #-0x74
  3f00ec: e59d3004     	ldr	r3, [sp, #0x4]
  3f00f0: e798a003     	ldr	r10, [r8, r3]
  3f00f4: e59a0040     	ldr	r0, [r10, #0x40]
  3f00f8: e5d036d0     	ldrb	r3, [r0, #0x6d0]
  3f00fc: e3530000     	cmp	r3, #0
  3f0100: 0a000022     	beq	0x3f0190 <Level::_LoadPlayer()+0x178> @ imm = #0x88
  3f0104: e59036c4     	ldr	r3, [r0, #0x6c4]
  3f0108: e2866001     	add	r6, r6, #1
  3f010c: e1560003     	cmp	r6, r3
  3f0110: baffffce     	blt	0x3f0050 <Level::_LoadPlayer()+0x38> @ imm = #-0xc8
  3f0114: e28dd01c     	add	sp, sp, #28
  3f0118: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  3f011c: e5d0308a     	ldrb	r3, [r0, #0x8a]
  3f0120: e1a05000     	mov	r5, r0
  3f0124: e3530000     	cmp	r3, #0
  3f0128: 0affffe3     	beq	0x3f00bc <Level::_LoadPlayer()+0xa4> @ imm = #-0x74
  3f012c: e5903374     	ldr	r3, [r0, #0x374]
  3f0130: e5972110     	ldr	r2, [r7, #0x110]
  3f0134: e1520003     	cmp	r2, r3
  3f0138: 03a01001     	moveq	r1, #1
  3f013c: 1affffde     	bne	0x3f00bc <Level::_LoadPlayer()+0xa4> @ imm = #-0x88
  3f0140: e59a200c     	ldr	r2, [r10, #0xc]
  3f0144: e3520000     	cmp	r2, #0
  3f0148: 1affffdf     	bne	0x3f00cc <Level::_LoadPlayer()+0xb4> @ imm = #-0x84
  3f014c: e59a3004     	ldr	r3, [r10, #0x4]
  3f0150: e593000c     	ldr	r0, [r3, #0xc]
  3f0154: e150000a     	cmp	r0, r10
  3f0158: 1a000005     	bne	0x3f0174 <Level::_LoadPlayer()+0x15c> @ imm = #0x14
  3f015c: e1a0a003     	mov	r10, r3
  3f0160: e5933004     	ldr	r3, [r3, #0x4]
  3f0164: e593200c     	ldr	r2, [r3, #0xc]
  3f0168: e15a0002     	cmp	r10, r2
  3f016c: 0afffffa     	beq	0x3f015c <Level::_LoadPlayer()+0x144> @ imm = #-0x18
  3f0170: e59a200c     	ldr	r2, [r10, #0xc]
  3f0174: e1530002     	cmp	r3, r2
  3f0178: 11a0a003     	movne	r10, r3
  3f017c: eaffffd8     	b	0x3f00e4 <Level::_LoadPlayer()+0xcc> @ imm = #-0xa0
  3f0180: e59d2004     	ldr	r2, [sp, #0x4]
  3f0184: e7983002     	ldr	r3, [r8, r2]
  3f0188: e5930040     	ldr	r0, [r3, #0x40]
  3f018c: eaffffdc     	b	0x3f0104 <Level::_LoadPlayer()+0xec> @ imm = #-0x90
  3f0190: e1a0100b     	mov	r1, r11
  3f0194: e1a00005     	mov	r0, r5
  3f0198: ebffe823     	bl	0x3ea22c <SpawnPoint::PlaceObject(GameObject*)> @ imm = #-0x5f74
  3f019c: e1a0000b     	mov	r0, r11
  3f01a0: e28b1e16     	add	r1, r11, #352
  3f01a4: ebfed5d2     	bl	0x3a58f4 <Character::SetInitialPosition(Point3D<float> const&)> @ imm = #-0x4a8b8
  3f01a8: e1a0000b     	mov	r0, r11
  3f01ac: e5971110     	ldr	r1, [r7, #0x110]
  3f01b0: e3e02000     	mvn	r2, #0
  3f01b4: ebff2db8     	bl	0x3bb89c <Character::SG_SetLevelEntryPoint(int, int)> @ imm = #-0x34920
  3f01b8: e59a0040     	ldr	r0, [r10, #0x40]
  3f01bc: eaffffd0     	b	0x3f0104 <Level::_LoadPlayer()+0xec> @ imm = #-0xc0
  3f01c0: 68 4a 5a 00  	.word	0x005a4a68
  3f01c4: f4 37 00 00  	.word	0x000037f4
