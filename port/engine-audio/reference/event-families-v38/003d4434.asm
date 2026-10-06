# _ZN6CharAI12_OnAnimEventEPKc
003d4434 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003d4438 ldr      r4, [pc, #0x528]
003d443c ldr      r7, [pc, #0x528]
003d4440 mov      r6, r0
003d4444 add      r4, pc, r4
003d4448 ldr      r3, [r4, r7]
003d444c ldr      r0, [r0, #4]
003d4450 sub      sp, sp, #0xd4
003d4454 ldr      r3, [r3]
003d4458 add      r0, r0, #0x490
003d445c add      r0, r0, #0xc
003d4460 mov      r5, r1
003d4464 str      r3, [sp, #0xcc]
003d4468 bl       #0x3c932c
003d446c mov      r8, r0
003d4470 ldr      r0, [r6, #4]
003d4474 add      r0, r0, #0x490
003d4478 add      r0, r0, #0xc
003d447c bl       #0x3c934c
003d4480 ldr      r1, [pc, #0x4e8]
003d4484 mov      r0, r5
003d4488 mov      r2, #3
003d448c add      r1, pc, r1
003d4490 bl       #0x30ec7c
003d4494 cmp      r0, #0
003d4498 beq      #0x3d4634
003d449c ldr      r1, [pc, #0x4d0]
003d44a0 mov      r0, r5
003d44a4 mov      r2, #3
003d44a8 add      r1, pc, r1
003d44ac bl       #0x30ec7c
003d44b0 cmp      r0, #0
003d44b4 beq      #0x3d464c
003d44b8 ldr      r1, [pc, #0x4b8]
003d44bc mov      r0, r5
003d44c0 mov      r2, #3
003d44c4 add      r1, pc, r1
003d44c8 bl       #0x30ec7c
003d44cc subs     sl, r0, #0
003d44d0 beq      #0x3d45bc
003d44d4 ldr      r1, [pc, #0x4a0]
003d44d8 mov      r0, r5
003d44dc mov      r2, #4
003d44e0 add      r1, pc, r1
003d44e4 bl       #0x30ec7c
003d44e8 cmp      r0, #0
003d44ec bne      #0x3d465c
003d44f0 ldr      r3, [pc, #0x488]
003d44f4 add      r5, r5, #4
003d44f8 ldr      r3, [r4, r3]
003d44fc ldr      sb, [r3]
003d4500 cmp      sb, #0
003d4504 beq      #0x3d459c
003d4508 ldr      r3, [pc, #0x474]
003d450c mov      r8, r0
003d4510 ldr      r3, [r4, r3]
003d4514 ldr      fp, [r3]
003d4518 b        #0x3d4528
003d451c add      r8, r8, #1
003d4520 cmp      r8, sb
003d4524 beq      #0x3d459c
003d4528 mov      r0, r5
003d452c ldr      r1, [fp, r8, lsl #2]
003d4530 bl       #0x30e31c
003d4534 subs     sl, r0, #0
003d4538 bne      #0x3d451c
003d453c cmn      r8, #1
003d4540 beq      #0x3d459c
003d4544 ldr      r3, [pc, #0x43c]
003d4548 ldr      r0, [r6, #4]
003d454c ldr      r3, [r4, r3]
003d4550 ldr      sb, [r3]
003d4554 bl       #0x3935dc
003d4558 ldr      lr, [r0, #4]
003d455c ldr      r5, [r0]
003d4560 ldr      r6, [r0, #8]
003d4564 mov      ip, #0xbf000000
003d4568 add      ip, ip, #0x800000
003d456c str      lr, [sp, #0x14]
003d4570 mov      r0, sb
003d4574 mov      lr, #1
003d4578 mov      r1, r8
003d457c mov      r3, sl
003d4580 add      r2, sp, #0x10
003d4584 str      r5, [sp, #0x10]
003d4588 str      r6, [sp, #0x18]
003d458c str      lr, [sp]
003d4590 str      ip, [sp, #8]
003d4594 str      ip, [sp, #4]
003d4598 bl       #0x36b5d8
003d459c ldr      r3, [r4, r7]
003d45a0 ldr      r2, [sp, #0xcc]
003d45a4 mov      r0, #1
003d45a8 ldr      r3, [r3]
003d45ac cmp      r2, r3
003d45b0 bne      #0x3d4964
003d45b4 add      sp, sp, #0xd4
003d45b8 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003d45bc ldr      r3, [pc, #0x3c8]
003d45c0 add      r5, r5, #3
003d45c4 ldr      r3, [r4, r3]
003d45c8 ldr      sb, [r3]
003d45cc cmp      sb, #0
003d45d0 beq      #0x3d459c
003d45d4 ldr      r3, [pc, #0x3b4]
003d45d8 ldr      r3, [r4, r3]
003d45dc ldr      fp, [r3]
003d45e0 b        #0x3d45f0
003d45e4 add      sl, sl, #1
003d45e8 cmp      sl, sb
003d45ec beq      #0x3d459c
003d45f0 mov      r0, r5
003d45f4 ldr      r1, [fp, sl, lsl #2]
003d45f8 bl       #0x30e31c
003d45fc subs     r8, r0, #0
003d4600 bne      #0x3d45e4
003d4604 cmn      sl, #1
003d4608 beq      #0x3d459c
003d460c ldr      r0, [r6, #4]
003d4610 bl       #0x3935dc
003d4614 ldr      r3, [pc, #0x378]
003d4618 mov      r2, r0
003d461c mov      r1, sl
003d4620 ldr      r0, [r4, r3]
003d4624 mov      r3, r8
003d4628 str      r8, [sp]
003d462c bl       #0x495d14
003d4630 b        #0x3d459c
003d4634 mov      r0, r6
003d4638 add      r1, r5, #3
003d463c ldr      r3, [r6]
003d4640 mov      lr, pc
003d4644 ldr      pc, [r3, #0x94]
003d4648 b        #0x3d459c
003d464c mov      r0, r6
003d4650 add      r1, r5, #3
003d4654 bl       #0x3d3af4
003d4658 b        #0x3d459c
003d465c ldr      r0, [r6, #4]
003d4660 add      r0, r0, #0x4f0
003d4664 add      r0, r0, #0xc
003d4668 bl       #0x3c01ac
003d466c sub      r0, r0, #5
003d4670 cmp      r0, #8
003d4674 addls    pc, pc, r0, lsl #2
003d4678 b        #0x3d459c
003d467c b        #0x3d4750
003d4680 b        #0x3d46f8
003d4684 b        #0x3d46a0
003d4688 b        #0x3d459c
003d468c b        #0x3d459c
003d4690 b        #0x3d459c
003d4694 b        #0x3d459c
003d4698 b        #0x3d459c
003d469c b        #0x3d482c
003d46a0 ldr      r1, [pc, #0x2f0]
003d46a4 mov      r0, r5
003d46a8 add      r1, pc, r1
003d46ac bl       #0x30e31c
003d46b0 cmp      r0, #0
003d46b4 bne      #0x3d459c
003d46b8 ldr      r3, [pc, #0x2dc]
003d46bc add      r5, sp, #0x3c
003d46c0 ldr      r8, [r4, r3]
003d46c4 mov      r0, r8
003d46c8 bl       #0x337888
003d46cc add      r1, sp, #0x24
003d46d0 mov      r0, r5
003d46d4 bl       #0x3d43ec
003d46d8 mov      r1, r5
003d46dc mov      r0, r8
003d46e0 bl       #0x337a88
003d46e4 mov      r0, r5
003d46e8 bl       #0x3139ac
003d46ec mov      r0, r6
003d46f0 bl       #0x3d8ba4
003d46f4 b        #0x3d459c
003d46f8 ldr      r1, [pc, #0x2a0]
003d46fc mov      r0, r5
003d4700 add      r1, pc, r1
003d4704 bl       #0x30e31c
003d4708 cmp      r0, #0
003d470c bne      #0x3d459c
003d4710 ldr      r3, [pc, #0x284]
003d4714 add      r5, sp, #0x54
003d4718 ldr      r8, [r4, r3]
003d471c mov      r0, r8
003d4720 bl       #0x337888
003d4724 add      r1, sp, #0x28
003d4728 mov      r0, r5
003d472c bl       #0x3d43ec
003d4730 mov      r1, r5
003d4734 mov      r0, r8
003d4738 bl       #0x337a88
003d473c mov      r0, r5
003d4740 bl       #0x3139ac
003d4744 mov      r0, r6
003d4748 bl       #0x3d8bf8
003d474c b        #0x3d459c
003d4750 ldr      r3, [r6, #4]
003d4754 add      r1, sp, #0x20
003d4758 mov      r2, r1
003d475c mov      r0, r3
003d4760 ldr      ip, [r3]
003d4764 add      r3, sp, #0x1c
003d4768 mov      lr, pc
003d476c ldr      pc, [ip, #0x128]
003d4770 cmp      r0, #0
003d4774 beq      #0x3d488c
003d4778 ldr      r1, [pc, #0x224]
003d477c mov      r0, r5
003d4780 add      r1, pc, r1
003d4784 bl       #0x30e31c
003d4788 cmp      r0, #0
003d478c beq      #0x3d47c0
003d4790 ldr      r1, [pc, #0x210]
003d4794 mov      r0, r5
003d4798 add      r1, pc, r1
003d479c bl       #0x30e31c
003d47a0 cmp      r0, #0
003d47a4 beq      #0x3d47c0
003d47a8 ldr      r1, [pc, #0x1fc]
003d47ac mov      r0, r5
003d47b0 add      r1, pc, r1
003d47b4 bl       #0x30e31c
003d47b8 cmp      r0, #0
003d47bc bne      #0x3d459c
003d47c0 ldr      r3, [pc, #0x1d4]
003d47c4 add      r5, sp, #0xb4
003d47c8 ldr      r8, [r4, r3]
003d47cc mov      r0, r8
003d47d0 bl       #0x337888
003d47d4 add      r1, sp, #0x38
003d47d8 mov      r0, r5
003d47dc bl       #0x3d43ec
003d47e0 mov      r1, r5
003d47e4 mov      r0, r8
003d47e8 bl       #0x337a88
003d47ec mov      r0, r5
003d47f0 bl       #0x3139ac
003d47f4 ldr      r3, [pc, #0x1b4]
003d47f8 mov      ip, #0
003d47fc ldr      r2, [r6, #4]
003d4800 ldr      r0, [r4, r3]
003d4804 ldr      r3, [pc, #0x1a8]
003d4808 ldr      r1, [sp, #0x1c]
003d480c str      ip, [sp]
003d4810 ldr      lr, [r4, r3]
003d4814 mov      r3, ip
003d4818 str      ip, [sp, #8]
003d481c str      lr, [sp, #4]
003d4820 str      ip, [sp, #0xc]
003d4824 bl       #0x3e701c
003d4828 b        #0x3d459c
003d482c ldr      r1, [pc, #0x184]
003d4830 mov      r0, r5
003d4834 add      r1, pc, r1
003d4838 bl       #0x30e31c
003d483c cmp      r0, #0
003d4840 bne      #0x3d459c
003d4844 ldr      r3, [pc, #0x150]
003d4848 add      r5, sp, #0x6c
003d484c ldr      r8, [r4, r3]
003d4850 mov      r0, r8
003d4854 bl       #0x337888
003d4858 add      r1, sp, #0x2c
003d485c mov      r0, r5
003d4860 bl       #0x3d43ec
003d4864 mov      r1, r5
003d4868 mov      r0, r8
003d486c bl       #0x337a88
003d4870 mov      r0, r5
003d4874 bl       #0x3139ac
003d4878 mov      r0, r6
003d487c ldr      r3, [r6]
003d4880 mov      lr, pc
003d4884 ldr      pc, [r3, #0xa0]
003d4888 b        #0x3d459c
003d488c ldr      r1, [pc, #0x128]
003d4890 mov      r0, r5
003d4894 add      r1, pc, r1
003d4898 bl       #0x30e31c
003d489c subs     sl, r0, #0
003d48a0 beq      #0x3d4910
003d48a4 ldr      r1, [pc, #0x114]
003d48a8 mov      r0, r5
003d48ac add      r1, pc, r1
003d48b0 bl       #0x30e31c
003d48b4 cmp      r0, #0
003d48b8 bne      #0x3d459c
003d48bc ldr      r3, [pc, #0xd8]
003d48c0 add      r5, sp, #0x84
003d48c4 ldr      sl, [r4, r3]
003d48c8 mov      r0, sl
003d48cc bl       #0x337888
003d48d0 add      r1, sp, #0x30
003d48d4 mov      r0, r5
003d48d8 bl       #0x3d43ec
003d48dc mov      r1, r5
003d48e0 mov      r0, sl
003d48e4 bl       #0x337a88
003d48e8 mov      r0, r5
003d48ec bl       #0x3139ac
003d48f0 mov      r0, r6
003d48f4 sub      r2, r8, #1
003d48f8 ldr      ip, [r6]
003d48fc ldr      r1, [r6, #0x74]
003d4900 mov      r3, #1
003d4904 mov      lr, pc
003d4908 ldr      pc, [ip, #0xa8]
003d490c b        #0x3d459c
003d4910 ldr      r3, [pc, #0x84]
003d4914 add      r5, sp, #0x9c
003d4918 ldr      sb, [r4, r3]
003d491c mov      r0, sb
003d4920 bl       #0x337888
003d4924 add      r1, sp, #0x34
003d4928 mov      r0, r5
003d492c bl       #0x3d43ec
003d4930 mov      r1, r5
003d4934 mov      r0, sb
003d4938 bl       #0x337a88
003d493c mov      r0, r5
003d4940 bl       #0x3139ac
003d4944 mov      r0, r6
003d4948 sub      r2, r8, #1
003d494c mov      r3, sl
003d4950 ldr      ip, [r6]
003d4954 ldr      r1, [r6, #0x74]
003d4958 mov      lr, pc
003d495c ldr      pc, [ip, #0xa8]
003d4960 b        #0x3d459c
003d4964 bl       #0x30e310
003d4968 subseq   r0, ip, ip, asr #12
003d496c andeq    r4, r0, ip, lsr #1
003d4970 strheq   r1, [pc], #-4
003d4974 subeq    r1, pc, r0, lsr #1
003d4978 subeq    r1, pc, ip, lsl #1
003d497c subeq    r1, pc, r8, ror r0
003d4980 andeq    r3, r0, r8, lsr sp
003d4984 andeq    r3, r0, r8, lsr #19
003d4988 andeq    r0, r0, r4, lsr #27
003d498c andeq    r0, r0, r4, asr #13
003d4990 muleq    r0, r4, r2
003d4994 andeq    r1, r0, r8, lsl #22
003d4998 subeq    r0, pc, r8, lsl #30
003d499c andeq    r0, r0, r4, lsl #17
003d49a0 subeq    r0, pc, r0, lsl #29
003d49a4 subeq    r0, pc, r0, ror #27
003d49a8 ldrdeq   r0, r1, [pc], #-0xd8
003d49ac ldrdeq   r0, r1, [pc], #-0xd0
003d49b0 andeq    r0, r0, ip, asr #16
003d49b4 andeq    r1, r0, r4, lsr r8
003d49b8 subeq    r0, pc, ip, ror #26
003d49bc ldrdeq   r0, r1, [pc], #-0xcc
003d49c0 subeq    r0, pc, r4, ror #25
