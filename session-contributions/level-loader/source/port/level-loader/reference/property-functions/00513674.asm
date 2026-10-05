
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00513674 <PropertyMap::DumpProperties()>:
  513674: e92d4010     	push	{r4, lr}
  513678: ebffff90     	bl	0x5134c0 <PropertyMap::GetPropertyMap()> @ imm = #-0x1c0
  51367c: e5903008     	ldr	r3, [r0, #0x8]
  513680: e1500003     	cmp	r0, r3
  513684: 0a00000a     	beq	0x5136b4 <PropertyMap::DumpProperties()+0x40> @ imm = #0x28
  513688: e593200c     	ldr	r2, [r3, #0xc]
  51368c: e3520000     	cmp	r2, #0
  513690: 1a000001     	bne	0x51369c <PropertyMap::DumpProperties()+0x28> @ imm = #0x4
  513694: ea000007     	b	0x5136b8 <PropertyMap::DumpProperties()+0x44> @ imm = #0x1c
  513698: e1a02003     	mov	r2, r3
  51369c: e5923008     	ldr	r3, [r2, #0x8]
  5136a0: e3530000     	cmp	r3, #0
  5136a4: 1afffffb     	bne	0x513698 <PropertyMap::DumpProperties()+0x24> @ imm = #-0x14
  5136a8: e1a03002     	mov	r3, r2
  5136ac: e1500003     	cmp	r0, r3
  5136b0: 1afffff4     	bne	0x513688 <PropertyMap::DumpProperties()+0x14> @ imm = #-0x30
  5136b4: e8bd8010     	pop	{r4, pc}
  5136b8: e5931004     	ldr	r1, [r3, #0x4]
  5136bc: e591c00c     	ldr	r12, [r1, #0xc]
  5136c0: e153000c     	cmp	r3, r12
  5136c4: 1a000005     	bne	0x5136e0 <PropertyMap::DumpProperties()+0x6c> @ imm = #0x14
  5136c8: e1a03001     	mov	r3, r1
  5136cc: e5911004     	ldr	r1, [r1, #0x4]
  5136d0: e591200c     	ldr	r2, [r1, #0xc]
  5136d4: e1520003     	cmp	r2, r3
  5136d8: 0afffffa     	beq	0x5136c8 <PropertyMap::DumpProperties()+0x54> @ imm = #-0x18
  5136dc: e593200c     	ldr	r2, [r3, #0xc]
  5136e0: e1510002     	cmp	r1, r2
  5136e4: 11a03001     	movne	r3, r1
  5136e8: eaffffe4     	b	0x513680 <PropertyMap::DumpProperties()+0xc> @ imm = #-0x70
