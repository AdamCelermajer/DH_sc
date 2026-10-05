
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0061c6f4 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, char const*) const>:
  61c6f4: e92d4070     	push	{r4, r5, r6, lr}
  61c6f8: e1a04001     	mov	r4, r1
  61c6fc: e1a01002     	mov	r1, r2
  61c700: e1a05000     	mov	r5, r0
  61c704: ebfffee1     	bl	0x61c290 <glitch::collada::CColladaDatabase::getNode(char const*) const> @ imm = #-0x47c
  61c708: e1a01004     	mov	r1, r4
  61c70c: e1a02000     	mov	r2, r0
  61c710: e1a00005     	mov	r0, r5
  61c714: e8bd4070     	pop	{r4, r5, r6, lr}
  61c718: eafffdbd     	b	0x61be14 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*) const> @ imm = #-0x90c
