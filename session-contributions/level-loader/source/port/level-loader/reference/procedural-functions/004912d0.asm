
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004912d0 <rnd::ForceBlock::LoadFromXml(TiXmlNode*)>:
  4912d0: e92d4070     	push	{r4, r5, r6, lr}
  4912d4: e1a05000     	mov	r5, r0
  4912d8: e5913000     	ldr	r3, [r1]
  4912dc: e1a00001     	mov	r0, r1
  4912e0: e1a04001     	mov	r4, r1
  4912e4: e1a0e00f     	mov	lr, pc
  4912e8: e593f02c     	ldr	pc, [r3, #0x2c]
  4912ec: e59f1014     	ldr	r1, [pc, #0x14]         @ 0x491308 <rnd::ForceBlock::LoadFromXml(TiXmlNode*)+0x38>
  4912f0: e08f1001     	add	r1, pc, r1
  4912f4: eb020e5d     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x83974
  4912f8: e1a00005     	mov	r0, r5
  4912fc: e1a01004     	mov	r1, r4
  491300: e8bd4070     	pop	{r4, r5, r6, lr}
  491304: eafffe34     	b	0x490bdc <rnd::Rule::LoadFromXml(TiXmlNode*)> @ imm = #-0x730
  491308: d0 3c 44 00  	.word	0x00443cd0
