
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00491500 <rnd::Tile::~Tile()>:
  491500: e92d4010     	push	{r4, lr}
  491504: e1a04000     	mov	r4, r0
  491508: e2800034     	add	r0, r0, #52
  49150c: ebffdb2f     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x9344
  491510: e1a00004     	mov	r0, r4
  491514: e8bd8010     	pop	{r4, pc}
