
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034e090 <ToLowerCase(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>&, int, int)>:
  34e090: e5903014     	ldr	r3, [r0, #0x14]
  34e094: e590c010     	ldr	r12, [r0, #0x10]
  34e098: e3720001     	cmn	r2, #1
  34e09c: e52d4004     	str	r4, [sp, #-0x4]!
  34e0a0: e063c00c     	rsb	r12, r3, r12
  34e0a4: 0a000012     	beq	0x34e0f4 <ToLowerCase(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>&, int, int)+0x64> @ imm = #0x48
  34e0a8: e152000c     	cmp	r2, r12
  34e0ac: aa000010     	bge	0x34e0f4 <ToLowerCase(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>&, int, int)+0x64> @ imm = #0x40
  34e0b0: e1510002     	cmp	r1, r2
  34e0b4: da000001     	ble	0x34e0c0 <ToLowerCase(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>&, int, int)+0x30> @ imm = #0x4
  34e0b8: ea00000b     	b	0x34e0ec <ToLowerCase(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>&, int, int)+0x5c> @ imm = #0x2c
  34e0bc: e5903014     	ldr	r3, [r0, #0x14]
  34e0c0: e7d34001     	ldrb	r4, [r3, r1]
  34e0c4: e0833001     	add	r3, r3, r1
  34e0c8: e2811001     	add	r1, r1, #1
  34e0cc: e6afc074     	sxtb	r12, r4
  34e0d0: e35c0040     	cmp	r12, #64
  34e0d4: da000002     	ble	0x34e0e4 <ToLowerCase(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>&, int, int)+0x54> @ imm = #0x8
  34e0d8: e35c005a     	cmp	r12, #90
  34e0dc: d2844020     	addle	r4, r4, #32
  34e0e0: d5c34000     	strble	r4, [r3]
  34e0e4: e1510002     	cmp	r1, r2
  34e0e8: dafffff3     	ble	0x34e0bc <ToLowerCase(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>&, int, int)+0x2c> @ imm = #-0x34
  34e0ec: e8bd0010     	ldm	sp!, {r4}
  34e0f0: e12fff1e     	bx	lr
  34e0f4: e24c2001     	sub	r2, r12, #1
  34e0f8: eaffffec     	b	0x34e0b0 <ToLowerCase(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>&, int, int)+0x20> @ imm = #-0x50
