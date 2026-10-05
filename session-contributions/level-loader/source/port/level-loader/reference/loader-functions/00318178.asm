
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00318178 <UserProperties::~UserProperties()>:
  318178: e92d4070     	push	{r4, r5, r6, lr}
  31817c: e59f304c     	ldr	r3, [pc, #0x4c]         @ 0x3181d0 <UserProperties::~UserProperties()+0x58>
  318180: e59f204c     	ldr	r2, [pc, #0x4c]         @ 0x3181d4 <UserProperties::~UserProperties()+0x5c>
  318184: e5901014     	ldr	r1, [r0, #0x14]
  318188: e08f3003     	add	r3, pc, r3
  31818c: e7932002     	ldr	r2, [r3, r2]
  318190: e3510000     	cmp	r1, #0
  318194: e1a04000     	mov	r4, r0
  318198: e2822008     	add	r2, r2, #8
  31819c: e5802000     	str	r2, [r0]
  3181a0: 0a000008     	beq	0x3181c8 <UserProperties::~UserProperties()+0x50> @ imm = #0x20
  3181a4: e2805004     	add	r5, r0, #4
  3181a8: e1a00005     	mov	r0, r5
  3181ac: e5941008     	ldr	r1, [r4, #0x8]
  3181b0: ebffffc3     	bl	0x3180c4 <std::priv::_Rb_tree<std::string, std::less<std::string>, std::pair<std::string const, std::string>, std::priv::_Select1st<std::pair<std::string const, std::string>>, std::priv::_MapTraitsT<std::pair<std::string const, std::string>>, std::allocator<std::pair<std::string const, std::string>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0xf4
  3181b4: e3a03000     	mov	r3, #0
  3181b8: e5845010     	str	r5, [r4, #0x10]
  3181bc: e5843014     	str	r3, [r4, #0x14]
  3181c0: e584500c     	str	r5, [r4, #0xc]
  3181c4: e5843008     	str	r3, [r4, #0x8]
  3181c8: e1a00004     	mov	r0, r4
  3181cc: e8bd8070     	pop	{r4, r5, r6, pc}
  3181d0: 08 c9 67 00  	.word	0x0067c908
  3181d4: 10 28 00 00  	.word	0x00002810
