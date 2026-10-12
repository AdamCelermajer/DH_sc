
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034fc18 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)>:
  34fc18: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  34fc1c: e59f5204     	ldr	r5, [pc, #0x204]        @ 0x34fe28 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x210>
  34fc20: e59f7204     	ldr	r7, [pc, #0x204]        @ 0x34fe2c <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x214>
  34fc24: e59fe204     	ldr	lr, [pc, #0x204]        @ 0x34fe30 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x218>
  34fc28: e08f5005     	add	r5, pc, r5
  34fc2c: e795c007     	ldr	r12, [r5, r7]
  34fc30: e795e00e     	ldr	lr, [r5, lr]
  34fc34: e24ddf46     	sub	sp, sp, #280
  34fc38: e59cc000     	ldr	r12, [r12]
  34fc3c: e28ee008     	add	lr, lr, #8
  34fc40: e580e000     	str	lr, [r0]
  34fc44: e1a04000     	mov	r4, r0
  34fc48: e28d6010     	add	r6, sp, #16
  34fc4c: e3a00000     	mov	r0, #0
  34fc50: e1a0a002     	mov	r10, r2
  34fc54: e5840008     	str	r0, [r4, #0x8]
  34fc58: e1a00006     	mov	r0, r6
  34fc5c: e58dc114     	str	r12, [sp, #0x114]
  34fc60: e1a08003     	mov	r8, r3
  34fc64: e5dd9138     	ldrb	r9, [sp, #0x138]
  34fc68: ebfefa2c     	bl	0x30e520 <.plt+0x7ac>   @ imm = #-0x41750
  34fc6c: e1a0100a     	mov	r1, r10
  34fc70: e1a00006     	mov	r0, r6
  34fc74: ebfefc45     	bl	0x30ed90 <.plt+0x101c>  @ imm = #-0x40eec
  34fc78: e59f11b4     	ldr	r1, [pc, #0x1b4]        @ 0x34fe34 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x21c>
  34fc7c: e1a0000a     	mov	r0, r10
  34fc80: e08f1001     	add	r1, pc, r1
  34fc84: ebfefbd2     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x410b8
  34fc88: e3500000     	cmp	r0, #0
  34fc8c: 0a000007     	beq	0x34fcb0 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x98> @ imm = #0x1c
  34fc90: e59f21a0     	ldr	r2, [pc, #0x1a0]        @ 0x34fe38 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x220>
  34fc94: e59f11a0     	ldr	r1, [pc, #0x1a0]        @ 0x34fe3c <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x224>
  34fc98: e1a0300a     	mov	r3, r10
  34fc9c: e7952002     	ldr	r2, [r5, r2]
  34fca0: e08f1001     	add	r1, pc, r1
  34fca4: e1a00006     	mov	r0, r6
  34fca8: e5922000     	ldr	r2, [r2]
  34fcac: ebfefb8c     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x411d0
  34fcb0: e59f3188     	ldr	r3, [pc, #0x188]        @ 0x34fe40 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x228>
  34fcb4: e7953003     	ldr	r3, [r5, r3]
  34fcb8: e5d330a8     	ldrb	r3, [r3, #0xa8]
  34fcbc: e3530000     	cmp	r3, #0
  34fcc0: 1a000052     	bne	0x34fe10 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x1f8> @ imm = #0x148
  34fcc4: e3580000     	cmp	r8, #0
  34fcc8: 0a000038     	beq	0x34fdb0 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x198> @ imm = #0xe0
  34fccc: e3590000     	cmp	r9, #0
  34fcd0: 0a000023     	beq	0x34fd64 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x14c> @ imm = #0x8c
  34fcd4: e59f2168     	ldr	r2, [pc, #0x168]        @ 0x34fe44 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x22c>
  34fcd8: e28d000c     	add	r0, sp, #12
  34fcdc: e1a01006     	mov	r1, r6
  34fce0: e08f2002     	add	r2, pc, r2
  34fce4: eb0877d5     	bl	0x56dc40 <glitch::io::CFileSystem::open(char const*, char const*)> @ imm = #0x21df54
  34fce8: e59d300c     	ldr	r3, [sp, #0xc]
  34fcec: e3530000     	cmp	r3, #0
  34fcf0: 15932000     	ldrne	r2, [r3]
  34fcf4: 12822001     	addne	r2, r2, #1
  34fcf8: 15832000     	strne	r2, [r3]
  34fcfc: e5940008     	ldr	r0, [r4, #0x8]
  34fd00: e5843008     	str	r3, [r4, #0x8]
  34fd04: e3500000     	cmp	r0, #0
  34fd08: 0a000000     	beq	0x34fd10 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0xf8> @ imm = #0x0
  34fd0c: ebfffcc9     	bl	0x34f038 <glitch::ISharedObject<glitch::io::CFile>::drop() const> @ imm = #-0xcdc
  34fd10: e59d000c     	ldr	r0, [sp, #0xc]
  34fd14: e3500000     	cmp	r0, #0
  34fd18: 0a000000     	beq	0x34fd20 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x108> @ imm = #0x0
  34fd1c: ebfffcc5     	bl	0x34f038 <glitch::ISharedObject<glitch::io::CFile>::drop() const> @ imm = #-0xcec
  34fd20: e3a03001     	mov	r3, #1
  34fd24: e5c4300d     	strb	r3, [r4, #0xd]
  34fd28: e5c4300c     	strb	r3, [r4, #0xc]
  34fd2c: e5943008     	ldr	r3, [r4, #0x8]
  34fd30: e3a02000     	mov	r2, #0
  34fd34: e5842004     	str	r2, [r4, #0x4]
  34fd38: e1530002     	cmp	r3, r2
  34fd3c: 15933004     	ldrne	r3, [r3, #0x4]
  34fd40: e1a00004     	mov	r0, r4
  34fd44: 15843004     	strne	r3, [r4, #0x4]
  34fd48: e7953007     	ldr	r3, [r5, r7]
  34fd4c: e59d2114     	ldr	r2, [sp, #0x114]
  34fd50: e5933000     	ldr	r3, [r3]
  34fd54: e1520003     	cmp	r2, r3
  34fd58: 1a000031     	bne	0x34fe24 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x20c> @ imm = #0xc4
  34fd5c: e28ddf46     	add	sp, sp, #280
  34fd60: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  34fd64: e59f20dc     	ldr	r2, [pc, #0xdc]         @ 0x34fe48 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x230>
  34fd68: e28d0008     	add	r0, sp, #8
  34fd6c: e1a01006     	mov	r1, r6
  34fd70: e08f2002     	add	r2, pc, r2
  34fd74: eb0877b1     	bl	0x56dc40 <glitch::io::CFileSystem::open(char const*, char const*)> @ imm = #0x21dec4
  34fd78: e59d3008     	ldr	r3, [sp, #0x8]
  34fd7c: e3530000     	cmp	r3, #0
  34fd80: 15932000     	ldrne	r2, [r3]
  34fd84: 12822001     	addne	r2, r2, #1
  34fd88: 15832000     	strne	r2, [r3]
  34fd8c: e5940008     	ldr	r0, [r4, #0x8]
  34fd90: e5843008     	str	r3, [r4, #0x8]
  34fd94: e3500000     	cmp	r0, #0
  34fd98: 0a000000     	beq	0x34fda0 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x188> @ imm = #0x0
  34fd9c: ebfffca5     	bl	0x34f038 <glitch::ISharedObject<glitch::io::CFile>::drop() const> @ imm = #-0xd6c
  34fda0: e59d0008     	ldr	r0, [sp, #0x8]
  34fda4: e3500000     	cmp	r0, #0
  34fda8: 1affffdb     	bne	0x34fd1c <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x104> @ imm = #-0x94
  34fdac: eaffffdb     	b	0x34fd20 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x108> @ imm = #-0x94
  34fdb0: e59f2094     	ldr	r2, [pc, #0x94]         @ 0x34fe4c <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x234>
  34fdb4: e28d0004     	add	r0, sp, #4
  34fdb8: e1a01006     	mov	r1, r6
  34fdbc: e08f2002     	add	r2, pc, r2
  34fdc0: eb08779e     	bl	0x56dc40 <glitch::io::CFileSystem::open(char const*, char const*)> @ imm = #0x21de78
  34fdc4: e59d3004     	ldr	r3, [sp, #0x4]
  34fdc8: e3530000     	cmp	r3, #0
  34fdcc: 15932000     	ldrne	r2, [r3]
  34fdd0: 12822001     	addne	r2, r2, #1
  34fdd4: 15832000     	strne	r2, [r3]
  34fdd8: e5940008     	ldr	r0, [r4, #0x8]
  34fddc: e5843008     	str	r3, [r4, #0x8]
  34fde0: e3500000     	cmp	r0, #0
  34fde4: 0a000000     	beq	0x34fdec <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x1d4> @ imm = #0x0
  34fde8: ebfffc92     	bl	0x34f038 <glitch::ISharedObject<glitch::io::CFile>::drop() const> @ imm = #-0xdb8
  34fdec: e59d0004     	ldr	r0, [sp, #0x4]
  34fdf0: e3500000     	cmp	r0, #0
  34fdf4: 0a000000     	beq	0x34fdfc <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x1e4> @ imm = #0x0
  34fdf8: ebfffc8e     	bl	0x34f038 <glitch::ISharedObject<glitch::io::CFile>::drop() const> @ imm = #-0xdc8
  34fdfc: e3a03001     	mov	r3, #1
  34fe00: e5c4300c     	strb	r3, [r4, #0xc]
  34fe04: e3a03000     	mov	r3, #0
  34fe08: e5c4300d     	strb	r3, [r4, #0xd]
  34fe0c: eaffffc6     	b	0x34fd2c <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0x114> @ imm = #-0xe8
  34fe10: e1a00006     	mov	r0, r6
  34fe14: e3a01000     	mov	r1, #0
  34fe18: e3e02000     	mvn	r2, #0
  34fe1c: ebfff97c     	bl	0x34e414 <ToLowerCase(char*, int, int)> @ imm = #-0x1a10
  34fe20: eaffffa7     	b	0x34fcc4 <FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)+0xac> @ imm = #-0x164
  34fe24: ebfef939     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x41b1c
  34fe28: 68 4e 64 00  	.word	0x00644e68
  34fe2c: ac 40 00 00  	.word	0x000040ac
  34fe30: 00 1c 00 00  	.word	0x00001c00
  34fe34: 00 0b 57 00  	.word	0x00570b00
  34fe38: 00 06 00 00  	.word	0x00000600
  34fe3c: 68 0e 57 00  	.word	0x00570e68
  34fe40: f4 37 00 00  	.word	0x000037f4
  34fe44: b0 0a 57 00  	.word	0x00570ab0
  34fe48: 28 0a 57 00  	.word	0x00570a28
  34fe4c: e4 09 57 00  	.word	0x005709e4
