
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040fe7c <CameraLevel::~CameraLevel()>:
  40fe7c: e92d4010     	push	{r4, lr}
  40fe80: e59f3058     	ldr	r3, [pc, #0x58]         @ 0x40fee0 <CameraLevel::~CameraLevel()+0x64>
  40fe84: e59f2058     	ldr	r2, [pc, #0x58]         @ 0x40fee4 <CameraLevel::~CameraLevel()+0x68>
  40fe88: e5901044     	ldr	r1, [r0, #0x44]
  40fe8c: e08f3003     	add	r3, pc, r3
  40fe90: e7932002     	ldr	r2, [r3, r2]
  40fe94: e3510000     	cmp	r1, #0
  40fe98: e1a04000     	mov	r4, r0
  40fe9c: e2822008     	add	r2, r2, #8
  40fea0: e5802000     	str	r2, [r0]
  40fea4: 0a000005     	beq	0x40fec0 <CameraLevel::~CameraLevel()+0x44> @ imm = #0x14
  40fea8: e5913000     	ldr	r3, [r1]
  40feac: e1a00001     	mov	r0, r1
  40feb0: e1a0e00f     	mov	lr, pc
  40feb4: e593f004     	ldr	pc, [r3, #0x4]
  40feb8: e3a03000     	mov	r3, #0
  40febc: e5843044     	str	r3, [r4, #0x44]
  40fec0: e2840064     	add	r0, r4, #100
  40fec4: ebfc0eb8     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xfc520
  40fec8: e284004c     	add	r0, r4, #76
  40fecc: ebfc0eb6     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xfc528
  40fed0: e1a00004     	mov	r0, r4
  40fed4: eb00074c     	bl	0x411c0c <CameraTarget::~CameraTarget()> @ imm = #0x1d30
  40fed8: e1a00004     	mov	r0, r4
  40fedc: e8bd8010     	pop	{r4, pc}
  40fee0: 04 4c 58 00  	.word	0x00584c04
  40fee4: 28 42 00 00  	.word	0x00004228
