_ZN14PhysicalObject15onCollisionTestEP18PhysicalBaseObjectsttstt
0046e6bc push     {r4, r5, r6, r7}
0046e6c0 ldr      ip, [r0, #8]
0046e6c4 ldr      r1, [r1, #8]
0046e6c8 ldrh     r5, [sp, #0x10]
0046e6cc cmp      ip, #0
0046e6d0 ldrsh    r4, [sp, #0x14]
0046e6d4 ldrh     r6, [sp, #0x18]
0046e6d8 ldrh     r7, [sp, #0x1c]
0046e6dc beq      #0x46e6f8
0046e6e0 ldrb     r0, [ip, #0x80]
0046e6e4 cmp      r0, #0
0046e6e8 bne      #0x46e6f8
0046e6ec mov      r0, #0
0046e6f0 pop      {r4, r5, r6, r7}
0046e6f4 bx       lr
0046e6f8 cmp      r1, #0
0046e6fc beq      #0x46e70c
0046e700 ldrb     r1, [r1, #0x80]
0046e704 cmp      r1, #0
0046e708 beq      #0x46e6ec
0046e70c cmp      r2, r4
0046e710 movne    r1, #0
0046e714 moveq    r1, #1
0046e718 cmp      r2, #0
0046e71c moveq    r1, #0
0046e720 cmp      r1, #0
0046e724 bne      #0x46e740
0046e728 tst      r6, r5
0046e72c beq      #0x46e6ec
0046e730 tst      r7, r3
0046e734 moveq    r0, #0
0046e738 movne    r0, #1
0046e73c b        #0x46e6f0
0046e740 cmp      r2, #0
0046e744 movle    r0, #0
0046e748 movgt    r0, #1
0046e74c b        #0x46e6f0
_ZN7PODecor17onCollisionBeginsEP18PhysicalBaseObjectRK7Point2DIfEb
00470074 bx       lr
_ZN14PhysicalObject19onCollisionPersistsEP18PhysicalBaseObjectRK7Point2DIfEb
003883c8 bx       lr
_ZN14PhysicalObject15onCollisionEndsEP18PhysicalBaseObjectRK7Point2DIfEb
003883cc bx       lr
_ZN14PhysicalObject18onCollisionResultsEP18PhysicalBaseObjectb
003883d0 bx       lr
