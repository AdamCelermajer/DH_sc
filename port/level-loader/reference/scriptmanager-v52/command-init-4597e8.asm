
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004597e8 <Script_HideFlash::Init()>:
  4597e8: e92d4070     	push	{r4, r5, r6, lr}
  4597ec: e1a04000     	mov	r4, r0
  4597f0: ebff4ca5     	bl	0x42ca8c <MenuManager::GetInstance()> @ imm = #-0x2cd6c
  4597f4: e594300c     	ldr	r3, [r4, #0xc]
  4597f8: e59f1038     	ldr	r1, [pc, #0x38]         @ 0x459838 <Script_HideFlash::Init()+0x50>
  4597fc: e1a06000     	mov	r6, r0
  459800: e5935010     	ldr	r5, [r3, #0x10]
  459804: e08f1001     	add	r1, pc, r1
  459808: e1a00005     	mov	r0, r5
  45980c: ebfad4f0     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x14ac40
  459810: e3500000     	cmp	r0, #0
  459814: 0a000002     	beq	0x459824 <Script_HideFlash::Init()+0x3c> @ imm = #0x8
  459818: e3a03000     	mov	r3, #0
  45981c: e5843010     	str	r3, [r4, #0x10]
  459820: e8bd8070     	pop	{r4, r5, r6, pc}
  459824: e1a00006     	mov	r0, r6
  459828: e1a01005     	mov	r1, r5
  45982c: ebff4e6f     	bl	0x42d1f0 <MenuManager::GetMenuByName(char const*)> @ imm = #-0x2c644
  459830: e5840010     	str	r0, [r4, #0x10]
  459834: e8bd8070     	pop	{r4, r5, r6, pc}
  459838: 04 37 47 00  	.word	0x00473704
