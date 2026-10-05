
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048c0bc <rnd::ListElem::operator=(rnd::ListElem const&)>:
  48c0bc: e92d4070     	push	{r4, r5, r6, lr}
  48c0c0: e1a03001     	mov	r3, r1
  48c0c4: e4932004     	ldr	r2, [r3], #4
  48c0c8: e1a05000     	mov	r5, r0
  48c0cc: e1a04001     	mov	r4, r1
  48c0d0: e4802004     	str	r2, [r0], #4
  48c0d4: e1500003     	cmp	r0, r3
  48c0d8: 0a000002     	beq	0x48c0e8 <rnd::ListElem::operator=(rnd::ListElem const&)+0x2c> @ imm = #0x8
  48c0dc: e5911018     	ldr	r1, [r1, #0x18]
  48c0e0: e5942014     	ldr	r2, [r4, #0x14]
  48c0e4: ebfa123d     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x17b70c
  48c0e8: e285001c     	add	r0, r5, #28
  48c0ec: e284301c     	add	r3, r4, #28
  48c0f0: e1500003     	cmp	r0, r3
  48c0f4: 0a000002     	beq	0x48c104 <rnd::ListElem::operator=(rnd::ListElem const&)+0x48> @ imm = #0x8
  48c0f8: e5941030     	ldr	r1, [r4, #0x30]
  48c0fc: e594202c     	ldr	r2, [r4, #0x2c]
  48c100: ebfa1236     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x17b728
  48c104: e2850034     	add	r0, r5, #52
  48c108: e2843034     	add	r3, r4, #52
  48c10c: e1500003     	cmp	r0, r3
  48c110: 0a000002     	beq	0x48c120 <rnd::ListElem::operator=(rnd::ListElem const&)+0x64> @ imm = #0x8
  48c114: e5941048     	ldr	r1, [r4, #0x48]
  48c118: e5942044     	ldr	r2, [r4, #0x44]
  48c11c: ebfa122f     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x17b744
  48c120: e594304c     	ldr	r3, [r4, #0x4c]
  48c124: e1a00005     	mov	r0, r5
  48c128: e585304c     	str	r3, [r5, #0x4c]
  48c12c: e8bd8070     	pop	{r4, r5, r6, pc}
