
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00459034 <Script_StartDialog::Init()>:
  459034: e59f3044     	ldr	r3, [pc, #0x44]         @ 0x459080 <Script_StartDialog::Init()+0x4c>
  459038: e59f2044     	ldr	r2, [pc, #0x44]         @ 0x459084 <Script_StartDialog::Init()+0x50>
  45903c: e92d4070     	push	{r4, r5, r6, lr}
  459040: e08f3003     	add	r3, pc, r3
  459044: e7932002     	ldr	r2, [r3, r2]
  459048: e590c00c     	ldr	r12, [r0, #0xc]
  45904c: e1a04000     	mov	r4, r0
  459050: e59f1030     	ldr	r1, [pc, #0x30]         @ 0x459088 <Script_StartDialog::Init()+0x54>
  459054: e592002c     	ldr	r0, [r2, #0x2c]
  459058: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x45908c <Script_StartDialog::Init()+0x58>
  45905c: e08f1001     	add	r1, pc, r1
  459060: e59c500c     	ldr	r5, [r12, #0xc]
  459064: e08f2002     	add	r2, pc, r2
  459068: eb01aedb     	bl	0x4c4bdc <PyDataConstants::getConstant(char const*, char const*) const> @ imm = #0x6bb6c
  45906c: e1550000     	cmp	r5, r0
  459070: 13a00000     	movne	r0, #0
  459074: 03a00001     	moveq	r0, #1
  459078: e5c40004     	strb	r0, [r4, #0x4]
  45907c: e8bd8070     	pop	{r4, r5, r6, pc}
  459080: 50 ba 53 00  	.word	0x0053ba50
  459084: f4 37 00 00  	.word	0x000037f4
  459088: cc 3b 47 00  	.word	0x00473bcc
  45908c: 04 3e 47 00  	.word	0x00473e04
