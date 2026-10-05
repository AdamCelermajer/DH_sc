
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004914e8 <rnd::Tile::~Tile()>:
  4914e8: e92d4010     	push	{r4, lr}
  4914ec: e1a04000     	mov	r4, r0
  4914f0: e2800034     	add	r0, r0, #52
  4914f4: ebffdb35     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x932c
  4914f8: e1a00004     	mov	r0, r4
  4914fc: e8bd8010     	pop	{r4, pc}
