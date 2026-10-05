
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

005138fc <PropertyMap::CloneProperties(PropertyMap&)>:
  5138fc: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  513900: e59f90f0     	ldr	r9, [pc, #0xf0]         @ 0x5139f8 <PropertyMap::CloneProperties(PropertyMap&)+0xfc>
  513904: e59fb0f0     	ldr	r11, [pc, #0xf0]        @ 0x5139fc <PropertyMap::CloneProperties(PropertyMap&)+0x100>
  513908: e24dd024     	sub	sp, sp, #36
  51390c: e08f9009     	add	r9, pc, r9
  513910: e799300b     	ldr	r3, [r9, r11]
  513914: e1a0a000     	mov	r10, r0
  513918: e1a00001     	mov	r0, r1
  51391c: e5933000     	ldr	r3, [r3]
  513920: e1a08001     	mov	r8, r1
  513924: e28d5004     	add	r5, sp, #4
  513928: e58d301c     	str	r3, [sp, #0x1c]
  51392c: ebfffee3     	bl	0x5134c0 <PropertyMap::GetPropertyMap()> @ imm = #-0x474
  513930: e5904008     	ldr	r4, [r0, #0x8]
  513934: e1a07000     	mov	r7, r0
  513938: e1570004     	cmp	r7, r4
  51393c: 0a000018     	beq	0x5139a4 <PropertyMap::CloneProperties(PropertyMap&)+0xa8> @ imm = #0x60
  513940: e5943028     	ldr	r3, [r4, #0x28]
  513944: e5946024     	ldr	r6, [r4, #0x24]
  513948: e1a02008     	mov	r2, r8
  51394c: e1a01003     	mov	r1, r3
  513950: e1a00005     	mov	r0, r5
  513954: e5933000     	ldr	r3, [r3]
  513958: e1a0e00f     	mov	lr, pc
  51395c: e593f000     	ldr	pc, [r3]
  513960: e59d2018     	ldr	r2, [sp, #0x18]
  513964: e1a0000a     	mov	r0, r10
  513968: e1a01006     	mov	r1, r6
  51396c: ebffffc2     	bl	0x51387c <PropertyMap::SetProperty(char const*, char const*)> @ imm = #-0xf8
  513970: e1a00005     	mov	r0, r5
  513974: ebf81236     	bl	0x318254 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::~basic_string()> @ imm = #-0x1fb728
  513978: e594200c     	ldr	r2, [r4, #0xc]
  51397c: e3520000     	cmp	r2, #0
  513980: 1a000001     	bne	0x51398c <PropertyMap::CloneProperties(PropertyMap&)+0x90> @ imm = #0x4
  513984: ea00000d     	b	0x5139c0 <PropertyMap::CloneProperties(PropertyMap&)+0xc4> @ imm = #0x34
  513988: e1a02003     	mov	r2, r3
  51398c: e5923008     	ldr	r3, [r2, #0x8]
  513990: e3530000     	cmp	r3, #0
  513994: 1afffffb     	bne	0x513988 <PropertyMap::CloneProperties(PropertyMap&)+0x8c> @ imm = #-0x14
  513998: e1a04002     	mov	r4, r2
  51399c: e1570004     	cmp	r7, r4
  5139a0: 1affffe6     	bne	0x513940 <PropertyMap::CloneProperties(PropertyMap&)+0x44> @ imm = #-0x68
  5139a4: e799300b     	ldr	r3, [r9, r11]
  5139a8: e59d201c     	ldr	r2, [sp, #0x1c]
  5139ac: e5933000     	ldr	r3, [r3]
  5139b0: e1520003     	cmp	r2, r3
  5139b4: 1a00000e     	bne	0x5139f4 <PropertyMap::CloneProperties(PropertyMap&)+0xf8> @ imm = #0x38
  5139b8: e28dd024     	add	sp, sp, #36
  5139bc: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  5139c0: e5943004     	ldr	r3, [r4, #0x4]
  5139c4: e593100c     	ldr	r1, [r3, #0xc]
  5139c8: e1540001     	cmp	r4, r1
  5139cc: 1a000005     	bne	0x5139e8 <PropertyMap::CloneProperties(PropertyMap&)+0xec> @ imm = #0x14
  5139d0: e1a04003     	mov	r4, r3
  5139d4: e5933004     	ldr	r3, [r3, #0x4]
  5139d8: e593200c     	ldr	r2, [r3, #0xc]
  5139dc: e1520004     	cmp	r2, r4
  5139e0: 0afffffa     	beq	0x5139d0 <PropertyMap::CloneProperties(PropertyMap&)+0xd4> @ imm = #-0x18
  5139e4: e594200c     	ldr	r2, [r4, #0xc]
  5139e8: e1520003     	cmp	r2, r3
  5139ec: 11a04003     	movne	r4, r3
  5139f0: eaffffd0     	b	0x513938 <PropertyMap::CloneProperties(PropertyMap&)+0x3c> @ imm = #-0xc0
  5139f4: ebf7ea45     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x2056ec
  5139f8: 84 11 48 00  	.word	0x00481184
  5139fc: ac 40 00 00  	.word	0x000040ac
