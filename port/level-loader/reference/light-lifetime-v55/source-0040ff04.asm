
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0040ff04 <CameraLevel::~CameraLevel()>:
  40ff04: e92d4010     	push	{r4, lr}
  40ff08: e59f3058     	ldr	r3, [pc, #0x58]         @ 0x40ff68 <CameraLevel::~CameraLevel()+0x64>
  40ff0c: e59f2058     	ldr	r2, [pc, #0x58]         @ 0x40ff6c <CameraLevel::~CameraLevel()+0x68>
  40ff10: e5901044     	ldr	r1, [r0, #0x44]
  40ff14: e08f3003     	add	r3, pc, r3
  40ff18: e7932002     	ldr	r2, [r3, r2]
  40ff1c: e3510000     	cmp	r1, #0
  40ff20: e1a04000     	mov	r4, r0
  40ff24: e2822008     	add	r2, r2, #8
  40ff28: e5802000     	str	r2, [r0]
  40ff2c: 0a000005     	beq	0x40ff48 <CameraLevel::~CameraLevel()+0x44> @ imm = #0x14
  40ff30: e5913000     	ldr	r3, [r1]
  40ff34: e1a00001     	mov	r0, r1
  40ff38: e1a0e00f     	mov	lr, pc
  40ff3c: e593f004     	ldr	pc, [r3, #0x4]
  40ff40: e3a03000     	mov	r3, #0
  40ff44: e5843044     	str	r3, [r4, #0x44]
  40ff48: e2840064     	add	r0, r4, #100
  40ff4c: ebfc0e96     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xfc5a8
  40ff50: e284004c     	add	r0, r4, #76
  40ff54: ebfc0e94     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0xfc5b0
  40ff58: e1a00004     	mov	r0, r4
  40ff5c: eb00072a     	bl	0x411c0c <CameraTarget::~CameraTarget()> @ imm = #0x1ca8
  40ff60: e1a00004     	mov	r0, r4
  40ff64: e8bd8010     	pop	{r4, pc}
  40ff68: 7c 4b 58 00  	.word	0x00584b7c
  40ff6c: 28 42 00 00  	.word	0x00004228
