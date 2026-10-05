
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00318e18 <UserProperties::AddProperty(char const*, char const*)>:
  318e18: e92d4030     	push	{r4, r5, lr}
  318e1c: e24dd00c     	sub	sp, sp, #12
  318e20: e28d3008     	add	r3, sp, #8
  318e24: e5231004     	str	r1, [r3, #-0x4]!
  318e28: e1a01003     	mov	r1, r3
  318e2c: e2800004     	add	r0, r0, #4
  318e30: e1a04002     	mov	r4, r2
  318e34: ebffff9e     	bl	0x318cb4 <std::string& std::map<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string>>>::operator[]<char const*>(char const* const&)> @ imm = #-0x188
  318e38: e1a05000     	mov	r5, r0
  318e3c: e1a00004     	mov	r0, r4
  318e40: ebffd403     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0xaff4
  318e44: e1a01004     	mov	r1, r4
  318e48: e0842000     	add	r2, r4, r0
  318e4c: e1a00005     	mov	r0, r5
  318e50: ebffdee2     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x8478
  318e54: e28dd00c     	add	sp, sp, #12
  318e58: e8bd8030     	pop	{r4, r5, pc}
