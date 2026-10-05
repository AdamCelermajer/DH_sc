
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003181f4 <UserProperties::~UserProperties()>:
  3181f4: e92d4070     	push	{r4, r5, r6, lr}
  3181f8: e59f304c     	ldr	r3, [pc, #0x4c]         @ 0x31824c <UserProperties::~UserProperties()+0x58>
  3181fc: e59f204c     	ldr	r2, [pc, #0x4c]         @ 0x318250 <UserProperties::~UserProperties()+0x5c>
  318200: e5901014     	ldr	r1, [r0, #0x14]
  318204: e08f3003     	add	r3, pc, r3
  318208: e7932002     	ldr	r2, [r3, r2]
  31820c: e3510000     	cmp	r1, #0
  318210: e1a04000     	mov	r4, r0
  318214: e2822008     	add	r2, r2, #8
  318218: e5802000     	str	r2, [r0]
  31821c: 0a000008     	beq	0x318244 <UserProperties::~UserProperties()+0x50> @ imm = #0x20
  318220: e2805004     	add	r5, r0, #4
  318224: e1a00005     	mov	r0, r5
  318228: e5941008     	ldr	r1, [r4, #0x8]
  31822c: ebffffa4     	bl	0x3180c4 <std::priv::_Rb_tree<std::string, std::less<std::string>, std::pair<std::string const, std::string>, std::priv::_Select1st<std::pair<std::string const, std::string>>, std::priv::_MapTraitsT<std::pair<std::string const, std::string>>, std::allocator<std::pair<std::string const, std::string>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x170
  318230: e3a03000     	mov	r3, #0
  318234: e5845010     	str	r5, [r4, #0x10]
  318238: e5843014     	str	r3, [r4, #0x14]
  31823c: e584500c     	str	r5, [r4, #0xc]
  318240: e5843008     	str	r3, [r4, #0x8]
  318244: e1a00004     	mov	r0, r4
  318248: e8bd8070     	pop	{r4, r5, r6, pc}
  31824c: 8c c8 67 00  	.word	0x0067c88c
  318250: 10 28 00 00  	.word	0x00002810
